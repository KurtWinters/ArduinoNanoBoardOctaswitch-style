const int BankUpPin = 10;  // + баттон
const int BankDimPin = 2;    // - баттон
int i = 0; //запись в 76
int check = 0; //защита от лишнего нажатия
int BankUpState = 0;  // +1
int BankDownState = 0; //-1
int bank = 0;

//запись в 76
int clockPin = 5;
int dataPin = 7;
int latchPin = 6;
void setup() {

  pinMode(BankUpPin, INPUT);
  pinMode(BankDimPin, INPUT);
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

}

void doit(int a)
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin,LSBFIRST, i);
    digitalWrite(latchPin, HIGH);
    Serial.print("bank #"); Serial.print(bank); Serial.print(" value "); Serial.println(i); delay(100);
}

void loop() {
  BankUpState = digitalRead(BankUpPin);
  BankDownState = digitalRead(BankDimPin);

   if (BankUpState == HIGH && check == 0) {
    check++;
    if (bank>2){bank=-1;}
    delay(100); bank++; 
    Serial.println("+"); 

  } 
  if (BankDownState == HIGH && check == 0) {
    check++; 
    if (bank<1){bank=4;}
    Serial.println("-");  
    delay(100); bank--;
  } 

  if (BankUpState == LOW && BankDownState == LOW) //переделать, условие вызывает нестабильность
  {
    BankUpState = digitalRead(BankUpPin);
    BankDownState = digitalRead(BankDimPin);
    check = 0; //приведение системы в 0
    
    //вывод
 doit(i);
    
  } 
}