#include "FonctionsMega.h"

#define RESET_TIME 5000
#define WATCHDOG_FREQUENCY 100
#define WATCHDOG_KEY "alive"

class WatchDog {

  private :

    long musicCardTime = millis();
    bool newMusicData = false;
    char musicCardMsg[10] = {};

    long headCardTime = millis();
    bool newHeadData = false;
    char headCardMsg[10] = {};

    long lastWatchDogMsg;


  public :

    void execute() {
      newMusicData = recvWithStartEndMarkersMega(musicCardMsg, newMusicData, 10, 2);
      if (newMusicData && !strcmp(musicCardMsg, WATCHDOG_KEY)) {
        musicCardTime = millis();
      }
      newHeadData = recvWithStartEndMarkersMega(headCardMsg, newHeadData, 10, 3);
      if (newHeadData && !strcmp(headCardMsg, WATCHDOG_KEY)) {
        headCardTime = millis();
      }

      if (millis() - lastWatchDogMsg > WATCHDOG_FREQUENCY && millis() - musicCardTime < RESET_TIME && millis() - headCardTime < RESET_TIME) {
        Serial1.println("<"); Serial1.print(WATCHDOG_KEY); Serial1.println(">");
      }

    }

};
