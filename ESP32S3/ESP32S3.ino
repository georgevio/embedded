void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-S3 is up and running!");
}

void loop() {
  Serial.println("Hello from ESP32-S3!");
  delay(1000);
}
