/**
 * @file       inverse_kinematics.h
 * @author     Ian H.S. Chan
 * @email      polyu.robocon@gmail.com
 * @brief      This stm program is a part of robot Architecture, named CCHS
 * @version    1.0.0
 * @date       27-12-2023
 * @copyright  Copyright (c) 2023
 *
 * @section    LICENSE
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * @section    DESCRIPTION
 *
 * The Chassis Command Handle System (CCHS) interprets CAN bus signals from
 * a ROS-based master computer to execute inverse kinematics for a swerve drive robot.
 * It translates high-level motion commands into
 * individual wheel speeds and angles, ensuring precise, agile maneuvering.
 */

/* //---Swerve Drive Chassis Steering and driving modules CAN ID configurations--//
 *
 *
 *       /----- --Front-------\           ---           Modulus Config
 *       | [5]            [6] |            |
 *       |   {1}        {2}   |            |             [0]------[1]
 *       |                    |            |              |        |
 *    Left                    Right      Length           |        |
 *       |                    |            |              |        |
 *       |   {3}        {4}   |            |             [2]------[3]
 *       | [7]            [8] |            |
 *       \--------Aft---------/           ---
 *
 *
 *      |---------Width--------|
 *
 * [Driving module];
 * {Steering Module}
 *
 *                              //Mechanical Data//
 * Swerve Module: Ian H.S. Chan Inwheel Swerve V2.0 (https://github.com/PolyU-Robocon/2022-23-V2.0-PolyU-Robotics-Club-inwheel-Swerve-Drive)
 * Wheel Diameter: 73mm
 * Length: 542mm
 * Width: 550mm
 * Note: length and width diamenter is determined by measuring from wheel hub center
 *
 * Driving Motor: T-Motor AT4130 450KV 14P;
 * Sun Gear: 21T 0.7m
 * Plantary Gear: 20T
 * Ring Gear: 63T
 * Gear Ratio = 1 + Zring / Zsun = 5 : 1 (Fix ring gear config)
 *
 * Steering Motor: DJI M3508 P19;
 * Gear Ratio = 102/48 * 19.2 (settled on vesc tools)
 *
 * //----------------------------------------------------------------------------//
 * Reference
 * htts://www.chiefdelphi.com/t/swerve-drive-direct-and-reverse-kinematics/395803
 * https://www.chiefdelphi.com/t/paper-4-wheel-independent-drive-independent-steering-swerve/107383
 * https://www.chiefdelphi.com/uploads/default/original/3X/e/f/ef10db45f7d65f6d4da874cd26db294c7ad469bb.pdf
 *
 */

/**
 * @brief Computes the individual wheel velocities and orientations based on the robot's desired velocity and angular velocity.
 *
 * @param vx Input - The x component of the robot's desired velocity, input joy data
 * @param vy Input - The y component of the robot's desired velocity, input joy data
 * @param vw Input - The robot's desired angular velocity, input joy data
 * @param getRpm Input array - Swerve modules driviing motor RPM
 * @param getAnglefloat Input array - Swerve modules steering motor angle
 *
 * @param modules_v Output array - Array to store the computed velocities for each module.
 * @param modules_w Output array - Array to store the computed orientations for each module.
 *
 */

#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <math.h>
#include <cmath>

#define RAD2DEG 57.2957795131 // 180 / PI
#define PI 3.14159265358979

class swerve_drive
{
public:
    swerve_drive(float length, float width, float diameter, int pole);

    void ik(float vx, float vy, float vw, float *getRpm, float *getAngle, float *_wheel_speed, float *_wheel_angle);

private:
    static constexpr int no_modules = 4; // 4 leg swerve drive

    // Calcualting varible
    int rx[4] = {1, 1, -1, -1}, ry[4] = {1, -1, 1, -1};
    float modules_vx[no_modules], modules_vy[no_modules];
    float wheel_speed[no_modules], wheel_angle[no_modules];

    float chassisLength;
    float chassisWidth;
    float wheelDiameter;
    int RPM2eRPM; // rpm to erpm
    float MS2RPM = 60 / (PI * wheelDiameter); // convert ms-1 to rpm

    double dyaw;
};

#endif /*KINEMATICS_H*/
