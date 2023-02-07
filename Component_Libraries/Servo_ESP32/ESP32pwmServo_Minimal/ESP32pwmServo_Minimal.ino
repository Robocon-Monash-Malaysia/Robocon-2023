
// This is a minimal example sketch to use servos with the esp32
// for a in depth understand of using servo in general, please refer to this youtube video https://youtu.be/1WnGv-DPexc



/*
 esp32 specific

    for the esp32, any digital output pin can be a pwm pin (unlike arduino being only on specific pins)
    but there are limitation for the esp32 where we are only allowed 16 pwm channels (channel 0 to 15)
    channels are what allows us to change the duty cycle which is important for the servo motors, the following link tells in detail on what the esp32 pwm pins are capable of
    https://randomnerdtutorials.com/esp32-pwm-arduino-ide/

    so for this example, we would use pin 18, and channel 0 to control our first servo 

*/

void setup() {

  // void PWMsetup(int channel, int pin)
  PWMsetup(0, 18);  // set pin 18 to be a servo pin, on channel 0
}

void loop() {
  // for our setPos function, we would need to call the channel that we need to change, and let the servo know how many degree we want (for our 180 degree motor, it is from 0 to 180 degrees that we can enter)

  // void setPos(int channel, int deg);
  setPos(0, 50);   // turn to 50 degree
  delay(1000);  
  setPos(0, 120);  //turn to 120 degree
  delay(1000);
}
