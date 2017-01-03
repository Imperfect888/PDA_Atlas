//**************************************PROJECT PDA ATLAS**************************************
#include <SPI.h> //подключаем библиотеку
#include <RF24.h> //подключаем библиотеку
#include <TFT_HX8357.h> //подключаем библиотеку
#include <User_Setup.h> //подключаем библиотеку

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
//**************************************СТРУКТУРЫ**************************************
struct player
  {
    float health = 100;
    float radiation = 0;
    unsigned short int radiationActivity = 0;
    unsigned short int anomalyActivity = 0;
  };

struct suit
  {
    String suitName = "Zarya";
    float integrity = 100;
    unsigned short int thermal = 25;
    unsigned short int chemical = 50;
    unsigned short int electrical = 75;
    unsigned short int gravitational = 0;
    unsigned short int psycho = 50;
    unsigned short int radiation = 25;
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
};

struct player player; //объявление структуры
struct suit suit; //объявление структуры
struct reducingDrugs reducingDrugs; //объявление структуры
struct protectingDrugs protectingDrugs; //объявление структуры
struct repair repair; //объявление структуры
struct foodAndDrinks foodAndDrinks; //объявление структуры

//**************************************ПЕРЕМЕННЫЕ**************************************
float anomalyTemp = 0;
float protectionTemp = 0; //процент защиты
unsigned short int radiationHitTime = 0; //таймер удара радиации
unsigned short int healthRegenerationTime = 0; //таймер регенерации здоровья
unsigned short int radiationRegenerationTime = 0; //таймер регенерации радиации
//*************************************************************************************
void setup() 
{
  displaySetup(); //настройки дисплея

  radioSetup(); //настройки радиомодуля
}

