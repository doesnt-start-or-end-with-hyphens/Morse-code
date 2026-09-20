#include "morseIndex.h"

const uint8_t totalEntries = sizeof(morseIndex) / sizeof(database);


bool eClk = digitalRead(3);
bool eCt = digitalRead(2);
const uint8_t morse = 13;
const uint8_t speed = A0;
const float sensitiv = 1.14;

void setup() {
  Serial.begin(9600);
  pinMode(morse, OUTPUT);
}

void loop() {
  morseWrite("Hello!");
  wait(7);
}

void morseWrite(String message) {
  message.toLowerCase();

  for (char c : message){
    bool found = false;
    for (uint8_t i = 0; i < totalEntries; i++){
      if (morseIndex[i].letter == c){
        for (uint8_t j = 0; j< morseIndex[i].len; j++){
          pulse(morseIndex[i].morse[j]);
          wait(3);
        }
        found = true;
        break;
      }
    }
    if (not found){
      Serial.println("Char not in database, skipping...");
    }
  }
}

String morseRead(){

}

void pulse(bool length){
  digitalWrite(morse, HIGH);
  if (length){
    wait(3);
  }
  else{
    wait(1);
  }
  digitalWrite(morse, LOW);
}

void wait(uint8_t step){
  int temp = 1023-analogRead(speed)/sensitiv;
  temp = temp*step;
  temp = temp/100;
  delay(temp*100);
}

int menu(){

}

void test(){
  while (true){

  }
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