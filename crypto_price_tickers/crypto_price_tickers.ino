#include <Adafruit_SSD1306.h>                                                 //Include the required libraries
#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <NTPClient.h> 
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <String>
#include <ESP32Encoder.h>
#include <driver/rtc_io.h>
#include "esp_sleep.h"
#include "esp_system.h"

#define SCREEN_WIDTH 128                                                      //Define the OLED display width and height
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1                                                     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C                                                   //I2C address for display
#define blueled 17
#define redled 18
// Rotary Encoder Inputs
#define inputCLK 32
#define inputDT 33
#define inputSW 13
Adafruit_SSD1306 display (SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);    //Create the display object

const char* ssid = "hihihuhuhaha";                                               //Set your WiFi network name and password
const char* password = "hihihahahuhu";

const int httpsPort = 443;
const String url = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=USD&include_24hr_change=true";                                                    //Bitcoin price API powered by CoinGecko
const String url_1 = "https://api.coingecko.com/api/v3/simple/price?ids=ethereum&vs_currencies=USD&include_24hr_change=true";
const String url_2 = "https://api.coingecko.com/api/v3/simple/price?ids=binancecoin&vs_currencies=USD&include_24hr_change=true";
const String url_3 = "https://api.coingecko.com/api/v3/simple/price?ids=dogecoin&vs_currencies=USD&include_24hr_change=true";
int status=0;
int counter = 0;
int currentStateCLK;
int previousStateCLK;
unsigned long lastButtonPress = 0;
const unsigned long sleepInterval = 60000; // 1 phút
String encdir ="";

WiFiClient client;                                                            //Create a new WiFi client
HTTPClient http;
ESP32Encoder encoder;

String formattedDate;                                                         //Create variables to store the date and time
String dayStamp;
String timeStamp;

const unsigned char bitcoinLogo [] PROGMEM =                                  // 'Bitcoin Logo', 128x64px
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x03, 0xe7, 0xc0, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x03, 0xe7, 0xc0, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x02, 0x24, 0x40, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x02, 0x24, 0x40, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x02, 0x24, 0x40, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x02, 0x24, 0x40, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x02, 0x3c, 0x40, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x03, 0xfe, 0x3c, 0x7c, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x1f, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0xf0, 0x3f, 0x80, 0x60, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0xf0, 0x3f, 0xe0, 0x20, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x18, 0x30, 0x70, 0x30, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x18, 0x30, 0x10, 0x30, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x18, 0x30, 0x10, 0x30, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x18, 0x30, 0x10, 0x30, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x18, 0x30, 0x30, 0x20, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x18, 0x30, 0xe0, 0x60, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x18, 0x3f, 0xc0, 0xc0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x18, 0x00, 0x01, 0xc0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x18, 0x00, 0x00, 0xf0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x18, 0x00, 0x00, 0x38, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x18, 0x3f, 0xe0, 0x18, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x18, 0x30, 0xf8, 0x0c, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x18, 0x30, 0x1c, 0x0c, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x18, 0x30, 0x0c, 0x0c, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x18, 0x30, 0x04, 0x0c, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x18, 0x30, 0x0c, 0x0c, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x18, 0x30, 0x0c, 0x0c, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x18, 0x30, 0x38, 0x0c, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0xf0, 0x3f, 0xf0, 0x08, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1c, 0x01, 0xe0, 0x1f, 0x00, 0x18, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x01, 0x00, 0x00, 0x00, 0x30, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x0f, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x03, 0xfe, 0x3c, 0x7e, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x02, 0x3c, 0x40, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x02, 0x24, 0x40, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x02, 0x24, 0x40, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x02, 0x24, 0x40, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x02, 0x24, 0x40, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x03, 0xe7, 0xc0, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x03, 0xe7, 0xc0, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned long previousMillis = 0; // Lưu thời gian lần cuối thực hiện request
const long interval = 300000; // 5 phút (300000 milliseconds)

struct CryptoData{
  double BTCUSDPrice;
  double USDChange;
  double ETHUSDPrice;
  double ETHUSDChange;
  double BNBPrice;
  double BNBChange;
  double DOGEPrice;
  double DOGEChange;
};

CryptoData data;

