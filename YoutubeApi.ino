
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <YoutubeApi.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//------- Replace the following! ------
char ssid[] = "your_network";       // your network SSID (name)
char password[] = "your_passkey";  // your network key
#define API_KEY "your_google_api"  // your google apps API Token
#define CHANNEL_ID "your_channel_id" // makes up the url of channel 

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

long subs = 0;
String chanelName = "Your_Channel_name";

unsigned long timeBetweenRequests = 3000;
unsigned long nextRunTime;

void setup(){
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
    // Clear the buffer
  display.clearDisplay();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,15);
  display.println("WELCOME TO");
  display.setTextSize(2);
  display.setCursor(5,35);
  display.println("TECHIESMS   STUDIO");
  display.display();

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);

  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  
  // Required if you are using ESP8266 V2.5 or above
  client.setInsecure();
  
  // If you want to enable some extra debugging
  api._debug = true;

}

void loop() {
  if (millis() > nextRunTime)  {

    if(api.getChannelStatistics(CHANNEL_ID))

    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // SUBSCRIBERS
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(5, 3);
      display.print("SUBSCRIBER:");
      display.println(api.channelStats.subscriberCount);
      display.setCursor(0, 50);
      display.println(chanelName);
      display.display();
      delay(timeBetweenRequests);
      display.clearDisplay();
      // VIEWS
      display.setTextSize(2);
      display.setCursor(5, 3);
      display.println("VIWES:");
      display.print(api.channelStats.viewCount);
      display.setCursor(0, 50);
      display.println(chanelName);
      display.display();
      delay(timeBetweenRequests);
      display.clearDisplay();
      // VIDEOS POSTED
      display.setTextSize(2);
      display.setCursor(5, 3);
      display.print("VIDEO POSTED:");
      display.print(api.channelStats.videoCount);
      display.setTextSize(2);
      display.setCursor(0, 50);
      display.println(chanelName);
      display.display();
      delay(timeBetweenRequests);

    }

    nextRunTime = millis() + timeBetweenRequests;

  }
}
