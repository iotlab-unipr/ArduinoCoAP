#ifndef CoapMessage_h
#define CoapMessage_h
#include "Arduino.h"
#include "option.h"
#include "options.h"
#include "config.h"
#include <EthernetUdp.h>
#define VER 1

class CoapMessage {
   byte ver;
   byte type;
   byte* token;
   byte code;
   unsigned int messageId;
   char* payload;
   Options* options;
   byte tokenLen;
   byte payloadLen;
   IPAddress remoteIPAddr;
   unsigned int remotePort;
public:
   CoapMessage(char* packetBuffer, byte packetLen);
   CoapMessage(byte type, byte code, char* payload);
   CoapMessage(byte type, byte code, Options* opts, char* payload);
   CoapMessage(byte type, byte code, byte* token, char* payload);
   CoapMessage(unsigned int messageId);
   ~CoapMessage();
   char* toBuffer();
   void toString();
   byte* pickToken();
   unsigned int pickMessageId();
   boolean isRequest();
   byte getCode();
   byte getType();
   unsigned int getMessageId();
   byte* getToken();
   char* getPayload();
   Options* getOptions();
   Option* parseCoapOption(byte prevOptNum, char* buf, byte index);
   void setRemoteSocketAddress(IPAddress address, unsigned int port);
   IPAddress getRemoteIPAddr();
   unsigned int getRemotePort();
   void send(EthernetUDP& Udp, IPAddress address, unsigned int port);
};

#endif
