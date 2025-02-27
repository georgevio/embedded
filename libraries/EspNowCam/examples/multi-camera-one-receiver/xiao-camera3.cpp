/**************************************************
 * ESPNowCam video Transmitter.
 * ----------------------------
 * 
 * XIAO FPV ESPNow transmitter uses some extra features of
 * this board to have some power consumption improvements 
 *
 * by @hpsaturn Copyright (C) 2024
 * This file is part ESP32S3 camera tests project:
 * https://github.com/hpsaturn/ESPNowCam
**************************************************/

#include <Arduino.h>
#include <OneButton.h>
#include <ESPNowCam.h>
#include <drivers/CamXiao.h>
#include <Utils.h>

CamXiao Camera;  
ESPNowCam radio;
OneButton btnB(GPIO_NUM_0, true);

void processFrame() {
  if (Camera.get()) {
    uint8_t *out_jpg = NULL;
    size_t out_jpg_len = 0;
    frame2jpg(Camera.fb, 12, &out_jpg, &out_jpg_len);
    radio.sendData(out_jpg, out_jpg_len);
    delay(30);
    printFPS("CAM:");
    free(out_jpg);
    Camera.free();
  }
}

void shutdown() {
  Serial.println("shutdown..");
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,0);
  delay(1000);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);

  delay(1000); // only for debugging 

  if(psramFound()){
    size_t psram_size = esp_spiram_get_size() / 1048576;
    Serial.printf("PSRAM size: %dMb\r\n", psram_size);
  }

  // M5Core2 receiver MAC: B8:F0:09:C6:0E:CC
  // const uint8_t macRecv[6] = {0xB8,0xF0,0x09,0xC6,0x0E,0xCC};
  // Makerfabs receiver 7C:DF:A1:F3:73:3C
  const uint8_t macRecv[6] = {0x7C,0xDF,0xA1,0xF3,0x73,0x3C};
  radio.setTarget(macRecv);
  radio.init();

  // You are able to change the Camera config E.g:
  Camera.config.fb_count = 1;
  Camera.config.fb_location = CAMERA_FB_IN_DRAM;
  Camera.config.frame_size = FRAMESIZE_QQVGA;

  if (!Camera.begin()) {
    Serial.println("Camera Init Fail");
    delay(1000);
  }

  btnB.attachClick([]() { shutdown(); });
  delay(100);
}

void loop() {
  processFrame();
  btnB.tick();
}
