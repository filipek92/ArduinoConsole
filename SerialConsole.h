/*
  SerialConsole.h - Library for parsing commands
  from Serial block
  Created by Filip Richter, April 24, 2015.
  Released into the public domain.
*/
#ifndef SerialConsole_h
#define SerialConsole_h

#define CONSOLE_BUFFER_LENGTH  80
#define CMD_COUNT      20
#define ARGS_COUNT     20

#include "Arduino.h"
#include "stdint.h"

#include <Print.h>

class SerialConsole
{
  public:
    SerialConsole(Stream &s);
    SerialConsole(Stream &s, const char *prompt);
    void begin();
    void begin(const char *info);
    void addCommand(void (*callback)(uint8_t argc, char *argv[]), const char cmd[]);
    void doWork();
    void setPrompt(const char *prompt);
    void help();
    Print& printer();
    void proccessCmd(char* buffer);
        
    //size_t write(uint8_t val) override;   // Overriding base functionality
    //size_t write(const uint8_t *buffer, size_t size) override;   // Overriding base functionality
    //void (*debugHandler)(const uint8_t *name, size_t size);
  private:
    char *_prompt;

    Stream& _s; 
    void (*cmd_ptrs[CMD_COUNT])(uint8_t, char **);
    const char *cmd_names[CMD_COUNT];
    char *cmd_args[ARGS_COUNT];
    uint8_t cmd_lenghts[CMD_COUNT];
    uint8_t cmd_cnt=0;
    char last;
    
    char buffer[CONSOLE_BUFFER_LENGTH];
    uint8_t buffer_ptr=0;
    
    void readChar();
    void proccessArgs(char* buffer, uint8_t n);
    boolean isWhiteChar(char ch);
};

#endif
