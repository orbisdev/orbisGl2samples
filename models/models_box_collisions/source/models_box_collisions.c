/*******************************************************************************************
*
*   raylib [models] example - Detect basic 3d collisions (box vs sphere vs box)
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
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


Vector3 playerPosition = { 0.0f, 1.0f, 2.0f };


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
			playerPosition.z-=0.2f;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
			playerPosition.z+=0.2f;	
		}
		if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) || orbisPadGetButtonHold(ORBISPAD_RIGHT))
		{
			debugNetPrintf(DEBUG,"Right pressed\n");
			playerPosition.x+=0.2f;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_LEFT) || orbisPadGetButtonHold(ORBISPAD_LEFT))
		{
			debugNetPrintf(DEBUG,"Left pressed\n");
			playerPosition.x-=0.2f;
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
			debugNetPrintf(DEBUG,"Cross pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			debugNetPrintf(DEBUG,"L1 pressed %d\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
			debugNetPrintf(DEBUG,"L2 pressed %d\n");
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

	const int screenWidth = ATTR_ORBISGL_WIDTH;
    const int screenHeight = ATTR_ORBISGL_HEIGHT;


	//init liborbis libraries
	flag=initApp();
	if(flag)
	{
		//wrapper values are ignored
		InitWindow(screenWidth, screenHeight,"raylib [models] example - box collisions");
	}
	// Define the camera to look into our 3d world
	Camera3D camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
	Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
	Color playerColor = GREEN;
	Vector3 enemyBoxPos = { -4.0f, 1.0f, 0.0f };
	Vector3 enemyBoxSize = { 2.0f, 2.0f, 2.0f };
	Vector3 enemySpherePos = { 4.0f, 0.0f, 0.0f };
	float enemySphereSize = 1.5f;
	bool collision = false;


	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

	while(flag)
	{

		updateController();
		collision = false;

        // Check collisions player vs enemy-box
        if (CheckCollisionBoxes(
            (BoundingBox){(Vector3){ playerPosition.x - playerSize.x/2,
                                     playerPosition.y - playerSize.y/2,
                                     playerPosition.z - playerSize.z/2 },
                          (Vector3){ playerPosition.x + playerSize.x/2,
                                     playerPosition.y + playerSize.y/2,
                                     playerPosition.z + playerSize.z/2 }},
            (BoundingBox){(Vector3){ enemyBoxPos.x - enemyBoxSize.x/2,
                                     enemyBoxPos.y - enemyBoxSize.y/2,
                                     enemyBoxPos.z - enemyBoxSize.z/2 },
                          (Vector3){ enemyBoxPos.x + enemyBoxSize.x/2,
                                     enemyBoxPos.y + enemyBoxSize.y/2,
                                     enemyBoxPos.z + enemyBoxSize.z/2 }})) collision = true;

        // Check collisions player vs enemy-sphere
        if (CheckCollisionBoxSphere(
            (BoundingBox){(Vector3){ playerPosition.x - playerSize.x/2,
                                     playerPosition.y - playerSize.y/2,
                                     playerPosition.z - playerSize.z/2 },
                          (Vector3){ playerPosition.x + playerSize.x/2,
                                     playerPosition.y + playerSize.y/2,
                                     playerPosition.z + playerSize.z/2 }},
            enemySpherePos, enemySphereSize)) collision = true;

        if (collision) playerColor = RED;
        else playerColor = GREEN;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw enemy-box
                DrawCube(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
                DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);

                // Draw enemy-sphere
                DrawSphere(enemySpherePos, enemySphereSize, GRAY);
                DrawSphereWires(enemySpherePos, enemySphereSize, 16, 16, DARKGRAY);

                // Draw player
                DrawCubeV(playerPosition, playerSize, playerColor);

                DrawGrid(10, 1.0f);        // Draw a grid

            EndMode3D();

            DrawText("Move player with cursors to collide", 220, 40, 20, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
	}
	CloseWindow();
	finishApp();
	sleep(3);
	exit(EXIT_SUCCESS);
}