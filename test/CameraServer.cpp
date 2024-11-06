

#include "CameraServer.h"
#include "index_html.h"
#include "Arduino_JSON.h"

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

uint8_t car_speed_val[CAR_SPEED_MAX] = {0, 30, 60, 100};

struct ctrl_cmd mCtrlCmds[CMD_MAX];
bool mLedSwitch;
bool mTurnSwitch;
bool mBtnRstCamServoOnClicked;
uint8_t mCarSpeed;
int mJoystickValue[JOYSTICK_INDEX_MAX];

int mObjSortingStatus = 0; // 0 for unknown; 1 for idle(need detecting obj color then reply to ESP32); 2 for sorting is executing;
struct obj_color mObjColor[COLOR_MAX];
struct obj_info mObjInfo;

// Json Variable to Hold car status
JSONVar carStatus;

httpd_handle_t camera_httpd;
httpd_handle_t stream_httpd;
httpd_handle_t stream_httpd1;
httpd_handle_t stream_httpd2;

esp_err_t initCamServer()
{
    esp_err_t err;

    mLedSwitch = false;
    mTurnSwitch = false;
    mBtnRstCamServoOnClicked = false;
    mCarSpeed = CAR_SPEED_1;

    mCtrlCmds[CMD_LED_SWITCH].cmd = CMD_LED_SWITCH;
    mCtrlCmds[CMD_LED_SWITCH].cmd_str = "LedSwitch";
    mCtrlCmds[CMD_LED_SWITCH].cmd_func = processCmd;

    mCtrlCmds[CMD_TURN_SWITCH].cmd = CMD_TURN_SWITCH;
    mCtrlCmds[CMD_TURN_SWITCH].cmd_str = "TurnSwitch";
    mCtrlCmds[CMD_TURN_SWITCH].cmd_func = processCmd;

    mCtrlCmds[CMD_BUTTON].cmd = CMD_BUTTON;
    mCtrlCmds[CMD_BUTTON].cmd_str = "button";
    mCtrlCmds[CMD_BUTTON].cmd_func = processCmd;

    mCtrlCmds[CMD_SPEED].cmd = CMD_SPEED;
    mCtrlCmds[CMD_SPEED].cmd_str = "speed";
    mCtrlCmds[CMD_SPEED].cmd_func = processCmd;

    mCtrlCmds[CMD_JOYSTICK_COUNT].cmd = CMD_JOYSTICK_COUNT;
    mCtrlCmds[CMD_JOYSTICK_COUNT].cmd_str = "count";
    mCtrlCmds[CMD_JOYSTICK_COUNT].cmd_func = processCmd;

    mCtrlCmds[CMD_OBJ_COLOR].cmd = CMD_OBJ_COLOR;
    mCtrlCmds[CMD_OBJ_COLOR].cmd_str = "color";
    mCtrlCmds[CMD_OBJ_COLOR].cmd_func = processCmd;

    mObjColor[COLOR_RED].c = COLOR_RED;
    mObjColor[COLOR_RED].c_name = "red";

    mObjColor[COLOR_GREEN].c = COLOR_GREEN;
    mObjColor[COLOR_GREEN].c_name = "green";

    mObjColor[COLOR_BLUE].c = COLOR_BLUE;
    mObjColor[COLOR_BLUE].c_name = "blue";

    mObjInfo.color = COLOR_NONE;
    mObjInfo.obj_x = 0;
    mObjInfo.obj_y = 0;

    mObjSortingStatus = 1;

    err = initCamera();
    if (err != ESP_OK)
    {
        return err;
    }

    err = initServer();
    if (err != ESP_OK)
    {
        return err;
    }

    return ESP_OK;
}

esp_err_t initCamera()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    if (psramFound())
    {
        config.frame_size = FRAMESIZE_HD;
        config.jpeg_quality = 20;
        config.fb_count = 2;
    }
    else
    {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }
    Serial.printf("psramFound=%s, config.pixel_format=%d, config.frame_size=%d;\n",
                  psramFound() ? "yes" : "no", config.pixel_format, config.frame_size);

    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return err;
    }

