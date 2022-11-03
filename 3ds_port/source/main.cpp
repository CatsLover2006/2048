// 2048 On 3DS
// By Half-Qilin AKA Hail

#include <citro2d.h>
#include <iostream>
#include <time.h>
#include <cmath>

#include "../../2048.cpp"

#define SCREEN_WIDTH  400
#define BOTTOM_SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

const unsigned int currentSaveVersion = 0;
unsigned int saveVersion = 0; // Save Version Update Handling

static C2D_Font regularFont;
static C2D_Font boldFont;
static double pi = acos(-1.0);

u8 consoleModel = 3;
u8 sysRegion = CFG_REGION_USA;
u8 serial = 0;
u8 wideModifier = 2;

int main(int argc, char* argv[]) {
    // Init Libs
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    srand(time(NULL));
	
	// Prepare Game Board
	game_2048::setRandom(rand());
	
	// Game-Unrelated Vars
	unsigned long long highScore;
	unsigned long winCount;
	double animTimer = 0;
    touchPosition touch;
	u32 kDown, kHeld, kUp;
	
	// Load Graphics
	// TODO: add files
	
	// Wide mode
    Result res = cfguInit();
    if (R_SUCCEEDED(res)) {
        CFGU_SecureInfoGetRegion(&sysRegion);
        CFGU_GetSystemModel(&consoleModel);
		CFGI_SecureInfoGetSerialNumber(&serial);
        cfguExit();
    }
    if (consoleModel == 3) wideModifier = 1;
	if (serial == 0) wideModifier = 1;
    gfxSetWide(wideModifier == 2);
	
	// Save Data
    FILE* saveData = fopen("saves/hail_2048.txt","r");
    if (saveData != NULL && fscanf(saveData, "%u~", &saveVersion) == 1) {
    	if (saveVersion == 0) {
			if (fscanf(saveData, "%ullx", &highScore) == 1) {
            	highScore = ~highScore;
        	}
    	}
    }
    fclose(saveData);
    saveData = fopen("saves/hail_2048.txt", "w");
    fprintf(saveData, "%u~%ullx", currentSaveVersion, ~highScore);
    fclose(saveData);
	
	// Main Loop
	while (aptMainLoop())
    {
        animTimer += 1 / 60.0;
		
		// Input
		hidScanInput();
        hidTouchRead(&touch);
		kDown = hidKeysDown();
		kHeld = hidKeysHeld();
		kUp = hidKeysUp();
		
		// Exit
		if (kDown & KEY_START) break;
	}
	
	return 0;
}