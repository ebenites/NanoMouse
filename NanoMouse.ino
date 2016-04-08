//#define DEBUG

#include "NanoMouseMotors.h"
#include "NanoMouseSensors.h"
#include "NanoMouseMaze.h"

NanoMouseMotors motors;
NanoMouseSensors<4,A7, 3,A6, 2,A5> sensors; // <lE,lD, fE,fD, rE,rD>
NanoMouseMaze<4,5> maze; // <rows,cols>

int targetFront;
int thresholdFront;

int targetSide;
int thresholdSide;

const byte ledPin = 13;
const byte buttonPin = 9;

void setup() 
{
  motors.attach(6,5);

  Serial.begin(9600);
  sensors.configure();
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  while(digitalRead(buttonPin)){}
  digitalWrite(ledPin, HIGH);
  delay(1000);

  // Staring robot ...

  //motors.forward();
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
 
  /*forwardWhiskers();
  motors.turn(RIGHT, 90);
  sensors.initialize();

  forwardWhiskers();
  motors.turn(RIGHT, 90);
  sensors.initialize();

  forwardWhiskers();*/
  
  
  calibrate();
  

  // *** Maze Solving ***

  // Start initial position (TEST: 2,0,NORTH | 3,3,WEST | 1,1,NORTH | 3,2,EAST | 1,5,SOUTH |  | 1,1,EAST | 3,3,NORTH)
  maze.mouseRow = 3;
  maze.mouseColumn = 0;
  maze.mouseHeading = NORTH;

  // Target position
  maze.targetRow = 2;
  maze.targetColumn = 3;

  // Init walls
  //maze.addVirtualWalls();
  //maze.addWalls(NORTH);
  //maze.addWalls(EAST);
  //maze.addWalls(WEST);
  //maze.addWalls(SOUTH);
  //scanWalls();

  //forwardWhiskers();

  //scanWalls();
  
  // Start solve
  //maze.solve();

  //turnTowardBestNeighbor();

  // Print maza
  //maze.print();

  // Best Neighbor
  //Serial.print(maze.findBestNeighbor());
  

  // Stepping through the maze!
  do{
    scanWalls();
    maze.solve();
    
    #ifdef DEBUG
    /*Serial.print(sensors.front); Serial.print(" > "); Serial.print(thresholdFront); Serial.print(" | "); 
    Serial.print(sensors.left); Serial.print(" > "); Serial.print(thresholdSide); Serial.print(" | "); 
    Serial.print(sensors.right); Serial.print(" > "); Serial.print(thresholdSide); Serial.println(""); */
    maze.print();
    while(digitalRead(buttonPin)){}
    delay(500);
    #endif
    delay(500); // temporal
    turnTowardBestNeighbor();
    forwardWhiskers();
  
  }while(maze.values[maze.mouseRow][maze.mouseColumn] != 0);
  
  #ifdef DEBUG
  scanWalls();
  maze.solve();
  maze.print();
  #endif

}

void loop() 
{
  //sensors.sense();
  //motors.forwardProportional(sensors.right - sensors.left);
  //sensors.view();

  /*if(sensors.front > 30){
    Serial.println("Obstacle Detected");  
  }else{
    Serial.println("No Obstacle Detected");    
  }*/

  //avoid(state());

  //////////////////////////////navigateLabyrinth(state());

  /*motors.turn(LEFT,90);
  delay(1000);
  motors.turn(RIGHT,90);
  delay(1000);*/


  /*Serial.print(sensors.front); Serial.print(" > "); Serial.print(thresholdFront); Serial.print(" | "); 
  Serial.print(sensors.left); Serial.print(" > "); Serial.print(thresholdSide); Serial.print(" | "); 
  Serial.print(sensors.right); Serial.print(" > "); Serial.print(thresholdSide); Serial.println(""); 
  delay(2000);*/
}

void calibrate()
{
  sensors.initialize();
  targetSide = (sensors.left + sensors.right)/2;
  
  motors.turn(RIGHT, 90);  
  sensors.initialize();
  targetFront = sensors.front;
  
  thresholdSide = (targetSide + sensors.left * 2)/3;  // * 2)/3 : Si a veces no detecta las paredes

  motors.turn(LEFT, 90);
  sensors.initialize();

  thresholdFront = (targetFront + sensors.front * 3)/4;  // * 2)/3 : Si a veces no detecta las paredes
}