#if 1
    sensor_t *s = esp_camera_sensor_get();

    // s->set_brightness(s, 1); // up the brightness just a bit
    // s->set_saturation(s, -2); // lower the saturation
    // s->set_framesize(s, FRAMESIZE_QVGA);
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
#endif

    return ESP_OK;
}

esp_err_t initServer()
{
    int ret;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    httpd_uri_t index_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = index_handler,
        .user_ctx = NULL};

    httpd_uri_t cmd_uri = {
        .uri = "/action",
        .method = HTTP_GET,
        .handler = cmd_handler,
        .user_ctx = NULL};

    httpd_uri_t status_uri = {
        .uri = "/status",
        .method = HTTP_GET,
        .handler = status_handler,
        .user_ctx = NULL};
    /*
        httpd_uri_t stream_uri = {
            .uri = "/stream",
            .method = HTTP_GET,
            .handler = stream_handler,
            .user_ctx = NULL};
    */
    httpd_uri_t stream_uri1 = {
        .uri = "/stream_one",
        .method = HTTP_GET,
        .handler = stream_handler1,
        .user_ctx = NULL};

    httpd_uri_t stream_uri2 = {
        .uri = "/stream_two",
        .method = HTTP_GET,
        .handler = stream_handler2,
        .user_ctx = NULL};

    if (httpd_start(&camera_httpd, &config) == ESP_OK)
    {
        ret = httpd_register_uri_handler(camera_httpd, &index_uri);
        Serial.printf("%s(): index_uri ret=0x%x\n", __func__, ret);

        ret = httpd_register_uri_handler(camera_httpd, &cmd_uri);
        Serial.printf("%s(): cmd_uri ret=0x%x\n", __func__, ret);

        ret = httpd_register_uri_handler(camera_httpd, &status_uri);
        Serial.printf("%s(): status_uri ret=0x%x\n", __func__, ret);
    }
    else
    {
        return ESP_FAIL;
    }

    // config.server_port += 1;
    // config.ctrl_port += 1;
    // if (httpd_start(&stream_httpd, &config) == ESP_OK)
    // {
    //     ret = httpd_register_uri_handler(stream_httpd, &stream_uri);
    //     Serial.printf("%s(): stream_uri ret=0x%x\n", __func__, ret);
    // }
    // else
    // {
    //     return ESP_FAIL;
    // }

    config.server_port = 81;
    config.ctrl_port += 1;
    if (httpd_start(&stream_httpd1, &config) == ESP_OK)
    {
        ret = httpd_register_uri_handler(stream_httpd1, &stream_uri1);
        Serial.printf("%s(): stream_uri1 ret=0x%x, config.server_port=%d, config.ctrl_port=%d\n", __func__, ret, config.server_port, config.ctrl_port);

        // ret = httpd_register_uri_handler(stream_httpd1, &stream_uri2);
        // Serial.printf("%s(): stream_uri2 ret=0x%x, config.server_port=%d, config.ctrl_port=%d\n", __func__, ret, config.server_port, config.ctrl_port);
    }
    else
    {
        return ESP_FAIL;
    }
    /*
        config.server_port = 82;
        config.ctrl_port += 1;
        if (httpd_start(&stream_httpd1, &config) == ESP_OK)
        {
            ret = httpd_register_uri_handler(stream_httpd1, &stream_uri2);
            Serial.printf("%s(): stream_uri2 ret=0x%x, config.server_port=%d, config.ctrl_port=%d\n", __func__, ret, config.server_port, config.ctrl_port);
        }
        else
        {
            return ESP_FAIL;
        }
    */
    return ESP_OK;
}

esp_err_t index_handler(httpd_req_t *req)
{
    Serial.println("index_handler E");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, (const char *)index_html, strlen(index_html));
}

