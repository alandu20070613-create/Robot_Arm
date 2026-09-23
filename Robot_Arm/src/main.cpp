#include <Arduino.h>
#include "arm_control.h"

void setup()
{
  base.attach(3);  delay(200);
  arm1.attach(5);  delay(200);
  arm2.attach(6);  delay(200);
  arm3.attach(9);  delay(200);
  wrist.attach(10); delay(200);
  claw.attach(11);  delay(200);

  Serial.begin(9600);
  Serial.println("Please input serial data: ");

  base.write(10);
  arm1.write(10);
  arm2.write(10);
  arm3.write(10);
  wrist.write(10);
  claw.write(10);
}

void loop()
{
  if (Serial.available()) {
    char serialCommand = Serial.read();
    if (mode) {
      armDataCommand(serialCommand);
    } else {
      armHandleCommand(serialCommand);
    }
  }
}
