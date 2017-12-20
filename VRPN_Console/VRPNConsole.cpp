// VRPNConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fglove.h"
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <windows.h>
#include <deque>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;
unsigned tracker_stride = 1;
//float CurrentTime;
int ExeTime;
WORD ColorIndex = 10;
struct tm now;
INPUT keyboardInput;

vector<string> DocWords;
map<string, string> KeyInputMap;

vector<string> StandardKeyList;
map<string, string> StandardKeyMap;

struct data_t : deque <deque <float> >
{
	typedef deque <deque <float> > ::iterator record_iterator;
	typedef deque        <float>   ::iterator field_iterator;
	bool load(const string& filename);
	bool loadStandardMapFile(const string& filename);
	bool save(const string& filename);
	bool save(ostream& outs);
};

bool loadStandardMapFile(const string& filename)
{
	string s;
	ifstream read;

	read.open(filename);
	if (read.fail())
	{
		cout << "File Read Failed";
	}

	else
	{
		cout << "StandardMap File Read Successful";
	}

	while (getline(read, s))
	{
		deque <string> record;
		istringstream iss(s);
		int index = 0;
		while (getline(iss, s, ','))
		{
			float fieldvalue = 0.0f;
			istringstream(s) >> fieldvalue;
			record.push_back(s);
			StandardKeyList.push_back(s);
			index++;
		}
		//this->push_back(record);
	}
	return true;
}
bool data_t::load(const string& filename)
{
	string s;
	ifstream read;
	//	read.open("C:\\Work\\VRPN_Console_Basic\\VRPN_Console\\GloveInput.txt");
	read.open(filename);
	if (read.fail())
	{
		cout << "File Read Failed";
	}

	else
	{
		cout << "File Read Successful";
	}

	while (getline(read, s))
	{
		deque <string> record;
		istringstream iss(s);
		int index =0;
		while (getline(iss, s, ','))
		{
			float fieldvalue = 0.0f;
			istringstream(s) >> fieldvalue;
			record.push_back(s);
			DocWords.push_back(s);
			index++;
		}
		//this->push_back(record);
	}
	return true;
}

void SetDictionary()
{
	for (int i = 0;i < (DocWords.size()-1);i++)
	{
		if(i % 2 == 0)
			KeyInputMap[DocWords[i]] = DocWords[i + 1];
	}
}


void PressKeyA()
{
	keyboardInput.type = INPUT_KEYBOARD;
	keyboardInput.ki.wScan = 0;
	keyboardInput.ki.time = 0;
	keyboardInput.ki.dwExtraInfo = 0;

	keyboardInput.ki.wVk = 0x41;
	keyboardInput.ki.dwFlags = 0;

	SendInput(1, &keyboardInput, sizeof(keyboardInput));

	keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &keyboardInput, sizeof(INPUT));
}
int main(int argc, char* argv[])
{
	//Testing read CSV File 
	// <string> DocWords;
	data_t data;

	string filename = "C:\\Work\\VRPN_Console_Basic\\VRPN_Console\\GloveInput.csv";
	filename = "D:\\Development\\Projects for C++\\VRPN_Console_Basic\\VRPN_Console\\KeyConfig.csv";
	cout << "File to read: \n";
	//getline(cin, filename);

	if (data.load(filename))
	{
		SetDictionary();
		for (auto address_entry : KeyInputMap)
		{
			cout << endl<< address_entry.first << " || " << address_entry.second << " " << endl;
		}
	}
		

	//cout << "The data is: %s \n", KeyInputMap[0];

	

	getchar();
	return 0;


	//////////////////////////////////////////////////////////////////////////
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


			switch (CurrentGesture)
			{
			case 0: printf("Fist! \n");
				PressKeyA();
				break;
			case 1: printf("Index Finger Point! \n");
				PressKeyA();
				break;
			case 2: printf("Index Finger Point!  \n");
				PressKeyA();
				break;
			case 3: printf("Middle Finger Point!  \n");
				PressKeyA();
				break;
			case 4: printf("Ring Finger Point!  \n");
				PressKeyA();
				break;
			case 5: printf("Ring-Index Finger Point!  \n");
				break;
			case 6: printf("Ring-Middle Finger Point!  \n");
				break;
			case 7: printf("Three Finger Point!  \n");
				break;
			case 8: printf("Little Finger Point!  \n");
				break;
			case 9: printf("Index and little Finger Point!  \n");
				break;
			case 10: printf("Little-Middle Finger Point!  \n");
				break;
			case 11: printf("Not ring  Finger Point!  \n");
				break;
			case 12: printf("Little-Ring Finger Point!  \n");
				break;
			case 13: printf("Not middle Finger Point!  \n");
				break;
			case 14: printf("Not Index Finger Point!  \n");
				break;
			case 15: printf("Flat Hand!  \n");
				break;
			default: printf("Unrecorded gesture %d \n", CurrentGesture);
				PressKeyA();

			}

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

