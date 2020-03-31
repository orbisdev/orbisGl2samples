/*******************************************************************************************
*
*   raylib example - procedural mesh generation
*
*   This example has been created using raylib 1.8 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2017 Ramon Santamaria (Ray San)
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

bool l1flag=false;
bool r1flag=false;

#define NUM_MODELS  8      // Parametric 3d shapes to generate



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
			debugNetPrintf(DEBUG,"Cross pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			debugNetPrintf(DEBUG,"L1 pressed %d\n");
			l1flag=1;
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
			debugNetPrintf(DEBUG,"L2 pressed %d\n");
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R1))
		{
			debugNetPrintf(DEBUG,"R1 pressed\n");
			r1flag=1;
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
		InitWindow(screenWidth, screenHeight,"raylib [models] example - mesh generation");
	}
	// We generate a checked image for texturing
    Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);

    Model models[NUM_MODELS] = { 0 };

    models[0] = LoadModelFromMesh(GenMeshPlane(2, 2, 5, 5));
    models[1] = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 2.0f));
    models[2] = LoadModelFromMesh(GenMeshSphere(2, 32, 32));
    models[3] = LoadModelFromMesh(GenMeshHemiSphere(2, 16, 16));
    models[4] = LoadModelFromMesh(GenMeshCylinder(1, 2, 16));
    models[5] = LoadModelFromMesh(GenMeshTorus(0.25f, 4.0f, 16, 32));
    models[6] = LoadModelFromMesh(GenMeshKnot(1.0f, 2.0f, 16, 128));
    models[7] = LoadModelFromMesh(GenMeshPoly(5, 2.0f));

    // Set checked texture as default diffuse component for all models material
    for (int i = 0; i < NUM_MODELS; i++) models[i].materials[0].maps[MAP_DIFFUSE].texture = texture;

    // Define the camera to look into our 3d world
    Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    // Model drawing position
    Vector3 position = { 0.0f, 0.0f, 0.0f };

    int currentModel = 0;

    SetCameraMode(camera, CAMERA_ORBITAL);  // Set a orbital camera mode


	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

	while(flag)
	{

		updateController();
		// Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);      // Update internal camera and our camera

        

        if (r1flag)
        {
            currentModel++;
            if (currentModel >= NUM_MODELS) currentModel = 0;
            r1flag=0;
        }
        else if (l1flag)
        {
            currentModel--;
            if (currentModel < 0) currentModel = NUM_MODELS - 1;
            l1flag=0;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawModel(models[currentModel], position, 1.0f, WHITE);

                DrawGrid(10, 1.0);

            EndMode3D();

            DrawRectangle(30, screenHeight-40, 310, 30, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(30, screenHeight-40, 310, 30, Fade(DARKBLUE, 0.5f));
            DrawText("L1/R1  BUTTON to CYCLE PROCEDURAL MODELS", 40, screenHeight-30, 10, BLUE);

            switch(currentModel)
            {
                case 0: DrawText("PLANE", screenWidth-150, 10, 20, DARKBLUE); break;
                case 1: DrawText("CUBE", screenWidth-150, 10, 20, DARKBLUE); break;
                case 2: DrawText("SPHERE", screenWidth-150, 10, 20, DARKBLUE); break;
                case 3: DrawText("HEMISPHERE", screenWidth-150, 10, 20, DARKBLUE); break;
                case 4: DrawText("CYLINDER", screenWidth-150, 10, 20, DARKBLUE); break;
                case 5: DrawText("TORUS", screenWidth-150, 10, 20, DARKBLUE); break;
                case 6: DrawText("KNOT", screenWidth-150, 10, 20, DARKBLUE); break;
                case 7: DrawText("POLY", screenWidth-150, 10, 20, DARKBLUE); break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
	}
	 // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture); // Unload texture

    // Unload models data (GPU VRAM)
    for (int i = 0; i < NUM_MODELS; i++) UnloadModel(models[i]);
	CloseWindow();
	finishApp();
	sleep(3);
	exit(EXIT_SUCCESS);
}