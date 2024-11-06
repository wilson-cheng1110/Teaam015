#include "kinematics.h"

swerve_drive::swerve_drive(float length, float width, float diameter, int pole)
    : chassisLength(length), chassisWidth(width), wheelDiameter(diameter), RPM2eRPM(pole)
{
   for (int i = 0; i < no_modules; i++)
   {
      modules_vx[i] = 0;
      modules_vy[i] = 0;
      wheel_speed[i] = 0;
      wheel_angle[i] = 0;
   }
}


void swerve_drive::ik(float vx, float vy, float vw, float *getRpm, float *getAngle, float *_wheel_speed, float *_wheel_angle) {
   for (int i = 0; i < no_modules; i++) {
    //   modules_vx[i] = vx + vw * (rx[i]);
    //   modules_vy[i] = vy + vw * (ry[i]);
    if(i == 0 || i == 3) {
        modules_vx[i] = vx - vw * (rx[i]);
        modules_vy[i] = vy - vw * (ry[i]);
    } else {
        modules_vx[i] = vx + vw * (rx[i]);
        modules_vy[i] = vy + vw * (ry[i]);
    }
      wheel_speed[i] = sqrt((modules_vx[i] * modules_vx[i] + modules_vy[i] *modules_vy[i]) )* MS2RPM * RPM2eRPM;
    //   wheel_angle[i] = atan2(modules_vy[i], modules_vx[i])* (180.0 / PI);
      if (modules_vx[i] == 0  && modules_vy[i] == 0) {
         _wheel_angle[i] = wheel_angle[i];
         _wheel_speed[i] = 0;
      } else {
        dyaw = atan2(modules_vy[i], modules_vx[i]) - wheel_angle[i];
        if (dyaw > PI) { // find closer direction
            dyaw -= 2 * PI;
        } else if (dyaw < -PI) {
            dyaw += 2 * PI;
        }

        if (dyaw >= PI * 0.5) {
            dyaw -= PI;
            wheel_speed[i] *= -1.0;
        } else if (dyaw <= -PI * 0.5) {
            dyaw += PI;
            wheel_speed[i] *= -1.0;
        }

        wheel_angle[i] += dyaw;

        // pass the calcaled kine to outside world. Here is so dark help...
        _wheel_speed[i] = wheel_speed[i];
        _wheel_angle[i] = wheel_angle[i];
     // }
        }
    }
}

// void swerve_drive::ik(float vx, float vy, float vw, float *getRpm, float *getAngle, float *_wheel_speed, float *_wheel_angle) {

//     // float speed_x = map(vx, -0.1, 0.1, -xySpeed, xySpeed);
//     // float speed_y = map(vy, -0.1, 0.1, -xySpeed, xySpeed);
//     // float speed_w = map(vw, -0.1, 0.1, wSpeed, -wSpeed);

//     float sw[4];
//     sw[0] = vx + (vw * (0.545 / 0.035)); 
//     sw[1] = vx - (vw * (0.545 / 0.035));
//     sw[2] = vy - (vw * (0.545 / 0.035));
//     sw[3] = vy + (vw * (0.545 / 0.035));
//     //Field Centric Kinematics

//     if(vx == 0 && vy == 0 && vw == 0){
//         //stop moving if the joystick is idle
//         for(int i=0;i<4;i++) {
//             wheel_speed[i] = 0;
//         }
//     } else {
//         //order may be incorrect -> refer to paper
//         wheel_speed[0] = sqrt(sw[1] * sw[1] + sw[3] * sw[3]) * MS2RPM * RPM2eRPM;
//         wheel_speed[1] = sqrt(sw[1] * sw[1] + sw[2] * sw[2]) * MS2RPM * RPM2eRPM;
//         wheel_speed[2] = sqrt(sw[0] * sw[0] + sw[3] * sw[3]) * MS2RPM * RPM2eRPM;
//         wheel_speed[3] = sqrt(sw[0] * sw[0] + sw[2] * sw[2]) * MS2RPM * RPM2eRPM;

//         wheel_angle[0] = ((atan2(sw[1], sw[3])) * RAD2DEG) * MS2RPM * RPM2eRPM; // this is set point
//         wheel_angle[1] = -((atan2(sw[1], sw[2])) * RAD2DEG) * MS2RPM * RPM2eRPM;
//         wheel_angle[2] = ((atan2(sw[0], sw[3])) * RAD2DEG) * MS2RPM * RPM2eRPM; //atan2(sw[0], sw[2])
//         wheel_angle[3] = -((atan2(sw[0], sw[2])) * RAD2DEG) * MS2RPM * RPM2eRPM; //atan2(sw[0], sw[3])
//     }

//     //stop moving if the joystick is idle??
//     if((vx + vy + vw) == 0) {
//         for(int i=0;i<4;i++) {
//             wheel_speed[i] = 0;
//         }
//     }
//     for(int i=0; i<4;i++) {
//         _wheel_speed[i] = wheel_speed[i];
//         _wheel_angle[i] = wheel_angle[i];
//     }

// }
