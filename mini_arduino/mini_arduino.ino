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
#define NUM_KEYS 8
#define BUZZER 8
#define DEBOUNCE 30

int tuning = 0;

int notes[NUMBER_OCTAVES * NOTES_OCTAVE] = {notes_xxx};

int octave_current = 3;

// note durations: 4 = quarter note, 8 = eighth note, etc.:
void octave_up(){
  octave_current = (octave_current + 1) % NUMBER_OCTAVES;
}
void octave_down(){
  octave_current = (octave_current - 1 + NUMBER_OCTAVES) % NUMBER_OCTAVES;
}
void tuning_down(){
    tuning  = (tuning - 1 + NOTES_OCTAVE) %  NOTES_OCTAVE;
}
void tuning_up(){
    tuning  = (tuning + 1) % NOTES_OCTAVE;
}
void setkey(int n){
    digitalWrite(2,bitRead(n,2));
    digitalWrite(3,bitRead(n,1));
    digitalWrite(4,bitRead(n,0));

}
int noteDurations[] = {4,4,4};
void play(int n){
   // Deslocamento da nota em função da oitava atual e da afinação
  int shift = NOTES_OCTAVE * octave_current + tuning;
  n = n + shift;
  int melody[] = {*(notes + shift + 0), *(notes + shift + 2), *(notes + shift + 4)};
 
  //Serial.println(melody[0]);
  //Serial.println(melody[1]);
  //Serial.println(melody[2]);
 
  //for (int thisNote = 0; thisNote < 3; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    //int noteDuration = 1000/noteDurations[thisNote];
    //tone(8, melody[thisNote],noteDuration);
    //int noteDuration = 1000/4;
    //noTone(BUZZER);
    tone(BUZZER, n);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    //int pauseBetweenNotes = noteDuration * 1.30;
    //delay(pauseBetweenNotes);
    //delay(30);// stop the tone playing:
    //noTone(8);
    //noTone(BUZZER);
  //}
}
int key;
char s[20];
int Boctave,Btuning;
int tocando;
int Batual;
unsigned long currTime;
unsigned long lastTime;
void setup() {
    Serial.begin(9600);
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    setkey(0);
    pinMode(9,INPUT);
    key = 0;
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);
    tocando = -1;
    lastTime = millis();
    currTime = lastTime;
}

int getnota(int estado[],int tam){
  int i = 0;
  int flag=0;
  int nota;
  for(;i<tam;i++){
    if(estado[i]==0){
      if(!flag){
        nota =i;
        flag=1;
      }
      else return -1;
    }
  }
  if(flag){
    return nota;
  }
  return -1;
}

void loop() {
    currTime = millis();
   
    if (currTime > lastTime + DEBOUNCE) {
      Boctave = analogRead(A0);
      Btuning = analogRead(A1);
      float Bo= (1023.0-(float)Boctave)/1023.0;
      octave_current = Bo*NUMBER_OCTAVES;
      float Bt= (1023.0-(float)Btuning)/1023.0;
      tuning = Bt*NOTES_OCTAVE;   
   
   
      int estado[NUM_KEYS];
      int i;
      for(i = 0;i<NUM_KEYS;i++){
        setkey(key);
        estado[key%NUM_KEYS] = digitalRead(9);
        key++;
      }
      //sprintf(s,"%d:%d - %d/%d",key%NUM_KEYS,Bkey,octave_current, tuning);
      int nota = getnota(estado,NUM_KEYS);
      Serial.println(nota);

      if(nota==-1 || (tocando != nota && tocando != -1))
      {
        noTone(BUZZER);
        tocando = -1; 
      }
      else
      {
        play(nota);
        tocando = nota;
      }
      //dt++;
      //delay(100);  
      lastTime = currTime;
    } 
}
