#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "DHT.h"
#include "LED.h"
#include "Buzzer.h"

const char *ssid = "birthday";
const char *password = "20010505";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;
</script>
</html>)rawliteral";

String processor(const String &var)
{
    // Serial.println(var);
    if (var == "TEMPERATURE")
    {
        return getTemperature();
    }
    else if (var == "HUMIDITY")
    {
        return getHumidity();
    }
    return String();
}

void setup()
{
    Serial.begin(115200);
    LEDInit();
    DHTInit();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", getTemperature().c_str()); });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", getHumidity().c_str()); });

    server.begin();
    DHTOn();
}
void loop()
{
}

/*
void DHTTest()
{
    updateDHT();
    Serial.print(getTemperature());
    Serial.println(" *C");
    Serial.print(getHumidity());
    Serial.println(" H");
    Serial.print("Core: ");
    Serial.println(xPortGetCoreID());
    delay(1000);
}

int i = 0;
void LEDTest()
{
    LEDON(i % 4);
    (i % 4 == 0) ? LEDOFF(3) : LEDOFF((i % 4) - 1);
    i++;
    Serial.print(getLEDState(3));
    Serial.print(getLEDState(2));
    Serial.print(getLEDState(1));
    Serial.println(getLEDState(0));
    Serial.print("Core: ");
    Serial.println(xPortGetCoreID());
    delay(1000);
}

void BuzzerTest(){
    Serial.println(getBuzzerState());
    buzzerAlarmOn();
    delay(1000);
    Serial.println(getBuzzerState());
    buzzerAlarmOff();
}
*/