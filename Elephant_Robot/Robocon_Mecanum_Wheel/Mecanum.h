// Mecanum control system for Cytron Motor driver
// Also includes PS3 for Mecanum drive


//  Mecanum control pin layout   [ Left Front, Right Front , Back Left , Back Right]
// const byte MecanumPin1[] =  {2, 16, 19, 5};
// const byte MecanumPWM[] = {15,  4, 18, 17};
const byte MecanumPin1[] =  {19, 5, 2, 16};
const byte MecanumPWM[] = {18,  17, 15, 4};

const byte MecanumChannel[] = {6, 7, 8, 9};

int Mecanum_Speed[] = {0, 0, 0, 0};
const byte Mecanum_PWM_resolution = 8;//16;
const int Mecanum_PWM_resolution_max_value = 255;//65536;


void Mecanum(int Speed[])
{
  // invert back Mecanums of the system
  Speed[0] = -Speed[0];
  Speed[1] = Speed[1];
  Speed[2] = -Speed[2];
  Speed[3] = Speed[3];

 // for loop to run each Mecanums individually
  for (int x = 0; x < sizeof(MecanumPin1); x++)
  {

    if (Speed[x] > 0)
    { // Forward
      digitalWrite(MecanumPin1[x], HIGH);
      ledcWrite(MecanumChannel[x] , abs(Speed[x]));
    }
    else if (Speed[x] < 0)
    { // Reverse
      digitalWrite(MecanumPin1[x], LOW);
      ledcWrite(MecanumChannel[x] , abs(Speed[x]));
    }
    else
    { //Stop
      digitalWrite(MecanumPin1[x], LOW);
      ledcWrite(MecanumChannel[x] , 0);
    }
  }
  
}

