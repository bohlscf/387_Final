//https://create.arduino.cc/projecthub/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19
#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//SCL - A5; SDA - A4; green - 7; timerButton - 2; buzzer - 3; resetRepCount - 13
int green = 7;
int buttonPin = 2;
int buzzerPin = 3;
int resetPin = 13;

int reps = 0;
int lastReps = 0;
int repFlag = 0;
int resetFlag = 0;
int timer = 0;  
int timerReady = 0;
int count = 0;


void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(green, OUTPUT);
  pinMode(buttonPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
}


void loop(){

//---------------  Rep Count  ----------------------------------------------------------------------------------------------------------------------------
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

    if(count == 10){
    count = 0;
//  Serial.print("Accelerometer: ");
//  Serial.print("X = "); Serial.print(AcX);
//  Serial.print(" | Y = "); Serial.println(AcY);
//  Serial.print(" | Z = "); Serial.println(AcZ); 
}
/*
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");
*/

  
  if(AcY > 25000 && repFlag == 0){
    digitalWrite(green, HIGH); 
    repFlag++;
     
  }
  else if (AcY < 15000 && repFlag == 1){
    repFlag = 0;
    digitalWrite(green, LOW);
    reps++;
    Serial.print("Rep count = ");
    Serial.println(reps);
  }
  delay(100);
  count++;

  if(digitalRead(resetPin) == HIGH && repFlag == 0 && resetFlag == 0){
    Serial.print("Past reps are: ");
    Serial.println(lastReps);
    Serial.print("Current reps are: ");
    Serial.println(reps);
    Serial.println("Rep count is reset");
    lastReps = reps;
    reps = 0;
    resetFlag = 1;
    delay(100);    
  }
  else if(digitalRead(resetPin) == LOW && resetFlag == 1){
    resetFlag = 0;
  }


//----------------  Timer  -------------------------------------------------------------------------------------------------------------------------- 

  if(digitalRead(buttonPin) == HIGH && timerReady == 0){
      delay(35);
      while(digitalRead(buttonPin) == HIGH);
      timer = 5;
      Serial.println("");
      Serial.println("TIME HAS BEGUN");
      Serial.println(timer);
      timerReady = 1;
  }
  
  while( digitalRead(buttonPin) == LOW && timerReady == 1){
      timer--;
      int count = 0;
      while(count < 100){// && digitalRead(buttonPin) == LOW){
        delay(10); 
        count++;
        while(digitalRead(buttonPin) == HIGH){
          delay(50);
          count = 1000;
          timerReady = 0;
          timer = 0;
        }
      }
      if(timer == 0 && count != 1000 ){
        Serial.println("TIME IS UP!!");
        timerReady = 0;
        tone(buzzerPin, 400);
        delay(2000);
        noTone(buzzerPin);
      }
      else if(timer == 0){
        Serial.println("TIMER IS STOPPED");
        tone(buzzerPin, 400);
        delay(500);
        noTone(buzzerPin);
      }
      Serial.println(timer);
  }
  
} 
