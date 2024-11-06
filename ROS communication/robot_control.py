#!/usr/bin/env python
import rospy
from std_msgs.msg import String

def robot_control():
    pub = rospy.Publisher('robot_status', String, queue_size=10)
    rospy.init_node('robot_control_node', anonymous=True)
    rate = rospy.Rate(10)  # 10 Hz

    while not rospy.is_shutdown():
        status_msg = "Robot is operational"
        rospy.loginfo(status_msg)
        pub.publish(status_msg)
        rate.sleep()

if __name__ == '__main__':
    try:
        robot_control()
    except rospy.ROSInterruptException:
        pass