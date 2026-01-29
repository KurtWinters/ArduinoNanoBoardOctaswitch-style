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
int bin[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int out = 0;
int mCheck = 0;
int Setup_flag = 0;
int OutPlusState = 0;
int OutMinState = 0;
int OnState = 0;
int OffState = 0;
int ButtonExState = 0;
    

LiquidCrystal_I2C lcd(0x27,20,4);



void setup() {
  EEPROM.begin();

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

  for (int btn = 0; btn < 3; btn++) {
    for (int bnk = 0; bnk < 4; bnk++) {
      value[btn][bnk] = load_value(btn, bnk);
    }
  }

}

int stepen(int a, int b) {
    if (a == 0 || b == 0) { 
        return 1;
    }
    int result = 1;
    for (int i = 0; i < b; i++) {
        result *= a;
    }
    return result;
}

int bintodec (int jopa[8])
{
  int dec = 0;
for (int a = 0; a < 8; a++) {  

    dec = jopa[a]*stepen(2,a)+dec;
    
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
  lcd.print(" value ");  lcd.print(value[button_value][bank]);
}
//проверить 
//array[button_value][bank]





  int adress_count(int count_buts, int count_banks) {
    // Адрес = (button * NUM_BANKS + bank) * VALUE_SIZE
    return (count_buts * 4 + count_banks) * sizeof(int);
}

// Запись значения
void save_value(int button, int bank, int valueSave) {
    int address = adress_count(button, bank);
    EEPROM.put(address, valueSave);  // Записывает весь int (2 или 4 байта)
}

// Чтение значения
int load_value(int button, int bank) {
    int valueLoad = 0;
    int address = adress_count(button, bank);
    EEPROM.get(address, valueLoad);  // Читает весь int
    return valueLoad;
}

  void NormalMode()
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



/*
EEPROM.write(адрес, данные) - пишет один байт данных по адресу
EEPROM.update(адрес, данные) - обновляет (записывает, если отличается) байт данных по адресу. Не реализована для esp8266/32
EEPROM.read(адрес) - читает и возвращает байт данных по адресу
EEPROM.put(адрес, переменная) - записывает (по факту - обновляет, update) данные из переменной любого типа по адресу
EEPROM.get(адрес, переменная) - читает данные по адресу и сам записывает их в указанную переменную
EEPROM[] - библиотека позволяет работать с EEPROM памятью как с обычным массивом типа uint8_t
*/


void loop() 
{
  ButtonWriteState = digitalRead(ButtonWritePin);
  if (ButtonWriteState == LOW)
  {
    NormalMode();
  }
  
  if (ButtonWriteState == HIGH)
  {
    if (Setup_flag == 0)
    {
          lcd.clear(); 
    lcd.setCursor(1,1);             
    lcd.print("==================");
    lcd.setCursor(4,2); 
    lcd.print("Setup mode");
    lcd.setCursor(1,3); 
    lcd.print("=================="); delay(300);

    lcd.clear();  // ОЧИСТИТЬ экран перед выводом новой информации
    lcd.setCursor(1,0);  // Использовать все 4 строки LCD
    lcd.print("Bank:"); lcd.print(bank);
    lcd.print(" Btn:"); 
    switch (button_value)
    {
    case 0: lcd.print("A"); break;
    case 1: lcd.print("B"); break;
    case 2: lcd.print("C"); break;
    default: lcd.print("wrong!");
    }
    delay(300);
    lcd.setCursor(1,1);             
    lcd.print("==================");
    
    lcd.setCursor(2,2); 
    lcd.print("Output "); lcd.print(out); 
    lcd.print(": "); lcd.print(bin[out]);  // Теперь должно работать
    
    lcd.setCursor(1,3); 
    lcd.print("=================="); delay(300);
    Setup_flag = 1;
    }

    OutPlusState = digitalRead(BankUpPin);
    OutMinState = digitalRead(BankDimPin);
    OnState = digitalRead(ButtonAPin);
    OffState = digitalRead(ButtonBPin);
    ButtonExState = digitalRead(ButtonCPin);
    
    lcd.setCursor(0,1);
    lcd.print("output modes"); 
    for (int iPrint = 0; iPrint <8; iPrint++)
  {  
    lcd.print(bin[iPrint]); 
  }
    lcd.setCursor(2,2); 
    lcd.print("output "); lcd.print(out); lcd.print(" status "); lcd.print(bin[out]);
    lcd.setCursor(1,3); 
    lcd.print("=================="); delay(10);
    
    

       
   if (OutPlusState == HIGH && mCheck == 0 && Setup_flag == 1)
    {
        mCheck = 1;
        if (out >= 7) { out = 0; } else { out++; }
        delay(10);  // Задержка для дребезга
    }
    else if (OutMinState == HIGH && mCheck == 0 && Setup_flag == 1)
    {
        mCheck = 1;
        if (out <= 0) { out = 7; } else { out--; }
        delay(10);
    }
    else if (OnState == HIGH && mCheck == 0 && Setup_flag == 1)
    {
        mCheck = 1;
        bin[out] = 1;
        delay(10);
    }
    else if (OffState == HIGH && mCheck == 0 && Setup_flag == 1)
    {
        mCheck = 1;
        bin[out] = 0;
        delay(10);
    }
    
    // Сброс флага нажатия
    if (OutPlusState == LOW && OutMinState == LOW && 
        OnState == LOW && OffState == LOW && mCheck == 1 && Setup_flag == 1)
    {
        mCheck = 0;
    }
  }
  if (ButtonExState == HIGH && Setup_flag == 1 && ButtonWriteState == LOW)
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
    value[button_value][bank] = bintodec(bin);
    int valArr = value[button_value][bank];
    save_value(button_value, bank, valArr); 
    doit();
    Setup_flag = 0;
  
  }

}