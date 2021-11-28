/*************************************************** 
WALL-E os v1.1
Engineer: Matthew Dwaine Hakin
Date:     November 27, 2021

 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "SR04.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


int sensorPin1 = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;    // select the input pin for the potentiometer
int sensorPin3 = A2;    // select the input pin for the potentiometer
int lightsensor = A3;    // select the input pin for the potentiometer


int sensorValue1 = 0;  // variable to store the value coming from the sensor
int sensorValue2 = 0;  // variable to store the value coming from the sensor
int sensorValue3 = 0;  // variable to store the value coming from the sensor

int lightsensorValue = 0;  // variable to store the value coming from the sensor

// Distance sensor
#define TRIG_PIN 12
#define ECHO_PIN 11

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  350 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


#define HEAD_SERVO_LEFT 490
#define HEAD_SERVO_RIGHT 160
#define HEAD_SERVO_FWD 310
#define HEAD_SERVO 0

int headTurn_currentPosition = HEAD_SERVO_FWD;

#define UPPER_NECK_UP 525
#define UPPER_NECK_DN 225
#define UPPER_NECK_SERVO 1
#define LOWER_NECK_UP 475
#define LOWER_NECK_DOWN 175
#define LOWER_NECK_SERVO 2

uint16_t current_neck_position = 0;


#define RIGHT_EYE_UP 450
#define RIGHT_EYE_DOWN 350
#define RIGHT_EYE_SERVO 3

uint16_t currentRightEyeValue = RIGHT_EYE_DOWN;


#define LEFT_EYE_UP 375
#define LEFT_EYE_DOWN 475
#define LEFT_EYE_SERVO 4

uint16_t currentLeftEyeValue = LEFT_EYE_DOWN;

// our servo # counter
uint8_t servonum = 7;
uint8_t servoDELAY = 10;

// Sonic Sensor
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

int currentposition = UPPER_NECK_DN;
int x = currentposition;

bool soundplay = false;

void serial_flush(void);
void readSerial();
void continiousloop();
void animate();
void look_left_or_right(int newHeadTurnPosition);
void neck_move(uint16_t newNeckPosition);
void animateEyes();
void lowerNeck(uint16_t newNeckPosition);
void uneckmove(uint16_t pos);
void loneckmove(uint16_t pos);
void right_eye_up_dn();
void left_eye_up_dn();
void blinkleft();
void blinkright();
void displaycommandlist();


void readSerial() {
  // Read incoming byte
  String cmd = Serial.readString();
  int test = cmd.indexOf("leud");
  int necktest100 = cmd.indexOf("neck100");
  int necktest200 = cmd.indexOf("neck200");
  int necktest300 = cmd.indexOf("neck300");
  int neckdown = cmd.indexOf("neckdown");
  int looklefttest100 = cmd.indexOf("left100");
  int lookcentertest = cmd.indexOf("center");
  int lookrighttest100 = cmd.indexOf("right100");
  int playonetest = cmd.indexOf("playone");
  int animateeyestest = cmd.indexOf("eyes");
  int cmdtest = cmd.indexOf("cmd");
  int animate1test = cmd.indexOf("a1");
  //Serial.println(necktest100);
  serial_flush();
  String formatted = cmd.substring(0, cmd.length() -1);
  x = cmd.toInt();
  // Serial.println(x);
  // Serial.println(cmd);
  // animate();
  
    //animate();
    if(test > -1)
    {
      left_eye_up_dn();
    }else if (formatted == "reud")
    {
      right_eye_up_dn();
    }else if (formatted == "bl")
    {
      blinkleft();
    }
    else if (formatted == "br")
    {
      blinkright();
    }else if (necktest100 > -1)
    {
      neck_move(100);
    }else if (necktest200 > -1)
    {
      neck_move(200);
    }
    else if (necktest300 > -1)
    {
      neck_move(300);
    }else if (neckdown > -1)
    {
      neck_move(1);
    }else if (playonetest > -1)
    {
      digitalWrite(5, HIGH);
      delay(100);
      digitalWrite(5, LOW);
    }else if (animateeyestest > -1)
    {
      animateEyes();
    }else if (cmdtest > -1)
    {
      displaycommandlist();
    }
    else if (animate1test > -1)
    {
      animate();
    }else if (looklefttest100 > -1)
    {
      
      look_left_or_right(450);
    }
    else if (lookrighttest100 > -1)
    {
      look_left_or_right(170);
    }
    else if(lookcentertest > -1)
    {
      look_left_or_right(310);
    }
    
  
  serial_flush();
 }

void displaycommandlist()
{
  Serial.println("Print Command List : cmd");
  Serial.println("Left Eye move up and down : leud");
  Serial.println("Right Eye move up and down : reud");
  Serial.println("Both Eyes move up and down : eyes");
  Serial.println("Move Neck up 100 : neck100");
  Serial.println("Move Neck up 200 : neck200");
  Serial.println("Move Neck up 300 : neck300");
  Serial.println("Move Neck down : neckdown");
  Serial.println("Look left 100% : left100");
  Serial.println("Look right 100% : right100");
  Serial.println("Look center : center");
  Serial.println("Blink Right Eye : br");
  Serial.println("Blink Left Eye : bl");
  Serial.println("Animation Routine One : a1");
}


void right_eye_up_dn()
{
  for(int pulseln = 0; pulseln < 101;pulseln++)
  {
    if(!(pulseln + RIGHT_EYE_DOWN > RIGHT_EYE_UP))
    {
      pwm.setPWM(RIGHT_EYE_SERVO, 0, pulseln + RIGHT_EYE_DOWN);
    }
    
    
  }
  delay(500);

  for(int pulseln = 100; pulseln > 0;pulseln--)
  {
    
    pwm.setPWM(RIGHT_EYE_SERVO, 0, pulseln + RIGHT_EYE_DOWN);
    
  }
}

void left_eye_up_dn()
{
  for(int pulseln = 0; pulseln < 101;pulseln++)
  {
    int right_eye = LEFT_EYE_DOWN - pulseln;
    
    pwm.setPWM(LEFT_EYE_SERVO, 0, right_eye);
    
  }
  delay(500);

  for(int pulseln = 100; pulseln > 0;pulseln--)
  {
    int right_eye = LEFT_EYE_DOWN - pulseln;
    
    pwm.setPWM(LEFT_EYE_SERVO, 0, right_eye);
    
  }
}

void blinkleft()
{
  digitalWrite(2, LOW);
  delay(1000);
  digitalWrite(2, HIGH);
}
void blinkright()
{
  digitalWrite(4, LOW);
  delay(1000);
  digitalWrite(4, HIGH);
}

void animateEyes()
{
  for(int pulseln = 0; pulseln < 101;pulseln++)
  {
    int right_eye = LEFT_EYE_DOWN - pulseln;
    pwm.setPWM(RIGHT_EYE_SERVO, 0, pulseln+ RIGHT_EYE_DOWN);
    pwm.setPWM(LEFT_EYE_SERVO, 0, right_eye);
    
  }
  delay(1000);

  for(int pulseln = 100; pulseln > 0;pulseln--)
  {
    int right_eye = LEFT_EYE_DOWN - pulseln;
    pwm.setPWM(RIGHT_EYE_SERVO, 0, pulseln+ RIGHT_EYE_DOWN);
    pwm.setPWM(LEFT_EYE_SERVO, 0, right_eye);
    
  }
  

}


void look_left_or_right(int newHeadTurnPosition)
{
  
  if(headTurn_currentPosition < newHeadTurnPosition && newHeadTurnPosition < HEAD_SERVO_LEFT)
  {
    for(uint16_t pulselen = headTurn_currentPosition; pulselen < newHeadTurnPosition; pulselen++)
    {
      
      pwm.setPWM(HEAD_SERVO, 0, pulselen);
      delay(3);
    }
  }else if(headTurn_currentPosition > newHeadTurnPosition && newHeadTurnPosition > HEAD_SERVO_RIGHT)
  {
    for(uint16_t pulselen = headTurn_currentPosition; pulselen > newHeadTurnPosition; pulselen--)
    {
      pwm.setPWM(HEAD_SERVO, 0, pulselen);
      delay(3);
    }
  }
  headTurn_currentPosition = newHeadTurnPosition;
}

void uneckmove(uint16_t pos)
{
  pwm.setPWM(UPPER_NECK_SERVO, 0, pos + UPPER_NECK_DN);
}

void loneckmove(uint16_t pos)
{
  pwm.setPWM(LOWER_NECK_SERVO, 0, pos + LOWER_NECK_DOWN);
}

void neck_move(uint16_t newNeckPosition)
{
  if (current_neck_position < newNeckPosition && newNeckPosition <= 300)
  {
    for(uint16_t pulseln = current_neck_position; pulseln < newNeckPosition; pulseln++)
    {
      
      loneckmove(pulseln);
      delay(4);
      uneckmove(pulseln);
      
      
    }
      current_neck_position = newNeckPosition;
      /* delay(500);
      look_left_or_right(450);
      delay(1000);
      look_left_or_right(170); 
      delay(1000);
      look_left_or_right(310); 
      delay(500);
      animateEyes();
      delay(500);
      lowerNeck(0); */
      
      
  }else if (current_neck_position > newNeckPosition && newNeckPosition >= 0)
  {
    for(uint16_t pulseln = current_neck_position; pulseln > newNeckPosition; pulseln--)
    {
      pwm.setPWM(UPPER_NECK_SERVO, 0, pulseln + UPPER_NECK_DN);
      delay(5);
      pwm.setPWM(LOWER_NECK_SERVO, 0, pulseln + LOWER_NECK_DOWN);
      
    }
    current_neck_position = newNeckPosition;
  }


}

