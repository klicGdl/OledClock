
/*****************************************************************************************************
 *  
 *  Get the clock from an NTP server, requieres to be connected to a Internet using WiFi
 *  Read the temperature and Humidity from DHT22
 *  Present information in a Oled Monocoromatic display
 *  
 ****************************************************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <NTPClient.h>
#include <time.h>
#include <TimeLib.h>
#include <Timezone.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//load DHT library
#include <DHT.h>

#define DEBUG 0

/***********************/
const unsigned char epd_bitmap_Bitmap [] PROGMEM = {
  // 'klic, 64x48px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x30, 0x41, 0x00, 0x1f, 0xc1, 0xf8, 0x00, 
  0x00, 0x31, 0xc1, 0x00, 0x1f, 0x03, 0x80, 0x00, 0x00, 0x33, 0x01, 0x00, 0x03, 0x02, 0x00, 0x00, 
  0x00, 0x36, 0x01, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x3c, 0x01, 0x00, 0x03, 0x06, 0x00, 0x00, 
  0x00, 0x38, 0x01, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x38, 0x01, 0x80, 0x03, 0x06, 0x04, 0x00, 
  0x00, 0x3e, 0x01, 0x80, 0x03, 0x06, 0x0c, 0x00, 0x00, 0x37, 0x81, 0x80, 0x03, 0x06, 0x08, 0x00, 
  0x00, 0x31, 0xe1, 0x87, 0x03, 0x82, 0x30, 0x00, 0x00, 0x10, 0x71, 0xfe, 0x0f, 0x83, 0xe0, 0x00, 
  0x00, 0x10, 0x00, 0xf0, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*****************************************/

/***************************************************************************************************/
// Configurar wifi
#include "secret.h"

// Definir propiedades NTP
#define NTP_OFFSET   60 * 60         // in seconds
#define NTP_INTERVAL 60 * 1000       // En miliseconds
#define NTP_ADDRESS  "pool.ntp.org"  // URL NTP

// Configure NTP UDP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

// Horario Guadalajara
TimeChangeRule mxCDT = {"CDT", First, Sun, Apr, 2, -360};    // Guadalajara day saveing 
TimeChangeRule mxCST = {"CST ", Last, Sun, Oct, 2, -420};    // Guadalajara standar time
Timezone gm(mxCDT, mxCST);

time_t local, utc;

// 
const char * days[] = {"Dom", "Lunes", "Martes", "Mier", "Jueves", "Vier", "Sab"} ;
const char * months[] = {"Ene", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"} ;

// Configure OLED
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define DHTTYPE  DHT22    //DHT model DHT22
#define DHTPIN   2        // shield is connected to pin 2

/* Create the DHT instance */
DHT dht(DHTPIN, DHTTYPE);

/*
 *  S E T U P 
 */

void setup() 
{
  // Inicializar OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with I2C 0x3C (for 64x48 display)
  display.clearDisplay();

   // Initialize serial port
  Serial.begin(115200);

  // Connect to wifi 
  Serial.println("");
  Serial.print("conectando a ");
  Serial.print(ssid);

  // OLED Display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.drawBitmap(0, 0, epd_bitmap_Bitmap, 64, 48,1);
  display.setCursor(0,32);
  display.println("Connecting");
  display.print(ssid);
  display.display();
  display.clearDisplay();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecting WiFi a ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  // initialize  a NTPClient to get time
  timeClient.begin();

  // init DHT
  dht.begin(40);
  delay(2000);
  
}


/*
 *  L O O P
 */
void loop() 
{
  float h, t;
  // read the humidity and temperature
  
  h = dht.readHumidity();
  t = dht.readTemperature();

#if DEBUG
  // print in the console
  print_temp_and_humid(t, h,"DHT22");
#endif

  // now print it in the Oled display
  DisplayTempAndHumidity(t,h);

  if (WiFi.status() == WL_CONNECTED) // Checking the WIFI connection
  {   
     
    // Update NTP CLient in UNIX UTC
    timeClient.forceUpdate();
    timeClient.update();
    unsigned long utc =  timeClient.getEpochTime();
    int tries = 0;
    if (utc < 1630373993 && tries < 5)
    {
      timeClient.update();
      tries++;
    }
    
    // Convert to local time
    local = gm.toLocal(utc);
#if DEBUG
    Serial.println(utc);
    Serial.println(local);
    Serial.println(timeClient.getFormattedTime());

    // Print formated in serial port
    printTime(local);
#endif
    //  Show in OLED Display
    printTimeOLED(local);
    
  }
  else // If lost connection try again
  {
    WiFi.begin(ssid, password);
    delay(1000);
  }
    
  delay(10000);    // wait 
}


/*
 * Print the temperature and humidity in the Oled Display
 */
void DisplayTempAndHumidity(float t, float h)
{
  display.clearDisplay();
  display.setTextSize(1);
  // set cursor - col, row 
  display.setCursor(0,0);
  display.print("T: ");
  display.print(t);
  display.println(" C");
  display.print("H: ");
  display.print(h);
  display.println(" %");
  //display.display();
}


// Format the date 
String ConvertDate(time_t t)
{
  String date = "";
  date += days[weekday(t)-1];
  date += ", ";
  date += day(t);
  date += " ";
  date += months[month(t)-1];
  date += ", ";
  date += year(t);
  return date;
}

// Format month and year only
String ConvertMonthAndYear(time_t t)
{
  String date = "";
  date += months[month(t)-1];
  date += "   ";
  date += year(t);
  return date;
}

// Convert the time to string
String ConvertTime(time_t t)
{
  // 
  String hora ="";
  if(hour(t) < 10)
    hora += "0";
  hora += hour(t);
  hora += ":";
  if(minute(t) < 10)  // add 0 if minutes are < 10
    hora += "0";
  hora += minute(t);
  return hora;
}

// Show in the OLED Display
void printTimeOLED(time_t t)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("");
  display.println("");
  display.setTextSize(2);
  display.println(ConvertTime(t));
  display.setTextSize(1);
  display.print(days[weekday(t)-1]);
  display.print(" ");
  display.println(day(t));
  display.println(ConvertMonthAndYear(t));
  display.display();
}

#if DEBUG 

void print_temp_and_humid(float t, float h, String text)
{
 // Print the values read
   Serial.println(text);
   Serial.print("Humedad: "); 
   Serial.println(h);
   Serial.print("Temperatura: ");
   Serial.println(t);
}

// Send time and date to serial port
void printTime(time_t t)
{
  Serial.println("");
  Serial.print("Fecha local: ");
  Serial.print(ConvertDate(t));
  Serial.println("");
  Serial.print("Hora local: ");
  Serial.print(ConvertTime(t));
}


#endif
