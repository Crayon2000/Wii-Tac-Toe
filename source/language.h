/**
 * @file language.h
 * Contains the Language class.
 */

#ifndef LanguageH
#define LanguageH
//---------------------------------------------------------------------------

#include <string>
#include <string_view>
#include <vector>
#include <gctypes.h>
#include <random>  // Include for std::mt19937

// Forward declarations
struct _mxml_node_s;

/**
 * This is a class to manage different languages.
 * @author Crayon
 */
class Language
{
public:
    Language();
    Language(Language const&) = delete;
    ~Language();
    Language& operator=(Language const&) = delete;
    std::string String(std::string_view From);
    std::string GetWinningMessage(s32 Index = -1);
    std::string GetTieMessage(s32 Index = -1);
    std::string GetTurnOverMessage(s32 Index = -1);
private:
    _mxml_node_s *First_Node;

    std::vector<std::string> WinningMessage;
    std::vector<std::string> TieMessage;
    std::vector<std::string> TurnOverMessage;

    std::mt19937 rng;  // Random number generator

    void SetLanguage(s32 Conf_Lang);
};
//---------------------------------------------------------------------------
#endif
