//knihovny pro SSD1306 OLED displej
#include <Adafruit_GFX.h>
#include <RalewayFonts.hpp>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)
Adafruit_BMP280 bmp;

void setup() {  
  bmp.begin();                                //Start the bmp                  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
}

void loop() {

    display.clearDisplay();
    float T = bmp.readTemperature();           //Read temperature in °C
    float P = bmp.readPressure()/100;         //Read Pressure in Pa and conversion to hPa
    float A = bmp.readAltitude(1019.66);      //Calculating the Altitude, the "1019.66" is the pressure in (hPa) at sea level at day in your region
                                              //If you don't know it just modify it until you get the altitude of your place
    
    display.setFont(&Raleway_SemiBold10pt7b);
    display.setCursor(0,22);
    display.print(T,1);
    display.print(" C");

    display.setFont(&Raleway_SemiBold6pt7b);
    display.setCursor(65,13);
    display.print(P);
    display.print(" hPa");

    display.setCursor(65,25);
    display.print(A,0);
    display.print(" m");
    
    display.display();
    for(byte contrast = 255; contrast > 0; contrast--){
      display.ssd1306_command(SSD1306_SETCONTRAST);
      display.ssd1306_command(contrast);
      delay(4);
    }
    delay(500);
    for(byte contrast = 0; contrast < 255; contrast++){
      display.ssd1306_command(SSD1306_SETCONTRAST);
      display.ssd1306_command(contrast);
      delay(4);
    }
}
