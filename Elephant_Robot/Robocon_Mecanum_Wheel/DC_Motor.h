// motor control system for Cytron motor driver
// Also includes PS3 to motor control


//  motor control pin layout   [Collector motor]
const byte motorPin1[] = {12};
const byte motorPWM[] = {14};

// flywheel control
const byte flywheelPin = 32;

const byte motorChannel[] = {4,0};



// constants
int motor_Speed[] = {0,0};
const byte PWM_resolution = 8;             //16;
const int PWM_resolution_max_value = 255;  //65536;


void dc_motor_setup() {
  // set motor pins as output
  for (int x = 0; x < sizeof(motorPin1); x++) {
    pinMode(motorPin1[x], OUTPUT);
    ledcSetup(motorChannel[x], 5000, PWM_resolution);  // up to 16 channels
    ledcAttachPin(motorPWM[x], motorChannel[x]);
  }

    ledcSetup(motorChannel[1], 5000, PWM_resolution);  // up to 16 channels
    ledcAttachPin(flywheelPin, motorChannel[1]);

}


void flywheel_motor(int Speed[]) {
  system();
  for (int x = 0; x < sizeof(motorPin1); x++) {
    if (Speed[x] > PWM_resolution_max_value) Speed[x] = PWM_resolution_max_value;
    else if (Speed[x] < -PWM_resolution_max_value) Speed[x] = -PWM_resolution_max_value;

    if (Speed[x] > 0) {  // Forward
      digitalWrite(motorPin1[x], LOW);
      ledcWrite(motorChannel[x], abs(Speed[x]));
    } else if (Speed[x] < 0) {  // Reverse
      digitalWrite(motorPin1[x], HIGH);
      ledcWrite(motorChannel[x], abs(Speed[x]));
    } else {  //Stop
      digitalWrite(motorPin1[x], HIGH);
      ledcWrite(motorChannel[x], 0);
    }
  }
}

void dc_motor(int Speed[])
{
  // invert back motors of the system
  // Speed[0] = Speed[0];
  // Speed[1] = Speed[1];
  // Speed[2] = -Speed[2];
  // Speed[3] = -Speed[3];

 // for loop to run each motors individually
  for (int x = 0; x < sizeof(motorPin1); x++)
  {

    if (Speed[x] > 0)
    { // Forward
      digitalWrite(motorPin1[x], HIGH);
      ledcWrite(motorChannel[x] , abs(Speed[x]));
    }
    else if (Speed[x] < 0)
    { // Reverse
      digitalWrite(motorPin1[x], LOW);
      ledcWrite(motorChannel[x] , abs(Speed[x]));
    }
    else
    { //Stop
      digitalWrite(motorPin1[x], LOW);
      ledcWrite(motorChannel[x] , 0);
    }
  }
  
}
