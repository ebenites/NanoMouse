#include <Arduino.h>
#include <Servo.h>

#define RIGHT -1
#define LEFT 1

class NanoMouseMotors 
{

  private:

    Servo leftServo;
    Servo rightServo;

    static const int power = 500;

    static constexpr float powerTweak = 0.88; // Factor para equilibrar la velocidad de los servos

  public:

    void attach(byte leftMotor, byte rightMotor) 
    {
      leftServo.attach(leftMotor);
      rightServo.attach(rightMotor);
    }

    void forward() 
    {
      leftServo.writeMicroseconds(1500 - power);
      rightServo.writeMicroseconds(1500 + power * powerTweak); // La derecha avanza más rápida y se multiplica powerTweak
    }

    void stop(int time = 500) 
    {
      rightServo.writeMicroseconds(1500);
      delay(20); // Delay porque la izquierda se detiene antes que la derecha
      leftServo.writeMicroseconds(1500);
      delay(time);
    }

    void forwardTime(unsigned int time) 
    {
      forward();
      delay(time);
      stop();
    }

    void turn(int direction, int degrees) 
    {
      leftServo.writeMicroseconds(1500 + power * direction);
      rightServo.writeMicroseconds(1500 + power * powerTweak * direction); // La derecha avanza más rápida y se multiplica powerTweak
      if (direction == LEFT) {
        delay(degrees * 4.9);
      } else {
        delay(degrees * 5.3);
      }
      stop();
    }

    void polygon(int sides, int direction = RIGHT, unsigned int time = 2000) 
    {
      for (int i = 0; i < sides; i++) 
      {
        forwardTime(time);
        turn(direction, 360 / sides);
      }
    }

    void forwardProportional(int error) 
    {
      // If error is (-) -> Turn the right
      // If error is (+) -> Turn the left
      const float Kp = 1.5;
      float leftSpeed = ( (1500 - power) + Kp * error );
      float rightSpeed = ( (1500 + power * powerTweak) + Kp * error ); // La derecha avanza más rápida y se multiplica powerTweak
      
      /*Serial.print("Motors: ");
      Serial.print(leftSpeed - 1500);
      Serial.print(" - ");
      Serial.print(rightSpeed - 1500);
      Serial.print(" -> ");
      Serial.print(leftSpeed);
      Serial.print(" - ");
      Serial.print(rightSpeed);
      Serial.print("\tERROR: ");
      Serial.println(error); */ 
      
      leftServo.writeMicroseconds(leftSpeed);
      rightServo.writeMicroseconds(rightSpeed);
    }
    
};
