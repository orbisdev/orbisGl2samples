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



int flagfolder=0;
int screenStatus;
int xflag;
int x=-62;
int y=132;



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
			y--;
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
			y++;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) || orbisPadGetButtonHold(ORBISPAD_RIGHT))
		{
			debugNetPrintf(DEBUG,"Right pressed\n");
			x++;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_LEFT) || orbisPadGetButtonHold(ORBISPAD_LEFT))
		{
			debugNetPrintf(DEBUG,"Left pressed\n");
			x--;
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
			debugNetPrintf(DEBUG,"Cross pressed %d %d\n",x,y);
			xflag=1;
			//orbisAudioStop();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
			//orbisAudioPause(0);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			//debugNetPrintf(DEBUG,"L1 pressed %d\n",orbisGlGetShapesTexture().id);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
			//debugNetPrintf(DEBUG,"L2 pressed %d\n",orbisGlGetFontDefault().texture.id);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R1))
		{
			debugNetPrintf(DEBUG,"R1 pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"R2 pressed\n");
			// debugNetPrintf(DEBUG,"orbisKernelLand return from syscall %d\n",syscall(11,orbisKernelLand));
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

Texture2D LoadTexture(const char *fileName);

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

	          // Useful for fading

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
	
    Texture2D texture = LoadTexture("raylib_logo.png");        // Texture loading
	
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

	while(flag)
	{

		updateController();
		 // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);

            DrawText("this is NOT a texture!", 905, 679, 10, GRAY);

        EndDrawing();
        //------------------------------------------------------------------------

	}
	CloseWindow();
	finishApp();
	sleep(3);
	exit(EXIT_SUCCESS);
}