

class Vibrator {

private :
	int pin;
	long timer;
	int strengh;


public :
	Vibrator(int p);
	void execute(int duration, int s);
	void process();

};

