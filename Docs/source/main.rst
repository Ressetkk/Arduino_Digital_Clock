Wstęp
=====
**Arduino Digital Clock** to projekt oprogramowania zegarka cyfrowego bazowany na Arduino UNO/ATmega328P.

Założenia
---------

* Wykonać implementację kompletnego oprogramowania zegarka cyfrowego zasilanego napięciem bateryjnym.
* Data i godzina muszą być zachowane po utracie zasilania bateryjnego.
* Działający wskaźnik baterii
* Możliwość ustawienia więcej niż jednego alarmu naraz.

Biblioteki
----------
Program korzysta z kilku ogólnodostępnych bibliotek:

* RTClib - biblioteka do obsługi zegara czasu rzeczywistego https://github.com/adafruit/RTClib
* LowPower - biblioteka do obsługi instrukcji uśpienia mikrokontrolera ATmega https://github.com/rocketscream/Low-Power
* Squawk - biblioteka do obsługi 8-bitowych melodii na mikrokontrolerze ATmega https://github.com/stg/Squawk (biblioteka jest wbudowana w bibliotekę ArduinoRobot)
* LiquidCrystal - biblioteka do obsługi ekranu ciekłokrystalicznego (wbudowana)
* Wire - biblioteka pozwalająca na komunikację urządzeń I2C/TWI z Arduino (wbudowana)

Na potrzeby tego projektu została utworzona biblioteka `MyMenu`. Jest to podstawowa, jeszcze nie rozwinięta implementacja Menu wyświetlanego na ekranie LCD.

Wymagania do kompilacji
-----------------------
Wymogiem kompilacji jest użycie środowiska Arduino IDE 1.6.9. Nowsze wersje nie są kompatybilne z implementacją biblioteki Squawk.
Wszystkie niestandardowe biblioteki muszą się znajdować w katalogu głównym szkicu.
