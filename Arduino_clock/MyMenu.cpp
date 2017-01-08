/*
  Kamil Kasperski - University of Silesia
  Menu library for Digital clock project
*/

#include "Arduino.h"
#include "MyMenu.h"

MyMenu::MyMenu(String label, int fieldCount, String *fields) {
  _fieldCount = fieldCount;
  _label = label;
  _fields = fields;
}
int MyMenu::getIndex() {
  return _index;
}
int MyMenu::getFieldCount() {
  return _fieldCount;
}
String MyMenu::getLabel() {
  return _label;
}
void MyMenu::setNextField() {
  if (_index == _fieldCount-1) {
    _index = 0;
  } else {
    _index++;
  }
}
void MyMenu::setPrevField() {
  if (_index == 0) {
    _index = _fieldCount-1;
  } else {
    _index--;
  }
}
String MyMenu::showField() {
  return _fields[_index];
}

