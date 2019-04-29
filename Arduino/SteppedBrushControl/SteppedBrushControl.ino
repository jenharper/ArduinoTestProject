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
  uduino.addCommand("releaseMotor", releaseMotor);
  uduino.addCommand("brush", brush);
  uduino.addCommand("setValues", setValues);

  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(motorSpeed);  
}


void loop() {
  long time1 = micros();
  potentiometerValue = analogRead(potentiometerPin);
  uduino.update();

  if (uduino.isConnected()) {
   //Oddly enough, this method of printing is the most memory efficient.  
    Serial.print("P:");
    Serial.print(potentiometerValue);
    Serial.println("!");
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
         myMotor->step(resolution, FORWARD, stepType);
  }
  else if ((currentState == 2) && (potentiometerValue < goalPosition)) //increase Potentiometer
  {
         myMotor->step(resolution, BACKWARD, stepType);
  }
  else
  {
     currentState = 3; //pause due to completion of stroke    
     if (uduino.isConnected()) {
        Serial.println("B:complete!");
     }
  }

}


// releases the motor so it stops generating heat
void releaseMotor () {
   currentState = 0;
   myMotor->release();
   if (uduino.isConnected()) {
    Serial.println("B:released!");
  }
}

//Change the operation values
void setValues() {
 
  int parameters = uduino.getNumberOfParameters(); 
  
  if (parameters == 5) {
    int requestSweepMax = uduino.charToInt(uduino.getParameter(0));
    int requestSweepMin = uduino.charToInt(uduino.getParameter(1)); 
    int requestResolution = uduino.charToInt(uduino.getParameter(2)); 
    int requestStepType = uduino.charToInt(uduino.getParameter(3));
    int requestSpeed = uduino.charToInt(uduino.getParameter(4));

    if ((requestSweepMax > requestSweepMin) && (requestSweepMax <= sweepMaxHW) && (requestSweepMin >= sweepMinHW))
    {
       sweepMax = requestSweepMax;
       sweepMin = requestSweepMin;
       if (uduino.isConnected()) 
       {
          Serial.print("S:setting Max=");
          Serial.print(sweepMax);
          Serial.print(" Min=");
          Serial.print(sweepMin);
          Serial.println("!");
        }
    }

    if ((requestResolution > 0) && (requestResolution <= 20))
    {
        resolution = requestResolution;
        if (uduino.isConnected()) 
        {
          Serial.print("S:setting Resolution=");
          Serial.print(resolution);
          Serial.println("!");
        }
    }

    if (requestStepType == SINGLE)
    {
      stepType = SINGLE;
      if (uduino.isConnected()) 
      {
          Serial.print("S:setting stepType=SINGLE!");
      }
    }
    else if (requestStepType == DOUBLE)
     {
      stepType = DOUBLE;
      if (uduino.isConnected()) 
      {
          Serial.print("S:setting stepType=DOUBLE!");
      }
    }
    else if (requestStepType == INTERLEAVE)
    {
      stepType = INTERLEAVE;
      if (uduino.isConnected()) 
      {
          Serial.print("S:setting stepType=INTERLEAVE!");
      }
    }
  
    if ((requestSpeed >= 1) && (requestSpeed <= 100))
    {
        motorSpeed = requestSpeed;
        myMotor->setSpeed(motorSpeed); 
        if (uduino.isConnected()) 
        {
          Serial.print("S:setting motorSpeed=");
          Serial.print(motorSpeed);
          Serial.println("!");
        }
    }
    
  }
}


