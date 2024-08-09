# fingerprintmatching
how to make attendance using fingerprint image with AZ620-m5, ESP8266 and Ethernet module

Wireing

![esp8266-W5500](https://github.com/user-attachments/assets/43f5678d-b819-4817-b4af-7ff17926ab39)

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

REFERENCE
https://gist.github.com/mthri/08a1f895bd0855a3ea2e0abb52c2fd81



