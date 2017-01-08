/*
  Kamil Kasperski - University of Silesia
  Menu implementation for Digital clock  project
  still in early development
 */
#ifndef MyMenu_h
#define MyMenu_h

#include "Arduino.h"

class MyMenu {
  private:
    int _index = 0;
    int _fieldCount;
    String _label;
    String *_fields;
    void *_options;
  public:
    MyMenu(String label, int fieldCount,String *fields);
    int getIndex();
    int getFieldCount();
    String getLabel();
    void setNextField();
    void setPrevField();
    String showField();
};

#endif
