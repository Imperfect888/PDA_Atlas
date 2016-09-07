//**************************************PROJECT PDA ATLAS**************************************
#include <SPI.h> //подключаем библиотеку
#include <RF24.h> //подключаем библиотеку
#include <TFT_HX8357.h> //подключаем библиотеку
#include <User_Setup.h> //подключаем библиотеку

TFT_HX8357 tft = TFT_HX8357(); //конструктор пинов

//***************************************РАДИО****************************************
RF24 radio(9, 53);
const uint32_t pipe = 123456789;
short int AnomalyTemp = 0;
//**************************************ДЖОЙСТИК**************************************
unsigned short int xPin = A1;
unsigned short int yPin = A0;
unsigned short int buttonPin = 2;

unsigned short int positionX = 0;
unsigned short int positionY = 0;
unsigned short int buttonState = 0;
//**************************************СТРУКТУРЫ**************************************
struct Player
  {
    short int Health = 100;
    unsigned short int Radiation = 0;
    unsigned short int RadiationActivity = 0;
    unsigned short int AnomalyActivity = 0;
  };

struct Suit
  {
    String SuitName = "Zarya";
    short int Integrity = 100;
    unsigned short int Thermal = 25;
    unsigned short int Chemical = 25;
    unsigned short int Electrical = 25;
    unsigned short int Gravitational = 25;
    unsigned short int Psycho = 10;
    unsigned short int Radiation = 15;
  };

struct Player Player; //объявление структуры
struct Suit Suit; //объявление структуры
//*************************************************************************************


void setup() 
{
  GeneralDisplaySetup();

  JoystickSetup();

  RadioSetup();
}

void loop() 
{
  JoystickSelect();
}


//**************************************ФУНКЦИИ**************************************

void GeneralDisplaySetup()
{
  tft.init();//инициализация дисплея
  tft.setRotation(1);//ориентация дисплея
  tft.fillScreen(TFT_BLACK);//фон дисплея
  tft.setTextFont(4);//типа шрифта дисплея
  tft.setTextColor(TFT_LIGHTGREY);//цвет шрифта дисплея
}

void PDAVersion()
{
  tft.setCursor(5,5);//версия пда
  tft.print("PDA   Atlas   v.1.0.2");//версия пда
}

