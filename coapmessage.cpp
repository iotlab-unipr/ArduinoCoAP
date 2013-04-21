#include "coapmessage.h"

CoapMessage::CoapMessage(byte type, byte code, Options* opts, char* payload) {
  this->ver = VER;
  this->type = type;
  this->code = code;
  this->messageId = this->pickMessageId();
  this->options = opts;
  this->token = pickToken();
  this->tokenLen = 2;
  this->payloadLen = strlen(payload);
  this->payload = (char*)malloc(this->payloadLen);
  strcpy(this->payload, payload);
}

CoapMessage::CoapMessage(byte type, byte code, char* payload) {
  this->ver = VER;
  this->type = type;
  this->code = code;
  this->messageId = this->pickMessageId();
  this->options = NULL;
  this->tokenLen = 2;
  this->token = pickToken();
  this->payloadLen = strlen(payload);
  this->payload = (char*)malloc(this->payloadLen);
  strcpy(this->payload, payload);
}

CoapMessage::CoapMessage(byte type, byte code, byte* token, char* payload) {
  this->ver = VER;
  this->type = type;
  this->code = code;
  this->messageId = this->pickMessageId();
  this->options = NULL;
  this->tokenLen = 2;
  this->token = (byte*)malloc(this->tokenLen);
  for(byte i=0; i<this->tokenLen; i++)
    this->token[i] = token[i];
  this->payloadLen = strlen(payload);
  this->payload = (char*)malloc(this->payloadLen);
  strcpy(this->payload, payload);
}

CoapMessage::CoapMessage(unsigned int messageId) {
  this->ver = VER;
  this->type = 2;
  this->code = 0;
  this->messageId = messageId;
  this->options = NULL;
  this->token = NULL;
  this->payload = NULL;
}

CoapMessage::~CoapMessage() {
  if(this->options != NULL){
    delete this->options;
    this->options = NULL;
  }
  if(this->token != NULL){
     free(this->token);
     this->token = NULL;  
  }
  if(this->payload != NULL){
    free(this->payload);
    this->payload = NULL;
  }
}

Option* CoapMessage::parseCoapOption(byte prevOptNum, char* buf, byte index) {
  Option* opt = new Option();
  opt->init(prevOptNum, buf, index);
  return opt;
}

void CoapMessage::send(EthernetUDP& Udp, IPAddress address, unsigned int port) {
  byte* packet = (byte*)this->toBuffer();
  Udp.beginPacket(address, port);
  Udp.write(&packet[1], packet[0]);
  Udp.endPacket();
  free(packet);
  packet = NULL;
}

CoapMessage::CoapMessage(char* packetBuffer, byte len) {
  byte index = 0;
  this->ver=(byte)(packetBuffer[index]>>6)&0x3;
  this->type=(byte)((packetBuffer[index]>>4)&0x3);
  this->tokenLen=(byte)packetBuffer[index++]&0xf;
  this->code=(byte)packetBuffer[index++]&0xff;
  this->messageId=((packetBuffer[index]&0xff)<<8)+(packetBuffer[index+1]&0xff);
  index += 2;
  if (tokenLen>0) {
    this->token = (byte*)malloc(tokenLen*sizeof(byte));
    if (token == NULL) Serial.println("MEMORY ERROR");
    for (int i=0; i<tokenLen; i++) {
      this->token[i] = (byte)packetBuffer[index++];
    }  
  }else{
    this->token = NULL;
  }
  this->options = new Options();
  byte prevOptNum=0;
  while (index<len && packetBuffer[index]!=PAYLOAD_MARKER) {
    Option* opt = this->parseCoapOption(prevOptNum, packetBuffer, index);
    this->options->addOption(opt, true);
    index += opt->getLen(prevOptNum);
    prevOptNum = opt->getNumber();
  }
  if (index<len) {
    index++;
    this->payloadLen = len-index;
    this->payload = (char*)malloc(payloadLen*sizeof(char));
    for (byte i=0; i<payloadLen; i++) {
      this->payload[i] = (char)packetBuffer[i+index];
    }
  }else{
    this->payloadLen = 0;
    this->payload = NULL;
  }
}

