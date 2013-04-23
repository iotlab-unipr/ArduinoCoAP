/* 
University of Parma - Italy

This file is part of ArduinoCoAP

ArduinoCoAP is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Author:
Daniele Sportillo (daniele.sportillo@gmail.com)
*/

#ifndef CoapMessage_h
#define CoapMessage_h
#include "Arduino.h"
#include "option.h"
#include "options.h"
#include "config.h"
#include <EthernetUdp.h>

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
