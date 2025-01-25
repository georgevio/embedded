/**
 * SPIFFS file storage: manual filename
 * 
 * This sketch shows how to save a picture on SPIFFS
 * filesystem by specifying the filename manually
 *
 * Open the Serial Monitor and enter 'capture' (without quotes)
 * to capture a new image and save it
 *
 * BE SURE TO SET "TOOLS > CORE DEBUG LEVEL = INFO"
 * to turn on debug messages
 */

#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/fs/spiffs.h>

using namespace eloq;


void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println("___SAVE PIC TO SPIFFS___");

    // camera settings
    // replace with your own model!
    camera.pinout.wroom_s3();
    camera.brownout.disable();
    camera.resolution.vga();
    camera.quality.high();

    // init camera
    while (!camera.begin().isOk())
        Serial.println(camera.exception.toString());

    // init SPIFFS
    while (!spiffs.begin().isOk())
        Serial.println(spiffs.exception.toString());

    Serial.println("Camera OK");
    Serial.println("SPIFFS OK");
    Serial.println("Enter the filename where to save the picture");
}


void loop() {
    // await for filename from the Serial Monitor
    if (!Serial.available())
        return;

    String filename = Serial.readStringUntil('\n');
    filename.trim();

    if (!filename.endsWith(".jpg") && !filename.endsWith(".jpeg"))
        filename = filename + ".jpg";

    // capture picture
    if (!camera.capture().isOk()) {
        Serial.println(camera.exception.toString());
        return;
    }

    // save to SPIFFS
    if (spiffs.save(camera.frame).to(filename).isOk()) {
      Serial.print("File written to ");
      Serial.println(spiffs.session.lastFilename);
    }
    else Serial.println(spiffs.session.exception.toString());

    // you can also save under a nested folder
    if (spiffs.save(camera.frame).inside("myfolder").to(filename).isOk()) {
      Serial.print("File written to ");
      Serial.println(spiffs.session.lastFilename);
    }
    else Serial.println(spiffs.session.exception.toString());

    // restart the loop
    Serial.println("Enter another filename");
}