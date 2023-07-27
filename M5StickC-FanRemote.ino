#include <M5StickCPlus.h>
#include <WiFi.h>
#include "secrets.h" // Changed password after upload

#define BUILTIN_LED 10
const char* resource = "/trigger/M5StickC-FanRemote/with/key/clDLCT7yMrXZB3Ol86yKv0";
const char* server = "maker.ifttt.com";

void setup(){
  pinMode(BUILTIN_LED, OUTPUT);
  
  Serial.begin(115200);
  M5.begin();
  delay(1000);
 
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SECRET_SSID);
  
  M5.Lcd.setCursor(25,40);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  
  M5.Lcd.println("Connecting to...");
  M5.Lcd.setCursor(70,70);
  M5.Lcd.print(SECRET_SSID);

  WiFi.begin(SECRET_SSID, SECRET_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  M5.update();

  M5.Lcd.setCursor(35,20);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Max's M5-Remote"); // Change to whatever you want the name of the remote on the screen to be.

  digitalWrite(BUILTIN_LED, HIGH);
  
  if (M5.BtnA.wasPressed()) {
    makeIFTTTRequest();
    digitalWrite(BUILTIN_LED, LOW);
    Serial.println("Fan Toggled");
    M5.Lcd.setCursor(48,70);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("Plug Toggled");
  }
  delay(500);
}

void makeIFTTTRequest() {
  Serial.print("Connecting to ");
  Serial.print(server);
  WiFiClient client;
  int retries = 5;  
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
     Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.print("Failed to connect. Entering Sleep.");
  }
  Serial.print("Request Resource: ");
  Serial.println(resource);
  client.print(String("GET ") + resource + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n");

  // Time is 5 seconds worth of 100 millisecond delays.
  int timeout = 5 * 10;
  while (!!!client.available() && timeout-- > 0) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.print("No response. Entering Sleep.");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("Closing Connection...");
  client.stop();
}
