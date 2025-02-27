#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// **Thông tin WiFi Hotspot**
const char* ssid = "son";  
const char* password = "123456789";  

// **Địa chỉ server Node.js**
const char* serverUrl = "http://192.168.137.1:3000/data";
const char* SENSOR_API_ENDPOINT = "/api/sensor";

// **Khai báo cảm biến**
#define smokeSensorPin 36  
#define gasSensorPin 34    
#define infraredSensorPin 35  
#define buzzerPin 25       
#define dhtPin 27          
#define buttonPin 32       

#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

// **Ngưỡng cảnh báo**
int smokeThreshold = 3000;  
int gasThreshold = 3000;    
int infraredThreshold = 600;  
int tempThreshold = 50;  // Cảnh báo nhiệt độ cao

bool manualAlarm = false;  

// **Kết nối WiFi**
void connectWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Đang kết nối WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi đã kết nối!");
    Serial.print("Địa chỉ IP: ");
    Serial.println(WiFi.localIP());
}

// **Gửi dữ liệu lên server**
void sendDataToServer(int smoke, int gas, int infrared, float temp, float humidity, bool alarm) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");

        String jsonData = "{";
        jsonData += "\"smoke\":" + String(smoke) + ",";
        jsonData += "\"gas\":" + String(gas) + ",";
        jsonData += "\"infrared\":" + String(infrared) + ",";
        jsonData += "\"temperature\":" + String(temp) + ",";
        jsonData += "\"humidity\":" + String(humidity) + ",";
        jsonData += "\"alarm\":" + String(alarm ? "true" : "false");
        jsonData += "}";

        int httpResponseCode = http.POST(jsonData);
        Serial.print("Gửi dữ liệu lên server, mã phản hồi: ");
        Serial.println(httpResponseCode);
        http.end();
    } else {
        Serial.println("WiFi không kết nối!");
    }
}

// **Xử lý ngắt nút bấm**
void IRAM_ATTR handleButtonPress() {
    manualAlarm = !manualAlarm;  
    digitalWrite(buzzerPin, manualAlarm ? HIGH : LOW);
}

void setup() {
    Serial.begin(115200);
    connectWiFi();

    pinMode(smokeSensorPin, INPUT);
    pinMode(gasSensorPin, INPUT);
    pinMode(infraredSensorPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    
    attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
    
    dht.begin();
}

void loop() {
    int smokeValue = analogRead(smokeSensorPin);
    int gasValue = analogRead(gasSensorPin);
    int infraredValue = analogRead(infraredSensorPin);
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    bool fireDetected = (smokeValue > smokeThreshold && gasValue > gasThreshold) ||
                        (smokeValue > smokeThreshold && infraredValue < infraredThreshold) ||
                        (gasValue > gasThreshold && infraredValue < infraredThreshold);

    if (fireDetected || manualAlarm) {
        digitalWrite(buzzerPin, HIGH);
    } else {
        digitalWrite(buzzerPin, LOW);
    }

    Serial.printf("Khói: %d | Gas: %d | Hồng ngoại: %d | Nhiệt độ: %.2f°C | Độ ẩm: %.2f%% | Báo động: %s\n",
                  smokeValue, gasValue, infraredValue, temperature, humidity, (manualAlarm ? "ON" : "OFF"));

    sendDataToServer(smokeValue, gasValue, infraredValue, temperature, humidity, fireDetected || manualAlarm);

    delay(2000);
}
