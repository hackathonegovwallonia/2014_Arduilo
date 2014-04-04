/*
  parcoursVelo
  
  Inspire de Button, et de SD DataLogger
  
  Appuye sur le Button de l'arduino, et, enregistre un texte dataString 
  dans un fichier datalog.txt
  Le texte enregistre est:
  |1|1|
  |2|1|
  |3|1|
  ...
  
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

#include <SD.h>

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

const int chipSelect = 4;   // pour ecrire sur la carte SD

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int onceState = 0;          // compter 1 fois
int dataStringCounter = 0;  // pour compter combien de fois j'ai appuyé sur le bouton



void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);  
  
  
  // inclure le code de datalogger
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
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
  
}

void loop(){
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