void request(CryptoData &data){

  //bitcoin

  Serial.print("Connecting to ");                                                       //Display url on Serial monitor for debugging
  Serial.println(url);

  http.begin(url);
  int httpCode = http.GET();                                                            //Get crypto price from API
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, http.getString());

  if (error)                                                                            //Display error message if unsuccessful
  {
    Serial.print(F("deserializeJson Failed"));
    Serial.println(error.f_str());
    delay(2500);
    return;
  }

  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);

  data.BTCUSDPrice = doc["bitcoin"]["usd"].as<double>();                              //Store crypto price and update date in local variables
  data.USDChange = doc["bitcoin"]["usd_24h_change"].as<double>();

  http.end();

  Serial.print("BTCUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(data.BTCUSDPrice);

  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(data.USDChange);

  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(data.USDChange);

  //ethereum

 Serial.print("Connecting to ");                                                       //Display url on Serial monitor for debugging
  Serial.println(url_1);

  http.begin(url_1);
  int httpCode1 = http.GET();                                                            //Get crypto price from API
  StaticJsonDocument<256> doc1;
  DeserializationError error1 = deserializeJson(doc1, http.getString());

  if (error1)                                                                            //Display error message if unsuccessful
  {
    Serial.print(F("deserializeJson Failed"));
    Serial.println(error1.f_str());
    delay(2500);
    return;
  }

  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode1);

  data.ETHUSDPrice = doc1["ethereum"]["usd"].as<double>();                              //Store crypto price and update date in local variables
  data.ETHUSDChange = doc1["ethereum"]["usd_24h_change"].as<double>();

  http.end();

  Serial.print("ETHUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(data.ETHUSDPrice);

  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(data.ETHUSDChange);
 
  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(data.ETHUSDChange);

  //binancecoin

 Serial.print("Connecting to ");                                                       //Display url on Serial monitor for debugging
  Serial.println(url_2);

  http.begin(url_2);
  int httpCode2 = http.GET();                                                            //Get crypto price from API
  StaticJsonDocument<256> doc2;
  DeserializationError error2 = deserializeJson(doc2, http.getString());

  if (error2)                                                                            //Display error message if unsuccessful
  {
    Serial.print(F("deserializeJson Failed"));
    Serial.println(error2.f_str());
    delay(2500);
    return;
  }

  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode2);

  data.BNBPrice = doc2["binancecoin"]["usd"].as<double>();                              //Store crypto price and update date in local variables
  data.BNBChange = doc2["binancecoin"]["usd_24h_change"].as<double>();

  http.end();

  Serial.print("BNBUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(data.BNBPrice);

  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(data.BNBChange);
 
  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(data.BNBChange);

  //Dogecoin

  Serial.print("Connecting to ");                                                       //Display url on Serial monitor for debugging
  Serial.println(url_3);

  http.begin(url_3);
  int httpCode3 = http.GET();                                                            //Get crypto price from API
  StaticJsonDocument<256> doc3;
  DeserializationError error3 = deserializeJson(doc3, http.getString());

  if (error3)                                                                            //Display error message if unsuccessful
  {
    Serial.print(F("deserializeJson Failed"));
    Serial.println(error3.f_str());
    delay(2500);
    return;
  }

  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode3);

  data.DOGEPrice = doc3["dogecoin"]["usd"].as<double>();                              //Store crypto price and update date in local variables
  data.DOGEChange = doc3["dogecoin"]["usd_24h_change"].as<double>();

  http.end();

  Serial.print("BTCUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(data.DOGEPrice);

  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(data.DOGEChange);

  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(data.DOGEChange);

}
void setup() 
{
  Serial.begin(9600);                                                       //Start the serial monitor

  pinMode(redled, OUTPUT);                                                     //Define the LED pin outputs
  pinMode(blueled, OUTPUT);
  // Set encoder pins as inputs  
  pinMode(inputCLK, INPUT_PULLUP);
  pinMode(inputDT, INPUT_PULLUP);
  pinMode(inputSW, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))                   //Connect to the display
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();                                                     //Clear the display
  display.setTextColor(SSD1306_WHITE);                                        //Set the text colour to white
  display.drawBitmap(0, 0, bitcoinLogo, 128, 64, WHITE);                             //Display bitmap from array
  display.display();
  delay(2000);

  display.clearDisplay();                                                     //Clear the display
