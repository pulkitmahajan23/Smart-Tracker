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
#define BTN_PIN 27

FirebaseData firebaseData;
MPU6050 mpu6050(Wire);

RTC_DATA_ATTR bool tamperStatus=false;
esp_sleep_wakeup_cause_t wakeup_reason;
void setup() {
  
  Serial.begin(115200);
  pinMode(BTN_PIN,INPUT);

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

  wakeup_reason = esp_sleep_get_wakeup_cause();
  if(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER)
  {
    tamperStatus=true;
  }
}

void loop() {
  mpu6050.update();
  float temp=mpu6050.getTemp();
  Serial.print("temp : ");Serial.println(mpu6050.getTemp());
  if(Firebase.set(firebaseData, "/Temp", temp))
  {
    //Success
     Serial.println("Set int data success");

  }
  else{
    Serial.print("Error in setting");
    Serial.println(firebaseData.errorReason());
  }
  if(Firebase.setBool(firebaseData,"/TamperStatus",tamperStatus))
  {
     Serial.println("Set int data success");
  }
  else{
    Serial.print("Error in setting");
    Serial.println(firebaseData.errorReason());
  }
  if(!digitalRead(BTN_PIN))
  {
    esp_sleep_enable_timer_wakeup(0);
    esp_deep_sleep_start();
  }
} 
