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

struct ReducingDrugs
{
  unsigned short int Bint = 4;
  unsigned short int Aptechka = 2;
  unsigned short int ArmAptechka = 1;
  unsigned short int NaychAptechka = 1;
  unsigned short int Antirad = 2;
  unsigned short int Barvinok = 1;
};

struct Player Player; //объявление структуры
struct Suit Suit; //объявление структуры
struct ReducingDrugs ReducingDrugs; //объявление структуры
//*************************************************************************************


void setup() 
{
  GeneralDisplaySetup();

  JoystickSetup();

  RadioSetup();
}

void loop() 
{
  positionX = analogRead(xPin);    // Считываем аналоговое значение оси Х

  if(positionX==511||positionX==512||positionX==1023)
  {
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      if(AnomalyTemp==1)
      {
        AnomalyTemp=0;
      }
      FirstPage();
      RadioLoop();
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=0);
   
  }

  if(positionX==0)
  {
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      if(AnomalyTemp==1)
      {
        AnomalyTemp=0;
      }
      SecondPage();
      RadioLoop();
      delay(1000);
      positionX = analogRead(xPin);      
    }while(positionX!=0&&positionX!=1023&&AnomalyTemp!=1);
  }
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
  tft.print("PDA   Atlas   v.1.0.3");//версия пда
  tft.drawFastHLine(0, 31, 480, TFT_LIGHTGREY);
}

void Footer()
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

void InventoryMenu()
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

void FirstPage()
{
  PDAVersion();
  
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

  Footer();

  tft.drawFastHLine(40, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(40, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(120, 295, 25, TFT_LIGHTGREY);


}

void SecondPage()
{
  PDAVersion();

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

  Footer();

  tft.drawFastHLine(200, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(200, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(280, 295, 25, TFT_LIGHTGREY);

}

void ThirdPage1()
{
  PDAVersion();

  tft.setCursor(150,45);
  tft.print("Reducing drugs");

  tft.setCursor(5,80);
  tft.print("Bint");
  tft.setCursor(440,80);
  tft.print(ReducingDrugs.Bint);

  tft.setCursor(5,110);
  tft.print("Aptechka");
  tft.setCursor(440,110);
  tft.print(ReducingDrugs.Aptechka);

  tft.setCursor(5,140);
  tft.print("Armeyskaya aptechka");
  tft.setCursor(440,140);
  tft.print(ReducingDrugs.ArmAptechka);

  tft.setCursor(5,170);
  tft.print("Naychnaya aptechka");
  tft.setCursor(440,170);
  tft.print(ReducingDrugs.NaychAptechka);

  tft.setCursor(5,200);
  tft.print("Protivoradiacionniye preparaty");
  tft.setCursor(440,200);
  tft.print(ReducingDrugs.Antirad);

  tft.setCursor(5,230);
  tft.print("Barvinok");
  tft.setCursor(440,230);
  tft.print(ReducingDrugs.Barvinok);

  InventoryMenu();

  tft.drawFastHLine(425, 80, 10, TFT_LIGHTGREY); //скобочки выбора право верх
  tft.drawFastHLine(460, 80, 10, TFT_LIGHTGREY); //скобочки выбора лево верх  
  tft.drawFastHLine(425, 100, 10, TFT_LIGHTGREY); //скобочки выбора право низ
  tft.drawFastHLine(460, 100, 10, TFT_LIGHTGREY); //скобочки выбора лево низ
  tft.drawFastVLine(425, 80, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные лево
  tft.drawFastVLine(469, 80, 20, TFT_LIGHTGREY); //скобочки выбора вертикальные право

  tft.drawFastVLine(20, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(59, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(20, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

  Footer();

  tft.drawFastHLine(360, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(360, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(440, 295, 25, TFT_LIGHTGREY);


}

void JoystickSetup()
{
  Serial.begin(9600);
  pinMode(xPin, INPUT);  
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
      AnomalyTemp = 1;
    }else if(50 < Suit.Integrity)
    {
      Player.Health = Player.Health - (AnomalyTemp/5);
      Suit.Integrity = Suit.Integrity - (AnomalyTemp/5);
      AnomalyTemp = 1;
    }else if(25 < Suit.Integrity)
    {
      Player.Health = Player.Health - (AnomalyTemp/2);
      Suit.Integrity = Suit.Integrity - (AnomalyTemp/2);
      AnomalyTemp = 1;
    }else if (0 < Suit.Integrity)
    {
      Player.Health = Player.Health - (AnomalyTemp);
      Suit.Integrity = Suit.Integrity - (AnomalyTemp);
      AnomalyTemp = 1;
    }else if (Suit.Integrity < 0 || Suit.Integrity == 0)
    {
      Player.Health = Player.Health - (AnomalyTemp);
      Suit.Integrity = 0;
      AnomalyTemp = 1;
    }
  }
}
