#include <IOShieldOled.h>
#include <stdio.h>

#define BUFFER_SIZE 64
#define BUTTON_NUMBER 4
#define BUTTON_PIN_1 35
#define BUTTON_PIN_2 8
#define BUTTON_PIN_3 7
#define BUTTON_PIN_4 2

char buffer[BUFFER_SIZE];

boolean toBePressed[BUTTON_NUMBER];

String inputString = "";         // a string to hold incoming data (this is general code you can reuse)
boolean stringComplete = false;  // whether the string is complete (this is general code you can reuse)

void setup() {
  // initialize serial: (this is general code you can reuse)
  Serial.begin(115200);
  IOShieldOled.begin();

  // pingprobe
  Serial.print("alp://rply/");
  Serial.print("ok?id=0");
  Serial.print('\n'); // End of Message
  Serial.flush();
}

void loop() {

  while (Serial.available() && !stringComplete) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      // add it to the inputString:
      inputString += inChar;
    }
  }

  
  // when a newline arrives:
  if (stringComplete) {
    
    if(inputString.startsWith("alp://")) { // OK is a message I know (this is general code you can reuse)
    
      boolean msgRecognized = true;
      String result = "";
      
      if(inputString.substring(6,10) == "cust") { // Custom Message
        // here you can write your own code.
        int separatorPosition = inputString.indexOf('/', 11 );
        int messageIdPosition = inputString.indexOf('?', 11 );
        if(messageIdPosition == -1) {
        	messageIdPosition = inputString.length();
        }
        String customId = inputString.substring(11,separatorPosition);
        String value = inputString.substring(separatorPosition + 1, messageIdPosition);
        if(customId == "setbutton") {
        	if(value.startsWith("on/")) {
        		value = value.substring(3, value.length());
                        setToBePressed(value.toInt(), true);
        	} else if(value.startsWith("off/")) {
        		value = value.substring(4, value.length());
                        setToBePressed(value.toInt(), false);
        	} else {
                  msgRecognized = false; // this sketch doesn't know other messages in this case command is ko (not ok)
        	}
        } else if(customId == "getResult") {
          result = getResult();
        } else {
          msgRecognized = false; // this sketch doesn't know other messages in this case command is ko (not ok)
        }
      } else {
        msgRecognized = false; // this sketch doesn't know other messages in this case command is ko (not ok)
      }
      
      // Prepare reply message if caller supply a message id (this is general code you can reuse)
      int idPosition = inputString.indexOf("?id=");
      if(idPosition != -1) {
        String id = inputString.substring(idPosition + 4);
        id.replace("\n", "");
        id.replace("\r", "");
        // print the reply
        Serial.print("alp://rply/");
        if(msgRecognized) { // this sketch doesn't know other messages in this case command is ko (not ok)
          Serial.print("ok?id=");
        } else {
          Serial.print("ko?id=");
        }
        Serial.print(id);
        if(result.length() > 0) {
        	Serial.print("&result=");
        	Serial.print(result);
        }
        Serial.print('\n'); // End of Message
        Serial.flush();
      }
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void setToBePressed(int index, boolean b) {
  toBePressed[index - 1] = b;
  
  buffer[0] = '\0';
  int pos = 0;
  for(int i = 0; i < BUTTON_NUMBER; i++) {
    if(toBePressed[i]) {
      sprintf(&buffer[pos++], "%d", (i + 1));
      buffer[pos++] = ' ';
    }
  }
  buffer[pos] = '\0';
  
  //Clear the virtual buffer
  IOShieldOled.clearBuffer();
  //Chosing Fill pattern 0
  IOShieldOled.setFillPattern(IOShieldOled.getStdPattern(0));
  //Turn automatic updating off
  IOShieldOled.setCharUpdate(0);
  IOShieldOled.setCursor(0, 0);
  IOShieldOled.putString(buffer);
  IOShieldOled.updateDisplay();
}

String getResult() {
  String result = "";
  boolean pressed[BUTTON_NUMBER];

  if(digitalRead(BUTTON_PIN_1) == HIGH) {
    result += "Button 1 on ";
    pressed[0] = true;
  } else {
    result += "Button 1 off ";
    pressed[0] = false;
  }
  if(digitalRead(BUTTON_PIN_2) == HIGH) {
    result += "Button 2 on ";
    pressed[1] = true;
  } else {
    result += "Button 2 off ";
    pressed[1] = false;
  }
  if(digitalRead(BUTTON_PIN_3) == HIGH) {
    result += "Button 3 on ";
    pressed[2] = true;
  } else {
    result += "Button 3 off ";
    pressed[2] = false;
  }
  if(digitalRead(BUTTON_PIN_4) == HIGH) {
    result += "Button 4 on";
    pressed[3] = true;
  } else {
    result += "Button 4 off";
    pressed[3] = false;
  }

  String summary = " RIGHT!";
  for(int i=0; i < BUTTON_NUMBER; i++) {
    if(pressed[i] != toBePressed[i]) {
      summary = " WRONG!";
    }
  }

  return result + summary;
}
