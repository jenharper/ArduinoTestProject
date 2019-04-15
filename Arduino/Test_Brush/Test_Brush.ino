#include<Uduino.h>
Uduino uduino("myBrushArduino"); // Declare and name your object

#include <Wire.h>
//#include <Adafruit_MotorShield.h>

int potentiometerPin = 2;
int potentiometerValue = 0;

// Create the motor shield object with the default I2C address
//Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 


// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
//Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

int stepsBetweenStops = 220;
int resetSteps = 220; // should usually be the same as stepsBetweenStops to ensure hitting the stop
int strokes = 10; // number of back and forth strokes in brushSequence
int motorSpeed = 20;
float swingRadius = .5; // percent of the stepsBetweenStops used in a standard stroke

void setup()
{
  Serial.begin(9600);
//  uduino.addCommand("resetBrush", resetBrush);
//  uduino.addCommand("brushSequence", brushSequence);
//  uduino.addCommand("stroke", stroke);
//  uduino.addCommand("strokeCycle", strokeCycle);
//  uduino.addCommand("releaseMotor", releaseMotor);
//  uduino.addCommand("setValues", setValues);
//
//  AFMS.begin();  // create with the default frequency 1.6KHz
//  myMotor->setSpeed(motorSpeed);  // 10 rpm
}

void loop()
{
  potentiometerValue = analogRead(potentiometerPin);
  uduino.update();

  if (uduino.isConnected()) {
    Serial.print(potentiometerValue);
    Serial.println("!");
  }

    delay(10); // Delay of your choice or to match Unity's Read Timout
}

