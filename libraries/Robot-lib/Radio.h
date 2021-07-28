#ifndef RADIO_H
#define RADIO_H

#define RFM95_INT 2
#define RFM95_RST 9
#define RFM95_CS 10
#define LED 13

#define RESET 7

/// start radio def /////

#define RF95_FREQ 868.0
//RH_RF95 rf95(RFM95_CS, RFM95_INT);


class Radio {

  private :
    uint8_t len;

  public :

    char msg[20];
    void init();
    void read();
    void msgset();

    void RfOut();
    void sendCharMsg(char m, char input);
    void sendArrayMsg(char m, char input[]);
    void sendDecMsg(char m, int value);
    void sendVectorMsg(char m, int valueX, int valueY);
    void sendIntMsg(char m, int value);
	void sendMsg(char input[], int size);

};

#endif