void forwardWhiskers()
{
  const long oneBlockTime = 1350;  // Tiempo que toma avanzar un bloque
  unsigned long startingTime = millis();
    
  while(sensors.front < targetFront && millis() - startingTime < oneBlockTime)
  {
    sensors.halfInitialize();  // sensors.sense() x5
    
    if(sensors.left > thresholdSide && sensors.right > thresholdSide)
      motors.forwardProportional(sensors.right - sensors.left);
    else if(sensors.right > thresholdSide)
      motors.forwardProportional(sensors.right - targetSide);
    else if(sensors.left > thresholdSide)
      motors.forwardProportional(targetSide - sensors.left);
    else
      motors.forward();
    
    /*Serial.print(sensors.front); Serial.print(" < "); Serial.print(targetFront); 
    Serial.print(" TIME: "); Serial.println(millis() - startingTime);*/
  }
  
  motors.stop();

  // Maze navigate
  maze.mouseRow += neighboringCells[maze.mouseHeading][0];
  maze.mouseColumn += neighboringCells[maze.mouseHeading][1];
  
}

void scanWalls(){
  
  if(sensors.front > thresholdFront)  
    maze.addWalls(maze.mouseHeading);
    
  if(sensors.right > thresholdSide)
    maze.addWalls((maze.mouseHeading + 1) % 4);

  if(sensors.left > thresholdSide)
    maze.addWalls((maze.mouseHeading - 1 + 4) % 4);
    
}

void turnTowardBestNeighbor(){
  
  byte desiredHeading = maze.findBestNeighbor();
  int difference = maze.mouseHeading - desiredHeading;

    if(difference == 1 || difference == -3){
      motors.turn(LEFT, 90);
      sensors.initialize();
    }else if(difference == 3 || difference == -1){
      motors.turn(RIGHT, 90);
      sensors.initialize();
    }else if(difference == 2 || difference == -2){
      motors.turn((random(2))?LEFT:RIGHT, 180);
      sensors.initialize();
    }

    maze.mouseHeading = desiredHeading;

}

byte state()
{
  byte event = 0;

  sensors.sense();

  if(sensors.front > thresholdFront)
    event += 1;

  if(sensors.left > thresholdSide)
    event += 2;

  if(sensors.right > thresholdSide)
    event += 4;

  /*Serial.print(sensors.front); Serial.print(" > "); Serial.print(thresholdFront); Serial.print(" | "); 
  Serial.print(sensors.left); Serial.print(" > "); Serial.print(thresholdSide); Serial.print(" | "); 
  Serial.print(sensors.right); Serial.print(" > "); Serial.print(thresholdSide); Serial.print(" | "); 
  Serial.print(event); Serial.println("");*/

  return event;
}

void avoid(byte event)
{
    switch(event)
    {
      case 7: // Front, Left and Right sensor triggered
      case 1: // Front sensor triggered
        if(random(2))
          motors.turn(LEFT, random(90, 181));
        else
          motors.turn(RIGHT, random(90, 181));
        sensors.initialize();
        break;
      case 2: // Left sensor triggered
      case 3: // Left and Front sensor triggered
        motors.turn(RIGHT, random(30, 61));
        sensors.initialize();
        break;
      case 4: // Right sensor triggered
      case 5: // Right and Front sensor triggered
        motors.turn(LEFT, random(30, 61));
        sensors.initialize();
        break;
      default:  // Left and Right, or Nothing sensor triggered
        motors.forwardProportional(sensors.right - sensors.left);
        break;
    }
}

void navigateLabyrinth(byte event)
{
  switch(event)
  {
    case 0: // Nothing
      /*for(int i=0; i<20; i++) //////////// then forward
      {
        if(i%2==0)
          digitalWrite(ledPin, HIGH);
        else
          digitalWrite(ledPin, LOW);
        delay(500);   
      }*/
      forwardWhiskers();
      break;
    case 1: // Only Front
      for(int i=0; i<100; i++)
      {
        if(i%2==0)
          digitalWrite(ledPin, HIGH);
        else
          digitalWrite(ledPin, LOW);
        delay(100);   
      }
      break;
    case 2:
    case 4:
    case 6:
      forwardWhiskers();
      break;
    case 3:
      motors.turn(RIGHT, 90);
      sensors.initialize();
      break;
    case 5:
      motors.turn(LEFT, 90);
      sensors.initialize();
      break;
    case 7: // END
      digitalWrite(ledPin, HIGH);
      break;
  }
}

