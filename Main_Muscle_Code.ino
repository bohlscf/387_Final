int flag = 1;
int rep = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int muscleVal = analogRead(A0);
  Serial.println(muscleVal);
  delay(200);

  if(muscleVal > 500 && flag == 1){
    Serial.println(muscleVal);
    delay(50);
    flag = 0;
    rep++;
    Serial.print("Rep is: ");
    Serial.println(rep);
  }

  else if(muscleVal < 300 && flag == 0){
    flag = 1;
    delay(50);
  }
}
