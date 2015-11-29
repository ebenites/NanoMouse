#include <Arduino.h>
#include <Servo.h>

#define RIGHT -1
#define LEFT 1

class NanoMouseMotors {

  private:

    Servo leftServo;
    Servo rightServo;

    static const byte power = 500;

  public:

    void attach(byte leftMotor, byte rightMotor) {
      leftServo.attach(leftMotor);
      rightServo.attach(rightMotor);
    }

    void forward() {
      leftServo.writeMicroseconds(1500 - power);
      delay(80); // Delay porque la derecha se accionaba antes que la izquierda
      rightServo.writeMicroseconds(1500 + power * 0.88); // La derecha avanza más rápida y se multiplica power*0.88
    }

    void stop(int time = 500) {
      leftServo.writeMicroseconds(1500);
      rightServo.writeMicroseconds(1500);
      delay(time);
    }

    void forwardTime(unsigned int time) {
      forward();
      delay(time);
      stop();
    }

    void turn(int direction, int degrees) {
      leftServo.writeMicroseconds(1500 + power * direction);
      rightServo.writeMicroseconds(1500 + power * 0.88 * direction); // La derecha avanza más rápida y se multiplica power*0.88
      if (direction == LEFT) {
        delay(degrees * 6.5);
      } else {
        delay(degrees * 7.5);
      }
      stop();
    }

    void polygon(unsigned int time, int sides) {
      for (int i = 0; i < sides; i++) {
        forwardTime(time);
        turn(RIGHT, 360 / sides);
      }
    }

    void forwardProportional(int error) {
      // If error is (-) -> Turn the right
      // If error is (+) -> Turn the left
      const float Kp = 5;
      float leftSpeed = ( (1500 - power) + Kp*error );
      float rightSpeed = ( (1500 + power*0.88) + Kp*error ); // La derecha avanza más rápida y se multiplica power*0.88
      
      Serial.print("Motors: ");
      Serial.print(leftSpeed - 1500);
      Serial.print(" - ");
      Serial.print(rightSpeed - 1500);
      Serial.print(" -> ");
      Serial.print(leftSpeed);
      Serial.print(" - ");
      Serial.print(rightSpeed);
      Serial.print("\tERROR: ");
      Serial.println(error);  
      
      leftServo.writeMicroseconds(leftSpeed);
      delay(80); // Delay porque la derecha se accionaba antes que la izquierda
      rightServo.writeMicroseconds(rightSpeed);
    }
    
};
