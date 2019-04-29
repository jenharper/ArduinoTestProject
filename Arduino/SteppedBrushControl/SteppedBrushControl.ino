#include<Uduino.h>
Uduino uduino("SelfEmbodimentBrush"); // Declare and name your object

#include <Wire.h>
#include <Adafruit_MotorShield.h>

const int potentiometerPin = 2;
int potentiometerValue = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

int currentState = 0; //0 = released, 1 = decrease Potentiometer value, 2 = increase Potentiometer value, 3 = paused


//const int potArraySize = 6;
//int potArray[potArraySize] = new int[6];

//initial setable Values
int sweepMax = 800;
int sweepMin = 200;
int resolution = 5;
uint8_t stepType = SINGLE;
int motorSpeed = 60;
int goalPosition = 500;

uint8_t sweepDirection = BACKWARD;

//constants
const int sweepMaxHW = 900;
const int sweepMinHW = 100;
const int minimumDelay = 20; //in milliseconds


void setup()
{
  
  Serial.begin(9600);
  Serial.println("Starting setup!");
//  uduino.addCommand("releaseMotor", releaseMotor);
//  uduino.addCommand("brush", brush);
//  uduino.addCommand("setValues", setValues);
//  uduino.addCommand("debugLevel", stroke);

  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(motorSpeed);  
}


void loop() {
   long time1 = micros();
    potentiometerValue = analogRead(potentiometerPin);
//  uduino.update();

//  if (uduino.isConnected()) {
//    Serial.print(potentiometerValue);
//    Serial.println("!");
//  }

  if (Serial.available() > 0) 
  {
    //Oddly enough, this method of printing is the most memory efficient.  
    Serial.print("P:");
    Serial.print(potentiometerValue);
    Serial.println("!");
    
    String tempString = Serial.readString(); // read the incoming string:
    Serial.print("I received: ");
    Serial.println(tempString);

    if (tempString == "c")
    {
        Serial.println("Releasing");
        releaseMotor ();
    }
    if (tempString == "b") //bigger
    {
      currentState = 2;
      goalPosition = 734;
    }
    if (tempString == "s") //smaller
    {
      currentState = 1;
      goalPosition = 271;
    }
  }

  if ((currentState == 1) || (currentState == 2))
  {
      progressStepper();
  }

  long time2 = micros();
  long deltaMilliseconds =  (time2 - time1)/1000; //delta in milliseconds instead of microseconds
  
  if (deltaMilliseconds < minimumDelay)
  {
      unsigned long delayDiff = minimumDelay - deltaMilliseconds;
      delay(delayDiff);
  }
}

void brush()
{
  int parameters = uduino.getNumberOfParameters();
  
  if (parameters == 2) 
  {
    int motorDirection = uduino.charToInt(uduino.getParameter(0)); 
    int g= uduino.charToInt(uduino.getParameter(1)); 

   //first check that the read in values are in range
   if ((g > sweepMin) && (g < sweepMax) && (motorDirection >=0) && (motorDirection <=1))
   {
     goalPosition = g;
     if (motorDirection == 0)
        currentState = 1;  //motorDirection = 0 = make potValue smaller 
     else
        currentState = 2; //motorDirection = 1 = make potValue larger 
   }
    
  }
}

void progressStepper()
{
  //check hard bounds
  if  ((potentiometerValue > sweepMax) || (potentiometerValue < sweepMin))
  {
      Serial.println("Warning - hit hard value!");
      currentState = 3; //pause  
      return;
  }
  
  if ((currentState == 1) && (potentiometerValue > goalPosition)) //decrease Potentiometer
  {
         myMotor->step(2, FORWARD, INTERLEAVE);
  }
  else if ((currentState == 2) && (potentiometerValue < goalPosition)) //increase Potentiometer
  {
         myMotor->step(2, BACKWARD, INTERLEAVE);
  }
  else
  {
     currentState = 3; //pause due to completion of stroke    
     if (uduino.isConnected()) {
        Serial.println("B:complete!");
     }
     else
     {
        Serial.println("Stroke complete");
     }
  }

}


// releases the motor so it stops generating heat
void releaseMotor () {
  currentState = 0;
   myMotor->release();
   if (uduino.isConnected()) {
    Serial.println("Stepper motor released!");
  }
}

