/*******************************************************************************************
*
*   raylib [shapes] example - rectangle scaling by mouse(pad on PlayStation 4)
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example contributed by Vlad Adrian (@demizdor) and reviewed by Ramon Santamaria (@raysan5)
*
*   Copyright (c) 2018 Vlad Adrian (@demizdor) and Ramon Santamaria (@raysan5)
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
bool xhold=false;
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

#define MOUSE_SCALE_MARK_SIZE   12



int x;
int y;



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
			y=y-10;
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
			y=y+10;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) || orbisPadGetButtonHold(ORBISPAD_RIGHT))
		{
			debugNetPrintf(DEBUG,"Right pressed\n");
			x=x+10;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_LEFT) || orbisPadGetButtonHold(ORBISPAD_LEFT))
		{
			debugNetPrintf(DEBUG,"Left pressed\n");
			x=x-10;
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
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS) || orbisPadGetButtonHold(ORBISPAD_CROSS))
		{
			xhold=true;
		}
		if(orbisPadGetButtonReleased(ORBISPAD_CROSS))
		{
			xhold=false;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
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
    x=screenWidth/2;
    y=screenHeight/2;
	
	


	//init liborbis libraries
	flag=initApp();
	if(flag)
	{
		//wrapper values are ignored
		InitWindow(screenWidth, screenHeight, "raylib [shapes] example - rectangle scaling mouse");
	}
	
	Rectangle rec = { 100, 100, 200, 80 };

    Vector2 mousePosition = { 0 };

    bool mouseScaleReady = false;
    bool mouseScaleMode = false;


	
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

	while(flag)
	{

		updateController();

		
		// Update
        //----------------------------------------------------------------------------------
        mousePosition.x=x;
        mousePosition.y=y;

        if (CheckCollisionPointRec(mousePosition, rec) &&
            CheckCollisionPointRec(mousePosition, (Rectangle){ rec.x + rec.width - MOUSE_SCALE_MARK_SIZE, rec.y + rec.height - MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE }))
        {
            mouseScaleReady = true;
            if (xhold) mouseScaleMode = true;
        }
        else mouseScaleReady = false;

        if (mouseScaleMode)
        {
            mouseScaleReady = true;

            rec.width = (mousePosition.x - rec.x);
            rec.height = (mousePosition.y - rec.y);

            if (rec.width < MOUSE_SCALE_MARK_SIZE) rec.width = MOUSE_SCALE_MARK_SIZE;
            if (rec.height < MOUSE_SCALE_MARK_SIZE) rec.height = MOUSE_SCALE_MARK_SIZE;

            if (!xhold) mouseScaleMode = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Scale rectangle dragging from bottom-right corner!", 10, 10, 20, GRAY);

            DrawRectangleRec(rec, Fade(GREEN, 0.5f));

            DrawRectangle(x,y,3,3,MAROON);

            if (mouseScaleReady)
            {
                DrawRectangleLinesEx(rec, 1, RED);
                DrawTriangle((Vector2){ rec.x + rec.width - MOUSE_SCALE_MARK_SIZE, rec.y + rec.height },
                             (Vector2){ rec.x + rec.width, rec.y + rec.height },
                             (Vector2){ rec.x + rec.width, rec.y + rec.height - MOUSE_SCALE_MARK_SIZE }, RED);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
	}
	CloseWindow();
	finishApp();
	sleep(3);
	exit(EXIT_SUCCESS);
}