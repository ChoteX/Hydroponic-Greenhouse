void nutrientsControl(){
  int pos;

  if(servoState == 0){
    servo.write(180);
  }
  if(servoState == 1){
    for (pos = 180; pos >= 122; pos -= 1) {
      servo.write(pos);                  
      delay(15);                           
    }
  }
  if(servoState == 2){
    for (pos = 122; pos >= 70; pos -= 1) {  
      servo.write(pos);                  
      delay(15);                           
    }
  }
  if(servoState == 3){
    for (pos = 70; pos >= 0; pos -= 1) {  
      servo.write(pos);                  
      delay(15);                           
    }
    servoState = 0;
  }
  
}