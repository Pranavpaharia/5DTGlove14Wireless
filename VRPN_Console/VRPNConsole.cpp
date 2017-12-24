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

int ExeTime;
WORD ColorIndex = 10;
struct tm now;
INPUT keyboardInput;

vector<string> LeftHandUserInputList;
map<string, string> LeftHandKeyInputMap;

vector<string> RightHandUserInputList;
map<string, string> RightHandKeyInputMap;

vector<string> StandardKeyList;
map<string, string> StandardKeyMap;

struct data_t : deque <deque <float> >
{
	bool loadRightHandUserInputFile(const string& filename);
	bool loadLeftHandUserInputFile(const string& filename);
	bool loadStandardInputFile(const string& filename);
	bool createUserLeftHandInputMap(const vector<string>& fileList);
	bool createUserRightHandInputMap(const vector<string>& fileList);
	bool createStandardInputMap(const vector<string>& fileList);
	bool save(const string& filename);
};

bool data_t::createUserLeftHandInputMap(const vector<string>& fileList)
{
	bool bMapCreated = false;

	if (fileList.capacity() < 1)
	{
		bMapCreated = false;
	}
	else
	{
		bMapCreated = true;
	}

	for (int i = 0; i < fileList.size(); i++)
	{
		if (i % 2 == 0)
		{
			LeftHandKeyInputMap[fileList[i]] = fileList[i + 1];
		}
	}

	return bMapCreated;
}

bool data_t::createUserRightHandInputMap(const vector<string>& fileList)
{
	bool bMapCreated = false;

	if (fileList.capacity() < 1)
	{
		bMapCreated = false;
	}
	else
	{
		bMapCreated = true;
	}

	for (int i = 0; i < fileList.size(); i++)
	{
		if (i % 2 == 0)
		{
			RightHandKeyInputMap[fileList[i]] = fileList[i + 1];
		}
	}

	return bMapCreated;
}

bool data_t::createStandardInputMap(const vector<string>& fileList)
{
	bool bMapCreated = false;

	if (fileList.capacity() < 1)
	{
		bMapCreated = false;
	}
	else
	{
		bMapCreated = true;
	}

	for (int i = 0; i < (fileList.size() - 1); i++)
	{
		if (i % 2 == 0)
		{
			StandardKeyMap[fileList[i]] = fileList[i + 1];
		}
	}

	return bMapCreated;
}


bool data_t::loadStandardInputFile(const string& filename)
{
	bool status = false;
	string s;
	ifstream read;
	read.open(filename);
	if (read.fail())
	{
		cout << "Standard Input Configuration File Read Failed! \n";
	}
	else
	{
		cout << "Standard Input Configuration File Read Successful! \n";
	}

	while (getline(read, s))
	{
		istringstream iss(s);
		while (getline(iss, s, ','))
		{
			StandardKeyList.push_back(s);
			status = true;
		}
	}
	return status;
}

bool data_t::loadLeftHandUserInputFile(const string& filename)
{
	bool status = false;
	string s;
	ifstream read;
	read.open(filename);
	if (read.fail())
	{
		cout << "User Input Configuration File Read Failed! \n";
	}
	else
	{
		cout << "User Input Configuration File Read Successfully! \n";
	}

	while (getline(read, s))
	{
		deque <float> record;
		istringstream iss(s);
		while (getline(iss, s, ','))
		{
			LeftHandUserInputList.push_back(s);
			status = true;
		}

	}
	return status;
}

bool data_t::loadRightHandUserInputFile(const string& filename)
{
	bool status = false;
	string s;
	ifstream read;
	read.open(filename);
	if (read.fail())
	{
		cout << "User Input Configuration File Read Failed! \n";
	}
	else
	{
		cout << "User Input Configuration File Read Successfully! \n";
	}

	while (getline(read, s))
	{
		deque <float> record;
		istringstream iss(s);
		while (getline(iss, s, ','))
		{
			RightHandUserInputList.push_back(s);
			status = true;
		}

	}
	return status;
}



void PressKeyA(string GestureString)
{
	int x;
	stringstream sstream;
	sstream << std::hex << GestureString;
	sstream >> x;



	keyboardInput.type = INPUT_KEYBOARD;
	keyboardInput.ki.wScan = 0;
	keyboardInput.ki.time = 0;
	keyboardInput.ki.dwExtraInfo = 0;



	keyboardInput.ki.wVk = x; //0x41;
	keyboardInput.ki.dwFlags = 0;

	SendInput(1, &keyboardInput, sizeof(keyboardInput));

	keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &keyboardInput, sizeof(INPUT));
}

