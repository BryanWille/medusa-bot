#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <HTTPClient.h>

// -- Configurações Iniciais -- //

// Wifi
const char *ssid = "AndroidAP7819";
const char *password = "0000aaaa";
// Google
const String googleApiKey = "AIzaSyBgCIKA1opotVstmkqtAOagYu3eGlF5faM";
// Telegram
String BOTtoken = "6293037693:AAHxrA2DF4sOZuWZ66Gb6MTGaPC8fXlzYf8" String CHAT_ID = "-1001885412723";
WiFiClientSecure clientTCP;
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
UniversalTelegramBot bot(BOTtoken, clientTCP);

// Motor
static const int servoPin = 14;
Servo servo1;

// Camera
bool sendPhoto = false;
#define FLASH_LED_PIN 4
bool flashState = LOW;

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

void setup()
{
  servo1.attach(servoPin);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  // Init Serial Monitor
  Serial.begin(115200);

  // Set LED Flash as output
  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState);

  // Config and init the camera
  configInitCamera();

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (sendPhoto)
  {
    Serial.println("Preparing photo");
    for (int i = 0; i < 3; i++)
    {
      // Tirar Foto
      camera_fb_t * fb = tirarFoto();
      // Classificar Imagens na Foto
      String rotulos = classificarRotulos(fb, fb->len);
      // Traduzir Classificação
      String traducao = traduzirTexto(rotulos, "pt-br");
      // Enviar a Foto para o Chat
      enviarFotoTelegram(fb);
      // Enviar os Rótulos no Chat;
      enviarRotulosTelegram(traducao);
      // Rotacionar
      rotacionar(i * 90);
    }
    rotacionar(-1);
    sendPhoto = false;
  }
  if (millis() > lastTimeBotRan + botRequestDelay)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

//-- Câmera --//

void configInitCamera()
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_LATEST;

  // init with high specs to pre-allocate larger buffers
  if (psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10; // 0-63 lower number means higher quality
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12; // 0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  // Drop down frame size for higher initial frame rate
  sensor_t *s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF); // UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
}

camera_fb_t *tirarFoto()
{

  String getAll = "";
  String getBody = "";

  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb)
  {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
    return null;
  }
  return fb;
}

//-- Motor --//

void rotacionar(int angulo)
{
  if (angulo >= 0)
  {
    for (int i = angulo; i <= angulo+90; i++)
    {
      servo1.write(i);
      delay(20);
    }
  }
  else
  {
    for(int i = 180; i >= 0; i--) {
        servo1.write(i);
        delay(20);
    }
  }
}

//-- Telegram --//

void handleNewMessages(int numNewMessages)
{
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID)
    {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;
    if (text == "/aoc3")
    {
      String welcome = "Welcome , " + from_name + "\n";
      welcome += "Use the following commands to interact with the ESP32-CAM \n";
      welcome += "/photo : takes a new photo\n";
      welcome += "/flash : toggles flash LED \n";
      bot.sendMessage(CHAT_ID, welcome, "");
    }
    if (text == "/flash")
    {
      flashState = !flashState;
      digitalWrite(FLASH_LED_PIN, flashState);
      Serial.println("Change flash LED state");
    }
    if (text == "/photo")
    {
      sendPhoto = true;
      Serial.println("New photo request");
    }
  }
}

String enviarRotulosTelegram(String rotulos)
{
  String payload = "{";
  payload += "\"chat_id\": \"" + CHAT_ID + "\", ";
  payload += "\"text\": \"" + rotulos + "\"";
  payload += "}";

  HTTPClient httpClient;
  String url = dominioTelegram + "bot/" BOTtoken + "/sendMessage";
  httpClient.begin(url);

  int httpCode = httpClient.POST(payload);

  // Check if the request was successful
  if (httpCode == HTTP_CODE_OK)
  {
    httpClient.end();

    return "Sucessfull";
  }
  else
  {
    // Close the connection
    return "API request failed with HTTP error code: " + String(httpCode);
  }
}

