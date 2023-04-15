// motor control system for Cytron motor driver
// Also includes PS3 to motor control


//  motor control pin layout   [Collector motor]
const byte motorPin1[] = { 12 };
const byte motorPWM[] = { 14 };

// flywheel control
const byte flywheelPin1 = 33;
const byte flywheelPin2 = 32;

const byte motorChannel[] = { 12, 13 };



// constants
int motor_Speed[] = { 0, 0 };
const byte PWM_resolution = 8;             //16;
const int PWM_resolution_max_value = 255;  //65536;


void dc_motor_setup() {
  // set motor pins as output
  // for (int x = 0; x < sizeof(motorPin1); x++) {
  //   pinMode(motorPin1[x], OUTPUT);
  //   ledcSetup(motorChannel[x], 5000, PWM_resolution);  // up to 16 channels
  //   ledcAttachPin(motorPWM[x], motorChannel[x]);
  // }

  ledcSetup( motorChannel[0], 5000, PWM_resolution);
  ledcSetup( motorChannel[1], 5000, PWM_resolution);
  ledcAttachPin(flywheelPin1, motorChannel[0]);
  ledcAttachPin(flywheelPin2, motorChannel[1]);
  //ledcSetup(motorChannel[1], 5000, PWM_resolution);  // up to 16 channels
  //ledcAttachPin(flywheelPin, motorChannel[1]);
}


void flywheel_motor(int runSpeed) {


  // system();
  if (runSpeed > PWM_resolution_max_value) runSpeed = PWM_resolution_max_value;
  else if (runSpeed < -PWM_resolution_max_value) runSpeed = -PWM_resolution_max_value;

  if (runSpeed > 0) {  // Forward
    //digitalWrite(motorPin1[0], LOW);
    ledcWrite(motorChannel[0], 0);
    ledcWrite(motorChannel[1], runSpeed);
    //ledcWrite(motorChannel[0], abs(runSpeed));
  } else if (runSpeed < 0) {  // Reverse
  //digitalWrite(motorPin1[0], HIGH);
    ledcWrite(motorChannel[1], 0);
    ledcWrite(motorChannel[0], runSpeed);
    // ledcWrite(motorChannel[x], abs(runSpeed));
    //ledcWrite(motorChannel[0], 0);
  } else {  //Stop
    //digitalWrite(motorPin1[0], HIGH);
    ledcWrite(motorChannel[0], 0);
    ledcWrite(motorChannel[1], 0);
    //ledcWrite(motorChannel[0], 0);
  }
}

void dc_motor(int Speed[]) {
  // invert back motors of the system
  // Speed[0] = Speed[0];
  // Speed[1] = Speed[1];
  // Speed[2] = -Speed[2];
  // Speed[3] = -Speed[3];

  // for loop to run each motors individually
  for (int x = 0; x < sizeof(motorPin1); x++) {

    if (Speed[x] > 0) {  // Forward
      digitalWrite(motorPin1[x], HIGH);
      ledcWrite(motorChannel[x], abs(Speed[x]));
    } else if (Speed[x] < 0) {  // Reverse
      digitalWrite(motorPin1[x], LOW);
      ledcWrite(motorChannel[x], abs(Speed[x]));
    } else {  //Stop
      digitalWrite(motorPin1[x], LOW);
      ledcWrite(motorChannel[x], 0);
    }
  }
}
