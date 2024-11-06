#!/usr/bin/env python
import rospy
from fkie_master_discovery import MasterDiscovery

def main():
    rospy.init_node('master_discovery_node', anonymous=True)
    master_discovery = MasterDiscovery()
    rospy.spin()

if __name__ == '__main__':
    main()