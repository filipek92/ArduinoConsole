#include "CommandLine.h"
CommandLine cmd(Serial, "Arduino>>");

void setup() {
  Serial.begin(115200);
  cmd.addCommand(test, "test");
}

void loop() {
  cmd.doWork();
}

//Test callback, only print numbered argumnets
void test(uint8_t argc, char *argv[]) {
  char buff[30];
  Serial.println("Test OK");
  uint8_t i = 0;
  while(argv[i]!=0 && i<20){
    sprintf(buff, "Argument %d is %s", i, argv[i]);
    Serial.println(buff);
    i++;
  }
}
