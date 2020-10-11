
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);
  
const float FACTOR = 30; //30A/1V
const float multiplier = 0.0905F;
//const float multiplier = 0.03125F;
//const float multiplier = 0.125F;
void ImprimirMedidas(String prefix, float value, String postfix);

void setup()
{
  Serial.begin(9600);
  ads.setGain(GAIN_TWO);        // ±2.048V  1 bit = 0.0625mV
  ads.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}
 
void loop()
{
 float CorrienteRMS = getCorriente();
 float Potencia = 220.0 * CorrienteRMS;
 
 ImprimirMedidas("Irms: ", CorrienteRMS, "A ,");
 ImprimirMedidas("Potencia: ", Potencia, "W");

 display.clearDisplay();

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 15);
  display.print("Irms: " );
  display.print(CorrienteRMS);
  display.print("A");
  display.setCursor(5, 35);
  display.print("Potencia: ");
  display.print(Potencia);
  display.print("W");
  display.display();      // Show initial text
  delay(100);
}

void ImprimirMedidas(String prefix, float value, String postfix)
{
 Serial.print(prefix);
 Serial.print(value, 3);
 Serial.println(postfix);
}
 

float getCorriente()
{
 float Volt_diferencial;
 float corriente;
 float sum = 0;
 long tiempo = millis();
 int counter = 0;
 
 while (millis() - tiempo < 1000)
 {
   Volt_diferencial = ads.readADC_Differential_0_1() * multiplier;
   corriente = Volt_diferencial * FACTOR;
   corriente /= 1000.0;
 
   sum += sq(corriente);
   counter = counter + 1;
  }
 
 corriente = sqrt(sum / counter);
 return(corriente);
}
