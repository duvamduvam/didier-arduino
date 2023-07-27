import pwmio


class Vibrator:

    def __init__(self, pin):
        self.vibrator = pwmio.PWMOut(pin, frequency=50)
    def update(self):
        pass

    def process(self):
        """for i in range(100):
            # PWM LED up and down
            if i < 50:
                self.vibrator.duty_cycle = int(i * 2 * 65535 / 100)  # Up
            else:
                self.vibrator.duty_cycle = 65535 - int((i - 50) * 2 * 65535 / 100)  # Down
            time.sleep(0.01)"""