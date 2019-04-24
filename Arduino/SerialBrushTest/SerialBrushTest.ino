#include<Uduino.h>
Uduino uduino("myBrushArduino"); // Declare and name your object

#include <Wire.h>
#include <Adafruit_MotorShield.h>

int potentiometerPin = 2;
int potentiometerValue = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

int currentState = 0; //0 = not running, 1=running, 2=complete, 3=stuck
int currentStrokeCounter = 0;
int strokes = 10; // number of back and forth strokes in brushSequence
int motorSpeed = 20;
int stepsPerCycle = 10;
int incomingByte = 0;

void setup()
{
  
  Serial.begin(9600);
    Serial.println("Starting setup!");
//  uduino.addCommand("resetBrush", resetBrush);
//  uduino.addCommand("brushSequence", brushSequence);
//  uduino.addCommand("stroke", stroke);
//  uduino.addCommand("strokeCycle", strokeCycle);
//  uduino.addCommand("releaseMotor", releaseMotor);
//  uduino.addCommand("setValues", setValues);
//
    AFMS.begin();  // create with the default frequency 1.6KHz
    myMotor->setSpeed(motorSpeed);  
}

void loop()
{
    potentiometerValue = analogRead(potentiometerPin);
//  uduino.update();

//  if (uduino.isConnected()) {
//    Serial.print(potentiometerValue);
//    Serial.println("!");
//  }

  if (Serial.available() > 0) 
  {
    Serial.print(potentiometerValue);
    Serial.println("!");
    
    String tempString = Serial.readString(); // read the incoming string:
    Serial.print("I received: ");
    Serial.println(tempString);

    if (tempString == "b")
    {
       Serial.println("Moving motor backward");
       myMotor->step(20, BACKWARD, INTERLEAVE);
        potentiometerValue = analogRead(potentiometerPin);
         Serial.print(potentiometerValue);
    Serial.println("!");
    }
    if (tempString == "f")
    {
       Serial.println("Moving motor forward");
       myMotor->step(20, FORWARD, INTERLEAVE);
        potentiometerValue = analogRead(potentiometerPin);
         Serial.print(potentiometerValue);
    Serial.println("!");
    }

//    if (incomingByte == 3)
//    {
//       Serial.println("Moving motor forward");
//    }
//    if (incomingByte == 4)
//    {
//       Serial.println("Moving motor backwards");
//    }
    
  }


  if (currentState == 1)
  {
      progressStepper();
  }
  
 
  
    delay(500); // Delay of your choice or to match Unity's Read Timout
}

void progressStepper()
{
  ;
}

// goes to brushSequenceState
void brushSequence () {
 currentState = 1;
 currentStrokeCounter = 0;
}

// releases the motor so it stops generating heat
void releaseMotor () {
  currentState = 0;
   myMotor->release();
   if (uduino.isConnected()) {
    Serial.println("Stepper motor released!");
  }
}

