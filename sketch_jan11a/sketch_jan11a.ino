#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

//управление банками
const int BankUpPin = 2;  // + баттон
const int BankDimPin = 3;    // - баттон
int check = 0; //защита от лишнего нажатия
int BankUpState = 0;  // +1
int BankDownState = 0; //-1
int bank = 0;

//управление баттонами
int ButtonAPin = 4;
int ButtonBPin = 5;
int ButtonCPin = 6;
int ButtonAState = 0;
int ButtonBState = 0;
int ButtonCState = 0;
int check1 = 0;
int value[3][4]; 
int button_value = 0;

//запись в 76
int clockPin = 7;
int dataPin = 9;
int latchPin = 8;


//нажать для записи в память значения
int ButtonWritePin = 10;
int ButtonWriteState = 0;
int check2 = 0;

//меню
int bin[8] {};
int out = 0;
int mCheck = 0;
int tmp_flag = 0;
int OutPlusState = 0;
int OutMinState = 0;
int OnState = 0;
int OffState = 0;
    

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
  //value [0][1] = 29; value [0][2] = 17;value [0][3] = 19;
  //value [1][1] = 179; value [1][2] = 49;value [1][3] = 175;
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

void doit()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin,LSBFIRST, value [button_value][bank]);
  digitalWrite(latchPin, HIGH);
  lcd.clear();              
  lcd.setCursor(3,0);
  lcd.print("bank #"); lcd.print(bank);
  lcd.setCursor(2,1);
  lcd.print("value ");  lcd.print(value[button_value][bank]);
}
//проверить 
//array[button_value][bank]

int adress_count(int count_buts, int count_banks)
  {
   int adressToReturn = (count_buts*3+count_banks)*4;
   //пофиксить, байт = 8 бит
   
    return adressToReturn;
  }



/*
EEPROM.write(адрес, данные) - пишет один байт данных по адресу
EEPROM.update(адрес, данные) - обновляет (записывает, если отличается) байт данных по адресу. Не реализована для esp8266/32
EEPROM.read(адрес) - читает и возвращает байт данных по адресу
EEPROM.put(адрес, переменная) - записывает (по факту - обновляет, update) данные из переменной любого типа по адресу
EEPROM.get(адрес, переменная) - читает данные по адресу и сам записывает их в указанную переменную
EEPROM[] - библиотека позволяет работать с EEPROM памятью как с обычным массивом типа uint8_t
*/


void loop() {
  ButtonWriteState = digitalRead(ButtonWritePin);
  if (ButtonWriteState == LOW)
  {

   
  BankUpState = digitalRead(BankUpPin);
  BankDownState = digitalRead(BankDimPin);
  ButtonAState = digitalRead(ButtonAPin);
  ButtonBState = digitalRead(ButtonBPin);
  ButtonCState = digitalRead(ButtonCPin);
  

   if (BankUpState == HIGH && check == 0) {
    check++;
    if (bank>2){bank=-1;}
    delay(100); bank++; 
    lcd.clear(); 
    lcd.setCursor(3,0);             
     lcd.print("bank ");lcd.print(bank);
    Serial.println("bank - "); Serial.println(bank); 
    lcd.setCursor(2,1);
  lcd.print("value "); lcd.print(value[button_value][bank]);

  } 
  if (BankDownState == HIGH && check == 0) {
    check++; 
    if (bank<0){bank=3;} 
    lcd.clear();  
    lcd.setCursor(3,0);            
    lcd.print("bank "); lcd.print(bank); Serial.println(bank);
    delay(100); bank--;
    lcd.setCursor(2,1);
  lcd.print("value "); lcd.print(value[button_value][bank]);
  } 

  if (ButtonAState == HIGH && check1 == 0) {
    check1++; 
    button_value = 0;
        lcd.clear(); 
        lcd.setCursor(2,1);             
     lcd.print("A");
    Serial.println("A");  
    delay(100); 
  }
    if (ButtonBState == HIGH && check1 == 0) {
    check1++; 
    button_value = 1;
    Serial.println("B");  
    lcd.clear(); 
    lcd.setCursor(2,1);             
    lcd.print("B");
    delay(100); 
  }
    if (ButtonCState == HIGH && check1 == 0) {
    check1++; 
    button_value = 2;
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

    doit();
    check1 = 0; //приведение системы в 0
  } 
  }
  
  if (ButtonWriteState == HIGH && check2 == 0)
  {
    if (tmp_flag == 0) 
    {     
      tmp_flag = 1;  
    lcd.clear(); 
    lcd.setCursor(1,1);             
    lcd.print("==================");
    lcd.setCursor(4,2); 
    lcd.print("Setup mode");
    lcd.setCursor(1,3); 
    lcd.print("=================="); delay(300);

    lcd.clear(); 
    lcd.setCursor(1,1);             
    lcd.print("==================");
    lcd.setCursor(2,2); 
    lcd.print("bank "); lcd.print(bank); lcd.print(" button "); lcd.print(button_value);
    lcd.setCursor(1,3); 
    lcd.print("=================="); delay(300);
    }

    OutPlusState = digitalRead(BankUpPin);
    OutMinState = digitalRead(BankDimPin);
    OnState = digitalRead(ButtonAPin);
    OffState = digitalRead(ButtonBPin);
    
    lcd.print("==================");
    lcd.setCursor(2,2); 
    lcd.print("output "); lcd.print(bin[out]); lcd.print(" status "); lcd.print(bin[out]);
    lcd.setCursor(1,3); 
    lcd.print("=================="); delay(500);

    if (OutPlusState == HIGH && mCheck == 0)
    {
      mCheck++; if (out>8){out=-1;} out++;
    }
   if (OutMinState == HIGH && mCheck == 0)
    {
      mCheck++; if (out<0){bank=8;} out--;
    }
   if (OnState == HIGH && mCheck == 0)
    {
      mCheck++; bin[out]=1;
    }
   if (OffState == HIGH && mCheck == 0)
    {
      mCheck++; bin[out]=0;
    }

if(BankUpState == LOW && BankDownState == LOW && ButtonAState == LOW && ButtonBState == LOW && mCheck == 1)
{mCheck = 1;}

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
    tmp_flag = 0;
    value[button_value][bank] = bintodec(bin[out]);
    EEPROM.update(adress_count(button_value, bank), value[button_value][bank]);
    doit();
  }

 // else {Serial.print(BankDownState); Serial.print(BankUpState); Serial.println(ButtonAState);}

 /*eeprom
 #include<EEPROM.h>
int six = 6;
int five = 5;
int four = 4;
int check = 0;

int sixstate = 0;
int fivestate = 0;
int fourstate = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(six, INPUT);
  pinMode(five, INPUT);
  pinMode(four, INPUT);
  Serial.begin(9600);

}

void loop() {
  Serial.println(EEPROM[500]);
  sixstate = digitalRead(six);
  fivestate = digitalRead(five);
  fourstate = digitalRead(four);
  if (sixstate == HIGH && check==0)
  {
    check++;
EEPROM.update(500, 6);
  }
  if (fivestate == HIGH && check==0)
  {
    check++;
EEPROM.update(500, 5);
  }
  if (fourstate == HIGH && check==0)
  {
    check++;
EEPROM.update(500, 4);
  }
  if (fourstate == LOW && fivestate == 0 && sixstate == 0 && check == 1)
  {check = 0;}
delay(1500);
}
*/
}
