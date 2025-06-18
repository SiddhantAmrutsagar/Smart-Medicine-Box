#include <ESP8266WiFi.h>
#include <Wire.h>
#include <DFRobotDFPlayerMini.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

#define RTC_SDA D2 // SDA pin of RTC module connected to D2 of ESP8266
#define RTC_SCL D1 // SCL pin of RTC module connected to D1 of ESP8266

#define DFPLAYER_RX D5 // RX pin of DFPlayer Mini connected to D5 of ESP8266
#define DFPLAYER_TX D6 // TX pin of DFPlayer Mini connected to D6 of ESP8266

#define ALARM_HOUR 1  // Alarm hour (24-hour format)
#define ALARM_MINUTE 33 // Alarm minute

RTC_DS3231 rtc; // Create an instance of RTC_DS3231

SoftwareSerial dfplayerSerial(DFPLAYER_RX, DFPLAYER_TX); // Create a SoftwareSerial object for DFPlayer Mini communication
DFRobotDFPlayerMini dfplayer; // Create an instance of DFRobotDFPlayerMini

void setup() {
  Serial.begin(9600);
  
  // Initialize I2C communication
  Wire.begin(RTC_SDA, RTC_SCL);
  
  // Initialize RTC module
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Initialize SoftwareSerial for DFPlayer Mini
  dfplayerSerial.begin(9600);

  // Initialize DFPlayer Mini
  if (!dfplayer.begin(dfplayerSerial)) {
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");
    while (true);
  }

  // Connect to Wi-Fi network
  connectToWiFi();
}

void loop() {
  DateTime now = rtc.now();

  // Check if it's time for the alarm
  if (now.hour() == ALARM_HOUR && now.minute() == ALARM_MINUTE) {
    // Play alarm sound
    dfplayer.play(1);
    delay(6000); // Wait for 6 seconds before stopping the alarm
    dfplayer.stop();
  }

  delay(1000); // Check every second
}

void connectToWiFi() {
  const char* ssid = "dada";
  const char* password = "11111111";

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}