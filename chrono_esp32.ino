/*
* MIT License
*
* Copyright (c) 2022 thieu-b55
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <SPI.h>
#include <TFT_eSPI.h>
#include "FS.h"
#include "SPIFFS.h"
#include "SD.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
TFT_eSPI tft = TFT_eSPI();

#define RXD2          16
#define TXD2          17
#define WIS_TIJDEN    27

typedef struct
{
  uint16_t micro_seconden;
  uint16_t milli_seconden;
  uint16_t seconden;
  uint16_t minuten;
  uint16_t uren;
}data;
data tijd;

bool spiffs_format_bool;
char aantal_char[10];
char aantal_html_char[10];
char chrono_char[20];
char lees_char[20];
char positie_char[10];
char tijden_char[502][20]; 
int aantal_int = 1;
int aantal_html_int;
int basis_int = 0;
int tft_add_int = 5;
int tft_positie_int = 10;
String aantal_string = "         ";
String aantal_html_string = "         ";
String chrono_string = "                    ";
String lees_chrono_string = "                    ";
String lees_string = "                   ";
String positie_string = "         ";
String tijd_string = "/tijd";
unsigned long scherm_update_long;

const char* MIN_INPUT = "min";
const char* PLUS_INPUT = "plus";
const char* APSSID = "ESP32Chrono";
const char* APPSWD = "ESP32pswd";

void write_char(fs::FS &fs, const char * path, const char * message){
  File file = fs.open(path, FILE_WRITE);
  if(!file){
      return;
  }
  if(file.print(message)){
      
  } 
  file.close();
}

void read_string(fs::FS &fs, const char * path){
  File file = fs.open(path);
  if(!file){
    return;
  }
  int teller = 0;
  int index;
  memset(lees_char, 0, sizeof(lees_char));
  while(file.available()){
    lees_char[teller] = file.read();
    teller++;
  }
  file.close();
  lees_string = String(lees_char);
}


const char chrono_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <iframe style="display:none" name="hidden-form"></iframe>
    <meta name="viewport" content="width=device-width, initial-scale=0.85">
    <title>Precisie chrono</title>
    <style>
        div.kader {
          position: relative;
          width: 400px;
          height: 12x;
        }
        div.links{
          position: absolute;
          left : 0px;
          width; auto;
          height: 12px;
        }
        div.links_midden{
          position:absolute;
          left:  80px;
          width: auto;
          height: 12px; 
        }
        div.titel{
          height: 25px;
          width: auto;
        }
        div.bottom{
          position: fixed;
          bottom: 0px;
        }
        div.data_links{
          position: absolute;
          left:  0px;
          width: 80px;
          height: 12px;
        }
        div.data_links_midden{
          position: absolute;
          left:  80px;
          width: 120px;
          height: 12px;
        }
        div.blanco_20{
          width: auto;
          height: 20px;
        }
        div.blanco_15{
          width: auto;
          height: 15px;
        }
        div.blanco_10{
          width: auto;
          height: 10px;
        }
    </style>
    
  </head>
  <body>
    <h3><center> ESP32 Precisie Chrono </center></h3>
    <small>
      <div class="blanco_10">&nbsp;</div>
      <div class="titel"><center><b>Aantal tijden &nbsp; : &nbsp; %aantal%</b></center></div>
      <div class="blanco_10">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_1%</div>
        <div class="data_links_midden">%tijd_1%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_2%</div>
        <div class="data_links_midden">%tijd_2%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_3%</div>
        <div class="data_links_midden">%tijd_3%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_4%</div>
        <div class="data_links_midden">%tijd_4%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_5%</div>
        <div class="data_links_midden">%tijd_5%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_6%</div>
        <div class="data_links_midden">%tijd_6%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_7%</div>
        <div class="data_links_midden">%tijd_7%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_8%</div>
        <div class="data_links_midden">%tijd_8%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_9%</div>
        <div class="data_links_midden">%tijd_9%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_10%</div>
        <div class="data_links_midden">%tijd_10%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_11%</div>
        <div class="data_links_midden">%tijd_11%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_12%</div>
        <div class="data_links_midden">%tijd_12%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_13%</div>
        <div class="data_links_midden">%tijd_13%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_14%</div>
        <div class="data_links_midden">%tijd_14%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_15%</div>
        <div class="data_links_midden">%tijd_15%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_16%</div>
        <div class="data_links_midden">%tijd_16%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_17%</div>
        <div class="data_links_midden">%tijd_17%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_18%</div>
        <div class="data_links_midden">%tijd_18%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_19%</div>
        <div class="data_links_midden">%tijd_19%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_20%</div>
        <div class="data_links_midden">%tijd_20%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_21%</div>
        <div class="data_links_midden">%tijd_21%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_22%</div>
        <div class="data_links_midden">%tijd_22%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_23%</div>
        <div class="data_links_midden">%tijd_23%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_24%</div>
        <div class="data_links_midden">%tijd_24%</div>
      </div>
      <div class="blanco_20">&nbsp;</div>
      <div class="kader">
        <div class="data_links">%nummer_25%</div>
        <div class="data_links_midden">%tijd_25%</div>
      </div>
      <br><br><br>
      <form action="/get" target="hidden-form">
          <center>
            <input type="submit" name="min" value="   -   " onclick="ok()">
            &nbsp;&nbsp;&nbsp;
            <input type="submit" name="plus" value="   +   " onclick="ok()">
          </center>
      </form>
      <div class="bottom">
        <h6><b>thieu-b55 december 2022</b></h6>
      </div>
    </small>
    <script>
      function ok(){
        setTimeout(function(){document.location.reload();},250);
      }
    </script>
  </body>  
</html>
)rawliteral";    

String processor(const String& var){
  int temp_int;
  char nummer_char[26][5];
  
  if(var == "aantal"){
    return(aantal_html_char);
  }
  if(var == "nummer_1"){
    temp_int = basis_int + 1;
    sprintf(nummer_char[1], "%03d%", temp_int);
    return(nummer_char[1]);
  }
  if(var == "nummer_2"){
    temp_int = basis_int + 2;
    sprintf(nummer_char[2], "%03d%", temp_int);
    return(nummer_char[2]);
  }
  if(var == "nummer_3"){
    temp_int = basis_int + 3;
    sprintf(nummer_char[3], "%03d%", temp_int);
    return(nummer_char[3]);
  }
  if(var == "nummer_4"){
    temp_int = basis_int + 4;
    sprintf(nummer_char[4], "%03d%", temp_int);
    return(nummer_char[4]);
  }
  if(var == "nummer_5"){
    temp_int = basis_int + 5;
    sprintf(nummer_char[5], "%03d%", temp_int);
    return(nummer_char[5]);
  }
  if(var == "nummer_6"){
    temp_int = basis_int + 6;
    sprintf(nummer_char[6], "%03d%", temp_int);
    return(nummer_char[6]);
  }
  if(var == "nummer_7"){
    temp_int = basis_int + 7;
    sprintf(nummer_char[7], "%03d%", temp_int);
    return(nummer_char[7]);
  }
  if(var == "nummer_8"){
    temp_int = basis_int + 8;
    sprintf(nummer_char[8], "%03d%", temp_int);
    return(nummer_char[8]);
  }
  if(var == "nummer_9"){
    temp_int = basis_int + 9;
    sprintf(nummer_char[9], "%03d%", temp_int);
    return(nummer_char[9]);
  }
  if(var == "nummer_10"){
    temp_int = basis_int + 10;
    sprintf(nummer_char[10], "%03d%", temp_int);
    return(nummer_char[10]);
  }
  if(var == "nummer_11"){
    temp_int = basis_int + 11;
    sprintf(nummer_char[11], "%03d%", temp_int);
    return(nummer_char[11]);
  }
  if(var == "nummer_12"){
    temp_int = basis_int + 12;
    sprintf(nummer_char[12], "%03d%", temp_int);
    return(nummer_char[12]);
  }
  if(var == "nummer_13"){
    temp_int = basis_int + 13;
    sprintf(nummer_char[13], "%03d%", temp_int);
    return(nummer_char[13]);
  }
  if(var == "nummer_14"){
    temp_int = basis_int + 14;
    sprintf(nummer_char[14], "%03d%", temp_int);
    return(nummer_char[14]);
  }
  if(var == "nummer_15"){
    temp_int = basis_int + 15;
    sprintf(nummer_char[15], "%03d%", temp_int);
    return(nummer_char[15]);
  }
  if(var == "nummer_16"){
    temp_int = basis_int + 16;
    sprintf(nummer_char[16], "%03d%", temp_int);
    return(nummer_char[16]);
  }
  if(var == "nummer_17"){
    temp_int = basis_int + 17;
    sprintf(nummer_char[17], "%03d%", temp_int);
    return(nummer_char[17]);
  }
  if(var == "nummer_18"){
    temp_int = basis_int + 18;
    sprintf(nummer_char[18], "%03d%", temp_int);
    return(nummer_char[18]);
  }
  if(var == "nummer_19"){
    temp_int = basis_int + 19;
    sprintf(nummer_char[19], "%03d%", temp_int);
    return(nummer_char[19]);
  }
  if(var == "nummer_20"){
    temp_int = basis_int + 20;
    sprintf(nummer_char[20], "%03d%", temp_int);
    return(nummer_char[20]);
  }
  if(var == "nummer_21"){
    temp_int = basis_int + 21;
    sprintf(nummer_char[21], "%03d%", temp_int);
    return(nummer_char[21]);
  }
  if(var == "nummer_22"){
    temp_int = basis_int + 22;
    sprintf(nummer_char[22], "%03d%", temp_int);
    return(nummer_char[22]);
  }
  if(var == "nummer_23"){
    temp_int = basis_int + 23;
    sprintf(nummer_char[23], "%03d%", temp_int);
    return(nummer_char[23]);
  }
  if(var == "nummer_24"){
    temp_int = basis_int + 24;
    sprintf(nummer_char[24], "%03d%", temp_int);
    return(nummer_char[24]);
  }
  if(var == "nummer_25"){
    temp_int = basis_int + 25;
    sprintf(nummer_char[25], "%03d%", temp_int);
    return(nummer_char[25]);
  }
  if(var == "tijd_1"){
    return(tijden_char[basis_int + 1]);
  }
  if(var == "tijd_2"){
    return(tijden_char[basis_int + 2]);
  }
  if(var == "tijd_3"){
    return(tijden_char[basis_int + 3]);
  }
  if(var == "tijd_4"){
    return(tijden_char[basis_int + 4]);
  }
  if(var == "tijd_5"){
    return(tijden_char[basis_int + 5]);
  }
  if(var == "tijd_6"){
    return(tijden_char[basis_int + 6]);
  }
  if(var == "tijd_7"){
    return(tijden_char[basis_int + 7]);
  }
  if(var == "tijd_8"){
    return(tijden_char[basis_int + 8]);
  }
  if(var == "tijd_9"){
    return(tijden_char[basis_int + 9]);
  }
  if(var == "tijd_10"){
    return(tijden_char[basis_int + 10]);
  }
  if(var == "tijd_11"){
    return(tijden_char[basis_int + 11]);
  }
  if(var == "tijd_12"){
    return(tijden_char[basis_int + 12]);
  }
  if(var == "tijd_13"){
    return(tijden_char[basis_int + 13]);
  }
  if(var == "tijd_14"){
    return(tijden_char[basis_int + 14]);
  }
  if(var == "tijd_15"){
    return(tijden_char[basis_int + 15]);
  }
  if(var == "tijd_16"){
    return(tijden_char[basis_int + 16]);
  }
  if(var == "tijd_17"){
    return(tijden_char[basis_int + 17]);
  }
  if(var == "tijd_18"){
    return(tijden_char[basis_int + 18]);
  }
  if(var == "tijd_19"){
    return(tijden_char[basis_int + 19]);
  }
  if(var == "tijd_20"){
    return(tijden_char[basis_int + 20]);
  }
  if(var == "tijd_21"){
    return(tijden_char[basis_int + 21]);
  }
  if(var == "tijd_22"){
    return(tijden_char[basis_int + 22]);
  }
  if(var == "tijd_23"){
    return(tijden_char[basis_int + 23]);
  }
  if(var == "tijd_24"){
    return(tijden_char[basis_int + 24]);
  }
  if(var == "tijd_25"){
    return(tijden_char[basis_int + 25]);
  }
}

void html_input(){
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/html", chrono_html, processor);
  });
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasParam(MIN_INPUT)){
        basis_int = basis_int - 25;
        if(basis_int < 0){
          basis_int = 0;
        }
      }
      if(request->hasParam(PLUS_INPUT)){
        basis_int = basis_int + 25;
        if(basis_int > 475){
          basis_int = 475;
        }
      }
  });
}

void setup() {
  pinMode(WIS_TIJDEN, INPUT);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  WiFi.disconnect();                                                      
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(APSSID, APPSWD);
  SPIFFS.begin();
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(50, tft_positie_int, 4);
  spiffs_format_bool = false;
  if(digitalRead(WIS_TIJDEN) == true){
    tft.print("SPIFFS wordt geformatteerd");
    SPIFFS.format();
    for(int x = 1; x < 502; x++){
      memset(tijden_char[x], 0, sizeof(tijden_char[x]));
    }
    tft.fillScreen(TFT_BLACK);
    spiffs_format_bool = true;
  }
  else{
    tft.fillScreen(TFT_RED);
    tft.setRotation(1);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setCursor(50, 120, 2);
    tft.print("OPGESLAGEN TIJDEN WORDEN GEKOPIEERD NAAR BUFFER");
    memset(aantal_html_char, 0, sizeof(aantal_html_char));
    read_string(SPIFFS, "/aantal");
    aantal_html_string = String(lees_string.toInt() - 1);
    aantal_html_string.toCharArray(aantal_html_char, aantal_html_string.length() + 1);
    for(int x = 1; x < 502; x++){
      memset(tijden_char[x], 0, sizeof(tijden_char[x]));
      positie_string = tijd_string + String(x);
      positie_string.toCharArray(positie_char, positie_string.length() + 1);
      read_string(SPIFFS, positie_char);
      strcpy(tijden_char[x], lees_char);
    }
    tft.fillScreen(TFT_RED);
    tft.setCursor(50, tft_positie_int, 4);
    tft.print("TIJDEN NIET GEWIST");
  }
  html_input();
  scherm_update_long = millis();
}

void loop() {
  if(spiffs_format_bool == true){
    if(Serial2.available() > 0){
      while(Serial2.available() > 0){
        Serial2.readBytes((char*)&tijd, sizeof(tijd));
      }
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(50, tft_positie_int, 6);
      sprintf(chrono_char, "%02d:%02d:%02d:%03d:%03d", tijd.uren, tijd.minuten, tijd.seconden, tijd.milli_seconden, tijd.micro_seconden);
      tft.print(chrono_char);
      scherm_update_long = millis();
      positie_string = tijd_string + aantal_int;
      positie_string.toCharArray(positie_char, positie_string.length() + 1);
      write_char(SPIFFS, positie_char, chrono_char);
      strcpy(tijden_char[aantal_int], chrono_char);
      aantal_int ++;
      if(aantal_int > 501){
        aantal_int = 501;
      }
      aantal_html_int = aantal_int - 1;
      aantal_html_string = String(aantal_html_int);
      memset(aantal_html_char, 0, sizeof(aantal_html_char));
      aantal_html_string.toCharArray(aantal_html_char, aantal_html_string.length() + 1);
      aantal_string = String(aantal_int);
      memset(aantal_char, 0, sizeof(aantal_char));
      aantal_string.toCharArray(aantal_char, aantal_string.length() + 1);
      write_char(SPIFFS, "/aantal", aantal_char);
    }
    if((millis() - scherm_update_long) > 10000){
      tft_positie_int = tft_positie_int + tft_add_int;
      if((tft_positie_int > 269) || (tft_positie_int < 11)){
        tft_add_int = tft_add_int * -1;
      }
      tft.setCursor(50, tft_positie_int, 6);
      tft.fillScreen(TFT_BLACK);
      tft.print(chrono_char);
      scherm_update_long = millis();
    }
  }
  else{
    
    if((millis() - scherm_update_long) > 10000){
      tft_positie_int = tft_positie_int + tft_add_int;
      if((tft_positie_int > 269) || (tft_positie_int < 11)){
        tft_add_int = tft_add_int * -1;
      }
      tft.setCursor(50, tft_positie_int, 4);
      tft.fillScreen(TFT_RED);
      tft.print("TIJDEN NIET GEWIST");
      scherm_update_long = millis();
    }
  }
}
