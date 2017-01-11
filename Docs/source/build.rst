Budowa urządzenia
=================
Poniżej przedstawiono informacje na temat budowy urządzenia.

Części
------

Spis wszystkich części i podzespołów bazowych wymaganych do zbudowania układu kompatybilnego z tym oprogramowaniem:

* ATmega328P
* Wyświetlacz OLED 16x2 Winstar WEH1602A
* Moduł DS1307 RTC
* Przetwornica step-up NCP1402 3.3V out

Ponadto:

* przełącznik ON/OFF
* rezystory 6,2K 1K 2K 10K
* kondensator elektrolityczny 100uF (C3)
* kondensatory ceramiczne 22pF (C1, C2)
* 16MHz oscylator kwarcowy (XTAL1)

Schemat urządzenia
------------------
Urządzenie jest dość proste w budowie. Bateria zasila układ NCP1402 napięciem od 0.8V do 3.3V, który zasila resztę urządzenia stałym napięciem 3.3V.

Równolegle podłączony jest filtr dolnoprzepustowy RC, który stabilizuje napięcie i wysyła je do odczytu na port analogowy ``A2`` w celu pomiaru napięcia baterii.

Użyty został kwarc 16MHz w celu zapewnienia odpowiedniego taktowania dla ATmega328P.

Ekran LCD został podpięty pod porty cyfrowe ``D13``, ``D12``, ``D11``, ``D10``, ``D9``, ``D8`` - które kolejno idą do złącz LCD ``RS``, ``E``, ``D4``, ``D5``, ``D6``, ``D7``. Pozwala to na pracę wyświetlacza w trybie 4-bit co wystarcza na aktualne zapotrzebowanie projektu. Na uzytym wyświetlaczu OLED porty ``V0``, ``LED+`` i ``LED-`` mają status *NC* i nie są w ogóle wykorzystywane przez kontroler ekranu przez co można je pominąć. Ekran potrzebuje napięcia minimum 3V. Ponizej tej wartości kontroler ekranu przestaje działać.

Układ DS1307 korzysta z portów ``SLC`` i ``SDA``, które są kolejno na portach analogowych ``A5`` i ``A4``. Układ ten korzysta z szeregowej magistrali dwukierunkowej I2C. Układ DS1307 jest zasilany osobną baterią CR2032 przez co możliwe jest zachowanie daty nawet po odłączeniu zasilania całgo urządzenia.

Do kontroli nad urządzeniem została zbudowana klawiatura bazująca na analogowym odczycie napięcia. Napięcie wejściowe jest dzielone poprzez sumę oporów R4 R5 R6 w zależności w której części został naciśnięty przycisk. Pozwala to na obsługę wielu przycisków wyłącznie przy użyciu jednego portu. Przycisk cyfrowy ``ALARM`` jest podpięty do portu cyfrowego ``D2`` przez co jest możliwe użycie go jako przycisk do wywołania zewnętrznego przerwania ``INT0``.
