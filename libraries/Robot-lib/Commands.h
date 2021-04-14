/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Commands_h
#define Commands_h

struct Commands
{
  char wheel[20] = {};
  char lights[20] = {};
  char neck[20] = {};
  char face[20] = {};
  char sound[20] = {};

  float leftSpeed;
  float rightSpeed;
};

class Command {
  private:

    char commands[10][10];

  public:

    unsigned  long attack;
    long finish;
    byte nbToken;

    void set(char* command);
    void getCommand(int token, char* action, byte a1, byte a2, byte f1, byte f2, byte c1, byte c2);
    bool doAttack();
    bool doFinish();
    void getInputList(char* str);
};

#endif