void PS3_move_Mecanum(int LX, int LY, int RX , int RY)
{
  // angle from 0 to 2*pi
  float angle = PS3_LeftAnalogStickAngle (LX, LY)  ;
   angle = angle - (90*PI)/180  ;

  //Speed  (range of 0 to 100)
  float Speed_total_percent = PS3_LeftAnalogStickSpeed(LX, LY);

  // turn speed for rotational movement
  float turn_Speed = 0;

  // filter to ignore inputs below 15 from controller for rotation only
  if (abs(RX) > 15) turn_Speed = map(RX, -128, 127 , -Mecanum_PWM_resolution_max_value, Mecanum_PWM_resolution_max_value);


  /*Mecanum wheels programming instructions found on this website
    https://seamonsters-2605.github.io/archive/mecanum
    similar principle is applied, however the direction of motion was changed (clockwise being positive angle)
  */

  Mecanum_Speed[0] = map(sin(angle + (1 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -Mecanum_PWM_resolution_max_value, Mecanum_PWM_resolution_max_value);
  Mecanum_Speed[1] = map(sin(angle + (3 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -Mecanum_PWM_resolution_max_value, Mecanum_PWM_resolution_max_value);
  Mecanum_Speed[2] = map(sin(angle + (3 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -Mecanum_PWM_resolution_max_value, Mecanum_PWM_resolution_max_value);
  Mecanum_Speed[3] = map(sin(angle + (1 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -Mecanum_PWM_resolution_max_value, Mecanum_PWM_resolution_max_value);

  //apply turn Speed to allow control using right analog stick
  Mecanum_Speed[0] = Mecanum_Speed[0] + turn_Speed;
  Mecanum_Speed[1] = Mecanum_Speed[1] - turn_Speed;
  Mecanum_Speed[2] = Mecanum_Speed[2] + turn_Speed;
  Mecanum_Speed[3] = Mecanum_Speed[3] - turn_Speed;

  // run all Mecanums with according speeds
  Mecanum(Mecanum_Speed);

}



void Mecanum_setup()
{
  // Mecanum setup
  for (int x = 0; x < sizeof(MecanumPin1); x++)
  {
    pinMode(MecanumPin1[x], OUTPUT);
    ledcSetup(MecanumChannel[x], 5000, Mecanum_PWM_resolution);
    ledcAttachPin(MecanumPWM[x] , MecanumChannel[x] );
  }
}

/*
void PS3_move_DPAD()
{
 // int Speed = pot_Value = map(analogRead(pot_Pin), 0, 4095, 0, PWM_resolution_max_value) ;
 int Speed =150 ;

 if ((Ps3.data.button.up == 0)  && (Ps3.data.button.right == 0) && (Ps3.data.button.down == 0) && (Ps3.data.button.left == 0) && (Ps3.data.button.l1 == 0) && (Ps3.data.button.r1 == 0))
 {
   Mecanum_Speed[0] = 0;
   Mecanum_Speed[1] = 0;
   Mecanum_Speed[2] = 0;
   Mecanum_Speed[3] = 0;
 }
 else if ((Ps3.data.button.up == 1)  && (Ps3.data.button.right == 0) && (Ps3.data.button.down == 0) && (Ps3.data.button.left == 0))
 {
   Mecanum_Speed[0] = +Speed;
   Mecanum_Speed[1] = +Speed;
   Mecanum_Speed[2] = +Speed;
   Mecanum_Speed[3] = +Speed;
 }
 else if ((Ps3.data.button.up == 1)  && (Ps3.data.button.right == 1) && (Ps3.data.button.down == 0) && (Ps3.data.button.left == 0))
 {
   Mecanum_Speed[0] = +Speed;
   Mecanum_Speed[1] = 0;
   Mecanum_Speed[2] = 0;
   Mecanum_Speed[3] = +Speed;
 }
 else if ((Ps3.data.button.up == 0)  && (Ps3.data.button.right == 1) && (Ps3.data.button.down == 0) && (Ps3.data.button.left == 0))
 {
   Mecanum_Speed[0] = +Speed;
   Mecanum_Speed[1] = -Speed;
   Mecanum_Speed[2] = -Speed;
   Mecanum_Speed[3] = +Speed;
 }
 else if ((Ps3.data.button.up == 0)  && (Ps3.data.button.right == 1) && (Ps3.data.button.down == 1) && (Ps3.data.button.left == 0))
 {
   Mecanum_Speed[0] = 0;
   Mecanum_Speed[1] = -Speed;
   Mecanum_Speed[2] = -Speed;
   Mecanum_Speed[3] = 0;
 }
 else if ((Ps3.data.button.up == 0)  && (Ps3.data.button.right == 0) && (Ps3.data.button.down == 1) && (Ps3.data.button.left == 0))
 {
   Mecanum_Speed[0] = -Speed;
   Mecanum_Speed[1] = -Speed;
   Mecanum_Speed[2] = -Speed;
   Mecanum_Speed[3] = -Speed;
 }
 else if ((Ps3.data.button.up == 0)  && (Ps3.data.button.right == 0) && (Ps3.data.button.down == 1) && (Ps3.data.button.left == 1))
 {
   Mecanum_Speed[0] = -Speed;
   Mecanum_Speed[1] = 0;
   Mecanum_Speed[2] = 0;
   Mecanum_Speed[3] = -Speed;
 }
 else if ((Ps3.data.button.up == 0)  && (Ps3.data.button.right == 0) && (Ps3.data.button.down == 0) && (Ps3.data.button.left == 1))
 {
   Mecanum_Speed[0] = -Speed;
   Mecanum_Speed[1] = +Speed;
   Mecanum_Speed[2] = +Speed;
   Mecanum_Speed[3] = -Speed;
 }
 else if ((Ps3.data.button.up == 1)  && (Ps3.data.button.right == 0) && (Ps3.data.button.down == 0) && (Ps3.data.button.left == 1))
 {
   Mecanum_Speed[0] = 0;
   Mecanum_Speed[1] = Speed;
   Mecanum_Speed[2] = Speed;
   Mecanum_Speed[3] = 0;
 }
 else if ((Ps3.data.button.l1 == 1) && (Ps3.data.button.r1 == 0))
 {
   Mecanum_Speed[0] = -Speed;
   Mecanum_Speed[1] = Speed;
   Mecanum_Speed[2] = -Speed;
   Mecanum_Speed[3] = Speed;
 }
 else if ((Ps3.data.button.l1 == 0) && (Ps3.data.button.r1 == 1))
 {
   Mecanum_Speed[0] = Speed;
   Mecanum_Speed[1] = -Speed;
   Mecanum_Speed[2] = Speed;
   Mecanum_Speed[3] = -Speed;
 }
 else
 {
   Mecanum_Speed[0] = 0;
   Mecanum_Speed[1] = 0;
   Mecanum_Speed[2] = 0;
   Mecanum_Speed[3] = 0;
 }

 Mecanum(Mecanum_Speed);
}*/