#ifndef OPTION_H
#define OPTION_H
#include "Arduino.h"

class Option {
   byte number;
   unsigned int len;
   char* value;
public:
   byte getNumber();
   char* getValue();
   unsigned int getLen();
   unsigned int getLen(byte number);
   void init(byte prevOptNum, char* buf, byte off);
   byte getBytes(byte prevOptNum, char* buf, byte off);
   Option(byte number, char* value);
   Option();
   ~Option();
};

#endif
