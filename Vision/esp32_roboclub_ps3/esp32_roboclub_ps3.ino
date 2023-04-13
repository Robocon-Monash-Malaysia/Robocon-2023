# include "Esp32_Servo_Library.h"
#include "PS3.h"

#define RXD2 16
#define TXD2 17
#define RXD1 27
#define TXD1 26

float* arr1;
float* arr2;
int arr1Size = 0;
int arr2Size = 0;
int currentArr = 1;
int arrPointer = 0;

bool square = false;
bool triangle = false;
bool circle = false;
bool cross = false;

bool dataReceived = false;
bool requesting = false;

void buttonPressed() {
  if (Ps3.event.button_down.square & !square) {Serial.println("Square Button pressed"); square = true;}  
  if (Ps3.event.button_down.triangle & !triangle) {Serial.println("Triangle Button pressed"); triangle = true;}
  if (Ps3.event.button_down.cross & !cross) {Serial.println("Cross Button pressed"); cross = true;}
  if (Ps3.event.button_down.circle & !circle) {Serial.println("Circle Button pressed"); circle = true;}
  if (Ps3.event.button_up.square & square) {square = false;}  
  if (Ps3.event.button_up.triangle & triangle) {triangle = false;}
  if (Ps3.event.button_up.cross & cross) {cross = false;}
  if (Ps3.event.button_up.circle & circle) {circle = false;}
}

void setup() {
  Serial.begin(9600);
  Serial.flush();
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1); //comm with raspberry
  //Serial1.setTimeout(1000);
  Serial1.flush();
  Serial2.begin(115200, RXD2, TXD2); //lidar
  Serial2.flush();

  PS3_setup();
  PWMsetup(0, 32);  //set pin 32 to servo pin, channel 0
  delay(100);
  setPos(0, 90);
}

void resizeArray1(float element) {
  float* new_arr = new float[arr1Size + 1];
  for (int i = 0; i < arr1Size; i++) {
      new_arr[i] = arr1[i];
  }
  new_arr[arr1Size] = element;
  delete[] arr1;
  arr1 = new_arr;
  arr1Size++;
}

void resizeArray2(float element) {
  float* new_arr = new float[arr2Size + 1];
  for (int i = 0; i < arr2Size; i++) {
      new_arr[i] = arr2[i];
  }
  new_arr[arr2Size] = element;
  delete[] arr2;
  arr2 = new_arr;
  arr2Size++;
}

void resetArr() {
  float* new_arr1;
  float* new_arr2;
  delete[] arr1;
  delete[] arr2;
  arr1 = new_arr1;
  arr2 = new_arr2;
  arr1Size = 0;
  arr2Size = 0;
}

void printArr() {
  Serial.print("Array 1:");
  for(int i = 0; i < arr1Size; i++) {
    Serial.print(arr1[i], 4);
    Serial.print(' ');
  }
  Serial.println();
  Serial.print("Array 2:");
  for(int i = 0; i < arr2Size; i++) {
    Serial.print(arr2[i], 4);
    Serial.print(' ');
  }
  Serial.println();
}

void requestFromRPI() {
  String received;
  
  while(Serial1.available() > 0 | requesting){
    received = Serial1.readStringUntil(' ');
//    Serial.print("Received string: ");
//    Serial.println(received);
    
    if (received == "$") {currentArr = 2; dataReceived = false;}
    else if (received == "%") {currentArr = 1; dataReceived = true; requesting = false; printArr();}
    else if (received == "^") {resetArr(); currentArr = 1; Serial.println("Arrays cleared"); dataReceived = false;}
    float tmp = received.toFloat();

    if (tmp != 0 & (received != "$" | received != "%" | received != "^")) {
//      Serial.print("Received number: ");
//      Serial.println(tmp, 4);
      if (currentArr == 1) resizeArray1(tmp);
      else if (currentArr == 2) resizeArray2(tmp + 90.0);
//      printArr();
    }
  }
}

void navigation() {
  if (square & arr1Size > 0) {
    square = false;
    if (arr1[arrPointer] < 0) arrPointer = -1;
    for (int i = 0; i < arr1Size; i++) {
      if (arr1[i] > 0 & i > arrPointer) {
        arrPointer = i;
        break;
      }
    }
    setPos(0, arr2[arrPointer]);
    Serial.print("Moving to: ");
    Serial.println(arr2[arrPointer]);
    delay(100);
  }
  if (circle & arr1Size > 0) {
    circle = false;
    if (arr1[arrPointer] > 0) arrPointer = -1;
    for (int i = 0; i < arr1Size; i++) {
      if (arr1[i] < 0 & i > arrPointer) {
        arrPointer = i;
        break;
      }
    }
    setPos(0, arr2[arrPointer]);
    Serial.print("Moving to: ");
    Serial.println(arr2[arrPointer]);
    delay(100);
  }
}

void loop() {
  buttonPressed();
  if (triangle && !requesting) {
    triangle = false;
    requesting = true;
//    Serial.print("triangle: ");
//    Serial.println(triangle);
    Serial1.println("OMAE WA MOU SHINDEIRU");
    delay(100);
  }

  if (requesting) requestFromRPI();
  else navigation();
}
