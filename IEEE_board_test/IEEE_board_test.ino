#define POWER_LED_PIN     4
#define DETECTION_LED_PIN 3
#define PIEZO_PIN         2
#define MQ_PIN            A0

void setup() {
  Serial.begin(115200);
  while (!Serial);
  pin_init();

  testPowerLED();
  delay(5000);
  testDetectionLED();
  delay(5000);
  testPiezo();
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
}

void pin_init() {
  Serial.println("initializing ...");
  pinMode(POWER_LED_PIN, OUTPUT);
  pinMode(DETECTION_LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(MQ_PIN, INPUT);
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
