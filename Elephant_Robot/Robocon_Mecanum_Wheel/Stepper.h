//https://microcontrollerslab.com/stepper-motor-a4988-driver-module-esp32/

#include <AccelStepper.h>

const int DIR = 5;
const int STEP = 18;

#define motorInterfaceType 1
AccelStepper motor(motorInterfaceType, STEP, DIR);

void setup() {
  Serial.begin(115200);
  motor.setMaxSpeed(1000);
  motor.setAcceleration(60);
  motor.setSpeed(200);
  motor.moveTo(200);
}

void loop() {
  if (motor.distanceToGo() == 0) {
    motor.moveTo(-motor.currentPosition());
    Serial.println("Rotating Motor in opposite direction...");
  }
  motor.run();
}








// For L298N Driver
#include <Stepper.h>

// Constant Variables
const int stepperSpeed = 300;       // Stepper Speed in RPM
const int stepsPerRevolution = 200; // Steps per Revolution for Stepper (Rated - Don't Change)

// Internal Variables
static int stepCount = 0;
static const int stepMax = 14900;
static const int stepMin = 100;

// ULN2003 Motor Driver Pins
#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27

// Stepper Definition
Stepper myStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);

// Stpper Initialization
void Stepper_setup() {
  // Speed in RPM
  myStepper.setSpeed(stepperSpeed);
}

// Stepper Service Routine
void Stepper_run(int L2, int R2) {
  // Gripper Going Downwards
  if(!L2 && R2) {
    if(stepCount > stepMin) {
      myStepper.step(stepsPerRevolution);
      stepCount = stepCount - stepsPerRevolution;
    }
    Serial.print("Gripper Going Downwards. Step: ");
    Serial.println(stepCount);
  }

  // Gripper Going Upwards
  if(L2 && !R2) {
    if(stepCount < stepMax) {
      myStepper.step(-stepsPerRevolution);
      stepCount = stepCount + stepsPerRevolution;
    }
    Serial.print("Gripper Going Upwards. Step: ");
    Serial.println(stepCount);
  }
}
