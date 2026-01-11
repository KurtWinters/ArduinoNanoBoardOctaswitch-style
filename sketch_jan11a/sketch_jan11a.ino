const int BankUpPin = 10;  // + баттон
const int BankDimPin = 2;    // - баттон
int check = 0; //защита от лишнего нажатия
int BankUpState = 0;  // +1
int BankDownState = 0; //-1
int bank = 0;

int ButtonAPin = 8;
int ButtonBPin = 11;
int ButtonCPin = 12;
int ButtonAState = 0;
int ButtonBState = 0;
int ButtonCState = 0;
int check1 = 0;


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
  pinMode(ButtonCPin, INPUT);

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
  ButtonAState = digitalRead(ButtonAPin);
  ButtonBState = digitalRead(ButtonBPin);
  ButtonCState = digitalRead(ButtonCPin);
  

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

  if (ButtonAState == HIGH && check1 == 0) {
    check1++; 
    switch (bank)
    {
      case 0: i = 0; break;
      case 1: i = 1; break;
      case 2: i = 2; break;
      case 3: i = 3; break;
      default:  break;
    }
    Serial.println("A");  
    delay(100); 
  }
    if (ButtonBState == HIGH && check1 == 0) {
    check1++; 
    switch (bank)
    {
      case 0: i = 0; break;
      case 1: i = 1; break;
      case 2: i = 2; break;
      case 3: i = 3; break;
      default:  break;
    }
    Serial.println("A");  
    delay(100); 
  }
    if (ButtonCState == HIGH && check1 == 0) {
    check1++; 
    switch (bank)
    {
      case 0: i = 0; break;
      case 1: i = 1; break;
      case 2: i = 2; break;
      case 3: i = 3; break;
      default:  break;
    }
    Serial.println("A");  
    delay(100); 
  }
  

//Serial.print(ButtonAState); delay(500);
//doit(i);
  if (BankUpState == LOW && BankDownState == LOW ) //переделать, условие вызывает нестабильность 
  {
    check = 0; //приведение системы в 0
    
    //вывод
 //doit(i);
    
  } 
    if (ButtonAState == LOW) //переделать, условие вызывает нестабильность && ButtonAState == LOW
  {
    check1 = 0; //приведение системы в 0
    
    //вывод
 doit(i);
    
  } 
 // else {Serial.print(BankDownState); Serial.print(BankUpState); Serial.println(ButtonAState);}
}