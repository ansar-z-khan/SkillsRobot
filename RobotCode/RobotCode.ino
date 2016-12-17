#include <Pixy.h>
#include <PixyI2C.h>
#include <PixySPI_SS.h>
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
    void setLeft (int speed) {
      leftServo.write (speed);
    }
    void setRight (int speed) {
      leftServo.write (speed);
    }
    void stopServos() {
      setLeft(90);
      setRight(90);
    }
    void turnRight (int speed, int duration) {
      int startTime = millis();
      int currentTime = startTime;
      while ((currentTime - startTime) <= duration) // do this till duration
      {
        setRight (speed);
        setLeft (180-speed);
        currentTime = millis();
      }
    }

    void turnLeft (int speed, int duration) {
      int startTime = millis();
      int currentTime = startTime;
      while ((currentTime - startTime) <= duration) // do this till duration
      {
        setLeft (speed);
        setRight (180-speed);
        currentTime = millis();
      }
    }


};


class Camera {
    Pixy pixy;
    Drive robot;
  public:
    Camera(int leftServo, int rightServo) : robot (leftServo, rightServo) { //Have to do this whole mess in order to have an object in a class
      intialize();
    }


    void intialize () {
      pixy.init();//Initializes Camera
      pixy.setServos (500, 500);//Center Servos
    }

    void tiltServosUp (int signiture) {
      Block* p =  getSpecialBlocks (signiture);
      Block specialBlocks [pixy.getBlocks()] = {p};//Poulates the array of blocks using the pointer
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
        //Breaks ot if there are no blocks detected
        return blocks;
      }
      else {
        while (!sorted) {
          //Bubble Sorts Blocks By Height
          sorted = true;
          for (int i = 0; i < pixy.getBlocks(); i++) {
            if (!blocks [i + 1].x == NULL) {//Breaks Out if next block is null
              if (blocks [i].height > blocks [i + 1].height) {
                //Swaps the index of blocks one is larger then the next one
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

    float getMidpoint (Block blocks []) {//Returns the midpoint of the top two blocks in an array
      float midpoint = (blocks [0].x + blocks [1].x) / 2;
      return midpoint;
    }

    boolean pointToBlock (Block target, int hedge) {//Points to a Block that is sent in
      int width = 320; // pixy cam width
      if (target.x > width / 2 + hedge) {
        //Turns Left until the blocks x val is within a range
        robot.setLeft(135);
      }
      else if (target.x < width / 2 - hedge) {
        //Turns Right until the blocks x val is within a range
        robot.setRight(135);
      }
      else if (target.x > width / 2 - hedge && target.x < width / 2 + hedge) {
        //Stops once the block is within a range
        robot.stopServos();
        return true;
      }
      return false;

    }



};




void setup() {

}

void loop() {

}
