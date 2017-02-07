//**************************************PROJECT PDA ATLAS**************************************
#include <SPI.h> //подключаем библиотеку
#include <RF24.h> //подключаем библиотеку
#include <TFT_HX8357.h> //подключаем библиотеку
#include <User_Setup.h> //подключаем библиотеку
#include <Time.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <EEPROM.h>

//***************************************ЭКРАН****************************************
TFT_HX8357 tft = TFT_HX8357(); //конструктор пинов
//***************************************РАДИО****************************************
RF24 radio(9, 53); //9 и 53 диджитал пины
const uint32_t pipe = 123456789;
//**************************************ДЖОЙСТИК**************************************
unsigned short int xPin = A1;
unsigned short int yPin = A0;
unsigned short int clickPin = A2;

unsigned short int positionX = 0;
unsigned short int positionY = 0;
unsigned short int clickState = 0;

short int x = 0;
short int y = 0;
short int c = 0;
//****************************************ЧАСЫ*****************************************
tmElements_t tm;

bool parse=false;
bool config=false;
//**************************************СТРУКТУРЫ**************************************
struct player
  {
    float health = 100;
    float radiation = 80000;
    float food = 100;
    float water = 100;
  };

struct suit
  {
    String suitName = "Zarya";
    float integrity = 30;
    unsigned short int thermal = 25;
    unsigned short int chemical = 50;
    unsigned short int electrical = 75;
    unsigned short int gravitational = 0;
    unsigned short int psycho = 60;
    unsigned short int radiation = 25;
    unsigned short int blowout = 0;
  };

struct reducingDrugs
{
  unsigned short int _1 = 4;
  unsigned short int _2 = 2;
  unsigned short int _3 = 1;
  unsigned short int _4 = 1;
  unsigned short int _5 = 2;
  unsigned short int _6 = 1;
};

struct protectingDrugs
{
  unsigned short int _1 = 2;
  unsigned short int _2 = 3;
  unsigned short int _3 = 1;
  unsigned short int _4 = 2;
};

struct repair
{
  unsigned short int _1 = 3;
  unsigned short int _2 = 1;
  unsigned short int _3 = 2;
  unsigned short int _4 = 0;
};

struct foodAndDrinks
{
  unsigned short int _1 = 1;
  unsigned short int _2 = 2;
  unsigned short int _3 = 5;
  unsigned short int _4 = 2;
  unsigned short int _5 = 4;
  unsigned short int _6 = 2;
};

struct artifacts
{
  unsigned short int _1 = 0;
  unsigned short int _2 = 0;
  unsigned short int _3 = 0;
  unsigned short int _4 = 0;
  unsigned short int _5 = 0;
  unsigned short int _6 = 0;
  unsigned short int _7 = 0;
  unsigned short int _8 = 0;
  unsigned short int _9 = 0;
  unsigned short int _10 = 0;
  unsigned short int _11 = 0;
  unsigned short int _12 = 0;
  unsigned short int _13 = 0;
  unsigned short int _14 = 0;
  unsigned short int _15 = 0;
  unsigned short int _16 = 0;
  unsigned short int _17 = 0;
  unsigned short int _18 = 0;
  unsigned short int _19 = 0;
  unsigned short int _20 = 0;
  unsigned short int _21 = 0;
  unsigned short int _22 = 0;
  unsigned short int _23 = 0;
  unsigned short int _24 = 0;
  unsigned short int _25 = 0;
  unsigned short int _26 = 0;
  unsigned short int _27 = 0;
  unsigned short int _28 = 0;
  unsigned short int _29 = 0;
  unsigned short int _30 = 0;
};

struct player player; //объявление структуры
struct suit suit; //объявление структуры
struct reducingDrugs reducingDrugs; //объявление структуры
struct protectingDrugs protectingDrugs; //объявление структуры
struct repair repair; //объявление структуры
struct foodAndDrinks foodAndDrinks; //объявление структуры
struct artifacts artifacts; //объявление структуры

//**************************************ПЕРЕМЕННЫЕ**************************************
short int id = 15;

float anomalyTemp = 0;
float protectionTemp = 0; //процент защиты
unsigned short int radiationHitTime = 0; //таймер удара радиации
unsigned short int healthRegenerationTime = 0; //таймер регенерации здоровья
unsigned short int radiationRegenerationTime = 0; //таймер регенерации радиации
unsigned short int foodHitTime = 0; //таймер удара еды
unsigned short int waterHitTime = 0; //таймер удара воды
unsigned short int dehydrating = 0;
unsigned short int hunger = 0;

short int timerTempPsycho=1000;
short int hourTempPsycho=1000;
short int minuteTempPsycho=1000;
short int suitTempPsycho=0;

short int timerTempRadiation=1000;
short int hourTempRadiation=1000;
short int minuteTempRadiation=1000;
short int suitTempRadiation=0;

short int timerTempChemical=1000;
short int hourTempChemical=1000;
short int minuteTempChemical=1000;
short int suitTempChemical=0;

short int timerTempBlowout=1000;
short int hourTempBlowout=1000;
short int minuteTempBlowout=1000;
short int suitTempBlowout=0;

short int timerTempEnergetik=1000;
short int hourTempEnergetik=1000;
short int minuteTempEnergetik=1000;

unsigned short int counter = 0;

long int transferResolution = 0;
long int transferId = 0;
long int transferItem = 0;
long int transferQuantity = 0;
//*************************************************************************************
void setup() 
{
  displaySetup(); //настройки дисплея

  //rtcSetup(); //установка времени

  radioSetup(); //настройки радиомодуля

  //writeEEPROM(); //запись в EEPROM

  //readEEPROM(); //считывание из EEPROM
  
}

