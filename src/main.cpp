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
      border-radius: 50%;
    }
  </style>
</head>

<body>
  <h2>Group 4TH Web Server</h2>
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
    <input type="checkbox" value="4" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
  </p>
  <p>
    <img src="" id="imgLED3">
    <img src="" id="imgLED2">
    <img src="" id="imgLED1">
    <img src="" id="imgLED0">
  </p>
  <p>
    <label class="switch">
    <input type="checkbox" value="3" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
    </label>
    <label class="switch">
    <input type="checkbox" value="2" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
    </label>
    <label class="switch">
    <input type="checkbox" value="1" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
    </label>
    <label class="switch">
    <input type="checkbox" value="0" onchange="toggleSwitch(this)">
    <span class="slider round"></span>
    </label>
  </p>
</body>
<script>
  var imgLEDON = "data:image/png;base64,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"
  var imgLEDOFF = "data:image/png;base64,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"
  for (let i = 0; i < 4; i++) {
    document.getElementById("imgLED"+i).src=imgLEDOFF;
  }
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
      if (e.value < 4) {
        document.getElementById("imgLED"+e.value).src=imgLEDON;
      }
    }else{
      xhr.open("GET", "/switch?value=" + e.value + "&state=0", true);
      if (e.value < 4) {
        document.getElementById("imgLED"+e.value).src=imgLEDOFF;
      }
    }
    xhr.send();
  }
</script>

</html>)rawliteral";

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
            { request->send_P(200, "text/html", index_html); });
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