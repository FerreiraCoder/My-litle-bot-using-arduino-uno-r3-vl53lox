//USED TO CHANGE THE CONTROL (FROM MANUAL TO AUTO/VICE-VERSA)
boolean assumeControl = false;

///////////////////////////////
int distancia = 0; //distance variable
int posControl = 0;//stores the servo posiition
boolean control = false; // determine if manual or auto mode
boolean esquerda = false; // determine if bot rotates to left
int score = 0; // used to determine changes in servo position
boolean ini = true;
int powerOn = 0; // used  when power up first time
int forLimit = 5;//how many times the turn left or right, controls the for loop to do this


//LIBRARY FOR VL53LOX
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();


//LIBRARY TO SERVO
#include <Servo.h>


// LIBRARY TO MOTOR SHIELD L2993D
#include <AFMotor.h>

// MOTOR 1 CONNECTION 3 OF L293D
AF_DCMotor motor1(3);
// MOTOR 1 CONNECTION 4 OF L293D
AF_DCMotor motor2(4);



char data = 0; //RECEIVE CHAR DATA VIA SERIAL, KEYBOARD IF CONNECTED OR HC05
int delayTempo = 200; // TEMPO DE ATIVAÇÃO DO MOTOR - 200ms

//SERVO/////////////
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
//SERVO///////////





//SETUP
void setup() {
  Serial.begin(115200); //DEFINES BAUD HATE TO 115200 (HC05 NATIVE BAUD IS 9600, I CHANGED TO 115200, ITS NECESSARY FOR THE VL53LOX WORK CORRECTLY)

 myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  

 //VL53/////////////////////////////////////////////////////////////
Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n"));
   Serial.println(F("LASER SENSOR READY!\n"));
  
}



