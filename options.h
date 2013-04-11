#ifndef Options_h
#define Options_h
#include "Arduino.h"
#include "option.h"


class Options {
   Option** opts;
   byte index;
public:
   boolean addOption(Option* opt, boolean overwrite);
   Option* getOptionAt(byte i);
   Option* getOptionByNumber(byte number);
   byte getOptionIndexByNumber(byte number);
   byte getSize();
   Options();
   ~Options();
};

#endif
