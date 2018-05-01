// ---------------------------- IMPORT OF LIBRARIES ----------------------------------
#include <SPI.h>
#include <SD.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// ---------------------------- DEFINITION OF SENSOR TYPE AND PIN LAYOUT -------------
File myFile;
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int maximum_value = 0;
int minimum_value = 1000;

// ---------------------------- VOID SETUP -------------------------------------------
void setup(void){
  Serial.begin(9600);


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("sensor.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to sensor.txt...");
    myFile.println("Sensor File successfully created");
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening sensor.txt");
  }


  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  lcd.home();
}



// ---------------------------- MAIN PROGRAM -----------------------------------------
void loop(){
  lcd.clear();
  int humidity_value = get_humidity_value();
  


  if (humidity_value >= maximum_value){
    maximum_value = humidity_value;
  }

  if (humidity_value <= minimum_value){
    minimum_value = humidity_value;

  }

  lcd.setCursor(3,0);
  lcd.print("Current: ");
  lcd.print(humidity_value);

  lcd.setCursor(5,1);
  lcd.print("----------");
  
  lcd.setCursor(3,2);
  lcd.print("Minimum: ");
  lcd.print(minimum_value);

  lcd.setCursor(3,3);
  lcd.print("Maximum: ");
  lcd.print(maximum_value);
  
  delay(60000);

  write_to_file(humidity_value);
 
}



// ---------------------------- SUBFUNCTIONS -----------------------------------------
void write_to_file(int humidityValue){
  myFile = SD.open("sensor.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(humidityValue);
  }
  myFile.close();  
}

int get_humidity_value(){
    int sensorValue = analogRead(A3);
    Serial.println(sensorValue);
    return sensorValue;
}