void lowerNeck(uint16_t newNeckPosition)
{
  for(uint16_t pulseln = current_neck_position; pulseln > newNeckPosition; pulseln--)
    {
      pwm.setPWM(UPPER_NECK_SERVO, 0, pulseln + UPPER_NECK_DN);
      
      pwm.setPWM(LOWER_NECK_SERVO, 0, pulseln + LOWER_NECK_DOWN);
      delay(5);
      
    }
    current_neck_position = newNeckPosition;
}





void move(int newPosition)
{
  if(newPosition > currentposition)
  {
    for (uint16_t pulselen = currentposition; pulselen < newPosition; pulselen++) {
      pwm.setPWM(LOWER_NECK_SERVO, 0, pulselen);
      
      delay(servoDELAY);
    }
    currentposition = newPosition;
  }else if(newPosition < currentposition)
  {
    for (uint16_t pulselen = currentposition; pulselen > newPosition; pulselen--) {
      pwm.setPWM(LOWER_NECK_SERVO, 0, pulselen);
      
      delay(servoDELAY);
    }
    currentposition = newPosition;
  }
}



void serial_flush(void) {
  while (Serial.available()) Serial.read();
}


 
void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  pwm.begin();
  soundplay = false;
   
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  pwm.setPWM(LOWER_NECK_SERVO, 0, LOWER_NECK_DOWN);
  pwm.setPWM(UPPER_NECK_SERVO, 0, UPPER_NECK_DN);
  pwm.setPWM(HEAD_SERVO, 0, HEAD_SERVO_FWD);
  pwm.setPWM(RIGHT_EYE_SERVO, 0, RIGHT_EYE_DOWN);
  pwm.setPWM(LEFT_EYE_SERVO, 0, LEFT_EYE_DOWN);
  
  delay(100);
   currentposition = LOWER_NECK_DOWN;
  //uint8_t p = pwm.readPrescale();
  Serial.println("<WALL_E os v1.1>");
  delay(500);
  Serial.print("int systems");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  Serial.print("\n");
  delay(1000);
  Serial.println("System initialized<>");
  Serial.print("\n");
  Serial.println("This is version 1.1 of WALL-E os.");
  Serial.println("Engineer: Matthew Dawine Hakin");
  Serial.println("Date: November 27, 2021");
  Serial.print("\n");

  displaycommandlist();


  //neck_move(current_neck_position);
  // move(currentposition);
  
}


// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  //Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  //Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  //pwm.setPWM(n, 0, pulse);
}





void loop() {
if (Serial.available() > 0){
    readSerial();
    //animate();
  }

  
  sensorValue1 = analogRead(sensorPin1); // y
  sensorValue2 = analogRead(sensorPin2); // x
  sensorValue3 = analogRead(sensorPin3); // joystick switch
  lightsensorValue = analogRead(lightsensor); // light

  /* if(sensorPin3 == 0)
  {
    soundplay = true;
    Serial.println("soundplay = true");
    Serial.print("Switch val: ");
   Serial.println(sensorValue3);
  }
  if(soundplay == true)
  {
    //digitalWrite(5, HIGH);
    soundplay = false;
    delay(1000);
    Serial.println("soundplay = false");
    Serial.print("Switch val: ");
   Serial.println(sensorValue3);
    //digitalWrite(5, LOW);

  } */

   /* Serial.print("Y Value: ");
   Serial.println(sensorValue1);
   Serial.print("X Value: ");
   Serial.println(sensorValue2); */
   /* Serial.print("Switch val: ");
   Serial.println(sensorValue3); */

   /* Serial.print("Light Sensor Val: ");
   Serial.println(lightsensorValue);
 */
  /* digitalWrite(6, HIGH);
  delay(1000);
  digitalWrite(6, LOW);
  delay(1000); */

    a=sr04.Distance();
   //Serial.print(a);
   //Serial.println("cm");
   //delay(1000);
  
}


void animate()
{
  int newposition = currentposition;
  
  newposition = 360;
  move(newposition);
  delay(1000);


  digitalWrite(2, HIGH);
  delay(3000);
  digitalWrite(4, HIGH);
  

  newposition = 260;
  move(newposition);
  delay(2000);

  newposition = 160;
  move(newposition);
  delay(4000);

  newposition = 260;
  move(newposition);
  delay(2000);

  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
}