void FirstPage()
{
  PDAVersion();
  
  tft.drawFastHLine(0, 31, 480, TFT_LIGHTGREY);
  
  tft.setCursor(5,50);
  tft.print("Health");
  tft.drawRoundRect(210,47,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,50,70,20,0,TFT_BLACK);
  tft.setCursor(315,50);
  tft.print(Player.Health);
  tft.print("%");

  tft.setCursor(5,100);
  tft.print("Suit");
  tft.drawRoundRect(210,97,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,100,70,20,0,TFT_BLACK);
  tft.setCursor(315,100);
  tft.print(Suit.Integrity);
  tft.print("%");
  
  tft.setCursor(5,150);
  tft.print("Radiation");
  tft.drawRoundRect(210,147,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,150,70,20,0,TFT_BLACK);
  tft.setCursor(315,150);
  tft.print(Player.Radiation);
  tft.print("%");

  tft.setCursor(5,200);
  tft.print("Radiation activity");
  tft.drawRoundRect(210,197,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,200,70,20,0,TFT_BLACK);
  tft.setCursor(315,200);
  tft.print(Player.RadiationActivity);
  tft.print("%");

  tft.setCursor(5,250);
  tft.print("Anomaly activity");
  tft.drawRoundRect(210,247,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,250,70,20,0,TFT_BLACK);
  tft.setCursor(315,250);
  tft.print(Player.AnomalyActivity);
  tft.print("%");

  tft.drawFastHLine(0, 295, 480, TFT_LIGHTGREY);
  tft.drawFastHLine(40, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(40, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(120, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(320, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(160, 295, 25, TFT_LIGHTGREY);

  tft.setCursor(73,297);
  tft.print("1");
  tft.setCursor(233,297);
  tft.print("2");
  tft.setCursor(393,297);
  tft.print("3");
}

void SecondPage()
{
  PDAVersion();

  tft.drawFastHLine(0, 31, 480, TFT_LIGHTGREY);

  tft.setCursor(200,45);
  tft.print(Suit.SuitName);
  
  tft.drawRoundRect(10,87,210,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(10,87,210,30,5,TFT_LIGHTGREY);
  tft.setCursor(40,90);
  tft.print("Thermal ");
  tft.print(Suit.Thermal);
  tft.print("%");

  tft.drawRoundRect(10,137,210,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(10,137,210,30,5,TFT_LIGHTGREY);
  tft.setCursor(40,140);
  tft.print("Gravity ");
  tft.print(Suit.Gravitational);
  tft.print("%");

  tft.drawRoundRect(260,87,210,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(260,87,210,30,5,TFT_LIGHTGREY);
  tft.setCursor(290,90);
  tft.print("Electrical ");
  tft.print(Suit.Electrical);
  tft.print("%");

  tft.drawRoundRect(260,137,210,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(260,137,210,30,5,TFT_LIGHTGREY);
  tft.setCursor(290,140);
  tft.print("Chemical ");
  tft.print(Suit.Chemical);
  tft.print("%");

  tft.drawRoundRect(40,187,400,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(40,187,400,30,5,TFT_LIGHTGREY);
  tft.setCursor(160,190);
  tft.print("Radiation ");
  tft.print(Suit.Radiation);
  tft.print("%");

  tft.drawRoundRect(40,237,400,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(40,237,400,30,5,TFT_LIGHTGREY);
  tft.setCursor(160,240);
  tft.print("Psycho ");
  tft.print(Suit.Psycho);
  tft.print("%");

  tft.drawFastHLine(0, 295, 480, TFT_LIGHTGREY);
  tft.drawFastHLine(200, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(200, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(280, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(320, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(160, 295, 25, TFT_LIGHTGREY);

  tft.setCursor(73,297);
  tft.print("1");
  tft.setCursor(233,297);
  tft.print("2");
  tft.setCursor(393,297);
  tft.print("3");
}

void JoystickSetup()
{
  Serial.begin(9600);
  pinMode(xPin, INPUT);  
}

void JoystickSelect()
{  
  positionX = analogRead(xPin);    // Считываем аналоговое значение оси Х

  if(positionX==511)
  {
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      FirstPage();
      RadioLoop();
      positionX = analogRead(xPin);
      delay(1000);
    }while(positionX!=0&&positionX!=1023);
   
  }

  if(positionX==0)
  {
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      SecondPage();
      RadioLoop();
      positionX = analogRead(xPin);
      delay(1000);
    }while(positionX!=0&&positionX!=1023&&!radio.available());
  }
  
  if(positionX==1023)
  {
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      FirstPage();
      RadioLoop();
      positionX = analogRead(xPin);
      delay(1000);
    }while(positionX!=0&&positionX!=1023);
  }
}

void RadioSetup()
{
  Serial.begin(9600);
  radio.begin();
  delay(2);
  radio.setChannel(9);
  radio.setDataRate(RF24_1MBPS); // RF24_1MBPS или RF24_2MBPS
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1,pipe); // открыть канал на приём
  radio.startListening(); // приём
}

void RadioLoop()
{
  if(Player.Health == 0 || Player.Health < 0)
  {
    tft.fillScreen(TFT_BLACK);
    while(Player.Health == 0 || Player.Health < 0)
    {
      tft.setCursor(170,140);
      tft.print("YOU DEAD ^^");
    }
  }

  if(radio.available()&&Player.Health!=0)
  {
    radio.read(&AnomalyTemp,1);

    if(75<Suit.Integrity)
    {
      Suit.Integrity = Suit.Integrity - (AnomalyTemp/10);
    }else if(50 < Suit.Integrity)
    {
      Player.Health = Player.Health - (AnomalyTemp/5);
      Suit.Integrity = Suit.Integrity - (AnomalyTemp/5);
    }else if(25 < Suit.Integrity)
    {
      Player.Health = Player.Health - (AnomalyTemp/2);
      Suit.Integrity = Suit.Integrity - (AnomalyTemp/2);
    }else if (0 < Suit.Integrity)
    {
      Player.Health = Player.Health - (AnomalyTemp);
      Suit.Integrity = Suit.Integrity - (AnomalyTemp);
    }else if (Suit.Integrity < 0 || Suit.Integrity == 0)
    {
      Player.Health = Player.Health - (AnomalyTemp);
      Suit.Integrity = 0;
    }
  }
}



























