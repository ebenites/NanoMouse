#include <Arduino.h>

template <byte leftEmitter, byte leftDetector, byte frontEmitter, byte frontDetector, byte rightEmitter, byte rightDetector>

class NanoMouseSensors {

  private:
  
    // Used to store sensor values when the emitters are off
    int leftAmbient;
    int frontAmbient;
    int rightAmbient;
    // Used to store sensor values when the emitters are on
    int leftCombined;
    int frontCombined;
    int rightCombined;
    // Used to filter out ambient light
    int leftReflected;
    int frontReflected;
    int rightReflected;

    // Variables used for smoothing
    int leftTotal;
    int frontTotal;
    int rightTotal;
    static const byte numReadings = 20;
    byte index;
    int leftReadings[numReadings];
    int frontReadings[numReadings];
    int rightReadings[numReadings];
    
    int leftSmoothed;
    int frontSmoothed;
    int rightSmoothed;

  public:

    int left;
    int front;
    int right;

    void configure() {
      pinMode(leftEmitter, OUTPUT);
      pinMode(frontEmitter, OUTPUT);
      pinMode(rightEmitter, OUTPUT);
    }

    void sense() {
      // Sense combined
      digitalWrite(leftEmitter, HIGH);
      digitalWrite(frontEmitter, HIGH);
      digitalWrite(rightEmitter, HIGH);
      delay(1);
      leftCombined = analogRead(leftDetector);
      frontCombined = analogRead(frontDetector);
      rightCombined = analogRead(rightDetector);

      // Sense Ambient
      digitalWrite(leftEmitter, LOW);
      digitalWrite(frontEmitter, LOW);
      digitalWrite(rightEmitter, LOW);
      delay(1);
      leftAmbient = analogRead(leftDetector);
      frontAmbient = analogRead(frontDetector);
      rightAmbient = analogRead(rightDetector);

      // Calculate Reflected value
      leftReflected = (leftCombined - leftAmbient);
      frontReflected = (frontCombined - frontAmbient) * 1.5; // * Factor para equilibrar valores (leds desiguales);
      rightReflected = (rightCombined - rightAmbient) * 2.0; // * Factor para equilibrar valores (leds desiguales);

      // Smoothing
      leftTotal -= leftReadings[index];
      frontTotal -= frontReadings[index];
      rightTotal -= rightReadings[index];
      
      leftReadings[index] = leftReflected;
      frontReadings[index] = frontReflected;
      rightReadings[index] = rightReflected;
      
      leftTotal += leftReadings[index];
      frontTotal += frontReadings[index];
      rightTotal += rightReadings[index];

      leftSmoothed = leftTotal / numReadings;
      frontSmoothed = frontTotal / numReadings;
      rightSmoothed = rightTotal / numReadings;

      left = leftSmoothed;
      front = frontSmoothed;
      right = rightSmoothed;
      
      index += 1;

      if (index >= numReadings) {
        index = 0;
      }
      
      //view();
    }

    void initialize(){
      for(byte i = 0; i < numReadings; i++){
        sense();  
      }  
    }

    void view() {

      Serial.print("Sensors: ");
      Serial.print(leftCombined);
      Serial.print("\t - ");
      Serial.print(leftAmbient);
      Serial.print("\t = ");
      Serial.print(leftReflected);
      Serial.print("\t -> ");
      Serial.print(left);
      
      Serial.print("\t | \t");
      
      Serial.print(frontCombined);
      Serial.print("\t - ");
      Serial.print(frontAmbient);
      Serial.print("\t = ");
      Serial.print(frontReflected);
      Serial.print("\t -> ");
      Serial.print(front);

      Serial.print("\t | \t");

      Serial.print(rightCombined);
      Serial.print("\t - ");
      Serial.print(rightAmbient);
      Serial.print("\t = ");
      Serial.print(rightReflected);
      Serial.print("\t -> ");
      Serial.println(right);

    }

};
