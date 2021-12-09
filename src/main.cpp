#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "DHT.h"
#include "LED.h"
#include "Buzzer.h"

const char *ssid = "birthday";
const char *password = "20010505";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

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

    h2 {
      font-size: 3.0rem;
    }

    p {
      font-size: 3.0rem;
    }

    .units {
      font-size: 1.2rem;
    }

    .dht-labels {
      font-size: 1.5rem;
      vertical-align: middle;
      padding-bottom: 15px;
    }
  </style>
  <style>
    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 34px;
    }

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      -webkit-transition: .4s;
      transition: .4s;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      -webkit-transition: .4s;
      transition: .4s;
    }

    input:checked+.slider {
      background-color: #2196F3;
    }

    input:focus+.slider {
      box-shadow: 0 0 1px #2196F3;
    }

    input:checked+.slider:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }

    .slider.round {
      border-radius: 34px;
    }

    .slider.round:before {
      border-radius: 50%%;
    }
  </style>
</head>

<body>
  <h2>Group 4TH Web Server</h2>
  <p>
    <label class="switch">
    <input type="checkbox" value="4" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">off</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">off</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    <label class="switch">
    <input type="checkbox" value="0" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
    </label>
    <label class="switch">
    <input type="checkbox" value="1" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
    </label>
    <label class="switch">
    <input type="checkbox" value="2" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
    </label>
    <label class="switch">
    <input type="checkbox" value="3" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
    </label>
  </p>
</body>
<script>
  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 1000);

  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("humidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
  }, 1000);
  
  function toggleSwitch(e) {
    var xhr = new XMLHttpRequest();
    if(e.checked){
      xhr.open("GET", "/switch?value=" + e.value + "&state=1", true);
    }else{
      xhr.open("GET", "/switch?value=" + e.value + "&state=0", true);
    }
    xhr.send();
  }
</script>

</html>)rawliteral";

String processor(const String &var)
{
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

// value
// 0~3 LED
// 4 DHT
void toggleSwitch(int value, int state)
{
  if (value < 4)
  {
    (state) ? LEDON(value) : LEDOFF(value);
  }
  if (value == 4)
  {
    (state) ? DHTON() : DHTOFF();
  }
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
            {
              (getHumidity().toInt() >= 70) ? buzzerAlarmON() : buzzerAlarmOFF();
              request->send_P(200, "text/plain", getHumidity().c_str());
            });
  server.on("/switch", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String value;
              String state;
              if (request->hasParam("value") && request->hasParam("state"))
              {
                value = request->getParam("value")->value();
                state = request->getParam("state")->value();
                Serial.println(value);
                Serial.println(state);
                toggleSwitch(value.toInt(), state.toInt());
              }
              request->send(200, "text/plain", "OK");
            });

  server.begin();
}
void loop()
{
}