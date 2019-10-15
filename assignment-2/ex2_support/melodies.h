
#ifndef MELODIES_H
#define MELODIES_H
// note legth division by 2^n
#define full    0
#define half    1
#define quarter 2
#define eigth   3
#define sixteenth 4

// half period lengths
#define nC  1349
#define nCs 1273
#define nD  1201
#define nDs 1143
#define nE  1070
#define nF  1010
#define nFs 954
#define nG  900
#define nGs 850
#define nA  802
#define nAs 757
#define nB  714
#define nRest 0

#define SAMPLE_PERIOD 317
#define MAX_VOLUME 150
typedef struct {
	uint8_t value;
	uint16_t noteType;
	uint8_t octave;
} note_t;


// Function declarations
bool pollTimer();
note_t selectMelody(uint8_t *desiredMelody, uint16_t *noteCounter);
uint16_t setNoteFreq(note_t currentNote, uint16_t *amplitude);
void playNote(note_t currentNote, uint16_t *amplitude, uint32_t tempo,uint16_t *noteCounter);
void playMelody(uint8_t *desiredMelody, uint16_t *amplitude, uint32_t tempo);


#endif