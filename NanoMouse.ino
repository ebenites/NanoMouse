#include "NanoMouseMotors.h"
#include "NanoMouseSensors.h"

NanoMouseMotors motors;
NanoMouseSensors<4,A7, 3,A6, 2,A5> sensors; // <lE,lD, fE,fD, rE,rD>

int targetFront;

const byte ledPin = 13;
const byte buttonPin = 9;

void setup() {
  
  motors.attach(6,5);

  Serial.begin(9600);
  sensors.configure();
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  while(digitalRead(buttonPin)){}
  digitalWrite(ledPin, HIGH);
  delay(1000);

  /*calibrate();
  while(sensors.front < targetFront){
      sensors.sense();
      motors.forwardProportional(sensors.right - sensors.left);
  }

  motors.stop();*/
  
  //motors.forwardTime(10000);
  //motors.turn(LEFT,180);
  //motors.turn(RIGHT,180);
  //motors.polygon(2000, 4);
}

void loop() {
  //sensors.sense();
  //sensors.view();

  /*if(sensors.front > 30){
    Serial.println("Obstacle Detected");  
  }else{
    Serial.println("No Obstacle Detected");    
  }*/

  avoid(state());
  
}

void calibrate(){
  motors.turn(RIGHT, 90);  
  sensors.initialize();
  targetFront = sensors.front;

  motors.turn(LEFT, 90);
  sensors.initialize();
}

byte state(){
  int threshold = 40;
  byte event = 0;

  sensors.sense();

  if(sensors.front > threshold)
    event += 1;

  if(sensors.left > threshold)
    event += 2;

  if(sensors.right > threshold)
    event += 4;

  return event;
}

void avoid(byte event){
    switch(event){
      case 1: // Front sensor triggered
        if(random(2)){
          motors.turn(LEFT, random(90, 181));
        }else{
          motors.turn(RIGHT, random(90, 181));
        }
        sensors.initialize();
        break;
      case 2: // Left sensor triggered
        motors.turn(RIGHT, random(30, 61));
        sensors.initialize();
        break;
      case 4: // Right sensor triggered
        motors.turn(LEFT, random(30, 61));
        sensors.initialize();
        break;
      default:
        motors.forwardProportional(sensors.right - sensors.left);
        break;
    }
}

