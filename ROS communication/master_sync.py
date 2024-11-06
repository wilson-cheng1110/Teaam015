#!/usr/bin/env python
import rospy
from fkie_master_sync import MasterSync

def main():
    rospy.init_node('master_sync_node', anonymous=True)
    master_sync = MasterSync()
    rospy.spin()

if __name__ == '__main__':
    main()