// library for I2C protocol
#include <Wire.h>
// this is a library for the screen provided by the distributor
// (copy 2 folders inside libraries folder in zip and paste in libraries folder in arduino folder (likely in your documents folder)
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// these are Macros defined here and cannot be changed once the program is running
#define POWER_LED_PIN     4   // pin number of arduino connected to the power led
#define DETECTION_LED_PIN 3   // these are digital pins (1 or 0, HIGH or LOW, 5V or 0V)
#define PIEZO_PIN         2
#define MQ_PIN            A0  // A for analog (from 5V to 0V, continuously)

// I2C LCD screen pins (I2C protocol uses 2 pins to communicate between arduino and the screen)
//#define SDA_PIN           A4
//#define SCL_PIN           A6  // should be A5

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET); // create an instance of the display

#define WARMUP_PERIOD     60000 // in milliseconds

// MQ macros
#define SAMPLES           5   // number of samples MQ sensor should read
#define THRESHOLD         150 // the threshold value that will trigger the sensor to start sampling
#define DRUNK             230 // this is a raw value from the sensor, higher than this means you had alcohol!

// piezo macros
#define FREQ              860 // the frequency it beeps
#define DURATION          50  // how long should it beep


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// this "setup" function will run only ONCE when the arduino starts
void setup() {
  pin_init();                         // call our function

  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);

  Serial.begin(115200);               // FOR DEBUGGING
  digitalWrite(POWER_LED_PIN, HIGH);  // signal life
  Serial.println("warming up");
  delay(WARMUP_PERIOD);               // MQ sensor needs some warmup before it can measure more precisely
  // arduino will be stuck here for that amount of time
}

// this "loop" function will loop forever as long as arduino is powered
void loop() {
  int sensor_val = analogRead(MQ_PIN);                    // read the mq value
  Serial.print("reading: ");
  Serial.println(sensor_val);
  // alcohol sensor will be triggered when the "value" is higher than the threshold
  if (sensor_val > THRESHOLD) {
    Serial.println("start measuring");
    int alcohol_sum = 0;                                  // this variable will hold the sum of all samples

    digitalWrite(DETECTION_LED_PIN, HIGH);
    
    for (int i = 0; i < SAMPLES; i++) {
      alcohol_sum = alcohol_sum + analogRead(MQ_PIN);
      delay(1000); // take a sample every second
    }
    
    digitalWrite(DETECTION_LED_PIN, LOW);                 // let user know it is done measuring
    tone(PIEZO_PIN, FREQ, DURATION);     
    delay(500);
    tone(PIEZO_PIN, FREQ, DURATION);

    float alcohol_average = alcohol_sum / SAMPLES;        // this variable will hold the average value
    // float variable because int / int will be round down in arduino, need float to store decimal points

    Serial.println("done measuring");
    Serial.print("average is: ");
    Serial.println(alcohol_average);
    // DISPLAY VALUE
    // DISPLAY AM I DRUNK?

    if (alcohol_average > DRUNK) {
      lcd_print("you are drunk!");
      Serial.println("you are drunk!");
    } else {
      lcd_print("you are sober!");
      Serial.println("you are sober!");
    }
    delay(5000);                                          // keep displaying before clearing the screen
    display.clearDisplay();
  }
  else {
    // do nothing
  }
}

// this is our function and we can call it in our code
void pin_init(void)
{
  pinMode(POWER_LED_PIN, OUTPUT); // arduino will output 5V through this pin
  pinMode(DETECTION_LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);

  pinMode(MQ_PIN, INPUT); // arduino will read voltage at this pin
}

void lcd_print(char *text) {
  display.clearDisplay();
  display.setTextSize(1); // set the size of the text
  display.setTextColor(WHITE); // set the color of the text
  display.setTextWrap(true); // make it wrap around when it goes off the screen
  display.setCursor(0, 0); // set the cursor position (where it should start printing)
  while (*text != '\0') {
    display.write(*text);
  }
  display.display(); // display to the screen
}