esp_err_t stream_handler(httpd_req_t *req)
{
    camera_fb_t *fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t *_jpg_buf = NULL;
    char *part_buf[64];

    Serial.println("stream_handler E");
    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if (res != ESP_OK)
    {
        return res;
    }

    while (true)
    {
        fb = esp_camera_fb_get();
        if (!fb)
        {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
        }
        else
        {
            // Serial.printf("fb->format=%d, fb->width=%d, fb->height=%d;\n", fb->format, fb->width, fb->height);
            if (fb->width > 400)
            {
                if (fb->format != PIXFORMAT_JPEG)
                {
                    bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                    esp_camera_fb_return(fb);
                    fb = NULL;
                    if (!jpeg_converted)
                    {
                        Serial.println("JPEG compression failed");
                        res = ESP_FAIL;
                    }
                }
                else
                {
                    _jpg_buf_len = fb->len;
                    _jpg_buf = fb->buf;
                }
            }
        }
        if (res == ESP_OK)
        {
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if (res == ESP_OK)
        {
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if (res == ESP_OK)
        {
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if (fb)
        {
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        }
        else if (_jpg_buf)
        {
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
        if (res != ESP_OK)
        {
            break;
        }
        // Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
    }

    Serial.println("stream_handler X");
    return res;
}

esp_err_t stream_handler1(httpd_req_t *req)
{
    camera_fb_t *fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t *_jpg_buf = NULL;
    char *part_buf[64];

    Serial.println("stream_handler1 E");
    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if (res != ESP_OK)
    {
        return res;
    }

    while (true)
    {
        fb = esp_camera_fb_get();
        if (!fb)
        {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
        }
        else
        {
            // Serial.printf("fb->format=%d, fb->width=%d, fb->height=%d;\n", fb->format, fb->width, fb->height);
            if (fb->width > 400)
            {
                if (fb->format != PIXFORMAT_JPEG)
                {
                    bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                    esp_camera_fb_return(fb);
                    fb = NULL;
                    if (!jpeg_converted)
                    {
                        Serial.println("JPEG compression failed");
                        res = ESP_FAIL;
                    }
                }
                else
                {
                    _jpg_buf_len = fb->len;
                    _jpg_buf = fb->buf;
                }
            }
        }

        if (res == ESP_OK)
        {
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        else
            Serial.println("send part_buf fail");

        if (res == ESP_OK)
        {
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        else
            Serial.println("send _jpg_buf fail");

        if (res == ESP_OK)
        {
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        else
            Serial.println("send _STREAM_BOUNDARY fail");

        if (fb)
        {
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        }
        else if (_jpg_buf)
        {
            free(_jpg_buf);
            _jpg_buf = NULL;
        }

        if (res != ESP_OK)
        {
            Serial.println("stream_handler break");
            break;
        }
        // Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
    }

    Serial.println("stream_handler1 X");
    return res;
}

esp_err_t stream_handler2(httpd_req_t *req)
{
    camera_fb_t *fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t *_jpg_buf = NULL;
    char *part_buf[64];

    Serial.println("stream_handler 2 E");
    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if (res != ESP_OK)
    {
        return res;
    }

    while (true)
    {
        fb = esp_camera_fb_get();
        if (!fb)
        {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
        }
        else
        {
            // Serial.printf("fb->format=%d, fb->width=%d, fb->height=%d;\n", fb->format, fb->width, fb->height);
            if (fb->width > 400)
            {
                if (fb->format != PIXFORMAT_JPEG)
                {
                    bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                    esp_camera_fb_return(fb);
                    fb = NULL;
                    if (!jpeg_converted)
                    {
                        Serial.println("JPEG compression failed");
                        res = ESP_FAIL;
                    }
                }
                else
                {
                    _jpg_buf_len = fb->len;
                    _jpg_buf = fb->buf;
                }
            }
        }
        if (res == ESP_OK)
        {
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if (res == ESP_OK)
        {
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if (res == ESP_OK)
        {
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if (fb)
        {
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        }
        else if (_jpg_buf)
        {
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
        if (res != ESP_OK)
        {
            break;
        }
        // Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
    }

    Serial.println("stream_handler 2 X");
    return res;
}

esp_err_t cmd_handler(httpd_req_t *req)
{
    char *buf;
    size_t buf_len;
#define VALUE_SIZE (32)
    char value[VALUE_SIZE] = {0};

    // Serial.println("cmd_handler E");
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1)
    {
        buf = (char *)malloc(buf_len);
        if (!buf)
        {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
        {
            for (int i = 0; i < CMD_MAX; i++)
            {
                if (httpd_query_key_value(buf, mCtrlCmds[i].cmd_str, value, VALUE_SIZE) == ESP_OK)
                {
                    // Serial.printf("turn=%s\n", value);
                    mCtrlCmds[i].cmd_func(mCtrlCmds[i].cmd, value, buf);
                }
            }
        }
        else
        {
            free(buf);
            httpd_resp_send_404(req);
            return ESP_FAIL;
        }
        free(buf);
    }
    else
    {
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }
    // Serial.println("cmd_handler X");

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_send(req, NULL, 0);
}

String constructCarStatus()
{
    carStatus["mLedSwitch"] = String(mLedSwitch);
    carStatus["mTurnSwitch"] = String(mTurnSwitch);
    carStatus["mCarSpeed"] = String(mCarSpeed);
    String jsonString = JSON.stringify(carStatus);
    return jsonString;
}

esp_err_t status_handler(httpd_req_t *req)
{
    esp_err_t res = ESP_OK;

    Serial.println("status_handler E");

    httpd_resp_set_type(req, HTTPD_TYPE_JSON);

    carStatus["mLedSwitch"] = String(mLedSwitch);
    carStatus["mTurnSwitch"] = String(mTurnSwitch);
    carStatus["mCarSpeed"] = String(mCarSpeed);
    carStatus["mObjSortingStatus"] = String(mObjSortingStatus);
    String jsonString = JSON.stringify(carStatus);
    res = httpd_resp_send(req, jsonString.c_str(), jsonString.length());

    Serial.printf("status_handler: X, res=%d, %s\n", res, jsonString.c_str());
    return res;
}

esp_err_t processCmd(int cmd, char *cmd_val, char *query_buf)
{
    uint8_t s = 0;
    switch (cmd)
    {
    case CMD_LED_SWITCH:
        if (0 == strcmp(cmd_val, "true"))
        {
            Serial.printf("mLedSwitch = true\n");
            mLedSwitch = true;
        }
        else
        {
            Serial.printf("mLedSwitch = false\n");
            mLedSwitch = false;
        }
        break;
    case CMD_TURN_SWITCH:
        if (0 == strcmp(cmd_val, "true"))
        {
            mTurnSwitch = true;
        }
        else
        {
            mTurnSwitch = false;
        }
        break;
    case CMD_BUTTON:
        if (0 == strcmp(cmd_val, "ResetCamServo"))
        {
            mBtnRstCamServoOnClicked = true;
        }
        break;
    case CMD_SPEED:
        s = atoi(cmd_val);
        if (s > CAR_SPEED_0 || s < CAR_SPEED_MAX)
        {
            mCarSpeed = s;
        }
        break;
    case CMD_JOYSTICK_COUNT:
        processJoySticks(atoi(cmd_val), query_buf);
        break;
    case CMD_OBJ_COLOR:
        processObjColor(cmd_val, query_buf);
        break;
    default:
        break;
    }

    return ESP_OK;
}

esp_err_t processObjColor(char *cmd_val, char *query_buf)
{
    char val[32] = {0};

    mObjInfo.color = COLOR_NONE;
    mObjInfo.obj_x = 0;
    mObjInfo.obj_y = 0;

    /*
    xmlhttp1.open("GET", "/action?color=" + colorName[res.m.toFixed()] 
    + "&count=" + res.c
    + "&area=" + res.area.toFixed() 
    + "&objX=" + res.x.toFixed() 
    + "&objY=" + res.y.toFixed(), true);
    */
    Serial.printf("cmd_val=%s\n", cmd_val);
    for (int i = 0; i < COLOR_MAX; i++)
    {
        if (0 == strcmp(cmd_val, mObjColor[i].c_name))
        {
            mObjInfo.color = mObjColor[i].c;
            break;
        }
    }

    if (COLOR_NONE == mObjInfo.color)
    {
        return ESP_FAIL;
    }

    memset(val, 0, 32);
    if (httpd_query_key_value(query_buf, "count", val, sizeof(val)) != ESP_OK)
    {
        mObjInfo.color = COLOR_NONE;
        return ESP_FAIL;
    }
    else
    {
        mObjInfo.count = atoi(val);
        Serial.printf("color=%s, count=%d\n", mObjColor[mObjInfo.color].c_name, mObjInfo.count);
    }

    memset(val, 0, 32);
    if (httpd_query_key_value(query_buf, "area", val, sizeof(val)) != ESP_OK)
    {
        mObjInfo.color = COLOR_NONE;
        return ESP_FAIL;
    }
    else
    {
        mObjInfo.area = atoi(val);
    }

    memset(val, 0, 32);
    if (httpd_query_key_value(query_buf, "objX", val, sizeof(val)) != ESP_OK)
    {
        mObjInfo.color = COLOR_NONE;
        return ESP_FAIL;
    }
    else
    {
        mObjInfo.obj_x = atoi(val);
    }

    memset(val, 0, 32);
    if (httpd_query_key_value(query_buf, "objY", val, sizeof(val)) != ESP_OK)
    {
        mObjInfo.color = COLOR_NONE;
        return ESP_FAIL;
    }
    else
    {
        mObjInfo.obj_y = atoi(val);
    }

    return ESP_OK;
}

esp_err_t processJoySticks(int count, char *query_buf)
{
    char key[8] = {0};
    char val[32] = {0};

    /*
    xmlhttp1.open("GET","/action?count=" + count
        + "&joy1X=" + joy1X + "&joy1Y=" + joy1Y
        + "&joy2X=" + joy2X + "&joy2Y=" + joy2Y
        + "&joy3X=" + joy3X + "&joy3Y=" + joy3Y
        + "&joy4X=" + joy4X + "&joy4Y=" + joy4Y,
          true);
    */
    for (int i = 0; i < 4; i++)
    {
        for (char j = 0; j < 2; j++)
        {
            memset(key, 0, 8);
            memset(val, 0, 32);
            snprintf(key, 8, "joy%d%c", i + 1, j + 'X');
            // Serial.printf("key=%s\n", key);
            if (httpd_query_key_value(query_buf, key, val, sizeof(val)) != ESP_OK)
            {
                return ESP_FAIL;
            }

            mJoystickValue[2 * i + j] = atoi(val);
        }
    }
#if 0
    for (int i = 0; i < 4; i++)
    {
        Serial.printf("joy[%d]X=%d, joy[%d]Y=%d\n",
                      i + 1, mJoystickValue[2 * i + 0], i + 1, mJoystickValue[2 * i + 1]);
    }
#endif
    // Serial.printf("joy1X=%d, joy1Y=%d, joy2X=%d, joy2Y=%d, joy3X=%d, joy3Y=%d, joy4X=%d, joy4Y=%d\n",
    //               mJoystickValue[0], mJoystickValue[1], mJoystickValue[2], mJoystickValue[3],
    //               mJoystickValue[4], mJoystickValue[5], mJoystickValue[6], mJoystickValue[7]);

    return ESP_OK;
}

bool getLedSwitchState(void)
{
    return mLedSwitch;
}

bool getTurnSwitchState(void)
{
    return mTurnSwitch;
}

bool needRstCamServo(void)
{
    bool need = mBtnRstCamServoOnClicked;
    mBtnRstCamServoOnClicked = false;
    return need;
}

uint8_t getCarSpeed(void)
{
    return car_speed_val[mCarSpeed];
}

void getJoystickValue(int *value)
{
    if (NULL == value)
    {
        return;
    }

    memcpy(value, mJoystickValue, JOYSTICK_INDEX_MAX * sizeof(int));
}

void getObjInfo(struct obj_info *info)
{
    if (NULL == info)
    {
        return;
    }

    memcpy(info, &mObjInfo, sizeof(struct obj_info));
}

void clearObjInfo(void)
{
    mObjInfo.color = COLOR_NONE;
    mObjInfo.obj_x = 0;
    mObjInfo.obj_y = 0;
}

void setObjSortingStatus(int status)
{
    mObjSortingStatus = status;
}
