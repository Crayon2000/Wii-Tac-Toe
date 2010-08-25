/**
 * @file language.h
 * Contains the Language class.
 */

#ifndef LanguageH
#define LanguageH
//---------------------------------------------------------------------------

#include "mxml/mxml.h"
#include <string>

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
    std::wstring String(const char *);
    std::wstring GetWinningMessage(s8 Index = -1);
    std::wstring GetTieMessage(s8 Index = -1);
    std::wstring GetTurnOverMessage(s8 Index = -1);
private:
    mxml_node_t *First_Node;
    s8 TieCount;
    s8 WinningCount;
    s8 TurnOverCount;

    std::wstring *WinningMessage;
    std::wstring *TieMessage;
    std::wstring *TurnOverMessage;

    void SetLanguage(s32);
    unsigned int ChildCount(mxml_node_t *, const char *);
    std::wstring Utf82Unicode(const std::string &);
};
//---------------------------------------------------------------------------
#endif
