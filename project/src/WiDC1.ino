//include libraries
#include <SerialESP8266wifi.h>
#include <AFMotor.h>
#include <SoftwareSerial.h>


AF_DCMotor motor1(3, MOTOR12_1KHZ); //create motor 1
AF_DCMotor motor2(4, MOTOR12_1KHZ); //create motor 2

SoftwareSerial esp8266(2, 3); //RX pin = 2, TX pin = 3

//definition of variables
#define DEBUG true //show messages between ESP8266 and Arduino in serial port
//#define MAX_SPEED 150 // sets speed of DC  motors
int state = 5; //define initial state of the robot (5 = stand-by)
//define motor pins
//const int motor1Pin1 = 5;
//const int motor1Pin2 = 6;
//const int motor2Pin1 = 9;
//const int motor2Pin2 = 10;
//define motor speed
//int motorSpeed = 150; //motor speed (PWM)

//*****
//SETUP
//*****
void setup()
{
  //set pin modes
 // pinMode(motor1Pin1, OUTPUT);
 // pinMode(motor1Pin2, OUTPUT);
  //pinMode(motor2Pin1, OUTPUT);
  //pinMode(motor2Pin2, OUTPUT);
  motor1.setSpeed(120);//set speed of DC  motor1
  motor2.setSpeed(120);//set speed of DC  motor2

  
  //start communication
  Serial.begin(9600);
  esp8266.begin(9600);
  
  sendData("AT+RST\r\n", 2000, DEBUG); //reset module
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG); //set station mode
  sendData("AT+CWJAP=\"CS207\",\"12345678\"\r\n", 2000, DEBUG); //connect wi-fi network (replace XXXXX by your Wi-Fi router SSID and YYYYY by its password
  delay(5000); //wait for connection

  sendData("AT+CIFSR\r\n", 1000, DEBUG); //show IP address
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); //allow multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // start web server on port 80
}

//*********
//MAIN LOOP
//*********
void loop()
{
  
  if (esp8266.available())  //verify incoming data
  {
    if (esp8266.find("+IPD,")) //if there is a message
    {
      String msg;
      esp8266.find("?"); //look for the message
      msg = esp8266.readStringUntil(' '); //read whole message
      String command = msg.substring(0, 3); //first 3 characters = command
      Serial.println(command);
           
      //move forward
      if(command == "cm1") {
          state = 1;
      }

      //move backward
      if(command == "cm2") {
          state = 2;
      }

      //turn right
      if(command == "cm3") {
          state = 3;
       }
       
       //turn left
       if(command == "cm4") {
          state = 4;
       }

       //do nothing       
       if(command == "cm5") {
          state = 5;
       }

    }
  }
  
  //STATE 1: move forward
  if (state == 1) {
     motor1.run(FORWARD); 
     motor2.run(FORWARD);
   // analogWrite(motor1Pin1, motorSpeed);
    //digitalWrite(motor1Pin2, LOW);
   // analogWrite(motor2Pin1, motorSpeed);
   // digitalWrite(motor2Pin2, LOW);
  }
  //STATE 2: move backward
  if (state == 2) {
     motor1.run(BACKWARD);      
     motor2.run(BACKWARD);
    /*digitalWrite(motor1Pin1, LOW);
    analogWrite(motor1Pin2, motorSpeed);
    digitalWrite(motor2Pin1, LOW);
    analogWrite(motor2Pin2, motorSpeed);*/
    }
  //STATE 3: move right
  if (state == 3) {
     motor1.run(FORWARD);
     motor2.run(BACKWARD);
    /*analogWrite(motor1Pin1, motorSpeed);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    analogWrite(motor2Pin2, motorSpeed);*/
  }
  //STATE 4: move left
  if (state == 4) {
    motor1.run(BACKWARD);     
    motor2.run(FORWARD);
    /*digitalWrite(motor1Pin1, LOW);
    analogWrite(motor1Pin2, motorSpeed);
    analogWrite(motor2Pin1, motorSpeed);
    digitalWrite(motor2Pin2, LOW);*/
  }
  //STATE 5: do nothing
  if (state == 5) {
    motor1.run(RELEASE); 
    motor2.run(RELEASE);
    
   /* digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);*/
  }
  
}

//*******************
//Auxiliary functions
//*******************
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
