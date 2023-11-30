/*
  SerialConsole.cpp - Library for parsing commands
  from Serial block
  Created by Filip Richter, April 24, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "SerialConsole.h"

SerialConsole::SerialConsole(Stream &s): _s(s)
{
  setPrompt("");
  debugHandler=0;
}

SerialConsole::SerialConsole(Stream &s, const char *prompt): _s(s)
{
  setPrompt(prompt);
  debugHandler=0;
}

void SerialConsole::begin(){
  _s.print(_prompt);
}

void SerialConsole::begin(const char *info){
  _s.println(info);
  _s.print(_prompt);
}

void SerialConsole::addCommand(void (*callback)(uint8_t argc, char *argv[]), const char cmd[])
{
  cmd_names[cmd_cnt] = cmd;
  cmd_ptrs[cmd_cnt] = callback;
  cmd_lenghts[cmd_cnt] = strlen(cmd);
  cmd_cnt++;
}

void SerialConsole::doWork()
{
  if(_s.available()) readChar();
}

void SerialConsole::setPrompt(const char *prompt){
  _prompt = (char*)prompt;
}

void SerialConsole::readChar()
{
  char ch;
  ch = _s.read();
  if(ch=='\r' || (ch=='\n' && last!='\r')){
    buffer[buffer_ptr]=0;
    _s.println();
    proccessCmd(buffer);
    buffer_ptr=0;
    _s.print(_prompt);
    return;
  }
  if(ch==127 && buffer_ptr>0){
    _s.write(ch);
    buffer_ptr--;
  }
  if(ch=='\n' || isWhiteChar(ch)){
    return;
  }
  if(buffer_ptr>=CONSOLE_BUFFER_LENGTH){
    _s.println();
    _s.println(F("Prilis dlouhy prikaz"));
    _s.print(_prompt);
    buffer_ptr=0;
    return;
  }
  buffer[buffer_ptr]=ch;
  buffer_ptr++;
  _s.write(ch);
  last = ch;
}

void SerialConsole::proccessCmd(char* buffer){
  for(int i=0; i<cmd_cnt; i++){
    if((strncmp(cmd_names[i], buffer, cmd_lenghts[i])==0) && ((buffer[cmd_lenghts[i]]==' ') || (buffer[cmd_lenghts[i]]==0))){
      proccessArgs(buffer, i);
      return;      
    }
  }
  if(strcmp("help", buffer)==0){
    help();
    return;
  }
  println("Prikaz nenalezen");
}

void SerialConsole::help(){
  println("List prikazu:");
  for(int i=0; i<cmd_cnt; i++){
    print("  ");
    println(cmd_names[i]);
  }
}

void SerialConsole::proccessArgs(char* buffer, uint8_t n){
  char *p = buffer+cmd_lenghts[n]+1;
  uint8_t argcnt=0;
  boolean hasArg = false;
  if(*(p-1)==' '){
    while(*p!=0 && argcnt<ARGS_COUNT){
      if(hasArg){
        if(*p==' '){
          hasArg=false;
          *p=0;
        }
      }else{
        if(*p!=' '){
          cmd_args[argcnt]=p;
          hasArg=true;
          argcnt++;
        }
      }
      p++;
    }
  }
  cmd_args[argcnt]=0;
  cmd_ptrs[n](argcnt,cmd_args);
}

boolean SerialConsole::isWhiteChar(char ch){
  return (ch<32) || (ch>126);
}

Print& SerialConsole::printer(){
  return *this;
}

size_t SerialConsole::write(uint8_t val) {
  if(debugHandler) debugHandler(&val, 1);
  return _s.write(val);
}
size_t SerialConsole::write(const uint8_t *buffer, size_t size) {
  if(debugHandler) debugHandler(buffer, size);
  return _s.write(buffer, size);
}
