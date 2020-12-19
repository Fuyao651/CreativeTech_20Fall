// This #include statement was automatically added by the Particle IDE.
#include "DFRobot_BMI160.h"

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>



//INPUT: I2C setting for bmin160
DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x69;


//SUBSCRIPTION: subscribe setting: Null


//OUTPUT: digitalRGB setting
#define PIN_LED 3
#define NUM_LED 1
#define PINTYPE WS2812B
Adafruit_NeoPixel RGB_Strip = Adafruit_NeoPixel(NUM_LED, PIN_LED, PINTYPE);

// Custom colour1: Yellow
#define RED_VAL_1       255
#define GREEN_VAL_1     255
#define BLUE_VAL_1      0

// Custom colour2: Purple
#define RED_VAL_2       255
#define GREEN_VAL_2     0
#define BLUE_VAL_2      255

// Custom colour3: Cyan
#define RED_VAL_3       0
#define GREEN_VAL_3     255
#define BLUE_VAL_3      255

//switch 6 colourtype 
int showType = 0;
String StringGyroX;

void setup() {
    
  Particle.variable("angleX", StringGyroX);


  //SUBSCRIPTION: Subscribe Zhuoyu's temperature sensor
 // Particle.subscribe("temperature", myHandler);
  
  //opens serial port, sets data rate to 9600 bps
  //Serial.begin(9600);
  //delay(100);


  //OUTPUT: setup digitalRGB
  RGB_Strip.begin();
  RGB_Strip.show();
  
  
  //INPUT:init the hardware bmin160
  if (bmi160.softReset() != BMI160_OK) {
    Serial.println("reset false");
    while (1);
  }
  //set and init the bmi160 i2c address
  if (bmi160.I2cInit(i2c_addr) != BMI160_OK) {
    Serial.println("init false");
    while (1);
  }
 RGB_Strip.setBrightness(128);

}


void loop() {

//get both accel and gyro data from bmi160
  int i = 0;
  int rslt;
  int16_t accelGyro[6] = {0}; //parameter accelGyro is the pointer to store the data
  rslt = bmi160.getAccelGyroData(accelGyro);  // return BMI160_OK(0) means success

  float GyroX = accelGyro[3] / 180.0; //  degree
  float GyroY = accelGyro[4] / 180.0;
  float GyroZ = accelGyro[5] / 180.0;
  float AccelX = accelGyro[0]/ 1000 ; // m / s2
  float AccelY = accelGyro[1] / 1000;
  float AccelZ = accelGyro[2] / 1000;
  
  StringGyroX =  String(GyroX);

// how to make the array into string... and publish them all...for now I just published one data
  Particle.publish("Gyro_X_by_Fuyao", String::format("%.1f", GyroX)); 
  delay(500);
  
  if (rslt == 0) {
      if ( AccelX > 1 || AccelY > 1 || AccelZ > 1 ){
        delay(20);
        showType++;
          if (showType > 5) {showType=0;
          }
        startShow(showType);
        }

  }else {
    Serial.println("err");
  }

}
/*
void myHandler(const char *event, const char *data)
{ 
  
//transform string_data into int_data from subscripition
 int remote_brightness = atoi(data); 

// Zhuoyu's sensor from 3000-2000, map my RGB brightness from the brightest to the darkest
 if (remote_brightness < 1750 ){
  RGB_Strip.setBrightness(0);
  } else if (remote_brightness > 3250 ){
    RGB_Strip.setBrightness(255);
    }else {
      remote_brightness = map ( remote_brightness, 3250, 1750, 255, 0 );
      RGB_Strip.setBrightness(remote_brightness);
    }
}
*/

// define functions for digital RGB 

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<RGB_Strip.numPixels(); i++) {
    RGB_Strip.setPixelColor(i, c);
    RGB_Strip.show();
    delay(wait);
  }
}

void startShow(int n) {
  switch(n){
    case 0: colorWipe(RGB_Strip.Color(255, 0, 0), 50);  // Red
            break;
    case 1: colorWipe(RGB_Strip.Color(0, 255, 0), 50);  // Green
            break;
    case 2: colorWipe(RGB_Strip.Color(0, 0, 255), 50);  // Blue
            break;
    case 3: colorWipe(RGB_Strip.Color(RED_VAL_1, GREEN_VAL_1, BLUE_VAL_1), 50);  //Custom colour1: Yellow
            break;
    case 4: colorWipe(RGB_Strip.Color(RED_VAL_2, GREEN_VAL_2, BLUE_VAL_2), 50);  //Custom colour2: Purple
            break;
    case 5: colorWipe(RGB_Strip.Color(RED_VAL_3, GREEN_VAL_3, BLUE_VAL_3), 50);  //Custom colour3: Cyan
            break;

  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return RGB_Strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return RGB_Strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return RGB_Strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
