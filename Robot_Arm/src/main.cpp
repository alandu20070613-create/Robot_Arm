#include <Arduino.h>
#include <Servo.h>


void armDataCommand(char serialCommand);
void armHandleCommand(char serialCommand);
void servoCommand(char serialCommand, int servoData, int DSD);
void reportStatus();
void armIniPos();
void grabObject();


Servo base, arm1, arm2, arm3, wrist, claw;


int const baseMin = 0;
int const baseMax = 180;
int const arm1Min = 0;
int const arm1Max = 180;
int const arm2Min = 0;
int const arm2Max = 180;
int const arm3Min = 0;
int const arm3Max = 180;
int const wristMin = 0;
int const wristMax = 180;
int const clawMin = 0;
int const clawMax = 180;


int DSD = 15; //Default Servo Delay
bool mode; //mode = 1: instruction mode, mode = 0: handle mode
int moveStep = 3; //handle mode: arm's movement amount




void setup() {
base.attach(3);
delay(200);
arm1.attach(5);
delay(200);
arm2.attach(6);
delay(200);
arm3.attach(9);
delay(200);
wrist.attach(10);
delay(200);
claw.attach(11);
delay(200);
Serial.begin(9600);
Serial.println("Please input serial data: ");


 base.write(10);
 arm1.write(10);
 arm2.write(10);
 arm3.write(10);
 wrist.write(10);
 claw.write(10);
}




void loop() {
 if(Serial.available()){
  char serialCommand = Serial.read();
  if(mode){
   armDataCommand(serialCommand);
  }
  else{
   armHandleCommand(serialCommand);
  }
  }
}


void armDataCommand(char serialCommand){
 if(serialCommand == 'f' || serialCommand == 't' || serialCommand == 'g' || serialCommand == 'y' ||
    serialCommand == 'h' || serialCommand == 'u' || serialCommand == 'j' || serialCommand == 'i' ||
    serialCommand == 'k' || serialCommand == 'o' || serialCommand == 'l' || serialCommand == 'p'){
     Serial.println("Warning: Robot in Instruction Mode");
     delay(100);
     while(Serial.available()){
       char wrongCommand = Serial.read();
     }
     return;
    }
 if(serialCommand == 'b' || serialCommand == '1' || serialCommand == '2' || serialCommand == '3' || serialCommand == 'w' || serialCommand == 'c'){
   int servoData = Serial.parseInt();
   servoCommand(serialCommand, servoData, DSD);
 }
 else{
   switch(serialCommand){
     case 'm':
       mode = false;
       Serial.println("Command: Switch to Arm Handle Mode");
       break;
     case 'v':
       reportStatus();
       break;
     case 'n':
       armIniPos();
       break;
     case 'x':
       grabObject();
       break;
     default:
       Serial.println("Unknown Command");
   }
 }
}


void armHandleCommand(char serialCommand){
 if(serialCommand == 'b' || serialCommand == '1' || serialCommand == '2' || serialCommand == '3' || serialCommand == 'w' || serialCommand == 'c'){
     Serial.println("Warning: Robot in Arm Handle Mode");
     delay(100);
     while(Serial.available()){
       char wrongCommand = Serial.read();
     }
     return;
   }
 int baseHandlePos;
 int arm1HandlePos;
 int arm2HandlePos;
 int arm3HandlePos;
 int wristHandlePos;
 int clawHandlePos;


 switch(serialCommand){
   case 'f':
     Serial.println("Received Command: Base Turns Clockwise");
     baseHandlePos = base.read() - moveStep;
     servoCommand('b', baseHandlePos, DSD);
     break;
   case 't':
     Serial.println("Received Command: Base Turns Anti-clockwise");
     baseHandlePos = base.read() + moveStep;
     servoCommand('b', baseHandlePos, DSD);
     break;
   case 'g':
     Serial.println("Received Command: Arm1 Down");
     arm1HandlePos = arm1.read() - moveStep;
     servoCommand('1', arm1HandlePos, DSD);
     break;
   case 'y':
     Serial.println("Received Command: Arm1 Up");
     arm1HandlePos = arm1.read() + moveStep;
     servoCommand('1', arm1HandlePos, DSD);
     break;
   case 'h':
     Serial.println("Received Command: Arm2 Down");
     arm2HandlePos = arm2.read() - moveStep;
     servoCommand('2', arm2HandlePos, DSD);
     break;
   case 'u':
     Serial.println("Received Command: Arm2 Up");
     arm2HandlePos = arm2.read() + moveStep;
     servoCommand('2', arm2HandlePos, DSD);
     break;
   case 'j':
     Serial.println("Received Command: Arm3 Down");
     arm3HandlePos = arm3.read() - moveStep;
     servoCommand('3', arm3HandlePos, DSD);
     break;
   case 'i':
     Serial.println("Received Command: Arm3 Up");
     arm3HandlePos = arm3.read() + moveStep;
     servoCommand('3', arm3HandlePos, DSD);
     break;
   case 'k':
     Serial.println("Received Command: Wrist Turns Clockwise");
     wristHandlePos = wrist.read() - moveStep;
     servoCommand('w', wristHandlePos, DSD);
     break;
   case 'o':
     Serial.println("Received Command: Wrist Turns Anti-clockwise");
     wristHandlePos = wrist.read() + moveStep;
     servoCommand('w', wristHandlePos, DSD);
     break;
   case 'l':
     Serial.println("Received Command: Claw Opens");
     clawHandlePos = claw.read() - moveStep;
     servoCommand('c', clawHandlePos, DSD);
     break;
   case 'p':
     Serial.println("Received Command: Claw Closes");
     clawHandlePos = claw.read() + moveStep;
     servoCommand('c', clawHandlePos, DSD);
     break;
   case 'm':
     mode = true;
     Serial.println("Command: Switch to Arm Instruction Mode");
     break;
   case 'v':
     reportStatus();
     break;
   case 'n':
     armIniPos();
     break;
   case 'x':
     grabObject();
     break;
   default:
     Serial.println("Unknown Command");
 }
}


