#include <Wire.h>  // Wire library - used for I2C communication

int ADXL345 = 0x53; // The ADXL345 sensor I2C address

float X_out, Y_out, Z_out;  // Outputs
float X_new, Y_new, Z_new;

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


void setup() {
  Serial.begin(9600); // Initiate serial communication for printing the results on the Serial monitor
  
  //Define pins----------------------------------------------------------------------------------------------------------
 pinMode(DIRA,OUTPUT); 
 pinMode(DIRB,OUTPUT); 
 pinMode(SpeedA,OUTPUT); 
 pinMode(SpeedB,OUTPUT); 
  //InfroRed
 pinMode(IRsensPin,INPUT);
 IR = digitalRead(IRsensPin); 


 
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // Bit D3 High for measuring enable (8dec -> 0000 1000 binary)
  Wire.endTransmission();
  delay(10);

  //Off-set Calibration------------------------------------------------------------------------------------------------
  //X-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);
  Wire.write(1);
  Wire.endTransmission();
  delay(10);
  //Y-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);
  Wire.write(-2);
  Wire.endTransmission();
  delay(10);

  //Z-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);
  Wire.write(-9);
  Wire.endTransmission();
  delay(10);



  //Cleaning logic----------------------------------------------------------------------------------------------------
  turnR();
  while(digitalRead(IRsensPin)==1){forward(); delay(20000);}
  turnR(); delay(500);
  for (int i = 0; i == 500; i = i + 1){
    Clean1();
  }

  
}

void loop() {
  
  // === Read acceleromter data ===---------------------------------------------------------------------------------- //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read() | Wire.read() << 8); // X-axis value
  X_out = X_out / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read() | Wire.read() << 8); // Y-axis value
  Y_out = Y_out / 256;
  Z_out = ( Wire.read() | Wire.read() << 8); // Z-axis value
  Z_out = Z_out / 256;


  Serial.print("X = ");
  Serial.println(X_out);
  Serial.print("Y = ");
  Serial.println(Y_out);
  Serial.print("Z = ");
  Serial.println(Z_out);
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
  
  X_out = X_new;
  Y_out = Y_new;
  Z_out = Z_new;
  
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
  
  X_out = X_new;
  Y_out = Y_new;
  Z_out = Z_new;
  
}
