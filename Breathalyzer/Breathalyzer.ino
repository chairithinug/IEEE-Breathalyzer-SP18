// IEEE Breathalyzer (DrinkBoy 2000!) Spring 2019
// Software by Anapat Chairithinugull, IEEE-Project Officer
// Hardware by Kai Pederson, IEEE-Project Officer

// this is a library for the screen provided by the distributor
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// these are Macros defined here and cannot be changed once the program is running
#define POWER_LED_PIN     5   // pin number of arduino connected to the power led
#define DETECTION_LED_PIN 4   // these are digital pins (1 or 0, HIGH or LOW, 5V or 0V)
#define PIEZO_PIN         3
#define MQ_PIN            A7  // A for analog (from 5V to 0V, continuously)

// I2C OLED screen pins (I2C protocol uses 2 pins to communicate between arduino and the screen)
// SDA_PIN           A4
// SCL_PIN           A5

Adafruit_SSD1306 display; // create an instance of the display

// MQ macros
#define WARMUP_PERIOD     30000 // (in milliseconds) MQ3 needs some time to warm up
#define SAMPLES           10   // number of samples MQ sensor should read
#define THRESHOLD         970 // the threshold value that will trigger the sensor to start sampling
#define DRUNK             1000 // this is a raw value from the sensor, higher than this means you had alcohol!
// the higher, the better ;)
// typical value (air) is below 900
// normal blow is around 950

// piezo macros
#define FREQ              860 // the frequency at which it beeps
#define DURATION          50  // how long should it beep

// this "setup" function will run only ONCE when the arduino starts
void setup() {
  pin_init();                         // call our function
  screen_init();
  welcome();
  Serial.begin(9600);                 // FOR DEBUGGING
  digitalWrite(POWER_LED_PIN, HIGH);  // signal life
  Serial.println("warming up");
  delay_with_loading(WARMUP_PERIOD);               // MQ sensor needs some warmup before it can measure more precisely
  // arduino will be stuck here for that amount of time
}

// this "loop" function will loop forever as long as arduino is powered
void loop() {
  int sensor_val = analogRead(MQ_PIN);                    // read the mq value
  Serial.print("reading: ");
  Serial.println(sensor_val);
  // alcohol sensor will be triggered when the "value" is higher than the threshold
  if (sensor_val > THRESHOLD) {
    lcd_print("\n Measuring\n");
    Serial.println("Measuring");
    int alcohol_sum = 0;                                  // this variable will hold the sum of all samples

    digitalWrite(DETECTION_LED_PIN, HIGH);

    for (int i = 0; i < SAMPLES; i++) {
      int value = analogRead(MQ_PIN);
      alcohol_sum = alcohol_sum + value;
      Serial.print("reading: ");
      Serial.println(value);
      delay(250); // take a sample every half a second
      int x = map(i, 0, SAMPLES, 0, 120); // this function scales the value from 0-delay_time to 0-120
      display.fillRect(4, 44, x, 8, WHITE); // Draw filled rectangle (x,y,width,height,color) from top-left to down-right
      display.display();
    }

    digitalWrite(DETECTION_LED_PIN, LOW);                 // let user know it is done measuring
    tone(PIEZO_PIN, FREQ, DURATION);
    delay(250);
    tone(PIEZO_PIN, FREQ, DURATION);

    float alcohol_average = alcohol_sum / SAMPLES;        // this variable will hold the average value
    // float variable because int / int will be round down in arduino, need float to store decimal points

    Serial.println("done measuring");
    Serial.print("average is: ");
    Serial.println(alcohol_average);

    if (alcohol_average > DRUNK) {
      lcd_print("  You are    DRUNK!");
      Serial.println("You are    drunk!");
      tone(PIEZO_PIN, 2 * FREQ, 4 * DURATION);
    } else {
      lcd_print("  You are    sober.\n   Drink     more!");
      Serial.println("You are sober. Drink more!");
    }
    delay(10000);                                          // keep displaying before clearing the screen
  }
  else {
    // do nothing

  }
  display.clearDisplay();
  display.display();
}

// this is our function and we can call it in our code
void pin_init(void)
{
  pinMode(POWER_LED_PIN, OUTPUT); // arduino will output 5V through this pin
  pinMode(DETECTION_LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);

  pinMode(MQ_PIN, INPUT); // arduino will read voltage at this pin
}

void screen_init() {
  // these functions are provided by the library. We users are happy!
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  display.setTextColor(WHITE);                // set the text color
  display.setRotation(0);                     // set the orientation
  display.setTextWrap(true);                  // set the text wrapping
  display.dim(0);                             // set the brightness (0 being the brightest, 1 dimmer)
  display.clearDisplay();                     // clear the screen
}

void welcome() {
  lcd_print("\n DrinkBoy\n\n   2000!");
  delay(5000);
  display.clearDisplay();
  display.display();
}

void delay_with_loading(int delay_time) {
  display.setCursor(4, 16);
  display.print("warming up");
  display.drawRect(4, 44, 120, 8, WHITE);
  for (int i = 0; i < delay_time; i = i + 1000) {
    int x = map(i, 0, delay_time, 0, 120); // this function scales the value from 0-delay_time to 0-120
    display.fillRect(4, 44, x, 8, WHITE); // Draw filled rectangle (x,y,width,height,color) from top-left to down-right
    display.display();
    delay(1000);                            
  }
}

void lcd_print(String text) {
  display.clearDisplay();
  display.setTextSize(2);       // set the size of the text
  display.setTextColor(WHITE);  // set the color of the text
  display.setTextWrap(true);    // make it wrap around when it goes off the screen
  display.setCursor(0, 0);      // set the cursor position (where it should start printing)
  display.println(text);
  display.display();            // display to the screen
}

void lcd_print(String text, int textSize) {
  display.clearDisplay();
  display.setTextSize(textSize);       // set the size of the text
  display.setTextColor(WHITE);  // set the color of the text
  display.setTextWrap(true);    // make it wrap around when it goes off the screen
  display.setCursor(0, 0);      // set the cursor position (where it should start printing)
  display.println(text);
  display.display();            // display to the screen
}

void lcd_add(String text) {
  display.print(text);
  display.display();
}
