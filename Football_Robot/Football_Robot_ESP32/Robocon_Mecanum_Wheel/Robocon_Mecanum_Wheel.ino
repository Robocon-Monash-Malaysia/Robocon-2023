// // Additional Functionalities
// #include "OTA.h"
// #include "User_interface.h"
// #include "MPU6050.h"

// Functional Macros
#include "PS3.h"
#include "Movement.h"

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
  Movement_setup();

  motor(motor_Speed);
  while (!Ps3.isConnected()) {
    motor(motor_Speed);
    PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);
  }

}

void loop() {
  PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);


}
