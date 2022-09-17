#!/usr/bin/python3
import rospy
import numpy as np
# import pandas as pd
from std_msgs.msg import Float64MultiArray
# import matplotlib.pyplot as plt

import time
import RPi.GPIO as GPIO
import VL53L0X
import yaml

class ranger:
    def __init__(self, address, shutdown_pin, sensing_mode):
        self.address = address
        self.shutdown_pin = shutdown_pin
        self.true_data_list = []
        self.measure_data_list = []
        self.calibration_m = 0
        self.calibration_c = 0

        GPIO.setwarnings(False)

        # Setup GPIO for shutdown pins on each VL53L0X
        # Set shutdown pins low to turn off VL53L0X
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.shutdown_pin, GPIO.OUT)
        GPIO.output(self.shutdown_pin, GPIO.LOW)

        # Keep all low for 500 ms or so to make sure they reset
        time.sleep(0.5)

        self.sensor = VL53L0X.VL53L0X(address = self.address)

        # Set shutdown pin high for the first VL53L0X then 
        # call to start ranging 
        GPIO.output(self.shutdown_pin, GPIO.HIGH)
        time.sleep(0.5)
        self.sensor.start_ranging(sensing_mode)

    def measureDistance(self):
        distance = self.sensor.get_distance()
        if (distance > 0):
            # print ("sensor %d - %d mm, %d cm" % (self.sensor.my_object_number, distance, (distance/10)))
            pass
        else:
            # print ("%d - Error" % tof.my_object_number)
            pass
        return distance/1000.0

    def stopRanging(self):
        self.sensor.stop_ranging()
        GPIO.output(self.shutdown_pin, GPIO.LOW)

    def curveFitting(self, plot_on):
        x = np.array(self.true_data_list)
        y = np.array(self.measure_data_list)
        print(x)
        print(y)
        # y = Ap
        A = np.vstack([x, np.ones(len(x))]).T
        self.calibration_m, self.calibration_c = np.linalg.lstsq(A, y, rcond=None)[0]
        print("m, c = ", self.calibration_m, self.calibration_c)
        # if plot_on:
        #     _ = plt.plot(x, y, 'o', label='Original data', markersize=10)
        #     _ = plt.plot(x, self.calibration_m*x + self.calibration_c, 'r', label='Fitted line')
        #     _ = plt.legend()
        #     plt.show()

    def saveCSV(self):
        data_ = {"true": self.true_data_list,
                 "measurement": self.measure_data_list}
        data_df = pd.DataFrame(data_)
        data_df.to_csv("../csv/raw_data.csv", index=False)
        pass
        
