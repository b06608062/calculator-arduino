// keyboad
#define AnaPin A0
int value;
String operand_and_operator[4] = {"", "", "", ""};
String temp = "", sign = "";
bool is_calculating = false;
long a = 0L, b = 0L;

// LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  MY_LCD_init();
}

void loop() {
  value = analogRead(AnaPin);
  if(value > 200) {
    
    for(int i = 1;i < 10;i++) {
      value += analogRead(AnaPin);
    }
    value /= 10;
    
    if(is_calculating) {
      is_calculating = false;
      for(int i = 0; i < 4;i++) {
        operand_and_operator[i] = "";
      }  
      a = 0L, b = 0L;
      lcd.cursor();
      lcd.blink();  
    }
    
    Serial.println(value);
    Data_Update();
    LCD_Update(); 
    delay(300);
  }
  value = 0;
}

void MY_LCD_init() {
  // LCD
  lcd.init();  // 初始化 LCD 
  lcd.backlight();  // 開啟背光
  lcd.setCursor(2, 0);
  lcd.print("Welcome to!!!");
  lcd.setCursor(0, 1);
  lcd.print("Jim's Calculator");
  lcd.cursor();  // 顯示游標 
  lcd.blink();   
}

void LCD_Update() {
  if(!is_calculating) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(operand_and_operator[0]);
    lcd.print(operand_and_operator[1]);
    lcd.print(operand_and_operator[2]); 
  }
}

void Data_Update() {
   Serial.print("使用者輸入: ");
   
  // 按鈕1
  if(value > 650) {
    Serial.println("1");
    temp = "1";  
  }
  // 按鈕2
  else if(value > 610) {
    Serial.println("2");
    temp = "2";    
  }
  // 按鈕3
  else if(value > 570) {
    Serial.println("3");
    temp = "3";      
  }
  // 按鈕A
  else if(value > 530) {
    Serial.println("+");
    sign = "+";
    Operand_Update();      
  }
  // 按鈕4  
  else if(value > 490) {
    Serial.println("4");
    temp = "4";      
  }
  // 按鈕5
  else if(value > 460) {
    Serial.println("5");
    temp = "5";      
  }
  // 按鈕6
  else if(value > 440) {
    Serial.println("6");
    temp = "6";      
  }
  // 按鈕B
  else if(value > 420) {
    Serial.println("-");
    sign = "-";
    Operand_Update();       
  }
  // 按鈕7
  else if(value > 390) {
    Serial.println("7");
    temp = "7";      
  }
  // 按鈕8
  else if(value > 380) {
    Serial.println("8");
    temp = "8";      
  }
  // 按鈕9
  else if(value > 360) {
    Serial.println("9");
    temp = "9";      
  }
  // 按鈕C
  else if(value > 340) {
    Serial.println("*");
    sign = "*";
    Operand_Update();      
  }
  // 按鈕*
  else if(value > 328) {
    Serial.println("del");
    if(operand_and_operator[2].length() > 0) {
      operand_and_operator[2] = operand_and_operator[2].substring(0, operand_and_operator[2].length() - 1);   
    }
    else if(operand_and_operator[1].length() == 1) {
      operand_and_operator[1] = "";   
    }
    else if(operand_and_operator[0].length() > 0)  {
      operand_and_operator[0] = operand_and_operator[0].substring(0, operand_and_operator[0].length() - 1);
    } 
  }
  // 按鈕0
  else if(value > 315) {   
    Serial.println("0");
    if(operand_and_operator[0].length() > 0 && operand_and_operator[1].length() == 0) {
      temp = "0";
    }
   else if(operand_and_operator[1].length() == 1 && operand_and_operator[2].length() > 0) {
      temp = "0";
   }
  }
  // 按鈕#
  else if(value > 305) {
    Serial.println("=");
    if(operand_and_operator[2].length() > 0) {
      Calculating(); 
    }    
  }
  // 按鈕/
  else if(value > 290) { 
    Serial.println("/");
    sign = "/";
    Operand_Update();
  }
  Operator_Update();
}

void Operator_Update() {
  if(operand_and_operator[1].length() > 0) {
    operand_and_operator[2] += temp;    
  }
  else if(operand_and_operator[1].length() == 0) {
    operand_and_operator[0] += temp;
  }
  temp = "";  
}

void Operand_Update() {
  if(operand_and_operator[0].length() > 0 && operand_and_operator[1].length() < 1) {
    operand_and_operator[1] += sign; 
  }
  sign = "";  
}

void Calculating() {
  is_calculating = true;
  
  a = atol(operand_and_operator[0].c_str());
  b = atol(operand_and_operator[2].c_str());
  
  long result;

  if(operand_and_operator[1] == "+") {
    result = a + b;
  }
  else if(operand_and_operator[1] == "-") {
    result = a - b;
  }
  else if(operand_and_operator[1] == "*") {
    result = a * b;
  }
  else if(operand_and_operator[1] ==  "/")  {
    result = a / b;
  }
  operand_and_operator[3] = (String)result;
  Serial.println(operand_and_operator[3]);
   
  lcd.setCursor(0, 0);
  lcd.print(operand_and_operator[0]);
  lcd.print(operand_and_operator[1]);
  lcd.print(operand_and_operator[2]);
  lcd.print("="); 
  lcd.setCursor(0, 1);
  lcd.print("Ans:"); 
  lcd.print(operand_and_operator[3]);
  lcd.noCursor();
  lcd.noBlink();
}
