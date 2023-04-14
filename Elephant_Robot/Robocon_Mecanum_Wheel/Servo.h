//#include <Servo.h>

// #include "Esp32_Servo_Library.h"

// Servo Pins
static const int loadingServo = 18;  // Servo for loading rings
static const int aimingServo = 3;    // Servo for aiming direction

// pwm channels
static const int loadingServoChannel = 5;  // Servo for loading rings
static const int aimingServoChannel = 2;   // Servo for aiming direction


// Loader mechanism
static const int LoadWaitTime = 1000;  // Loader Wait Time in millis
static boolean LoadReady = true;
static boolean LoadMoving = false;
static unsigned long LoadStartTime = 0;
unsigned long prevMillisLoad = 0;
unsigned long curMillisLoad = 0;


// Arrow mechanism
// static const int LoadWaitTime = 1000;  // Loader Wait Time in millis
// static boolean LoadReady = true;
// static boolean LoadMoving = false;
// static unsigned long LoadStartTime = 0;
// unsigned long prevMillisLoad = 0;
// unsigned long curMillisLoad = 0;



// Function Prototypes
  void run_Loader(int);


// Servos Initialization
void Servo_setup() {
  PWMsetup(loadingServoChannel, loadingServo);  // Loading servo 
  setPos(loadingServoChannel, 0); // set initial loader position to 0
  // Serial.println("running");
}

// Servos Service Routine
void Servo_run(int Triangle, int Square, int L1, int R1) {
  run_Loader(Triangle);
}


void run_Loader(int Triangle) {
  if (Triangle && LoadReady) {  // Josh added LoadReady condition
    LoadStartTime = millis();
    LoadReady = false;
    // Load servos set to 0 deg
    setPos(loadingServoChannel, 90);

    // Load.write(90);
    prevMillisLoad = curMillisLoad;
    // Serial.println("Loader Initiated.");
  }


  curMillisLoad = millis();
  if (!LoadMoving && !LoadReady && (curMillisLoad - prevMillisLoad >= LoadWaitTime)) {
    LoadStartTime = millis();
    LoadMoving = true;
  }

  if (LoadMoving) {
    // Load servos set to 0 deg
    unsigned long progress = millis() - LoadStartTime;

    if (progress <= 2500) {
      long LoadAngle = map(progress, 0, 1500, 90, 0);
      setPos(loadingServoChannel, LoadAngle);
      //Load.write(0);
    }

    if (progress > 2500) {
      // Serial.println("Load Reset.");
      LoadMoving = false;
      LoadReady = true;
    }
  }
}