String enviarFotoTelegram(camera_fb_t *fb)
{
  if (clientTCP.connect(dominioTelegram, 443))
  {
    Serial.println("Connection successful");

    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + CHAT_ID + "\r\n--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomNerdTutorials--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;

    clientTCP.println("POST /bot" + BOTtoken + "/sendPhoto HTTP/1.1");
    clientTCP.println("Host: " + String(dominioTelegram));
    clientTCP.println("Content-Length: " + String(totalLen));
    clientTCP.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
    clientTCP.println();
    clientTCP.print(head);

    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n = 0; n < fbLen; n = n + 1024)
    {
      if (n + 1024 < fbLen)
      {
        clientTCP.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen % 1024 > 0)
      {
        size_t remainder = fbLen % 1024;
        clientTCP.write(fbBuf, remainder);
      }
    }

    clientTCP.print(tail);

    esp_camera_fb_return(fb);

    int waitTime = 10000; // timeout 10 seconds
    long startTimer = millis();
    boolean state = false;

    while ((startTimer + waitTime) > millis())
    {
      Serial.print(".");
      delay(100);
      while (clientTCP.available())
      {
        char c = clientTCP.read();
        if (state == true)
          getBody += String(c);
        if (c == '\n')
        {
          if (getAll.length() == 0)
            state = true;
          getAll = "";
        }
        else if (c != '\r')
          getAll += String(c);
        startTimer = millis();
      }
      if (getBody.length() > 0)
        break;
    }
    clientTCP.stop();
    Serial.println(getBody);
  }
  else
  {
    getBody = "Connected to api.telegram.org failed.";
    Serial.println("Connected to api.telegram.org failed.");
  }
  return getBody;
}

//-- Google --//

String encodeBase64(const uint8_t *data, size_t length)
{
  const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String encodedData;
  size_t encodedLength = 4 * ((length + 2) / 3);

  for (size_t i = 0; i < length; i += 3)
  {
    uint32_t chunk = (data[i] << 16) | ((i + 1 < length ? data[i + 1] : 0) << 8) | ((i + 2 < length ? data[i + 2] : 0));

    encodedData += base64Chars[(chunk >> 18) & 0x3F];
    encodedData += base64Chars[(chunk >> 12) & 0x3F];
    encodedData += base64Chars[(chunk >> 6) & 0x3F];
    encodedData += base64Chars[chunk & 0x3F];
  }

  if (length % 3 == 1)
  {
    encodedData[encodedLength - 1] = '=';
    encodedData[encodedLength - 2] = '=';
  }
  else if (length % 3 == 2)
  {
    encodedData[encodedLength - 1] = '=';
  }

  return encodedData;
}

String traduzirTexto(String text, String target_language)
{
  // Prepare the API request payload
  String payload = "{";
  payload += "\"q\": \"" + text + "\", ";
  payload += "\"target\": \"" + target_language + "\", ";
  payload += "\"key\": \"" + googleApiKey + "\"";
  payload += "}";

  // Create an HTTPClient object
  HTTPClient httpClient;

  // Set the target URL
  String url = "https://translation.googleapis.com/language/translate/v2?key=" + googleApiKey;
  httpClient.begin(url);

  int httpCode = httpClient.POST(payload);

  // Check if the request was successful
  if (httpCode == HTTP_CODE_OK)
  {
    // Read the response
    String response = httpClient.getString();

    // Close the connection
    httpClient.end();

    // Parse the JSON response
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, response);

    // Extract the label annotations
    String result = jsonDoc["responses"][0]["data"]['translations']['translatedText'].as<String>();

    return result;
  }
  else
  {
    // Close the connection
    httpClient.end();

    return "API request failed with HTTP error code: " + String(httpCode);
  }
}

String classificarRotulos(camera_fb_t *fb, size_t imageSize)
{
  // Prepare the API request payload
  String payload = "{\n";
  payload += "  \"requests\": [\n";
  payload += "    {\n";
  payload += "      \"image\": {\n";
  payload += "        \"content\": \"";
  // Encode image data as base64
  String encodedData = encodeBase64(fb->buf, fb->len);
  payload += encodedData;
  payload += "\"\n";
  payload += "      },\n";
  payload += "      \"features\": [\n";
  payload += "        {\n";
  payload += "          \"type\": \"LABEL_DETECTION\",\n";
  payload += "          \"maxResults\": 5\n";
  payload += "        }\n";
  payload += "      ]\n";
  payload += "    }\n";
  payload += "  ]\n";
  payload += "}\n";

  HTTPClient httpClient;

  String url = "https://vision.googleapis.com/v1/images:annotate?key=" + googleApiKey;
  httpClient.begin(url);

  int httpCode = httpClient.POST(payload);

  if (httpCode == HTTP_CODE_OK)
  {
    String response = httpClient.getString();

    httpClient.end();

    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, response);

    JsonArray labelAnnotations = jsonDoc["responses"][0]["labelAnnotations"].as<JsonArray>();
    String result;
    for (JsonVariant label : labelAnnotations)
    {
      result += label["description"].as<String>() + ", ";
    }
    if (result.endsWith(", "))
    {
      result = result.substring(0, result.length() - 2);
    }

    return result;
  }
  else
  {
    httpClient.end();

    return "API request failed with HTTP error code: " + String(httpCode);
  }
}
