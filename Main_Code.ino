//https://create.arduino.cc/projecthub/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19
#include<Wire.h>
#include <Keypad.h> //arduino library from Mark Stanley and Alexander Brevig
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//SCL - A5; SDA - A4; green - 12; timerButton - 2; buzzer - 3; resetRepCount - 13
int green = 12;
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
int start = 0;
int exercise = 0;

const byte ROWS = 4; //rows and columns for keypad
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8}; //{A0, A1, A2, A3};
byte colPins[COLS] = {7, 6, 5, 4};
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

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
 char customKey = myKeypad.getKey();

if(start == 0){
  startMethod();
  start = 1;
}


//---------------  Rep Calc  ----------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------  Reps   -----------------------------------------------------------------------------------------  
  while(exercise == 1 && start == 1){
    Serial.println("I MADE IT IN THE REP COUNT LOOP");
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
  
    if(digitalRead(resetPin) == HIGH && repFlag == 0 && reps > 0){
      Serial.print("Past reps are: ");
      Serial.println(lastReps);
      Serial.print("Current reps are: ");
      Serial.println(reps);
      Serial.println("Rep count is reset");
      Serial.println();
      lastReps = reps;
      reps = 0;
      start = 0;
      delay(100);    
    }
  }

//------------------------------   Timer   -------------------------------------------------------------------------------------------------------------------------- 
 // while(exercise == 3 && start == 1){
    if(digitalRead(buttonPin) == HIGH && timerReady == 0 && timer > 0){
        delay(35);
        while(digitalRead(buttonPin) == HIGH);
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
          start = 0;
        }
        else if(timer == 0){
          Serial.println("TIMER IS STOPPED");
          tone(buzzerPin, 400);
          delay(500);
          noTone(buzzerPin);
        }
        Serial.println(timer);
    }
 // }

//-------------------------------   Keypad   -------------------------------------------------------------------------------
  if(customKey){
    Serial.println(customKey);

    if(customKey == 'A'){
      exercise = 1;
    }

    else if(customKey == 'B'){
      exercise = 2;
    }

    else if(customKey == 'C'){
      exercise = 3;
    }
    
    else if(customKey == '1'){
      timer = timer * 10 + 1;     //This statement adds on the number pressed
      Serial.println(timer);
    }

    else if(customKey == '2'){
      timer = timer * 10 + 2;
      Serial.println(timer);
    }

    else if(customKey == '3'){
      timer = timer * 10 + 3;
      Serial.println(timer);
    }

    else if(customKey == '4'){
      timer = timer * 10 + 4;
      Serial.println(timer);
    }

    else if(customKey == '5'){
      timer = timer * 10 + 5;
      Serial.println(timer);
    }

    else if(customKey == '6'){
      timer = timer * 10 + 6;
      Serial.println(timer);
    }

    else if(customKey == '7'){
      timer = timer * 10 + 7;
      Serial.println(timer);
    }

    else if(customKey == '8'){
      timer = timer * 10 + 8;
      Serial.println(timer);
    }

    else if(customKey == '9'){
      timer = timer * 10 + 9;
      Serial.println(timer);
    }
    
  }
  
} 
//---------------- OTHER METHODS  ----------------------------------------------------------------

void startMethod(){
  Serial.println("Press 'A' to do bicep curls");
  Serial.println("Press 'B'");
  Serial.println("Press 'C' to do a timed workout");
  Serial.println();
}



