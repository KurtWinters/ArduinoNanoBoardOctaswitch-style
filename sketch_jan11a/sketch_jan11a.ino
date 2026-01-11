const int buttonPin = 5;  // + баттон
const int downbuttonPin = 7;    // - баттон
int i = 0; //запись в 76
int check = 0; //защита от лишнего нажатия
int buttonState = 0;  // +1
int downstate = 0; //-1

//запись в 76
int clockPin = 5;
int dataPin = 7;
int latchPin = 6;
void setup() {

  pinMode(buttonPin, INPUT);
  pinMode(downPin, INPUT);
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

}

void loop() {
  buttonState = digitalRead(buttonPin);
  downstate = digitalRead(downPin);

   if (buttonState == HIGH && check == 0) {
    check++;
    if (i>255){i=0;}
    delay(100); i++; 
    Serial.println("+"); 

  } 
  if (downstate == HIGH && check == 0) {
    check++; 
    if (i<0){i=256;}
    Serial.println("-");  
    delay(100); i--;
  } 

  if (buttonState == LOW && downstate == LOW) //переделать, условие вызывает нестабильность
  {
    buttonState = digitalRead(buttonPin);
    downstate = digitalRead(downPin);
    check= 0; //приведение системы в 0
    
    //вывод
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin,LSBFIRST, i);
    digitalWrite(latchPin, HIGH);
    Serial.println(i); delay(100);
    
  } 
}