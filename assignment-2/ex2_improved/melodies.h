
#ifndef MELODIES_H
#define MELODIES_H
// note length division by 2^n
#define full 0
#define half 1
#define quarter 2
#define eigth 3
#define sixteenth 4

// half period lengths, frequencies are seperated by 2^(1/12)
#define nC 1349
#define nCs 1273
#define nD 1201
#define nDs 1143
#define nE 1070
#define nF 1010
#define nFs 954
#define nG 900
#define nGs 850
#define nA 802
#define nAs 757
#define nB 714
#define nRest 0

#define SAMPLE_PERIOD 317
#define MAX_VOLUME 150

enum tempos {
    FAST_TEMPO = 44100,
    NORMAL_TEMPO = 88200,
    SLOW_TEMPO = 176400
};

typedef struct {
    uint8_t value;          // length of note
    uint16_t noteType;      // f0, e.g. C, G#, B, E
    uint8_t octave;         // how many times the frequency is doubled, f0*2^octave
} note_t;

// Function declarations
note_t selectMelody(uint8_t* desiredMelody, uint16_t* noteCounter);
uint16_t setNoteFreq(note_t currentNote, uint16_t* amplitude);
void playNote(note_t currentNote, uint16_t* amplitude, uint32_t tempo, uint16_t* noteCounter);
void playMelody(uint8_t* desiredMelody, uint16_t* amplitude, uint32_t tempo);

#endif