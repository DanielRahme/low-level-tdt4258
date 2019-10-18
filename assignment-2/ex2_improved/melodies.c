#include <stdbool.h>
#include <stdint.h>
#include "melodies.h"
#include "buttons.h"
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

const note_t shittyTones[] = {
    { sixteenth, nD, 4 },
    { sixteenth, nC, 2 },
    { eigth, nA, 1 },
    { eigth, nC, 4 },
    { quarter, nF, 2 },
    { sixteenth, nAs, 7 },
    { sixteenth, nC, 2 },
    { eigth, nC, 4 },
    { sixteenth, nD, 4 },
    { eigth, nCs, 4 },
    { sixteenth, nG, 4 },
    { eigth, nFs, 4 },
    { sixteenth, nC, 4 },
    { eigth, nCs, 7 },
    { eigth, nC, 7 },
    { sixteenth, nDs, 7 },
    { sixteenth, nD, 7 },
    { eigth, nE, 2 },
    { sixteenth, nDs, 2 },
    { sixteenth, nB, 2 },
    { eigth, nB, 2 },
    { sixteenth, nA, 7 },
    { sixteenth, nDs, 7 },
    { quarter, nG, 1 },
    { eigth, nF, 1 },
};

const note_t soundFx[] = {
    { eigth, nDs, 7 },
    { sixteenth, nE, 1 },
    { sixteenth, nA, 3 },
    { eigth, nE, 3 },
    { quarter, nB, 5 },
};

const note_t boink[] = {
    { sixteenth, nC, 6 },
    { quarter, nA, 6 }
};


/**
 * @brief Select which melody to play depending on 
 * selected melody
 */ 
note_t selectMelody(uint8_t* desiredMelody, uint16_t* noteCounter)
{
    if (*desiredMelody == 0) {
        *noteCounter = 0;
        *GPIO_PA_DOUT = 0xffff;    // turn off leds
        return (note_t){ full, nRest, 0 };
    }

    // Play happy birthday
    else if (*desiredMelody == BUTTON0) {
        if (*noteCounter >= (sizeof(happyBDay) / sizeof(happyBDay[0]))) {
            *noteCounter = 0;
            *desiredMelody = 0; 
        }
        return happyBDay[*noteCounter];

    // Play fanfare
    } else if (*desiredMelody == BUTTON3) {
        if (*noteCounter >= (sizeof(fanfare) / sizeof(fanfare[0]))) {
            *noteCounter = 0;
            *desiredMelody = 0;
        }
        return fanfare[*noteCounter];

    // Play shitty notes
    } else if (*desiredMelody == BUTTON2) {
        if (*noteCounter >= (sizeof(shittyTones) / sizeof(shittyTones[0]))) {
            *noteCounter = 0;
            *desiredMelody = 0;
        }
        return shittyTones[*noteCounter];

    // Play sound effect
    } else if (*desiredMelody == BUTTON1) {
        if (*noteCounter >= (sizeof(soundFx) / sizeof(soundFx[0]))) {
            *noteCounter = 0;
            *desiredMelody = 0;
        }
        return soundFx[*noteCounter];

    // Play boink
    } else if (*desiredMelody == BUTTON4) {
        if (*noteCounter >= (sizeof(boink) / sizeof(boink[0]))) {
            *noteCounter = 0;
            *desiredMelody = 0;
        }
        return boink[*noteCounter];

    } else if (*desiredMelody == BUTTON7) {
        if (*noteCounter >= (sizeof(shittyTones) / sizeof(shittyTones[0]))) {
            *noteCounter = 0;
            *desiredMelody = BUTTON7;
        }
        return shittyTones[*noteCounter];
    }
    return (note_t){ full, nRest, 0 };
}

uint16_t setNoteFreq(note_t currentNote, uint16_t* amplitude)
{
    static bool onOff = 0;  // used to generate square wave
    static uint16_t periodCounter = 0;      // used to count up to the desired period
    if (periodCounter >= currentNote.noteType >> (currentNote.octave)) {  // when one half period is complete
        onOff = !onOff;                                                   // toggle square wave  ___|||___|||___|||   
        periodCounter = 0;                                                // reset to begin counting the next half period
        *amplitude = *amplitude > 0 ? *amplitude - 1 : 0;                 // add some decay to the note to make it a little more natural
    }
    periodCounter++;                    // count the amount of samples since the start of the half period
    return *amplitude * (onOff & 1);    // return amplified square wave
}

void playNote(note_t currentNote, uint16_t* amplitude, uint32_t tempo, uint16_t* noteCounter)
{
    uint16_t data = 0;                  // output data
    static uint32_t noteDuration = 0;   // keeps track of the length of the note
    if (noteDuration < (tempo >> currentNote.value)) {  // if the note is still not finished
        if (currentNote.noteType != nRest) {            // and the current note is not a pause
            data = setNoteFreq(currentNote, amplitude); // creates square wave with desired amplitude and frequency
        }
    }
    else {   // re-initialize note when note complete
        (*noteCounter)++;  // move note pointer to the next note
        noteDuration = 0;  // reset note length counter
        *amplitude = MAX_VOLUME;    // set the amplitude back to max volume
    }
    
    noteDuration++;  // increment note duration counter
     // output data to DAC channels 1 and 2
    *DAC0_CH0DATA = data;  
    *DAC0_CH1DATA = data;
}

void playMelody(uint8_t* desiredMelody, uint16_t* amplitude, uint32_t tempo)
{
    extern volatile bool playOneSample; 
    static uint16_t noteCounter = 0;        // keeps track of which note in the melody it is on
    note_t currentNote;                     // contains information about the current note
    if(playOneSample){                      // if it is time to play one sample
        currentNote = selectMelody(desiredMelody, &noteCounter);    // fetch the correct note from the desired melody 
        playNote(currentNote, amplitude, tempo, &noteCounter);      // play the desired note
        playOneSample = false;  // mark that the sample has been played
    }
}
