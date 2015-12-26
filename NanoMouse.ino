#include "NanoMouseMotors.h"
#include "NanoMouseSensors.h"

NanoMouseMotors motors;
NanoMouseSensors<4,A7, 3,A6, 2,A5> sensors; // <lE,lD, fE,fD, rE,rD>

int targetFront;

int targetSide;
int thresholdSide;

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


  //motors.forwardTime(10000);
  /*motors.turn(LEFT,180);
  delay(100);
  motors.turn(RIGHT,180);
  delay(100);
  motors.turn(LEFT,90);
  delay(100);
  motors.turn(RIGHT,90);
  delay(4000);
  motors.polygon(4);
  delay(4000);
  motors.polygon(4, LEFT);*/

  calibrate();
  
  forwardWhiskers();
  motors.turn(RIGHT, 90);
  sensors.initialize();

  forwardWhiskers();
  motors.turn(RIGHT, 90);
  sensors.initialize();

  forwardWhiskers();


  //sensors.view();
  
    /*Serial.print("Target: ");
    Serial.print(targetFront);
    Serial.print(" - ");
    Serial.print(targetSide);
    Serial.print(" - ");
    Serial.println(thresholdSide);*/

/*
    digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);*/
}

void loop() {
  //sensors.sense();
  //sensors.view();

  /*if(sensors.front > 30){
    Serial.println("Obstacle Detected");  
  }else{
    Serial.println("No Obstacle Detected");    
  }*/

  //avoid(state());

/*
  int l = analogRead(A7);
  int f = analogRead(A6);
  int r = analogRead(A5);
  
  int x = analogRead(A4);
  int y = analogRead(A3);
  int z = analogRead(A2);
  int o = analogRead(A3);

  delay(1);

  if(l!=0 || f!=0 || r !=0 
    || x!=0 || y!=0 || z!=0 || o!=0
  ){
    Serial.print("L:");
    Serial.print(l);
    Serial.print("\tF:");
    Serial.print(f);
    Serial.print("\tR:");
    Serial.println(r);
    
    Serial.print("\tX:");
    Serial.print(x);
    Serial.print("\tY:");
    Serial.print(y);
    Serial.print("\tZ:");
    Serial.print(z);
    Serial.print("\tO:");
    Serial.println(o);
    
    delay(1);
  }
  */
}

void calibrate(){
  
  sensors.initialize();
  targetSide = (sensors.left + sensors.right)/2;
  //Serial.print("targetSide:" );
  //Serial.println(targetSide);
  
  motors.turn(RIGHT, 90);  
  sensors.initialize();
  
  targetFront = sensors.front/3;
  //Serial.print("targetFront:");
  //Serial.println(targetFront);
  thresholdSide = (targetSide + 3*sensors.left)/3;

  motors.turn(LEFT, 90);
  sensors.initialize();
  
}

void forwardWhiskers(){
  while(sensors.front < targetFront){
      sensors.sense();
      if(sensors.left > thresholdSide && sensors.right > thresholdSide)
        motors.forwardProportional(sensors.right - sensors.left);
      else if(sensors.right > thresholdSide)
        motors.forwardProportional(sensors.right - targetSide);
      else if(sensors.left > thresholdSide)
        motors.forwardProportional(targetSide - sensors.left);
      else
        motors.forward();
  }

  motors.stop();  
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

