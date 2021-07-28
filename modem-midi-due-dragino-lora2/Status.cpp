

class Status {

  private :

  public :
    char infos[20] = {};
    bool voice = false;


    void update() {
      infos[0] = 'S';
      if (voice) {
        infos[1] = 'I';
      } else {
        infos[1] = '0';
      }
      infos[2] = '\0';
    }

};
