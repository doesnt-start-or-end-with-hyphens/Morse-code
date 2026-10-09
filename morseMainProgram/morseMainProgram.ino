#include "morseIndex.h"
#include "Encoder.h"

const uint8_t totalEntries = sizeof(morseIndex) / sizeof(database);

//sets up pin locations
const uint8_t eClk = 2;
const uint8_t eCt = 3;
const uint8_t select = 12;
const uint8_t ledG = 5;
const uint8_t ledR = 4;
const uint8_t morseIn = 7;
const uint8_t speed = A0;
const uint8_t buzz = 8;
//sensitivity for speed tuning
const float sensitiv = 1.14;
//the size of the array has to be defined
const uint8_t maxInputMorseSize = 500;
//defult speed as well as enabling that function, used if not potentiometer
const int defaultDelay = 100;
const bool defaultBeep = false;
//tuning for morse reading
const float dotBoundTuning = 2.5;
const float gapBoundTuning = 1.3;
//sets up other things
Encoder selector(eClk, eCt);
bool pcAccess;
bool writeSpeedDisplay;
void (*resetFunc) (void) = 0;

void setup() {
  writeSpeedDisplay = false;
  //checks for interface connection
  pcAccess = pcConnect();
  //gets random seed from stray values from n unused analog pin
  randomSeed(analogRead(A5));
  //sets I/O
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(morseIn, INPUT);
  pinMode(eClk, INPUT);
  pinMode(eCt, INPUT);
  pinMode(select, INPUT);
  buzzer("startup");
  if (pcAccess){buzzer("PC");}
  delay(500);
  menu();
}

void loop() {
  //loop is never used here
}

//checks connection to interface
bool pcConnect(){
  Serial.begin(9600);
  bool access = false;
  for (int i = 0; i < 10; i++){
    Serial.println(0);
    //checks if the interface is trying to send data
    if (Serial.available() > 0){
      access = true;
      break;
    }
    delay(200);
  }
  return access;
}

//writes morse from the proveded string
void morseWrite(String message) {
  message.toLowerCase();

  //for each char in the message
  for (char c : message){
    bool found = false;
    //if c is a space, wait 7 steps
    if (c == " "){
      wait(7);
    }
    else{
      for (uint8_t i = 0; i < totalEntries; i++){
        //finds the letter in database
        if (morseIndex[i].letter == c){
          for (uint8_t j = 0; j < morseIndex[i].len; j++){
            //pulses according to the internal bool values
            pulse(morseIndex[i].morse[j]);
            wait(3);
          }
          found = true;
          break;
        }
      }
    }
    //if a charicter is not in the database
    if (not found){
      Serial.println("Char not in database, skipping...");
    }
    wait(3);
  }
}

