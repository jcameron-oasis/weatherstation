/************************************************************************************************
Slapped together by Joseph Cameron with no intent of stepping on others creative claim for a kit
I bought off of Amazon using BME280, ESP32, and 128 x 64 OLED screen with I2C for communication.
I have documented input from others(which is all of it basically, I just copy, pasted, and twisted), as best as possible.
Please inform me if you think there is any other credit or documentation that I need to include.

Also found this useful and should be checked out
https://www.youtube.com/watch?v=RPHP4fAisz8

At several points I thought I had fried my chip and screen.  The chip from too much heat during soldering
and the screen from having the power backwards.  But in the end it was only I2C communication problems
and my IDE needing updated

****************************************************************************************************/




//BME280 code from https://lastminuteengineers.com/bme280-arduino-tutorial/



 
/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers
 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98
 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).
 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!
 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.

Notes from Joseph on this part:
Found at https://github.com/adafruit/Adafruit_SSD1306/blob/master/examples/ssd1306_128x32_i2c/ssd1306_128x32_i2c.ino
It's actually the  128 x 32 code but there is something wrong with the 128 x 64 code, so this one with a slight modification
gets a 128 x 64 working properly.  Found this tip in the comments of https://www.youtube.com/watch?v=PrIAnDZ9dp8

Also don't trust the I2C address on the back of the display, mine said 0x78 but was actually 0x3C
Use the I2C scanner sketch to find the proper address
https://playground.arduino.cc/Main/I2cScanner/

Also I powered the screen with 3.3 volts to leave the 5 volt pin open for the BME280 sensor with an Arduino
with perfect results, was even suggest in the above youtube video to reduce electronic humming noise.


 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//Note from Joseph: The above line was edited to turn 128x32 code to 128x64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

Adafruit_BME280 bme;
long int x; //used to store info for pressDiff compare
long int y; //used to store info for tempDiff compare
long int z; //used to store info for humDiff compare

static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };




void setup() {
  Wire.begin();
  Serial.begin(9600);
  !bme.begin(0x76);


//  SSD1306_SWITCHCAPVCC = 3.3//generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();
  delay(250);



  x = bme.readPressure();
  y = bme.readTemperature();
  z = bme.readHumidity();

}

void loop() {

  display.clearDisplay();
//  HiBeth();
//  delay(2000);
  weatherOverview();
  delay(5000);  
  pressDiff();
  delay(2500);
  tempDiff();
  delay(2500);
  humDiff();
  delay(2500);
  
}



void HiBeth(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(BLACK, WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Adafruit Rules"));

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,17);
  display.println(F("Portable"));
  
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.println(F("Weather"));

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.println(F("Station!!!"));



  display.display();
}


void humDiff(void) {

  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(BLACK, WHITE);        // Draw white text
  display.setCursor(13,1);             // Start at top-left corner (X,Y)
  display.println(F(" Hum Diff"));

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(17,37);             // Start at top-left corner (X,Y)
  display.print(((bme.readHumidity()) - z ));
  display.println(F("%"));
  
  display.display();
}


void tempDiff(void) {

  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(BLACK, WHITE);        // Draw white text
  display.setCursor(8,1);             // Start at top-left corner (X,Y)
  display.println(F(" T Diff since start "));


  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(17,17);             // Start at top-left corner (X,Y)
  display.print((((bme.readTemperature()) - y)*1.8));
  display.println(F("F"));

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(17,37);             // Start at top-left corner (X,Y)
  display.print(((bme.readTemperature()) - y));
  display.println(F("C"));
  
  display.display();
}


void pressDiff(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(BLACK, WHITE);        // Draw white text
  display.setCursor(8,1);             // Start at top-left corner (X,Y)
  display.println(F(" P Diff since start "));


  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(17,17);             // Start at top-left corner (X,Y)
  display.print(((x - bme.readPressure()))*.0002953);
  display.println(F("In"));

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(17,37);             // Start at top-left corner (X,Y)
  display.print(((bme.readPressure() - x))/100);
  display.println(F("hPa"));
  
  display.display();
}

void weatherOverview(void) {
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(1,1);             // Start at top-left corner
  display.print(int((bme.readTemperature()*1.8)+32));
  display.print(F("F/"));

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.print(bme.readTemperature());
  display.println(F("C"));
  
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.print(bme.readHumidity());
  display.println(F("% Hum"));
  display.display();
  
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.print(bme.readPressure() * 0.0002953);
  display.println(F("In"));
  display.display();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.print(int(bme.readPressure()/100));
  display.print(F("hPa"));
  display.display();

}
