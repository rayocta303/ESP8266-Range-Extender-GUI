/*
 * WIFI Range Extender NEW GUI
 * CODED BY HANIF BAGUS SAPUTRA A.K.A ARTASK MEDIA
 * https://github.com/rayocta303/ESP8266-Range-Extender-GUI
 * INDONESIA DEVELOPER
 * SPECIAL THANKS TO ALLAH SWT
 * 
 * DONATION LINKS 
 * https://trakteer.id/artask-technology-5bkbr
 */
#if LWIP_FEATURES && !LWIP_IPV6

#define HAVE_NETDUMP 0
//#include <WiFiNINA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <lwip/napt.h>
#include <lwip/dns.h>
#include <LwipDhcpServer.h>

#define NAPT 1000
#define NAPT_PORT 10

#if HAVE_NETDUMP

#include <NetDump.h>

void dump(int netif_idx, const char* data, size_t len, int out, int success) {
  (void)success;
  Serial.print(out ? F("out ") : F(" in "));
  Serial.printf("%d ", netif_idx);

  // optional filter example: if (netDump_is_ARP(data))
  {
    netDump(Serial, data, len);
    //netDumpHex(Serial, data, len);
  }
}
#endif

// MY FUNCTIONS
bool testwifi() {
  Serial.printf("\nTesting connection with '%s'\n", WiFi.SSID().c_str());
  int count = 0;
  digitalWrite(2, LOW);
  while (count < 20) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.printf("\nWiFi Connected! \nSTA: %s (dns: %s / %s)\n\n",
                    WiFi.localIP().toString().c_str(),
                    WiFi.dnsIP(0).toString().c_str(),
                    WiFi.dnsIP(1).toString().c_str());

      // give DNS servers to AP side
      dhcpSoftAP.dhcps_set_dns(0, WiFi.dnsIP(0));
      dhcpSoftAP.dhcps_set_dns(1, WiFi.dnsIP(1));
      digitalWrite(2, HIGH);
      return true;
    }
    Serial.print(".");
    delay(1000);
    count++;
  }
  Serial.printf("\nCan't connect to WiFi, connect to AP '%s' and configure...\n\n", WiFi.softAPSSID());
  return false;
}

// SERVER
ESP8266WebServer server(80);