//listens to user inputs and converts to string
String morseRead() {
  //tuning values
  const unsigned long debounceTime = 15;
  const unsigned long minPulseTime = 30;
  const unsigned long messageTimeout = 2000;
  const float onClusterRatio = 1.8;
  const float offClusterRatio = 1.8;
  //creates the on and off time banks
  unsigned long timeOn[maxInputMorseSize];
  unsigned long timeOff[maxInputMorseSize];
  uint8_t timeOnCount = 0;
  uint8_t timeOffCount = 0;
  bool userTimeOut = false;
  //waits until the first on signal starts
  while (!digitalRead(morseIn)) {
    delay(1);
  }
  //protects aginst ouncy switches
  delay(debounceTime);
  //runs until time between inputs is too long
  while (!userTimeOut && timeOnCount < maxInputMorseSize) {
    //time is relitive to the systems run time
    unsigned long startTime = millis();
    tone(buzz, 1000);
    while (digitalRead(morseIn)) {
      delay(1);
    }
    noTone(buzz);
    //ends that time tracking
    unsigned long onDuration = millis() - startTime;
    //ignores if suspected bouncy switch
    if (onDuration < minPulseTime) {
      delay(debounceTime);
      continue;
    }
    //adds to time on ba
    timeOn[timeOnCount++] = onDuration;
    delay(debounceTime);
    //same process for gaps (off time)
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
  //skips calculations if the user inputs nothing
  if (timeOnCount == 0) {
    return "";
  }
  //finds the dash length
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
  //same thing but for gaps
  float letterGapThreshold = findTimingSplit(timeOff, timeOffCount, offClusterRatio);
  //preps the message for construction
  String message = "";
  bool checkedLetter[maxInputMorseSize];
  uint8_t lenOfLetter = 0;
  //adds all letters
  for (uint8_t i = 0; i < timeOnCount; i++) {
    checkedLetter[lenOfLetter] = timeOn[i] >= dashThreshold;
    lenOfLetter++;
    bool endOfLetter = i == timeOnCount - 1;
    if (!endOfLetter && i < timeOffCount && letterGapThreshold > 0) {
      //terminate current letter if gap long enough
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

//finds the split in gap timings
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

//runs a morse pulse length dependant on input bool
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

//the delay steps dependant on speed*steps
void wait(uint8_t step){
  //if the defult step is set true
  if (defaultBeep){
    if (writeSpeedDisplay){
      Serial.print(3);
      Serial.println(defaultBeep);
    }
    delay(defaultDelay*step);
  }
  else{
    int temp = analogRead(speed);
    temp = temp*step;
    temp = temp/100;
    if (writeSpeedDisplay){
      Serial.print(3);
      Serial.println(defaultBeep);
    }
    delay(temp*50);
  }
}

//first gamemode
void simpleReadBack(){
  uint8_t size = 42;
  //runs forever
  while (true){
    //picks letter
    String qWord = String(morseIndex[random(0, totalEntries)].letter);
    bool correct = false;
    //repeats until user gets right
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

//seconds gamemode
void morseReadBack(){
  int arraySize = 0;
  for (String c : quizWords){arraySize += 1;}
  while(true){
    //same as before but whole words
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

//third gamemode
void pcReadBack(){
  int arraySize = 0;
  for (String c : quizWords){arraySize += 1;}
  while(true){
    String qWord = String(morseIndex[random(0, arraySize-1)].letter);
    bool correct = false;
    while (!correct){
      //sends graphical data to interface
      Serial.print("10");
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

//fourth gamemode
void pcWriteBack(){
  int arraySize = 0;
  for (String c : quizWords){arraySize += 1;}
  while(true){
    String qWord = String(morseIndex[random(0, arraySize-1)].letter);
    bool correct = false;
    while (!correct){
      morseWrite(qWord);
      delay(500);
      Serial.println("14");
      delay(500);
      //clears the buffer
      while (Serial.available() > 0) {
        Serial.read(); 
      }
      //waits for interface to return the users answer
      while(Serial.available() == 0);
      if(Serial.readStringUntil("\n") == qWord){
        Serial.println("5");
        buzzer("correct");
        correct = true;
        delay(3000);
      }
      else{
        Serial.println("6");
        buzzer("incorrect");
        delay(3000);
      }
    }
  }
}

//main menu
int menu(){
  Serial.println("1");
  delay(200);
  Serial.println("2");
  uint8_t menuSize;
  //standalone only allows the first 3 options
  if (pcAccess){
    menuSize = 7;
  }
  else{
    menuSize = 3;
  }
  unsigned long lastDebounceTime = 0;
  long selected = 1;
  bool enter = true;
  const unsigned long debounceDelay = 15;
  long oldPosition = -999;
  while (true){
    if ((millis() - lastDebounceTime) > debounceDelay) {
      long currentClick = selector.read() / 4;
      //encoder button is inverted in signal
      if (!digitalRead(select)){
        //executes the mode
        if (selected == 1){
          writeSpeedDisplay = true;
          Serial.println("4");
          buzzer("select");
          delay(2000);
          testOutput();
        }
        else if (selected == 2){
          Serial.println("7");
          buzzer("select");
          delay(2000);
          simpleReadBack();
        }
        else if (selected == 3){
          Serial.println("8");
          buzzer("select");
          delay(2000);
          morseReadBack();
        }
        else if (selected == 4){
          Serial.println("11");
          buzzer("select");
          delay(2000);
          testInput();
        }
        else if (selected == 5){
          Serial.println("9");
          buzzer("select");
          delay(2000);
          pcReadBack();
        }
        else if (selected == 6){
          Serial.println("13");
          buzzer("select");
          delay(2000);
          pcWriteBack();
        }
        else if (selected == 7){
          Serial.println("15");
          delay(3000);
          resetFunc();
        }
      }
      //selects baised on encoder turns
      if (currentClick != oldPosition) {
        if (enter){
          selected = 1;
          enter = false;
        }
        else if (currentClick > oldPosition){
          selected += 1;
        }
        else if (currentClick < oldPosition){
          selected -= 1;
        }
        if (selected < 1){
          selected = menuSize;
        }
        else if (selected > menuSize){
          selected = 1;
        }
        oldPosition = currentClick;
        lastDebounceTime = millis();
        //plays tone baised on selected option
        tone(buzz, selected*100, 250);
        Serial.print(2);
        Serial.println(selected);
      }
    }
  }
}

//tests thw users input
void testInput(){
  while (true){
    Serial.println("Enter morse signal now");
    String detected = morseRead();
    Serial.print("12");
    Serial.println(detected);
  }
}

//used to addjust the writing speed
void testOutput(){
  while (true){
    morseWrite("Testing");
  }
}

//stores all the buzzzer sequences
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
    Serial.println("5");
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
    Serial.println("6");
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
    tone(buzz, 392);
    delay(150);
    noTone(buzz);
    delay(150);
  }
  noTone(buzz);
}

//sets the tricolor led
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