void loop() 
{  
  if(tm.Hour>=hourTempPsycho&&tm.Minute>=minuteTempPsycho)
  {
    suit.psycho=suitTempPsycho;
    EEPROM.write(10,suit.psycho);
    hourTempPsycho=1000;
    EEPROM.write(33,hourTempPsycho);
    minuteTempPsycho=1000;
    EEPROM.write(34,minuteTempPsycho);
  }

  if(tm.Hour>=hourTempRadiation&&tm.Minute>=minuteTempRadiation)
  {
    suit.radiation=suitTempRadiation;
    EEPROM.write(11,suit.radiation);
    hourTempRadiation=1000;
    EEPROM.write(36,hourTempRadiation);
    minuteTempRadiation=1000;
    EEPROM.write(37,minuteTempRadiation);
  }

  if(tm.Hour>=hourTempChemical&&tm.Minute>=minuteTempChemical)
  {
    suit.chemical=suitTempChemical;
    EEPROM.write(7,suit.chemical);
    hourTempChemical=1000;
    EEPROM.write(39,hourTempChemical);
    minuteTempChemical=1000;
    EEPROM.write(40,minuteTempChemical);
  }

  if(tm.Hour>=hourTempBlowout&&tm.Minute>=minuteTempBlowout)
  {
    suit.blowout=suitTempBlowout;
    EEPROM.write(12,suit.blowout);
    hourTempBlowout=1000;
    EEPROM.write(42,hourTempBlowout);
    minuteTempBlowout=1000;
    EEPROM.write(43,minuteTempBlowout);
  }

  if(tm.Hour>=hourTempEnergetik&&tm.Minute>=minuteTempEnergetik)
  {
    player.food=player.food-50;
    EEPROM.write(2,player.food);
    player.water=player.water-50;
    EEPROM.write(3,player.water);
    hourTempEnergetik=1000;
    EEPROM.write(45,hourTempEnergetik);
    minuteTempEnergetik=1000;
    EEPROM.write(46,minuteTempEnergetik);
  }
  
  positionXLoop();

  switch(x)
  {
    case 0:
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();

      foodDamage();

      waterDamage();

      playerHunger();

      playerDehydrating();
      
      healthRegeneration();
      
      radiationRegeneration();
      
      firstPage();
      
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0);
    break;
    
    case 1:
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();

      foodDamage();

      waterDamage();

      playerHunger();

      playerDehydrating();
      
      healthRegeneration();
      
      radiationRegeneration();
      
      secondPage();
      
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=1023&&positionX!=0&&x==1);
    break;
    
    case 2:
    y=0;
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();

      foodDamage();

      waterDamage();

      playerHunger();

      playerDehydrating();
      
      healthRegeneration();
      
      radiationRegeneration();
      
      positionYLoop(); 

      switch(y)
      {
        case 0:      
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerOne();
        clickState=analogRead(clickPin);
        if(clickState==0&&reducingDrugs._1>0)
        {
          player.health=player.health+10;
          EEPROM.write(0,player.health);
          reducingDrugs._1--;
          EEPROM.write(13,reducingDrugs._1);
          tft.fillRect(435,80,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
        
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerTwo();
        clickState=analogRead(clickPin);
        if(clickState==0&&reducingDrugs._2>0)
        {
          player.health=player.health+25;
          EEPROM.write(0,player.health);
          reducingDrugs._2--;
          EEPROM.write(14,reducingDrugs._2);
          tft.fillRect(435,110,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
          
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerThree();
        clickState=analogRead(clickPin);
        if(clickState==0&&reducingDrugs._3>0)
        {
          player.health=player.health+50;
          EEPROM.write(0,player.health);
          player.radiation=player.radiation-(player.radiation*25/100);
          EEPROM.write(1,player.radiation*100/1000000);
          reducingDrugs._3--;
          EEPROM.write(15,reducingDrugs._3);
          tft.fillRect(435,140,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerFour();
        clickState=analogRead(clickPin);
        if(clickState==0&&reducingDrugs._4>0)
        {
          player.health=player.health+75;
          EEPROM.write(0,player.health);
          player.radiation=player.radiation-(player.radiation*50/100);
          EEPROM.write(1,player.radiation*100/1000000);
          reducingDrugs._4--;
          EEPROM.write(16,reducingDrugs._4);
          tft.fillRect(435,170,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      
        case 4:
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerFive();
        clickState=analogRead(clickPin);
        if(clickState==0&&reducingDrugs._5>0)
        {
          player.radiation=0;
          EEPROM.write(1,player.radiation*100/1000000);
          reducingDrugs._5--;
          EEPROM.write(17,reducingDrugs._5);
          tft.fillRect(435,200,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      
        case 5:
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        thirdPageOne();
        chooseMarkerSix();
        clickState=analogRead(clickPin);
        if(clickState==0&&reducingDrugs._6>0)
        {
          player.health=100;
          EEPROM.write(0,player.health);
          reducingDrugs._6--;
          EEPROM.write(18,reducingDrugs._6);
          tft.fillRect(435,230,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      }
            
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0&&positionX!=1023&&x==2);
    break;

    case 3:
    y=0;
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();

      foodDamage();

      waterDamage();

      playerHunger();

      playerDehydrating();
      
      healthRegeneration();
      
      radiationRegeneration();
      
      positionYLoop(); 

     switch(y)
     {
        case 0:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageTwo();
        chooseMarkerOne();
        clickState=analogRead(clickPin);
        if(clickState==0&&protectingDrugs._1>0)
        {
          suitTempPsycho=suit.psycho;
          suit.psycho=suit.psycho+50;
          EEPROM.write(10,suit.psycho);
          protectingDrugs._1--;
          EEPROM.write(19,protectingDrugs._1);
          hourTempPsycho=tm.Hour;
          minuteTempPsycho=tm.Minute;
          timerTempPsycho=1;
          minuteTempPsycho=minuteTempPsycho+timerTempPsycho;
          if(minuteTempPsycho>60)
          {
            hourTempPsycho++;
            minuteTempPsycho=minuteTempPsycho-60;          
          }
          EEPROM.write(33,hourTempPsycho);
          EEPROM.write(34,minuteTempPsycho);
          EEPROM.write(35,suitTempPsycho);
          tft.fillRect(435,80,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
        
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageTwo();
        chooseMarkerTwo();
        clickState=analogRead(clickPin);
        if(clickState==0&&protectingDrugs._2>0)
        {
          suitTempRadiation=suit.radiation;
          suit.radiation=suit.radiation+50;
          EEPROM.write(11,suit.radiation);
          protectingDrugs._2--;
          EEPROM.write(20,protectingDrugs._2);
          hourTempRadiation=tm.Hour;
          minuteTempRadiation=tm.Minute;
          timerTempRadiation=1;
          minuteTempRadiation=minuteTempRadiation+timerTempRadiation;
          if(minuteTempRadiation>60)
          {
            hourTempRadiation++;
            minuteTempRadiation=minuteTempRadiation-60;          
          }
          EEPROM.write(36,hourTempRadiation);
          EEPROM.write(37,minuteTempRadiation);
          EEPROM.write(38,suitTempRadiation);
          tft.fillRect(435,110,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
        
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageTwo();
        chooseMarkerThree();
        clickState=analogRead(clickPin);
        if(clickState==0&&protectingDrugs._3>0)
        {
          suitTempChemical=suit.chemical;
          suit.chemical=suit.chemical+50;
          EEPROM.write(7,suit.chemical);
          protectingDrugs._3--;
          EEPROM.write(21,protectingDrugs._3);
          hourTempChemical=tm.Hour;
          minuteTempChemical=tm.Minute;
          timerTempChemical=1;
          minuteTempChemical=minuteTempChemical+timerTempChemical;
          if(minuteTempChemical>60)
          {
            hourTempChemical++;
            minuteTempChemical=minuteTempChemical-60;          
          }
          EEPROM.write(39,hourTempChemical);
          EEPROM.write(40,minuteTempChemical);
          EEPROM.write(41,suitTempChemical);
          tft.fillRect(435,140,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageTwo();
        chooseMarkerFour();
        clickState=analogRead(clickPin);
        if(clickState==0&&protectingDrugs._4>0)
        {
          suitTempBlowout=suit.blowout;
          suit.blowout=suit.blowout+100;
          EEPROM.write(12,suit.blowout);
          protectingDrugs._4--;
          EEPROM.write(22,protectingDrugs._4);
          hourTempBlowout=tm.Hour;
          minuteTempBlowout=tm.Minute;
          timerTempBlowout=1;
          minuteTempBlowout=minuteTempBlowout+timerTempBlowout;
          if(minuteTempBlowout>60)
          {
            hourTempBlowout++;
            minuteTempBlowout=minuteTempBlowout-60;          
          }
          EEPROM.write(42,hourTempBlowout);
          EEPROM.write(43,minuteTempBlowout);
          EEPROM.write(44,suitTempBlowout);
          tft.fillRect(435,170,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
     }
      
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0&&positionX!=1023&&x==3);
    break;

    case 4:
    y=0;
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();

      foodDamage();

      waterDamage();

      playerHunger();

      playerDehydrating();
      
      healthRegeneration();
      
      radiationRegeneration();
      
      positionYLoop();

      switch(y)
      {
        case 0:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageThree();
        chooseMarkerOne();
        clickState=analogRead(clickPin);
        if(clickState==0&&repair._1>0)
        {
          suit.integrity=suit.integrity+10;
          EEPROM.write(5,suit.integrity);
          repair._1--;
          EEPROM.write(23,repair._1);
          tft.fillRect(435,80,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageThree();
        chooseMarkerTwo();
        clickState=analogRead(clickPin);
        if(clickState==0&&repair._2>0)
        {
          suit.integrity=suit.integrity+25;
          EEPROM.write(5,suit.integrity);
          repair._2--;
          EEPROM.write(24,repair._2);
          tft.fillRect(435,110,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
       
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageThree();
        chooseMarkerThree();
        clickState=analogRead(clickPin);
        if(clickState==0&&repair._3>0)
        {
          suit.integrity=suit.integrity+50;
          EEPROM.write(5,suit.integrity);
          repair._3--;
          EEPROM.write(25,repair._3);
          tft.fillRect(435,140,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageThree();
        chooseMarkerFour();
        clickState=analogRead(clickPin);
        if(clickState==0&&repair._4>0)
        {
          suit.integrity=suit.integrity+75;
          EEPROM.write(5,suit.integrity);
          repair._4--;
          EEPROM.write(26,repair._4);
          tft.fillRect(435,170,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      }
      
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0&&positionX!=1023&x==4);
    break;

    case 5:
    y=0;
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();

      foodDamage();

      waterDamage();

      playerHunger();

      playerDehydrating();
      
      healthRegeneration();
      
      radiationRegeneration();
      
      positionYLoop();

      switch(y)
      {
        case 0:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerOne();
        clickState=analogRead(clickPin);
        if(clickState==0&&foodAndDrinks._1>0)
        {
          player.food=player.food+10;
          EEPROM.write(2,player.food);
          player.health=player.health+5;
          EEPROM.write(0,player.health);
          foodAndDrinks._1--;
          EEPROM.write(27,foodAndDrinks._1);
          tft.fillRect(435,80,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
       
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerTwo();
        clickState=analogRead(clickPin);
        if(clickState==0&&foodAndDrinks._2>0)
        {
          player.food=player.food+25;
          EEPROM.write(2,player.food);
          player.health=player.health+10;
          EEPROM.write(0,player.health);
          foodAndDrinks._2--;
          EEPROM.write(28,foodAndDrinks._2);
          tft.fillRect(435,110,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
       
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerThree();
        clickState=analogRead(clickPin);
        if(clickState==0&&foodAndDrinks._3>0)
        {
          player.food=player.food+20;
          EEPROM.write(2,player.food);
          player.water=player.water+10;
          EEPROM.write(3,player.water);
          player.health=player.health+10;
          EEPROM.write(0,player.health);
          foodAndDrinks._3--;
          EEPROM.write(29,foodAndDrinks._3);
          tft.fillRect(435,140,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerFour();
        clickState=analogRead(clickPin);
        if(clickState==0&&foodAndDrinks._4>0)
        {
          player.water=player.water-10;
          EEPROM.write(3,player.water);
          player.health=player.health-10;
          EEPROM.write(0,player.health);
          player.radiation=player.radiation-5000;
          EEPROM.write(1,player.radiation*100/1000000);
          foodAndDrinks._4--;
          EEPROM.write(30,foodAndDrinks._4);
          tft.fillRect(435,170,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;
      
        case 4:
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerFive();   
        clickState=analogRead(clickPin);
        if(clickState==0&&foodAndDrinks._5>0)
        {
          player.food=player.food+50;
          EEPROM.write(2,player.food);
          player.water=player.water+50;  
          EEPROM.write(3,player.water);        
          foodAndDrinks._5--;
          EEPROM.write(31,foodAndDrinks._5);
          hourTempEnergetik=tm.Hour;
          minuteTempEnergetik=tm.Minute;
          timerTempEnergetik=1;
          minuteTempEnergetik=minuteTempEnergetik+timerTempEnergetik;
          if(minuteTempEnergetik>60)
          {
            hourTempEnergetik++;
            minuteTempEnergetik=minuteTempEnergetik-60;          
          }  
          EEPROM.write(45,hourTempEnergetik);
          EEPROM.write(46,minuteTempEnergetik);
          tft.fillRect(435,200,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;  

        case 5:
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        thirdPageFour();
        chooseMarkerSix();
        clickState=analogRead(clickPin);
        if(clickState==0&&foodAndDrinks._6>0)
        {
          player.water=player.water+40;
          EEPROM.write(3,player.water);
          foodAndDrinks._6--;
          EEPROM.write(32,foodAndDrinks._6);
          tft.fillRect(435,230,20,20,TFT_BLACK); //стирание маркера сверху-вниз
        }
        break;

        
      }
      
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0&&positionX!=1023&&x==5);
    break; 

    case 6:
    y=0;
   
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();

      foodDamage();

      waterDamage();

      playerHunger();

      playerDehydrating();
      
      healthRegeneration();
      
      radiationRegeneration();
      
      positionYLoopArtifacts();

      switch(y)
      {
        case 0:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveOne();
        chooseMarkerOne();
        break;
       
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveOne();
        chooseMarkerTwo();
        break;
       
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveOne();
        chooseMarkerThree();
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveOne();
        chooseMarkerFour();
        break;
      
        case 4:
        if(counter==1)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter--;
        }
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveOne();
        chooseMarkerFive();
        break;    
        //---//
        case 5:
        if(counter==0)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter++;
        }
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveTwo();
        chooseMarkerOne();
        break;

        case 6:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveTwo();
        chooseMarkerTwo();
        break;
       
        case 7:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveTwo();
        chooseMarkerThree();
        break;
      
        case 8:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveTwo();
        chooseMarkerFour();
        break;
      
        case 9:
        if(counter==0)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter++;
        }
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveTwo();
        chooseMarkerFive();    
        break;  
        //---//
        case 10:
        if(counter==1)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter--;
        }
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveThree();
        chooseMarkerOne();
        break;
       
        case 11:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveThree();
        chooseMarkerTwo();
        break;
       
        case 12:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveThree();
        chooseMarkerThree();
        break;
      
        case 13:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveThree();
        chooseMarkerFour();
        break;
      
        case 14:
        if(counter==1)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter--;
        }
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveThree();
        chooseMarkerFive();    
        break;  
        //---//
        case 15:
        if(counter==0)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter++;
        }
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFour();
        chooseMarkerOne();
        break;
       
        case 16:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFour();
        chooseMarkerTwo();
        break;
       
        case 17:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFour();
        chooseMarkerThree();
        break;
      
        case 18:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFour();
        chooseMarkerFour();
        break;
      
        case 19:
        if(counter==0)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter++;
        }
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFour();
        chooseMarkerFive();    
        break;  
        //---//
        case 20:
        if(counter==1)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter--;
        }
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFive();
        chooseMarkerOne();
        break;
       
        case 21:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFive();
        chooseMarkerTwo();
        break;
       
        case 22:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFive();
        chooseMarkerThree();
        break;
      
        case 23:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFive();
        chooseMarkerFour();
        break;
      
        case 24:
        if(counter==1)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter--;
        }
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveFive();
        chooseMarkerFive();    
        break;  
        //---//
        case 25:
        if(counter==0)
        {
          tft.fillScreen(TFT_BLACK);//фон дисплея
          counter++;
        }
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveSix();
        chooseMarkerOne();
        break;
       
        case 26:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveSix();
        chooseMarkerTwo();
        break;
       
        case 27:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveSix();
        chooseMarkerThree();
        break;
      
        case 28:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveSix();
        chooseMarkerFour();
        break;
      
        case 29:
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFiveSix();
        chooseMarkerFive();    
        break;  
        //---//
      }
      
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0&&positionX!=1023&&x==6);
    break; 







    case 7:
    y=0;
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();

      foodDamage();

      waterDamage();

      playerHunger();

      playerDehydrating();
      
      healthRegeneration();
      
      radiationRegeneration();
      
      positionYLoop(); 

      thirdPageSix();

      radioReceiving();
            
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=1023&&x==7);
    break;
  }
}

//**************************************ФУНКЦИИ**************************************
void displaySetup()
{
  tft.init();//инициализация дисплея
  tft.setRotation(1);//ориентация дисплея
  tft.fillScreen(TFT_BLACK);//фон дисплея
  tft.setTextFont(4);//типа шрифта дисплея
  tft.setTextColor(TFT_LIGHTGREY);//цвет шрифта дисплея
}

void radioSetup()
{
  Serial.begin(9600);
  radio.begin();
  //delay(2);
  radio.setChannel(9);
  radio.setDataRate(RF24_1MBPS); // RF24_1MBPS или RF24_2MBPS
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1,pipe); // открыть канал на приём
  radio.startListening(); // приём
}

void positionXLoop()
{
  positionX = analogRead(xPin);
  if(positionX == 1023) x--;
  if(positionX == 0) x++;
  if(x<0) x=0;
  if(x>7) x=7;
}

void positionYLoop()
{
  positionY = analogRead(yPin);
  if(positionY == 1023) y--;
  if(positionY == 0) y++;
  if(y<0) y=0;
  if(y>5) y=5;
}

void positionYLoopArtifacts()
{
  positionY = analogRead(yPin);
  if(positionY == 1023) y--;
  if(positionY == 0) y++;
  if(y<0) y=0;
  if(y>29) y=29;
}

void timeAndDate()
{
  if (RTC.read(tm)) 
 {
    tft.fillRoundRect(5,5,100,20,0,TFT_BLACK);
    tft.fillRoundRect(350,5,130,20,0,TFT_BLACK);
  
    tft.setCursor(5,5);//время
    print2digits(tm.Hour);
    tft.write(':');
    print2digits(tm.Minute);
    tft.write(':');
    print2digits(tm.Second);

    tft.setCursor(350,5);
    tft.print(tm.Day);
    tft.write('/');
    tft.print(tm.Month);
    tft.write('/');
    tft.print(tmYearToCalendar(tm.Year));
  
    tft.drawFastHLine(0, 31, 480, TFT_LIGHTGREY);
 }
}

void footer()
{
  tft.drawFastHLine(0, 295, 480, TFT_LIGHTGREY);
  tft.drawFastVLine(160, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(320, 295, 25, TFT_LIGHTGREY);

  tft.setCursor(73,297);
  tft.print("1");
  tft.setCursor(233,297);
  tft.print("2");
  tft.setCursor(393,297);
  tft.print("3");
}

void inventoryMenu()
{
  tft.drawFastHLine(0, 270, 480, TFT_LIGHTGREY); //вертикальная линия подменю
  tft.drawFastVLine(80, 270, 25, TFT_LIGHTGREY);  //-
  tft.drawFastVLine(160, 270, 25, TFT_LIGHTGREY); //--
  tft.drawFastVLine(240, 270, 25, TFT_LIGHTGREY); //--- 1-5 вертикальные линии подменю
  tft.drawFastVLine(320, 270, 25, TFT_LIGHTGREY); //--
  tft.drawFastVLine(400, 270, 25, TFT_LIGHTGREY); //-

  tft.fillRoundRect(37,275,6,16,0,TFT_LIGHTGREY); //крест вертикальная линия
  tft.fillRoundRect(32,280,16,6,0,TFT_LIGHTGREY); //крест горизонтальная линия

  tft.drawRoundRect(110,278,20,10,4,TFT_LIGHTGREY); //пилюля общий прямоугольник
  tft.fillRoundRect(119,278,11,10,4,TFT_LIGHTGREY); //пилюля заполнение наполовину

  tft.drawFastHLine(195,280,10,TFT_LIGHTGREY); //ключ верх рукоятки
  tft.drawFastHLine(195,285,10,TFT_LIGHTGREY); //ключ низ рукоятки
  tft.drawLine(205,280,208,277,TFT_LIGHTGREY); //головка ключа право верх
  tft.drawLine(205,285,208,288,TFT_LIGHTGREY); //головка ключа право низ
  tft.drawLine(195,280,192,277,TFT_LIGHTGREY); //головка ключа лево верх
  tft.drawLine(195,285,192,288,TFT_LIGHTGREY); //головка ключа лево низ
  tft.drawFastHLine(188,277,4,TFT_LIGHTGREY); //головка ключа от угла прямо лево верх
  tft.drawFastHLine(188,288,4,TFT_LIGHTGREY); //головка ключа от угла прямо лево низ
  tft.drawFastHLine(209,277,4,TFT_LIGHTGREY); //головка ключа от угла прямо право верх
  tft.drawFastHLine(209,288,4,TFT_LIGHTGREY); //головка ключа от угла прямо право низ
  tft.drawLine(213,277,210,280,TFT_LIGHTGREY); //головка ключа от второго угла во внутрь право верх
  tft.drawLine(213,288,210,285,TFT_LIGHTGREY); //головка ключа от второго угла во внутрь право низ
  tft.drawLine(187,277,190,280,TFT_LIGHTGREY); //головка ключа от второго угла во внутрь лево верх
  tft.drawLine(187,288,190,285,TFT_LIGHTGREY); //головка ключа от второго угла во внутрь лево низ
  tft.drawFastVLine(190, 280, 5, TFT_LIGHTGREY); //перемычка головки лево
  tft.drawFastVLine(210, 280, 5, TFT_LIGHTGREY); //перемычка головки право

  tft.drawRoundRect(270,278,20,10,3,TFT_LIGHTGREY); //хлебушек общий прямоугольник 
  tft.drawLine(272,287,276,278,TFT_LIGHTGREY); //хлебушек наклонная слева первая 
  tft.drawLine(277,287,281,278,TFT_LIGHTGREY); //хлебушек наклонная слева вторая 
  tft.drawLine(278,287,282,278,TFT_LIGHTGREY); //хлебушек наклонная слева третья
  tft.drawLine(283,287,287,278,TFT_LIGHTGREY); //хлебушек наклонная слева четвертая

  tft.drawRoundRect(347,276,27,14,3,TFT_LIGHTGREY); //книга общий контур
  tft.drawFastVLine(360, 276, 14, TFT_LIGHTGREY); //переплет книги
  tft.drawFastHLine(350,279,7,TFT_LIGHTGREY); //первая строчка книги слева
  tft.drawFastHLine(350,282,7,TFT_LIGHTGREY); //вторая строчка книги слева
  tft.drawFastHLine(350,283,7,TFT_LIGHTGREY); //третья строчка книги слева
  tft.drawFastHLine(350,286,7,TFT_LIGHTGREY); //чтвертая строчка книги слева
  tft.drawFastHLine(364,279,7,TFT_LIGHTGREY); //первая строчка книги слева
  tft.drawFastHLine(364,282,7,TFT_LIGHTGREY); //вторая строчка книги слева
  tft.drawFastHLine(364,283,7,TFT_LIGHTGREY); //третья строчка книги слева
  tft.drawFastHLine(364,286,7,TFT_LIGHTGREY); //чтвертая строчка книги слева

  tft.fillCircle(427,282,3,TFT_LIGHTGREY);
  tft.fillCircle(440,282,3,TFT_LIGHTGREY);
  tft.fillCircle(453,282,3,TFT_LIGHTGREY);
}

void firstPage()
{
  timeAndDate();
  
  tft.setCursor(5,50);
  tft.print("Health");
  tft.drawRoundRect(210,47,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,50,100,20,0,TFT_BLACK);
  tft.setCursor(315,50);
  tft.print(player.health);
  tft.print("%");

  tft.setCursor(5,100);
  tft.print("Suit");
  tft.drawRoundRect(210,97,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,100,100,20,0,TFT_BLACK);
  tft.setCursor(315,100);
  tft.print(suit.integrity);
  tft.print("%");
  
  tft.setCursor(5,150);
  tft.print("Radiation");
  tft.drawRoundRect(210,147,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,150,120,20,0,TFT_BLACK);
  tft.setCursor(315,150);
  tft.print(player.radiation);
  //tft.print("%");

  tft.setCursor(5,200);
  tft.print("Food");
  tft.drawRoundRect(210,197,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,200,100,20,0,TFT_BLACK);
  tft.setCursor(315,200);
  tft.print(player.food);
  tft.print("%");

  tft.setCursor(5,250);
  tft.print("Water");
  tft.drawRoundRect(210,247,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,250,100,20,0,TFT_BLACK);
  tft.setCursor(315,250);
  tft.print(player.water);
  tft.print("%");

  footer();

  tft.drawFastHLine(40, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(40, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(120, 295, 25, TFT_LIGHTGREY);
}

void secondPage()
{
  timeAndDate();

  tft.setCursor(200,45);
  tft.print(suit.suitName);
  
  tft.drawRoundRect(10,87,210,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(10,87,210,30,5,TFT_LIGHTGREY);
  tft.setCursor(40,90);
  tft.print("Thermal ");
  tft.print(suit.thermal);
  tft.print("%");

  tft.drawRoundRect(10,137,210,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(10,137,210,30,5,TFT_LIGHTGREY);
  tft.setCursor(40,140);
  tft.print("Gravity ");
  tft.print(suit.gravitational);
  tft.print("%");

  tft.drawRoundRect(260,87,210,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(260,87,210,30,5,TFT_LIGHTGREY);
  tft.setCursor(290,90);
  tft.print("Electrical ");
  tft.print(suit.electrical);
  tft.print("%");

  tft.drawRoundRect(260,137,210,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(260,137,210,30,5,TFT_LIGHTGREY);
  tft.setCursor(290,140);
  tft.print("Chemical ");
  tft.print(suit.chemical);
  tft.print("%");

  tft.drawRoundRect(40,187,400,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(40,187,400,30,5,TFT_LIGHTGREY);
  tft.setCursor(160,190);
  tft.print("Radiation ");
  tft.print(suit.radiation);
  tft.print("%");

  tft.drawRoundRect(40,237,400,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(40,237,400,30,5,TFT_LIGHTGREY);
  tft.setCursor(160,240);
  tft.print("Psycho ");
  tft.print(suit.psycho);
  tft.print("%");

  footer();

  tft.drawFastHLine(200, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(200, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(280, 295, 25, TFT_LIGHTGREY);
}

void thirdPageOne()
{
  timeAndDate();

  tft.setCursor(150,45);
  tft.print("Reducing Drugs");

  tft.setCursor(5,80);
  tft.print("Bint");
  tft.setCursor(440,80);
  tft.print(reducingDrugs._1);

  tft.setCursor(5,110);
  tft.print("Aptechka");
  tft.setCursor(440,110);
  tft.print(reducingDrugs._2);

  tft.setCursor(5,140);
  tft.print("Armeyskaya aptechka");
  tft.setCursor(440,140);
  tft.print(reducingDrugs._3);

  tft.setCursor(5,170);
  tft.print("Naychnaya aptechka");
  tft.setCursor(440,170);
  tft.print(reducingDrugs._4);

  tft.setCursor(5,200);
  tft.print("Protivoradiacionniye preparaty");
  tft.setCursor(440,200);
  tft.print(reducingDrugs._5);

  tft.setCursor(5,230);
  tft.print("Barvinok");
  tft.setCursor(440,230);
  tft.print(reducingDrugs._6);

  inventoryMenu();

  tft.drawFastVLine(20, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(59, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(20, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageTwo()
{
  timeAndDate();

  tft.setCursor(150,45);
  tft.print("Protecting Drugs");

  tft.setCursor(5,80);
  tft.print("Psi-blokada");
  tft.setCursor(440,80);
  tft.print(protectingDrugs._1);

  tft.setCursor(5,110);
  tft.print("Radioprotektor");
  tft.setCursor(440,110);
  tft.print(protectingDrugs._2);

  tft.setCursor(5,140);
  tft.print("Antidot");
  tft.setCursor(440,140);
  tft.print(protectingDrugs._3);

  tft.setCursor(5,170);
  tft.print("Anabiotik");
  tft.setCursor(440,170);
  tft.print(protectingDrugs._4);

  inventoryMenu();

  tft.drawFastVLine(100, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(139, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(100, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageThree()
{
  timeAndDate();

  tft.setCursor(150,45);
  tft.print("Repair");

  tft.setCursor(5,80);
  tft.print("Zaplatka");
  tft.setCursor(440,80);
  tft.print(repair._1);

  tft.setCursor(5,110);
  tft.print("Karmanny remnabor");
  tft.setCursor(440,110);
  tft.print(repair._2);

  tft.setCursor(5,140);
  tft.print("Poxodnyi remnabor");
  tft.setCursor(440,140);
  tft.print(repair._3);

  tft.setCursor(5,170);
  tft.print("Professionalniy remnabor");
  tft.setCursor(440,170);
  tft.print(repair._4);

  inventoryMenu();

  tft.drawFastVLine(180, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(219, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(180, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageFour()
{
  timeAndDate();

  tft.setCursor(150,45);
  tft.print("Eda i Napitki");

  tft.setCursor(5,80);
  tft.print("Hleb");
  tft.setCursor(440,80);
  tft.print(foodAndDrinks._1);

  tft.setCursor(5,110);
  tft.print("Kolbasa");
  tft.setCursor(440,110);
  tft.print(foodAndDrinks._2);

  tft.setCursor(5,140);
  tft.print("Konserva");
  tft.setCursor(440,140);
  tft.print(foodAndDrinks._3);

  tft.setCursor(5,170);
  tft.print("Vodka Kazaki");
  tft.setCursor(440,170);
  tft.print(foodAndDrinks._4);

  tft.setCursor(5,200);
  tft.print("Energeticheeskiy napitok");
  tft.setCursor(440,200);
  tft.print(foodAndDrinks._5);

  tft.setCursor(5,230);
  tft.print("Chistaya voda");
  tft.setCursor(440,230);
  tft.print(foodAndDrinks._6);

  inventoryMenu();

  tft.drawFastVLine(260, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(299, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(260, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageFiveOne()
{
  timeAndDate();

  tft.setCursor(170,45);
  tft.print("Artifacts");

  tft.setCursor(5,80);
  tft.print("artifact_1");
  tft.setCursor(440,80);
  tft.print(artifacts._1);

  tft.setCursor(5,110);
  tft.print("artifact_2");
  tft.setCursor(440,110);
  tft.print(artifacts._2);

  tft.setCursor(5,140);
  tft.print("artifact_3");
  tft.setCursor(440,140);
  tft.print(artifacts._3);

  tft.setCursor(5,170);
  tft.print("artifact_4");
  tft.setCursor(440,170);
  tft.print(artifacts._4);

  tft.setCursor(5,200);
  tft.print("artifact_5");
  tft.setCursor(440,200);
  tft.print(artifacts._5);

  tft.fillTriangle (228,230,244,230,236,241,TFT_LIGHTGREY);

  inventoryMenu();

  tft.drawFastVLine(340, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(379, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(340, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageFiveTwo()
{
  timeAndDate();

  tft.fillTriangle (228,65,244,65,236,54,TFT_LIGHTGREY);

  tft.setCursor(5,80);
  tft.print("artifact_6");
  tft.setCursor(440,80);
  tft.print(artifacts._6);

  tft.setCursor(5,110);
  tft.print("artifact_7");
  tft.setCursor(440,110);
  tft.print(artifacts._7);

  tft.setCursor(5,140);
  tft.print("artifact_8");
  tft.setCursor(440,140);
  tft.print(artifacts._8);

  tft.setCursor(5,170);
  tft.print("artifact_9");
  tft.setCursor(440,170);
  tft.print(artifacts._9);

  tft.setCursor(5,200);
  tft.print("artifact_10");
  tft.setCursor(440,200);
  tft.print(artifacts._10);

  tft.fillTriangle (228,230,244,230,236,241,TFT_LIGHTGREY);

  inventoryMenu();

  tft.drawFastVLine(340, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(379, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(340, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageFiveThree()
{
  timeAndDate();

  tft.fillTriangle (228,65,244,65,236,54,TFT_LIGHTGREY);

  tft.setCursor(5,80);
  tft.print("artifact_11");
  tft.setCursor(440,80);
  tft.print(artifacts._11);

  tft.setCursor(5,110);
  tft.print("artifact_12");
  tft.setCursor(440,110);
  tft.print(artifacts._12);

  tft.setCursor(5,140);
  tft.print("artifact_13");
  tft.setCursor(440,140);
  tft.print(artifacts._13);

  tft.setCursor(5,170);
  tft.print("artifact_14");
  tft.setCursor(440,170);
  tft.print(artifacts._14);

  tft.setCursor(5,200);
  tft.print("artifact_15");
  tft.setCursor(440,200);
  tft.print(artifacts._15);

  tft.fillTriangle (228,230,244,230,236,241,TFT_LIGHTGREY);

  inventoryMenu();

  tft.drawFastVLine(340, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(379, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(340, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageFiveFour()
{
  timeAndDate();

  tft.fillTriangle (228,65,244,65,236,54,TFT_LIGHTGREY);

  tft.setCursor(5,80);
  tft.print("artifact_16");
  tft.setCursor(440,80);
  tft.print(artifacts._16);

  tft.setCursor(5,110);
  tft.print("artifact_17");
  tft.setCursor(440,110);
  tft.print(artifacts._17);

  tft.setCursor(5,140);
  tft.print("artifact_18");
  tft.setCursor(440,140);
  tft.print(artifacts._18);

  tft.setCursor(5,170);
  tft.print("artifact_19");
  tft.setCursor(440,170);
  tft.print(artifacts._19);

  tft.setCursor(5,200);
  tft.print("artifact_20");
  tft.setCursor(440,200);
  tft.print(artifacts._20);

  tft.fillTriangle (228,230,244,230,236,241,TFT_LIGHTGREY);

  inventoryMenu();

  tft.drawFastVLine(340, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(379, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(340, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageFiveFive()
{
  timeAndDate();

  tft.fillTriangle (228,65,244,65,236,54,TFT_LIGHTGREY);

  tft.setCursor(5,80);
  tft.print("artifact_21");
  tft.setCursor(440,80);
  tft.print(artifacts._21);

  tft.setCursor(5,110);
  tft.print("artifact_22");
  tft.setCursor(440,110);
  tft.print(artifacts._22);

  tft.setCursor(5,140);
  tft.print("artifact_23");
  tft.setCursor(440,140);
  tft.print(artifacts._23);

  tft.setCursor(5,170);
  tft.print("artifact_24");
  tft.setCursor(440,170);
  tft.print(artifacts._24);

  tft.setCursor(5,200);
  tft.print("artifact_25");
  tft.setCursor(440,200);
  tft.print(artifacts._25);

  tft.fillTriangle (228,230,244,230,236,241,TFT_LIGHTGREY);

  inventoryMenu();

  tft.drawFastVLine(340, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(379, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(340, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageFiveSix()
{
  timeAndDate();

  tft.fillTriangle (228,65,244,65,236,54,TFT_LIGHTGREY);

  tft.setCursor(5,80);
  tft.print("artifact_26");
  tft.setCursor(440,80);
  tft.print(artifacts._26);

  tft.setCursor(5,110);
  tft.print("artifact_27");
  tft.setCursor(440,110);
  tft.print(artifacts._27);

  tft.setCursor(5,140);
  tft.print("artifact_28");
  tft.setCursor(440,140);
  tft.print(artifacts._28);

  tft.setCursor(5,170);
  tft.print("artifact_29");
  tft.setCursor(440,170);
  tft.print(artifacts._29);

  tft.setCursor(5,200);
  tft.print("artifact_30");
  tft.setCursor(440,200);
  tft.print(artifacts._30);

  inventoryMenu();

  tft.drawFastVLine(340, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(379, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(340, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
}

void thirdPageSix()
{
  timeAndDate();

  tft.setCursor(170,45);
  tft.print("Receiving mode");

  inventoryMenu();

  tft.drawFastVLine(420, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(459, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(420, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);
  
}

void chooseMarkerOne()
{
  tft.drawFastHLine(425, 80, 10, TFT_LIGHTGREY); //скобочки выбора право верх
  tft.drawFastHLine(460, 80, 10, TFT_LIGHTGREY); //скобочки выбора лево верх  
  tft.drawFastHLine(425, 100, 10, TFT_LIGHTGREY); //скобочки выбора право низ
  tft.drawFastHLine(460, 100, 10, TFT_LIGHTGREY); //скобочки выбора лево низ
  tft.drawFastVLine(425, 80, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные лево
  tft.drawFastVLine(469, 80, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные право
}

void chooseMarkerTwo()
{
  tft.drawFastHLine(425, 110, 10, TFT_LIGHTGREY); //скобочки выбора право верх
  tft.drawFastHLine(460, 110, 10, TFT_LIGHTGREY); //скобочки выбора лево верх  
  tft.drawFastHLine(425, 130, 10, TFT_LIGHTGREY); //скобочки выбора право низ
  tft.drawFastHLine(460, 130, 10, TFT_LIGHTGREY); //скобочки выбора лево низ
  tft.drawFastVLine(425, 110, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные лево
  tft.drawFastVLine(469, 110, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные право
}

void chooseMarkerThree()
{
  tft.drawFastHLine(425, 140, 10, TFT_LIGHTGREY); //скобочки выбора право верх
  tft.drawFastHLine(460, 140, 10, TFT_LIGHTGREY); //скобочки выбора лево верх  
  tft.drawFastHLine(425, 160, 10, TFT_LIGHTGREY); //скобочки выбора право низ
  tft.drawFastHLine(460, 160, 10, TFT_LIGHTGREY); //скобочки выбора лево низ
  tft.drawFastVLine(425, 140, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные лево
  tft.drawFastVLine(469, 140, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные право
}

void chooseMarkerFour()
{
  tft.drawFastHLine(425, 170, 10, TFT_LIGHTGREY); //скобочки выбора право верх
  tft.drawFastHLine(460, 170, 10, TFT_LIGHTGREY); //скобочки выбора лево верх  
  tft.drawFastHLine(425, 190, 10, TFT_LIGHTGREY); //скобочки выбора право низ
  tft.drawFastHLine(460, 190, 10, TFT_LIGHTGREY); //скобочки выбора лево низ
  tft.drawFastVLine(425, 170, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные лево
  tft.drawFastVLine(469, 170, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные право
}

void chooseMarkerFive()
{
  tft.drawFastHLine(425, 200, 10, TFT_LIGHTGREY); //скобочки выбора право верх
  tft.drawFastHLine(460, 200, 10, TFT_LIGHTGREY); //скобочки выбора лево верх  
  tft.drawFastHLine(425, 220, 10, TFT_LIGHTGREY); //скобочки выбора право низ
  tft.drawFastHLine(460, 220, 10, TFT_LIGHTGREY); //скобочки выбора лево низ
  tft.drawFastVLine(425, 200, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные лево
  tft.drawFastVLine(469, 200, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные право
}

void chooseMarkerSix()
{
  tft.drawFastHLine(425, 230, 10, TFT_LIGHTGREY); //скобочки выбора право верх
  tft.drawFastHLine(460, 230, 10, TFT_LIGHTGREY); //скобочки выбора лево верх  
  tft.drawFastHLine(425, 250, 10, TFT_LIGHTGREY); //скобочки выбора право низ
  tft.drawFastHLine(460, 250, 10, TFT_LIGHTGREY); //скобочки выбора лево низ
  tft.drawFastVLine(425, 230, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные лево
  tft.drawFastVLine(469, 230, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные право
}

void print2digits(int number) 
{
 if (number >= 0 && number < 10) 
 {
  tft.write('0');
 }
 tft.print(number);
}

bool getTime(const char *str)
{
 int Hour, Min, Sec;
 
 if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
 tm.Hour = Hour;
 tm.Minute = Min;
 tm.Second = Sec;
 return true;
}
 
bool getDate(const char *str)
{
 char Month[12];
 int Day, Year;
 uint8_t monthIndex;
 
 if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
 if (monthIndex >= 12) return false;
 tm.Day = Day;
 tm.Month = monthIndex + 1;
 tm.Year = CalendarYrToTm(Year);
 return true;
}

void rtcSetup()
{
  // получаем дату и время на момент компиляции
  if (getDate(__DATE__) && getTime(__TIME__)) 
 {
  parse = true;
  // и конфигурируем RTC используя эту информацию
  if (RTC.write(tm)) 
  {
    config = true;
  }
  }
}

void radiationDamage()
{
  if(player.radiation > 1000000)
  {
    player.health = 0;
    EEPROM.write(0,player.health);
  }else if(player.radiation > 750000)
  {
    radiationHitTime++;
    if(radiationHitTime == 30)
    {
      radiationHitTime = 0;
      player.health -= 2;
      EEPROM.write(0,player.health);
    }
  }else if(player.radiation > 500000)
  {
    radiationHitTime++;
    if(radiationHitTime == 30)
    {
      radiationHitTime = 0;
      player.health -= 1;
      EEPROM.write(0,player.health);
    }
  }else if(player.radiation > 250000)
  {
    radiationHitTime++;
    if(radiationHitTime == 30)
    {
      radiationHitTime = 0;
      player.health -= 0.5;
      EEPROM.write(0,player.health);
    }
  }
}

void foodDamage()
{
  if(player.food==0)
  {
    foodHitTime++;
    if(foodHitTime == 60)
    {
      foodHitTime=0;
      player.health -= 5;
      EEPROM.write(0,player.health);
    }
  }else if(player.food < 25)
  {
    foodHitTime++;
    if(foodHitTime == 60)
    {
      foodHitTime = 0;
      player.health -= 2.5;
      EEPROM.write(0,player.health);
    }
  }else if(player.food < 50)
  {
    foodHitTime++;
    if(foodHitTime == 60)
    {
      foodHitTime = 0;
      player.health -= 1.5;
      EEPROM.write(0,player.health);
    }
  }else if(player.food < 75)
  {
    foodHitTime++;
    if(foodHitTime == 60)
    {
      foodHitTime = 0;
      player.health -= 0.5;
      EEPROM.write(0,player.health);
    }
  }
}

void waterDamage()
{
  if(player.water==0)
  {
    waterHitTime++;
    if(waterHitTime == 60)
    {
      waterHitTime=0;
      player.health -= 5;
      EEPROM.write(0,player.health);
    }
  }else if(player.water < 25)
  {
    waterHitTime++;
    if(waterHitTime == 60)
    {
      waterHitTime = 0;
      player.health -= 2.5;
      EEPROM.write(0,player.health);
    }
  }else if(player.water < 50)
  {
    waterHitTime++;
    if(waterHitTime == 60)
    {
      waterHitTime = 0;
      player.health -= 1.5;
      EEPROM.write(0,player.health);
    }
  }else if(player.water < 75)
  {
    waterHitTime++;
    if(waterHitTime == 60)
    {
      waterHitTime = 0;
      player.health -= 0.5;
      EEPROM.write(0,player.health);
    }
  }
}

void playerHunger()
{
  if(player.food>0)
  {
     hunger++;
     if(hunger == 60)
     {
       hunger=0;
       player.food-=1;
       EEPROM.write(2,player.food);
     }
   }
}

void playerDehydrating()
{
  if(player.water>0)
  {
     dehydrating++;
     if(dehydrating ==60)
     {
       dehydrating=0;
       player.water-=1;
       EEPROM.write(3,player.water);
     }
   }
}

void healthRegeneration()
{
  if(player.health > 0 && player.health < 100)
  {
    healthRegenerationTime++;
    if(healthRegenerationTime == 60)
    {
      healthRegenerationTime = 0;
      player.health += 0.5;
      EEPROM.write(0,player.health);
    }
  }
}

void radiationRegeneration()
{
  if(player.radiation > 0 && player.radiation < 1000000)
  {
    radiationRegenerationTime++;
    if(radiationRegenerationTime == 60)
    {
      radiationRegenerationTime = 0;
      player.radiation -= 1000;
      EEPROM.write(1,player.radiation*100/1000000);
    }
  }
}

void suitAndHealthHit()
{
  if(75<suit.integrity)
  {
    suit.integrity = suit.integrity - (anomalyTemp/5);
    EEPROM.write(5,suit.integrity);
    x = 0; //возврат на первую страницу
  }else if(50<suit.integrity)
  {
    player.health = player.health - (anomalyTemp-protectionTemp);
    EEPROM.write(0,player.health);
    suit.integrity = suit.integrity - (anomalyTemp/5);
    EEPROM.write(5,suit.integrity);
    x = 0; //возврат на первую страницу
  }else if(25 < suit.integrity)
  {
    player.health = player.health - (anomalyTemp-protectionTemp/1.5);
    EEPROM.write(0,player.health);
    suit.integrity = suit.integrity - (anomalyTemp/5);
    EEPROM.write(5,suit.integrity);
    x = 0; //возврат на первую страницу
  }else if (0 < suit.integrity)
  {
    player.health = player.health - (anomalyTemp-protectionTemp/2);
    EEPROM.write(0,player.health);
    suit.integrity = suit.integrity - (anomalyTemp/5);
    EEPROM.write(5,suit.integrity);
    x = 0; //возврат на первую страницу
  }else if (suit.integrity == 0)
  {
    player.health = player.health - (anomalyTemp);
    EEPROM.write(0,player.health);
    suit.integrity = 0;
    EEPROM.write(5,suit.integrity);
    x = 0; //возврат на первую страницу
  }
}

void radiationHit()
{
  if(75 < suit.integrity)
  {
    player.radiation = player.radiation + (anomalyTemp-protectionTemp);
    EEPROM.write(1,player.radiation*100/1000000);
    x = 0; //возврат на первую страницу
  }else if(50 < suit.integrity)
  {
    player.radiation = player.radiation + (anomalyTemp-protectionTemp/1.25);
    EEPROM.write(1,player.radiation*100/1000000);
    x = 0; //возврат на первую страницу
  }else if(25 < suit.integrity)
  {
    player.radiation = player.radiation - (anomalyTemp-protectionTemp/1.5);
    EEPROM.write(1,player.radiation*100/1000000);
    x = 0; //возврат на первую страницу
  }else if (0 < suit.integrity)
  {
    player.radiation = player.radiation - (anomalyTemp-protectionTemp/2);
    EEPROM.write(1,player.radiation*100/1000000);
    x = 0; //возврат на первую страницу
  }else if (suit.integrity == 0)
  {
    player.radiation = player.radiation - (anomalyTemp);
    EEPROM.write(1,player.radiation*100/1000000);
    suit.integrity = 0;
    EEPROM.write(5,suit.integrity);
    x = 0; //возврат на первую страницу
  }
}

void radioLoop()
{
  if(player.health == 0)
  {
    tft.fillScreen(TFT_BLACK);
    
    while(player.health == 0)
    {
      tft.setCursor(170,140);
      tft.print("YOU DIED ^^"); 
    }
  }

  if(player.health>100)
  {
    player.health=100;
    EEPROM.write(0,player.health);
  }

  if(suit.integrity>100)
  {
    suit.integrity=100;
    EEPROM.write(5,suit.integrity);
  }

  if(player.food>100)
  {
    player.food=100;
    EEPROM.write(2,player.food);
  }

  if(player.water>100)
  {
    player.water=100;
    EEPROM.write(3,player.water);
  }

  if(suit.blowout>100)
  {
    suit.blowout=100;
    EEPROM.write(12,suit.blowout);
  }

  if(suit.chemical>100)
  {
    suit.chemical=100;
    EEPROM.write(7,suit.chemical);
  }

  if(suit.radiation>100)
  {
    suit.radiation=100;
    EEPROM.write(11,suit.radiation);
  }

  if(suit.psycho>100)
  {
    suit.psycho=100;
    EEPROM.write(10,suit.psycho);
  }

  if(player.health<0)
  {
    player.health=0;
    EEPROM.write(0,player.health);
  }

  if(suit.integrity<0)
  {
    suit.integrity=0;
    EEPROM.write(5,suit.integrity);
  }

  if(player.radiation<0)
  {
    player.radiation=0;
    EEPROM.write(1,player.radiation*100/1000000);
  }

  if(player.food<0)
  {
    player.food=0;
    EEPROM.write(2,player.food);
  }

  if(player.water<0)
  {
    player.water=0;
    EEPROM.write(3,player.water);
  }

  if(suit.blowout<0)
  {
    suit.blowout=0;
    EEPROM.write(12,suit.blowout);
  }

  if(suit.chemical<0)
  {
    suit.chemical=0;
    EEPROM.write(7,suit.chemical);
  }

  if(suit.radiation<0)
  {
    suit.radiation=0;
    EEPROM.write(11,suit.radiation);
  }

  if(suit.psycho<0)
  {
    suit.psycho=0;
    EEPROM.write(10,suit.psycho);
  }

  if(radio.available()&&player.health!=0)
  {
    radio.read(&anomalyTemp,4);

    if(anomalyTemp>1000&&anomalyTemp<2000) //1000-2000 термо
    {
      anomalyTemp -= 1000;

      protectionTemp = anomalyTemp*suit.thermal/100;
      
      suitAndHealthHit();
    }
    if(anomalyTemp>2000&&anomalyTemp<3000) //2000-3000 электро
    {
      anomalyTemp -= 2000;

      protectionTemp = anomalyTemp*suit.electrical/100;
      
      suitAndHealthHit();
    }
    if(anomalyTemp>3000&&anomalyTemp<4000) //3000-4000 грави
    {
      anomalyTemp -= 3000;

      protectionTemp = anomalyTemp*suit.gravitational/100;
      
      suitAndHealthHit();
    }
    if(anomalyTemp>4000&&anomalyTemp<5000) //4000-5000 химия
    {
      anomalyTemp -= 4000;

      protectionTemp = anomalyTemp*suit.chemical/100;
      
      suitAndHealthHit();
    }
    if(anomalyTemp>5000&&anomalyTemp<6000) //5000-6000 пси
    {
      anomalyTemp -= 5000;

      protectionTemp = anomalyTemp*suit.psycho/100;
      
      suitAndHealthHit();
    }
    if(anomalyTemp>10000&&anomalyTemp<20000) //10000-20000 радиация
    {
      anomalyTemp -= 10000;

      protectionTemp = anomalyTemp*suit.radiation/100;
      
      radiationHit();
    }
  }
}

void radioReceiving()
{
  if(radio.available())
  {
    radio.read(&transferResolution,4);
    
    if(transferResolution>500000)
    {
    transferId=transferResolution/1000000;
    if(id==transferId)
    {
      transferItem=transferResolution%100000/1000;
      transferQuantity=transferResolution%1000;

      if(transferItem==1)
      {
        reducingDrugs._1+=transferQuantity;
        x=2;
      }

      if(transferItem==2)
      {
        reducingDrugs._2+=transferQuantity;
        x=2;
      }

      if(transferItem==3)
      {
        reducingDrugs._3+=transferQuantity;
        x=2;
      }

      if(transferItem==4)
      {
        reducingDrugs._4+=transferQuantity;
        x=2;
      }

      if(transferItem==5)
      {
        reducingDrugs._5+=transferQuantity;
        x=2;
      }

      if(transferItem==6)
      {
        reducingDrugs._6+=transferQuantity;
        x=2;
      }

      if(transferItem==7)
      {
        protectingDrugs._1+=transferQuantity;
        x=3;
      }

      if(transferItem==8)
      {
        protectingDrugs._2+=transferQuantity;
        x=3;
      }

      if(transferItem==9)
      {
        protectingDrugs._3+=transferQuantity;
        x=3;
      }

      if(transferItem==10)
      {
        protectingDrugs._4+=transferQuantity;
        x=3;
      }

      if(transferItem==11)
      {
        repair._1+=transferQuantity;
        x=4;
      }

      if(transferItem==12)
      {
        repair._2+=transferQuantity;
        x=4;
      }

      if(transferItem==13)
      {
        repair._3+=transferQuantity;
        x=4;
      }

      if(transferItem==14)
      {
        repair._4+=transferQuantity;
        x=4;
      }

      if(transferItem==15)
      {
        foodAndDrinks._1+=transferQuantity;
        x=5;
      }
      
      if(transferItem==16)
      {
        foodAndDrinks._2+=transferQuantity;
        x=5;
      }

      if(transferItem==17)
      {
        foodAndDrinks._3+=transferQuantity;
        x=5;
      }

      if(transferItem==18)
      {
        foodAndDrinks._4+=transferQuantity;
        x=5;
      }

      if(transferItem==19)
      {
        foodAndDrinks._5+=transferQuantity;
        x=5;
      }

      if(transferItem==20)
      {
        foodAndDrinks._6+=transferQuantity;
        x=5;
      }
      
    }    
  }
  }else{
    radioLoop();
  }
}

void writeEEPROM()
{
  EEPROM.write(0,player.health);
  EEPROM.write(1,player.radiation*100/1000000);
  EEPROM.write(2,player.food);
  EEPROM.write(3,player.water);
  
  //EEPROM.write(4,suit.suitName);
  EEPROM.write(5,suit.integrity);
  EEPROM.write(6,suit.thermal);
  EEPROM.write(7,suit.chemical);
  EEPROM.write(8,suit.electrical);
  EEPROM.write(9,suit.gravitational);
  EEPROM.write(10,suit.psycho);
  EEPROM.write(11,suit.radiation);
  EEPROM.write(12,suit.blowout);

  EEPROM.write(13,reducingDrugs._1);
  EEPROM.write(14,reducingDrugs._2);
  EEPROM.write(15,reducingDrugs._3);
  EEPROM.write(16,reducingDrugs._4);
  EEPROM.write(17,reducingDrugs._5);
  EEPROM.write(18,reducingDrugs._6);

  EEPROM.write(19,protectingDrugs._1);
  EEPROM.write(20,protectingDrugs._2);
  EEPROM.write(21,protectingDrugs._3);
  EEPROM.write(22,protectingDrugs._4);

  EEPROM.write(23,repair._1);
  EEPROM.write(24,repair._2);
  EEPROM.write(25,repair._3);
  EEPROM.write(26,repair._4);

  EEPROM.write(27,foodAndDrinks._1);
  EEPROM.write(28,foodAndDrinks._2);
  EEPROM.write(29,foodAndDrinks._3);
  EEPROM.write(30,foodAndDrinks._4);
  EEPROM.write(31,foodAndDrinks._5);
  EEPROM.write(32,foodAndDrinks._6);
}

void readEEPROM()
{
  player.health = EEPROM.read(0);
  player.radiation = EEPROM.read(1);
  player.radiation = player.radiation*1000000/100;
  player.food = EEPROM.read(2);
  player.water = EEPROM.read(3);
  
  //suit.suitName = EEPROM.read(4);
  suit.integrity = EEPROM.read(5);
  suit.thermal = EEPROM.read(6);
  suit.chemical = EEPROM.read(7);
  suit.electrical = EEPROM.read(8);
  suit.gravitational = EEPROM.read(9);
  suit.psycho = EEPROM.read(10);
  suit.radiation = EEPROM.read(11);
  suit.blowout = EEPROM.read(12);

  reducingDrugs._1 = EEPROM.read(13);
  reducingDrugs._2 = EEPROM.read(14);
  reducingDrugs._3 = EEPROM.read(15);
  reducingDrugs._4 = EEPROM.read(16);
  reducingDrugs._5 = EEPROM.read(17);
  reducingDrugs._6 = EEPROM.read(18);

  protectingDrugs._1 = EEPROM.read(19);
  protectingDrugs._2 = EEPROM.read(20);
  protectingDrugs._3 = EEPROM.read(21);
  protectingDrugs._4 = EEPROM.read(22);

  repair._1 = EEPROM.read(23);
  repair._2 = EEPROM.read(24);
  repair._3 = EEPROM.read(25);
  repair._4 = EEPROM.read(26);

  foodAndDrinks._1 = EEPROM.read(27);
  foodAndDrinks._2 = EEPROM.read(28);
  foodAndDrinks._3 = EEPROM.read(29);
  foodAndDrinks._4 = EEPROM.read(30);
  foodAndDrinks._5 = EEPROM.read(31);
  foodAndDrinks._6 = EEPROM.read(32);

  hourTempPsycho = EEPROM.read(33);
  minuteTempPsycho = EEPROM.read(34);
  suitTempPsycho = EEPROM.read(35);

  hourTempRadiation = EEPROM.read(36);
  minuteTempRadiation = EEPROM.read(37);
  suitTempRadiation = EEPROM.read(38);

  hourTempChemical = EEPROM.read(39);
  minuteTempChemical = EEPROM.read(40);
  suitTempChemical = EEPROM.read(41);

  hourTempBlowout = EEPROM.read(42);
  minuteTempBlowout = EEPROM.read(43);
  suitTempBlowout = EEPROM.read(44);

  hourTempEnergetik = EEPROM.read(45);
  minuteTempEnergetik = EEPROM.read(46);
}
