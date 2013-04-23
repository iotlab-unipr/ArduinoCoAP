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
