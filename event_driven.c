// event_driven.c
#include "Arduino.h"

int NUM_BUTTONS;
int NUM_TIMERS;

typedef struct timer {
	int delay;
	unsigned long beenActive;
} Timer;

typedef struct button {
	int pin;
	int state;
	int pastState;
} Button;

Button * buttons;
Timer * timers;
unsigned long pastTime;

void initializeButtonsArray(int size) {
	NUM_BUTTONS = size;
	buttons = (Button *) malloc(NUM_BUTTONS * sizeof(Button));
}
void initializeTimersArray(int size) {
	NUM_TIMERS = size;
	timers = (Timer *) malloc(NUM_TIMERS * sizeof(Timer));
}

/* Funções de registro: */
void button_listen (int pin, int buttonIndex) {
	pinMode(pin, INPUT);
	buttons[buttonIndex].pin = pin;
	buttons[buttonIndex].pastState = 0;
	buttons[buttonIndex].state = digitalRead(pin);
}

void timer_set (int ms, int timerIndex) {
	timers[timerIndex].delay = ms;
	timers[timerIndex].beenActive = millis();
}

/* Callbacks */
void button_changed (int buttonIndex);
void timer_expired (int timerIndex);

/* Programa principal: */
void setup () {
	pastTime = millis();
	initialize();
}

void loop () {
	
	unsigned long currTime = millis();
	int i;
	
	// Verifica se os botões estão apertados
	for (i = 0; i < NUM_BUTTONS; i++) {
		buttons[i].state = digitalRead(buttons[i].pin);
		if (buttons[i].state != buttons[i].pastState) {
			button_changed(i);
			buttons[i].pastState = buttons[i].state;
		}
	}

	// Verifica se os timers estão ativos
	for (i = 0; i < NUM_TIMERS; i++) {
		if(currTime >= timers[i].beenActive + timers[i].delay) {
			timer_expired(i);
			timers[i].beenActive = currTime;
		}
	}
}
