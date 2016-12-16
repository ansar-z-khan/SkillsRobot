#include <StackArray.h>
#include <Pixy.h>
#include <PixyI2C.h>
#include <PixySPI_SS.h>
//#include <PixyUART.h>
#include <TPixy.h>

#include <Servo.h>

class Drive {
//  /http://arduino.stackexchange.com/questions/1321/servo-wont-stop-rotating
    int leftServoPin;
    int rightServoPin;
    Servo leftServo;
    Servo rightServo;

  public:
    Drive (int leftServo, int rightServo) {
      leftServoPin = leftServo;
      rightServoPin = rightServo;
    }
    void attachServos() {
      //initialzes Servos
      leftServo.attach(leftServoPin);
      rightServo.attach(rightServoPin);
      //Put Servos back in the middle
      leftServo.write (90);
      rightServo.write (90);
    }
    void driveStraight (int speed) {
      leftServo.write (speed);
      rightServo.write (speed);
    }
    void turn (int direction, int time, int speed) { //1 = right -1 = left
      //Get Current State of Servo's
      int leftPos = leftServo.read();
      int rightPos = rightServo.read();

      leftServo.write (-direction * (leftPos + speed));
      rightServo.write (direction * (rightPos + speed));
    }

    void setLeft (int speed){
      leftServo.write (speed);
    }
   void setRight (int speed){
      leftServo.write (speed);
    }
   void stopServos(){
     setLeft(90);
     setRight(90);
   }
   

};


Drive robot (10, 11);
StackArray <Block> yellowBlocks;


Pixy pixy;


void setup() {
  robot.attachServos();
  pixy.init();//Initialize Pixy
  pixy.setServos (500, 500);//Center Servos
  Serial.begin(9600);
}


void loop() {

 
}


void tiltServosUp (int signiture) {
  Block* p =  getSpecialBlocks (1);
  Block specialBlocks [pixy.getBlocks()] = {p};
  Block backWall = specialBlocks [3];
  int currentTilt = 500;
  if (backWall.x != NULL) {
    while (backWall.signature != 1 || backWall.x == NULL) {
      currentTilt += 5;
      pixy.setServos (500, currentTilt);
    }
  }
}


Block*getSpecialBlocks (int signiture) {

  Block blocks [pixy.getBlocks()] = {NULL};//Makes an Array of Blocks
  int numYellow = 0;//Ctr to count how many yellow blocks it has seen
  for (int i = 0; i < pixy.getBlocks(); i++) {
    if (pixy.blocks[i].signature == signiture) {//Checks if signiture of current block matches signiture sent in
      //Pushes Yellow Block into array created earlier
      blocks [numYellow] = (pixy.blocks[i]);
      numYellow++;
    }
  }
  Block swap;
  boolean sorted;
  if (pixy.getBlocks() < 1) {
    return blocks;
  }
  else {
    while (!sorted) {
      sorted = true;
      for (int i = 0; i < pixy.getBlocks(); i++) {
        if (!blocks [i + 1].x == NULL) {
          if (blocks [i].height > blocks [i + 1].height) {
            swap = blocks [i];
            blocks [i + 1] = blocks [i];
            blocks [i + 1] = swap;
            sorted = false;
          }
        }
      }
    }
  }


  return blocks;

}


float getMidpoint (Block blocks []) {
  float midpoint = (blocks [0].x + blocks [1].x) / 2;
  return midpoint;
}


boolean pointToBlock (Block target, int hedge){
  int width = 480; //need pixy cam width
  if (target.x>width/2+hedge){
      //Turns Left until the blocks x val is within a range
      robot.setLeft(135);
  }
  else if (target.x < width/2 - hedge){
    //Turns Right until the blocks x val is within a range
    robot.setRight(135);
  }
  else if (target.x > width/2 - hedge && target.x<width/2+hedge){
    //Stops once the block is within a range
    robot.stopServos();
    return true;
  }
    return false;

}










