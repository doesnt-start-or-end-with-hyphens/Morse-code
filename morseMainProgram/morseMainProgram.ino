#include "morseIndex.h"

bool eClk = digitalRead(3);
bool eCt = digitalRead(2);
bool morse = digitalRead(13);
String hello = "hi";
morseIndex morseI;

void setup() {
  morseWrite("SOS");
}

void loop() {
  
}

int morseWrite(String message) {
  Serial.begin(9600);
  for (char c : message){
    for (bool m : morseI.c){
      pulse(m);
    }
  }
}

String morseRead(){

}

void pulse(bool length){
  digitalWrite(morse, HIGH);
  if (length){
    delay(3);
  }
  else{
    delay(1);
  }
  digitalWrite(morse, LOW);
  delay(0.25);
}

int menu(){

}

void test(){
  while (true){

  }
}

int delay(){

}

int lColour(String colour) {
  if (colour == "green"){
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
  }
  else if (colour == "red"){
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
  }
  else if (colour == "orange"){
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
  }
  else{
    digitalWrite(7, LOW);
    digitalWrite(7, LOW);
  }
}