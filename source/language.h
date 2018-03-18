/**
 * @file language.h
 * Contains the Language class.
 */

#ifndef LanguageH
#define LanguageH
//---------------------------------------------------------------------------

#include <string>
#include <vector>
#include <gctypes.h>

// Forward declarations
struct mxml_node_s;

/**
 * This is a class to manage different languages.
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

    std::vector<std::string> WinningMessage;
    std::vector<std::string> TieMessage;
    std::vector<std::string> TurnOverMessage;

    void SetLanguage(s32 Conf_Lang);
};
//---------------------------------------------------------------------------
#endif
