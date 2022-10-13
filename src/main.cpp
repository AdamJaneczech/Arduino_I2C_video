//knihovny pro SSD1306 OLED displej
#include <Adafruit_GFX.h>
#include <RalewayFonts.hpp>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BMP280 bmp;

double time = 2000;
double prevTime = 0;
byte contrast;

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  bmp.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
}

void loop() {
    time = millis();
    if(time - prevTime > 2000){
      display.clearDisplay();
      float teplota = bmp.readTemperature();
      float tlak = bmp.readPressure()/100;
      float tlak_def = bmp.readAltitude(1019);

      display.setFont(&Raleway_SemiBold10pt7b);
      display.setCursor(0,22);
      display.print(teplota,1);
      display.setCursor(36, 15);
      display.drawCircle(43,15,3,WHITE);
      display.setCursor(48, 22);
      display.print("C");

      display.setFont(&Raleway_SemiBold6pt7b);
      display.setCursor(67,13);
      display.print(tlak);
      display.print(" hPa");

      display.setCursor(67,25);
      display.print(tlak_def,0);
      display.print(" m");
      
      display.display();
      prevTime = time;
    }
  int reading = map(analogRead(A0), 0, 1023, 0, 255);
  if(reading != contrast){
    contrast = reading;
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(contrast);
    Serial.println(contrast);
  }
}
