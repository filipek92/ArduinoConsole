/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Console.h"

CommandLine::CommandLine(Stream &s): _s(s)
{
  setPrompt("");
}

CommandLine::CommandLine(Stream &s, const char *prompt): _s(s)
{
  setPrompt(prompt);
}

void CommandLine::addCommand(void (*callback)(uint8_t argc, char *argv[]), const char cmd[])
{
  cmd_names[cmd_cnt] = cmd;
  cmd_ptrs[cmd_cnt] = callback;
  cmd_lenghts[cmd_cnt] = strlen(cmd);
  cmd_cnt++;
}

void CommandLine::doWork()
{
  if(_s.available()) readChar();
}

void CommandLine::setPrompt(const char *prompt){
  _prompt = (char*)prompt;
}

void CommandLine::readChar()
{
  char ch;
  ch = _s.read();
  if(ch=='\r' || (ch=='\n' && last!='\r')){
    buffer[buffer_ptr]=0;
    _s.println();
    proccessCmd();
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
  if(buffer_ptr>=BUFFER_LENGTH){
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

void CommandLine::proccessCmd(){
  for(int i=0; i<cmd_cnt; i++){
    if((strncmp(cmd_names[i], buffer, cmd_lenghts[i])==0) && ((buffer[cmd_lenghts[i]]==' ') || (buffer[cmd_lenghts[i]]==0))){
      proccessArgs(i);
      return;      
    }
  }
  if(strcmp("help", buffer)==0){
    _s.println("List prikazu:");
    for(int i=0; i<cmd_cnt; i++){
      _s.print("  ");
      _s.println(cmd_names[i]);  
    }  
  }
  _s.println("Prikaz nenalezen");
}

void CommandLine::proccessArgs(uint8_t n){
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

boolean CommandLine::isWhiteChar(char ch){
  return (ch<32) || (ch>126);
}

Print& CommandLine::printer(){
  return _s;
}