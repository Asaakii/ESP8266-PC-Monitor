#include <Arduino.h>

// ESP8266 Libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFi Setup
const char * ssid = "";      // 填写WiFi名称
const char * password = "";       // 填写WiFi密码
const char *url = "http://192.168.31.213:8085/data.json";     //填写LibreHardwareMonitor服务地址，端口默认8058
const char *hostname = "Hardware Monitor";

// JSON Library
#include <ArduinoJson.h>

//  ST7789 1.54" 240x240
#include <TFT_eSPI.h>
#include <SPI.h>


// Webserver Libraries
#include <ESP8266WebServer.h>

#define LCD_BL_PIN 5    //LCD背光引脚
TFT_eSPI tft = TFT_eSPI();  // 引脚请自行配置tft_espi库中的 User_Setup.h文件 


void setup()
{
  pinMode(LCD_BL_PIN, OUTPUT); //定义数字引脚接口LCD_BL_PIN为输出
  tft.init();//屏幕初使化
  tft.fillScreen(TFT_BLACK);//屏幕填充颜色TFT_WHITE TFT_BLACK

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // 显示正在连接到wifi
  Serial.print("\nconnecting to wifi:\n");

  // 在等待过程中，每隔1秒显示一个小点，以表示正在连接
  // 最多等待20秒，超过20秒则显示连接失败
  for (int i = 0; i < 20; i++) {
    // 检查连接状态是否已经连接上
    if (WiFi.status() == WL_CONNECTED) {
      // 显示连接成功信息, 包含从网络中获得的IP地址
      Serial.println("Wifi conntected.");

      // 然后就可以直接返回了
      return;
    }
    delay(1000);
    Serial.print(".");
  }
  
  Serial.print("\nConnect failed: timemout!\n");
}


