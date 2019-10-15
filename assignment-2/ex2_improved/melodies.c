#include <stdbool.h>
#include <stdint.h>
#include "melodies.h"
#include "efm32gg.h"

const note_t fanfare[] = {
    { sixteenth, nG, 4 },
    { eigth, nC, 5 },
    { sixteenth, nG, 4 },
    { eigth, nD, 5 },
    { sixteenth, nG, 4 },
    { eigth, nE, 5 },
    { sixteenth, nG, 4 },
    { eigth, nD, 5 },
    { sixteenth, nG, 4 },
    { eigth, nC, 5 },
    { sixteenth, nG, 4 },
    { eigth, nD, 5 },
    { sixteenth, nG, 4 },
    { eigth, nE, 5 }
};
const note_t happyBDay[] = {
    { sixteenth, nC, 4 },
    { sixteenth, nC, 4 },
    { eigth, nD, 4 },
    { eigth, nC, 4 },
    { quarter, nF, 4 },
    { eigth, nE, 4 },
    { eigth, nC, 4 },
    { eigth, nC, 4 },
    { eigth, nD, 4 },
    { eigth, nC, 4 },
    { quarter, nG, 4 },
    { eigth, nF, 4 },
    { eigth, nC, 4 },
    { eigth, nC, 4 },
    { eigth, nC, 5 },
    { eigth, nA, 4 },
    { eigth, nF, 4 },
    { eigth, nE, 4 },
    { eigth, nD, 4 },
    { eigth, nB, 4 },
    { eigth, nB, 4 },
    { quarter, nA, 4 },
    { eigth, nF, 4 },
    { eigth, nG, 4 },
    { eigth, nF, 4 },
};


note_t selectMelody(uint8_t* desiredMelody, uint16_t* noteCounter)
{

    if (*desiredMelody == 0) {
        *noteCounter = 0;
        return (note_t){ full, nRest, 0 };
    }
    // Play happy birthday
    else if (*desiredMelody == 1) {
        if (*noteCounter >= (sizeof(happyBDay) / sizeof(happyBDay[0]))) {
            *noteCounter = 0;
            *desiredMelody = 0;
        }
        return happyBDay[*noteCounter];
    }
    // Play fanfare
    else if (*desiredMelody == 2) {
        if (*noteCounter >= (sizeof(fanfare) / sizeof(fanfare[0]))) {
            *noteCounter = 0;
            *desiredMelody = 0;
        }
        return fanfare[*noteCounter];
    }
    return (note_t){ full, nRest, 0 };
}

uint16_t setNoteFreq(note_t currentNote, uint16_t* amplitude)
{
    static bool onOff = 0;
    static uint16_t periodCounter = 0;
    if (periodCounter >= currentNote.noteType >> (currentNote.octave)) {
        onOff = !onOff;
        periodCounter = 0;
        *amplitude = *amplitude > 0 ? *amplitude - 1 : 0; //add some decay to the note to make it a little more natural
    }
    periodCounter++;
    return *amplitude * (onOff & 1);
}

void playNote(note_t currentNote, uint16_t* amplitude, uint32_t tempo, uint16_t* noteCounter)
{
    uint16_t data = 0;
    static uint32_t noteDuration = 0;
    if (noteDuration < (tempo >> currentNote.value)) {
        if (currentNote.noteType != nRest) {
            data = setNoteFreq(currentNote, amplitude);
        }
    }
    else { // re-initialize note
        (*noteCounter)++;
        noteDuration = 0;
        *amplitude = MAX_VOLUME;
    }
    noteDuration++;
    *DAC0_CH0DATA = data;
    *DAC0_CH1DATA = data;
}

void playMelody(uint8_t* desiredMelody, uint16_t* amplitude, uint32_t tempo)
{
    extern volatile bool playOneSample;
    static uint16_t noteCounter = 0;
    note_t currentNote;
    if(playOneSample){
        currentNote = selectMelody(desiredMelody, &noteCounter);
        playNote(currentNote, amplitude, tempo, &noteCounter);
        playOneSample = false;  //needed for interrupt implementation
        //visualizer(currentNote);
    }
}

/*
void visualizer(note_t currentNote){
	uint8_t shift = 0;
	shift =  (currentNote.noteType == nG) ? 1 : 0;
	shift =  (currentNote.noteType == nA) ? 2 : 0;
	shift =  (currentNote.noteType == nB) ? 3 : 0;
	shift =  (currentNote.noteType == nC) ? 4 : 0;
	shift =  (currentNote.noteType == nD) ? 5 : 0;
	shift =  (currentNote.noteType == nE) ? 6 : 0;
	shift =  (currentNote.noteType == nF) ? 7 : 0;

	*GPIO_PA_DOUT = (0xffff>>shift);		
	}
*/