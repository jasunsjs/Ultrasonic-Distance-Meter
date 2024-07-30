/*
Title: Ultrasonic Distance Meter
Description: Distance measurement with ultrasonic sensor and using LCD Display to show measurements.
Updated 2023-07-28
Author: Jason Sun
Version 2.0
*/

#include <LiquidCrystal.h>
#include <Keypad.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Pins ------------------------------------
const int trigPin = 39;
const int echoPin = 37; 
const int powerPin = 41;
const int groundPin = 35;  

const double SPEED_OF_SOUND = 0.034;
const byte ROWS = 4;
const byte COLUMNS = 4;
char key;
char keys[ROWS][COLUMNS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = {14, 15, 16, 17};
byte columnPins[COLUMNS] = {18, 19, 20, 21};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, ROWS, COLUMNS); 

long duration = 0;  
double distance = 0;
int mode = 0; // For switching between MEASURE, HOLD, REFERENCE
bool cmMode = true;
int keyValue = 5;
int index = 0;
double refDurations[5]= { 0 };
String input = "";
int integerInput = 0;
long average = 0;

// Setup function ---------------------------------------------
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(powerPin, OUTPUT);
  pinMode(groundPin, OUTPUT);
  lcd.begin(16, 2);
}

// Main program -----------------------------------------------
void loop() {
  digitalWrite(powerPin, HIGH);
  digitalWrite(groundPin, LOW);
  // Take the average of five measurments
  for (int i = 0; i<5; i++){
    measureDuration();
    average += duration;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  keyValue = readKeys();

  // Select key
  if (keyValue == 4){
    changeUnit();    
  }
  // Up key
  if (keyValue == 1){
    changeMode();    
  }

  // MEASURE MODE
  if (mode == 0){
    lcd.print("MEASURE");
    convertToCm();
    compareToReference();
    average = 0; // reset average
    printDistance();
  }

  // HOLD MODE
  else if (mode == 1){
    lcd.print("HOLD");
    printDistance();
  }

  // REFERENCE MODE 
  else{
    lcd.print("REFERENCE");
    inputReference();
    if (input.equals("")){ // is not inputting from keypad
      printReference();
    }
    else{ // is inputting from keypad
      printReferenceInput();
      storeReference();
    }
    scrollIndex();
  }
  delay(500);
}

// Methods----------------------------------------------------------
void measureDuration(){
  digitalWrite(trigPin, LOW);   
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
}

void convertToCm(){
  average = average / 5;
  distance = average * SPEED_OF_SOUND / 2;
}

void printDistance(){
  lcd.setCursor(0, 1);
  if (cmMode == true){
    lcd.print(distance);
    lcd.print(" cm");
  }
  else{
    lcd.print(distance * 0.3937);
    lcd.print(" inch");
  }
} 

void changeMode(){
  if (mode == 2){
    mode = mode - 2; // Changes from reference to measure
  }
  else{
    mode++; // Changes from measure to hold or hold to reference
  }
}

void changeUnit(){
  if (cmMode){
    cmMode = false;
  }
  else{
    cmMode = true;
  }
}

void scrollIndex(){
  lcd.setCursor(11, 0);
  lcd.print("#");
  lcd.print(index+1);
  if (keyValue == 0 && index < 4){ // Right key
    index++;
  }
  else if (keyValue == 3 && index > 0){ // Left key
    index--;
  }
}

void inputReference(){
  lcd.setCursor(0, 1);
  key = keypad.getKey(); // get key in char
  if (key != '0'){ // initial digit cannot be 0
    input += (String)key; // convert key to String and store in input
  }
  integerInput = input.toInt(); // convert input to integer
}

void printReferenceInput(){ // display the inputting process
  if (cmMode){
    lcd.print(integerInput);
    lcd.print(" cm");
  }
  else{
    lcd.print(integerInput);
    lcd.print(" inch");
  }
}

void storeReference(){
  if (keyValue == 2){
    if (cmMode){
      refDurations[index] = integerInput * 2 / SPEED_OF_SOUND;
    }
    else{
      refDurations[index] = integerInput / 0.3937 * 2 / SPEED_OF_SOUND;
    }
    input = ""; // reset input string
  }
}

void printReference(){ // displaying stored reference values
  lcd.setCursor(0, 1);
  if (cmMode){
    lcd.print(refDurations[index] * SPEED_OF_SOUND / 2);
    lcd.print(" cm");
  }
  else{
    lcd.print(refDurations[index] * SPEED_OF_SOUND / 2 * 0.3937);
    lcd.print(" inch");
  }
}

void compareToReference(){
  for (int i=0; i<5; i++){
    // Upper and lower bounds for range of reference
    double upperLimit = refDurations[i] + 100;
    double lowerLimit = refDurations[i] - 100;
    if (average > lowerLimit && average < upperLimit){
      lcd.print(" Ref");
      lcd.print(i+1);
    }
  }
}

int readKeys() {
  int ADCvalue = analogRead(0);
  if (ADCvalue > 1000) return 5;
  else if (ADCvalue < 50) return 0;
  else if (ADCvalue < 195) return 1;
  else if (ADCvalue < 380) return 2;
  else if (ADCvalue < 555) return 3;
  else if (ADCvalue < 790) return 4;
  else return 5;
}