class tof_calibration:
    def __init__(self):
        self.ranging_mode = VL53L0X.VL53L0X_BETTER_ACCURACY_MODE
        # VL53L0X_GOOD_ACCURACY_MODE       Good Accuracy mode
        # VL53L0X_BETTER_ACCURACY_MODE     Better Accuracy mode
        # VL53L0X_BEST_ACCURACY_MODE       Best Accuracy mode
        # VL53L0X_LONG_RANGE_MODE          Longe Range mode
        # VL53L0X_HIGH_SPEED_MODE          High Speed mode
        self.ranging_data = []
        self.ranging_duration_in_ms = 66

        # GPIO for each sensor shutdwon pin
        # self.sensor_AL = ranger(0x1A, 5, self.ranging_mode)
        # self.sensor_AR = ranger(0x1B, 6, self.ranging_mode)
        # self.sensor_BL = ranger(0x1C, 7, self.ranging_mode)
        # self.sensor_BR = ranger(0x1D, 8, self.ranging_mode)
        # self.sensor_CL = ranger(0x1E, 9, self.ranging_mode)
        # self.sensor_CR = ranger(0x1F, 10, self.ranging_mode)

        self.sensor_BL = ranger(0x1A, 17, self.ranging_mode)
        self.sensor_BR = ranger(0x1B, 9, self.ranging_mode)
        self.sensor_GL = ranger(0x1C, 27, self.ranging_mode)
        self.sensor_GR = ranger(0x1D, 10, self.ranging_mode)
        self.sensor_RL = ranger(0x1E, 11, self.ranging_mode)
        self.sensor_RR = ranger(0x1F, 22, self.ranging_mode)
        self.ranger_list = [self.sensor_BL, self.sensor_BR, 
                            self.sensor_GL, self.sensor_GR,
                            self.sensor_RL, self.sensor_RR]
        # self.ranger_list = [self.sensor_AL, self.sensor_AR]
        self.rangerPub = rospy.Publisher("tof_data", Float64MultiArray, queue_size=10)
        self.rangerCalibration()

    def rangerCalibration(self):
        print("VL53L0X sensor calibration initialized")
        sensor_idx = "B"
        sample_number = input("Enter sample number : ")
        sample_number = int(sample_number)

        while True:
            sensor_idx = input("Enter index of the sensor (B, G, R, q for quit, wq for save and quit) : ")
            if sensor_idx == "q":
                print("QUIT VL53L0X CALIBRATION")
                break

            if sensor_idx == "wq":
                yaml_dir = input("Enter yaml file directory : ")
                data = dict(
                    BL_M = float(self.sensor_BL.calibration_m),
                    BL_C = float(self.sensor_BL.calibration_c),
                    BR_M = float(self.sensor_BR.calibration_m),
                    BR_C = float(self.sensor_BR.calibration_c),

                    GL_M = float(self.sensor_GL.calibration_m),
                    GL_C = float(self.sensor_GL.calibration_c),
                    GR_M = float(self.sensor_GR.calibration_m),
                    GR_C = float(self.sensor_GR.calibration_c),

                    RL_M = float(self.sensor_RL.calibration_m),
                    RL_C = float(self.sensor_RL.calibration_c),
                    RR_M = float(self.sensor_RR.calibration_m),
                    RR_C = float(self.sensor_RR.calibration_c),
                )
                with open(yaml_dir, 'w') as outfile:
                    yaml.dump(data, outfile, default_flow_style=False)
                print("QUIT VL53L0X CALIBRATION")
                # self.sensor_AL.saveCSV()
                # self.sensor_AR.saveCSV()
                break

            if(sensor_idx == "B"):
                calib_sensor_list = [self.sensor_BL, self.sensor_BR]
            if(sensor_idx == "G"):
                calib_sensor_list = [self.sensor_GL, self.sensor_GR]
            if(sensor_idx == "R"):
                calib_sensor_list = [self.sensor_RL, self.sensor_RR]

            while True and sensor_idx != "q":
                true_data = input("Enter True Distance (in meter) (q to quit) : ")
                if true_data == "q":
                    print("QUIT", sensor_idx, "FACE SENSOR CALIBRATION")
                    break

                l_true_data = []
                l_measure_data = []
                r_true_data = []
                r_measure_data = []
                for i in range(sample_number):
                    print("true distance = ", true_data, "left measurement = ", calib_sensor_list[0].measureDistance(), "right measurement = ", calib_sensor_list[1].measureDistance())
                    # left
                    l_true_data.append(float(true_data))
                    l_measure_data.append(calib_sensor_list[0].measureDistance())
                    # calib_sensor_list[0].true_data_list.append(float(true_data))
                    # calib_sensor_list[0].measure_data_list.append(calib_sensor_list[0].measureDistance())
                    # right
                    r_true_data.append(float(true_data))
                    r_measure_data.append(calib_sensor_list[1].measureDistance())
                    # calib_sensor_list[1].true_data_list.append(float(true_data))
                    # calib_sensor_list[1].measure_data_list.append(calib_sensor_list[1].measureDistance())
                # filter outliar
                l_mean = np.mean(np.array(l_measure_data))
                l_stdev = np.std(np.array(l_measure_data), ddof=1)
                r_mean = np.mean(np.array(r_measure_data))
                r_stdev = np.std(np.array(r_measure_data), ddof=1)
                for i in range(len(l_measure_data)):
                    if l_measure_data[i] < l_mean + 1 * l_stdev and l_measure_data[i] > l_mean - 1 * l_stdev:
                        calib_sensor_list[0].true_data_list.append(float(true_data))
                        calib_sensor_list[0].measure_data_list.append(l_measure_data[i])
                    if r_measure_data[i] < r_mean + 1 * r_stdev and r_measure_data[i] > r_mean - 1 * r_stdev:
                        calib_sensor_list[1].true_data_list.append(float(true_data))
                        calib_sensor_list[1].measure_data_list.append(r_measure_data[i])
                print("finish sampling")
            
            calib_sensor_list[0].curveFitting(0)
            calib_sensor_list[1].curveFitting(0)
            print(sensor_idx, "FACE SENSOR calibration finished")


if __name__ == "__main__":
    rospy.init_node('tof_calibration', anonymous = True)
    TOF = tof_calibration()
    rospy.spin()
