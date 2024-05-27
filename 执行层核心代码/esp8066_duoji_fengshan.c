#include <ESP8266WiFi.h>
#include <Servo.h>

const char ssid[] = "USER_509";      // WiFi��
const char pass[] = "3643731ZSC";    // WiFi����
WiFiClient client;                   // ����һ��WiFiClient����
const char* server = "192.168.43.94";   // ��������IP��ַ
const int port = 8888;                // �������Ķ˿ں�
Servo myServo;  // ����Servo����������
int pos = 0;    // �Ƕȴ洢����

// ��ʼ��
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

// ��ѭ��
void loop()
{
  Serial.println("hello esp8266");
  delay(1000);
  // ���WiFi�����������������ӷ�����
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(server);
    // ������ӳɹ�������HTTP����
    if (client.connect(server, port)) {
      Serial.println("Connected to server");
      // ����GET�����������������FENSHAN��״̬
      client.println("GET_STATE_FENGSHAN&fengshan_ready");
      delay(1000);
      client.println("GET_STATE_CHUANGLIAN&chuanglian_ready");
      // �ȴ�����������Ӧ
      while (client.connected() || client.available()) {
        // ��������ݿɶ�����ȡ������
        if (client.available()) {
          String line = client.readStringUntil('\n');
          Serial.println(line);
          // ���������FENSHAN��״̬������״̬����FENSHAN�ƣ�������������ͷ���
          if (line.startsWith("FENGSHAN:")) {
            int fengshanState = line.substring(9).toInt();
            Serial.print("FENGSHAN state: ");
            Serial.println(fengshanState);
            if (fengshanState == 2) {
              // Full speed forward
              analogWrite(D1, 255); // ����PWMռ�ձ�Ϊ���ֵ
              analogWrite(D2, 0);   // ֹͣGPIO4��PWM���
              client.println("FENGSHAN state:2");
            }
            else if (fengshanState == 1) {
              // Half speed forward
              analogWrite(D1, 128); // ����PWMռ�ձ�Ϊһ��
              analogWrite(D2, 0);   // ֹͣGPIO4��PWM���
              client.println("FENGSHAN state:1");
            }
            else if (fengshanState == 0) {
              // Stop
              analogWrite(D1, 0); // ֹͣGPIO5��PWM���
              analogWrite(D2, 0); // ֹͣGPIO4��PWM���
              client.println("FENGSHAN state:0");
            }
          }
          else if (line.startsWith("CHUANGLIAN:")) {
            int chuanglianState = line.substring(11).toInt();
            Serial.print("CHUANGLIAN state: ");
            Serial.println(chuanglianState);
            if (chuanglianState == 1) {
              // To 0��
              myServo.write(0);
              client.println("CHUANGLIAN state:1");
              delay(1000);
            }
            else if (chuanglianState == 2) {
              // To 90��
              myServo.write(90);
              client.println("CHUANGLIAN state:2");
              delay(1000);
            }
            else if (chuanglianState == 0) {
              // To 180��
              myServo.write(180);
              client.println("CHUANGLIAN state:0");
              delay(1000);
            }
          }
        }
      }
      // �Ͽ�����
      client.stop();
      Serial.println("Disconnected from server");
    }
    else {
      // �������ʧ�ܣ���ӡ������Ϣ
      Serial.println("Connection failed");
    }
  }
  else {
    // ���WiFi���ӶϿ�����ӡ������Ϣ
    Serial.println("WiFi disconnected");
  }
}

// ��ʼ��WIFI
void initWiFi()
{
  Serial.print("Connecting WiFi...");
  WiFi.mode(WIFI_STA); // ����WIFIΪStationģʽ
  // ���þ�̬IP��ַ�����أ��������룬DNS
  IPAddress local_IP(192, 168, 43, 22);
  IPAddress gateway(192, 168, 43, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);
  IPAddress secondaryDNS(8, 8, 4, 4);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, pass); // ����WIFI�ȵ��ssid������
  while (WiFi.status() != WL_CONNECTED) // �ȴ����ӳɹ�
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // ��ӡ�Լ���IP��ַ
}