void PrintMapValues(const map<string, string> tMap)
{
	for (const auto& tMapPair : tMap)
	{
		cout << " Key:  " << tMapPair.first << " | Value:  " << tMapPair.second << endl;
	}
}

string GetLeftHandHexValue(const string GestureString)
{
	string hexValue = "";
	string KeyboardInputValue = "";
	KeyboardInputValue = LeftHandKeyInputMap[GestureString];

	hexValue = StandardKeyMap[KeyboardInputValue];

	return hexValue;
}

string GetRightHandHexValue(const string GestureString)
{
	string hexValue = "";
	string KeyboardInputValue = "";
	KeyboardInputValue = RightHandKeyInputMap[GestureString];

	hexValue = StandardKeyMap[KeyboardInputValue];

	return hexValue;
}


int main(int argc, char* argv[])
{
	data_t data;

	string filenameL = "C:\\Work\\VRPN_Console_Basic\\VRPN_Console\\GloveInputL.csv";
	string filenameR = "C:\\Work\\VRPN_Console_Basic\\VRPN_Console\\GloveInputR.csv";
	string filenameStandard = "C:\\Work\\VRPN_Console_Basic\\VRPN_Console\\StandardInputFile.csv";
	
	if (data.loadStandardInputFile(filenameStandard))
	{
		data.createStandardInputMap(StandardKeyList);
	}

	if (data.loadLeftHandUserInputFile(filenameL))
	{
		data.createUserLeftHandInputMap(LeftHandUserInputList);
	}

	if (data.loadRightHandUserInputFile(filenameR))
	{
		data.createUserRightHandInputMap(RightHandUserInputList);
	}
		
	
	//////////////////////////////////////////////////////////////////////////
	char* pPort = "COM1";
	HANDLE hConsole;
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	fdGlove* LeftGlove = fdOpen(pPort);
	fdGlove* RightGlove = fdOpen(pPort);

	if (LeftGlove == NULL || RightGlove == NULL)
	{
		printf("5DT Glove Detected Not Detected \n");
	}

	else
	{
		printf("5DT Glove Detected Left Hand. Checking Version...\n");
	}


	EfdGloveHand eLHand;
	int id = fdGetGloveHand(LeftGlove);

	eLHand = static_cast<EfdGloveHand>(id);

	switch (eLHand)
	{
	case FD_HAND_LEFT: printf("5DT Glove Left Hand Detected \n");
		break;

	case FD_HAND_RIGHT: printf("5DT Glove Right Hand Detected \n");
		break;

	default:  printf("Glove Type Detection Error. \n");
		break;
	}

	EfdGloveTypes eLType;
	int idL = fdGetGloveType(LeftGlove);
	eLType = static_cast<EfdGloveTypes>(idL);

	switch (eLType)
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

	EfdGloveHand eRHand;
	int idr = fdGetGloveHand(RightGlove);

	eRHand = static_cast<EfdGloveHand>(idr);

	switch (eRHand)
	{
	case FD_HAND_LEFT: printf("5DT Glove Left Hand Detected \n");
		break;

	case FD_HAND_RIGHT: printf("5DT Glove Right Hand Detected \n");
		break;

	default:  printf("Glove Type Detection Error. \n");
		break;
	}

	EfdGloveTypes eRType;
	int idR = fdGetGloveType(RightGlove);
	eRType = static_cast<EfdGloveTypes>(idR);

	switch (eRType)
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

	int CurrentGestureL;
	int CurrentGestureR;

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
			/*thumbIndexSensor = fdGetSensorRaw(LeftGlove, 2);
			printf("Thumb Index data values! : %d \n", thumbIndexSensor);

			thumbFarSensor = fdGetSensorRaw(LeftGlove, 1);
			printf("Thumb Far data values! : %d \n", thumbFarSensor);

			thumbNearSensor = fdGetSensorRaw(LeftGlove, 0);
			printf("Thumb Near data values! : %d \n", thumbNearSensor);
			
			NumberOfGestures = fdGetNumGestures(LeftGlove);
			printf("Number of Gestures! : %d \n", NumberOfGestures);*/

			CurrentGestureL = fdGetGesture(LeftGlove);
			//printf("Get Gesture ! : %d \n", CurrentGesture);
			
			CurrentGestureR = fdGetGestureA(RightGlove);
			//printf("Get Gesture A ! : %d \n", CurrentGestureR);

			switch (CurrentGestureL)
			{
					case 0: printf("RightHand Fist! \n");
						{
							string Gesture = "Fist";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 1: printf("RightHand Index Finger Point! \n");
						{
							string Gesture = "Index-Finger";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 2: printf("RightHand Middle Finger Point!  \n");
						{
							string Gesture = "Middle-Finger";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 3: printf("RightHand Two Finger Point!  \n");
						{
							string Gesture = "Two-Finger";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 4: printf("RightHand Ring Finger Point!  \n");
						{
							string Gesture = "Ring-Finger";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
					break;
					case 5: printf("RightHand Ring-Index Finger Point!  \n");
						{
							string Gesture = "Ring-Index-Finger";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 6: printf("RightHand Ring-Middle Finger Point!  \n");
						{
							string Gesture = "Ring-Middle-Finger";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 7: printf("RightHand Three Finger Point!  \n");
						{
							string Gesture = "Three-Finger-Point";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 8: printf("RightHand Little Finger Point!  \n");
						{
							string Gesture = "Little-Finger-Point";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 9: printf("RightHand Index and little Finger Point!  \n");
						{
							string Gesture = "Index-Little-Finger-Point";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 10: printf("RightHand Little-Middle Finger Point!  \n");
						{
							string Gesture = "Little-Middle-Finger-Point";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 11: printf("RightHand Not ring  Finger Point!  \n");
						{
							string Gesture = "Not-Ring-Finger-Point";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 12: printf("RightHand Little-Ring Finger Point!  \n");
						{
							string Gesture = "Little-Ring-Finger-Point";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 13: printf("RightHand Not middle Finger Point!  \n");
						{
							string Gesture = "Not-Middle-Finger-Point";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 14: printf("RightHand Not Index Finger Point!  \n");
						{
							string Gesture = "Not-Index-Finger-Point";
							string str = GetLeftHandHexValue(Gesture);
							PressKeyA(str);
						}
						break;
					case 15: printf("RightHand Flat Hand!  \n");
					{
						string Gesture = "Flat-Hand";
						string str = GetLeftHandHexValue(Gesture);
						PressKeyA(str);
					}
					break;
					default: printf("RightHand Unrecorded gesture %d \n", CurrentGestureL);
						//PressKeyA();

			}

			switch (CurrentGestureR)
			{
			case 0: printf("LeftHand Fist! \n");
			{
				string Gesture = "Fist Right";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 1: printf("LeftHand Index Finger Point! \n");
			{
				string Gesture = "Index-Finger";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 2: printf("LeftHand Middle Finger Point!  \n");
			{
				string Gesture = "Middle-Finger";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 3: printf("LeftHand Two Finger Point!  \n");
			{
				string Gesture = "Two-Finger";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 4: printf("LeftHand Ring Finger Point!  \n");
			{
				string Gesture = "Ring-Finger";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 5: printf("LeftHand Ring-Index Finger Point!  \n");
			{
				string Gesture = "Ring-Index-Finger";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 6: printf("LeftHand Ring-Middle Finger Point!  \n");
			{
				string Gesture = "Ring-Middle-Finger";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 7: printf("LeftHand Three Finger Point!  \n");
			{
				string Gesture = "Three-Finger-Point";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 8: printf("LeftHand Little Finger Point!  \n");
			{
				string Gesture = "Little-Finger-Point";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 9: printf("LeftHand Index and little Finger Point!  \n");
			{
				string Gesture = "Index-Little-Finger-Point";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 10: printf("LeftHand Little-Middle Finger Point!  \n");
			{
				string Gesture = "Little-Middle-Finger-Point";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 11: printf("LeftHand Not ring  Finger Point!  \n");
			{
				string Gesture = "Not-Ring-Finger-Point";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 12: printf("LeftHand Little-Ring Finger Point!  \n");
			{
				string Gesture = "Little-Ring-Finger-Point";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 13: printf("LeftHand Not middle Finger Point!  \n");
			{
				string Gesture = "Not-Middle-Finger-Point";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 14: printf("LeftHand Not Index Finger Point!  \n");
			{
				string Gesture = "Not-Index-Finger-Point";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}
			break;
			case 15: printf("LeftHand Flat \n");
			{
				string Gesture = "Flat-Hand";
				string str = GetRightHandHexValue(Gesture);
				PressKeyA(str);
			}

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

