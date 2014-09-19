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

int tuning = 0;

int notes[NUMBER_OCTAVES * NOTES_OCTAVE] = {notes_xxx};

int octave_current = 3;

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4,4,4 };
void octave_up(){
  octave_current = (octave_current + 1) % NUMBER_OCTAVES;
}
void octave_down(){
  octave_current = (octave_current - 1 + NUMBER_OCTAVES) % NUMBER_OCTAVES;
}
void setup() {
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  //Serial.begin(9600);
}
void play(){
   // Deslocamento da nota em função da oitava atual e da afinação
  int shift = NOTES_OCTAVE * octave_current + tuning;
  
  int melody[] = {*(notes + shift + 0), *(notes + shift + 2), *(notes + shift + 4)};
 
  //Serial.println(melody[0]);
  //Serial.println(melody[1]);
  //Serial.println(melody[2]);
  
  for (int thisNote = 0; thisNote < 3; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(8, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}
void loop() {
  if(!digitalRead(2)) octave_up();
  if(!digitalRead(3)) octave_down();
  play(); 
}
