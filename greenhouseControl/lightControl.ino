void lightControl (){
  
  if((millis() - previousMillis) < dayTime && state == 0){
    state = 0;
    digitalWrite(lightPin, HIGH);
  }
  if((millis() - previousMillis) > dayTime && state == 0){
    state = 1;
    digitalWrite(lightPin, LOW);
  }
  if((millis() - previousMillis) > dayTime + nightTime && state == 1){
    state = 0;
    digitalWrite(lightPin, HIGH);
    previousMillis = millis();
  }
}