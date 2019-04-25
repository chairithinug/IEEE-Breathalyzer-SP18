// Screen codes taken from https://www.youtube.com/watch?v=PrIAnDZ9dp8
// Please check him out!

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>

Adafruit_SSD1306 display;

#define POWER_LED_PIN     5
#define DETECTION_LED_PIN 4
#define PIEZO_PIN         3
#define MQ_PIN            A7
// SDA  A4
// SCL  A5

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("initializing ...");
  pin_init();
  screen_init();

  testPowerLED();
  delay(5000);
  testDetectionLED();
  delay(5000);
  testPiezo();
  delay(5000);
  testScreen();
  delay(5000);
  testMQ();
  delay(5000);
  Serial.print("Done");

  Serial.println(" Starting loop");
}

void loop() {
  float reading = analogRead(MQ_PIN);
  Serial.print("reading: ");
  Serial.println(reading);
  delay(1000);
  display.clearDisplay();
}

void pin_init() {
  pinMode(POWER_LED_PIN, OUTPUT);
  pinMode(DETECTION_LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(MQ_PIN, INPUT);
}

void screen_init() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.setTextWrap(true);
  display.dim(0);
  display.clearDisplay();
}

void testPowerLED() {
  Serial.println("testing P_LED");
  for (int i = 0; i < 5; i++) {
    digitalWrite(POWER_LED_PIN, LOW);
    delay(250);
    digitalWrite(POWER_LED_PIN, HIGH);
    delay(750);
  }
}

void testDetectionLED() {
  Serial.println("testing D_LED");
  for (int i = 0; i < 5; i++) {
    digitalWrite(DETECTION_LED_PIN, LOW);
    delay(250);
    digitalWrite(DETECTION_LED_PIN, HIGH);
    delay(750);
  }
}

void testPiezo() {
  Serial.println("testing Piezo");
  for (int i = 1; i <= 5; i++) {
    tone(PIEZO_PIN, i * 100, 50);
    delay(1000);
  }
}

void testMQ() {
  Serial.println("testing MQ");
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    float reading = analogRead(MQ_PIN);
    Serial.print("reading #");
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(reading);
    sum += reading;
    delay(1000);
  }
  Serial.print("average is: ");
  Serial.println(sum / 10);
}

void testScreen() {
  Serial.println("testing Screen");
  display.setFont(&FreeMono9pt7b);
  display.setTextSize(0);
  display.setCursor(0, 10);
  display.println("Hello");
  display.drawTriangle(40,40,   50,20,   60,40, WHITE);
  display.fillTriangle(0,63,   15,45,   30,63, WHITE);
  display.drawLine(40, 63, 70, 63, WHITE);
  display.drawCircle(47, 36, 20, WHITE);
  display.fillCircle(12, 27, 10, WHITE);
  display.fillRoundRect(58, 0, 18, 18, 5, WHITE);
  display.drawRect(79, 0, 49, 27, WHITE);
  display.setFont(&FreeMonoBold12pt7b);
  display.drawRoundRect(79, 37, 49, 27, 8, WHITE);
  display.display();
}
