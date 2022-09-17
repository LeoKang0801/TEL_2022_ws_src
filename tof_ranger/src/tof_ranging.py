#!/usr/bin/python3

import rospy
from std_msgs.msg import Float64MultiArray

import time
import board
from digitalio import DigitalInOut
from adafruit_vl53l0x import VL53L0X

class ranger:
    def __init__(self, idx, address, shutdown_pin, outliar_threshold):
        self.idx = idx
        self.address = address
        self.xshut = shutdown_pin
        self.vl53 = None
        self.outliar_threshold = outliar_threshold
        
        # calibration parameters
        self.calib_m = 1.05
        self.calib_c = 0.03
        self.calib_m = rospy.get_param('/vl53_tof_ranger/' + idx + "_M")
        self.calib_c = rospy.get_param('/vl53_tof_ranger/' + idx + "_C")

        self.initHardware()

    def initHardware(self):
        # make sure these pins are a digital output, not a digital input
        self.xshut.switch_to_output(value=False)
        pass

    def measureDistance(self):
        distance = self.vl53.range/1000
        if (distance > 0):
            # print ("sensor %d - %d mm, %d cm" % (self.sensor.my_object_number, distance, (distance/10)))
            distance = (distance - self.calib_c)/self.calib_m
            pass
        else:
            # print ("%d - Error" % tof.my_object_number)
            pass

        if (distance > self.outliar_threshold):
            distance = -1
        return distance
        
class tof:
    def __init__(self):
        self.sensor_BL = ranger("BL", 0x1A, DigitalInOut(board.D17), 6.5)
        self.sensor_BR = ranger("BR", 0x1B, DigitalInOut(board.D9), 6.5)
        self.sensor_GL = ranger("GL", 0x1C, DigitalInOut(board.D27), 6.5)
        self.sensor_GR = ranger("GR", 0x1D, DigitalInOut(board.D10), 6.5)
        self.sensor_RL = ranger("RL", 0x1E, DigitalInOut(board.D11), 6.5)
        self.sensor_RR = ranger("RR", 0x1F, DigitalInOut(board.D22), 6.5)

        self.ranging_data = []
        self.ranger_list = [self.sensor_BL, self.sensor_BR,
                            self.sensor_GL, self.sensor_GR,
                            self.sensor_RL, self.sensor_RR]
        
        self.rangerPub = rospy.Publisher("tof_data", Float64MultiArray, queue_size=10)
        self.initHardware()
        rospy.Timer(rospy.Duration(0.001 * 80), self.timerCallback)
    
    def initHardware(self):
        i2c = board.I2C()
        for i, ranger in enumerate(self.ranger_list):
            ranger.xshut.value = True
            ranger.vl53 = VL53L0X(i2c)
            ranger.vl53.start_continuous()
            ranger.vl53.measurement_timing_budget = 80000
            ranger.vl53.set_address(ranger.address) 

    def timerCallback(self, event):
        # print("A_L = ",self.sensor_AL.measureDistance(), "A_R = ",self.sensor_AR.measureDistance(),)
        del self.ranging_data[:]
        for ranger in self.ranger_list:
            self.ranging_data.append(ranger.measureDistance())
        
        msg = Float64MultiArray()
        msg.data = self.ranging_data
        self.rangerPub.publish(msg)
        pass
    
if __name__ == "__main__":
    rospy.init_node('tof_ranging', anonymous = True)
    TOF = tof()
    rospy.spin()
