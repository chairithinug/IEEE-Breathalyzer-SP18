#include<LiquidCrystal.h>

volatile bool detect = false;

#define BUTTON_PIN 2

// Buzzer Components
#define BUZZER_PIN 7
#define PITCH 860
#define DURATION 50

// LCD Components
#define RS 12
#define EN 11
#define D4 6
#define D5 5
#define D6 4
#define D7 3

// MQ3 Components
#define SENSOR_PIN A5
#define WARMUP_DURATION 60
#define SOBER 200
#define DRUNK 300

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  mq3_init();
  
  tone(BUZZER_PIN, PITCH, DURATION);
  delay(500);
  tone(BUZZER_PIN, PITCH, DURATION);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isButtonPressed, FALLING);
  lcd.clear();
  lcd.print("Alco. lv: ");
}

void loop() {
  if (detect) {
    int data = analogRead(SENSOR_PIN); // Raw number... to get V -> num/1024*5.0
    lcd.setCursor(10, 0);
    if (data < 100) {
      lcd.print(data, DEC);
      lcd.print(" ");
    } else {
      lcd.print(data, DEC);
    }
    if (data > SOBER) {
      lcd.setCursor(0, 1);
      lcd.print("You had alcohol!");
      tone(BUZZER_PIN, PITCH, DURATION);
      lcd.home();
    }
  } else {
    
  }
}

bool isButtonPressed() {
  detect = !detect;
  if (detect) {
    lcd.clear();
    lcd.print("Alco. lv: ");
    lcd.display();
  } else if (!detect) {
    lcd.noDisplay();
  }
}

void mq3_init() {
  int time = 0;
  lcd.begin(16, 2);
  lcd.print("Please wait...");
  while (time < WARMUP_DURATION)
  {
    lcd.setCursor(0, 1);
    time = millis() / 1000;
    lcd.print(time, DEC);
  }
  lcd.clear();
  lcd.print("Ready to use");
  delay(4000);
  lcd.clear();
}
