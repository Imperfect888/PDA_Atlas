//**************************************PROJECT PDA ATLAS**************************************
#include <TFT_HX8357.h> //подключаем библиотеку
#include <User_Setup.h> //подключаем библиотеку

TFT_HX8357 tft = TFT_HX8357(); //конструктор пинов

//**************************************ВОЛЬТАЖ**************************************
// резисторы делителя напряжения
const float r1 = 99700;  // 100K
const float r2 =  9870;  // 10K
// эту константу (typVbg) необходимо откалибровать индивидуально
const float typVbg = 1.179; // 1.0 -- 1.2
//объявляем переменные для вольтажа
float Vcc = 0.0;
float MaxVoltage = 0.0;
#define A_PIN 1
#define COUNT 5
int i;
float curVoltage;
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

  BatteryPercentageSetup();

  JoystickSetup();
}

void loop() 
{
  JoystickSelect();            
}

//**************************************ФУНКЦИИ**************************************
float readVcc() 
{
  byte i;
  float result = 0.0;
  float tmp = 0.0;

  for (i = 0; i < 5; i++) {
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        ADMUX = _BV(MUX5) | _BV(MUX0);
    #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        ADMUX = _BV(MUX3) | _BV(MUX2);
    #else
        // works on an Arduino 168 or 328
        ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    #endif

    delay(3); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA,ADSC)); // measuring

    uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both

    tmp = (high<<8) | low;
    tmp = (typVbg * 1023.0) / tmp;
    result = result + tmp;
    delay(5);
  }

  result = result / 5;
  return result;
}

void GeneralDisplaySetup()
{
  tft.init();//инициализация дисплея
  tft.setRotation(1);//ориентация дисплея
  tft.fillScreen(TFT_BLACK);//фон дисплея
  tft.setTextFont(4);//типа шрифта дисплея
  tft.setTextColor(TFT_LIGHTGREY);//цвет шрифта дисплея
}

void BatteryPercentageSetup()
{
  analogReference(DEFAULT);  // DEFAULT INTERNAL использовать Vcc как AREF
  delay(100);//задержка
  Vcc = readVcc();
  MaxVoltage = Vcc / (r2 / (r1 + r2));
  analogWrite(A_PIN, 0);
}

void BatteryPercentageLoop()
{
  Vcc = readVcc();
  // считываем точное напряжение с A0, где будет находиться наш вольтметр с делителем напряжения
  curVoltage = 0.0;
  for (i = 0; i < COUNT; i++) {
      curVoltage = curVoltage + analogRead(A_PIN);
      delay(10);
  }
  curVoltage = curVoltage / COUNT;
  float v  = (curVoltage * Vcc) / 1024.0;
  float v2 = (v / (r2 / (r1 + r2)))/2; 
  
  tft.setCursor(415,5);//установка курсора для вывода процентов
  tft.fillRoundRect(415,5,70,20,0,TFT_BLACK);
  if(12<v2)//вывод процентного заряда батареии
  {
    tft.print("100%");
  }else if (11.4<v2)
  {
    tft.print("90%");
  }else if (10.8<v2)
  {
    tft.print("80%");
  }else if (10.2<v2)
  {
    tft.print("70%");
  }else if (9.6<v2)
  {
    tft.print("60%");
  }else if (9<v2)
  {
    tft.print("50%");
  }else if (8.4<v2)
  {
    tft.print("40%");
  }else if (7.8<v2)
  {
    tft.print("30%");
  }else if (7.2<v2)
  {
    tft.print("20%");
  }else if (6.6<v2)
  {
    tft.print("10%");
  }else if (v2<6)
  {
    tft.print("0%");
  }
}

void FirstPage()
{
  BatteryPercentageLoop();
  
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
  BatteryPercentageLoop();
  
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
