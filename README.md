# fingerprintmatching
how to make attendance using fingerprint image with AZ620-m5, ESP8266 and Ethernet module

Wireing
ESP8266 - ZA620-M5
![wiring_esp8266_za620_m5](https://github.com/user-attachments/assets/951670e7-3217-4f70-8202-b349b67d2dc8)

ESP32 - W5500
![ESP32-DOIT-DevKit-v1-w5500-wiring-external-power-supply](https://github.com/user-attachments/assets/a92108b2-d531-49a0-b0cc-624eca35c553)

ESP8266 - W5500
![esp8266-W5500](https://github.com/user-attachments/assets/43f5678d-b819-4817-b4af-7ff17926ab39)

| SPI PHY  | -- |ESP8266 |
| ---------|----| ------------- |
|SCK	  |---|	GPIO14(D5)|
|MISO	  |---|	GPIO12(D6)|
|MOSI	|---|	GPIO13(D7)|
|CS  |---|GPIO16(D0)(*)|
|RST  |---|RESET|
|5V |---| Vin|
|GND  |---|GND|


(*) You can change it to any GPIO.

REFERENCE
https://gist.github.com/mthri/08a1f895bd0855a3ea2e0abb52c2fd81



