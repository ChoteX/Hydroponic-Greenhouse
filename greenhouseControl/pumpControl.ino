void pumpControl(){
   if(getDistance(trigPin2, echoPin2) > minDistance){
     digitalWrite(pumpPin, HIGH);
   }else if(getDistance(trigPin2, echoPin2) < minDistance){
     digitalWrite(pumpPin, LOW);
   }
}