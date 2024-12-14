# CRYPTO CURRENCY TICKERS
## Introduction

Đây là mã nguồn của tôi cho dự án này. Với mã của tôi:
<p>
Crypto Tickers là mã Arduino cho bo mạch ESP32. Mã này hiển thị giá Bitcoin/USD, Ethereum/USD,vv...  bạn có thể thiết lập nó để hiển thị bất kỳ loại tiền điện tử nào hoặc thậm chí nhiều loại tiền điện tử bằng bất kỳ loại tiền địa phương nào, miễn là có sẵn API cho nó trên một trong các sàn giao dịch. Mã này dữ liệu giá đến từ API Coingecko. 
<p>
Cùng với giá, nó hiển thị phần trăm thay đổi so với giá đóng cửa ngày hôm qua. Nó cũng sáng đèn LED màu đỏ hoặc xanh lục bên dưới màn hình tùy thuộc vào việc giá tăng hay giảm. 
<p>
Hỗ trợ sự thuận tiện của người dùng bằng cách sử dụng sleep mode giảm tiêu thụ pin, có thể config để lựa chọn được wifi và loại coin hiển thị mà ko cần can thiệp lại đoạn code.
<p>
Sản phẩm được hướng dẫn chi tiết cách làm và cách sử dụng ở phía bên dưới
 
# Requirements
## Hardware

![Components-Required-768x432](https://github.com/user-attachments/assets/e9a152f1-a4ec-4885-b548-0d71d6c21139)


 - ESP32 - Wroom Bo mạch phát triển
 - Màn hình oled SSD1306 (hiển thị OLED I2C)
 - 5mm Red & Green LEDs
 - Volume xoay rotary
   
# Wiring Diagram
Một điều cần lưu ý là có hai phiên bản khác nhau của những màn hình này có sẵn trên internet, một phiên bản có các chân GND và VCC ngược lại. Đảm bảo bạn kiểm tra cực trước khi cấp nguồn cho màn hình, nếu không bạn có thể làm hỏng màn hình.

![image](https://github.com/user-attachments/assets/4b20a8dd-c46c-48e2-bfa2-429b2fef8cb2)

# Library
 - Adafruit_SSD1306 (Để hiển thị OLED)
 - WiFi (Cho WiFi trên bo mạch ESP32)
 - Wire
 - HTTPClient (Để requests đến Coingecko)
 - NTPClinet (Được sử dụng để có được thời gian cho "Last Updated")
 - WiFiUdp (Được sử dụng để có được thời gian cho "Last Updated")
 - ArduinoJson (Được sử dụng để xử lý dữ liệu JSON được truy xuất từ ​​Coingecko)
 - Wifi Manager for Esp32

# Installation
 - Bước 1: Lắp các thiết bị phần cứng theo sơ đồ diagram ở bên trên
 - Bước 2: Kết nối ESP32 vào máy tính của bạn qua 1 chiếc cáp sạc
 - Bước 3: Tải và cài đặt drive tương ứng với ESP32 của bạn (có thể là CP210x, CH340,...) trên trình duyệt
 - Bước 4: Cài đặt Arduino IDE from website https://www.arduino.cc/en/software, sau đó vào file chọn preferences rồi nhập vào đường dẫn này https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json (phải làm bước này bên dưới mới tìm được Board để cài đặt)
   
 ![image](https://github.com/user-attachments/assets/a852d64a-83bf-484d-bc8f-e6a152f6a74f)

 - Bước 5: Sau đó cài đặt Board ESP32 từ BOARD MANAGER. Xong rồi cài thêm các thư viện được liệt kê bên trên từ LIBRARY MANAGER
   
![image](https://github.com/user-attachments/assets/15a506d2-a7bf-41f9-990d-bb82d9b5d6ce)
![image](https://github.com/user-attachments/assets/f61d31a9-ab18-4601-a67f-a017a1f02ea9)

 - Bước 6: Mở device manager và kiểm tra xem port ở vị trí nào (bằng cách ấn chuột phải vào nút WINDOWN sau đó chọn divice manager).
   
![image](https://github.com/user-attachments/assets/16bb1a21-76b5-4b7a-808a-fdfcf717da3f)

* Nếu máy của bạn ko có phần này. Đừng lo, hãy làm theo các bước sau (hiện rồi thì làm tiếp bước 7 nhé):
  Đầu tiên ấn chọn Action phía bên trên, chọn Add legacy hardware (bạn phải ấn vào thiết bị nào đó thì mới hiện lên phần này trong action), sau đó ấn next rồi chọn như Install the hardware that I manually select from a list
  Tiếp theo kéo xuống dưới sẽ có phần port, ấn chọn vào đó rồi ấn next => đã xuất hiện phần port (sau bước này bạn có thể khởi động lại máy và rút cáp sạc ra cắm lại esp32 để chắc chắn nó có thể hiện phần port và nhận drive)
  
![z6086645402105_d22b40e974d916db1d19b3a85f11c8ab](https://github.com/user-attachments/assets/1905a51c-357e-497d-a632-64a1269b64d2)

 - Bước 7: Sau đó tải đoạn code của tôi về, mở file crypto_price_tickers.ino bằng Arduino IDE sau đó ấn vào tool chọn board esp32 (theo đúng loại esp32 bạn có) rồi chọn port chính là số port vừa tìm thấy tại device manager bước trên
   
![image](https://github.com/user-attachments/assets/8a6d7973-965d-4585-bedd-feac6ca38df8)

 - Bước 8: Ấn upload để tải lên đoạn code => như vậy là đã hoàn thành chương trình đã có thể chạy


# Usage
- Sau khi upload xong bạn sử dụng thiết bị của mình để kết nối với wifi do esp32 phát ra
- Sau khi đã kết nối (sẽ ko có mạng), bạn vào trang 192.168.4.1 để điền wifi cho thiết bị. Ấn save và thiết bị đã có thể chạy bình thường.
  
# Support You
Bất cứ điều gì bạn cần; câu hỏi được trả lời, yêu cầu, lỗi; gặp phải một vấn đề. Liên lạc với tôi và tôi sẽ hỗ trợ ngay khi có thể.
