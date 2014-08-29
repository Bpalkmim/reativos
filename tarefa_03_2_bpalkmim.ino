#include "event_driven.c"

const int LED = 13;
const int BUT1 = 2;
const int BUT2 = 4;

void button_changed (int buttonIndex) {
	if (buttons[buttonIndex].state) {
		if (buttonIndex == 0) {
		
		}
		else if (buttonIndex == 0) {
		
		}
	}
}

void timer_expired(int timerIndex) {
	Serial.printf("Timer que expirou: %d\n", timerIndex + 1);
}

void init () {
	pinMode(LED, OUTPUT);
	initializeButtonsArray(2);
	initializeTimersArray(2);
	button_listen(BUT1, 0);
	button_listen(BUT2, 1);
	timer_set(1000, 0);
	timer_set(1000, 1);
}
