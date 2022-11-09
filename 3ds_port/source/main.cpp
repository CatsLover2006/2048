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

static C2D_Font font300;
static C2D_Font font400;
static C2D_Font font500;
static C2D_Font font600;
static C2D_Font font700;
static C2D_Font font800;
static C2D_Font font900;
const double pi = acos(-1.0);
const double max_time = 0.8 / pi / pi;

u8 wideModifier = 2;

static u32 color[13][2];
static u32 clrClear, clrWin, clrWhite;

enum gameStates{
    MAIN,
    MENU
};

void drawTileAtLocation (int xLocation, int yLocation, uint8_t value) {
    C2D_DrawRectSolid(xLocation, yLocation, (value==0)?0.2f:0.3f, 48, 48, (value>12)?clrWin:color[value][0]);
    if (value == 0) return;
	C2D_Text numberText;
    C2D_TextBuf numberBuf = C2D_TextBufNew(11);
    char val[11];
    if (value <= 31) {
        unsigned long l = 1;
        snprintf(val, 11, "%lu", (l << value));
    } else {
        snprintf(val, 6, "2^%u", value);
    }
    C2D_TextFontParse(&numberText, (value>24&&value<31)?font600:font500, numberBuf, val);
    C2D_DrawText(&numberText, C2D_AlignCenter | C2D_AtBaseline | C2D_WithColor, xLocation + 24, yLocation + 30, 0.4f, (value>13&&value<32)?((value>19)?((value>24)?((value>29)?0.32f:0.34f):0.4f):0.48f):0.64f, 0.64f, (value>12)?clrWhite:color[value][1]);
    C2D_TextBufDelete(numberBuf);
}

