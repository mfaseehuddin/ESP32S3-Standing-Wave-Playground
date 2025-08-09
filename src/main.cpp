#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <U8g2lib.h>
#include <math.h>

// OLED setup
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Wi-Fi credentials
const char *ssid = "Boi";
const char *password = "Password123";

// Web server
AsyncWebServer server(80);

// Wave parameters (defaults)
float amplitude = 16.0;
float wavelength = 256.0;
float frequency = 0.05;

const int width = 128;
const int height = 64;

void drawWave(float t)
{
  u8g2.clearBuffer();

  for (int x = 0; x < width; x++)
  {
    float kx = 2.0 * M_PI * x / wavelength;
    float y = 2 * amplitude * sin(kx) * cos(t);
    int yPos = (height / 2) - int(y);
    if (yPos >= 0 && yPos < height)
    {
      u8g2.drawPixel(x, yPos);
    }
  }

  u8g2.sendBuffer();
}

String processor(const String &var)
{
  if (var == "AMPLITUDE") return String(amplitude);
  if (var == "WAVELENGTH") return String(wavelength);
  if (var == "FREQUENCY") return String(frequency);
  return String();
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Wave Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; padding-top: 50px; }
    input[type=range] { width: 300px; }
  </style>
</head>
<body>
  <h2>Wave Controller</h2>
  <form action="/update" method="GET">
    <p>Amplitude: <span id="ampVal">%AMPLITUDE%</span></p>
    <input type="range" name="amplitude" min="0" max="32" step="0.1" value="%AMPLITUDE%" oninput="ampVal.innerText = this.value">
    <p>Wavelength: <span id="waveVal">%WAVELENGTH%</span></p>
    <input type="range" name="wavelength" min="10" max="300" step="1" value="%WAVELENGTH%" oninput="waveVal.innerText = this.value">
    <p>Frequency: <span id="freqVal">%FREQUENCY%</span></p>
    <input type="range" name="frequency" min="0.01" max="0.2" step="0.01" value="%FREQUENCY%" oninput="freqVal.innerText = this.value">
    <br><br>
    <input type="submit" value="Update">
  </form>
</body>
</html>
)rawliteral";

void setup()
{
  Serial.begin(115200);
  u8g2.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected! IP: " + WiFi.localIP().toString());

  // Route to serve HTML with values
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });

  // Handle parameter updates
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (request->hasParam("amplitude"))
                amplitude = request->getParam("amplitude")->value().toFloat();
              if (request->hasParam("wavelength"))
                wavelength = request->getParam("wavelength")->value().toFloat();
              if (request->hasParam("frequency"))
                frequency = request->getParam("frequency")->value().toFloat();

              Serial.printf("Updated - Amplitude: %.2f, Wavelength: %.2f, Frequency: %.2f\n", amplitude, wavelength, frequency);
              request->redirect("/"); });

  server.begin();
}

void loop()
{
  static float t = 0;
  drawWave(t);
  t += frequency;
  delay(50);
}