void hardwareMonitorST7789()
{
  WiFiClient client;
  HTTPClient http;

  // Send request
  http.useHTTP10(true);
  http.begin(client, url);
  http.GET();

  // Parse response
  const size_t capacity = 89 * JSON_ARRAY_SIZE(0) + 11 * JSON_ARRAY_SIZE(1) + 6 * JSON_ARRAY_SIZE(2) + 4 * JSON_ARRAY_SIZE(3) + 3 * JSON_ARRAY_SIZE(4) + 2 * JSON_ARRAY_SIZE(5) + JSON_ARRAY_SIZE(6) + JSON_ARRAY_SIZE(7) + 2 * JSON_ARRAY_SIZE(8) + 4 * JSON_ARRAY_SIZE(9) + 123 * JSON_OBJECT_SIZE(7) + 12530;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, http.getStream(), DeserializationOption::NestingLimit(12));


  // 从json文件中获取各参数信息
  String cpuName = doc["Children"][0]["Children"][1]["Text"];
  String cpuLoad = doc["Children"][0]["Children"][1]["Children"][4]["Children"][0]["Value"];  
  String cpuTempPackage = doc["Children"][0]["Children"][0]["Children"][0]["Children"][1]["Children"][0]["Value"];
  String pumpFan = doc["Children"][0]["Children"][0]["Children"][0]["Children"][2]["Children"][1]["Value"];
  String cpuFan = doc["Children"][0]["Children"][0]["Children"][0]["Children"][2]["Children"][0]["Value"];
  String caseFan = doc["Children"][0]["Children"][0]["Children"][0]["Children"][2]["Children"][3]["Value"];
  String caseTemp = doc["Children"][0]["Children"][0]["Children"][0]["Children"][1]["Children"][1]["Value"];
  String gpuName = doc["Children"][0]["Children"][3]["Text"];
  String gpuTemp = doc["Children"][0]["Children"][3]["Children"][2]["Children"][0]["Value"];
  String gpuLoad = doc["Children"][0]["Children"][3]["Children"][3]["Children"][0]["Value"];
  String gpuFan = doc["Children"][0]["Children"][3]["Children"][4]["Children"][0]["Value"];
  String ramLoad = doc["Children"][0]["Children"][2]["Children"][0]["Children"][0]["Value"];
  String usedRAM = doc["Children"][0]["Children"][2]["Children"][1]["Children"][0]["Value"];
  String freeRAM = doc["Children"][0]["Children"][2]["Children"][1]["Children"][1]["Value"];


  String degree = degree.substring(degree.length()) + (char)247 + "C";
  String percentage = percentage.substring(percentage.length()) + (char)37;

  // Console Log
  Serial.print("\n\n\n\nCPU: ");
  Serial.println(cpuName);
  Serial.print("CPU Load: ");
  Serial.println(cpuLoad);  
  Serial.print("CPU Package: ");
  Serial.println(cpuTempPackage);
  Serial.print("CPU Fan: ");
  Serial.println(cpuFan);  
  Serial.print("GPU Name: ");
  Serial.println(gpuName);
  Serial.print("GPU Temp: ");
  Serial.println(gpuTemp);
  Serial.print("GPU Load: ");
  Serial.println(gpuLoad);
  Serial.print("Used RAM: ");
  Serial.println(usedRAM);
  Serial.print("Free RAM: ");
  Serial.println(freeRAM);

  // CPU - Name
  tft.setCursor(6, 10);
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.print("Core i5-12400F");

  // // CPU - Load （有bug）
  // tft.setCursor(10, 35);
  // tft.setTextSize(3);
  // tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // tft.print(cpuLoad.substring(0, cpuLoad.length() - 4));
  // tft.setCursor(48, 41);  
  // tft.setTextSize(2);
  // tft.print(percentage);

  // CPU - Temperature
  tft.setCursor(10, 35);
  tft.setTextSize(3);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(cpuTempPackage.substring(0, cpuTempPackage.length() - 6));
  tft.setCursor(48, 41);
  tft.setTextSize(2);  
  tft.print(degree);

  // PUMP - FanSpeed
  tft.setCursor(136, 35);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.print(pumpFan.substring(0, pumpFan.length() - 4));
  tft.setCursor(224, 41);
  tft.setTextSize(2);  
  tft.print("R");

 // CPU - FanSpeed
  tft.setCursor(136, 65);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.print(cpuFan.substring(0, cpuFan.length() - 4));
  tft.setCursor(224, 71);
  tft.setTextSize(2);  
  tft.print("R");

  // Case - Name
  tft.setCursor(10, 65);
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.print("ASUS GT502");

  // Case - Temperature
  tft.setCursor(10, 95);
  tft.setTextSize(3);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(caseTemp.substring(0, caseTemp.length() - 6));
  tft.setCursor(48, 101);
  tft.setTextSize(2);  
  tft.print(degree);

  // Case - FanSpeed
  tft.setCursor(136, 95);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.print(caseFan.substring(0, caseFan.length() - 4));
  tft.setCursor(224, 101);
  tft.setTextSize(2);  
  tft.print("R");

  // RAM - Name
  tft.setCursor(6, 130);
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.print("Asgard 16G x 2 3200");

  // RAM - Load
  tft.setCursor(10, 153);
  tft.setTextSize(3); 
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println(ramLoad.substring(0, cpuLoad.length() - 4));
  tft.setCursor(48, 159);
  tft.setTextSize(2);
  tft.print(percentage);

  // RAM - Used
  tft.setCursor(112, 153);
  tft.setTextSize(3);
  tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
  tft.println(usedRAM);

  // GPU - Name
  tft.setCursor(6, 190);  
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.print("GeForce RTX 2060s");

  // // GPU - Load （有bug）
  // tft.setCursor(10, 215);
  // tft.setTextSize(3);
  // tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // tft.print(gpuLoad.substring(0, gpuLoad.length() - 4));
  // tft.setCursor(48, 221);  
  // tft.setTextSize(2);
  // tft.print(percentage);

  // GPU - Temperature
  tft.setCursor(10, 215);
  tft.setTextSize(3);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(gpuTemp.substring(0, gpuTemp.length() - 6));
  tft.setCursor(48, 221);
  tft.setTextSize(2);
  tft.print(degree);

  // GPU - FanSpeed
  tft.setCursor(136, 215);
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.print(gpuFan.substring(0, gpuFan.length() - 4));
  tft.setCursor(224, 221);
  tft.setTextSize(2);  
  tft.print("R");

  // Disconnect
  http.end();
}


void loop() //每1秒钟刷新1次上面的那些监控数据
{
  if (WiFi.status() == WL_CONNECTED)
  {
    hardwareMonitorST7789();
  }

  delay(1000);
}
