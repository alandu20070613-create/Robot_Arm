#include "arm_control.h"

Servo base, arm1, arm2, arm3, wrist, claw;

const int baseMin  = 0,  baseMax  = 180;
const int arm1Min  = 0,  arm1Max  = 180;
const int arm2Min  = 0,  arm2Max  = 180;
const int arm3Min  = 0,  arm3Max  = 180;
const int wristMin = 0,  wristMax = 180;
const int clawMin  = 0,  clawMax  = 180;

int  DSD      = 15;
bool mode     = true;
int  moveStep = 3;

void armDataCommand(char serialCommand)
{
  if (serialCommand == 'f' || serialCommand == 't' || serialCommand == 'g' ||
      serialCommand == 'y' || serialCommand == 'h' || serialCommand == 'u' ||
      serialCommand == 'j' || serialCommand == 'i' || serialCommand == 'k' ||
      serialCommand == 'o' || serialCommand == 'l' || serialCommand == 'p')
  {
    Serial.println("Warning: Robot in Instruction Mode");
    delay(100);
    while (Serial.available()) { Serial.read(); }
    return;
  }

  if (serialCommand == 'b' || serialCommand == '1' || serialCommand == '2' ||
      serialCommand == '3' || serialCommand == 'w' || serialCommand == 'c')
  {
    int servoData = Serial.parseInt();
    servoCommand(serialCommand, servoData, DSD);
    return;
  }

  switch (serialCommand)
  {
    case 'm': mode = false; Serial.println("Command: Switch to Arm Handle Mode"); break;
    case 'v': reportStatus();   break;
    case 'n': armIniPos();      break;
    case 'x': grabObject();     break;
    default:  Serial.println("Unknown Command"); break;
  }
}

void armHandleCommand(char serialCommand)
{
  if (serialCommand == 'b' || serialCommand == '1' || serialCommand == '2' ||
      serialCommand == '3' || serialCommand == 'w' || serialCommand == 'c')
  {
    Serial.println("Warning: Robot in Arm Handle Mode");
    delay(100);
    while (Serial.available()) { Serial.read(); }
    return;
  }

  switch (serialCommand)
  {
    case 'f':
      Serial.println("Received Command: Base Turns Clockwise");
      servoCommand('b', base.read() - moveStep, DSD);
      break;
    case 't':
      Serial.println("Received Command: Base Turns Anti-clockwise");
      servoCommand('b', base.read() + moveStep, DSD);
      break;
    case 'g':
      Serial.println("Received Command: Arm1 Down");
      servoCommand('1', arm1.read() - moveStep, DSD);
      break;
    case 'y':
      Serial.println("Received Command: Arm1 Up");
      servoCommand('1', arm1.read() + moveStep, DSD);
      break;
    case 'h':
      Serial.println("Received Command: Arm2 Down");
      servoCommand('2', arm2.read() - moveStep, DSD);
      break;
    case 'u':
      Serial.println("Received Command: Arm2 Up");
      servoCommand('2', arm2.read() + moveStep, DSD);
      break;
    case 'j':
      Serial.println("Received Command: Arm3 Down");
      servoCommand('3', arm3.read() - moveStep, DSD);
      break;
    case 'i':
      Serial.println("Received Command: Arm3 Up");
      servoCommand('3', arm3.read() + moveStep, DSD);
      break;
    case 'k':
      Serial.println("Received Command: Wrist Turns Clockwise");
      servoCommand('w', wrist.read() - moveStep, DSD);
      break;
    case 'o':
      Serial.println("Received Command: Wrist Turns Anti-clockwise");
      servoCommand('w', wrist.read() + moveStep, DSD);
      break;
    case 'l':
      Serial.println("Received Command: Claw Opens");
      servoCommand('c', claw.read() - moveStep, DSD);
      break;
    case 'p':
      Serial.println("Received Command: Claw Closes");
      servoCommand('c', claw.read() + moveStep, DSD);
      break;
    case 'm':
      mode = true;
      Serial.println("Command: Switch to Arm Instruction Mode");
      break;
    case 'v': reportStatus(); break;
    case 'n': armIniPos();    break;
    case 'x': grabObject();   break;
    default:  Serial.println("Unknown Command"); break;
  }
}

