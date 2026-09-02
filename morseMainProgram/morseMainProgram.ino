bool eClk = digitalRead(3);
bool eCt = digitalRead(2);
bool morse = digitalRead(4);
String hello = "hi";

void setup() {
  morseWrite("SOS");
}

void loop() {
  
}

int morseWrite(String message) {
  Serial.begin(9600);
  
  Serial.print(morseI.r);
}

String morseRead(){

}

int pulse(int length){
  digitalWrite(morse, HIGH);
  delay(length);
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