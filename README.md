# OledClock
This example gets the time from NTP server and presents in an OLED Display

## Platform

The board used to test it is a Wemos D1 mini based on a ESP8266

this is a simple example that connects to a local WIFi (need to provide the information in the secret.h file)
Connects to NTP server to get the current UTC date and converts to a Guadalajara time zone

## Libraries
it requres several libraries

WiFi suport and UDP (Layer protocol)

```
    #include <ESP8266WiFi.h>
    #include <WiFiUdp.h>
```

Connect to NTP server and handle time and time zones

```
#include <NTPClient.h>
#include <time.h>
#include <TimeLib.h>
#include <Timezone.h>
```
i2C support
```
#include <SPI.h>
```
Oled Display

```
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
```

Temperature and Humidity device
```
#include <DHT.h>
```

## SSID and Password
the `secret.h` is used to set the SSID for the local network and the password for it, since they are not encripted and may be a security issues, it is maintained separately from the code. Be sure **not** to share this file