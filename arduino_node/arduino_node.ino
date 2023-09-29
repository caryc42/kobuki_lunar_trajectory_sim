//------------------------------------------------
//--- arduino_node
//---
//--- This code implements a ROS node on the Arduino.
//--- It publishes a message to the rosserial Publisher
//--- named "chatter".  A subscriber node on the laptop
//--- that runs the kobuki robot will listen on the 
//--- /chatter topic and take action on the text commands
//--- that come from this node.
//--- Initially, this node will simply output the same
//--- canned path that was put out by the original script
//--- on the laptop named drive_kobuki.
//--- Later the receiveEvent function was added to read
//--- characters from an I2C line and an attempt was 
//--- made to control the servo holding the CANSAT.
//--- However, this was never full executed.
//---
//--- Author: Tom Swift
//------------------------------------------------
#include <ros.h>
#include <std_msgs/String.h>
#include <Wire.h>
#include <Servo.h>

int pos = 0;

// The node handle of the publisher
ros::NodeHandle nh;

// This will hold the message sent to the robot laptop
std_msgs::String str_msg;

// Define the publisher.
ros::Publisher chatter("chatter", &str_msg);

int stage = 1;

//-----------------------------------------------------
//--- receiveEvent
//--- This function executes whenever data is received 
//--- from the I2C master.  It is registered as an event
//--- (see setup()). It will receive the characters, '0'
//--- '1' and '2' corresponding to straight, left and
//--- right.  Theoretically, this should cause the kobuki
//--- to turn one way and the server to turn the opposite
//--- way to maintain it's orientation.  This was never
//--- fully implemented, however. 
//-----------------------------------------------------

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); 	  // receive byte as a character
    //Serial.print(c);           // print the character
  }
  int x = Wire.read();    	  // receive byte as an integer
  Serial.println(x);             // print the integer

  //--- Decimal 48 corresponds to hex 30 which is the code
  //--- for the character '0'.  49 and 50 are for characters
  //--- '1' and '2'.  '0' is for going straight, '1' is for
  //--- left, and '2' is for right.
  if(x == 48){
    //Stright
    myservo1.write(120);
    myservo2.write(70);
  }
  else if(x == 49){
    //Left
    myservo1.write(110);
    myservo2.write(50);
  }
  else if(x == 50){
    //Right
    myservo1.write(140);
    myservo2.write(80);
  }


// Initialize the node and advertize the topic
void setup()
  Wire.begin(8)
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  nh.initNode();
  nh.advertise(chatter);
}

// This is the main loop
void loop() {
  //--- This was the code for the canned path.
  if (1 == stage)
  {
    str_msg.data = "forward";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(3000);

    str_msg.data = "right";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(700);

    str_msg.data = "right";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(700);

    str_msg.data = "forward";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(1500);

    str_msg.data = "left";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(700);

    str_msg.data = "left";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(700);

    str_msg.data = "forward";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(1500);

    str_msg.data = "right";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(700);

    str_msg.data = "forward";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(2000);

    str_msg.data = "stop";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(2000);

    stage = 2;  // The loop will contine to spin but won't do anything.
  }
  else
  {
    nh.spinOnce();
  }

}
