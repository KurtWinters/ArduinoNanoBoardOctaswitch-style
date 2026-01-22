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
int value[3][4]; int button_value = 0;

int ButtonWritePin = 10;
int ButtonWriteState = 0;
int check2 = 0;

//меню
int mas[8] {};
int mCheck = 0;
int result = 0;

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
  value [0][1] = 29; value [0][2] = 17;value [0][3] = 19;
  value [1][1] = 179; value [1][2] = 49;value [1][3] = 175;
}

int stepen (int a, int b) //костыль для возведения в степень
{
  //Serial.print("ses1");
int c=a;
if (a == 0 or b == 0) {a=1;}
else {
for (b>0;b=b-1;)
{
a=c*a;
}
}
return a;
}

int bintodec (int jopa[8])
{
  int dec = 0;
for (int a = 0; a < 8; a++) {  

    dec = jopa[a]*stepen(2,a)+dec;
    delay(100);
}
return dec;
}

void doit(int massive[3][4])
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin,LSBFIRST, value [bank][button_value]);
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
    button_value = 1;
        lcd.clear(); 
        lcd.setCursor(2,1);             
     lcd.print("A");
    Serial.println("A");  
    delay(100); 
  }
    if (ButtonBState == HIGH && check1 == 0) {
    check1++; 
    button_value = 2;
    Serial.println("B");  
    lcd.clear(); 
    lcd.setCursor(2,1);             
    lcd.print("B");
    delay(100); 
  }
    if (ButtonCState == HIGH && check1 == 0) {
    check1++; 
    button_value = 3;
    Serial.println("C");
    lcd.clear(); 
    lcd.setCursor(2,1);             
    lcd.print("C");  
    delay(100); 
  }
  if (BankUpState == LOW && BankDownState == LOW && check == 1) //переделать, условие вызывает нестабильность 
  {
    check = 0; //приведение системы в 0
  } 
    if (ButtonAState == LOW && ButtonBState == LOW && ButtonCState == LOW && check1 == 1) //переделать, условие вызывает нестабильность && ButtonAState == LOW
  {

    doit(value [bank][button_value]);
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