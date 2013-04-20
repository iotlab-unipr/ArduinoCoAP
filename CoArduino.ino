#include "option.h"
#include "options.h"
#include "coapmessage.h"
#include "config.h"

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <EthernetBonjour.h>
#include <dht.h>

#define DEBUG
#define DHT22_PIN 7
EthernetUDP Udp;
dht DHT;

char *ftoa(char *a, double f, int precision) {
  long p[] = {0,10,100};
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

//I DON'T CARE ABOUT ACK
/*void sendAck(CoapMessage* msg) {
 if(msg->getType()!=0) return;
 Serial.print("\n====ACK SENT====\n");
 CoapMessage* ack = new CoapMessage(2, 0, msg->getToken(), "");
 //ack->toString();
 ack->send(Udp, msg->getRemoteIPAddr(), msg->getRemotePort());
 delete ack;
 }*/

void onReceivedMessage(CoapMessage* msg) {
  //sendAck(msg);
  #ifdef DEBUG
    Serial.println("\n====MESSAGE RECEIVED=====\n");
    msg->toString();
  #endif
  
  byte responseCode;
  double value;
  int chk = DHT.read22(DHT22_PIN);
  if(msg->isRequest()) {
    char* reqService = msg->getOptions()->getOptionByNumber(11)->getValue();
    if(strcmp(reqService,"temp")==0) {
      Serial.println("eccolo");
      value = DHT.temperature;
    } else if(strcmp(reqService,"humidity")==0) {
      value = DHT.humidity;
    }
    switch(msg->getCode()) {
    case 1 :
      responseCode = 69;
      break;
    case 2 :
      responseCode = 65;
      break;
    case 3 :
      responseCode = 134;
      break;
    case 4 :
      responseCode = 134;
      break;
    default:
      responseCode = 134;
    }
    char val[6];
    ftoa(val,value, 2);
    CoapMessage* response = new CoapMessage(2, responseCode, msg->getToken(), val);
    #ifdef DEBUG
      response->toString();
    #endif
    response->send(Udp, msg->getRemoteIPAddr(), msg->getRemotePort());
    if(response != NULL){ 
      delete response;
      response = NULL;
    }
    if(reqService != NULL){
      free(reqService);
      reqService = NULL;
    }
  }
}

void setup() {
  Serial.begin(9600);
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  IPAddress ip(192, 168, 1, 102);
  int localPort = 8888;
  Ethernet.begin(mac, ip);
  Serial.println(Ethernet.localIP());
  Udp.begin(localPort);
  EthernetBonjour.begin();
  EthernetBonjour.addServiceRecord("Temp Coap Sensor._coap", localPort, MDNSServiceUDP, "\xApath=/temp");
  EthernetBonjour.addServiceRecord("Humidity Coap Sensor._coap", localPort, MDNSServiceUDP, "\xEpath=/humidity");
  Serial.println("Bonjourned!");

  //IF REGISTRATION BY PUT
  /*CoapMessage* init = new CoapMessage(1, 3, "temp");
   init->toString();
   init->send(Udp, server, 4000);
   delete init;
   init = NULL;*/
}

void loop() {
  EthernetBonjour.run();
  byte packetSize = Udp.parsePacket();
  if(packetSize)
  {
    char* packetBuffer = (char*)malloc(packetSize);
    #ifdef DEBUG
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remote = Udp.remoteIP();
      for (int i =0; i < 4; i++)
      {
        Serial.print(remote[i], DEC);
        if (i < 3)
          Serial.print(".");
      }
      Serial.print(", port: ");
      Serial.println(Udp.remotePort());
    #endif
    Udp.read(packetBuffer,packetSize);
    CoapMessage* msg = new CoapMessage(packetBuffer, packetSize);
    msg->setRemoteSocketAddress(Udp.remoteIP(), Udp.remotePort());
    onReceivedMessage(msg);
    if(packetBuffer != NULL) {
      free(packetBuffer);
      packetBuffer = NULL;
    }
    delete msg;
    msg = NULL;
    #ifdef DEBUG
      Serial.println("\nDeleted\n");
    #endif
  }
  delay(10);
}

