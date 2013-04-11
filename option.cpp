#include "option.h"

void Option::init(byte prevOptNum, char* buf, byte off) {
  byte index = off;
  byte delta = (buf[index]>>4)&0xf;
  byte valueLen=buf[index]&0xf;
  index++;
  if (delta==13)
    delta = 13+buf[index++];
  else
    if (delta==14) {
      delta = (269+(buf[index]&0xff)<<8)+(buf[index+1]&0xff);
      index+=2;
    }
  this->number = prevOptNum+delta;
  if (valueLen==13)
    valueLen=13+buf[index++];
  else
    if (valueLen==14) {
      valueLen=(269+(buf[index]&0xff)<<8)+(buf[index+1]&0xff);
      index+=2;
    }

  this->value=(char*)malloc(valueLen+1);
  for(byte i = 0; i<valueLen; i++)
    this->value[i] = buf[index++];
    this->value[valueLen] = 0;
  this->len=valueLen;
}

byte Option::getBytes(byte prevOptNum, char* buf, byte off) {
  int delta = this->number-prevOptNum;
  if(delta<0) return 0;
  byte i = off;
  byte length = this->len;
  buf[i++]=(byte)((((delta<13)? delta : (delta<269)? 13 : 14)<<4) | ((length<13)? length : (length<269)? 13 : 14));
      if (delta>=269)
      {  delta-=269;
         buf[i++]=(byte)((delta>>8)&0xff);
         buf[i++]=(byte)(delta&0xff);
      }
      else
      if (delta>=13)
      {  delta-=13;
         buf[i++]=(byte)delta;
      }
      if (length>=269)
      {  length-=269;
         buf[i++]=(byte)((length>>8)&0xff);
         buf[i++]=(byte)(length&0xff);
      }
      else
      if (length>=13)
      {  length-=13;
         buf[i++]=(byte)length;
      }
      return i-off+strlen(this->value);
}

Option::~Option(){
  if(this->value != NULL || this->value!=""){
    //free(this->value);
    this->value = NULL;  
  }
}

Option::Option() {
  number = 0;
  len = 0;
  value = NULL;
}

Option::Option(byte number, char* value) {
  this->number = number;
  this->len = strlen(value);
  this->value = (char*)malloc(len);
  strcpy(this->value, value);
}

byte Option::getNumber() {
  return this->number;
}

char* Option::getValue() {
  return this->value;
}

unsigned int Option::getLen() {
  return this->len;
}

unsigned int Option::getLen(byte prevOptNum){
  int delta=this->number-prevOptNum;
  if (delta<0) return 0;
  int data_len=1+this->len;
  if (delta>12) data_len++;
  if (delta>268) data_len++;
  if (this->len>12) data_len++;
  if (this->len>268) data_len++;
  return data_len;
}

