/**
 * @file language.h
 * Contains the Language class.
 */

#ifndef LanguageH
#define LanguageH
//---------------------------------------------------------------------------

#include "mxml/mxml.h"
#include <string>

using namespace std;

/**
 * This a class to manage different languages.
 * @author Crayon
 */
class Language
{
public:
    Language();
    ~Language();
    const char *Text(const char *);
    wstring String(const char *);
    wstring GetRandomWinningMessage();
    wstring GetRandomTieMessage();
    wstring GetRandomTurnOverMessage();
private:
    mxml_node_t *First_Node;
    int TieCount;
    int WinningCount;
    int TurnOverCount;

    wstring *WinningMessage;
    wstring *TieMessage;
    wstring *TurnOverMessage;

    void SetLanguage(s32);
    unsigned int ChildCount(mxml_node_t *, const char *);
    wstring Utf82Unicode(const string &);
};
//---------------------------------------------------------------------------
#endif
