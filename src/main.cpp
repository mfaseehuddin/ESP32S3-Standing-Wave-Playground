// printing hello world on the screen.
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <U8g2lib.h>
#include <math.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const int width = 128;
const int height = 64;
const float amplitude = 16.0;
const float wavelength = 256.0; // pixels
const float frequency = 0.05;  // Controls speed of animation

void setup()
{
  Serial.begin(115200);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x13_mf);
  u8g2.drawStr(0, 24, "Hello World!");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearDisplay();
}

void loop()
{
  // for (size_t x = 0; x < 128; x++)
  // {
  //   for (size_t y = 0; y < 64; y++)
  //   {
  //     u8g2.drawPixel(x, y);
  //     u8g2.drawPixel(x, y+1);
  //     u8g2.drawPixel(x+1, y+1);
  //     u8g2.drawPixel(x-1, y-1);
  //     u8g2.sendBuffer();
  //     // delay();
  //     u8g2.clearDisplay();
  //   }
  // }

  // for (size_t i = 0; i < 128; i++)
  // {
  //   int sinVal = cos(i * 5 *  M_PI / 180) * 30 + 32; // Adjusting the sine value to fit the display
  //   u8g2.drawPixel(i, sinVal);
  //   u8g2.sendBuffer();
  //   Serial.println(sinVal);
  // }
  static float t = 0;

  u8g2.clearBuffer(); // Clear display buffer

  for (int x = 0; x < width; x++)
  {
    float kx = 2.0 * M_PI * x / wavelength;
    float y = 2 * amplitude * sin(kx) * cos(t); // Standing wave equation
    int yPos = (height / 2) - int(y);           // Center vertically

    if (yPos >= 0 && yPos < height)
    {
      u8g2.drawPixel(x, yPos);
    }
  }

  u8g2.sendBuffer(); // Push buffer to display
  delay(50);         // Control animation speed

  t += frequency; // Advance time
}