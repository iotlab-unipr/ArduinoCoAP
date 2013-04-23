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
