#include <Arduino.h>
#include "option.h"
#include "options.h"
#include "coapmessage.h"
#include "config.h"

#include "Arduino.h"
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 2);
IPAddress server(192, 168, 1, 1);
unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
byte index=0;
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
//Service* service;

void sendAck(CoapMessage* msg) {
  if(msg->getType()!=0) return;
  Serial.print("\n====ACK SENT====\n");
  CoapMessage* ack = new CoapMessage(2, 0, msg->getToken(), "");
  ack->toString();
  ack->send(Udp, msg->getRemoteIPAddr(), msg->getRemotePort());
  delete ack;
}

void onReceivedMessage(CoapMessage* msg) {
  //sendAck(msg);
  byte responseCode;
  Serial.flush();
  Serial.println("\n====MESSAGE RECEIVED=====\n");
  delay(100);
  msg->toString();
  char* reqService = msg->getOptions()->getOptionByNumber(11)->getValue();
  if(msg->isRequest()) {
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
    CoapMessage* response = new CoapMessage(msg->getType(), responseCode, msg->getToken(), "42gradi");
    response->toString();
    response->send(Udp, msg->getRemoteIPAddr(), msg->getRemotePort());
    delete response;
  }
}

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  Serial.begin(9600);
  //Start SD
  /*if (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }else{
  Serial.println("initialization done.");
  service = new Service();
  }*/
  CoapMessage* init = new CoapMessage(1, 3, "temp");
  init->toString();
  init->send(Udp, server, 4000);
  delete init;
  init = NULL;
}

void loop() {
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    char* packetBuffer = (char*)malloc(packetSize);

    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port: ");
    Serial.println(Udp.remotePort());

    Udp.read(packetBuffer,packetSize);
//    Serial.println(packetBuffer);
    CoapMessage* msg = new CoapMessage(packetBuffer, packetSize);
    msg->setRemoteSocketAddress(Udp.remoteIP(), Udp.remotePort());
    onReceivedMessage(msg);
    //free(packetBuffer);
    delete msg;
    Serial.println("\nFATTO\n");
  }
  delay(10);
}
