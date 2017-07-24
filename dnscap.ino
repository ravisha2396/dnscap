#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include "cap.h"
#include "index.h"
#include "tag1.h"
#include "tag2.h"
#include "tag3.h"
#include "tag4.h"


const byte DNS_PORT = 53; // UDP port 53 used by DNS requests and replies
IPAddress apIP(192, 168, 1, 1); //IP address assignment for the server we are configuring.
DNSServer dnsServer; //dnsserver object to configure the dns of the server we create
ESP8266WebServer webServer(80); //declare a object of type ESP8266WebServer which is active at port 80
 String s = MAIN_page; 
 String h=Second_page;
 String p=tag1;
 String q=tag2;
 String r=tag3;
 String l=tag4;
void handleNotFound() {
 // Serial.print("\t\t\t\t URI Not Found: ");
  //Serial.println(server.uri());
  webServer.send(200, "text/html", s);
}
void handle_L2() {//?v=0
    webServer.send(200, "text/html", h);

}
void handle_L3() {//?v=0
    webServer.send(200, "text/html", p);

}

void handle_L4() {//?v=0
    webServer.send(200, "text/html", q);

}
void handle_L5() {//?v=0
    webServer.send(200, "text/html", r);

}
void handle_L6() {//?v=0
    webServer.send(200, "text/html", l);

}

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Manchester United");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound(handleNotFound); 
    webServer.on("/L2", handle_L2);
    webServer.on("/L3",handle_L3);
   webServer.on("/L4",handle_L4);
    webServer.on("/L5",handle_L5);
    webServer.on("/L6",handle_L6);

   
  webServer.begin();
}


void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}




