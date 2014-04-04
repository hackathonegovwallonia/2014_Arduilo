/*
  parcoursVelo
  
  v1 Inspire de Button, et de SD DataLogger
  v2 Integre le code de GPStest_RMC
  
  Appuye sur le Button de l'arduino, et, enregistre un texte dataString 
  dans un fichier datalog.txt
  Le texte enregistre est:
  |1|1|
  |2|1|
  |3|1|
  ...
  
  Ajouté le texte venant du GPS


  created 2014
  by Patrick, Francois, Hugues, Julien, Philippe
  
  
  
  
  
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// FROM parcoursVelo

#include <SD.h>

// END parcoursVelo


// BEGIN of Include here below, the code from GPStest_RMC
//
// A simple sketch to read GPS data and parse the $GPRMC string 
// see http://www.ladyada.net/make/gpsshield for more info

// If using Arduino IDE prior to version 1.0,
// make sure to install newsoftserial from Mikal Hart
// http://arduiniana.org/libraries/NewSoftSerial/
#if ARDUINO >= 100
 #include "Arduino.h"
 #include "SoftwareSerial.h"
#else
 #include "WProgram.h"
 #include "NewSoftSerial.h"
#endif

// Use pins 2 and 3 to talk to the GPS. 2 is the TX pin, 3 is the RX pin
#if ARDUINO >= 100
SoftwareSerial mySerial = SoftwareSerial(2, 3);
#else
NewSoftSerial mySerial = NewSoftSerial(2, 3);
#endif

// Use pin 4 to control power to the GPS
#define powerpin 4

// Set the GPSRATE to the baud rate of the GPS module. Most are 4800
// but some are 38400 or other. Check the datasheet!
#define GPSRATE 4800
//#define GPSRATE 38400

// The buffer size that will hold a GPS sentence. They tend to be 80 characters long
// so 90 is plenty.
#define BUFFSIZ 90 // plenty big


// global variables
char buffer[BUFFSIZ];        // string buffer for the sentence
char *parseptr;              // a character pointer for parsing
char buffidx;                // an indexer into the buffer

// The time, date, location data, etc.
uint8_t hour, minute, second, year, month, date;
uint32_t latitude, longitude;
uint8_t groundspeed, trackangle;
char latdir, longdir;
char status;


// END of Include here below, the code from GPStest_RMC



// FROM parcoursVelo

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 7;     // the number of the pushbutton pin --- in this format, i change the buttonPin to 7
const int ledPin =  13;      // the number of the LED pin

const int chipSelect = 10;   // pour ecrire sur la carte SD

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int onceState = 0;          // compter 1 fois
int dataStringCounter = 0;  // pour compter combien de fois j'ai appuyé sur le bouton

// END from parcoursVelo



// VOID SETUP CODE

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);  
  
  
  // inclure le code de datalogger
  
  // Open serial communications and wait for port to open:
  Serial.begin(4800);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
   }


  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  // 
  
  // include here below the code from GPStest_RMC
  // BEGIN from GPStest_RMC
  
  
  
  if (powerpin) {
    pinMode(powerpin, OUTPUT);
  }
  
  // Use the pin 13 LED as an indicator
  pinMode(13, OUTPUT);
  
  // connect to the serial terminal at 9600 baud
  Serial.begin(4800);
  
  // connect to the GPS at the desired rate
  mySerial.begin(GPSRATE);
   
  // prints title with ending line break 
  Serial.println("GPS parser"); 
 
   digitalWrite(powerpin, LOW);         // pull low to turn on!

  
  
  // END from GPStest_RMC

  
}



// BEGIN ici les methodes et les functions et les types // integrer le uint32 et le void readline


 
// la premiere

uint32_t parsedecimal(char *str) {
  uint32_t d = 0;
  
  while (str[0] != 0) {
   if ((str[0] > '9') || (str[0] < '0'))
     return d;
   d *= 10;
   d += str[0] - '0';
   str++;
  }
  return d;
}




// la deuxième

void readline(void) {
  char c;
  
  buffidx = 0; // start at begninning
  while (1) {
      c=mySerial.read();
      if (c == -1)
        continue;
      Serial.print(c);
      if (c == '\n')
        continue;
      if ((buffidx == BUFFSIZ-1) || (c == '\r')) {
        buffer[buffidx] = 0;
        return;
      }
      buffer[buffidx++]= c;
  }
}



// la nouvelle methode



String dataStringGPSMethod(){

  
  // String dataStringGPS = "";
  
  
  // mettre tout ceci dans une methode qui sera appellee une fois
  // BEGIN intégrer le code du GPS
  
  
   uint32_t tmp;
    
  readline();
  
  // check if $GPRMC (global positioning fixed data)
  if (strncmp(buffer, "$GPRMC",6) == 0) {
    
    // hhmmss time data
    parseptr = buffer+7;
    tmp = parsedecimal(parseptr); 
    hour = tmp / 10000;
    minute = (tmp / 100) % 100;
    second = tmp % 100;
    
    parseptr = strchr(parseptr, ',') + 1;
    status = parseptr[0];
    parseptr += 2;
    
    // grab latitude & long data
    // latitude
    latitude = parsedecimal(parseptr);
    if (latitude != 0) {
      latitude *= 10000;
      parseptr = strchr(parseptr, '.')+1;
      latitude += parsedecimal(parseptr);
    }
    parseptr = strchr(parseptr, ',') + 1;
    // read latitude N/S data
    if (parseptr[0] != ',') {
      latdir = parseptr[0];
    }
    
    //Serial.println(latdir);
    
    // longitude
    parseptr = strchr(parseptr, ',')+1;
    longitude = parsedecimal(parseptr);
    if (longitude != 0) {
      longitude *= 10000;
      parseptr = strchr(parseptr, '.')+1;
      longitude += parsedecimal(parseptr);
    }
    parseptr = strchr(parseptr, ',')+1;
    // read longitude E/W data
    if (parseptr[0] != ',') {
      longdir = parseptr[0];
    }
    

    // groundspeed
    parseptr = strchr(parseptr, ',')+1;
    groundspeed = parsedecimal(parseptr);

    // track angle
    parseptr = strchr(parseptr, ',')+1;
    trackangle = parsedecimal(parseptr);


    // date
    parseptr = strchr(parseptr, ',')+1;
    tmp = parsedecimal(parseptr); 
    day = tmp / 10000;
    month = (tmp / 100) % 100;
    year = tmp % 100;
        
    String dataStringGPS = year + "-" + month + "-" + day + "_" hour + ":" + minute + ":" + second + "_";
        
    if (latdir == 'N') {
       dataStringGPS = dataStringGPS + "+";
    }else if (latdir == 'S') {
       dataStringGPS = dataStringGPS + "-";
    }

    
    dataStringGPS += latitude/1000000 + "* " + (latitude/10000)%100 + "' ";
    dataStringGPS += (latitude%10000)*6/1000 + "." + ((latitude%10000)*6/10)%100 + "\"";
    dataStringGPS += "_";

    
    
    if (longdir == 'E'){
       dataStringGPS = dataStringGPS + "+";
    }else if (longdir == 'W'){
       dataStringGPS = dataStringGPS + "-";
    }
    dataStringGPS = dataStringGPS + longitude/1000000 + "* " + (longitude/10000)%100 + "' ";
    dataStringGPS = dataStringGPS + (longitude%10000)*6/1000 + "." + ((longitude%10000)*6/10)%100 + "\"";
   
  
  }
  
  // END integrer le code du GPS


  
  
  return dataStringGPS;
}



// END // integrer le uint32 et le void readline
 


void loop()
{
  
  // XXX
  // XXX
  
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  // add the dataString that i want to write in the DataLogger on SD
  String dataString = "";
  
 

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH); 
    
    if (onceState == 0) {
      Serial.println(buttonState);
      
      // i want to add the buttonState info in the dataString
      
      dataString += "|";
      
      dataStringCounter = dataStringCounter + 1;
      dataString += String(dataStringCounter);
      
      dataString += "|";
      
      dataString += String(buttonState);
      dataString += "|";
      
      
      dataString += dataStringGPSMethod();
      
      dataString += "|";      
      
      // BEGIN integration ici le code GPS
      
      
      
      // END integration du code GPS
      
      
      
      
      // début de ecriture sur file
      
      
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      File dataFile = SD.open("datalog.txt", FILE_WRITE);

      // if the file is available, write to it:
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        Serial.println(dataString);
        // Serial.println("x");                // for debug
        // Serial.println(dataStringCounter);  // for debug
        // Serial.println("xy");              // for debug
      }  
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening datalog.txt");
      } 
      
      
      // fin du file write
      
      // fin du travail, je mets la variable à 1
      
      onceState = 1;
    }
    
    
} 
  else if (buttonState == LOW) {  
    // turn LED off:
    digitalWrite(ledPin, LOW); 
    
    if (onceState == 1) {
      Serial.println(buttonState);
      onceState = 0;
    }
    
  }
}
