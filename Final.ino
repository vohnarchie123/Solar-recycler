

#include <Servo.h>; // include servo library

#include <LiquidCrystal.h>; // iclude LCD library       //pleasecheck_pleasecheck

  
#include <SPI.h>;
#include <SD.h>; // include SD Card Library
const int chipSelect = 4;

#include <max6675.h>; // include themocouple library
#include <Wire.h>;


//SERVO:
Servo myservo;  // create servo object to control a servo
                // LCD object
 
int pos = 90;   //MANUALLY ADJUST TO INITIAL POSITION TO AVOID SHOCK AND MECH DAMAGE
// SERVO LDR pins
int sens1 = A0; // LRD 1 pin (North)
int sens2 = A1; // LDR 2 pin  (South)
int tolerance = 2;   //2 degreee margin of error
                     //otherwise the solar panel would be continously adjusting its position.

//STEPPER / LDR pins    
#include <Stepper.h>;
int sens3 = A2; // LDR 3 pin  (East)CCW
int sens4 = A3; //LDR 4 pin West (CW)

//THERMOCOUPLE pins
int soPin = 4;// SO=Serial Out
int csPin = 5;// CS = chip select CS pin
int sckPin = 6;// SCK = Serial Clock pin

MAX6675 thermocouple(sckPin, csPin, soPin);

//LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/*
The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

  
//LED pins
int LED_MOTOR= 30;
int LED_TRACKING= 31;
int LED_ALIGN_ERROR= 32;
int LED_ALIGN_CORRECT =33;


//RELAY pins 

int MOTOR_RELAY =36;

//BUTTON pins
int MOTOR_SWITCH=34;
int TRACKING_SWITCH=35;

/*
//SD CARD logger pins
//// const int chipSelect = 4;
/*The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
  */

  void setup(){
  // run once set-up code

  
  //SERVO output
   myservo.attach(10);  // attaches the servo on pin 10 to the servo object 
   myservo.write(pos);
  delay(5000); // a 5 seconds delay while we position the solar lens

  
  // LDR sensor inputs
  pinMode(sens1, INPUT);  // North
  pinMode(sens2, INPUT);  //South
  pinMode(sens3, INPUT);  //East
  pinMode(sens4, INPUT);  //West

  //STEPPER outputs

  //TEMP inputs
  int soPin = 4;// SO=Serial Out
  int csPin = 5;// CS = chip select CS pin
  int sckPin = 6;// SCK = Serial Clock pin)


MAX6675 thermocouple(sckPin, csPin, soPin);



  
  
  //LCD output
  // INITIALIZE and set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
    lcd.print("Starting....");
      lcd.setCursor(0,1);
      lcd.print("Temperature and Alignment");  
 Serial.begin(9600);// initialize serial monitor with 9600 baud
    Serial.println("ON"); 
    
 delay(3000);// give time to user to read the display at the beginning
 // temperature and alignment USLS

  
  
  //LED output
// initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);  // lights up at initialization
  pinMode(LED_MOTOR, OUTPUT); // lights up motor
  pinMode(LED_TRACKING, OUTPUT); //lights  uptracking 
  pinMode(LED_ALIGN_ERROR, OUTPUT); //lights at alignment error
  pinMode(LED_ALIGN_CORRECT, OUTPUT); //lights at alignment correct
  
  //RELAY output
  pinMode(MOTOR_RELAY, OUTPUT);//toggle motor power supply
  
  //BUTTON input
  pinMode(MOTOR_SWITCH,INPUT); //toggle motor
  pinMode(TRACKING_SWITCH,INPUT); //toggle tracking

  //SD card logger output
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

}

void loop() {
  // put your main code here, to run repeatedly:                   
  
  
 //pacheck sang mga digitalwrite kag read base matrigger lang sila once kag indi na mapatay

//pacheck b sang SD CARD logger kag palista nlang sang iya nga pins nga indi magparehas sa iban
//pacheeeccck ahahhaha
//ang LCD di gid na tarong kay wala mayo na linyada hahahaha pacheck aman

  

//SERVO positioning
int val1 = analogRead(sens1); // read the value of sensor 1
  int val2 = analogRead(sens2); // read the value of sensor 2

  if((abs(val1 - val2) <= tolerance) || (abs(val2 - val1) <= tolerance)) {
    //do nothing if the difference between values is within the tolerance limit
//    serial.println("aligned||");
    digitalWrite(LED_ALIGN_CORRECT, HIGH);
  } else {    
    if(val1 > val2) // if north value is greater
    {
      pos = --pos; // then decrease position gradually
//      serial.println("Move up");
       digitalWrite(LED_ALIGN_ERROR, HIGH);
    }
    if(val1 < val2) // if south value is greater
    {
      pos = ++pos; // then increase position gradually
//      serial.println("Move down");
      digitalWrite(LED_ALIGN_ERROR, HIGH);
    }
  }
  int deflect = pos ;//deflect initial value is current position
  if(pos > 170) { pos = 170; }; // reset to 170 if it goes higher
  if(pos < 20) { pos = 20; }; // reset to 20 if it goes lower
  if((thermocouple.readCelsius() > 240)&&(pos < 90))
  {deflect = 10 + pos; myservo.write(deflect); };
//    serial.println("Too hot, deflect...")};    // deflect lens down when already up
  if((thermocouple.readCelsius() > 240)&&(pos > 90)) 
  {deflect = pos - 10; myservo.write(deflect); };
//  serial.println("Too hot, deflect...")};  // deflect lens up when already done
  
  myservo.write(pos); // write the position to servo
  delay(600); // delay to avoid SHOCK stress
  
//TEMPERATURE DISPLAY
  
//LCD     
// when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) 
      // display each character to the LCD
      lcd.write(Serial.read());
  }
       Serial.print("C = "); 
   Serial.println(thermocouple.readCelsius());
int   temp = (thermocouple.readCelsius());


   lcd.clear();// clear previous values from screen 
      lcd.setCursor(0,0);// set cursor at character 0, line 0       
      lcd.print("Temp:");


      lcd.setCursor(0,1);// set cursor at character 0, line 1
      lcd.print(thermocouple.readCelsius()); 
      lcd.setCursor(5,1);// set cursor at character 9, line 1
      lcd.print((char)223); 
      lcd.setCursor(6,1);// set cursor at character 9, line 1
      lcd.print("C");    
                  
      lcd.setCursor(7,1);// set cursor at character 9, line 1
      lcd.print(" ");       
      lcd.setCursor(8,1);// set cursor at character 9, line 1

delay(1000); }
   
//LED 
// digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//    delay(10);                       // wait for a second
  

 //BUTTON
//  if (digitalRead(MOTOR_SWITCH, HIGH)) {digitalWrite(LED_MOTOR, HIGH); digitalWrite(MOTOR_RELAY, HIGH)}

//KULANG PA NI ELSE?????
//  if (digitalRead(TRACKING_SWITCH, HIGH)) {digitalWrite(LED_TRACKING, HIGH) }


    
 //RELAY



//DATA LOGGER
/*
// DAAAAATAAAAAAAAAA LOGGGGGGG TEMPERATUREEEEE
// make a string for assembling the data to log:
  String dataString = "";

  // read sensor and append to the string:
  for  {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt"); 
    
    
    */
   



  
