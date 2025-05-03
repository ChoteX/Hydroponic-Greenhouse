float getTemp(byte thermistorPin){
  for (int i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(thermistorPin);
   delay(10);
  }
  
  // average all the samples out
  average = 0;
  for (int i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  
  //Serial.println(average);
  
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273;

  return steinhart;
}
