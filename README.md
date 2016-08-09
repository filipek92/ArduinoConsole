# SerialConsole
Library that act as console on arduino Serial port. You can register callback functions for commands.
Callback function gets number of string arguments and list of it. Arguments are whitespace separated

## Basic usage
Registering one simple command "test" which returns all arguments numbered. Prompt is also customizable.
```
#include <SerialConsole.h>
SerialConsole cmd(Serial, "Arduino>>");

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
```
See an example

## Instalation posibilities
1. Git
  - Use ```git clone https://github.com/filipek92/SerialConsole.git``` in your arduino library folder
2. Arduino
  - TODO
