int redLED = 4; // LED pin for pressure warning
int greenLED = 2; // LED pin for start use
int yellowLED = 3; // LED pin for initialization
int buzzerPin = 7; // Buzzer pin
int pressurePin = A0; // Pin to take pressure reading
int pressureValue; // Pressure value in analog units
float pressureInmmHg; // Pressure value in mmHg
float desiredVoltage = (((760*4/15/51.7149326)+.5)*1024/5); // Desired analog read value (assuming 17mmHg)
float basePressure; // Pressure that will be used as base (intraocular pressure)
long delayValue = 1000000/2500/2; // Delay for buzzer

// Required variables for running average
const int numReadings = 10;
float total = 0;
float average;

// Test variables
int averageCounter = 0;
int canBuzz = 0;
boolean turnOnLED = false;

void setup(){
 
 pinMode(yellowLED, OUTPUT); 
  
 pinMode(redLED, OUTPUT);
 
 pinMode(greenLED, OUTPUT);
 
 pinMode(buzzerPin, OUTPUT);
 
 Serial.begin(9600); 
}

void loop(){
 
  if (canBuzz == 1){
     for(long i = 0; i < 1250; i++)
     {
      digitalWrite(buzzerPin,HIGH); // write the buzzer pin high to push out the diaphram
      delayMicroseconds(delayValue); // wait for the calculated delay value
      digitalWrite(buzzerPin,LOW); // write the buzzer pin low to pull back the diaphram
      delayMicroseconds(delayValue); // wait againf or the calculated delay value
     }
   }
  
  else {
     pressureValue = analogRead(pressurePin);
     pressureInmmHg = (((float)pressureValue * 5 / 1024) - .5) * 15 / 4 * 51.7149326; 
 
     // Turn on LED if at desiredVoltage +/- 5
     if (pressureValue > desiredVoltage - 5 && pressureValue < desiredVoltage + 5)
     {
         // Set variable to allow LED lighting
         if( averageCounter == 0){
           averageCounter = 1;
         }
     
         // Turn on green LED when user has reached desiredVoltage
         if (averageCounter >= 0 && averageCounter <= 10){
           turnOnLED = true;
         }
         else {
           turnOnLED = false;
         }
     
     }
 
     else {
       turnOnLED = false;
     }
 
     if (turnOnLED == true)
     {
      digitalWrite(yellowLED, HIGH); 
     }
     else {
      digitalWrite(yellowLED, LOW);
     }

    if (averageCounter >= 1 && averageCounter <= 10)
    {
      total += pressureValue;
      average = (float)total / (float)numReadings; 
 
      if (averageCounter == 10){
         basePressure = average;
      }
      
      averageCounter += 1;
    }

    if (averageCounter >= 10) {
      digitalWrite(greenLED, HIGH);
    }
    else{
      digitalWrite(greenLED, LOW);
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
      digitalWrite(redLED, HIGH);
      canBuzz = 1;
     }
 
     else {
      digitalWrite(redLED, LOW);
      digitalWrite(buzzerPin, LOW); 
     }
  } 
}
