#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <Adafruit_BMP085.h>
#include "TouchScreen.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define DEBUG

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

#define LCD_RESET A4

// These are the pins for the shield!
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// calibration mins and max for raw data when touching edges of screen
// YOU CAN USE THIS SKETCH TO DETERMINE THE RAW X AND Y OF THE EDGES TO GET YOUR HIGHS AND LOWS FOR X AND Y
#define TS_MINX 210
#define TS_MINY 210
#define TS_MAXX 915
#define TS_MAXY 910

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
//#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY      0xCE79
#define LIGHTGREY 0xDEDB

const char* touchMeStr = " WELCOME the message !@";

const int BASEVAL = 500; // Center graph baseline

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);

//HTU21D htu21d; //main & atmospherics boards
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
Adafruit_BMP085 bmp; // 
  float humd=0; 
  float temp=0; 

 
void setup(void) 
{

  Serial.begin(115200);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);

  tft.fillScreen(BLACK);
  tft.setRotation(1);
  tft.setCursor(30, 100);
  tft.setTextColor(RED);  tft.setTextSize(2);
 // tft.println("LCD driver chip: ");
  tft.setCursor(100, 150);
  tft.setTextColor(BLUE);
 // tft.println(identifier, HEX);

  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);

  tft.setCursor(0, 0);
  tft.println(touchMeStr);
  Wire.begin();
if (!bmp.begin()) {
   Serial.println(F("\n\rCould not find a valid BMP085 sensor, check system!"));} 
// init humidity 
 // htu21d.initialize();//
  //Serial.println(htu21d.testConnection() ? "\n\r/HTU21D connection successful" : "HTU21D connection failed");

showData();
}


double Thermistor10bit(int ADCreading) {// arduino Atmel 8 bit boards Uno, Nano, Mega
 double Temp;
 //Temp = log(10000.0*((1024.0/ADCreading-1))); // for pull down confi guration (NTC to + VCC)
 Temp =log(10000.0/(1024.0/ADCreading-1)); // for pull-up configuration
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
 return Temp;
}


//////////////////////////////////////////////////////////////////////////////////


void loop(void) {
 /*humd = myHumidity.readHumidity();
  temp = myHumidity.readTemperature();*/
      Serial.print("Raw Sensor 1 = "); Serial.println(Thermistor10bit(analogRead(A15)));
       Serial.println(" *C");
      Serial.print("Raw Sensor 2 = "); Serial.println(Thermistor10bit(analogRead(A14)));
      Serial.println(" *C");
      Serial.print("Raw Sensor 3 = "); Serial.println(Thermistor10bit(analogRead(A13)));
      Serial.println(" *C");
      Serial.print("Raw Sensor 4 = "); Serial.println(Thermistor10bit(analogRead(A12)));
      Serial.println(" *C");
      Serial.print("Raw Sensor 5 = "); Serial.println(Thermistor10bit(analogRead(A11)));
      Serial.println(" *C");

    /*Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");*/
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");

 // Serial.print("Temp: "); Serial.print(htu.readTemperature());
  Serial.print("\t\tHum: "); Serial.println(htu.readHumidity());
  //delay(500);
  /*Serial.print(" Temperature:");
  Serial.print(temp, 1);
  Serial.print("C");
  Serial.print(" Humidity:");
  Serial.print(humd, 1);
  Serial.print("%");*/

  Serial.println();

    
  updateData();
  delay(500);
}


////////////////////////////////////////////////////////////////////////////////////////////////


