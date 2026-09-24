#include "morseIndex.h"
#include "Encoder.h"

const uint8_t totalEntries = sizeof(morseIndex) / sizeof(database);


const uint8_t eClk = 3;
const uint8_t eCt = 2;
const uint8_t select = 12;
const uint8_t ledG = 5;
const uint8_t ledR = 4;
const uint8_t morseIn = 7;
const uint8_t speed = A0;
const uint8_t buzz = 8;
const float sensitiv = 1.14;
const uint8_t maxInputMorseSize = 50;
const int defaultDelay = 100;
const bool defaultBeep = true;
const float dotBoundTuning = 2.5;
const float gapBoundTuning = 1.3;
Encoder selector(eClk, eCt);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A5));
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(morseIn, INPUT);
  pinMode(eClk, INPUT);
  pinMode(eCt, INPUT);
  pinMode(select, INPUT);
  buzzer("startup");
  delay(500);
  menu();
}

void loop() {
  
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

String morseRead() {
  const unsigned long debounceTime = 15;
  const unsigned long minPulseTime = 30;
  const unsigned long messageTimeout = 2000;
  const float onClusterRatio = 1.8;
  const float offClusterRatio = 1.8;
  unsigned long timeOn[maxInputMorseSize];
  unsigned long timeOff[maxInputMorseSize];
  uint8_t timeOnCount = 0;
  uint8_t timeOffCount = 0;
  bool userTimeOut = false;
  while (!digitalRead(morseIn)) {
    delay(1);
  }
  delay(debounceTime);
  while (!userTimeOut && timeOnCount < maxInputMorseSize) {
    unsigned long startTime = millis();
    tone(buzz, 1000);
    while (digitalRead(morseIn)) {
      delay(1);
    }
    noTone(buzz);
    unsigned long onDuration = millis() - startTime;
    if (onDuration < minPulseTime) {
      delay(debounceTime);
      continue;
    }
    timeOn[timeOnCount++] = onDuration;
    delay(debounceTime);
    startTime = millis();
    while (!digitalRead(morseIn)) {
      if (millis() - startTime >= messageTimeout) {
        userTimeOut = true;
        break;
      }
      delay(1);
    }
    if (userTimeOut) {
      break;
    }
    unsigned long offDuration = millis() - startTime;
    if (offDuration >= minPulseTime && timeOffCount < maxInputMorseSize) {
      timeOff[timeOffCount++] = offDuration;
    }
    delay(debounceTime);
  }
  if (timeOnCount == 0) {
    return "";
  }
  float dashThreshold = findTimingSplit(timeOn, timeOnCount, onClusterRatio);
  if (dashThreshold == 0) {
    unsigned long average = 0;
    for (uint8_t i = 0; i < timeOnCount; i++) {
      average += timeOn[i];
    }
    average /= timeOnCount;
    if (average < 300) {
      dashThreshold = 1000;
    }
    else {
      dashThreshold = 0;
    }
  }
  float letterGapThreshold = findTimingSplit(timeOff, timeOffCount, offClusterRatio);
  String message = "";
  bool checkedLetter[maxInputMorseSize];
  uint8_t lenOfLetter = 0;
  for (uint8_t i = 0; i < timeOnCount; i++) {
    checkedLetter[lenOfLetter] = timeOn[i] >= dashThreshold;
    lenOfLetter++;
    bool endOfLetter = i == timeOnCount - 1;
    if (!endOfLetter && i < timeOffCount && letterGapThreshold > 0) {
      if (timeOff[i] >= letterGapThreshold) {
        endOfLetter = true;
      }
    }
    if (endOfLetter) {
      bool found = false;
      for (uint8_t j = 0; j < totalEntries; j++) {
        if (morseIndex[j].len != lenOfLetter) {
          continue;
        }
        bool match = true;
        for (uint8_t k = 0; k < lenOfLetter; k++) {
          if (morseIndex[j].morse[k] != checkedLetter[k]) {
            match = false;
            break;
          }
        }
        if (match) {
          message += morseIndex[j].letter;
          found = true;
          break;
        }
      }
      if (!found) {
        message += "?";
      }
      lenOfLetter = 0;
    }
  }
  if (userTimeOut) {
    message += " ";
  }
  return message;
}

float findTimingSplit(unsigned long values[], uint8_t count, float minRatio) {
  if (count < 2) return 0;
  float low = values[0];
  float high = values[0];
  for (uint8_t i = 1; i < count; i++) {
    if (values[i] < low) low = values[i];
    if (values[i] > high) high = values[i];
  }
  if (low == high) return 0;
  for (uint8_t iteration = 0; iteration < 10; iteration++) {
    float threshold = (low + high) / 2.0;
    unsigned long lowTotal = 0;
    unsigned long highTotal = 0;
    uint8_t lowCount = 0;
    uint8_t highCount = 0;
    for (uint8_t i = 0; i < count; i++) {
      if (values[i] < threshold) {
        lowTotal += values[i];
        lowCount++;
      } else {
        highTotal += values[i];
        highCount++;
      }
    }
    if (lowCount == 0 || highCount == 0) return 0;
    low = (float)lowTotal / lowCount;
    high = (float)highTotal / highCount;
  }
  if ((high / low) < minRatio) return 0;
  return (low + high) / 2.0;
}

void pulse(bool length){
  lColour("orange");
  tone(buzz, 500);
  if (length){
    wait(3);
  }
  else{
    wait(1);
  }
  noTone(buzz);
  lColour("");
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

void simpleReadBack(){
  uint8_t size = 42;
  while (true){
    String qWord = String(morseIndex[random(0, totalEntries)].letter);
    bool correct = false;
    while (!correct){
      morseWrite(qWord);
      String answer = morseRead();
      answer.trim();
      if (answer == qWord){
        buzzer("correct");
        delay(3000);
        correct = true;
      }
      else{
        buzzer("incorrect");
        delay(3000);
      }
    }
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
      String answer = morseRead();
      answer.trim();
      if (answer == qWord){
        buzzer("correct");
        correct = true;
        delay(3000);
      }
      else{
        buzzer("incorrect");
        delay(3000);
      }
    }
  }
}

void pcReadBack(){
  int arraySize = 0;
  for (String c : quizWords){arraySize += 1;}
  while(true){
    String qWord = String(morseIndex[random(0, arraySize-1)].letter);
    bool correct = false;
    while (!correct){
      Serial.print("Encode this as morse: ");
      Serial.println(qWord);
      String answer = morseRead();
      answer.trim();
      if (answer == qWord){
        buzzer("correct");
        correct = true;
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
  uint8_t menuSize;
  bool confirm = false;
  menuSize = 5;
  unsigned long lastDebounceTime = 0;
  long selected = 1;
  bool enter = true;
  const unsigned long debounceDelay = 15;
  long oldPosition = -999;
  while (true){
    if ((millis() - lastDebounceTime) > debounceDelay) {
      long currentClick = selector.read() / 4;
      if (!digitalRead(select)){
        if (selected == 1){
          buzzer("select");
          delay(2000);
          testOutput();
        }
        else if (selected == 2){
          if (confirm){
            buzzer("select");
            delay(2000);
            testInput();
          }
          else{
            confirm = true;
            buzzer("PC");
          }
        }
        else if (selected == 3){
          buzzer("select");
          delay(2000);
          simpleReadBack();
        }
        else if (selected == 4){
          buzzer("select");
          delay(2000);
          morseReadBack();
        }
        else if (selected == 5){
          if (confirm){
            buzzer("select");
            delay(2000);
            pcReadBack();
          }
          else{
            confirm = true;
            buzzer("PC");
          }
        }
      }
      if (currentClick != oldPosition) {
        if (enter){
          selected = 1;
          enter = false;
        }
        else if (currentClick > oldPosition){
          selected += 1;
          confirm = false;
        }
        else if (currentClick < oldPosition){
          selected -= 1;
          confirm = false;
        }
        if (selected < 1){
          selected = menuSize;
        }
        else if (selected > menuSize){
          selected = 1;
        }
        oldPosition = currentClick;
        lastDebounceTime = millis();
        tone(buzz, selected*100, 250);
      }
    }
  }
}

void testInput(){
  while (true){
    Serial.println("Enter morse signal now");
    Serial.println(morseRead());
  }
}

void testOutput(){
  while (true){
    morseWrite("Testing");
  }
}

void buzzer(String type){
  if (type == "startup"){
    lColour("orange");
    tone(buzz, 392);
    delay(150);
    lColour("red");
    tone(buzz, 294);
    delay(150);
    lColour("orange");
    tone(buzz, 392);
    delay(150);
    lColour("green");
    tone(buzz, 587);
    delay(150);
    lColour("");
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
    lColour("green");
    tone(buzz, 523);
    delay(200);
    tone(buzz, 784);
    delay(200);
    tone(buzz, 1046);
    delay(200);
    lColour("");
  }
  else if (type == "incorrect"){
    lColour("red");
    tone(buzz, 587);
    delay(200);
    tone(buzz, 554);
    delay(200);
    tone(buzz, 523);
    delay(200);
    lColour("");
  }
  else if (type = "PC"){
    tone(buzz, 349);
    delay(150);
    tone(buzz, 698);
    delay(150);
    noTone(buzz);
    delay(150);
  }
  noTone(buzz);
}

void lColour(String colour) {
  if (colour == "green"){
    digitalWrite(ledG, HIGH);
    digitalWrite(ledR, LOW);
  }
  else if (colour == "red"){
    digitalWrite(ledG, LOW);
    digitalWrite(ledR, HIGH);
  }
  else if (colour == "orange"){
    digitalWrite(ledG, HIGH);
    digitalWrite(ledR, HIGH);
  }
  else{
    digitalWrite(ledG, LOW);
    digitalWrite(ledR, LOW);
  }
}