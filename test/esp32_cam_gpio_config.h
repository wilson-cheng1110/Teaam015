#include <Arduino.h>

#ifndef ESP32_CAM_GPIO_CONFIG_H
#define ESP32_CAM_GPIO_CONFIG_H

enum
{
	GPIO_00 = 0,
	GPIO_01,
	GPIO_02,
	GPIO_03,
	GPIO_04,
	GPIO_05,
	GPIO_06,
	GPIO_07,
	GPIO_08,
	GPIO_09,
	GPIO_10,
	GPIO_11,
	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15,
	GPIO_16,
	GPIO_17,
	GPIO_18,
	GPIO_19,
	GPIO_20,
	GPIO_21,
	GPIO_22,
	GPIO_23,
	GPIO_24,
	GPIO_25,
	GPIO_26,
	GPIO_27,
	GPIO_28,
	GPIO_29,
	GPIO_30,
	GPIO_31,
	GPIO_32,
	GPIO_33,
	GPIO_34,
	GPIO_35,
	GPIO_36,
	GPIO_37,
	GPIO_38,
	GPIO_39,
	GPIO_40,
	GPIO_41,
	GPIO_42,
	GPIO_43,
	GPIO_44,
	GPIO_45,
	GPIO_46,
	GPIO_47,
	GPIO_48,
	GPIO_49,
	GPIO_50,
};

/*****************************************  AI-Thinker ESP32-CAM Board *****************************************/
// MicroSD card / TFCard
#define HS2_CLK      (GPIO_14)
#define HS2_CMD      (GPIO_15)
#define HS2_DATA0    (GPIO_02)
#define HS2_DATA1    (GPIO_04)
#define HS2_DATA2    (GPIO_12)
#define HS2_DATA3    (GPIO_13)

#define TFCARD_CLK  	HS2_CLK  	
#define TFCARD_CMD  	HS2_CMD  	
#define TFCARD_DATA0	HS2_DATA0	
#define TFCARD_DATA1	HS2_DATA1	
#define TFCARD_DATA2	HS2_DATA2	
#define TFCARD_DATA3	HS2_DATA3	

// Camera
#define CAMERA_D0   	(GPIO_05)
#define CAMERA_D1   	(GPIO_18)
#define CAMERA_D2   	(GPIO_19)
#define CAMERA_D3   	(GPIO_21)
#define CAMERA_D4   	(GPIO_36)
#define CAMERA_D5   	(GPIO_39)
#define CAMERA_D6   	(GPIO_34)
#define CAMERA_D7   	(GPIO_35)
#define CAMERA_XCLK 	(GPIO_00)
#define CAMERA_PCLK 	(GPIO_22)
#define CAMERA_VSYNC    (GPIO_25)
#define CAMERA_HREF     (GPIO_23)
#define CAMERA_SDA      (GPIO_26)
#define CAMERA_SCL      (GPIO_27)
#define CAMERA_POWER    (GPIO_32)

#define Y2_GPIO_NUM     CAMERA_D0
#define Y3_GPIO_NUM     CAMERA_D1
#define Y4_GPIO_NUM     CAMERA_D2
#define Y5_GPIO_NUM     CAMERA_D3
#define Y6_GPIO_NUM     CAMERA_D4
#define Y7_GPIO_NUM     CAMERA_D5
#define Y8_GPIO_NUM     CAMERA_D6
#define Y9_GPIO_NUM     CAMERA_D7
#define XCLK_GPIO_NUM   CAMERA_XCLK
#define PCLK_GPIO_NUM   CAMERA_PCLK
#define VSYNC_GPIO_NUM  CAMERA_VSYNC
#define HREF_GPIO_NUM   CAMERA_HREF
#define SIOD_GPIO_NUM   CAMERA_SDA
#define SIOC_GPIO_NUM   CAMERA_SCL
#define PWDN_GPIO_NUM   CAMERA_POWER
#define RESET_GPIO_NUM    -1

// FlashLight
#define ESP32_CAM_FLASHLIGHT_PIN    (GPIO_33)   

// Notify LED
#define ESP32_CAM_NOTIFY_LED_PIN    (GPIO_04) // HS2_DATA1

// PCA9685
#define PCA9685_SDA_PIN     (GPIO_13)   // HS2_DATA3
#define PCA9685_SCL_PIN		  (GPIO_14)   // HS2_CLK

// Ultrasonic
#define US_ECHO_PIN     (GPIO_15)   // HS2_CMD
#define US_TRIG_PIN     (GPIO_12)   // HS2_DATA2

// Buzzer
#define BUZZER_PIN  (GPIO_02)   // HS2_DATA0


#endif /* ESP32_CAM_GPIO_CONFIG_H */