void servoCommand(char serialCommand, int servoData, int DSD){
 Servo servo2go;


 Serial.println("");
 Serial.print("Command: Servo ");
 Serial.print(serialCommand);
 Serial.print(" to ");
 Serial.print(servoData);
 Serial.print(" at servoDelay value ");
 Serial.print(DSD);
 Serial.println(". ");
 Serial.println("");


 int fromPos = 0;
 switch(serialCommand){
   case 'b':
     if(servoData >= baseMin && servoData <= baseMax){
     servo2go = base;
     fromPos = base.read();
     break;
   }
   else{
     Serial.println("Warning: Base Servo Data Out Of Limit!");
     return;
   }
   case '1':
     if(servoData >= arm1Min && servoData <= arm1Max){
     servo2go = arm1;
     fromPos = arm1.read();
     break;
   }
   else{
     Serial.println("Warning: Arm1 Servo Data Out Of Limit!");
     return;
   }
   case '2':
     if(servoData >= arm2Min && servoData <= arm2Max){
     servo2go = arm2;
     fromPos = arm2.read();
     break;
   }
   else{
     Serial.println("Warning: Arm2 Servo Data Out Of Limit!");
     return;
   }
   case '3':
     if(servoData >= arm3Min && servoData <= arm3Max){
     servo2go = arm3;
     fromPos = arm3.read();
     break;
   }
   else{
     Serial.println("Warning: Arm3 Servo Data Out Of Limit!");
     return;
   }
   case 'w':
     if(servoData >= wristMin && servoData <= wristMax){
     servo2go = wrist;
     fromPos = wrist.read();
     break;
   }
   else{
     Serial.println("Warning: Wrist Servo Data Out Of Limit!");
     return;
   }
   case 'c':
     if(servoData >= clawMin && servoData <= clawMax){
     servo2go = claw;
     fromPos = claw.read();
     break;
   }
   else{
     Serial.println("Warning: Claw Servo Data Out Of Limit!");
     return;
   }
 }


 if(fromPos <= servoData){
   for(int i = fromPos; i<= servoData; i++){
     servo2go.write(i);
     delay(DSD);
   }
 }
 else{
   for(int i = fromPos; i >= servoData; i--){
     servo2go.write(i);
     delay(DSD);
   }
 }
}


void reportStatus(){
 Serial.println("");
 Serial.println("*******************************");
 Serial.println("Robot Arm Status Report");
 Serial.print("Base Position: "); Serial.println(base.read());
 Serial.print("Arm1 Position: "); Serial.println(arm1.read());
 Serial.print("Arm2 Position: "); Serial.println(arm2.read());
 Serial.print("Arm3 Position: "); Serial.println(arm3.read());
 Serial.print("Wrist Position: "); Serial.println(wrist.read());
 Serial.print("Claw Position: "); Serial.println(claw.read());
 Serial.println("*******************************");
}


void armIniPos(){
 Serial.println("Command: Restore Initial Position");
 int robotIniPosArray[6][3]= {
   {'b', 90, DSD},
   {'1', 90, DSD},
   {'2', 90, DSD},
   {'3', 90, DSD},
   {'w', 90, DSD},
   {'c', 90, DSD}
 };


 for(int i = 0; i < 6; i++){
     servoCommand(robotIniPosArray[i][0], robotIniPosArray[i][1], robotIniPosArray[i][2]);
 }
}


void grabObject(){
 Serial.println("Let's play some dice!");
 armIniPos();


 int objectMove1[12][3] = {
   {'b', 30, DSD},
   {'1', 40, DSD},
   {'2', 77, DSD},
   {'3', 18, DSD},
   {'w', 60, DSD},
   {'c', 50, DSD},
   {'b', 70, DSD},
   {'1', 30, DSD},
   {'2', 120, DSD},
   {'3', 50, DSD},
   {'w', 32, DSD},
   {'c', 64, DSD}
 };


 for(int i = 0; i < 12; i++){
     servoCommand(objectMove1[i][0], objectMove1[i][1], objectMove1[i][2]);
     delay(200);
 }
 armIniPos();
}
