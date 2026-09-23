#include "morseIndex.h"
#include "Encoder.h"

const uint8_t totalEntries = sizeof(morseIndex) / sizeof(database);


const uint8_t eClk = 2;
const uint8_t eCt = 3;
const bool select = 12;
const uint8_t morseOut = 4;
const uint8_t morseIn = 7;
const uint8_t speed = A0;
const uint8_t buzz = 8;
const float sensitiv = 1.14;
const uint8_t maxInputMorseSize = 50;
const int defaultDelay = 100;
const bool defaultBeep = true;
const float dotBoundTuning = 2.3;
const float gapBoundTuning = 1.5;
Encoder selector(eClk, eCt);

void setup() {
  buzzer("startup");
  delay(3000);
  buzzer("select");
  delay(3000);
  buzzer("correct");
  delay(3000);
  buzzer("incorrect");
  Serial.begin(9600);
  pinMode(morseOut, OUTPUT);
  pinMode(morseIn, INPUT);
  pinMode(eClk, INPUT);
  pinMode(eCt, INPUT);
  pinMode(select, INPUT);
  menu();
}

void loop() {
  //Serial.println(morseRead());
  morseWrite("Hello");
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
    wait(3);
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
  tone(buzz, 1000);
  digitalWrite(morseOut, HIGH);
  if (length){
    wait(3);
  }
  else{
    wait(1);
  }
  noTone(buzz);
  digitalWrite(morseOut, LOW);
}

void wait(uint8_t step){
  if (defaultBeep){
    delay(defaultDelay*step);
  }
  else{
    int temp = analogRead(speed);
    temp = temp*step;
    temp = temp/100;
    delay(temp*100);
  }
}

void morseReadBack(){
  int arraySize = 0;
  for (String c : quizWords){arraySize += 1;}
  while(true){
    String qWord = quizWords[random(0, arraySize-1)];
    bool correct = false;
    while (!correct){
      morseWrite(qWord);
      if (morseRead() == qWord){
        buzzer("correct");
        delay(3000);
      }
      else{
        buzzer("incorrect");
        delay(3000);
      }
    }
  }
}

int menu(){
  unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 15;
  long oldPosition = -999;
  while (true){
    if ((millis() - lastDebounceTime) > debounceDelay) {
      long currentClick = selector.read() / 4; 
      if (currentClick != oldPosition) {
        oldPosition = currentClick;
        lastDebounceTime = millis();
        tone(buzz, currentClick*100, 250);
        Serial.println(currentClick);
      }
    }
  }
}

void test(){
  while (true){
    morseWrite("Testing");
  }
}

void buzzer(String type){
  if (type == "startup"){
    tone(buzz, 392);
    delay(150);
    tone(buzz, 294);
    delay(150);
    tone(buzz, 392);
    delay(150);
    tone(buzz, 587);
    delay(150);
  }
  else if (type == "select"){
    tone(buzz, 587);
    delay(150);
    tone(buzz, 349);
    delay(150);
    tone(buzz, 392);
    delay(150);
  }
  else if (type == "correct"){
    tone(buzz, 523);
    delay(200);
    tone(buzz, 784);
    delay(200);
    tone(buzz, 1046);
    delay(200);
  }
  else if (type == "incorrect"){
    tone(buzz, 587);
    delay(200);
    tone(buzz, 554);
    delay(200);
    tone(buzz, 523);
    delay(200);
  }
  noTone(buzz);
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