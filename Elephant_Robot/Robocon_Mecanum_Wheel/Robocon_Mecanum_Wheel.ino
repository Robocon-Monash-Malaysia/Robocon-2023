// // Additional Functionalities
// #include "OTA.h"
// #include "User_interface.h"
// #include "MPU6050.h"

bool stopFlag = false;
int flywheelSpeed = 200;

void setup();
void loop();
void system();

// Functional Macros
#include "Esp32_Servo_Library.h"
#include "PS3.h"
#include "Mecanum.h"
#include "Servo.h"
#include "DC_Motor.h"
#include "Stepper.h"
#include "OLED.h"
// #include "Vision.h"


void setup() {
  // Setup Read Spead and Serial monitor's Baud Rate
  // analogReadResolution(12);
  Serial.begin(115200);

  // // Additional Macro Setups
  // OTA_setup();
  // User_interface_setup();
  // MPU6050_setup();

  // Macro Setups
  PS3_setup();
  Mecanum_setup();
  Servo_setup();
  OLED_setup();
  stepper_setup();

  // dc_motor_setup();
  // vision_setup();


  Mecanum(Mecanum_Speed);
  while (!Ps3.isConnected()) {
    Mecanum(Mecanum_Speed);
    PS3_move_Mecanum(stick_LX, stick_LY, stick_RX, stick_RY);
  }


  for (int i = 0; i < flywheelSpeed; i++) {
    OLED_display_stats();
    flywheel_motor(i);
    delay(5);  // takes 5 seconds to spin up
  }
}

void loop() {
  PS3_move_Mecanum(stick_LX, stick_LY, stick_RX, stick_RY);
  Servo_run(Triangle, Square, L1, R1);
  flywheel_motor(flywheelSpeed);
  stepper_loop(L1, R1);
  // vision_loop();
}

void system() {
}
