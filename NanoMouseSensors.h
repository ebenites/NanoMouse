#include <Arduino.h>

template <byte leftEmitter,byte leftDetector, byte frontEmitter,byte frontDetector, byte rightEmitter,byte rightDetector>

class NanoMouseSensors{

  public:

    void configure(){
      pinMode(leftEmitter, OUTPUT);
      pinMode(frontEmitter, OUTPUT);
      pinMode(rightEmitter, OUTPUT);

      digitalWrite(leftEmitter, HIGH);
      digitalWrite(frontEmitter, HIGH);
      digitalWrite(rightEmitter, HIGH);
    }
    
    void view(){
      Serial.print(analogRead(leftDetector));
      Serial.print("\t");
      Serial.print(analogRead(frontDetector));
      Serial.print("\t");
      Serial.println(analogRead(rightDetector));
      
      
    }
  
};
