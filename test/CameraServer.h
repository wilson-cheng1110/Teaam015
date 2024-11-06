#include <Arduino.h>

#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h"          // disable brownout problems
#include "soc/rtc_cntl_reg.h" // disable brownout problems
#include "esp_http_server.h"
#include "esp32_cam_gpio_config.h"

#ifndef _CAMERA_SERVER_H
#define _CAMERA_SERVER_H

/*
Joysitck 1: control ARM baseServo & LeftServo
Joystick 2: contorl ARM rightServo & ClawServo
*/
enum
{
    JOYSTICK_1_X = 0,
    JOYSTICK_1_Y,
    JOYSTICK_2_X,
    JOYSTICK_2_Y,
    JOYSTICK_3_X,
    JOYSTICK_3_Y,
    JOYSTICK_4_X,
    JOYSTICK_4_Y,
    JOYSTICK_INDEX_MAX,
    JOYSTICK_CLAW_SERVO = JOYSTICK_2_X,
};

enum
{
    CAR_SPEED_0 = 0,
    CAR_SPEED_1,
    CAR_SPEED_2,
    CAR_SPEED_3,
    CAR_SPEED_MAX,
};

enum
{
    CMD_LED_SWITCH = 0,
    CMD_TURN_SWITCH,
    CMD_BUTTON,
    CMD_SPEED,
    CMD_JOYSTICK_COUNT,
    CMD_OBJ_COLOR,
    CMD_MAX,
};

enum
{
    COLOR_MIN = 0,
    COLOR_RED = COLOR_MIN,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_GREY,
    COLOR_MAX,
    COLOR_NONE,
};

typedef esp_err_t (*cmd_func_t)(int cmd, char *cmd_val, char *query_buf);

struct ctrl_cmd
{
    int cmd;
    const char *cmd_str;
    cmd_func_t cmd_func;
};

struct obj_color
{
    int c;
    const char *c_name;
};

struct obj_info
{
    int color;
    int count;
    int area;
    int obj_x;
    int obj_y;
};

esp_err_t initCamServer(void);
esp_err_t initServer(void);
esp_err_t initCamera(void);

esp_err_t index_handler(httpd_req_t *req);
esp_err_t stream_handler(httpd_req_t *req);
esp_err_t stream_handler1(httpd_req_t *req);
esp_err_t stream_handler2(httpd_req_t *req);
esp_err_t cmd_handler(httpd_req_t *req);
esp_err_t status_handler(httpd_req_t *req);

esp_err_t processCmd(int cmd, char *cmd_val, char *query_buf);
esp_err_t processJoySticks(int count, char *query_buf);
esp_err_t processObjColor(char *cmd_val, char *query_buf);

bool getLedSwitchState(void);
bool getTurnSwitchState(void);
bool needRstCamServo(void);
uint8_t getCarSpeed(void);
void getJoystickValue(int *value);
void getObjInfo(struct obj_info *info);
void clearObjInfo(void);
void setObjSortingStatus(int status);


#endif /* _CAMERA_SERVER_H */
