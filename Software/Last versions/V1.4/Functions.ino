void reboot( void )
{
  asm volatile("jmp 0x00");
}

void updateDisplay( int m )      //отрисовка показаний
{
  int page;
  
  page = (m + 2) / 2;
  lcd.clear();
  lcd.setCursor(0, 0);
  if ((int)(data1[m] / 100) < 10)  //
    lcd.print(0);
  lcd.print((int)(data1[m] / 100));
  lcd.print("-");
  if (data1[m] % 100 < 10)
    lcd.print(0);
  lcd.print(data1[m] % 100);
  lcd.print("-");
  lcd.print(data2[m]);
  lcd.print(" ");
  if ((int)(tm[m] / 100) < 10)
    lcd.print(0);
  lcd.print((int)(tm[m] / 100));
  lcd.print(":");
  if (tm[m] % 100 < 10)
    lcd.print(0);
  lcd.print(tm[m] % 100);
  lcd.setCursor(0, 1);
  lcd.print("TEM");
  lcd.write(4);
  lcd.print(":");
  lcd.print(temp[m]);
  lcd.write(223);
  lcd.setCursor(0, 2);
  if ((int)(data1[m + 1] / 100) < 10)
    lcd.print(0);
  lcd.print((int)(data1[m + 1] / 100));
  lcd.print("-");
  if (data1[m + 1] % 100 < 10)
    lcd.print(0);
  lcd.print(data1[m + 1] % 100);
  lcd.print("-");
  lcd.print(data2[m + 1]);
  lcd.print(" ");
  if ((int)(tm[m + 1] / 100) < 10)
    lcd.print(0);
  lcd.print((int)(tm[m + 1] / 100));
  lcd.print(":");
  if (tm[m + 1] % 100 < 10)
    lcd.print(0);
  lcd.print(tm[m + 1] % 100);
  lcd.setCursor(0, 3);
  lcd.print("TEM");
  lcd.write(4);
  lcd.print(":");
  lcd.print(temp[m + 1]);
  lcd.write(223);
  if (page < 10)
  {
    lcd.setCursor(19, 3);  
    lcd.print(page);
  }
  else
  {
    lcd.setCursor(18, 3);
    lcd.print(page);
  }  
}

int catchEnc( int inp )    //catchEnc(1)
{
  enc1.tick();
  if ((enc1.isLeft() || enc1.isRight()) && inp != 1)
    drawMenu();
  else
    if ((enc1.isLeft() || enc1.isRight()) && inp == 1)
      return 1;
}

void drawMenu( void )    //отрисовка показаний температур
{
  int m = 0;
  unsigned long t = millis();
  
  updateDisplay(m);
  while (millis() - t <= DELTA2 * 1000)
  {
    enc1.tick();
    if (enc1.isLeft())
    {
      if (m == 0)
      {
        //pushSound();
        m = 70;
        t = millis();
        updateDisplay(m);
      }
      else
      {
        //pushSound();
        m -= 2;
        t = millis();
        updateDisplay(m);
      }
    }
    if (enc1.isRight())
    {
      if (m == 70)
      {
        //pushSound();
        m = 0;
        t = millis();
        updateDisplay(m);  
      }
      else
      {
        //pushSound();
        m += 2;
        t = millis();
        updateDisplay(m);
      }
    }
  }
  lcd.clear();
  printTemp();
}
void readMas( float *tmp, int *dat1, int *dat2,int *tim )    //заполнение массива из памяти
{
  int i, j;

  for (i = 600, j = 0; i < 888; i += 4, j++)
    tmp[j] = eeprom_read_float(i);
  for (i = 0, j = 0; i < 144; i += 2, j++)
    dat1[j] = eeprom_read_word(i);
  for (i = 200, j = 0; i < 344; i += 2, j++)
    dat2[j] = eeprom_read_word(i);
  for (i = 400, j = 0; i < 544; i +=2, j++)
    tim[j] = eeprom_read_word(i);  
}

void saveMas( float *tmp, int *dat1, int *dat2,int *tim )     //сохранение массива в энергонезависимую память
{
  int i, j;

  for (i = 600, j = 0; i < 888; i += 4, j++)
    eeprom_update_float(i, tmp[j]);
  for (i = 0, j = 0; i < 144; i += 2, j++)
    eeprom_update_word(i, dat1[j]);
  for (i = 200, j = 0; i < 344; i += 2, j++)
    eeprom_update_word(i, dat2[j]);
  for (i = 400, j = 0; i < 544; i +=2, j++)
    eeprom_update_word(i, tim[j]);
}

void moveMas( float *tmp, int *dat1, int *dat2,int *tim )    //смещение массива влево
{
  int i;

  for (i = 0; i < 71; i++)
  {
    tmp[i] = tmp[i + 1];
    dat1[i] = dat1[i + 1];
    dat2[i] = dat2[i + 1];
    tim[i] = tim[i + 1];
  }  
}

void printTemp( void ) //функция главного экрана
{
  lcd.setCursor(0, 0);
  lcd.print("TEKY");
  lcd.write(2);
  lcd.print("A");
  lcd.write(3);
  lcd.print(" TEM");
  lcd.write(4);
  lcd.print("EPATYPA");
  lcd.setCursor(0, 1);
  lcd.print(time.gettime("d-m-Y H:i"));
  lcd.setCursor(0, 2);
  lcd.print("TEM");
  lcd.write(4);
  lcd.print(":");
  if (lTemp != getTemp())
  {
    lcd.setCursor(11, 2);
    lcd.print("   ");
    lTemp = getTemp();
  }
  if (lTemp >= 10 && getTemp() < 10)
  {
    lcd.setCursor(10, 2);
    lcd.print("   ");
    lTemp = getTemp();  
  }
  lcd.setCursor(5, 2);
  lcd.print(getTemp());
  lcd.write(223);
}

void tempError( void )   //объявление об ошибке сенсора
{
  unsigned long t = millis();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   SENSOR ERROR!!");
  ledOn();
  sensors.requestTemperatures();
  pushSound();
  while (getTemp() == -127)
  {
    sensors.requestTemperatures();
    if (millis() - t >= BOOP * 1000)
    {
      pushSound();
      t = millis();
    }
  }
  lcd.clear();  
  ledOff();
}

float getTemp( void )    //функция получения температуры
{
  sensors.requestTemperatures();
  return sensors.getTempC(insideThermometer);  
}

void pushSound( void )    //звук пищалки
{
  tone(BUZ, 400);
  delay(50);
  noTone(BUZ);
}

void ledOn( void )     //включение светодиода
{
  digitalWrite(LED, HIGH);
}

void ledOff( void )   //выключение светодиода
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

byte bissextile( int yr )  //проверка на високосность
{
  if (yr % 4 == 0)
    if (yr % 100 != 0)
      return 1;
    else 
      if (yr % 400 == 0)
        return 1;
  return 0;
}

byte CheckData( int dy, int mnth, int yr )  //проверка корректности дат (1 если все хорошо 0 если плохо)
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
  
void SetTime( void )      //функция настройки времени
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
