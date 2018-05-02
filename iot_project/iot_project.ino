#include <MsTimer2.h>

#include <Servo.h>

Servo myservo;//create a object of servo,named as myservo

#define ENA_PIN   5 // ENA of DC motor driver module attach to pin5 of sunfounder uno board
#define ENB_PIN   6 // ENB of DC motor driver moduleattach to pin6 of sunfounder uno board
#define MOTOR_L_1 8 // left MOTOR_L_1 attach to pin8 
#define MOTOR_L_2 9 // left MOTOR_L_2 attach to pin9
#define MOTOR_R_1 10 //right  MOTOR_R_1 attach to pin10
#define MOTOR_R_2 11 //right MOTOR_R_2 attach to pin11
#define PIN_1      3
#define PIN_2      4
#define PIN_3      7

#define FORWARD 0  //define forward=0,car move forward
#define BACK    1 //define back=1,car move back
#define SPEED 200 //define SPEED=180,it is the rotate speed of motor

void setup()
{
  Serial.begin(9600);
    /* set below pins as OUTPUT */
    pinMode(ENA_PIN, OUTPUT);
    pinMode(ENB_PIN, OUTPUT);
    pinMode(MOTOR_L_1, OUTPUT);
    pinMode(MOTOR_L_2, OUTPUT);
    pinMode(MOTOR_R_1, OUTPUT);
    pinMode(MOTOR_R_2, OUTPUT);
    pinMode(PIN_1, INPUT_PULLUP);
    pinMode(PIN_2, INPUT);
    pinMode(PIN_3, INPUT);
    myservo.attach(2);//servo attach to pin2
    myservo.write(45);//set the angle(90) of servo 
    //CAR_move(FORWARD,SPEED,SPEED);//car move forward with speed 180
    //CAR_move(BACK,0,0);//car stop
}

void loop()
{
  int sensorVal = digitalRead(PIN_1);
  int sensor_val2 = digitalRead(PIN_2);
  int sensor_val3 = digitalRead(PIN_3);
  Serial.println(sensorVal);
      if(sensorVal == 1 && sensor_val2 == 1 && sensor_val3 == 1) 
      {
        Serial.println("Hello");
        digitalWrite(MOTOR_L_1,HIGH);digitalWrite(MOTOR_L_2,LOW);//left motor clockwise rotation
        digitalWrite(MOTOR_R_1,HIGH);digitalWrite(MOTOR_R_2,LOW);//right motor clockwise rotation
         analogWrite(ENA_PIN,SPEED);//write speed_left to ENA_PIN,if speed_left is high,allow left motor rotate
         analogWrite(ENB_PIN,SPEED);//write speed_right to ENB_PIN,if speed_right is high,allow right motor rotate
        delay(10);    
      } 
      else if(sensorVal == 1 && sensor_val2 == 0 && sensor_val3 == 0)
      {
         digitalWrite(MOTOR_L_1,LOW);digitalWrite(MOTOR_L_2,HIGH);//left motor clockwise rotation
        digitalWrite(MOTOR_R_1,LOW);digitalWrite(MOTOR_R_2,HIGH);//right motor clockwise rotation
         analogWrite(ENA_PIN,SPEED);//write speed_left to ENA_PIN,if speed_left is high,allow left motor rotate
         analogWrite(ENB_PIN,SPEED);//write speed_right to ENB_PIN,if speed_right is high,allow right motor rotate
      }
      else    {
        Serial.println("Hello");
        digitalWrite(MOTOR_L_1,LOW);digitalWrite(MOTOR_L_2,LOW);//left motor clockwise rotation
        digitalWrite(MOTOR_R_1,LOW);digitalWrite(MOTOR_R_2,LOW);//right motor clockwise rotation
         analogWrite(ENA_PIN,0);//write speed_left to ENA_PIN,if speed_left is high,allow left motor rotate
         analogWrite(ENB_PIN,0);//write speed_right to ENB_PIN,if speed_right is high,allow right motor rotate
        delay(10);    
      } 
}

void CAR_move(unsigned char direction, unsigned char speed_left, unsigned char speed_right)
{
    switch(direction)
    {
        //car move forward with speed 180
        case 0: digitalWrite(MOTOR_L_1,HIGH);digitalWrite(MOTOR_L_2,LOW);//left motor clockwise rotation
    digitalWrite(MOTOR_R_1,HIGH);digitalWrite(MOTOR_R_2,LOW);break;//right motor clockwise rotation
        //car move back with speed 180
  case 1: digitalWrite(MOTOR_L_1,LOW);digitalWrite(MOTOR_L_2,HIGH);
    digitalWrite(MOTOR_R_1,LOW);digitalWrite(MOTOR_R_2,HIGH);break;
  default: break;
    }
    analogWrite(ENA_PIN,speed_left);//write speed_left to ENA_PIN,if speed_left is high,allow left motor rotate
    analogWrite(ENB_PIN,speed_right);//write speed_right to ENB_PIN,if speed_right is high,allow right motor rotate
}

