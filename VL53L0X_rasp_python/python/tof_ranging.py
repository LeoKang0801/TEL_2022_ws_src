#!/usr/bin/python3

import rospy
from std_msgs.msg import Float64MultiArray

import time
import RPi.GPIO as GPIO
import VL53L0X

class ranger:
    def __init__(self, idx, address, shutdown_pin, sensing_mode):
        self.idx = idx
        self.address = address
        self.shutdown_pin = shutdown_pin
        self.sensing_mode = sensing_mode
        # self.initialize_validate_count = 5
        self.calib_m = 1.05
        self.calib_c = 0.03
        self.calib_m = rospy.get_param('vl53_tof_ranger/' + idx + "_M")
        self.calib_c = rospy.get_param('vl53_tof_ranger/' + idx + "_C")

        self.initHardware()
        restart_count = 0
        while self.measureDistance() == -0.001 and not rospy.is_shutdown():
            if restart_count == 5:
                print("Error still occurs after restarting ", restart_count, "times")
                break
            print("sensor initialize error ", [self.address], ", restarting")
            self.initHardware()
            restart_count +=1
            time.sleep(0.5)

    def initHardware(self):
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
        self.sensor.start_ranging(self.sensing_mode)
        pass

    def measureDistance(self):
        distance = self.sensor.get_distance()/1000
        if (distance > 0):
            # print ("sensor %d - %d mm, %d cm" % (self.sensor.my_object_number, distance, (distance/10)))
            distance = (distance - self.calib_c)/self.calib_m
            pass
        else:
            # print ("%d - Error" % tof.my_object_number)
            pass
        if (distance > 1 or distance < 0):
            distance = -1
        return distance

    def stopRanging(self):
        self.sensor.stop_ranging()
        GPIO.output(self.shutdown_pin, GPIO.LOW)
        
class tof:
    def __init__(self):
        self.ranging_mode = VL53L0X.VL53L0X_BETTER_ACCURACY_MODE
        # self.ranging_mode = VL53L0X.VL53L0X_GOOD_ACCURACY_MODE
        # VL53L0X_GOOD_ACCURACY_MODE       Good Accuracy mode
        # VL53L0X_BETTER_ACCURACY_MODE     Better Accuracy mode
        # VL53L0X_BEST_ACCURACY_MODE       Best Accuracy mode
        # VL53L0X_LONG_RANGE_MODE          Longe Range mode
        # VL53L0X_HIGH_SPEED_MODE          High Speed mode
        self.ranging_data = []
        self.ranging_duration_in_ms = 66

        # GPIO for each sensor shutdwon pin
        # self.sensor_BL = ranger("BL",0x1A, 27, self.ranging_mode)
        # self.sensor_BR = ranger("BR",0x1B, 17, self.ranging_mode)
        # self.sensor_GL = ranger("GL", 0x1C, 10, self.ranging_mode)
        # self.sensor_GR = ranger("GR", 0x1D, 9, self.ranging_mode)
        # self.sensor_RL = ranger("RL",0x1E, 22, self.ranging_mode)
        # self.sensor_RR = ranger("RR",0x1F, 11, self.ranging_mode)

        self.sensor_BL = ranger("BL",0x1A, 16, self.ranging_mode)
        self.sensor_BR = ranger("BR",0x1B, 13, self.ranging_mode)
        # self.sensor_GL = ranger("GL", 0x1C, 27, self.ranging_mode)
        # self.sensor_GR = ranger("GR", 0x1D, 10, self.ranging_mode)
        # self.sensor_RL = ranger("RL",0x1E, 11, self.ranging_mode)
        # self.sensor_RR = ranger("RR",0x1F, 22, self.ranging_mode)

        # self.ranger_list = [self.sensor_BR]
        self.ranger_list = [self.sensor_BL, self.sensor_BR]

        # self.ranger_list = [self.sensor_BL, self.sensor_BR, 
        #                     self.sensor_GL, self.sensor_GR,
        #                     self.sensor_RL, self.sensor_RR]
        #self.ranger_list = [self.sensor_BR]
        self.rangerPub = rospy.Publisher("tof_data", Float64MultiArray, queue_size=10)
        # rospy.Timer(rospy.Duration(0.001 * self.ranging_duration_in_ms), self.timerCallback)
        rospy.Timer(rospy.Duration(0.001 * 80), self.timerCallback)

    def timerCallback(self, event):
        # print("A_L = ",self.sensor_AL.measureDistance(), "A_R = ",self.sensor_AR.measureDistance(),)
        del self.ranging_data[:]
        for ranger in self.ranger_list:
            self.ranging_data.append(ranger.measureDistance()*100) #change to centermeter
        
        msg = Float64MultiArray()
        msg.data = self.ranging_data
        self.rangerPub.publish(msg)
        pass


if __name__ == "__main__":
    rospy.init_node('tof_ranging', anonymous = True)
    TOF = tof()
    rospy.spin()
