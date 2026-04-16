#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int screen_width = 128; // OLED display width, in pixels
int screen_height = 64; // OLED display height, in pixels

int button_pin_one = 12;
int button_state_one = 0;
int button_pin_two = 11;
int button_state_two = 0;
int poop_count = 0;
int pee_count = 0;
int litter_box_visits_today = 0;

Adafruit_SSD1306 lcd(128, 64, &Wire, -1);

void setup() {
  screen_settings();
  lcd.clearDisplay();
  lcd.display();
  
  Serial.begin(115200);
  pinMode(button_pin_one, INPUT_PULLUP); 
  pinMode(button_pin_two, INPUT_PULLUP);           
}

void push_button() {
  lcd.clearDisplay();
  lcd.setCursor(0,0);
  lcd.println("Leila Potty Tracker");
  lcd.println("Visits Today: " + String(litter_box_visits_today));
  button_state_one = digitalRead(button_pin_one);
  button_state_two = digitalRead(button_pin_two);
  if (button_state_one == LOW) {  
    delay(500); 
    poop_count += 1;
    lcd.display();                  // Show on screen
  }
  if (button_state_two == LOW){
    delay(500);
    pee_count += 1;
  }
  lcd.println("Last Poop: " + String(poop_count));
  lcd.println("Last Pee: " + String(pee_count));
  lcd.display();  
  return "";
}

void screen_settings(){
  if (!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //This is needed to intiate and turn screen on.
  for (;;); // Halt if screen isn't found
}
  lcd.setTextSize(1);             // Normal 1:1 pixel scale
  lcd.setTextColor(SSD1306_WHITE);// Draw white text
  lcd.setCursor(0,0);             // Start at top-left x,y value
  
}

void loop() {
  push_button();
  
}