void loop() 
{
  positionXLoop();

  switch(x)
  {
    case 0:
    tft.fillScreen(TFT_BLACK);//фон дисплея
    do
    {
      radiationDamage();
      
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
        break;
        
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerTwo();
        break;
          
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerThree();
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerFour();
        break;
      
        case 4:
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageOne();
        chooseMarkerFive();
        break;
      
        case 5:
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        thirdPageOne();
        chooseMarkerSix();
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
        break;
        
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageTwo();
        chooseMarkerTwo();
        break;
        
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageTwo();
        chooseMarkerThree();
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageTwo();
        chooseMarkerFour();
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
        break;
      
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageThree();
        chooseMarkerTwo();
        break;
       
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageThree();
        chooseMarkerThree();
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageThree();
        chooseMarkerFour();
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
        break;
 
       
        case 1:
        tft.fillRect(425,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,80,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerTwo();
        break;
       
        case 2:
        tft.fillRect(425,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,110,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerThree();
        break;
      
        case 3:
        tft.fillRect(425,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,140,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,200,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerFour();
        break;
      
        case 4:
        tft.fillRect(425,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(454,170,16,21,TFT_BLACK); //стирание маркера сверху-вниз
        tft.fillRect(425,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        tft.fillRect(454,230,16,21,TFT_BLACK); //стирание маркера снизу-вверх
        thirdPageFour();
        chooseMarkerFive();    
        break;    
      }
      
      radioLoop();
      
      delay(1000);
      positionX = analogRead(xPin);
    }while(positionX!=1023&&x==5);
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
  delay(2);
  radio.setChannel(9);
  radio.setDataRate(RF24_1MBPS); // RF24_1MBPS или RF24_2MBPS
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1,pipe); // открыть канал на приём
  radio.startListening(); // приём
}

void positionXLoop()
{
  positionX = analogRead(xPin);
  if(positionX == 0) x++;
  if(positionX == 1023) x--;
  if(x<0) x=0;
  if(x>5) x=5;
}

void positionYLoop()
{
  positionY = analogRead(yPin);
  if(positionY == 1023) y--;
  if(positionY == 0) y++;
  if(y<0) y=0;
  if(y>5) y=5;
}

void pdaVersion()
{
  tft.setCursor(5,5);//версия пда
  tft.print("PDA   ATLAS   v.1.0.7");//версия пда
  tft.drawFastHLine(0, 31, 480, TFT_LIGHTGREY);
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
  pdaVersion();
  
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
  tft.print("Radiation activity");
  tft.drawRoundRect(210,197,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,200,70,20,0,TFT_BLACK);
  tft.setCursor(315,200);
  tft.print(player.radiationActivity);
  tft.print("%");

  tft.setCursor(5,250);
  tft.print("Anomaly activity");
  tft.drawRoundRect(210,247,265,30,5,TFT_DARKGREY);
  tft.fillRoundRect(315,250,70,20,0,TFT_BLACK);
  tft.setCursor(315,250);
  tft.print(player.anomalyActivity);
  tft.print("%");

  footer();

  tft.drawFastHLine(40, 319, 80, TFT_LIGHTGREY);
  tft.drawFastVLine(40, 295, 25, TFT_LIGHTGREY);
  tft.drawFastVLine(120, 295, 25, TFT_LIGHTGREY);
}

void secondPage()
{
  pdaVersion();

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
  pdaVersion();

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
  pdaVersion();

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
  pdaVersion();

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
  pdaVersion();

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

  inventoryMenu();

  tft.drawFastVLine(260, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, левая вертикальная линия
  tft.drawFastVLine(299, 270, 23, TFT_LIGHTGREY); //прямоугольник выбора подменю, правая вертикальная линия
  tft.drawFastHLine(260, 292, 39, TFT_LIGHTGREY); //прямоугольник выбора подменю, горизонтальная линия

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

void radiationDamage()
{
  if(player.radiation > 1000000)
  {
    player.health = 0;
  }else if(player.radiation > 750000)
  {
    radiationHitTime++;
    if(radiationHitTime == 30)
    {
      radiationHitTime = 0;
      player.health -= 2;
    }
  }else if(player.radiation > 500000)
  {
    radiationHitTime++;
    if(radiationHitTime == 30)
    {
      radiationHitTime = 0;
      player.health -= 1;
    }
  }else if(player.radiation > 250000)
  {
    radiationHitTime++;
    if(radiationHitTime == 30)
    {
      radiationHitTime = 0;
      player.health -= 0.5;
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
      player.health += 0.25;
    }
  }
}

void radiationRegeneration()
{
  if(player.radiation > 0 && player.radiation < 1000000)
  {
    radiationRegenerationTime++;
    if(radiationRegenerationTime == 61)
    {
      radiationRegenerationTime = 0;
      player.radiation -= 1000;
    }
  }
}

void suitAndHealthHit()
{
  if(75<suit.integrity)
  {
    suit.integrity = suit.integrity - (anomalyTemp/5);
    x = 0; //возврат на первую страницу
  }else if(50<suit.integrity)
  {
    player.health = player.health - (anomalyTemp-protectionTemp);
    suit.integrity = suit.integrity - (anomalyTemp/5);
    x = 0; //возврат на первую страницу
  }else if(25 < suit.integrity)
  {
    player.health = player.health - (anomalyTemp-protectionTemp/1.5);
    suit.integrity = suit.integrity - (anomalyTemp/5);
    x = 0; //возврат на первую страницу
  }else if (0 < suit.integrity)
  {
    player.health = player.health - (anomalyTemp-protectionTemp/2);
    suit.integrity = suit.integrity - (anomalyTemp/5);
    x = 0; //возврат на первую страницу
  }else if (suit.integrity < 0 || suit.integrity == 0)
  {
    player.health = player.health - (anomalyTemp);
    suit.integrity = 0;
    x = 0; //возврат на первую страницу
  }
}

void radiationHit()
{
  if(75 < suit.integrity)
  {
    player.radiation = player.radiation + (anomalyTemp-protectionTemp);
    x = 0; //возврат на первую страницу
  }else if(50 < suit.integrity)
  {
    player.radiation = player.radiation + (anomalyTemp-protectionTemp/1.25);
    x = 0; //возврат на первую страницу
  }else if(25 < suit.integrity)
  {
    player.radiation = player.radiation - (anomalyTemp-protectionTemp/1.5);
    x = 0; //возврат на первую страницу
  }else if (0 < suit.integrity)
  {
    player.radiation = player.radiation - (anomalyTemp-protectionTemp/2);
    x = 0; //возврат на первую страницу
  }else if (suit.integrity < 0 || suit.integrity == 0)
  {
    player.radiation = player.radiation - (anomalyTemp);
    suit.integrity = 0;
    x = 0; //возврат на первую страницу
  }
}

void radioLoop()
{
  if(player.health == 0 || player.health < 0)
  {
    tft.fillScreen(TFT_BLACK);
    
    while(player.health == 0 || player.health < 0)
    {
      tft.setCursor(170,140);
      tft.print("YOU DIED ^^"); 
    }
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
