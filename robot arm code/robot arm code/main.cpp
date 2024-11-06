#include <ThisThread.h>
#include <mbed.h>

#include "vesc.h"
#include "kinematics.h"
//#include "cchs_can_bus.h"

#include <cstdint>

const int canBaudRate = 1000000;   // 1000000
CAN can1(PD_0, PD_1, canBaudRate); // Can bus for transceiving controller commands
CAN can2(PB_5, PB_6, canBaudRate); // Can bus for transceiving chassis vesc commands
CAN can3(PB_3, PB_4, canBaudRate); // Can bus for transceiving sensor data

//-------------------------------CAN 1-------------------------------//
typedef enum {
      CAN_PACKET_CHASSIS_COMMAND_SPEED_X__Y = 0,
      CAN_PACKET_CHASSIS_COMMAND_SPEED_W,
      CAN_PACKET_CCHS_ALIVE_STATE,

} CAN_PACKET_CHASSIS_ID;

//-------------------------------CAN 2-------------------------------//
int canDrivingId[4] = {5, 6, 7, 8}; // ref above graph
int canSteeringId[4] = {1, 2, 3, 4};
const int canAliveStateId = 5;
CANMessage rx1Msg;
CANMessage rx2Msg;


static BufferedSerial pc(USBTX, USBRX);

Vesc _vesc;

Thread canThread;
Thread aliveStateThread;
Thread publishVescThread;

// Kinematics Settings
swerve_drive swerve(0.542f, 0.550f, 0.073f, 7); // figure pls ref INVERSE_KINEMATICS_H
float joyX = 0, joyY = 0, joyW = 0;
float getDrivingRpm[4], getSteeringAngle[4];
float sendDrivingRpm[4], sendSteeringAngle[4];

// Frequency Setting
int chassis2vesc_CAN_bus_frequency = 50; // Hz
int CCHS2nuc_alive_CAN_bus_frequency = 2; // Hz

void vesc_read() {
    for (int i = 0; i < 4; i++) {
        _vesc.can_read(canDrivingId[i]);
        _vesc.can_read(canSteeringId[i]);
    }
}

void printMsg(CANMessage &msg) {
    // printf("  ID      = 0x%.3x\r\n", msg.id);
    // printf("  Type    = %d\r\n", msg.type);
    // printf("  Format  = %d\r\n", msg.format);
    // printf("  Length  = %d\r\n", msg.len);
    // printf("  Data    =");
    // for(int i = 0; i < msg.len; i++)
    //     printf(" 0x%.2X", msg.data[i]);
    // printf("\r\n");
    printf("  ID      = 0x%.3x   ", msg.id);
    printf("%02X", msg.data[0]);
    printf("\r\n");
}

float uint8ArrayToFloat32(const uint8_t *array) {
    uint32_t floatAsUint32 = array[0] | (array[1] << 8) | (array[2] << 16) | (array[3] << 24); // Combine the individual bytes into a uint32_t value
    float *floatPointer = reinterpret_cast<float *>(&floatAsUint32);                           // Interpret the uint32_t value as a float pointer
    return *floatPointer;
}

void can1MessageHandler(CANMessage msg) {
    int msgId = msg.id;
    switch (msgId) {
        case CAN_PACKET_CHASSIS_COMMAND_SPEED_X__Y: {
            uint8_t arrX[4] = {msg.data[0], msg.data[1], msg.data[2], msg.data[3]};
            joyX = uint8ArrayToFloat32(arrX);
            //printf("%f\r\n", joyX);

            uint8_t arrY[4] = {msg.data[4], msg.data[5], msg.data[6], msg.data[7]};
            joyY = uint8ArrayToFloat32(arrY);
            // printf("%f\r\n", joyY);
            break;
        }
        case CAN_PACKET_CHASSIS_COMMAND_SPEED_W: {
            uint8_t arrW[4] = {msg.data[0], msg.data[1], msg.data[2], msg.data[3]};
            joyW = uint8ArrayToFloat32(arrW);
            //printf("%f\r\n", joyW);

            break;
        }
        default:{ // stop the motors on error
            joyX = 0;
            joyY = 0;
            joyW = 0;
            //printf("ERROR IN READING CAN MESSAGE ID");
            break;
        }
    }
}