String content;
void serverconfig() {

  server.begin();

  server.on("/", []() {
    content = "<html><head><meta name='viewport' content='initial-scale=1.0, width=device-width'> <title>ESP8266 Dashboard</title> <style> /* Global */ body { background: #36393e; color: #bfbfbf; font-family: sans-serif; margin: 0; } h1 { font-size: 1.7rem; margin-top: 1rem; background: #2f3136; color: #bfbfbb; padding: 0.2em 1em; border-radius: 3px; border-left: solid red 5px; font-weight: 100; } h2 { font-size: 1.1rem; margin-top: 1rem; background: #2f3136; color: #bfbfbb; padding: 0.4em 1.8em; border-radius: 3px; border-left: solid #20c20e 5px; font-weight: 100; } table{ border-collapse: collapse; } label{ line-height: 38px; } p{ margin: 0.5em 0; } .left { float: left; } .right { float: right; } .bold { font-weight: bold; } .red{ color: #F04747; } .green{ color:#43B581; } .clear { clear: both; } .centered{ text-align: center; } .labelFix { line-height: 44px; } .clickable{ cursor: pointer; } footer { font-size: .95em; text-align: center; margin-top: 3em; margin-bottom: 3em; } ";
    content += "keyframes fadeIn { 0% {opacity: 0;} 100% {opacity: 1;} } hr { background: #3e4146; } a { color: #5281bb; text-decoration: none; } a:hover { color: #95b8e4; } li{ margin: 4px 0; } /* Tables */ table { width: 100%; min-width: 400px; margin-bottom: 2em; } td{ word-break: break-all; } th{ word-break: break-word; } th, td { padding: 10px 6px; text-align: left; border-bottom: 1px solid #5d5d5d; } nav { display: block; background: #2f3136; font-weight: bold; padding: 10px; } /* Inputs and buttons */ .upload-script, .button, button, input[type=\"submit\"], input[type=\"reset\"], input[type=\"button\"] { display: inline-block; height: 38px; padding: 0 20px; color:#fff; text-align: center; font-size: 11px; font-weight: 600; line-height: 38px; letter-spacing: .1rem; text-transform: uppercase; text-decoration: none; white-space: nowrap; background: #2f3136; border-radius: 4px; border: none; cursor: pointer; box-sizing: border-box; } button:hover, input[type=\"submit\"]:hover, input[type=\"reset\"]:hover, input[type=\"button\"]:hover { background: #42444a; } button:active, input[type=\"submit\"]:active, input[type=\"reset\"]:active, input[type=\"button\"]:active { transform: scale(.93); } button:disabled:hover, input[type=\"submit\"]:disabled:hover, input[type=\"reset\"]:disabled:hover, input[type=\"button\"]:disabled:hover { background: white; cursor: not-allowed; opacity: 0.40; filter: alpha(opacity=40); transform: scale(1); } button::-moz-focus-inner { border: 0; } /* Forms */ input[type=\"email\"], input[type=\"number\"], input[type=\"search\"], input[type=\"text\"], input[type=\"tel\"], input[type=\"url\"], input[type=\"password\"], textarea, select { height: 38px; padding: 6px 10px; /* The 6px vertically centers text on FF, ignored by Webkit */ background-color: #2f3136; border-radius: 4px; box-shadow: none; box-sizing: border-box; color: #d4d4d4; border: none; width: 5em; } input[type=\"text\"] { width: 15em; } input[type=\"password\"] { width: 15em; } /* ==== GRID SYSTEM ==== */ .container { width: 100%; margin-left: auto; margin-right: auto; max-width: 1140px; } .row { position: relative; width: 100%; } .row [class^=\"col\"] { float: left; margin: 0.25rem 2%; min-height: 0.125rem; } .col-1, .col-2, .col-3, .col-4, .col-5, .col-6, .col-7, .col-8, .col-9, .col-10, .col-11, .col-12 { width: 96%; } .row::after { content: \"\"; display: table; clear: both; } .hidden-sm { display: none; } ";
    content += "media only screen and (min-width: 24em) { .col-1 { width: 4.33%; } .col-2 { width: 12.66%; } .col-3 { width: 21%; } .col-4 { width: 29.33%; } .col-5 { width: 37.66%; } .col-6 { width: 46%; } .col-7 { width: 54.33%; } .col-8 { width: 62.66%; } .col-9 { width: 71%; } .col-10 { width: 79.33%; } .col-11 { width: 87.66%; } .col-12 { width: 96%; } .hidden-sm { display: block; } } ";
    content += "nav a { color: inherit; padding: 0 .5em; } .menu { list-style-type: none; margin: 0; padding: 0; margin: 0 auto; display: flex; flex-direction: row; display:block; } .menu li { margin: 10px 20px 10px 0; display: inline-block; } .menu li:last-child { float: right; }</style> </head> <body>";
    // content += "<nav style=\"font-size: 1.5rem;\"><b><span style='color:red;'>ESP8266 WiFi</span> Range Extender</b> <a style='text-align:right;' href='./survey'>Survey</a></nav>";
    content += "<nav><ul class=\"menu\"><li><a href=\"/\">Config</a></li><li><a href=\"/survey\">Survey</a></li><li><a href=\"https://trakteer.id/artask-technology-5bkbr\">Donate</a></li> </ul> </nav>";
    // if (WiFi.status() != WL_CONNECTED) {
    //   content += "<div id=\"status\" style=\"text-align: center;text-transform: capitalize;padding: 5px;color: #fff;position: sticky;top: 0;z-index: 99;background-color:#20c20e;\">Connected</div>";
    // } else {
    //   content += "<div id=\"status\" style=\"text-align: center;text-transform: capitalize;padding: 5px;color: #fff;position: sticky;top: 0;z-index: 99;background-color:red;\">Disconnected</div>";
    // }
    content += "<div class='container'><div class='row'><div class='col-12'>";
    content += "<h2>Wireless Station Settings</h2>";
    content += "<form method='post'>";
    content += "<label>SSID:</label><br><input type='text' name='stassid' placeholder='";
    content += WiFi.SSID();
    content += "' length=32><br>";
    content += "<label>Password:</label><br><input type='password' placeholder='********' name='stapsk' minlength=8 maxlength=63><br><small>Must be at least 8 characters or blank!</small><br><br>";
    //content += "<button style='display:inline-block;' type='submit' formaction='survey'>Survey</button>";
    content += "<button style='display:inline-block;' type='submit' formaction='stasettings'>Save</button>";
    //content += "<button style='margin:0 10px; display:inline-block;' type='submit' formaction='tempstasettings'>Save Temporarily (Until Reboot)</button>";
    content += "</form>";
    content += "<h2>Wireless Access Point Settings</h2>";
    content += "<form method='post'>";
    content += "<label>SSID:</label><br><input type='text' name='apssid' placeholder='";
    content += WiFi.softAPSSID();
    content += "' length=32><br>";
    content += "<label>Password:</label><br><input type='password' placeholder='";
    content += WiFi.softAPPSK();
    content += "' name='appsk' minlength=8 maxlength=63><br><small>Must be at least 8 characters or blank!</small><br><br>";
    content += "<button style='display:inline-block;' type='submit' formaction='apsettings'>Save</button>";
    //content += "<button style='margin:0 10px; display:inline-block;' type='submit' formaction='tempapsettings'>Save Temporarily (Until Reboot)</button>";
    content += "</form>";
    content += "<h2>Miscellaneous</h2>";
    content += "<form method='get' action='reboot'><input type='submit' value='Reboot'></form>";
    content += "</div></div></div><footer><span id='version'>Version 2.0 STABLE</span><br><br><a href='https://github.com/rayocta303/ESP8266-Range-Extender-GUI' target='_blank'>Github</a> | <a href='/info'>Credit</a></footer></body></html>";
    server.send(200, "text/html", content);
  });

  server.onNotFound([]() {
    server.send(404, "text/html", "<center><h1>404 Not Found</h1><br><hr><p>Maaf, Url yang Anda Cari Tidak Tersedia</p></center>");
  });

  server.on("/survey", []() {
    String _scanHTML;
    uint8_t encryptionType;
    int32_t RSSI;
    uint8_t* BSSID;
    int32_t channel;

    int numberOfNetworks = WiFi.scanNetworks();
    _scanHTML = "<html><head><meta name='viewport' content='initial-scale=1.0, width=device-width'> <title>ESP8266 Dashboard</title> <style> /* Global */ body { background: #36393e; color: #bfbfbf; font-family: sans-serif; margin: 0; } h1 { font-size: 1.7rem; margin-top: 1rem; background: #2f3136; color: #bfbfbb; padding: 0.2em 1em; border-radius: 3px; border-left: solid red 5px; font-weight: 100; } h2 { font-size: 1.1rem; margin-top: 1rem; background: #2f3136; color: #bfbfbb; padding: 0.4em 1.8em; border-radius: 3px; border-left: solid #20c20e 5px; font-weight: 100; } table{ border-collapse: collapse; } label{ line-height: 38px; } p{ margin: 0.5em 0; } .left { float: left; } .right { float: right; } .bold { font-weight: bold; } .red{ color: #F04747; } .green{ color:#43B581; } .clear { clear: both; } .centered{ text-align: center; } .labelFix { line-height: 44px; } .clickable{ cursor: pointer; } footer { font-size: .95em; text-align: center; margin-top: 3em; margin-bottom: 3em; } ";
    _scanHTML += "keyframes fadeIn { 0% {opacity: 0;} 100% {opacity: 1;} } hr { background: #3e4146; } a { color: #5281bb; text-decoration: none; } a:hover { color: #95b8e4; } li{ margin: 4px 0; } /* Tables */ table { width: 100%; min-width: 400px; margin-bottom: 2em; } td{ word-break: break-all; } th{ word-break: break-word; } th, td { padding: 10px 6px; text-align: left; border-bottom: 1px solid #5d5d5d; } nav { display: block; background: #2f3136; font-weight: bold; padding: 10px; } /* Inputs and buttons */ .upload-script, .button, button, input[type=\"submit\"], input[type=\"reset\"], input[type=\"button\"] { display: inline-block; height: 38px; padding: 0 20px; color:#fff; text-align: center; font-size: 11px; font-weight: 600; line-height: 38px; letter-spacing: .1rem; text-transform: uppercase; text-decoration: none; white-space: nowrap; background: #2f3136; border-radius: 4px; border: none; cursor: pointer; box-sizing: border-box; } button:hover, input[type=\"submit\"]:hover, input[type=\"reset\"]:hover, input[type=\"button\"]:hover { background: #42444a; } button:active, input[type=\"submit\"]:active, input[type=\"reset\"]:active, input[type=\"button\"]:active { transform: scale(.93); } button:disabled:hover, input[type=\"submit\"]:disabled:hover, input[type=\"reset\"]:disabled:hover, input[type=\"button\"]:disabled:hover { background: white; cursor: not-allowed; opacity: 0.40; filter: alpha(opacity=40); transform: scale(1); } button::-moz-focus-inner { border: 0; } /* Forms */ input[type=\"email\"], input[type=\"number\"], input[type=\"search\"], input[type=\"text\"], input[type=\"tel\"], input[type=\"url\"], input[type=\"password\"], textarea, select { height: 38px; padding: 6px 10px; /* The 6px vertically centers text on FF, ignored by Webkit */ background-color: #2f3136; border-radius: 4px; box-shadow: none; box-sizing: border-box; color: #d4d4d4; border: none; width: 5em; } input[type=\"text\"] { width: 15em; } input[type=\"password\"] { width: 15em; } /* ==== GRID SYSTEM ==== */ .container { width: 100%; margin-left: auto; margin-right: auto; max-width: 1140px; } .row { position: relative; width: 100%; } .row [class^=\"col\"] { float: left; margin: 0.25rem 2%; min-height: 0.125rem; } .col-1, .col-2, .col-3, .col-4, .col-5, .col-6, .col-7, .col-8, .col-9, .col-10, .col-11, .col-12 { width: 96%; } .row::after { content: \"\"; display: table; clear: both; } .hidden-sm { display: none; } ";
    _scanHTML += "media only screen and (min-width: 24em) { .col-1 { width: 4.33%; } .col-2 { width: 12.66%; } .col-3 { width: 21%; } .col-4 { width: 29.33%; } .col-5 { width: 37.66%; } .col-6 { width: 46%; } .col-7 { width: 54.33%; } .col-8 { width: 62.66%; } .col-9 { width: 71%; } .col-10 { width: 79.33%; } .col-11 { width: 87.66%; } .col-12 { width: 96%; } .hidden-sm { display: block; } } ";
    _scanHTML += "nav a { color: inherit; padding: 0 .5em; } .menu { list-style-type: none; margin: 0; padding: 0; margin: 0 auto; display: flex; flex-direction: row; display:block; } .menu li { margin: 10px 20px 10px 0; display: inline-block; } .menu li:last-child { float: right; }</style> </head> <body>";
    _scanHTML += "<nav><ul class=\"menu\"><li><a href=\"/\">Config</a></li><li><a href=\"/survey\">Survey</a></li><li><a href=\"https://trakteer.id/artask-technology-5bkbr\">Donate</a></li> </ul> </nav>";
    _scanHTML += "<div class='container'><div class='row'><div class='col-12'>";
    _scanHTML += "<h2>Survey Wi-Fi Networks</h2>";
    _scanHTML +=  "<div class='col-12' style='overflow:auto;'><table>";
    _scanHTML +=  "<tr><th>CH</th><th>SSID</th><th>MAC</th><th>SIGNAL</th><th>SECURITY</th></tr>";
    for (int i = 0; i < numberOfNetworks; i++) {
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));

      //_scanHTML += "<tr><td>" + WiFi.channel(i) + "</td><td>" + WiFi.SSID(i) + "</td><td>" + WiFi.BSSIDstr(i) + "</td><td>" + WiFi.RSSI(i) + "dBm</td>";
      _scanHTML += "<tr><td>";
      _scanHTML += WiFi.channel(i);
      _scanHTML += "</td><td>" + WiFi.SSID(i) + "</td><td>" + WiFi.BSSIDstr(i) + "</td><td>" + WiFi.RSSI(i) + "dBm</td>";
      if (WiFi.encryptionType(i) == ENC_TYPE_NONE)
      {
        Serial.println(" <<***OPEN***>>");
        _scanHTML += "<td>OPEN</td></tr>";
      } else if (WiFi.encryptionType(i) == ENC_TYPE_WEP ) {
        Serial.println();
        _scanHTML += "<td style='color:red;'>WEP</td></tr>";
      } else if (WiFi.encryptionType(i) == ENC_TYPE_TKIP ) {
        Serial.println();
        _scanHTML += "<td style='color:red;'>WPA / PSK</td></tr>";
      } else if (WiFi.encryptionType(i) == ENC_TYPE_CCMP ) {
        Serial.println();
        _scanHTML += "<td style='color:red;'>WPA2 / PSK</td></tr>";
      } else if (WiFi.encryptionType(i) == ENC_TYPE_AUTO ) {
        Serial.println();
        _scanHTML += "<td style='color:red;'>WPA / WPA2 / PSK</td></tr>";
      } else {
        Serial.println();
        _scanHTML += "<td style='color:yellow;'>Unknown</td></tr>";
      }

      Serial.println("-----------------------");
    }
    _scanHTML += "</table>";
    _scanHTML += "</div></div></div><footer><span id='version'>Version 2.0 STABLE</span><br><br><a href='https://github.com/rayocta303/ESP8266-Range-Extender-GUI' target='_blank'>Github</a> | <a href='/info'>Credit</a></footer></body></html>";
    server.send(404, "text/html", _scanHTML);
  });
  server.on("/info", []() {
    //about page
    String _aboutHtml;
    _aboutHtml = "<html><head><meta name='viewport' content='initial-scale=1.0, width=device-width'> <title>ESP8266 Dashboard</title> <style> /* Global */ body { background: #36393e; color: #bfbfbf; font-family: sans-serif; margin: 0; } h1 { font-size: 1.7rem; margin-top: 1rem; background: #2f3136; color: #bfbfbb; padding: 0.2em 1em; border-radius: 3px; border-left: solid red 5px; font-weight: 100; } h2 { font-size: 1.1rem; margin-top: 1rem; background: #2f3136; color: #bfbfbb; padding: 0.4em 1.8em; border-radius: 3px; border-left: solid #20c20e 5px; font-weight: 100; } table{ border-collapse: collapse; } label{ line-height: 38px; } p{ margin: 0.5em 0; } .left { float: left; } .right { float: right; } .bold { font-weight: bold; } .red{ color: #F04747; } .green{ color:#43B581; } .clear { clear: both; } .centered{ text-align: center; } .labelFix { line-height: 44px; } .clickable{ cursor: pointer; } footer { font-size: .95em; text-align: center; margin-top: 3em; margin-bottom: 3em; } ";
    _aboutHtml += "keyframes fadeIn { 0% {opacity: 0;} 100% {opacity: 1;} } hr { background: #3e4146; } a { color: #5281bb; text-decoration: none; } a:hover { color: #95b8e4; } li{ margin: 4px 0; } /* Tables */ table { width: 100%; min-width: 400px; margin-bottom: 2em; } td{ word-break: break-all; } th{ word-break: break-word; } th, td { padding: 10px 6px; text-align: left; border-bottom: 1px solid #5d5d5d; } nav { display: block; background: #2f3136; font-weight: bold; padding: 10px; } /* Inputs and buttons */ .upload-script, .button, button, input[type=\"submit\"], input[type=\"reset\"], input[type=\"button\"] { display: inline-block; height: 38px; padding: 0 20px; color:#fff; text-align: center; font-size: 11px; font-weight: 600; line-height: 38px; letter-spacing: .1rem; text-transform: uppercase; text-decoration: none; white-space: nowrap; background: #2f3136; border-radius: 4px; border: none; cursor: pointer; box-sizing: border-box; } button:hover, input[type=\"submit\"]:hover, input[type=\"reset\"]:hover, input[type=\"button\"]:hover { background: #42444a; } button:active, input[type=\"submit\"]:active, input[type=\"reset\"]:active, input[type=\"button\"]:active { transform: scale(.93); } button:disabled:hover, input[type=\"submit\"]:disabled:hover, input[type=\"reset\"]:disabled:hover, input[type=\"button\"]:disabled:hover { background: white; cursor: not-allowed; opacity: 0.40; filter: alpha(opacity=40); transform: scale(1); } button::-moz-focus-inner { border: 0; } /* Forms */ input[type=\"email\"], input[type=\"number\"], input[type=\"search\"], input[type=\"text\"], input[type=\"tel\"], input[type=\"url\"], input[type=\"password\"], textarea, select { height: 38px; padding: 6px 10px; /* The 6px vertically centers text on FF, ignored by Webkit */ background-color: #2f3136; border-radius: 4px; box-shadow: none; box-sizing: border-box; color: #d4d4d4; border: none; width: 5em; } input[type=\"text\"] { width: 15em; } input[type=\"password\"] { width: 15em; } /* ==== GRID SYSTEM ==== */ .container { width: 100%; margin-left: auto; margin-right: auto; max-width: 1140px; } .row { position: relative; width: 100%; } .row [class^=\"col\"] { float: left; margin: 0.25rem 2%; min-height: 0.125rem; } .col-1, .col-2, .col-3, .col-4, .col-5, .col-6, .col-7, .col-8, .col-9, .col-10, .col-11, .col-12 { width: 96%; } .row::after { content: \"\"; display: table; clear: both; } .hidden-sm { display: none; } ";
    _aboutHtml += "media only screen and (min-width: 24em) { .col-1 { width: 4.33%; } .col-2 { width: 12.66%; } .col-3 { width: 21%; } .col-4 { width: 29.33%; } .col-5 { width: 37.66%; } .col-6 { width: 46%; } .col-7 { width: 54.33%; } .col-8 { width: 62.66%; } .col-9 { width: 71%; } .col-10 { width: 79.33%; } .col-11 { width: 87.66%; } .col-12 { width: 96%; } .hidden-sm { display: block; } } ";
    _aboutHtml += "nav a { color: inherit; padding: 0 .5em; } .menu { list-style-type: none; margin: 0; padding: 0; margin: 0 auto; display: flex; flex-direction: row; display:block; } .menu li { margin: 10px 20px 10px 0; display: inline-block; } .menu li:last-child { float: right; }</style> </head> <body>";
    _aboutHtml += "<nav><ul class=\"menu\"><li><a href=\"/\">Config</a></li><li><a href=\"/survey\">Survey</a></li><li><a href=\"https://trakteer.id/artask-technology-5bkbr\">Donate</a></li> </ul> </nav>";
    _aboutHtml += "<div class='container'><div class='row'><div class='col-12'>";
    _aboutHtml += "<h1>Credit</h1>";
    _aboutHtml += "<h2>Acknowledgements</h2> <p> A huge thanks to:<br> <ul> <li><a href=\"http://github.com/gulshankumark\" target=\"_blank\">gulshankumark</a></li> <li><a href=\"http://github.com/martin-ger\" target=\"_blank\">martin-ger</a></li> <li><a href=\"http://github.com/ahmetozer\" target=\"_blank\">ahmetozer</a></li> <li><a href=\"http://github.com/SpacehuhnTech\" target=\"_blank\">SpacehuhnTech</a></li> <li><a href=\"http://github.com/rayocta303\" target=\"_blank\">rayocta303</a></li> </ul> for helping out with various things regarding this project and keeping it alive!<br> <br> Also thanks to everyone working on the libraries used for this project:<br> <ul> <li><a href=\"https://github.com/esp8266/Arduino/blob\" target=\"_blank\">ESP8266WiFi</a></li> <li><a href=\"https://github.com/esp8266/ESPWebServer\" target=\"_blank\">ESPWebServer</a></li> <li><a href=\"https://github.com/martin-ger/esp-open-lwip\" target=\"_blank\">ESP8266 Open lwIP </a></li> <li><a href=\"https://github.com/esp8266/Arduino/tree/master/cores/esp8266\" target=\"_blank\">lwIP DCHP Server</a></li> </ul> We also thank Espressif and their community for this awesome chip and all the software and hardware projects around it and the countless tutorials you can find online!<br> </p>";
    _aboutHtml += "<h2>License</h2> <p> <b>In regards to the firmware:</b><br> <br> MIT License<br> Copyright (c) 2022 Artask Technologies<br> <br> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:<br> <br> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.<br> <br> THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. </p>";
    _aboutHtml += "</div></div></div><footer><span id='version'>Version 2.0 STABLE</span><br><br><a href='https://github.com/rayocta303/ESP8266-Range-Extender-GUI' target='_blank'>Github</a> | <a href='/info'>Credit</a></footer></body></html>";
    server.send(200, "text/html", _aboutHtml);

  });
  server.on("/stasettings", []() {
    String stassid = server.arg("stassid");
    String stapsk = server.arg("stapsk");
    //String staip = server.arg("staip");
    if (stassid.length() > 0) {
      server.send(200, "text/plain", "Settings Recieved");
      Serial.printf("\n\nAttempting to connect to '%s' using password '%s' \n", stassid.c_str(), stapsk.c_str());
      WiFi.persistent(true);
      WiFi.begin(stassid, stapsk);
      testwifi();
    }
  });

  server.on("/tempstasettings", []() {
    String stassid = server.arg("stassid");
    String stapsk = server.arg("stapsk");
    //String staip = server.arg("staip");
    if (stassid.length() > 0) {
      server.send(200, "text/plain", "Settings Recieved");
      Serial.printf("\n\nAttempting to connect to '%s' using password '%s' \n", stassid.c_str(), stapsk.c_str());
      WiFi.persistent(false);
      WiFi.begin(stassid, stapsk);
      testwifi();
    }
  });

  server.on("/apsettings", []() {
    String apssid = server.arg("apssid");
    String appsk = server.arg("appsk");
    if (apssid.length() > 0) {
      server.send(200, "text/plain", "Settings Recieved");
      Serial.printf("\n\nSetting AP Credentials \nSSID: %s \nPassword: %s \n", apssid.c_str(), appsk.c_str());
      WiFi.persistent(true);
      WiFi.softAP(apssid, appsk);
    }
  });

  server.on("/tempapsettings", []() {
    String apssid = server.arg("apssid");
    String appsk = server.arg("appsk");
    if (apssid.length() > 0) {
      server.send(200, "text/plain", "Settings Recieved");
      Serial.printf("\n\nSetting Temporary AP Credentials \nSSID: %s \nPassword: %s \n", apssid.c_str(), appsk.c_str());
      WiFi.persistent(false);
      WiFi.softAP(apssid, appsk);
    }
  });

  server.on("/reboot", []() {
    server.send(200, "text/plain", "Rebooting now...");
    delay(5000);
    ESP.reset();
  });
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  delay(1000);
  Serial.printf("\n\nESP8266 Range Extender");
  Serial.printf("\nDeveloped By Hanif Bagus Saputra");
  Serial.printf("\nDonate : https://trakteer.id/artask-technology-5bkbr\n");
  Serial.printf("Heap on start: %d\n", ESP.getFreeHeap());

