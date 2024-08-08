# fingerprintmatching
how to make attendance using fingerprint image with AZ620-m5, ESP8266 and Ethernet module

Wireing
| SPI PHY  | -- |ESP8266 |
| ---------|----| ------------- |
|SCK	  |---|	GPIO14(D5)|
|MISO	  |---|	GPIO12(D6)|
|MOSI	|---|	GPIO13(D7)|
|CS  |---|GPIO16(D0)(*)|
|RST  |---|RESET|
|3.3V |---| 3.3V|
|GND  |---|GND|


(*) You can change it to any GPIO.


