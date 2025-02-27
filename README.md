# Hệ Thống Cảnh Báo Cháy Dựa Trên ESP32

Đây là dự án xây dựng một hệ thống cảnh báo cháy sử dụng ESP32, tích hợp các cảm biến khói, khí gas, hồng ngoại, nhiệt Ascendancy nhiệt độ và độ ẩm (DHT11), cùng với khả năng gửi dữ liệu thời gian thực qua WiFi tới một server Node.js để giám sát từ xa.

## Tính Năng
- **Cảm biến**: 
  - Khói (Smoke Sensor)
  - Khí Gas (Gas Sensor)
  - Hồng ngoại (Infrared Sensor)
  - Nhiệt độ và Độ ẩm (DHT11)
- **Cảnh báo**: Kích hoạt còi (buzzer) khi phát hiện cháy hoặc nhấn nút bấm thủ công.
- **Kết nối WiFi**: Gửi dữ liệu cảm biến tới server qua HTTP POST.
- **Giao diện Web**: Hiển thị dữ liệu thời gian thực bằng HTML và WebSocket.

## Yêu Cầu Phần Cứng
- ESP32 Dev Module (ví dụ: ESP32-WROOM-32)
- Cảm biến khói (MQ-2 hoặc tương tự)
- Cảm biến khí gas (MQ-5 hoặc tương tự)
- Cảm biến hồng ngoại
- Cảm biến DHT11
- Còi (Buzzer)
- Nút bấm (Push Button)
- Nguồn 3.3V/5V (USB hoặc pin)

## Cấu Trúc Thư Mục
fire-alarm-system/
├── esp32/              # Code cho ESP32
│   └── fire_alarm.ino  # File Arduino cho ESP32
├── server/             # Code server Node.js
│   ├── server.js       # File chính của server
│   ├── public/         # Thư mục chứa giao diện web
│   │   └── index.html  # Giao diện HTML
│   └── package.json    # Cấu hình dependencies Node.js
└── README.md           # File này
