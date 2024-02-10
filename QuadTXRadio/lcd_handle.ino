//  #include <LiquidCrystal.h>
//  LiquidCrystal lcd(8, 9, 6, 5, 4, 3);// db14->>pin 3, lcd5->pin8
//  #ifndef  KEYVAL_STRUCT_DEF
//  #define KEYVAL_STRUCT_DEF
//typedef struct
//{
//  char msg[4];
//  float val;  
//}keyValPair_st;
//#endif
//void lcdInit();
// void printOnLcd(keyValPair_st *dispInfo,int count);
// void lcdInit(String msg)
// {
//  lcd.begin(16, 2);
//  lcd.setCursor(0,0);
//  lcd.print(msg);
//  
// }
//
// void printOnLcd(keyValPair_st *dispInfo,int count)
// {
//  int i=0;
//  int pos=0;
//  int j=0;
//  lcd.clear();
//  while(i<count/2)
//  {   
//  lcd.setCursor(pos,i);
//  lcd.print(dispInfo->msg);
//  Serial.print(dispInfo->msg);
//  lcd.setCursor(pos+=4,i);
//  lcd.print(dispInfo->val);
//   Serial.print(dispInfo->val);
//  dispInfo++;
//  pos+=4;
//  lcd.setCursor(pos,i);
//  lcd.print(dispInfo->msg);
//   Serial.print(dispInfo->msg);
//  lcd.setCursor(pos+=4,i);
//  lcd.print(dispInfo->val);
//  Serial.print(dispInfo->val);
//  i++;
//  pos=0;
//  dispInfo++;
//  }
//delay(1000);
//
//  
// }
