# CRYPTO CURRENCY TICKERS

## Introduction

Here is my source code for this project. With my code:
<p>
Bitcoin Ticker is Arduino code for ESP32 boards. This ticker displays the price of Bitcoin/USD, Ethereum/USD,... and idicates whether the crypto currency went up or down. The price data comes from the Coingecko API.

# Requirements
## Hardware:

![Components-Required-768x432](https://github.com/user-attachments/assets/e9a152f1-a4ec-4885-b548-0d71d6c21139)


 - ESP32 - Wroom Development Board
 - Màn hình oled SSD1306 (I2C OLED Display)
 - 5mm Red & Green LEDs
 - Volume xoay rotary
   
# Wiring Diagram

![image](https://github.com/user-attachments/assets/4b20a8dd-c46c-48e2-bfa2-429b2fef8cb2)

## Library:
 - Adafruit_SSD1306 (For the OLED Display)
 - WiFi (For ESP32 onboard WiFi)
 - Wire
 - HTTPClient (To make requests to Coingecko)
 - NTPClinet (Used to get the time for "Last Updated")
 - WiFiUdp (Used to get the time for "Last Updated")
 - ArduinoJson (Used to process JSON data retreived from CoinMarketCap)
 - Wifi Manager for Esp32

# Usage:
 - STEP 1: Lắp các thiết bị phần cứng theo sơ đồ diagram ở bên trên
 - STEP 1: Kết nối ESP32 vào máy tính của bạn qua 1 chiếc cáp sạc
 - STEP 2: Cài đặt drive tương ứng với ESP32 của bạn (có thể là CP210x, CH340,...)
 - STEP 3: Cài đặt Arduino IDE from website https://www.arduino.cc/en/software, sau đó cài đặt Board ESP32 từ BOARD MANAGER. Xong rồi cài thêm các thư viện được liệt kê bên trên từ LIBRARY MANAGER
   
![image](https://github.com/user-attachments/assets/15a506d2-a7bf-41f9-990d-bb82d9b5d6ce)
![image](https://github.com/user-attachments/assets/f61d31a9-ab18-4601-a67f-a017a1f02ea9)

 - STEP 4: Mở device manager và kiểm tra xem port ở vị trí nào (bằng cách ấn chuột phải vào nút WINDOWN sau đó chọn divice manager).
 
 - STEP 5: Sau đó tải đoạn code của tôi về, mở file crypto_price_tickers.ino bằng Arduino IDE sau đó ấn vào tool chọn board esp32 (theo đúng loại esp32 bạn có) rồi chọn port chính là số port vừa tìm thấy tại device manager bước trên
 - STEP 6: Ấn upload để tải lên đoạn code => như vậy là đã hoàn thành chương trình đã có thể chạy
# Support You
Whatever you need; questions answered, requests, bugs; make an Issue. I'll get to them as soon as I can.