void drawScaledTileAtLocation (int xLocation, int yLocation, uint8_t value, float scalingFactor) {
    C2D_DrawRectSolid(xLocation + 24 * (1-scalingFactor), yLocation + 24 * (1-scalingFactor), (value==0)?0.2f:0.3f, 48 * scalingFactor, 48 * scalingFactor, (value>12)?clrWin:color[value][0]);
    if (value == 0) return;
	C2D_Text numberText;
    C2D_TextBuf numberBuf = C2D_TextBufNew(11);
    char val[11];
    if (value <= 31) {
        unsigned long l = 1;
        snprintf(val, 11, "%lu", (l << value));
    } else {
        snprintf(val, 6, "2^%u", value);
    }
    C2D_TextFontParse(&numberText, (value>24&&value<31)?font600:font500, numberBuf, val);
    C2D_DrawText(&numberText, C2D_AlignCenter | C2D_AtBaseline | C2D_WithColor, xLocation + 24 + 24 * (1-scalingFactor), yLocation + 30 + 24 * (1-scalingFactor), 0.4f, ((value>13&&value<32)?((value>19)?((value>24)?((value>29)?0.32f:0.34f):0.4f):0.48f):0.64f) * scalingFactor, 0.64f * scalingFactor, (value>12)?clrWhite:color[value][1]);
    C2D_TextBufDelete(numberBuf);
}

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
	
	// Other Game Vars
	unsigned long long highScore;
	unsigned long winCount;
	double animTimer = 0;
    gameStates cGameState = MAIN;
        
    // Input Vars
    touchPosition touch;
	u32 kDown, kHeld, kUp;
    float timeHeld[4];
    for (int i = 0; i < 4; i++) timeHeld[i] = 0;
	
	// Load Fonts
	font300 = C2D_FontLoad("romfs:/gfx/Figtree-Light.bcfnt");
	font400 = C2D_FontLoad("romfs:/gfx/Figtree-Regular.bcfnt");
	font500 = C2D_FontLoad("romfs:/gfx/Figtree-Medium.bcfnt");
	font600 = C2D_FontLoad("romfs:/gfx/Figtree-SemiBold.bcfnt");
	font700 = C2D_FontLoad("romfs:/gfx/Figtree-Bold.bcfnt");
	font800 = C2D_FontLoad("romfs:/gfx/Figtree-ExtraBold.bcfnt");
	font900 = C2D_FontLoad("romfs:/gfx/Figtree-Black.bcfnt");
	
    // Create Colors
    clrClear = C2D_Color32(230, 234, 235, 0xFF);
    clrWhite = C2D_Color32(255, 255, 255, 0xFF);
    clrWin = C2D_Color32(90, 161, 237, 0xFF);
    for (int i = 0; i < 11; i++) {
        color[i][1] = C2D_Color32(0, 0, 0, 0xFF);
    }
    for (int i = 11; i < 13; i++) {
        color[i][1] = C2D_Color32(239, 252, 121, 0xFF);
    }
    color[0][0] = clrClear;
    color[1][0] = C2D_Color32(203, 224, 148, 0xFF);
    color[2][0] = C2D_Color32(171, 224, 148, 0xFF);
    color[3][0] = C2D_Color32(148, 224, 167, 0xFF);
    color[4][0] = C2D_Color32(148, 209, 224, 0xFF);
    color[5][0] = C2D_Color32(148, 176, 224, 0xFF);
    color[6][0] = C2D_Color32(205, 148, 224, 0xFF);
    color[7][0] = C2D_Color32(224, 148, 208, 0xFF);
    color[8][0] = C2D_Color32(214, 144, 168, 0xFF);
    color[9][0] = C2D_Color32(214, 149, 144, 0xFF);
    color[10][0] = C2D_Color32(237, 163, 114, 0xFF);
    color[11][0] = C2D_Color32(15, 105, 115, 0xFF);
    color[12][0] = C2D_Color32(15, 58, 115, 0xFF);
	
	// Wide mode
    Result res = cfguInit();
    if (R_SUCCEEDED(res)) {
        CFGU_GetModelNintendo2DS(&wideModifier);
        cfguExit();
    }
    gfxSetWide(wideModifier == 2);
	
	// Create Screens
    C3D_RenderTarget * top_main = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget * bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	
	// Save Data
    FILE* saveData = fopen("./saves/hail_2048.txt","r");
    /*if (saveData != NULL && fscanf(saveData, "%u~", &saveVersion) == 1) {
    	switch (saveVersion) {
            case 0: {
                if (fscanf(saveData, "%llx", &highScore) == 1) {
                    highScore = ~highScore;
                }
                break;
            }
        }
    }
    fclose(saveData);*/
    saveData = fopen("./saves/hail_2048.txt", "w");
    fprintf(saveData, "%u~%llx", currentSaveVersion, ~highScore);
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
        
        switch (cGameState) {
            case MAIN: {
                // Game Handling
                if (kHeld & KEY_UP) timeHeld[0] += 1 / 60.0f;
                else timeHeld[0] = 0;
                if (kHeld & KEY_LEFT) timeHeld[1] += 1 / 60.0f;
                else timeHeld[1] = 0;
                if (kHeld & KEY_DOWN) timeHeld[2] += 1 / 60.0f;
                else timeHeld[2] = 0;
                if (kHeld & KEY_RIGHT) timeHeld[3] += 1 / 60.0f;
                else timeHeld[3] = 0;
                if (kDown & KEY_UP || timeHeld[0] > 1) {
                    game_2048::doStep(1);
                    animTimer = 0;
                }
                if (kDown & KEY_LEFT || timeHeld[1] > 1) {
                    game_2048::doStep(0);
                    animTimer = 0;
                }
                if (kDown & KEY_DOWN || timeHeld[2] > 1) {
                    game_2048::doStep(3);
                    animTimer = 0;
                }
                if (kDown & KEY_RIGHT || timeHeld[3] > 1) {
                    game_2048::doStep(2);
                    animTimer = 0;
                }
                if (timeHeld[0] > 1) timeHeld[0] -= 0.33333333333333333333333f;
                if (timeHeld[1] > 1) timeHeld[1] -= 0.33333333333333333333333f;
                if (timeHeld[2] > 1) timeHeld[2] -= 0.33333333333333333333333f;
                if (timeHeld[3] > 1) timeHeld[3] -= 0.33333333333333333333333f;
                break;
            }
        }   
		
		// Start Render
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top_main, clrClear);
		C2D_TargetClear(bottom, clrClear);
		
		// Top Screen
		C2D_SceneBegin(top_main);
        
                    uint8_t tMoved[4][4];
                    uint8_t target[4][4][2];
        switch (cGameState) {
            case MAIN: {
                C2D_DrawRectSolid(SCREEN_WIDTH/2 - 101, SCREEN_HEIGHT/2 - 101, 0.1f, 202, 202, clrWhite);
                // Draw Board
                if (animTimer < max_time) {
                    // Animate
                    for (int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) {
                        tMoved[i][j] = game_2048::moved[i][j];
                        target[i][j][0] = i;
                        target[i][j][1] = j;
                    }
                    switch (game_2048::lastMovement) {
                        case 0: {
                            for (int i = 0; i < 4; i++) for(int j = 1; j < 4; j++) {
                                if (game_2048::oldBoard[j][i] == 0) continue;
                                int k = j;
                                while (tMoved[k][i] > 0 && k > 0) {
                                    tMoved[k][i]--;
                                    k--;
                                }
                                target[j][i][0]=k;
                            }
                            break;
                        }
                        case 1: {
                            for (int i = 0; i < 4; i++) for(int j = 1; j < 4; j++) {
                                if (game_2048::oldBoard[i][j] == 0) continue;
                                int k = j;
                                while (tMoved[i][k] > 0 && k > 0) {
                                    tMoved[i][k]--;
                                    k--;
                                }
                                target[i][j][1]=k;
                            }
                            break;
                        }
                        case 2: {
                            for (int i = 0; i < 4; i++) for(int j = 2; j >= 0; j--) {
                                if (game_2048::oldBoard[j][i] == 0) continue;
                                int k = j;
                                while (tMoved[k][i] > 0 && k < 3) {
                                    tMoved[k][i]--;
                                    k++;
                                }
                                target[j][i][0]=k;
                            }
                            break;
                        }
                        case 3: {
                            for (int i = 0; i < 4; i++) for(int j = 2; j >= 0; j--) {
                                if (game_2048::oldBoard[i][j] == 0) continue;
                                int k = j;
                                while (tMoved[i][k] > 0 && k < 3) {
                                    tMoved[i][k]--;
                                    k++;
                                }
                                target[i][j][1]=k;
                            }
                            break;
                        }
                    }
                    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
                        drawTileAtLocation(SCREEN_WIDTH / 2 - 99 + 50 * i, SCREEN_HEIGHT / 2 - 99 + 50 * j, 0);
                        if (game_2048::generated[i][j])
                            drawScaledTileAtLocation(SCREEN_WIDTH / 2 - 99 + 50 * i, 
                                                     SCREEN_HEIGHT / 2 - 99 + 50 * j,
                                                     game_2048::board[i][j], animTimer / max_time);
                        if (game_2048::oldBoard[i][j] == 0) continue;
                        drawTileAtLocation(SCREEN_WIDTH / 2 - 99 + (50/max_time)*((i * (max_time-animTimer)) + (target[i][j][0] * animTimer)), 
                                           SCREEN_HEIGHT / 2 - 99 + (50/max_time)*((j * (max_time-animTimer)) + (target[i][j][1] * animTimer)),
                                           game_2048::oldBoard[i][j]);
                    }
                } else {
                    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) drawTileAtLocation(SCREEN_WIDTH / 2 - 99 + 50 * i, 
                                                                                               SCREEN_HEIGHT / 2 - 99 + 50 * j,
                                                                                               game_2048::board[i][j]);
                }
                break;
            }
        }
		
		// Bottom Scene
        C2D_SceneBegin(bottom);
        
        // Done Rendering!
		C3D_FrameEnd(0);
	}
	
	// Free Fonts
	C2D_FontFree(font300);
	C2D_FontFree(font400);
	C2D_FontFree(font500);
	C2D_FontFree(font600);
	C2D_FontFree(font700);
	C2D_FontFree(font800);
	C2D_FontFree(font900);

    // Deinit Graphics
    C2D_Fini();
    C3D_Fini();
    gfxExit();
	
	// Exit ROM
    romfsExit();
	
	return 0;
}