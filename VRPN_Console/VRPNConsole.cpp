// VRPNConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fglove.h"
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <windows.h>

using namespace std;
unsigned tracker_stride = 1;
//float CurrentTime;
int ExeTime;
WORD ColorIndex = 10;
struct tm now;

int main(int argc, char* argv[])
{
	
	char* pPort = "COM1";
	HANDLE hConsole;
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	fdGlove* LeftGlove = fdOpen(pPort);

	if (LeftGlove == NULL)
	{
		printf("5DT Glove Detected Not Detected \n");
	}

	else
	{
		printf("5DT Glove Detected. Checking Version...\n");
	}


	EfdGloveHand eHand;
	int id = fdGetGloveHand(LeftGlove);

	eHand = static_cast<EfdGloveHand>(id);

	switch (eHand)
	{
	case FD_HAND_LEFT: printf("5DT Glove Left Hand Detected \n");
		break;

	case FD_HAND_RIGHT: printf("5DT Glove Right Hand Detected \n");
		break;

	default:  printf("Glove Type Detection Error. \n");
		break;
	}

	EfdGloveTypes eType;
	int pid = fdGetGloveType(LeftGlove);
	eType = static_cast<EfdGloveTypes>(pid);

	switch (eType)
	{
	case FD_GLOVENONE:
		break;
	case FD_GLOVE5U:
		break;
	case FD_GLOVE5UW:
		break;
	case FD_GLOVE5U_USB:
		break;
	case FD_GLOVE7:
		break;
	case FD_GLOVE7W:
		break;
	case FD_GLOVE16:
		break;
	case FD_GLOVE16W:
		break;
	case FD_GLOVE14U:
		break;
	case FD_GLOVE14UW: printf("Data Gloves 14 Wireless Selected! \n");
		break;
	case FD_GLOVE14U_USB:
		break;
	default:printf("Data Gloves Selection Error! Can't find Data Gloves Types \n");
		break;
	}

	int fGetSensorID = fdGetNumSensors(LeftGlove);

	unsigned short thumbIndexSensor;
	unsigned short thumbFarSensor;
	unsigned short thumbNearSensor;

	int NumberOfGestures;
	int CurrentGesture;
	int CurrentGestureA;

	time_t CurrentTime = time(0);
	localtime_s(&now, &CurrentTime);
	 ExeTime = now.tm_sec;
	getchar();

	while (1 && LeftGlove != nullptr)
	{
		CurrentTime = time(0);
		localtime_s(&now, &CurrentTime);
		if(ExeTime != now.tm_sec)
		{ 
			SetConsoleTextAttribute(hConsole, ColorIndex);
			thumbIndexSensor = fdGetSensorRaw(LeftGlove, 2);
			printf("Thumb Index data values! : %d \n", thumbIndexSensor);

			thumbFarSensor = fdGetSensorRaw(LeftGlove, 1);
			printf("Thumb Far data values! : %d \n", thumbFarSensor);

			thumbNearSensor = fdGetSensorRaw(LeftGlove, 0);
			printf("Thumb Near data values! : %d \n", thumbNearSensor);
			
			NumberOfGestures = fdGetNumGestures(LeftGlove);
			printf("Number of Gestures! : %d \n", NumberOfGestures);

			CurrentGesture = fdGetGesture(LeftGlove);
			printf("Get Gesture ! : %d \n", CurrentGesture);
			
			CurrentGestureA = fdGetGestureA(LeftGlove);
			printf("Get Gesture A ! : %d \n", CurrentGestureA);

			ExeTime = now.tm_sec;
			ColorIndex++;

			if (ColorIndex == 15)
			{
				ColorIndex = 10;
			}
		}
	}



    return 0;
}

