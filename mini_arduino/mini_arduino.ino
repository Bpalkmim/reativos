// Mini projeto Arduino: Teclado de transposição
//
// Bernardo Alkmim
// Leonardo Kaplan

#include "pitches.h"
// Macro para criação da matriz com os valores definidos em pitches.h
#define not_o(x) NOTE_C##x, NOTE_CS##x, NOTE_D##x, NOTE_DS##x, NOTE_E##x, NOTE_F##x, NOTE_FS##x, NOTE_G##x, NOTE_GS##x, NOTE_A##x, NOTE_AS##x, NOTE_B##x
#define notes_xxx not_o(1),not_o(2),not_o(3),not_o(4),not_o(5),not_o(6)

#define NUMBER_OCTAVES 6
#define NOTES_OCTAVE 12
#define NUMBER_KEYS 8
#define BUZZER 8

int tuning = 0;

int notes[NUMBER_OCTAVES * NOTES_OCTAVE] = {notes_xxx};

int octave_current = 3;


void octave_up(){
  octave_current = (octave_current + 1) % NUMBER_OCTAVES;
}
void octave_down(){
  octave_current = (octave_current - 1 + NUMBER_OCTAVES) % NUMBER_OCTAVES;
}

void tuning_up() {
  tuning = (tuning + 1) % NOTES_OCTAVE;
}
void tuning_down() {
  tuning = (tuning - 1 + NOTES_OCTAVE) % NOTES_OCTAVE;
}

void setkey(int n) {
  digitalWrite(2, bitRead(n, 2));
  digitalWrite(3, bitRead(n, 1));
  digitalWrite(4, bitRead(n, 0));
}

void play(int n){
   // Deslocamento da nota em função da oitava atual e da afinação
  int shift = NOTES_OCTAVE * octave_current + tuning;
  n = *(notes + n + shift);
  int noteDuration = 1000/4;
  tone(BUZZER, n, noteDuration);

  // to distinguish the notes, set a minimum time between them.
  // the note's duration + 30% seems to work well:
  int pauseBetweenNotes = noteDuration * 1.30;
  delay(pauseBetweenNotes);
  // stop the tone playing:
  noTone(BUZZER);
  
}

int ket;
void setup() {

  //Serial.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  setkey(0);
  pinMode(9, INPUT);
  key = 0;
}

void loop() {
  setkey(key);

  int sensorValue = digitalRead(9);
  
  if(sensorValue == 0) {
    play(key % NUMBER_KEYS);
  }
  
  key++;
}