#if HAVE_NETDUMP
  phy_capture = dump;
#endif

  WiFi.setPhyMode(WIFI_PHY_MODE_11N); // Set radio type to N
  WiFi.mode(WIFI_AP_STA);
  WiFi.persistent(false);
  WiFi.begin(); // Use stored credentials to connect to network
  testwifi();
  WiFi.softAPConfig(  // Set IP Address, Gateway and Subnet
    IPAddress(192, 168, 0, 254),
    IPAddress(192, 168, 0, 254),
    IPAddress(255, 255, 255, 0));

  WiFi.softAP(WiFi.softAPSSID(), WiFi.softAPPSK()); // Use stored credentials to create AP

  Serial.printf("Heap before: %d\n", ESP.getFreeHeap());
  err_t ret = ip_napt_init(NAPT, NAPT_PORT);
  Serial.printf("ip_napt_init(%d,%d): ret=%d (OK=%d)\n", NAPT, NAPT_PORT, (int)ret, (int)ERR_OK);
  if (ret == ERR_OK) {
    ret = ip_napt_enable_no(SOFTAP_IF, 1);
    Serial.printf("ip_napt_enable_no(SOFTAP_IF): ret=%d (OK=%d)\n", (int)ret, (int)ERR_OK);
    if (ret == ERR_OK) {
      Serial.printf("\nWiFi Network '%s' with Passowrd '%s' and IP '%s' is now setup\n", WiFi.softAPSSID(), WiFi.softAPPSK().c_str(), WiFi.softAPIP().toString().c_str());
    }
  }
  Serial.printf("Heap after napt init: %d\n", ESP.getFreeHeap());
  if (ret != ERR_OK) {
    Serial.printf("NAPT initialization failed\n");
  }

  serverconfig();
}

#else

void setup() {
  Serial.begin(115200);
  Serial.printf("\n\nNAPT not supported in this configuration\n");
}

#endif

void loop() {

  server.handleClient();

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, LOW);
  }
  else {
    digitalWrite(2, HIGH);
  }

}
