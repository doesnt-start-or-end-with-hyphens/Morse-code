#include "morseIndex.h"

const uint8_t totalEntries = sizeof(morseIndex) / sizeof(database);


bool eClk = digitalRead(3);
bool eCt = digitalRead(2);
const uint8_t morseOut = 13;
const uint8_t morseIn = 2;
const uint8_t speed = A0;
const float sensitiv = 1.14;
const uint8_t maxInputMorseSize = 50;
const int defaultDelay = 100;
const bool defaultBeep = true;
const float dotBoundTuning = 2.0;
const float gapBoundTuning = 2.0;

void setup() {
  Serial.begin(9600);
  pinMode(morseOut, OUTPUT);
  pinMode(morseIn, INPUT);
  Serial.println(morseRead());
}

void loop() {
  morseWrite("Hello!");
  wait(7);
}

void morseWrite(String message) {
  message.toLowerCase();

  for (char c : message){
    bool found = false;
    if (c == " "){
      wait(7);
    }
    else{
      for (uint8_t i = 0; i < totalEntries; i++){
        if (morseIndex[i].letter == c){
          for (uint8_t j = 0; j < morseIndex[i].len; j++){
            pulse(morseIndex[i].morse[j]);
            wait(3);
          }
          found = true;
          break;
        }
      }
    }
    if (not found){
      Serial.println("Char not in database, skipping...");
    }
  }
}

String morseRead(){
  Serial.println("Init morseRead");
  int timeOn[maxInputMorseSize];
  uint8_t timeOnIndexSize = 0;
  int timeOff[maxInputMorseSize];
  uint8_t timeOffIndexSize = 0;
  bool userTimeOut = false;
  uint8_t morseOff[maxInputMorseSize];
  for (uint8_t idx = 0; idx < maxInputMorseSize; idx++) {
    morseOff[idx] = 1; 
  }
  while (!digitalRead(morseIn));
  while (!userTimeOut){
    unsigned long timeStart = millis();
    while (digitalRead(morseIn));
    unsigned long durationOn = millis() - timeStart;
    if (durationOn > 20) {
      timeOn[timeOnIndexSize] = durationOn;
      timeOnIndexSize += 1;
    } else {
      continue; 
    }
    timeStart = millis();
    while (!digitalRead(morseIn)) {
      if ((millis() - timeStart) >= 2000) {
        userTimeOut = true;
        break;
      }
    }
    if (!userTimeOut){
      unsigned long durationOff = millis() - timeStart;
      if (durationOff > 20) {
        timeOff[timeOffIndexSize] = durationOff;
        timeOffIndexSize += 1;
      }
    }
  }
  int minOn = timeOn[0];
  int maxOn = timeOn[0];
  for (uint8_t i = 1; i < timeOnIndexSize; i++) {
    if (timeOn[i] < minOn) minOn = timeOn[i];
    if (timeOn[i] > maxOn) maxOn = timeOn[i];
  }
  float upperDotBound = (minOn + maxOn) / dotBoundTuning;
  int minOff = timeOff[0];
  int maxOff = timeOff[0];
  for (uint8_t i = 1; i < timeOffIndexSize; i++) {
    if (timeOff[i] < minOff) minOff = timeOff[i];
    if (timeOff[i] > maxOff) maxOff = timeOff[i];
  }
  float upperSmallGapBound = (minOff + maxOff) / gapBoundTuning;
  bool morseOn[maxInputMorseSize];
  for (uint8_t i = 0; i < timeOnIndexSize; i++){
    morseOn[i] = (timeOn[i] >= upperDotBound);
  }
  for (uint8_t i = 0; i < timeOffIndexSize; i++){
    if (timeOff[i] <= upperSmallGapBound){
      morseOff[i] = 0; 
    }
    else {
      morseOff[i] = 1; 
    }
  }
  if (timeOnIndexSize > 0) {
    morseOff[timeOnIndexSize - 1] = 1;
  }
  String message = "";
  uint8_t letterStart = 0;
  uint8_t lenOfLetter = 0;
  bool checkedLetter[maxInputMorseSize];
  for (uint8_t i = 0; i < timeOnIndexSize; i++) {
    checkedLetter[lenOfLetter] = morseOn[i];
    lenOfLetter++;
    if (morseOff[i] >= 1) {
      for (uint8_t j = 0; j < totalEntries; j++){
        if (morseIndex[j].len == lenOfLetter){
          bool match = true;
          for (uint8_t k = 0; k < lenOfLetter; k++){
            if (morseIndex[j].morse[k] != checkedLetter[k]){
              match = false;
              break;
            }
          }
          if (match){
            message += morseIndex[j].letter;
            break;
          }
        }
      }
      letterStart += lenOfLetter; 
      lenOfLetter = 0;
    }
  }
  if (userTimeOut) {
    message += " ";
  }
  return message;
}




void pulse(bool length){
  digitalWrite(morseOut, HIGH);
  if (length){
    wait(3);
  }
  else{
    wait(1);
  }
  digitalWrite(morseOut, LOW);
}

void wait(uint8_t step){
  if (defaultBeep){
    delay(defaultDelay);
  }
  else{
    int temp = 1023-analogRead(speed)/sensitiv;
    temp = temp*step;
    temp = temp/100;
    delay(temp*100);
  }
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