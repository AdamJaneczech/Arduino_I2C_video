//knihovny pro SSD1306 OLED displej
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//knihovna pro font Raleway použitý na displeji
#include <RalewayFonts.hpp>
//knihovna pro senzor BMP280
#include <Adafruit_BMP280.h>
//nadefinování A0 jako vstupního pinu potenciometru
#define POTENTIOMETER A0
//parametry displeje
#define SCREEN_WIDTH 128  //šířka displeje
#define SCREEN_HEIGHT 32  //výška displeje
#define OLED_RESET    -1  //reset pin displeje
//vytvoření objektu displeje
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//vytvoření objektu senzoru
Adafruit_BMP280 bmp;
//proměnné pro časový interval měření (aktuální a předchozí naměřený čas)
int time = 2000;
int prevTime = 0;
//proměnná pro uložení hodnoty kontrastu displeje
byte contrast;

void setup() {
  //nastavení pinu potenciometru
  pinMode(POTENTIOMETER, INPUT);
  //zahájení I2C komunikace (knihovna Wire)
  Wire.begin();
  //inicializace senzoru
  bmp.begin();
  //inicializace displeje, nastavení parametrů
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  //nastavení rychlosti přenosu I2C (bit/s)
  Wire.setClock(100000);
}

void loop() {
    //změření aktuálního času - funkce millis()
    time = millis();
    //pokud je rozdíl od předchozího měření vyšší, než 2000 ms (2 s)
    if(time - prevTime > 2000){
      display.clearDisplay();
      //přečtení údajů o teplotě v °C
      float teplota = bmp.readTemperature();
      //přečtení údajů o tlaku, převod na hPa (/100)
      float tlak = bmp.readPressure()/100;
      //uložení hodnoty nadmořské výšky do proměnné, v paramteru tlak na úrovni hladiny moře (zde 1018 hPa)
      float tlak_def = bmp.readAltitude(1018);
      //nastavení fontu (Raleway - viz knihovna RalewayFonts.hpp)
      display.setFont(&Raleway_SemiBold10pt7b);
      //výpis hodnot teploty
      display.setCursor(0,22);
      display.print(teplota,1);
      display.setCursor(36, 15);
      display.drawCircle(43,15,3,WHITE);
      display.setCursor(48, 22);
      display.print("C");
      //výpis hodnot tlaku
      display.setFont(&Raleway_SemiBold6pt7b);
      display.setCursor(67,13);
      display.print(tlak, 1);
      display.print(" hPa");
      //výpis nadmořské výšky
      display.setCursor(67,25);
      display.print(tlak_def,0);
      display.print(" m");
      //zobrazení na displeji
      display.display();
      //vynulování rozdílu časů
      prevTime = time;
    }
  //proměnná pro čtení z analogového pinu potenciometru, převedení z 10-bitových hodnot (0-1023) na 8-bitové (0-255) pomocí funkce map
  byte reading = map(analogRead(A0), 0, 1023, 0, 255);
  //pokud se kontrast nerovná přečtené hodnotě, změnit kontrast
  if(reading != contrast){
    //vyrovnání proměnných
    contrast = reading;
    //První způsob přenosu hodnoty kontrastu displeje
    //-----------------------------------------------
    //zahájit komunikaci přes I2C (START podmínka, odeslání adresy)
    Wire.beginTransmission(0x3C);
    //Kontrolní byte (datasheet str. 20)
    Wire.write(0);
    //Příkaz pro nastavení kontrastu (datasheet str. 36)
    Wire.write(0x81);
    //Odeslání hodnoty kontrastu
    Wire.write(contrast);
    //Ukončení komunikace (STOP podmínka)
    Wire.endTransmission();
    //-----------------------------------------------
    //Druhý způsob přenosu hodnoty kontrastu displeje
    /*
    //příkaz pro zápis do registru displeje pro kontrast
    display.ssd1306_command(SSD1306_SETCONTRAST);
    //odeslání 8-bitové hodnoty kontrastu displeje
    display.ssd1306_command(contrast);
    //vynulování rozdílů proměnných
    */
  }
}
