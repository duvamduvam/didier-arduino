#include "Commands.h"

#define TABLE_SIZE 33

#define LOG_NOTICE_MAPPING

class Mapping {

  private:
    char* mapping[TABLE_SIZE];

  public :

    Mapping();

    Commands getCommands(char* key);

    char* getValue(char* key);

};
