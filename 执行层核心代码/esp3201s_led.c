
#include <ESP8266WiFi.h>?
const char ssid[] = "USER_509";      //WiFi名
const char pass[] = "3643731ZSC";       //WiFi密码
WiFiClient client; //创建一个WiFiClient对象
const char* server = "192.168.43.94"; //服务器的IP地址
const int port = 8888; //服务器的端口号
const int ledPin2 = 2; //继电器的引脚
const int ledPin0 = 0; //LED的引脚
//初始化
void setup()
{
  Serial.begin(115200);
  Serial.println("esp8266 test");
  initWiFi();
  pinMode(ledPin0, OUTPUT);
  digitalWrite(ledPin0, HIGH);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, HIGH);
}
//主循环
void loop()
{
  Serial.println("hello esp8266");
  delay(1000);
  //如果WiFi连接正常，尝试连接服务器
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(server);
    //如果连接成功，发送HTTP请求
    if (client.connect(server, port)) {
      Serial.println("Connected to server");
      //发送GET请求，请求服务器发送LED的状态
      client.println("GET_STATE_LED&led_ready");
      //等待服务器的响应
      while (client.connected() || client.available()) {
        //如果有数据可读，读取并处理
        if (client.available()) {
          String line = client.readStringUntil('\n');
          Serial.println(line);
          //如果读到了LED的状态，根据状态控制LED灯，并向服务器发送反馈
          if (line.startsWith("LED:")) {
            int ledState = line.substring(4).toInt();
            Serial.print("LED state: ");
            Serial.println(ledState);
            if (ledState == 1) {
              digitalWrite(ledPin0, LOW);
              digitalWrite(ledPin2, LOW);
              //client.println("led_ready");
            }
            else if (ledState == 2) {
              digitalWrite(ledPin0, LOW);
              digitalWrite(ledPin2, LOW);
              //client.println("led_on");
            }
            else if (ledState == 0) {
              digitalWrite(ledPin0, HIGH);
              digitalWrite(ledPin2, HIGH);
              //client.println("led_off");
            }
          }
        }
      }
      //断开连接
      client.stop();
      Serial.println("Disconnected from server");
    }
    else {
      //如果连接失败，打印错误信息
      Serial.println("Connection failed");
    }
  }
  else {
    //如果WiFi连接断开，打印错误信息
    Serial.println("WiFi disconnected");
  }
}
//初始化WIFI
void initWiFi()
{
  Serial.print("Connecting WiFi...");
  WiFi.mode(WIFI_STA); //配置WIFI为Station模式
  //设置静态IP地址，网关，子网掩码，DNS
  IPAddress local_IP(192, 168, 43, 20);
  IPAddress gateway(192, 168, 43, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);
  IPAddress secondaryDNS(8, 8, 4, 4);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, pass); //传入WIFI热点的ssid和密码
  while (WiFi.status() != WL_CONNECTED) //等待连接成功
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //打印自己的IP地址
}

