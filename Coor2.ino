#include <Wire.h>

#define ADXL345_ADDRESS (0x53) // Адрес датчика ADXL345

#define ADXL345_REGISTER_XLSB (0x32) // Начальный регистр для чтения оси X (младший байт)
#define ADXL345_REGISTER_XMSB (0x33) // Начальный регистр для чтения оси X (старший байт)
#define ADXL345_REGISTER_YLSB (0x34) // Начальный регистр для чтения оси Y (младший байт)
#define ADXL345_REGISTER_YMSB (0x35) // Начальный регистр для чтения оси Y (старший байт)
#define ADXL345_REGISTER_ZLSB (0x36) // Начальный регистр для чтения оси Z (младший байт)
#define ADXL345_REGISTER_ZMSB (0x37) // Начальный регистр для чтения оси Z (старший байт)

float accelerationThreshold = 0.1; // Пороговое значение ускорения для определения движения.
float distance = 0.0; // Пройденное расстояние.


  //Motor pins-----------------------------------------------------------------------------------------------------------
int SpeedA = 10; 
int SpeedB = 11; 
int DIRA = 12; 
int DIRB = 13; 
int IRsensPin = 2;
int IR; 

  //Basement of movement logic---------------------------------------------------------------------------------------------
int Length = 12.3;
int Radius = 1.7;
int S = 2 * PI * Radius * Length + 2 * PI * pow (Radius, 2);
int Sq = S / 500;
int a = sqrt(Sq);

void setup(void) {
  Serial.begin(9600);
  Wire.begin();

 pinMode(DIRA,OUTPUT); 
 pinMode(DIRB,OUTPUT); 
 pinMode(SpeedA,OUTPUT); 
 pinMode(SpeedB,OUTPUT); 
  //InfroRed
 pinMode(IRsensPin,INPUT);
 IR = digitalRead(IRsensPin); 



  //Cleaning logic----------------------------------------------------------------------------------------------------
  turnR();
  while(digitalRead(IRsensPin)==1){forward(); delay(20000);}
  turnR(); delay(500);
  for (int i = 0; i == 500; i = i + 1){
    Clean1();
  }

  
  // Включаем датчик ADXL345
  writeToRegister(ADXL345_ADDRESS, 0x2D, 0);
  writeToRegister(ADXL345_ADDRESS, 0x2D, 16);
  writeToRegister(ADXL345_ADDRESS, 0x2D, 8);
}

void loop(void) {
  // Считываем значения осей X, Y и Z
  int16_t x = readAxis(ADXL345_REGISTER_XLSB, ADXL345_REGISTER_XMSB);
  int16_t y = readAxis(ADXL345_REGISTER_YLSB, ADXL345_REGISTER_YMSB);
  int16_t z = readAxis(ADXL345_REGISTER_ZLSB, ADXL345_REGISTER_ZMSB);

  float acceleration = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

  if (acceleration > accelerationThreshold) {
    // Обновляем пройденное расстояние на основе ускорения и времени.
    distance += 0.5 * acceleration * pow(z, 2); // Здесь используется упрощенная формула для расчета расстояния, исходя из ускорения по оси Z. Может потребоваться настройка в зависимости от конкретных требований.
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" meters");
  }

  delay(100); // Пауза между измерениями для стабильной работы.
}

// Функция для записи значения в регистр датчика
void writeToRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t value) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
Wire.write(value);
Wire.endTransmission();
}

// Функция для чтения значения оси из регистров датчика
int16_t readAxis(uint8_t lsbRegister, uint8_t msbRegister) {
Wire.beginTransmission(ADXL345_ADDRESS);
Wire.write(lsbRegister);
Wire.endTransmission();
Wire.requestFrom(ADXL345_ADDRESS, 2);

byte lsb = Wire.read();
byte msb = Wire.read();

int16_t axisValue = (msb << 8) | lsb;

return axisValue;
}

 //Movement---------------------------------------------------------------------------------------------------------------
 void forward(){ 
 digitalWrite(DIRA,HIGH); 
 digitalWrite(DIRB,HIGH); 
 analogWrite(SpeedA,170); 
 analogWrite(SpeedB,170); 
} 
 
void back(){ 
 digitalWrite(DIRA,LOW); 
 digitalWrite(DIRB,LOW); 
 analogWrite(SpeedA,170); 
 analogWrite(SpeedB,170); 
 delay(80); 
} 
 
void turnL(){ 
 digitalWrite(DIRA,LOW);//Едет назад 
 analogWrite(SpeedA,45); 
 digitalWrite(DIRB,HIGH);//Едет вперед 
 analogWrite(SpeedB,170); 
} 
 
void turnR(){ 
 digitalWrite(DIRA,LOW);//Едет вперед 
 analogWrite(SpeedA,170); 
 digitalWrite(DIRB,HIGH);//Едет вперед 
 analogWrite(SpeedB,45); 
} 

void Stop(){ 
 digitalWrite(DIRA,HIGH); 
 digitalWrite(DIRB,LOW); 
 analogWrite(SpeedA,0); 
 analogWrite(SpeedB,0); 
}   
void Clean1(){
  forward(); delay(2000); Stop();delay(200);
  turnR(); delay(250); Stop();delay(200);
  forward(); delay(2000); Stop();delay(200);
  turnR(); delay(250); Stop();delay(200);
  forward(); delay(2000); Stop();delay(200);
  turnR(); delay(250); Stop();delay(200);
  forward(); delay(1000); Stop();delay(200);
  turnR(); delay(250); Stop();delay(200);
  forward(); delay(1000); Stop();delay(200);
  forward(); delay(100); Stop();delay(200);
  
  
}

void Clean2(){
  forward(); delay(2000); Stop();delay(200);
  turnL(); delay(250); Stop();delay(200);
  forward(); delay(2000); Stop();delay(200);
  turnL(); delay(250); Stop();delay(200);
  forward(); delay(2000); Stop();delay(200);
  turnL(); delay(250); Stop();delay(200);
  forward(); delay(1000); Stop();delay(200);
  turnL(); delay(250); Stop();delay(200);
  forward(); delay(1000); Stop();delay(200);
  forward(); delay(100); Stop();delay(200);
