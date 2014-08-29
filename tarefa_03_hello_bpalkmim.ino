#include "event_driven.c"

const int LED = 13;
const int BUT = 2;

void button_changed (int buttonIndex) {
	if (buttons[buttonIndex].pin == BUT) {
		digitalWrite(LED, buttons[buttonIndex].state);
	}
}

void timer_expired(int timerIndex) {
  // faz nada, não há timers
}

void initialize() {
	pinMode(LED, OUTPUT);
	initializeButtonsArray(1);
	initializeTimersArray(0);
	button_listen(BUT, 0);
}