void servoCommand(char serialCommand, int servoData, int DSD)
{
  Servo* servo2go = nullptr;
  int    fromPos  = 0;

  Serial.println();
  Serial.print("Command: Servo ");
  Serial.print(serialCommand);
  Serial.print(" to ");
  Serial.print(servoData);
  Serial.print(" at servoDelay value ");
  Serial.print(DSD);
  Serial.println(".");
  Serial.println();

  switch (serialCommand)
  {
    case 'b':
      if (servoData >= baseMin && servoData <= baseMax) {
        servo2go = &base;  fromPos = base.read();  break;
      } else { Serial.println("Warning: Base Servo Data Out Of Limit!");  return; }
    case '1':
      if (servoData >= arm1Min && servoData <= arm1Max) {
        servo2go = &arm1;  fromPos = arm1.read();  break;
      } else { Serial.println("Warning: Arm1 Servo Data Out Of Limit!");  return; }
    case '2':
      if (servoData >= arm2Min && servoData <= arm2Max) {
        servo2go = &arm2;  fromPos = arm2.read();  break;
      } else { Serial.println("Warning: Arm2 Servo Data Out Of Limit!");  return; }
    case '3':
      if (servoData >= arm3Min && servoData <= arm3Max) {
        servo2go = &arm3;  fromPos = arm3.read();  break;
      } else { Serial.println("Warning: Arm3 Servo Data Out Of Limit!");  return; }
    case 'w':
      if (servoData >= wristMin && servoData <= wristMax) {
        servo2go = &wrist; fromPos = wrist.read(); break;
      } else { Serial.println("Warning: Wrist Servo Data Out Of Limit!"); return; }
    case 'c':
      if (servoData >= clawMin && servoData <= clawMax) {
        servo2go = &claw;  fromPos = claw.read();  break;
      } else { Serial.println("Warning: Claw Servo Data Out Of Limit!");  return; }
  }

  if (servo2go == nullptr) return;

  if (fromPos <= servoData) {
    for (int i = fromPos; i <= servoData; i++) {
      servo2go->write(i);
      delay(DSD);
    }
  } else {
    for (int i = fromPos; i >= servoData; i--) {
      servo2go->write(i);
      delay(DSD);
    }
  }
}

void reportStatus()
{
  Serial.println();
  Serial.println("*******************************");
  Serial.println("Robot Arm Status Report");
  Serial.print("Base Position:  "); Serial.println(base.read());
  Serial.print("Arm1 Position:  "); Serial.println(arm1.read());
  Serial.print("Arm2 Position:  "); Serial.println(arm2.read());
  Serial.print("Arm3 Position:  "); Serial.println(arm3.read());
  Serial.print("Wrist Position: "); Serial.println(wrist.read());
  Serial.print("Claw Position:  "); Serial.println(claw.read());
  Serial.println("*******************************");
}

void armIniPos()
{
  Serial.println("Command: Restore Initial Position");
  MoveCmd robotIniPosArray[6] = {
    {'b', 90, DSD},
    {'1', 90, DSD},
    {'2', 90, DSD},
    {'3', 90, DSD},
    {'w', 90, DSD},
    {'c', 90, DSD},
  };
  for (int i = 0; i < 6; i++) {
    servoCommand(robotIniPosArray[i].cmd,
                 robotIniPosArray[i].pos,
                 robotIniPosArray[i].dsd);
  }
}

void grabObject()
{
  Serial.println("Let's play some dice!");
  armIniPos();
  MoveCmd objectMove1[12] = {
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
    {'c', 64, DSD},
  };
  for (int i = 0; i < 12; i++) {
    servoCommand(objectMove1[i].cmd,
                 objectMove1[i].pos,
                 objectMove1[i].dsd);
    delay(200);
  }
  armIniPos();
}