void loop() {

// LITLE "BOOT SEQUENCE"
delay(2000);
 
powerOn +=1;

while(powerOn == 1){
  powerOn+=1;
  
posControl = 100;
myservo.write(posControl);
delay(500);
posControl = 60;
myservo.write(posControl);
delay(500);
posControl = 140;
myservo.write(posControl);
delay(500);
posControl = 100;
myservo.write(posControl);
delay(500);

}


if (powerOn==10){
  powerOn = 2;
}
// PEQUENO COMANDO PARA BOOT END

  
  if (Serial.available() > 0){ // ESTA PARTE É PARA INICIAR O BOT, SE ATIVAR TEM DE MANDAR A VARIÁVEL PARA MANUAL 2X, USAR DEPOIS??
   // data = Serial.read(); //Read the  incoming  data and store it into variable data
   // assumeControl = Serial.read();

    //NO TERMINAL ESSE COMANDO IMPRIME O CARACTERE ENVIADO 
  //  Serial.print(data);

    
//ASSUMING CONTROL!
if (data=='j'){
assumeControl = true;


//WHEN IN MANUAL THE SPEED IS SET TO MAXIMUM (255)
  motor1.setSpeed(255);
  motor2.setSpeed(255);

do{


  

digitalWrite(A2,LOW); // GREEN OFF
digitalWrite(A1,HIGH); // BLUE ON

 if (Serial.available() > 0){
    data = Serial.read(); //Read the  incoming  data and store it into variable data
   // assumeControl = Serial.read();

    //IN TERMINAL THIS SHOWS WITCH CHARACTER WAS RECEIVED (KEYBOARD OR HC-05)
  //  Serial.print(data);

 //CONTROL MOTOR 1 ---------------------------------------------
  //WHEN 'a' IS RECEIVED LEFT MOTORS GOES FORWARD (200ms)
  if (data == 'a') {
    motor1.run(FORWARD);
    delay (delayTempo);

//WHEN 'b' IS RECEIVED RIGHT MOTORS GOES BACKWARD (200ms)
  } else if (data == 'b') {
    motor1.run(BACKWARD);
    delay (delayTempo);
  }
  else if (data == 'z') {
    motor1.run(RELEASE);
  }


 //CONTROL MOTOR 2 ---------------------------------------------
  if (data == 'c') {
    motor2.run(FORWARD);
    delay (delayTempo);
  } else if (data == 'd') {
    motor2.run(BACKWARD);
    delay (delayTempo);
  }
  else if (data == 'y') {
    motor2.run(RELEASE);
  }


//EXIT MANUAL CONTROL
 if (data == 'k'){
    assumeControl = false;

//WHEN IN AUTOMODE THIS SETS THE SPEED TO (200), THATS BECAUSE I FIND THAT THE BOT REACTS BETTER IN THIS SPEED WHEN IN AUTO
  motor1.setSpeed(200);
  motor2.setSpeed(200);
    
  }
//EXIT MANUAL CONTROL

 }

}while(assumeControl == true );
  
}



//AUTO MODE LOOP
//AUTO////AUTO////AUTO////AUTO////AUTO////AUTO////AUTO////AUTO////AUTO////AUTO////AUTO//
do{

////////////////////////
   score = 0;
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  //digitalWrite(A2,LOW);
////////////////////////

digitalWrite(A2,HIGH);
digitalWrite(A1,LOW);



/////////////////////////////////////////////////////////////////////////////////////////
//LASER///////////////////////////////////////////////////////////////////////
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//BLOCO CONTROLE 1
distancia = measure.RangeMilliMeter;
}
//LASER///////////////////////////////////////////////////////////////////////

//ORIENTAÇÃO/////////////////////////////////////////////////////////////////
posControl = 100;
myservo.write(posControl);
delay(150);

if (distancia >400){


//MOTOR//MOTOR////MOTOR//
    motor1.run(FORWARD);
    motor2.run(FORWARD);
//MOTOR//MOTOR////MOTOR//
 
 Serial.println(" GO FRENTE!");
 delay(10);
 

}else if(distancia<400){

//MOTOR//MOTOR////MOTOR//
    delay (20);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
//MOTOR//MOTOR////MOTOR//
  
//  delay(10);
  Serial.print("OBSTÁCULO FRENTE: ");Serial.println(distancia);
  Serial.println("GIRAR DIREITA");

// GIRAR 60o DIREITA INI
posControl = 60;
myservo.write(posControl);
delay(5);
// GIRAR 60o DIREITA END
  
  digitalWrite(A1,HIGH);
  control = true;

}

if (control == true){

for (int i = 0;i<forLimit;i++){
  //LASER////////////////////////////////////////////////////////////////////////////////////////////
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//BLOCO CONTROLE 1
distancia = measure.RangeMilliMeter;
}
//LASER////////////////////////////////////////////////////////////////////////////////////////////

Serial.print("DIREITA: ");Serial.println(distancia);
  delay(200);

//MOTOR//MOTOR////MOTOR//
   
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    delay(100);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    
//MOTOR//MOTOR////MOTOR//

  
}
//FIM DIREITA////FIM DIREITA////FIM DIREITA////FIM DIREITA////FIM DIREITA////FIM DIREITA////FIM DIREITA//

if (distancia < 400){
  esquerda = true;

//MOTOR//MOTOR////MOTOR//
   motor1.run(RELEASE);
   motor2.run(RELEASE);
//MOTOR//MOTOR////MOTOR//
  
}

if (esquerda == true){
Serial.print("GIRAR ESQUERDA:");


// GIRAR 60o ESQUERDA INI////////////////////////////////////////////////////////////
posControl = 130;
myservo.write(posControl);
delay(5);
// GIRAR 60o DIREITA END////////////////////////////////////////////////////////////



  for (int i = 0;i<forLimit;i++){
  //LASER////////////////////////////////////////////////////////////////////////////////////////////
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//BLOCO CONTROLE 1
distancia = measure.RangeMilliMeter;
}
//LASER////////////////////////////////////////////////////////////////////////////////////////////


digitalWrite(A0,HIGH);
digitalWrite(A1,LOW);
Serial.print("ESQUERDA: ");Serial.println(distancia);
delay(200);


//MOTOR//MOTOR////MOTOR//
  
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(100);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    
//MOTOR//MOTOR////MOTOR//
  
}


if (distancia < 400){
  score+=1;
  
//MOTOR//MOTOR////MOTOR//

    motor1.run(RELEASE);
    motor2.run(RELEASE);
    
//MOTOR//MOTOR////MOTOR//
  
}

  
}
esquerda = false;

if (control == true){
  score += 1;
}

}
control = false;

Serial.print("Score: ");Serial.println(score);
if (score>=2){

// CENTRALIZAR ////////////////////////////////////////////////////////////
posControl = 100;
myservo.write(posControl);
delay(5);
//CENTRALIZAR ////////////////////////////////////////////////////////////

  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,HIGH);
  Serial.println("MANEUVER");

//MOTOR//MOTOR////MOTOR//

    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    delay(1000);
    motor1.run(RELEASE);
    motor2.run(RELEASE);

//MOTOR//MOTOR////MOTOR//
  
delay(2000);
digitalWrite(A2,LOW);  
}
/////////////////////////////////////////////////////////////////////////////////////////




 if (Serial.available() > 0){
    
    data = Serial.read(); //Read the  incoming  data and store it into variable data


//data = Serial.read();
//ENTER MANUAL CONTROL
 if (data == 'j'){
    assumeControl = true;
    motor1.run(RELEASE);
    motor2.run(RELEASE);
  }
 }


}while(assumeControl == false);
//AUTOEND////AUTOEND////AUTOEND////AUTOEND////AUTOEND////AUTOEND////AUTOEND////AUTOEND//


 
//ENTER MANUAL CONTROL

} //ESTE COLCHETE É DO READ SERIAL DATA MARCADO SE EU DESMARCAR ELE VAI PEDIR UM CHAR QUALQUER PRA COMEÇAR USAR PARA SYSTEM POWER ON???
/////////////////////////////////////////////////////////////

  //FIM LOOP
}


 








 






