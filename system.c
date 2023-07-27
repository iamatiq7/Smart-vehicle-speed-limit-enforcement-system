//Arduino Uno Pin used

//servo: 2; Ultrasonic trig: 3, echo: 4;

//RFID sda or SS: 10, sck: 13, mosi: 11, miso: 12

 

// servo

#include <Servo.h>

//RFID

#include <SPI.h>

#include <MFRC522.h>

//BlueTooth

#include <SoftwareSerial.h>

// Create a servo object

Servo Servo1;

// defines pins numbers-------------------------

// Servo motor pins

int servoPin = 2;

// Ultrasonic pins

const int trigPin = 3;

const int echoPin = 4;

//RFID

#define SS_PIN 10

#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

//led light

const int ledpin = 5;

 

// defines variables----------------------------

//measure speed

long duration1, duration2;

int distance1, distance2;

int range = 30;

double limit = 80;  // set speed limit

 

// Main function **********************************

void setup() {

  Serial.begin(9600);  // Starts the serial communication

 

  // servo

  Servo1.attach(servoPin);

  //ultrasonic

  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output

  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input

  //rfid

  SPI.begin();         // Initiate  SPI bus

  mfrc522.PCD_Init();  // Initiate MFRC522

 

  pinMode(ledpin, OUTPUT);

}

void loop() {

  //Rfid

  //String rf = rfidRead();

  // measure speed using ultrasonic sensor

  double speed = measureSpeed();

  // set speed bump up or down using servo motor

  if (speed > limit){

    //Serial.print("RFID: ");  

    //Serial.println(rf);

    //Serial.println("\n");

    digitalWrite(ledpin, HIGH);

    Serial.print("speed: ");

    Serial.print(speed);

    Serial.println("m/s");

    setBump(speed);

  }

    Servo1.write(54);

}




// User Defined Functions    +++++++++++++++++++++++++++++++

// set speed breaker using servo motor

void setBump(double speed) {

    Servo1.write(0);  //bump up

    Serial.println("Bump Up");

    delay(2000);

    digitalWrite(ledpin, LOW);

}

// Speed Measure function using ultrasonic

int measureSpeed() {

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);  // Sets the trigPin on HIGH state for 10 micro seconds

  //delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration1 = pulseIn(echoPin, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds

  distance1 = duration1 * 0.034 / 2;   // Calculating the distance

 

  delay(100);  // measure distance 2 after 100 ms

  digitalWrite(trigPin, LOW);

 

  digitalWrite(trigPin, HIGH);  // Sets the trigPin on HIGH state for 10 micro seconds

  //delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration2 = pulseIn(echoPin, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds

  distance2 = duration2 * 0.034 / 2;   // Calculating the distance

 

  if (distance1 <= range && distance2 <= range && distance1 > distance2) {

    //Serial.println(distance1);

    //Serial.println(distance2);

    double speed = abs(distance1 - distance2) / .1;  // m/s

 

    //Serial.print("speed: ");  // Prints the distance on the Serial Monitor

    //Serial.print(speed);

    //Serial.println("m/s");

    return speed;

  }

  return 0;

}

//RFID

String rfidRead() {

  // Look for new cards

  if (!mfrc522.PICC_IsNewCardPresent()) {

   Serial.print("LOOK: \n");

    return;

  }

  // Select one of the cards

  if (!mfrc522.PICC_ReadCardSerial()) {

   Serial.print("Select: \n");

    return;

  }

  //Show UID on serial monitor

    String content = "";

    byte letter;

    for (byte i = 0; i < mfrc522.uid.size; i++) {

      content.concat(String(mfrc522.uid.uidByte[i] <0x10 ? " 0" : " "));

      content.concat(String(mfrc522.uid.uidByte[i], HEX));

    }

    Serial.print(content);

    Serial.print("\n");      

    return content;  //sending data to loop function to save rfid data

}