void showData()
{

  int indentRight = 40;
  int indentTop = 20; // where to start the text block
  int lineHeight = 20;
  int DataArea[] = {0, 0, 0, 0};
  DataArea[0] = indentRight - 5;
  DataArea[1] = indentTop;
  DataArea[2] = 250;
  DataArea[3] = 250;
  
  //tft.drawRect(DataArea[0], DataArea[1], DataArea[2], DataArea[3], RED);

  tft.setTextSize(2);
  tft.setTextColor( YELLOW); // converts the pressure reading to a color
  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);

  tft.println("SENSOR");


  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+92, indentTop-1, 110,20, BLACK);
  tft.setTextColor( RED);  
  tft.print("RED        "); tft.println(Thermistor10bit(analogRead(A12)));
  

  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+101, indentTop-1, 110,20, BLACK);
  tft.setTextColor( BLUE);
  tft.print("BLUE       "); tft.println(Thermistor10bit(analogRead(A15)));
 
 
  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+111, indentTop-1, 80,20, BLACK);
  tft.setTextColor( YELLOW);
  tft.print("YELLOW     "); tft.println(Thermistor10bit(analogRead(A13)));
  
  
  
  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+110, indentTop-1, 80,20, BLACK);
  tft.setTextColor(WHITE);
  tft.print("BLACK      "); tft.println(Thermistor10bit(analogRead(A11)));
 
  
  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+110, indentTop-1, 80,20, BLACK);
  tft.setTextColor(GREEN);
  tft.print("GREEN      "); tft.println(Thermistor10bit(analogRead(A14)));
  
  
  tft.setCursor(  indentRight-1, indentTop = indentTop + lineHeight);
  tft.setTextColor(BLUE);
  tft.print("ALT ***** M ****** hPa");  
  tft.setCursor(  indentRight+40, indentTop );
  tft.fillRect(indentRight+40, indentTop-1, 70,20, BLACK);
  tft.println(bmp.readAltitude());
  tft.setCursor(  indentRight+140, indentTop );
  tft.fillRect(indentRight+140, indentTop-1, 80,20, BLACK);
  tft.println(bmp.readPressure());

 
  
  tft.setCursor(  indentRight-1, indentTop = indentTop + lineHeight);
  tft.setTextColor(WHITE);
  tft.print("RH ***** % ***** C");
  tft.setCursor(  indentRight+40, indentTop);
  tft.fillRect(indentRight+30, indentTop-1, 70,20, BLACK);
  tft.println(htu.readHumidity()); 
  tft.setCursor(  indentRight+130, indentTop );
  tft.fillRect(indentRight+130, indentTop-1, 60,20, BLACK);
  tft.println(htu.readTemperature());
    




}

void updateData()
{

  int indentRight = 40;
  int indentTop = 20; // where to start the text block
  int lineHeight = 20;
  int DataArea[] = {0, 0, 0, 0};
  DataArea[0] = indentRight - 5;
  DataArea[1] = indentTop;
  DataArea[2] = 250;
  DataArea[3] = 250;
  
  //tft.drawRect(DataArea[0], DataArea[1], DataArea[2], DataArea[3], RED);

  tft.setTextSize(2);
  tft.setTextColor( YELLOW); // converts the pressure reading to a color
  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);

  tft.println("SENSOR");


  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+92, indentTop-1, 110,20, BLACK);
  tft.setTextColor( RED);  
  tft.print("RED        "); tft.println(Thermistor10bit(analogRead(A12)));
  

  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+101, indentTop-1, 110,20, BLACK);
  tft.setTextColor( BLUE);
  tft.print("BLUE       "); tft.println(Thermistor10bit(analogRead(A15)));
 
 
  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+111, indentTop-1, 80,20, BLACK);
  tft.setTextColor( YELLOW);
  tft.print("YELLOW     "); tft.println(Thermistor10bit(analogRead(A13)));
  
  
  
  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+110, indentTop-1, 80,20, BLACK);
  tft.setTextColor(WHITE);
  tft.print("BLACK      "); tft.println(Thermistor10bit(analogRead(A11)));
 
  
  tft.setCursor(  indentRight, indentTop = indentTop + lineHeight);
  tft.fillRect(indentRight+110, indentTop-1, 80,20, BLACK);
  tft.setTextColor(GREEN);
  tft.print("GREEN      "); tft.println(Thermistor10bit(analogRead(A14)));
  
  
  tft.setCursor(  indentRight-1, indentTop = indentTop + lineHeight);
  tft.setTextColor(BLUE);
 // tft.print("ALT ***** M ****** hPa");  
  tft.setCursor(  indentRight+40, indentTop );
  tft.fillRect(indentRight+40, indentTop-1, 70,20, BLACK);
  tft.println(bmp.readAltitude());
  tft.setCursor(  indentRight+140, indentTop );
  tft.fillRect(indentRight+140, indentTop-1, 80,20, BLACK);
  tft.println(bmp.readPressure());

 
  
  tft.setCursor(  indentRight-1, indentTop = indentTop + lineHeight);
  tft.setTextColor(WHITE);
//  tft.print("RH ***** % ***** C");
  tft.setCursor(  indentRight+40, indentTop);
  tft.fillRect(indentRight+30, indentTop-1, 70,20, BLACK);
  tft.println(htu.readHumidity()); 
  tft.setCursor(  indentRight+130, indentTop );
  tft.fillRect(indentRight+130, indentTop-1, 60,20, BLACK);
  tft.println(htu.readTemperature());
    




}
