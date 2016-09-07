//**************************************PROJECT PDA ATLAS**************************************
#include <TFT_HX8357.h> //подключаем библиотеку
#include <User_Setup.h> //подключаем библиотеку

TFT_HX8357 tft = TFT_HX8357(); //конструктор пинов

//**************************************ДЖОЙСТИК**************************************
int xPin = A1;
int yPin = A0;
int buttonPin = 2;

int positionX = 0;
int positionY = 0;
int buttonState = 0;
//**************************************СТРУКТУРЫ**************************************
struct Player
{
  unsigned short int Health = 100;
  unsigned short int SuitIntegrity = 100;
  unsigned short int Radiation = 0;
  unsigned short int RadiationActivity = 0;
  unsigned short int AnomalyActivity = 0;
};

struct Suit
  {
    String SuitName = "Zarya";
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

void FirstPage()
{
  
  tft.setCursor(5,5);//версия пда
  tft.print("PDA v.1.0");//версия пда

  tft.setCursor(220,5);//время
  tft.print("00:00");//время

  tft.drawFastHLine(0, 31, 480, TFT_LIGHTGREY);
  
  tft.setCursor(5,50);
  tft.print("Health");
  tft.drawRoundRect(210,47,265,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(210,47,265,30,5,TFT_LIGHTGREY);
  tft.setCursor(315,50);
  tft.print(Player.Health);
  tft.print("%");

  tft.setCursor(5,100);
  tft.print("Suit");
  tft.drawRoundRect(210,97,265,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(210,97,265,30,5,TFT_LIGHTGREY);
  tft.setCursor(315,100);
  tft.print(Player.SuitIntegrity);
  tft.print("%");
  
  tft.setCursor(5,150);
  tft.print("Radiation");
  tft.drawRoundRect(210,147,265,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(210,147,265,30,5,TFT_LIGHTGREY);
  tft.setCursor(315,150);
  tft.print(Player.Radiation);
  tft.print("%");

  tft.setCursor(5,200);
  tft.print("Radiation activity");
  tft.drawRoundRect(210,197,265,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(210,197,265,30,5,TFT_LIGHTGREY);
  tft.setCursor(315,200);
  tft.print(Player.RadiationActivity);
  tft.print("%");

  tft.setCursor(5,250);
  tft.print("Anomaly activity");
  tft.drawRoundRect(210,247,265,30,5,TFT_DARKGREY);
  //tft.fillRoundRect(210,247,265,30,5,TFT_LIGHTGREY);
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
  
  tft.setCursor(5,5);//версия пда
  tft.print("PDA v.1.0");//версия пда

  tft.setCursor(220,5);//время
  tft.print("00:00");//время

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
    FirstPage();
    delay(1000);    
  }

  if(positionX==0)
  {
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      SecondPage();
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0&&positionX!=1023);
  }
  
  if(positionX==1023)
  {
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      FirstPage();
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0&&positionX!=1023);
  }
}
