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

#include "options.h"

Options::Options() {
   this->opts = (Option**)malloc(sizeof(Option*));
   this->index = 0;
}

Options::~Options(){
   for(byte i=0; i<this->index; i++) {
     if(this->opts[i] != NULL){
       delete this->opts[i];
       this->opts[i] = NULL;
     }
   }
   if(this->opts!=NULL){
     free(this->opts);
     this->opts = NULL;  
   }
 }

boolean Options::addOption(Option* opt, boolean over) {
   if (false==over) {
      this->opts[this->index++] = opt;
      this->opts = (Option**)realloc(this->opts, (this->index+1)*sizeof(Option*));
      if(opts==NULL) Serial.println("MEMORY OPTION ERROR");
      return (this->opts==NULL);
   } else {
      int j = this->getOptionIndexByNumber(opt->getNumber());
      if (index>0) {
         this->opts[index] = opt;
         return true;
      } else {
         return this->addOption(opt, false);
      }
   }
   
}

Option* Options::getOptionByNumber(byte number) {
   for (byte i=0; i<this->index; i++) {
      Option* opt = this->opts[i];
      if (opt->getNumber()==number) {
         return opt;
      }else{
        delete opt;
        opt = NULL;
      }
   }
}

byte Options::getOptionIndexByNumber(byte number) {
   for (byte i=0; i<this->index; i++) {
      Option* o = this->opts[i];
      if (o->getNumber()==number) {
         return i;
      }
   }
   return -1;
}

Option* Options::getOptionAt(byte index) {
   return this->opts[index];
}

byte Options::getSize() {
   return this->index;
}
