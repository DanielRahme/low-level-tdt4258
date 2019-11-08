#define IDLE 0
#define PLAY 1
#define PLAYED 2

typedef struct tSystem
{
    int button;
	int currentState;
	int nextState;
	int soundCoice;
} TSystem;
