#include <ESP8266WiFi.h>
#include <Servo.h>

const char ssid[] = "USER_509";      // WiFi名
const char pass[] = "3643731ZSC";    // WiFi密码
WiFiClient client;                   // 创建一个WiFiClient对象
const char* server = "192.168.43.94";   // 服务器的IP地址
const int port = 8888;                // 服务器的端口号
Servo myServo;  // 定义Servo对象来控制
int pos = 0;    // 角度存储变量

// 初始化
void setup()
{
  Serial.begin(115200);
  Serial.println("esp8266 test");
  initWiFi();
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  myServo.attach(14); //D5   
}

// 主循环
void loop()
{
  Serial.println("hello esp8266");
  delay(1000);
  // 如果WiFi连接正常，尝试连接服务器
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(server);
    // 如果连接成功，发送HTTP请求
    if (client.connect(server, port)) {
      Serial.println("Connected to server");
      // 发送GET请求，请求服务器发送FENSHAN的状态
      client.println("GET_STATE_FENGSHAN&fengshan_ready");
      delay(1000);
      client.println("GET_STATE_CHUANGLIAN&chuanglian_ready");
      // 等待服务器的响应
      while (client.connected() || client.available()) {
        // 如果有数据可读，读取并处理
        if (client.available()) {
          String line = client.readStringUntil('\n');
          Serial.println(line);
          // 如果读到了FENSHAN的状态，根据状态控制FENSHAN灯，并向服务器发送反馈
          if (line.startsWith("FENGSHAN:")) {
            int fengshanState = line.substring(9).toInt();
            Serial.print("FENGSHAN state: ");
            Serial.println(fengshanState);
            if (fengshanState == 2) {
              // Full speed forward
              analogWrite(D1, 255); // 设置PWM占空比为最大值
              analogWrite(D2, 0);   // 停止GPIO4的PWM输出
              client.println("FENGSHAN state:2");
            }
            else if (fengshanState == 1) {
              // Half speed forward
              analogWrite(D1, 128); // 设置PWM占空比为一半
              analogWrite(D2, 0);   // 停止GPIO4的PWM输出
              client.println("FENGSHAN state:1");
            }
            else if (fengshanState == 0) {
              // Stop
              analogWrite(D1, 0); // 停止GPIO5的PWM输出
              analogWrite(D2, 0); // 停止GPIO4的PWM输出
              client.println("FENGSHAN state:0");
            }
          }
          else if (line.startsWith("CHUANGLIAN:")) {
            int chuanglianState = line.substring(11).toInt();
            Serial.print("CHUANGLIAN state: ");
            Serial.println(chuanglianState);
            if (chuanglianState == 1) {
              // To 0°
              myServo.write(0);
              client.println("CHUANGLIAN state:1");
              delay(1000);
            }
            else if (chuanglianState == 2) {
              // To 90°
              myServo.write(90);
              client.println("CHUANGLIAN state:2");
              delay(1000);
            }
            else if (chuanglianState == 0) {
              // To 180°
              myServo.write(180);
              client.println("CHUANGLIAN state:0");
              delay(1000);
            }
          }
        }
      }
      // 断开连接
      client.stop();
      Serial.println("Disconnected from server");
    }
    else {
      // 如果连接失败，打印错误信息
      Serial.println("Connection failed");
    }
  }
  else {
    // 如果WiFi连接断开，打印错误信息
    Serial.println("WiFi disconnected");
  }
}

// 初始化WIFI
void initWiFi()
{
  Serial.print("Connecting WiFi...");
  WiFi.mode(WIFI_STA); // 配置WIFI为Station模式
  // 设置静态IP地址，网关，子网掩码，DNS
  IPAddress local_IP(192, 168, 43, 22);
  IPAddress gateway(192, 168, 43, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);
  IPAddress secondaryDNS(8, 8, 4, 4);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, pass); // 传入WIFI热点的ssid和密码
  while (WiFi.status() != WL_CONNECTED) // 等待连接成功
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // 打印自己的IP地址
}

