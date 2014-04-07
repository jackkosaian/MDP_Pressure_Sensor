int ledPin = 4; // LED pin for pressure warning
int initializationLED = 2; // LED pin for initialization
int buzzerPin = 7; // Buzzer pin
int pressurePin = A0; // Pin to take pressure reading
int pressureValue; // Pressure value in analog units
int pressureThreshold = 950; // Warn if over this value
long delayValue = 1000000/2500/2; // Delay for buzzer
float desiredVoltage = (((760*4/15/51.7149326)+.5)*1024/5); // Desired analog read value (assuming 17mmHg)
float pressureInmmHg; // Pressure value in mmHg
float basePressure; // Pressure that will be used as base (intraocular pressure)

// Required variables for running average
const int numReadings = 10;
float readings[numReadings];
int index = 0;
float total = 0;
float average;

// Test variables
int averageCounter = 0;
boolean turnOnLED = false;
long numCycles = 2500 * 2 / 1000;

void setup(){
 
 pinMode(initializationLED, OUTPUT); 
  
 pinMode(ledPin, OUTPUT);
 
 pinMode(buzzerPin, OUTPUT);
 
 Serial.begin(9600); 

 // Initialize all readings to be 0
 for (int thisReading = 0; thisReading < numReadings; thisReading++){
   readings[thisReading] = 0.0;
 } 
}

void loop(){
  
 pressureValue = analogRead(pressurePin);
 pressureInmmHg = (((float)pressureValue * 5 / 1024) - .5) * 15 / 4 * 51.7149326; 
 
 // Turn on LED if at desiredVoltage +/- 5
 if (pressureValue > desiredVoltage - 5 && pressureValue < desiredVoltage + 5)
 {
   //digitalWrite(initializationLED, HIGH);
     if( averageCounter == 0)
       averageCounter = 1;
     if (averageCounter >= 0 && averageCounter < 10){
       turnOnLED = true;
     }
   //takeAverage = true;
 }
 
 else {
   turnOnLED = false;
 }
 
 if (turnOnLED == true)
 {
  digitalWrite(initializationLED, HIGH); 
 }
 else {
  digitalWrite(initializationLED, LOW);
 }

if (averageCounter >= 1 && averageCounter <= 10)
{
  //total -= readings[index];
  //readings[index] = pressureValue;
  total += pressureValue;
  index += 1; // increment the index
  average = (float)total / (float)numReadings; 
 
  if (averageCounter == 10){
     basePressure = average;
  } 
  averageCounter += 1;
}

 Serial.print("mmHg: ");
 Serial.print(pressureInmmHg);
 Serial.print("          Analog Value: ");
 Serial.print(pressureValue);
 Serial.print("          Average: ");
 Serial.print(average);
 Serial.print("          Base Pressure: ");
 Serial.print(basePressure);
 Serial.print("\n");
 
 // If pressure gets too low
 if(pressureValue < basePressure - 20){
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin,HIGH); // write the buzzer pin high to push out the diaphram
  delayMicroseconds(delayValue); // wait for the calculated delay value
  digitalWrite(buzzerPin,LOW); // write the buzzer pin low to pull back the diaphram
  delayMicroseconds(delayValue); // wait againf or the calculated delay value
 }
 
 else {
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW); 
 } 
}