byte* CoapMessage::pickToken(){
  byte* t = (byte*)malloc(2);
  t[0] = 0xe6;
  t[1] = 0xbb;
  return t;
  int n = random(255);
  byte *token = (byte*)&n;
}

unsigned int CoapMessage::pickMessageId() {
  return rand() % 20000;
}

char* CoapMessage::toBuffer() {
  byte index=1;
  char* buf = (char*)malloc(UDP_TX_PACKET_MAX_SIZE);
  buf[index++] = (char)(((ver&0x3)<<6) | ((type&0x3)<<4) | (tokenLen&0xf));
  buf[index++] = (char)0xff&code;
  buf[index++] = (char)0xFF& (messageId >> 8);
  buf[index++] = (char)0xFF & messageId;
  buf[index++] = (char)token[0];
  buf[index++] = (char)token[1];
  if(this->options != NULL) {
    byte prevOptNum = 0;
    for(byte i = 0; i<options->getSize(); i++){
      Option* opt = options->getOptionAt(i);
      index+=opt->getBytes(prevOptNum, buf, index);
      prevOptNum = opt->getNumber();
      free(opt);
      opt = NULL;
    }
  }
  if (payload && payloadLen>0) {
    buf[index++] = PAYLOAD_MARKER;
    for (byte i=0; i<payloadLen; i++)
      buf[index++]=(char)payload[i];
  }
  buf[index++] = 0;
  buf[0]=index-2;
  return buf;
}

boolean CoapMessage::isRequest() {
  return this->code>=1 && this->code <32;
}

void CoapMessage::toString(){
  Serial.println("\n######START#####");
  Serial.print("Version: ");
  Serial.println(this->ver);
  Serial.print("Type [0 CON, 1 NON, 2 ACK, 3 RST]: ");
  Serial.println(this->type);
  Serial.print("Code [1 GET, 2 POST, 3 PUT, 4 DELETE]: ");
  Serial.println(this->code);
  Serial.print("Message ID: ");
  Serial.println(this->messageId);
  Serial.print("Token: ");
  if(this->token != NULL){
    for(byte i=0; i<this->tokenLen; i++)
      Serial.print(token[i], HEX);
  }
  Serial.print("\nOptions: ");
  if(this->options && this->options->getSize()>0){
    for(int i = 0; i<this->options->getSize(); i++) {
      Serial.print("\n\tNumber: ");
      Serial.println(this->options->getOptionAt(i)->getNumber());
      Serial.print("\tValue: ");
      Serial.println(this->options->getOptionAt(i)->getValue());
    }
  }
  Serial.print("\nPayload: ");
  if (this->payload!=NULL) {
    for(byte i = 0; i<this->payloadLen; i++)
    Serial.print(this->payload[i]);
  }
  Serial.print("\nPayloadLEN: ");
  Serial.println(this->payloadLen);
  Serial.println("\n######END#####");
}

byte CoapMessage::getCode() {
  return this->code;
}

byte CoapMessage::getType() {
  return this->type;
}

unsigned int CoapMessage::getMessageId() {
  return this->messageId;
}

char* CoapMessage::getPayload() {
  return this->payload;
}

Options* CoapMessage::getOptions() {
  return this->options;
}

byte* CoapMessage::getToken(){
  return this->token;
}

void CoapMessage::setRemoteSocketAddress(IPAddress address, unsigned int port){
  this->remoteIPAddr = address;
  this->remotePort = port;
}

IPAddress CoapMessage::getRemoteIPAddr() {
  return this->remoteIPAddr;
}

unsigned int CoapMessage::getRemotePort() {
  return this->remotePort;
}

