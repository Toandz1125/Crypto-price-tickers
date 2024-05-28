#include <Adafruit_SSD1306.h>                                                 //Include the required libraries
#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <NTPClient.h> 
#include <WiFiUdp.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128                                                      //Define the OLED display width and height
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1                                                     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C                                                   //I2C address for display
#define upLED 13
#define downLED 12
Adafruit_SSD1306 display (SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);    //Create the display object

const char* ssid = "WiFi Name";                                               //Set your WiFi network name and password
const char* password = "Password";

const int httpsPort = 443;
const String url = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=USD&include_24hr_change=true";                                                    //Bitcoin price API powered by CoinGecko
const String url_1 = "https://api.coingecko.com/api/v3/simple/price?ids=ethereum&vs_currencies=USD&include_24hr_change=true";
const String url_2 = "https://api.coingecko.com/api/v3/simple/price?ids=binancecoin&vs_currencies=USD&include_24hr_change=true";
const String url_3 = "https://api.coingecko.com/api/v3/simple/price?ids=dogecoin&vs_currencies=USD&include_24hr_change=true";

WiFiClient client;                                                            //Create a new WiFi client
HTTPClient http;

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

void setup() 
{
  Serial.begin(9600);                                                       //Start the serial monitor
  
  pinMode(upLED, OUTPUT);                                                     //Define the LED pin outputs
  pinMode(downLED, OUTPUT);

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
}

void loop() 
{
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

  String BTCUSDPrice = doc["bitcoin"]["usd"].as<String>();                              //Store crypto price and update date in local variables
  float USD24hrFloat = doc["bitcoin"]["usd_24h_change"].as<float>();
  String USDChange = String(USD24hrFloat, 3);
  http.end();

  Serial.print("BTCUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(BTCUSDPrice.toDouble());

  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(USDChange);
  
  bool isUp = USD24hrFloat > 0;                                                         //Check whether price has increased or decreased
  double percentChange;
  String dayChangeString = "24hr. Change: "; 
  if (isUp)                                                                             //If price has increased from yesterday
  {
    digitalWrite(upLED, HIGH);
    digitalWrite(downLED, LOW);
  } 
  else                                                                                  //If price has decreased from yesterday
  {
    digitalWrite(downLED, HIGH);
    digitalWrite(upLED, LOW);
  }

  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(USDChange);

  display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
printCenter("BTC/USD", 0, 0);                                                         //Display the comparison header

  display.setTextSize(2);
  printCenter("$" + BTCUSDPrice, 0, 25);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  dayChangeString = dayChangeString + USDChange + "%";
  printCenter(dayChangeString, 0, 55);
  display.display();                                                                    //Execute the new display

  http.end();                                                                           //End the WiFi connection
  delay(20000);
  display.clearDisplay();
  
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

  String ETHUSDPrice = doc1["ethereum"]["usd"].as<String>();                              //Store crypto price and update date in local variables
  float USD24hrFloat1 = doc1["ethereum"]["usd_24h_change"].as<float>();
  String USDChange1 = String(USD24hrFloat1, 3);
  http.end();

  Serial.print("ETHUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(ETHUSDPrice.toDouble());

  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(USDChange1);
  
  bool isUp1 = USD24hrFloat1 > 0;                                                         //Check whether price has increased or decreased
  double percentChange1;
  String dayChangeString1 = "24hr. Change: "; 
  if (isUp1)                                                                             //If price has increased from yesterday
  {
    digitalWrite(upLED, HIGH);
    digitalWrite(downLED, LOW);
  } 
  else                                                                                  //If price has decreased from yesterday
  {
    digitalWrite(downLED, HIGH);
    digitalWrite(upLED, LOW);
  }

  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(USDChange1);
display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
  printCenter("ETH/USD", 0, 0);                                                         //Display the comparison header

  display.setTextSize(2);
  printCenter("$" + ETHUSDPrice, 0, 25);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  dayChangeString1 = dayChangeString1 + USDChange1 + "%";
  printCenter(dayChangeString1, 0, 55);
  display.display();                                                                    //Execute the new display

  http.end();                                                                           //End the WiFi connection
  delay(20000);
  display.clearDisplay(); 


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

  String BNBUSDPrice = doc2["binancecoin"]["usd"].as<String>();                              //Store crypto price and update date in local variables
  float USD24hrFloat2 = doc2["binancecoin"]["usd_24h_change"].as<float>();
  String USDChange2 = String(USD24hrFloat2, 3);
  http.end();

  Serial.print("BNBUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(BNBUSDPrice.toDouble());

  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(USDChange2);
  
  bool isUp2 = USD24hrFloat2 > 0;                                                         //Check whether price has increased or decreased
  double percentChange2;
  String dayChangeString2 = "24hr. Change: "; 
  if (isUp2)                                                                             //If price has increased from yesterday
  {
    digitalWrite(upLED, HIGH);
    digitalWrite(downLED, LOW);
  } 
  else                                                                                  //If price has decreased from yesterday
  {
    digitalWrite(downLED, HIGH);
    digitalWrite(upLED, LOW);
  }

  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(USDChange2);
display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
  printCenter("BNB/USD", 0, 0);                                                         //Display the comparison header

  display.setTextSize(2);
  printCenter("$" + BNBUSDPrice, 0, 25);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  dayChangeString2 = dayChangeString2 + USDChange2 + "%";
  printCenter(dayChangeString2, 0, 55);
  display.display();                                                                    //Execute the new display

  http.end();                                                                           //End the WiFi connection
  delay(20000);
  display.clearDisplay();


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

  String DOGEUSDPrice = doc3["dogecoin"]["usd"].as<String>();                              //Store crypto price and update date in local variables
  float USD24hrFloat3 = doc3["dogecoin"]["usd_24h_change"].as<float>();
  String USDChange3 = String(USD24hrFloat3, 3);
  http.end();

  Serial.print("DOGEUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(DOGEUSDPrice.toDouble());

  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(USDChange3);
  
  bool isUp3 = USD24hrFloat3 > 0;                                                         //Check whether price has increased or decreased
  double percentChange3;
  String dayChangeString3 = "24hr. Change: "; 
  if (isUp3)                                                                             //If price has increased from yesterday
  {
    digitalWrite(upLED, HIGH);
    digitalWrite(downLED, LOW);
  } 
  else                                                                                  //If price has decreased from yesterday
  {
    digitalWrite(downLED, HIGH);
    digitalWrite(upLED, LOW);
  }

  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(USDChange3);
display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
  printCenter("DOGE/USD", 0, 0);                                                         //Display the comparison header

  display.setTextSize(2);
  printCenter("$" + DOGEUSDPrice, 0, 25);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  dayChangeString3 = dayChangeString3 + USDChange3 + "%";
  printCenter(dayChangeString3, 0, 55);
  display.display();                                                                    //Execute the new display

  http.end();                                                                           //End the WiFi connection
  delay(20000);
  display.clearDisplay(); 
}
void printCenter(const String buf, int x, int y)                          //Function to centre the current price in the display width
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h);                     //Calculate string width
  display.setCursor((x - w / 2) + (128 / 2), y);                          //Set cursor to print string in centre
  display.print(buf);                                                     //Display string
}