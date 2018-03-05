// ---------------------------- IMPORT OF LIBRARIES ----------------------------------
// Wire Library for I2C protocol
#include "Wire.h"

// Real Time Clock Library
#include "RTClib.h"


// ---------------------------- DEFINITION OF SENSOR TYPE AND PIN LAYOUT -------------
// RTC Module
RTC_DS1307 RTC;

// LED
const int ledPin = 6;
const int pumpPin = 8;

// BUTTON
const int buttonPin = 2;
const int buttonPin_on = 3;
volatile int buttonState = 0;
volatile int buttonState_on = 0;

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers


// ---------------------------- VOID SETUP -------------------------------------------
void setup(void){

  // Set baud rate
  Serial.begin(9600);

  // Initialization of I2C
  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  Serial.println("RTC Initialization completed");

  // LED Pin Setup
  pinMode(ledPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);

  // Button Pin Setup
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin_on, INPUT_PULLUP);

  // Define the interrupt function
  attachInterrupt(0, pin_ISR, FALLING);

  attachInterrupt(1, pin_ISR_1, FALLING);
}



// ---------------------------- MAIN PROGRAM -----------------------------------------
void loop(){

  // Boolean used for the lateron check if it is time to water the plant or not
  boolean is_watertime = false;

  // Show the current date on the serial monitor
  DateTime now=RTC.now();
  show_time_and_date(now);

  // Get the is_watertime variable
  is_watertime = get_watertime(now);

  // Check, if it is time to water the plants
  if(is_watertime == true){
    //switch_on_pump();
    // Wait two hours to not water the plants again in this hour (see get_watertime for more info)
    delay(432000000);
  }
  else{
    //switch_off_pump();
  }


}


// If a button press is detected, shut off the pump immediately
void pin_ISR() {
  switch_off_pump();
}

// If a button press is detected, switch on the pump immediately
void pin_ISR_1() {
  switch_on_pump();
}


// Extended output for the RTC module
void show_time_and_date(DateTime datetime){

  //  Day of the week --- Day/Month/Year
  Serial.print(datetime.dayOfTheWeek(),DEC);
  Serial.print(" --- ");
  Serial.print(datetime.day(),DEC);
  Serial.print("/");
  Serial.print(datetime.month(),DEC);
  Serial.print("/");
  Serial.print(datetime.year(),DEC);
  Serial.print(" --- ");

  // --- Hour:Minute:Second
  if(datetime.hour()<10)Serial.print(0);
  Serial.print(datetime.hour(),DEC);
  Serial.print(":");
  if(datetime.minute()<10)Serial.print(0);
  Serial.print(datetime.minute(),DEC);
  Serial.print(":");
  if(datetime.second()<10)Serial.print(0);
  Serial.println(datetime.second(),DEC);
}



// Check if is time to water the plants
boolean get_watertime(DateTime datetime){
  boolean is_watertime;
  is_watertime = false;

  // Get the day of the week
  int day;
  day = datetime.dayOfTheWeek();  // day = 1 means Monday, day = 2 means Tuesday, ...

  // Check, if it is the right time
  if((datetime.hour() >= 17) && (datetime.hour() < 18)){
    // Check if is the right day
    if(day == 1){
      is_watertime = true;
    }
    else{
      is_watertime = false;
    }
  }
  else{
    is_watertime = false;
  }
  return is_watertime;
}

// Switch the pump off
void switch_off_pump(){
  digitalWrite(ledPin, LOW);
  digitalWrite(pumpPin, LOW);
}

// Switch the pump on
void switch_on_pump(){
  digitalWrite(ledPin, HIGH);
  digitalWrite(pumpPin, HIGH);
}
