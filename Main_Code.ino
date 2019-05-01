//https://create.arduino.cc/projecthub/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19
#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//SCL - A5; SDA - A4; green - 7; button - 2
int green = 7;
int buttonPin = 2;

int reps = 0;
int repFlag = 0;
int timer = 0;  

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(green, OUTPUT);
  pinMode(buttonPin, OUTPUT);
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  /*
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");
*/
  Serial.println(reps);
  if(AcY > 25000 && repFlag == 0){
    digitalWrite(green, HIGH); 
    repFlag++;
  }
  else if (AcY < 15000 && repFlag == 1){
    repFlag = 0;
    digitalWrite(green, LOW);
    reps++;
  }
  delay(100);
  
  if(digitalRead(buttonPin) == HIGH && timer == 0){
    Serial.println("I HAVE BEEN PRESSED");
    timer = 30;
    Serial.println(timer);
  }
  while(timer > 0 && digitalRead(buttonPin) == LOW){
    timer--;
    delay(1000);    
    Serial.println(timer);
  }
}