display.setTextSize(1);                                                     //Set display parameters
  display.setTextColor(WHITE);
  display.println("Connecting to WiFi...");
  display.display();

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)                                        //Connect to the WiFi network
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  display.println("Connected to: ");                                           //Display message once connected
  display.print(ssid);
  display.display();
  delay(1500);
  display.clearDisplay();
  display.display();

  request(data);
  previousStateCLK = digitalRead(inputCLK);

}
void loop() 
{
  currentStateCLK = digitalRead(inputCLK);
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    request(data);
  }
  //led
  if(data.USDChange >= 0 && data.ETHUSDChange >= 0 && data.BNBChange >= 0 && data.DOGEChange >= 0){
    digitalWrite(blueled, HIGH);
    digitalWrite(redled, LOW);
  }
  else{
    digitalWrite(redled, HIGH);
    digitalWrite(blueled, LOW);
  }
  
  // If the previous and the current state of the inputCLK are different then a pulse has occured
  if (currentStateCLK != previousStateCLK){ 
    lastButtonPress = currentMillis;
    // If the inputDT state is different than the inputCLK state then 
    // the encoder is rotating counterclockwise
    if (digitalRead(inputDT) != currentStateCLK) { 
      counter --;
      if (counter < 0){
        counter = 3;
      }
      encdir ="CCW";  
    } else {
      // Encoder is rotating clockwise
      counter ++;
      if(counter > 3){
        counter = 0;
      }
      encdir ="CW";
    }
    Serial.print("Direction: ");
    Serial.print(encdir);
    Serial.print(" -- Value: ");
    Serial.println(counter);
    delay(100);
  }
  // Update previousStateCLK with the current state
  previousStateCLK = currentStateCLK;
  if (counter == 0){
    bitcoin();
  }
  if (counter == 1){
    ethereum();
  }
  if (counter == 2){
    binance();
  }
  if (counter == 3){
    dogecoin();
  }
  // Kiểm tra thời gian không hoạt động và đưa vào chế độ sleep
  if (currentMillis - lastButtonPress >= sleepInterval) {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);
    esp_deep_sleep_start();
  }
}
void bitcoin(){
  String dayChangeString = "24hr. Change: " + String(data.USDChange, 3) + "%"; 
  display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
printCenter("BTC/USD", 0, 0);                                                         //Display the comparison header

  display.setTextSize(2);
  printCenter("$" + String(data.BTCUSDPrice, 2), 0, 25);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  printCenter(dayChangeString, 0, 55);
  display.display();                                                                    //Execute the new display

}

void ethereum(){
  String dayChangeString1 = "24hr. Change: " + String(data.ETHUSDChange, 3)  + "%"; 
display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
  printCenter("ETH/USD", 0, 0);                                                         //Display the comparison header

  display.setTextSize(2);
  printCenter("$" + String(data.ETHUSDPrice, 2), 0, 25);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  printCenter(dayChangeString1, 0, 55);
  display.display();                                                                    //Execute the new display

}

void binance(){
  String dayChangeString2 = "24hr. Change: " + String(data.BNBChange, 3)  + "%"; 
display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
  printCenter("BNB/USD", 0, 0);                                                         //Display the comparison header

  display.setTextSize(2);
  printCenter("$" + String(data.BNBPrice, 2), 0, 25);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  printCenter(dayChangeString2, 0, 55);
  display.display();                                                                    //Execute the new display

}

void dogecoin(){
  String dayChangeString = "24hr. Change: " + String(data.DOGEChange, 3) + "%"; 
  display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
printCenter("DOGE/USD", 0, 0);                                                         //Display the comparison header

  display.setTextSize(2);
  printCenter("$" + String(data.DOGEPrice, 2), 0, 25);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  printCenter(dayChangeString, 0, 55);
  display.display();                                                                    //Execute the new display

}

void printCenter(const String buf, int x, int y)                          //Function to centre the current price in the display width
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h);                     //Calculate string width
  display.setCursor((x - w / 2) + (128 / 2), y);                          //Set cursor to print string in centre
  display.print(buf);                                                     //Display string
}