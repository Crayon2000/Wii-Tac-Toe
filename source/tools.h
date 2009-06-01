//---------------------------------------------------------------------------
#ifndef ToolsH
#define ToolsH
//---------------------------------------------------------------------------

#include <string>
using namespace std;

void RUMBLE_Wiimote(s32, int);
void RUMBLE_Verify();
void RUMBLE_Init();

void WIILIGHT_TurnOn();
void WIILIGHT_TurnOff();

string str_replace(const string &, const string &, const string &);
//---------------------------------------------------------------------------
#endif
