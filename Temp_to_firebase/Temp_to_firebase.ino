#include <WiFi.h>
#include <FirebaseESP32.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

#define FIREBASE_HOST "tris-46e5f.firebaseio.com"
#define FIREBASE_AUTH "TfZnOyBGJ3AupSan1WXbXFl13xDvCioT4J4q0cI9"
#define WIFI_SSID "pulkit"
#define WIFI_PASSWORD "pulkit123"
#define SDA_PIN 21
#define SCL_PIN 22

FirebaseData firebaseData;
MPU6050 mpu6050(Wire);

void setup() {
  
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Wire.begin();
  mpu6050.begin();
  
}

void loop() {
  mpu6050.update();
  int temp=round(mpu6050.getTemp());
  Serial.print("temp : ");Serial.println(mpu6050.getTemp());
  
  if(Firebase.setInt(firebaseData, "/Temp", temp))
  {
    //Success
     Serial.println("Set int data success");

  }else{
    //Failed?, get the error reason from firebaseData

    Serial.print("Error in setInt, ");
    Serial.println(firebaseData.errorReason());
  }
}
