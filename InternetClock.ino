
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include "DHT.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>



int pinCS = D4;
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;
char time_value[20];

// LED Matrix Pin -> ESP8266 Pin
// Vcc            -> 3v  (3V on NodeMCU 3V3 on WEMOS)
// Gnd            -> Gnd (G on NodeMCU)
// DIN            -> D7  (Same Pin for WEMOS)
// CS             -> D4  (Same Pin for WEMOS)
// CLK            -> D5  (Same Pin for WEMOS)

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 70;  // In milliseconds

int spacer = 1;
int width = 5 + spacer;  // The font width is 5 pixels

int m;

#define DHTPIN 0       // D3
#define DHTTYPE DHT11  // DHT 11
#define CD_API "/v1/bpi/currentprice.json"
#define CD_URL "api.coindesk.com"
static char respBuffer[4096];
const char* ssid = "";
const char* password = "";
WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);
String t, h, data;
 
void setup() {
  Serial.begin(9600);
  dht.begin();

  //INSERT YOUR SSID AND PASSWORD HERE

  WiFi.begin(ssid, password);

  //CHANGE THE POOL WITH YOUR CITY. SEARCH AT https://www.ntppool.org/zone/@

  configTime(0 * 3600, 0, "europe.pool.ntp.org", "time.nist.gov");

  setenv("TZ", "GMT-1BST", 1);

  matrix.setIntensity(0);    // Use a value between 0 and 15 for brightness
  matrix.setRotation(0, 1);  // The first display is position upside down
  matrix.setRotation(1, 1);  // The first display is position upside down
  matrix.setRotation(2, 1);  // The first display is position upside down
  matrix.setRotation(3, 1);  // The first display is position upside down
  matrix.fillScreen(LOW);
  matrix.write();

  while (WiFi.status() != WL_CONNECTED) {
    matrix.drawChar(2, 0, 'W', HIGH, LOW, 1);   // H
    matrix.drawChar(8, 0, 'I', HIGH, LOW, 1);   // HH
    matrix.drawChar(14, 0, '-', HIGH, LOW, 1);  // HH:
    matrix.drawChar(20, 0, 'F', HIGH, LOW, 1);  // HH:M
    matrix.drawChar(26, 0, 'I', HIGH, LOW, 1);  // HH:MM
    matrix.write();                             // Send bitmap to display
    delay(250);
    matrix.fillScreen(LOW);
    matrix.write();
    delay(250);
  }
}

void loop() {
  m = map(analogRead(0), 0, 1023, 0, 12);
  matrix.setIntensity(m);
  matrix.fillScreen(LOW);
  time_t now = time(nullptr);
  String time = String(ctime(&now));
  time.trim();
  Serial.println(time);
  time.substring(11, 19).toCharArray(time_value, 10);
  matrix.drawChar(2, 0, time_value[0], HIGH, LOW, 1);   // H
  matrix.drawChar(8, 0, time_value[1], HIGH, LOW, 1);   // HH
  matrix.drawChar(14, 0, time_value[2], HIGH, LOW, 1);  // HH:
  matrix.drawChar(20, 0, time_value[3], HIGH, LOW, 1);  // HH:M
  matrix.drawChar(26, 0, time_value[4], HIGH, LOW, 1);  // HH:MM
  matrix.write();                                       // Send bitmap to display

  delay(20000);

  matrix.fillScreen(LOW);
  h = (String)(int)dht.readHumidity();
  t = (String)(int)dht.readTemperature();
  data = getData();
  display_message(t + "C " + h + "% " + "BTC:$"+ data);
}

String getData() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(CD_URL, httpPort)) {
    Serial.println("Connection failed");
    String result = "Connection failed";
    return result;
  }
  Serial.print("Requesting URL: ");
  Serial.println(CD_URL);
  client.print(String("GET ") + CD_API + " HTTP/1.1\r\n" +
               "Host: " + CD_URL + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(100);
  String answer;
  while(client.available()){
    String line = client.readStringUntil('\r');
    answer += line;
  }
  
  client.stop();
  Serial.println();
  Serial.println("closing connection");

  String jsonAnswer;
  int jsonIndex;
  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{') {

      jsonIndex = i;
    
      break;

    }
  }
  jsonAnswer = answer.substring(jsonIndex);
  Serial.println();
  Serial.println("JSON answer: ");
  Serial.println(jsonAnswer);
  jsonAnswer.trim();
  int rateIndex = jsonAnswer.indexOf("rate_float");
  String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 18);
  priceString.trim();
  float price = priceString.toFloat();
  String result = (String)(float)price;
  Serial.print("BTC (USD): $");
  Serial.println(result);
  return result;
}

void display_message(String message) {
  for (int i = 0; i < width * message.length() + matrix.width() - spacer; i++) {
    //matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2;  // center the text vertically
    while (x + width - spacer >= 0 && letter >= 0) {
      if (letter < message.length()) {
        matrix.drawChar(x, y, message[letter], HIGH, LOW, 1);  // HIGH LOW means foreground ON, background off, reverse to invert the image
      }
      letter--;
      x -= width;
    }
    matrix.write();  // Send bitmap to display
    delay(wait / 2);
  }
}