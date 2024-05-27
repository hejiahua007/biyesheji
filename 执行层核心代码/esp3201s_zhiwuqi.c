
#include <ESP8266WiFi.h>?
const char ssid[] = "USER_509";      //WiFi��
const char pass[] = "3643731ZSC";       //WiFi����
WiFiClient client; //����һ��WiFiClient����
const char* server = "192.168.43.94"; //��������IP��ַ
const int port = 8888; //�������Ķ˿ں�
const int zhiwuqiPin2 = 2; //�̵���������
const int ledPin0 = 0; //LED������
//��ʼ��
void setup()
{
  Serial.begin(115200);
  Serial.println("esp8266 test");
  initWiFi();
  pinMode(ledPin0, OUTPUT);
  digitalWrite(ledPin0, HIGH);
  pinMode(zhiwuqiPin2, OUTPUT);
  digitalWrite(zhiwuqiPin2, HIGH);
}
//��ѭ��
void loop()
{
  Serial.println("hello esp8266");
  delay(1000);
  //���WiFi�����������������ӷ�����
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(server);
    //������ӳɹ�������HTTP����
    if (client.connect(server, port)) {
      Serial.println("Connected to server");
      //����GET�����������������zhiwuqi��״̬
      client.println("GET_STATE_ZHIWUQI&zhiwuqi_ready");
      //�ȴ�����������Ӧ
      while (client.connected() || client.available()) {
        //��������ݿɶ�����ȡ������
        if (client.available()) {
          String line = client.readStringUntil('\n');
          Serial.println(line);
          //���������zhiwuqi��״̬������״̬����zhiwuqi�ƣ�������������ͷ���
          if (line.startsWith("ZHIWUQI:")) {
            int zhiwuqiState = line.substring(8).toInt();
            Serial.print("ZHIWUQI state: ");
            Serial.println(zhiwuqiState);
            if (zhiwuqiState == 2) {
              digitalWrite(ledPin0, LOW);
              digitalWrite(zhiwuqiPin2, LOW);
              delay(1000);
              digitalWrite(ledPin0, HIGH);
              digitalWrite(zhiwuqiPin2, HIGH);
            }
            else if (zhiwuqiState == 0) {
              digitalWrite(ledPin0, LOW);
              digitalWrite(zhiwuqiPin2, LOW);
              delay(1000);
              digitalWrite(ledPin0, HIGH);
              digitalWrite(zhiwuqiPin2, HIGH);
              delay(1000);
              digitalWrite(ledPin0, LOW);
              digitalWrite(zhiwuqiPin2, LOW);
              delay(1000);
              digitalWrite(ledPin0, HIGH);
              digitalWrite(zhiwuqiPin2, HIGH);
            }
          }
        }
      }
      //�Ͽ�����
      client.stop();
      Serial.println("Disconnected from server");
    }
    else {
      //�������ʧ�ܣ���ӡ������Ϣ
      Serial.println("Connection failed");
    }
  }
  else {
    //���WiFi���ӶϿ�����ӡ������Ϣ
    Serial.println("WiFi disconnected");
  }
}
//��ʼ��WIFI
void initWiFi()
{
  Serial.print("Connecting WiFi...");
  WiFi.mode(WIFI_STA); //����WIFIΪStationģʽ
  //���þ�̬IP��ַ�����أ��������룬DNS
  IPAddress local_IP(192, 168, 43, 21);
  IPAddress gateway(192, 168, 43, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);
  IPAddress secondaryDNS(8, 8, 4, 4);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, pass); //����WIFI�ȵ��ssid������
  while (WiFi.status() != WL_CONNECTED) //�ȴ����ӳɹ�
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //��ӡ�Լ���IP��ַ
}
