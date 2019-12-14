void pushSound( void )
{
  tone(BUZ, 400);
  delay(70);
  noTone(BUZ);
}

void ledOn( void )
{
  digitalWrite(LED, HIGH);
}

void ledOff( void )
{
  digitalWrite(LED, LOW);
}

void buttonTick( void )
{
  triger = 1;
}

void PrintTime( int dy, int mnth, int yr, int hr, int mnt, int cln )
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HACTPO");            //надпись НАСТРОЙКИ
  lcd.write(0);
  lcd.print("K");
  lcd.write(1);
  if (cln == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("                  ");
  }
    lcd.setCursor(0, 1);
    if (dy < 10)
      lcd.print(0);
    lcd.print(dy);
    lcd.print(".");
    if (mnth < 10)
      lcd.print(0);
    lcd.print(mnth);
    lcd.print(".");
    lcd.print(yr);
    lcd.print(" ");
    if (hr < 10)
      lcd.print(0);
    lcd.print(hr);
    lcd.print(":");
    if (mnt < 10)
      lcd.print(0);
    lcd.print(mnt);
}

byte bissextile( int yr )
{
  if (yr % 4 == 0)
    if (yr % 100 != 0)
      return 1;
    else 
      if (yr % 400 == 0)
        return 1;
  return 0;
}

byte CheckData( int dy, int mnth, int yr )  //1 если все хорошо 0 если плохо
{
  byte days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  if (mnth == 2)
    if (bissextile(yr) == 1 && dy <= 29)
      return 1;
    else
      if (bissextile(yr) == 0 && dy <= 28)
        return 0;
  if (dy <= days[mnth - 1])
    return 1;
  return 0;
  }
  
void SetTime( void )
{   
    time.gettime();
    int dy = time.day, mnth = time.month, yr = time.year + 2000, hr = time.Hours, mnt = time.minutes, trigger = 1;

    if (beginning == 0)
    {
      beginning = 1;
      return;
    }
    PrintTime(dy, mnth, yr, hr, mnt, 0);
    while (trigger != 6) 
    {
      switch (trigger)
      {
        case 1:                               //выставка дня
          enc1.tick();
          lcd.setCursor(1, 1);
          lcd.blink();
          if (enc1.isRight())
          {
            if (dy + 1 > 31)
              dy = 1;
            else
              dy++;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isLeft())  
          {
            if (dy - 1 < 1)
              dy = 31;
            else
              dy--;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isClick())
          { 
            pushSound();
            trigger++;
          }   
          break; 
          
        case 2:                      //выставка месца
          enc1.tick();
          lcd.setCursor(4, 1);
          lcd.blink();
          if (enc1.isRight())
          {
            if (mnth + 1 > 12)
              mnth = 1;
            else
              mnth++;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isLeft())  
          {
            if (mnth - 1 < 1)
              mnth = 12;
            else
              mnth--;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isClick())
          { 
            pushSound();
            trigger++;
          }   
          break; 
        
        case 3:                    //выставка года
          enc1.tick();
          lcd.setCursor(9, 1);
          lcd.blink();
          if (enc1.isRight())
          {
            if (yr + 1 > 2100)
              yr = 2000;
            else
              yr++;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isLeft())  
          {
            if (yr - 1 < 2000)
              yr = 2100;
            else
              yr--;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isClick())
          { 
            pushSound();
            trigger++;
          }   
          break; 

        case 4:                   //выставка часов
          enc1.tick();
          lcd.setCursor(12, 1);
          lcd.blink();
          if (enc1.isRight())
          {
            if (hr + 1 > 23)
              hr = 0;
            else
              hr++;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isLeft())  
          {
            if (hr - 1 < 0)
              hr = 23;
            else
              hr--;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isClick())
          { 
            pushSound();
            trigger++;
          }   
          break; 

        case 5:                 //выставка минут
          enc1.tick();
          lcd.setCursor(15, 1);
          lcd.blink();
          if (enc1.isRight())
          {
            if (mnt + 1 > 59)
              mnt = 0;
            else
              mnt++;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isLeft())  
          {
            if (mnt - 1 < 0)
              mnt = 59;
            else
              mnt--;
            PrintTime(dy, mnth, yr, hr, mnt, 1);
          }
          if (enc1.isClick())
          { 
            pushSound();
            trigger++;
          } 
          break;
    }
  }
  if (CheckData(dy, mnth, yr) == 0)  //проверка на корректность
  {
    lcd.clear();
    lcd.noBlink();
    ledOn();
    lcd.setCursor(2, 0);
    lcd.print("DATA ERROR!!");
    delay(3000);
    ledOff();
    SetTime();
  }
  else
  {
  lcd.noBlink(); 
  lcd.clear(); 
  triger = 0;
  yr = yr % 2000;
  time.settime(0, mnt, hr, dy, mnth, yr, 1); 
  }   
}
