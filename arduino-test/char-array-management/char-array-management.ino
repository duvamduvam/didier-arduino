
#define msg "09: 35: 16 | 08 - 16 - 2019 | 1 | 2 | 3 | 4"


const char received_msg[] = "09: 35: 16 | 08 - 16 - 2019 | 1 | 2 | 3 | 4";

//max 256
char* extractChar(char*str, byte s, byte e){
  char* result;

  byte i = 0;
  for(byte x = s;x<=e;x++){
    result[i] = str[x];
    i++;
  }

  return result;
}

char** getInputList(char* str)
{

  char** results;
  byte nbToken = 0;

  char delim = '|';
  byte i = 0;

  byte s = 0;
  byte e = 0;

  while(str[i]!=0){
    if(str[i] == delim){
      e = i-1;
      results[nbToken] = extractChar(str, s, e);
      s = i+1;
    }
    i++;
  }
  return results;
}

char* nextToken(char* str)
{
  return strtok(str, "|");
}

byte arraySize(char array[])
{
  byte cont = 0;
  for (byte i = 0; array[i] != 0; i++)
    cont++;
  return cont;
}

char** parseData(char* in) {      // split the data into its parts

  char* delim = "|";

  byte inputSize = arraySize(in);

  //Serial.print("inputSize "); Serial.println(inputSize);
  byte tokenNb = 1;
  for (int i ; i < inputSize; i++) {
    if (in[i] == delim[0]) {
      tokenNb++;
    }
  }

  //Serial.print("nb token "); Serial.println(tokenNb);

  char* splited[6];
  char * strtokIndx; // this is used by strtok() as an index

  int i = 0;
  strtokIndx = strtok(in, delim);     // get the first part - the string
  strcpy(splited[0], strtokIndx); // copy it to messageFromPC

  //Serial.println(strtokIndx);

  while (strtokIndx != NULL) {
    strtokIndx = strtok(NULL, delim);
    i++;
    strcpy(splited[i], strtokIndx);
    Serial.println(strtokIndx);
  }
  return splited;
}

void setup() {
  Serial.begin(115200);

  char** splited = parseData(msg);
  //byte tokenNb =  arraySize(splited);

 // Serial.print("token nb"); Serial.println(tokenNb);
  for (int i = 0; i < 6; i++) {
    Serial.print(i); Serial.print(" "); Serial.println(splited[i]);
  }

  delay(5000);

}



void loop() {}
