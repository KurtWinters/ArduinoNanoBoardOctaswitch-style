const int BankUpPin = 2;  // + баттон
const int BankDimPin = 12;    // - баттон
int check = 0; //защита от лишнего нажатия
int BankUpState = 0;  // +1
int BankDownState = 0; //-1
int bank = 0;

int ButtonAPin = 10;
int ButtonBPin = 11;
int ButtonAState = 0;
int ButtonBState = 0;


//запись в 76
int clockPin = 5;
int dataPin = 7;
int latchPin = 6;
int i = 0; //записываемое число
void setup() {

  pinMode(BankUpPin, INPUT);
  pinMode(BankDimPin, INPUT);
  Serial.begin(9600);

  pinMode(ButtonAPin, INPUT);
  pinMode(ButtonBPin, INPUT);

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
  ButtonAState = digitalRead(ButtonAState);
  ButtonBState = digitalRead(ButtonBState);

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

  if (ButtonAState == HIGH && check == 0) {
    check++; 
    switch (bank)
    {
      case 0: i = 0; break;
      case 1: i = 1; break;
      case 2: i = 2; break;
      case 3: i = 3; break;
      default:  break;
    }
    Serial.println("A");  
    delay(1000); 
  }
  


//doit(i);
  if (BankUpState == LOW && BankDownState == LOW && ButtonAState == LOW) //переделать, условие вызывает нестабильность
  {
    BankUpState = digitalRead(BankUpPin);
    BankDownState = digitalRead(BankDimPin);
    ButtonAState = digitalRead(ButtonAPin);
    check = 0; //приведение системы в 0
    
    //вывод
 doit(i);
    
  } 
  else
  {Serial.print(BankDownState); Serial.print(BankUpState); Serial.println(ButtonAState);}
}