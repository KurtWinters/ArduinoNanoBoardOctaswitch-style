#include<EEPROM.h>
#include <LiquidCrystal_I2C.h>

const int BankUpPin = 2;  // + баттон
const int BankDimPin = 3;    // - баттон
int check = 0; //защита от лишнего нажатия
int BankUpState = 0;  // +1
int BankDownState = 0; //-1
int bank = 0;

int ButtonAPin = 4;
int ButtonBPin = 5;
int ButtonCPin = 6;
int ButtonAState = 0;
int ButtonBState = 0;
int ButtonCState = 0;
int check1 = 0;

//запись в 76
int clockPin = 7;
int dataPin = 9;
int latchPin = 8;
int i = 255; //записываемое число

int ButtonWritePin = 10;
int ButtonWriteState = 0;
int check2 = 0;

LiquidCrystal_I2C lcd(0x27,20,4);



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

  pinMode(ButtonWritePin, INPUT);

  lcd.init(); 
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Choose bank");
  lcd.setCursor(3,2);
  lcd.print("and program");
  delay(1000);
}

void doit(int a)
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin,LSBFIRST, i);
    digitalWrite(latchPin, HIGH);
  lcd.clear();              
  lcd.setCursor(3,0);
  lcd.print("bank #"); lcd.print(bank);
  lcd.setCursor(2,1);
  lcd.print("value "); lcd.print(i);
  
}

void loop() {

   
  BankUpState = digitalRead(BankUpPin);
  BankDownState = digitalRead(BankDimPin);
  ButtonAState = digitalRead(ButtonAPin);
  ButtonBState = digitalRead(ButtonBPin);
  ButtonCState = digitalRead(ButtonCPin);
  ButtonWriteState = digitalRead(ButtonWritePin);
 /* */

   if (BankUpState == HIGH && check == 0) {
    check++;
    if (bank>2){bank=-1;}
    delay(100); bank++; 
    lcd.clear(); 
    lcd.setCursor(3,0);             
     lcd.print("bank ");lcd.print(bank);
    Serial.println("bank - "); Serial.println(bank); 
    lcd.setCursor(2,1);
  lcd.print("value "); lcd.print(i);

  } 
  if (BankDownState == HIGH && check == 0) {
    check++; 
    if (bank<0){bank=3;}
    lcd.clear();  
    lcd.setCursor(3,0);            
    lcd.print("bank "); lcd.print(bank); Serial.println(bank);
    delay(100); bank--;
    lcd.setCursor(2,1);
  lcd.print("value "); lcd.print(i);
  } 

  if (ButtonAState == HIGH && check1 == 0) {
    check1++; 
    switch (bank)
    {
      case 0: i = 29; break;
      case 1: i = 179; break;
      case 2: i = 1; break;
      case 3: i = 1; break;
      default:  break;
    }
        lcd.clear(); 
        lcd.setCursor(2,1);             
     lcd.print("A");
    Serial.println("A");  
    delay(100); 
  }
    if (ButtonBState == HIGH && check1 == 0) {
    check1++; 
    switch (bank)
    {
      case 0: i = 17; break;
      case 1: i = 49; break;
      case 2: i = 1; break;
      case 3: i = 1; break;
      default:  break;
    }
    Serial.println("B");  
    lcd.clear(); 
    lcd.setCursor(2,1);             
    lcd.print("B");
    delay(100); 
  }
    if (ButtonCState == HIGH && check1 == 0) {
    check1++; 
    switch (bank)
    {
      case 0: i = 19; break;
      case 1: i = 175; break;
      case 2: i = 1; break;
      case 3: i = 1; break;
      default:  break;
    }
    Serial.println("C");
    lcd.clear(); 
    lcd.setCursor(2,1);             
    lcd.print("C");  
    delay(100); 
  }
  

//Serial.print(ButtonAState); delay(500);
//doit(i);
  if (BankUpState == LOW && BankDownState == LOW && check == 1) //переделать, условие вызывает нестабильность 
  {
    check = 0; //приведение системы в 0
  } 
    if (ButtonAState == LOW && ButtonBState == LOW && ButtonCState == LOW && check1 == 1) //переделать, условие вызывает нестабильность && ButtonAState == LOW
  {
    doit(i);
    check1 = 0; //приведение системы в 0
  } 
  if (ButtonWriteState == HIGH && check2 == 0)
  {
    Serial.print("========"); Serial.print("Setup mode"); Serial.println("========"); check2 = 1;

        lcd.clear(); 
    lcd.setCursor(1,1);             
    lcd.print("==================");
    lcd.setCursor(4,2); 
    lcd.print("Setup mode");
    lcd.setCursor(1,3); 
    lcd.print("==================");
  }
  if (ButtonWriteState == LOW && check2 == 1)
  {
    Serial.print("=================="); Serial.print("Setup done"); Serial.println("=================="); check2 = 0;
            lcd.clear(); 
    lcd.setCursor(1,1);             
    lcd.print("==================");
    lcd.setCursor(4,2); 
    lcd.print("Setup done");
    lcd.setCursor(1,3); 
    lcd.print("==================");
    delay(1000);
    doit(i);
  }

 // else {Serial.print(BankDownState); Serial.print(BankUpState); Serial.println(ButtonAState);}
}