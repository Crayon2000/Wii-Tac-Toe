/**
 * @file language.h
 * Contains the Language class.
 */

#ifndef LanguageH
#define LanguageH
//---------------------------------------------------------------------------

#include <string>

// Forward declarations
struct mxml_node_s;

/**
 * This a class to manage different languages.
 * @author Crayon
 */
class Language
{
public:
    Language();
    ~Language();
    std::string String(const char *From);
    std::string GetWinningMessage(s8 Index = -1);
    std::string GetTieMessage(s8 Index = -1);
    std::string GetTurnOverMessage(s8 Index = -1);
private:
    mxml_node_s *First_Node;
    s8 TieCount;
    s8 WinningCount;
    s8 TurnOverCount;

    std::string *WinningMessage;
    std::string *TieMessage;
    std::string *TurnOverMessage;

    void SetLanguage(s32 Conf_Lang);
    unsigned int ChildCount(mxml_node_s *Up_Node, const char *Name);
};
//---------------------------------------------------------------------------
#endif