void can1Listener() {
    while (true) {
        while (can1.read(rx1Msg) == 1) {
            if (can1.rderror()) {
                printf("\ncan error\n");
                can1.reset();
            } else {
                can1MessageHandler(rx1Msg);
            }
        }
        // printf("can ping\n");
    }
}

void stmAliveStateSender() {// Send alive state to the CAN Bus
    while (true) {
        can1.write(CANMessage(canAliveStateId, "1", 1)); // msgID, msg, msgSize(8bits)
        ThisThread::sleep_for(500ms);                    // 2Hz
    }
}

void vescPublisher() {
    while (true) {
        swerve.ik(joyX, joyY, joyW, getDrivingRpm, getSteeringAngle, sendDrivingRpm, sendSteeringAngle);
        for (int i = 0; i < 4; i++) {
            // if(joyX ==0 && joyY == 0)
            // {
                _vesc.set_rpm(canDrivingId[i], sendDrivingRpm[i]);
            // }
            // else
            // {
            //     _vesc.set_current_brake(canDrivingId[i], 1);
            // }

        }
        for (int i = 0; i < 4; i++) {
            _vesc.set_pos(canSteeringId[i], sendSteeringAngle[i]* (180.0 / PI));
        }
        //printf("%f\n", sendDrivingRpm[2]);     
        ThisThread::sleep_for(30ms); // unit in milliseconds
    }
}

// testing
//  void rpmHandler() {
//      while(true) {
//          if(can1.read(msg)) {
//              if(msg.id == VESC_CAN_ID) {
//                  int32_t erpm = (msg.data[0] << 24) | (msg.data[1] << 16) | (msg.data[2] << 8) | msg.data[3];
//                  float rpm = erpm / 7.0;
//                  printf("VESC RPM: %f\n", rpm);
//              }
//          }
//          wait_ms(100);
//      }
//  }

int main() {
    _vesc.vesc_init(&can2, canBaudRate); // VESC Initialization
    for (int i = 0; i < 4; i++) {
        _vesc.set_monitor_id(canDrivingId[i]);
        _vesc.set_monitor_id(canSteeringId[i]);
    }
    // vesc_read();

    // Main Setup Program
    char msg[] = "Hello world";
    // debugMsg(msg);
    printf("%s\n", msg);

    // Threads
    canThread.start(can1Listener);               // listen to can1
    aliveStateThread.start(stmAliveStateSender); // send alive state
    publishVescThread.start(vescPublisher);      //

    // printf("hihihi %d\n",123);
    // pc.write(msg, sizeof(msg));
    // can1.write(CANMessage(1, "hihi", 2)); //msgID, msg, msgSize(8bits)
    // can3.write(CANMessage(1, "hihi", 2, CANRemote));

    // float rpm = 9.9;

    while (true) {
        // _vesc.set_rpm(6, 2000);
        // _vesc.set_pos(canSteeringId[0], 90);
        // vesc_read();
        //_-vesc.can_read(8);
        // rpm = _vesc.read_rpm(8);
        // printf("%f\n",rpm);
        ThisThread::sleep_for(1ms); // small delay for updating
    }
}

/*
STEPS:
1. try controlling vesc √(1-NOV-2023): fixed can bus buffer overflow issue
2. try communication with computer √(3-NOV-2023): can transceive CAN messages
3. receive joy data √(6-NOV-2023): can receive data from NUC -> next: process data
4. implement kinematics √(7-NOV-2023): waiting data from NUC to test
5. test kinematics √(13-NOV-2023): require fixing in kinematics
6. test vesc library (read pos & rpm)
7. try controlling the robot
*/

/*
TODO:
- point all wheels to same direction (red line) (to ensure all starting angle are same)
*/
