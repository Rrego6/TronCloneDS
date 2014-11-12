/*---------------------------------------------------------------------------------

	Tron LightCycle game for DS
---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include "text.h"
//using namespace std;

//---------------------------------------------------------------------------------
#define BLACK           ARGB16(0,0,0,0)
#define BLUE            ARGB16(0xFF,00,00,0xFF)
#define BROWN			ARGB16(0xFF,0xA5,0x2A,0x2A)
#define RED             ARGB16(0xFF,0xFF,00,00)
#define ORANGE			ARGB16(0xFF,0xFF,0xA5,00)
#define GREEN           ARGB16(0xFF,00,0x80,00)
#define CYAN            ARGB16(0xFF,00,0xFF,0xFF)
#define MAGENTA         ARGB16(0xFF,0xFF,00,0xFF)
#define YELLOW          ARGB16(0xFF,0xFF,0xFF,00)
#define WHITE           ARGB16(0xFF,0xFF,0xFF,0xFF)

#define HEIGHT			3

typedef enum {UP=0, RIGHT=1, DOWN=3, LEFT=4 } direction;

typedef struct
{
    int width;
    int height;
    u16 color;
	direction dir;
    struct Coordinates
    {
        int x;
        int y;
    }CD;
} Snake;
//---------------------------------------------------------------------------------
void clearScreen            ( u16* videoMemory );
void clearScreens           ( u16* videoMemoryMain, u16* videoMemorySub );
void drawRect               ( u16* videoMemory, int x, int y, int width, int height, u16 color );
void startLoop              (u16* videoMemoryMain, u16* videoMemorySub );
void quickFlashStartOption  ( u16* videoMemorySub );
void drawSnake              ( u16* videoMemory, Snake snake);
void gameLoop               (u16* videoMemoryMain, u16* videoMemorySub);
void drawBackground		    (u16* videoMemoryMain, u16* videoMemorySub);
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();
	//set the mode to allow for an extended rotation background
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D);

	//allocate a vram bank for each display
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	//create a background on each display
	int bgMain = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	int bgSub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);

	u16* videoMemoryMain = bgGetGfxPtr(bgMain);
	u16* videoMemorySub = bgGetGfxPtr(bgSub);

	clearScreens(videoMemoryMain, videoMemorySub);
	startLoop(videoMemoryMain, videoMemorySub);

	clearScreens(videoMemoryMain, videoMemorySub);
	gameLoop(videoMemoryMain, videoMemorySub);

	while (1) {
		scanKeys();
		switch (keysDown()) {
			case KEY_A:
				//iprintf("KEY_A pressed\n");
				break;
			case KEY_B:
				//iprintf("KEY_B pressed\n");
				break;
			case KEY_RIGHT:
				iprintf("KEY_RIGHT pressed\n");
				break;
			case KEY_LEFT :
				iprintf("KEY_LEFT pressed\n");
				break;
			case KEY_UP:
				iprintf("KEY_UP pressed\n");
				break;
			case KEY_DOWN:
				iprintf("KEY_DOWN pressed\n");
				break;
			case KEY_TOUCH:
				iprintf("KEY_TOUCH pressed\n");
				break;
		}
		swiWaitForVBlank();
	}
}

void clearScreen( u16* videoMemory ) {
	for(int x = 0; x < 256; x++)
    for(int y = 0; y < 256; y++)
        videoMemory[x + y * 256] = BLACK;
}

void clearScreens( u16* videoMemoryMain, u16* videoMemorySub ) {
	clearScreen(videoMemoryMain);
	clearScreen(videoMemorySub);
}

void drawRect( u16* videoMemory, int x, int y, int width, int height, u16 color) {
    for (int i = y; i <= height; i++)
        for (int j = x; j <= width;j++)
			        videoMemory[x + y * 256] = WHITE;
}

void startLoop(u16* videoMemoryMain, u16* videoMemorySub ) {
	drawString(videoMemoryMain,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"TRON", WHITE, MEDIUM);
	int timer = 0;
	bool cont = true;
	while(cont) {
		timer++;
		if( timer > 90)
			timer = 0;
		if( !timer ) {
			clearScreen( videoMemorySub );
		}
		else if(timer > 45) {
			drawString(videoMemorySub,55,100,"PRESS START TO BEGIN", RED, TINY);
		}
		scanKeys();
		if(keysDown() & KEY_START ) {
			quickFlashStartOption(videoMemorySub);
			cont = false;
		}
		swiWaitForVBlank();
	}
}
void quickFlashStartOption(u16* videoMemorySub ) {
	for(int i = 0; i < 35; i++ ) {
		if(i%10 >= 5) {
			drawString(videoMemorySub,55,100,"PRESS START TO BEGIN", RED, TINY);
		} else {
			clearScreen(videoMemorySub);
		}
		swiWaitForVBlank();
	}
	clearScreen(videoMemorySub);
	swiWaitForVBlank();
}
void gameLoop(u16* videoMemoryMain, u16* videoMemorySub) {
	drawBackground(videoMemoryMain, videoMemorySub);

	//init Snake
	Snake snake;
    Snake enemy;

	snake.CD.x = 15;
    snake.CD.y = 100;
    snake.color = CYAN;
	snake.dir = UP;


    enemy.CD.x = 200;
    enemy.CD.y = 100;
    enemy.color = MAGENTA;
	u16* currentMemory = videoMemoryMain;
	while(1) {
		drawSnake(currentMemory, snake);
		scanKeys();
		switch (keysDown()) {
			case KEY_RIGHT:
				if(snake.dir != LEFT)
					snake.dir = RIGHT;
				break;
			case KEY_LEFT :
				if(snake.dir != RIGHT)
				snake.dir = LEFT;
				break;
			case KEY_UP:
				if(snake.dir != DOWN)
				snake.dir = UP;
				break;
			case KEY_DOWN:
				if(snake.dir != UP)
				snake.dir = DOWN;
				break;
		}
		switch (snake.dir) {
			case UP:
				if(currentMemory == videoMemorySub && snake.CD.y == 0) {
					currentMemory = videoMemoryMain;
					snake.CD.y = SCREEN_HEIGHT;
					break;
				}
				snake.CD.y--;
				break;
			case RIGHT :
				snake.CD.x++;
				break;
			case DOWN:
				if(currentMemory == videoMemoryMain && snake.CD.y == SCREEN_HEIGHT) {
					currentMemory = videoMemorySub;
					snake.CD.y = 0;
					break;
				}
				snake.CD.y++;
				break;
			case LEFT:
				snake.CD.x--;
				break;
		}
		swiWaitForVBlank();
	}
}
void drawSnake(u16* videoMemory, Snake snake) {
	for(int i = 0; i < 3; i++ )
		for(int j = 0; j < 3; j++ )
			videoMemory[(snake.CD.x + i) + (snake.CD.y + j) * 256] = CYAN;
}
void drawBackground(u16* videoMemoryMain, u16* videoMemorySub) {
	for (int i = 0; i < 3; i++ ) {
		for (int j = 0; j < SCREEN_HEIGHT; j++ ) {
			videoMemoryMain[i + j *256] = WHITE;
			videoMemoryMain[SCREEN_WIDTH - i + j *256] = WHITE;
			videoMemorySub[i + j *256] = WHITE;
			videoMemorySub[SCREEN_WIDTH - i + j *256] = WHITE;
		}
	}
	for (int i = 0; i < SCREEN_WIDTH; i++ ) {
		for (int j = 0; j < 3; j++ ) {
			videoMemoryMain[i + j *256] = WHITE;
			videoMemorySub[i + (SCREEN_HEIGHT-j) *256] = WHITE;

		}
	}
}

void pauseGame(u16* videoMemoryMain, u16* videoMemorySub) {
}
