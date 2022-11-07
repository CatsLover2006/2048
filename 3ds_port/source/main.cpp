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
	
	// Load Fonts
	font300 = C2D_FontLoad("romfs:/gfx/Figtree-Light.bcfnt");
	font400 = C2D_FontLoad("romfs:/gfx/Figtree-Regular.bcfnt");
	font500 = C2D_FontLoad("romfs:/gfx/Figtree-Medium.bcfnt");
	font600 = C2D_FontLoad("romfs:/gfx/Figtree-SemiBold.bcfnt");
	font700 = C2D_FontLoad("romfs:/gfx/Figtree-Bold.bcfnt");
	font800 = C2D_FontLoad("romfs:/gfx/Figtree-ExtraBold.bcfnt");
	font900 = C2D_FontLoad("romfs:/gfx/Figtree-Black.bcfnt");
	
	// Font Buffers
	C2D_TextBuf hiWorld = C2D_TextBufNew(11);
	
	// Prepare Text
	C2D_Text hiText;
	C2D_TextFontParse(&hiText, font800, hiWorld, "Hello World!");
	//C2D_TextOptimize(&hiText);
	
    // Create colors
    u32 clrClear = C2D_Color32(230, 234, 235, 0xFF);
    u32 clrWhite = C2D_Color32(255, 255, 255, 0xFF);
    u32 clrWin = C2D_Color32(90, 161, 237, 0xFF);
	
	// Create Screens
    C3D_RenderTarget * top_main = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget * bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	GX_TRANSFER_SCALE(GX_TRANSFER_SCALE_XY);
	
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
        animTimer += 1 / 2.0;
		if (animTimer > 220) {
			clrWin = C2D_Color32(rand()%256, rand()%256, rand()%256, 0xFF);
			animTimer = -1;
		}
		
		// Input
		hidScanInput();
        hidTouchRead(&touch);
		kDown = hidKeysDown();
		kHeld = hidKeysHeld();
		kUp = hidKeysUp();
		
		// Exit
		if (kDown & KEY_START) break;
		
		// Start Render
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top_main, clrClear);
		C2D_TargetClear(bottom, clrClear);
		
		// Top Screen
		C2D_SceneBegin(top_main);
		
		// Draw two triangles
		C2D_DrawTriangle(0, 0, clrWin, 0, animTimer*6.0f, clrWhite, animTimer*8.0f, 0, clrWhite, 0.1f);
		C2D_DrawTriangle(0, 0, clrClear, 0, animTimer*2.5f, clrClear, animTimer*3.333333333333333333f, 0, clrClear, 0.2f);
		
		// Draw text
		for (int i = -2; i < 3; i++) for (int j = -2; j < 3; j++) C2D_DrawText(&hiText, C2D_AlignCenter | C2D_WithColor | C2D_AtBaseline, SCREEN_WIDTH/2 - i, SCREEN_HEIGHT/2 + 20 - j, 0.25f, 1.6f, 1.6f, clrWhite);
		C2D_DrawText(&hiText, C2D_AlignCenter | C2D_AtBaseline, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 20, 0.3f, 1.6f, 1.6f);
		
		// Bottom Scene
        C2D_SceneBegin(bottom);
		
		// Draw two triangles
		C2D_DrawTriangle(BOTTOM_SCREEN_WIDTH, SCREEN_HEIGHT, clrWin, BOTTOM_SCREEN_WIDTH, SCREEN_HEIGHT - animTimer*6.0f, clrWhite, BOTTOM_SCREEN_WIDTH - animTimer*8.0f, SCREEN_HEIGHT, clrWhite, 0.5f);
		C2D_DrawTriangle(BOTTOM_SCREEN_WIDTH, SCREEN_HEIGHT, clrClear, BOTTOM_SCREEN_WIDTH, SCREEN_HEIGHT - animTimer*2.5f, clrClear, BOTTOM_SCREEN_WIDTH - animTimer*3.333333333333333333f, SCREEN_HEIGHT, clrClear, 0.6f);
		
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
	
	return 0;
}