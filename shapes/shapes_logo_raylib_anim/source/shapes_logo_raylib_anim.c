/*******************************************************************************************
*
*   raylib [shapes] example - raylib logo animation
*
*   This example has been created using raylib 2.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*	Copyright (c) 2020 Antonio Jose Ramos Marquez (@psxdev) adapt sample to liborbis
********************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel.h>
#include <systemservice.h>
#include <orbis2d.h>
#include <orbisPad.h>
#include <orbisKeyboard.h>
#include <orbisAudio.h>
#include <ps4link.h>
#include <debugnet.h>
#include <orbisNfs.h>
#include <orbisGl2.h>
#include <unistd.h>
#include <stdarg.h>


OrbisPadConfig *confPad;
bool flag=true;
typedef struct OrbisGlobalConf
{
	Orbis2dConfig *conf;
	OrbisPadConfig *confPad;
	OrbisAudioConfig *confAudio;
	OrbisKeyboardConfig *confKeyboard;
	ps4LinkConfiguration *confLink;
	int orbisLinkFlag;
	debugNetConfiguration *confDebug;
	OrbisNfsConfig *confNfs;
}OrbisGlobalConf;

OrbisGlobalConf *myConf;

int xflag;


void updateController()
{
	int ret;
	unsigned int buttons=0;
	ret=orbisPadUpdate();
	if(ret==0)
	{
		if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R2) || orbisPadGetButtonHold(ORBISPAD_L2|ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"Combo L2R2 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L2|ORBISPAD_R2);
			orbisPadSetCurrentButtonsPressed(buttons);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R1) )
		{
			debugNetPrintf(DEBUG,"Combo L1R1 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L1|ORBISPAD_R1);
			orbisPadSetCurrentButtonsPressed(buttons);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R2) || orbisPadGetButtonHold(ORBISPAD_L1|ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"Combo L1R2 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L1|ORBISPAD_R2);
			orbisPadSetCurrentButtonsPressed(buttons);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R1) || orbisPadGetButtonHold(ORBISPAD_L2|ORBISPAD_R1) )
		{
			debugNetPrintf(DEBUG,"Combo L2R1 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L2|ORBISPAD_R1);
			orbisPadSetCurrentButtonsPressed(buttons);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_UP) || orbisPadGetButtonHold(ORBISPAD_UP))
		{
			debugNetPrintf(DEBUG,"Up pressed\n");			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) || orbisPadGetButtonHold(ORBISPAD_RIGHT))
		{
			debugNetPrintf(DEBUG,"Right pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_LEFT) || orbisPadGetButtonHold(ORBISPAD_LEFT))
		{
			debugNetPrintf(DEBUG,"Left pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
		{
			debugNetPrintf(DEBUG,"Triangle pressed exit\n");
			flag=0;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
		{
			debugNetPrintf(DEBUG,"Circle pressed\n");            
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
		{
			debugNetPrintf(DEBUG,"Cross pressed \n");
			xflag=1;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			debugNetPrintf(DEBUG,"L1 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
			debugNetPrintf(DEBUG,"L2 pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R1))
		{
			debugNetPrintf(DEBUG,"R1 pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"R2 pressed\n");
		}
	}
}

void finishApp()
{
	//orbisAudioFinish();
	//orbisKeyboardFinish();
	//orbisGlFinish();
	orbisPadFinish();
	orbisNfsFinish();
	//ps4LinkFinish();
}



bool initApp()
{
	int ret;
	//orbisNfsInit(NFSEXPORT);
	//orbisFileInit();
	sceSystemServiceHideSplashScreen();
	
	ret=orbisPadInitWithConf(myConf->confPad);
	if(ret)
	{
		confPad=orbisPadGetConf(); 
	}
	else
	{
		return false;
	}
	
	return true;
}


int main(int argc, char *argv[])
{
	uintptr_t intptr=0;
	sscanf(argv[1],"%p",&intptr);
	myConf=(OrbisGlobalConf *)intptr;
	
	debugNetInitWithConf(myConf->confDebug);
	orbisNfsInitWithConf(myConf->confNfs);
	debugNetPrintf(INFO,"[ORBISGL] raylib sample for Playstation 4 Retail fucking yeah!!!\n");

	sleep(1);
	const int screenWidth = ATTR_ORBISGL_WIDTH;
    const int screenHeight = ATTR_ORBISGL_HEIGHT;

	int logoPositionX = screenWidth/2 - 128;
	int logoPositionY = screenHeight/2 - 128;
	int framesCounter = 0;
	int lettersCount = 0;
	int topSideRecWidth = 16;
	int leftSideRecHeight = 16;
	int bottomSideRecWidth = 16;
	int rightSideRecHeight = 16;
	int state = 0;                  // Tracking animation states (State Machine)
	float alpha = 1.0f;             // Useful for fading

	//init liborbis libraries
	flag=initApp();
	if(flag)
	{
		//wrapper values are ignored
		InitWindow(screenWidth, screenHeight, "raylib [shapes] example - raylib logo animation");
	}
	//load raylib default font
	//LoadFontDefault();
	//SetShapesTexture(GetFontDefault().texture, GetFontDefault().recs[95]);
	
	const char msg1[21] = "powered by liborbis";

	
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

	while(flag)
	{

		updateController();
		     // Update
        //----------------------------------------------------------------------------------
		if (state == 0)                 // State 0: Small box blinking
		{
			framesCounter++;
			if (framesCounter == 120)
			{
				state = 1;
				framesCounter = 0;      // Reset counter... will be used later...
			}
		}
		else if (state == 1)            // State 1: Top and left bars growing
		{
			topSideRecWidth += 4;
			leftSideRecHeight += 4;
			if (topSideRecWidth == 256) state = 2;
		}
		else if (state == 2)            // State 2: Bottom and right bars growing
		{
			bottomSideRecWidth += 4;
			rightSideRecHeight += 4;
			if (bottomSideRecWidth == 256) state = 3;
		}
		else if (state == 3)            // State 3: Letters appearing (one by one)
		{
			framesCounter++;
			if (framesCounter/12)       // Every 12 frames, one more letter!
			{
				lettersCount++;
				framesCounter = 0;
			}
			if (lettersCount >= 10)     // When all letters have appeared, just fade out everything
			{
				alpha -= 0.02f;
				if (alpha <= 0.0f)
				{
					alpha = 0.0f;
					state = 4;
				}
			}
		}
		else if (state == 4)            // State 4: Reset and Replay
		{
			if (xflag)
			{
				framesCounter = 0;
				lettersCount = 0;
				topSideRecWidth = 16;
				leftSideRecHeight = 16;
				bottomSideRecWidth = 16;
				rightSideRecHeight = 16;
				alpha = 1.0f;
				state = 0;          // Return to State 0
				xflag=0;
			}
		}
		//----------------------------------------------------------------------------------
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
			ClearBackground(RAYWHITE);
			if (state == 0)
			{
				if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
			}
			else if (state == 1)
			{
				DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
				DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
			}
			else if (state == 2)
			{
				DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
				DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
				DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
				DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
			}
			else if (state == 3)
			{
				DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
				DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));
				DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
				DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));
				DrawRectangle(screenWidth/2 - 112, screenHeight/2 - 112, 224, 224, Fade(RAYWHITE, alpha));
				DrawText(TextSubtext("raylib", 0, lettersCount), screenWidth/2 - 44, screenHeight/2 + 48, 50, Fade(BLACK, alpha));
			}
			else if (state == 4)
			{	
				//draw msg1 text 
				DrawText(msg1, screenWidth/2-262, screenHeight/2-68, 50, BLACK);
			}
			
		EndDrawing();
		//----------------------------------------------------------------------------------
 

	}
	CloseWindow();
	finishApp();
	sleep(3);
	exit(EXIT_SUCCESS);
}