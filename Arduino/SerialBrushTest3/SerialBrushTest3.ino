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

int currentState = 0; //0 = not running, 1=running, 2=sweep complete, 3=return to start, 4 = return to start complete, 5 = stuck
int currentStrokeCounter = 0;
int strokes = 10; // number of back and forth strokes in brushSequence
int motorSpeed = 20;
int stepsPerCycle = 10;
int incomingByte = 0;

const int sweepMax = 634;
const int sweepMin = 371;
const int sweepHoldPosition = 306;
const int strokesToDo = 10;
const int minimumDelay = 20; //in milliseconds
uint8_t sweepDirection = BACKWARD;

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
    long time1 = micros();
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
        currentState = 0;
        Serial.println("Moving motor backward");
        long time1 = micros();
        myMotor->step(5, BACKWARD, INTERLEAVE);
        long time2 = micros();
        long delta = time2 - time1;
        potentiometerValue = analogRead(potentiometerPin);
        Serial.print(potentiometerValue);
        Serial.println("!");
        Serial.print(delta);
        Serial.println(" microseconds");
    }
    if (tempString == "f")
    {
        currentState = 0;
        Serial.println("Moving motor forward");
        long time1 = micros();
        myMotor->step(5, FORWARD, INTERLEAVE);
        long time2 = micros();
        long delta = time2 - time1;
        potentiometerValue = analogRead(potentiometerPin);
        Serial.print(potentiometerValue);
        Serial.println("!");
        Serial.print(delta);
        Serial.println(" microseconds");
    }
    if (tempString == "s")
    {
        Serial.println("Stopping");
        currentState = 0;
    }
    if (tempString == "g")
    {
        Serial.println("Starting");
        brushSequence ();
    }
    if (tempString == "r")
    {
        Serial.println("Returning");
        returnToStart ();
    }
    if (tempString == "c")
    {
        Serial.println("Releasing");
        releaseMotor ();
    }
   
  }


  if (currentState == 1)
  {
      progressStepper();
  }
  if (currentState == 3)
  {
      progressToStart();
  }
  
  long time2 = micros();
  long deltaMilliseconds =  (time2 - time1)/1000; //delta in milliseconds instead of microseconds
  
 
  if (deltaMilliseconds < minimumDelay)
  {
      unsigned long delayDiff = minimumDelay - deltaMilliseconds;
      delay(delayDiff);
  }
  
  // delay(10); // Delay of your choice or to match Unity's Read Timout
}

void progressStepper()
{
  if (sweepDirection == BACKWARD)
  {
    //if past max value, reverse direction
     if  (potentiometerValue > sweepMax)
     {
         currentStrokeCounter++;
         sweepDirection = FORWARD;
     }
  }
  else if (sweepDirection == FORWARD)
  {
    if (potentiometerValue < sweepMin)
    {
         currentStrokeCounter++;
         sweepDirection = BACKWARD;
    }
  }

  if (currentStrokeCounter > strokesToDo)
  {
    currentState = 2;
  }
  else
  {
    myMotor->step(2, sweepDirection, INTERLEAVE);
  }
  
}

void progressToStart()
{
    if (potentiometerValue > sweepHoldPosition)
    {
        myMotor->step(1, FORWARD, INTERLEAVE);
    }
    else
    {
      currentState = 4;
    }
}

void returnToStart ()
{
    currentState = 3;
}

// goes to brushSequenceState
void brushSequence () {
 currentState = 1;
 currentStrokeCounter = 0;
 sweepDirection = BACKWARD;
}

// releases the motor so it stops generating heat
void releaseMotor () {
  currentState = 0;
   myMotor->release();
   if (uduino.isConnected()) {
    Serial.println("Stepper motor released!");
  }
}

