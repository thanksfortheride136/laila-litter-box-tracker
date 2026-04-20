#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "time.h"

int screen_width = 128;
int screen_height = 64;

int button_pin_one = 12;
int button_state_one = 0;
int button_pin_two = 13;
int button_state_two = 0;

int litter_box_visits_today = 0;

String last_poop_time = "None";
String last_pee_time = "None";

Adafruit_SSD1306 lcd(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);

  pinMode(button_pin_one, INPUT_PULLUP);
  pinMode(button_pin_two, INPUT_PULLUP);

  connect_wifi();
  screen_settings();

  configTzTime("EST5EDT,M3.2.0/2,M11.1.0/2", "pool.ntp.org");

  lcd.clearDisplay();
  lcd.display();
  delay(500);
}

void connect_wifi() {
  String ssid = "The LAN Before Time";
  String password = "thereisnocowlevel";

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  Serial.println("\nConnecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

String get_time() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    return "No Time";
  }

  char time_string[30];
  strftime(time_string, sizeof(time_string), "%m/%d %I:%M %p", &timeinfo);

  return String(time_string);
}

void push_button() {
  button_state_one = digitalRead(button_pin_one);
  button_state_two = digitalRead(button_pin_two);

  if (button_state_one == LOW) {
    delay(500);
    litter_box_visits_today += 1;
    last_poop_time = get_time();
  }

  if (button_state_two == LOW) {
    delay(500);
    litter_box_visits_today += 1;
    last_pee_time = get_time();
  }

  lcd.clearDisplay();
  lcd.setCursor(0, 0);
  lcd.println("Leila Potty Tracker");
  lcd.println("Visits Today: " + String(litter_box_visits_today));
  lcd.println("Last Poop:");
  lcd.println(last_poop_time);
  lcd.println("Last Pee:");
  lcd.print(last_pee_time);
  lcd.display();
}

void screen_settings() {
  if (!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }

  lcd.setTextSize(1);
  lcd.setTextColor(SSD1306_WHITE);
  lcd.setCursor(0, 0);
}

void loop() {
  push_button();
}
