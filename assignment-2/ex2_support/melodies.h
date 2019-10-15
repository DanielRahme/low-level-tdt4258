#define full    0
#define half    1
#define quarter 2
#define eigth   3
#define sixteenth 4


struct note
{
	uint8_t value;
	uint8_t noteType;
	uint8_t octave;
};

enum notes				{nC  , nCs , nD  , nDs , nE  , nF  , nFs, nG , nGs, nA , nAs, nB , nRest};
uint16_t halfPeriod[] = {1349, 1273, 1201, 1134, 1070, 1010, 954, 900, 850, 802, 757, 714, 0};
struct note currentNote;
uint16_t periodCounter;
uint16_t amplitude;
uint16_t data;
uint32_t tempo;
uint32_t noteDuration;
uint16_t noteCounter;
//bool onOff;
bool noteComplete;


const struct note fanfare[] = {
	{sixteenth, nG, 4},
	{eigth, 	nC, 5},
	{sixteenth, nG, 4},
	{eigth, 	nD, 5},
	{sixteenth, nG, 4},
	{eigth, 	nE, 5},
	{sixteenth, nG, 4},
	{eigth, 	nD, 5},
	{sixteenth, nG, 4},
	{eigth, 	nC, 5},
	{sixteenth, nG, 4},
	{eigth, 	nD, 5},
	{sixteenth, nG, 4},
	{eigth, 	nE, 5}
};

void selectMelody(){
    if (noteCounter > 14){
		noteCounter = 0;
	}
	currentNote = fanfare[noteCounter];
}

void setNoteFreq(){
	static bool onOff = 0;
	if (periodCounter >= halfPeriod[currentNote.noteType] >> currentNote.octave ){
		onOff = !onOff;
		periodCounter = 0;
        amplitude = amplitude > 0 ? amplitude - 1 : 0;       //add some decay to the note to make it a little more natural
	}
	periodCounter++;
	data = amplitude*(onOff & 1);
}

void playNote(){	
	if (noteDuration < (tempo >> currentNote.value) ){
		if (currentNote.noteType != nRest){	
			setNoteFreq();
		}
		else{
			data = 0;
		}
	}
	else{
		noteCounter++;
		noteDuration = 0;
        amplitude = MAX_VOLUME;
	}
	noteDuration++;
	*DAC0_CH0DATA = data;
	*DAC0_CH1DATA = data;
}

void visualizer(){
	*GPIO_PA_DOUT = !(0xff00>>currentNote.noteType);		
	}