#include <dht11.h>
#include <ESP32_Servo.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Wire.h>

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Router Parameters (Access Point) ****************************/
#define WLAN_SSID "Dachis pixel"
#define WLAN_PASS "ggezs123"

/************************* Adafruit.io Broker Parameters ************************************/
#define AIO_SERVER "io.adafruit.com"               // Broker address
#define AIO_SERVERPORT 1883                        // Server port
#define AIO_USERNAME "Chota"                       // MQTT username
#define AIO_KEY "aio_iuOS45ZMYcKuoKLT0vxrnd4HwIwC"  // MQTT key

/************ MQTT Client Connection to Broker ******************/
WiFiClient client;

// Create MQTT client
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// MQTT Topics for publishing data
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/tempSensor");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish waterLevel = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/waterLevel");

// MQTT Topics for subscribing to commands
Adafruit_MQTT_Subscribe tempSetpoint = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/tempSetpoint");
Adafruit_MQTT_Subscribe manualControl = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/manual control");
Adafruit_MQTT_Subscribe lightSwitch = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lightSwitch");
Adafruit_MQTT_Subscribe heaterSwitch = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/heaterSwitch");
Adafruit_MQTT_Subscribe pumpSwitch = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/pumpSwitch");
Adafruit_MQTT_Subscribe addNutrients = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/addNutrients");

// State variables
bool manualSwitch = 0;
bool lightState = 0;
bool heaterState = 0;
bool pumpState = 0;
uint8_t setpoint_Temp = 30;
bool buttonState = 0;

// OLED Display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Thermistor parameters
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 5
#define BCOEFFICIENT 5500
#define SERIESRESISTOR 10000

// Pin definitions
#define heaterPin 12
#define pumpPin 2
#define lightPin 13
#define trigPin1 9
#define echoPin1 A0
#define trigPin2 3
#define echoPin2 1
#define DHT11PIN 14
#define servoPin 15

// Create display object
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Sensor and actuator objects
dht11 DHT11;
Servo servo;
byte servoState = 0;

// Variables for thermistor
int samples[NUMSAMPLES];
float average;
float steinhart;
float Temp1;

// Variables for DHT sensor
float dhtTemp;
float dhtHum;

// Variables for ultrasonic sensor
float duration;
float distance;
float waterLvl;
float minDistance = 4;

// Light control timing
unsigned long previousMillis = 0;
unsigned long dayTime = 1000 * 3600 * 12;
unsigned long nightTime = 1000 * 3600 * 12;
bool state;

//***************************************** Setup *****************************************
void setup() {
  Serial.begin(115200);

  // Initialize pins
  pinMode(heaterPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  // Attach servo motor
  servo.attach(servoPin);

  // Initialize OLED display
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Infinite loop if OLED fails
  }

  delay(2000);          // Wait for display to initialize
  oled.clearDisplay();  // Clear the display

  oled.setTextSize(2);       // Set text size
  oled.setTextColor(WHITE);  // Set text color
  oled.setCursor(0, 0);      // Set cursor position
  oled.println("tirili");    // Display text
  oled.display();            // Update OLED

  // Connect to WiFi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Subscribe to MQTT topics
  mqtt.subscribe(&tempSetpoint);
  mqtt.subscribe(&manualControl);
  mqtt.subscribe(&lightSwitch);
  mqtt.subscribe(&heaterSwitch);
  mqtt.subscribe(&pumpSwitch);
}

//***************************************** Loop *****************************************
void loop() {
  MQTT_connect();  // Ensure MQTT connection is alive

  oled.clearDisplay();

  Serial.print("tirili");

  // Read data from DHT11 sensor
  int chk = DHT11.read(DHT11PIN);
  dhtTemp = (float)DHT11.temperature;
  dhtHum = (float)DHT11.humidity;

  // Display temperature on OLED
  oled.setCursor(0, 0);
  oled.print("Temp:");
  oled.print(dhtTemp, 1);
  oled.println("C");

  // Display humidity on OLED
  oled.setCursor(0, 20);
  oled.print("Hum:");
  oled.print(dhtHum, 1);
  oled.println("%");

  // Calculate water level using ultrasonic sensor
  waterLvl = (30 - getDistance(trigPin1, echoPin1)) / 30;
  oled.setCursor(0, 40);
  oled.print("water:");
  oled.print(waterLvl);
  oled.println("%");
  oled.display();

  delay(3000);

  getSubscribtion(); // Check and handle incoming MQTT subscriptions

  // Publish sensor readings to Adafruit.io
  temperature.publish(dhtTemp);
  humidity.publish(dhtHum);
  waterLevel.publish(waterLvl);

  // Control devices based on manual or automatic mode
  if (manualSwitch) {
    digitalWrite(lightPin, lightState);
    digitalWrite(heaterPin, heaterState);
    digitalWrite(pumpPin, pumpState);
  } else {
    tempControl();
    lightControl();
    pumpControl();
  }
  
  nutrientsControl();
}

//***************************************** Functions *****************************************

// Ensure connection to MQTT broker
void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 10;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      while (1); // Infinite loop if connection fails
    }
  }
}

// Handle incoming MQTT messages
void getSubscribtion() {
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &manualControl) {
      if (strcmp((char *)manualControl.lastread, "auto") == 0) {
        manualSwitch = 0;
      } else if (strcmp((char *)manualControl.lastread, "manual") == 0) {
        manualSwitch = 1;
      }
    }
    if (subscription == &lightSwitch) {
      if (strcmp((char *)lightSwitch.lastread, "OFF") == 0) {
        lightState = 0;
      } else if (strcmp((char *)lightSwitch.lastread, "ON") == 0) {
        lightState = 1;
      }
    }
    if (subscription == &heaterSwitch) {
      if (strcmp((char *)heaterSwitch.lastread, "OFF") == 0) {
        heaterState = 1;
      } else if (strcmp((char *)heaterSwitch.lastread, "ON") == 0) {
        heaterState = 0;
      }
    }
    if (subscription == &pumpSwitch) {
      if (strcmp((char *)pumpSwitch.lastread, "OFF") == 0) {
        pumpState = 0;
      } else if (strcmp((char *)pumpSwitch.lastread, "ON") == 0) {
        pumpState = 1;
      }
    }
    if (subscription == &tempSetpoint) {
      setpoint_Temp = atoi((char *)tempSetpoint.lastread);
    }
    if (subscription == &addNutrients) {
      if (strcmp((char *)pumpSwitch.lastread, "1") == 0) {
        servoState++;
      }
    }
  }
}
