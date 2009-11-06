#include <ogc/conf.h>
#include "language.h"

// Languages
#include "../languages/english.h"
#include "../languages/french.h"
#include "../languages/german.h"
#include "../languages/dutch.h"
#include "../languages/spanish.h"
#include "../languages/italian.h"

#define ICONV_BUFSIZE 1024

/**
 * Constructor for the Language class.
 */
Language::Language()
{
    SetLanguage(CONF_GetLanguage());

    TieCount = ChildCount(mxmlFindElement(First_Node, First_Node, "tie_game", NULL, NULL, MXML_DESCEND), "message");
    WinningCount = ChildCount(mxmlFindElement(First_Node, First_Node, "winning_game", NULL, NULL, MXML_DESCEND), "message");
    TurnOverCount = ChildCount(mxmlFindElement(First_Node, First_Node, "turn_over", NULL, NULL, MXML_DESCEND), "message");
}

/**
 * Destructor for the Language class.
 */
Language::~Language()
{
    if(First_Node)
        mxmlDelete(First_Node);
}

/**
 * Load a text from an XML file in memory.
 */
const char *Language::Text(const char *From)
{
    mxml_node_t *Text_Node;

    if(From == NULL)
    {
        return "";
    }

    Text_Node = mxmlFindElement(First_Node, First_Node, "translation", "from", From, MXML_DESCEND);
    if(Text_Node == NULL)
    {
        return "";
    }

    return mxmlElementGetAttr(Text_Node, "to");
}

/**
 * Load a text from an XML file in memory.
 */
wstring Language::String(const char *From)
{
    return Utf82Unicode(Text(From));
}

/**
 * Set the proper language.
 * @param[in] Conf_Lang Language ID to set.
 */
void Language::SetLanguage(s32 Conf_Lang)
{
    mxml_node_t *Root_Node;
    switch(Conf_Lang)
    {
        case CONF_LANG_FRENCH:
            Root_Node = mxmlLoadString(NULL, (char*)french, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_GERMAN:
            Root_Node = mxmlLoadString(NULL, (char*)german, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_DUTCH:
            Root_Node = mxmlLoadString(NULL, (char*)dutch, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_SPANISH:
            Root_Node = mxmlLoadString(NULL, (char*)spanish, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_ITALIAN:
            Root_Node = mxmlLoadString(NULL, (char*)italian, MXML_TEXT_CALLBACK);
            break;
        case CONF_LANG_JAPANESE:
        case CONF_LANG_KOREAN:
        case CONF_LANG_SIMP_CHINESE:
        case CONF_LANG_TRAD_CHINESE:
        default:    // CONF_LANG_ENGLISH
            Root_Node = mxmlLoadString(NULL, (char*)english, MXML_TEXT_CALLBACK);
    }
    First_Node = mxmlFindElement(Root_Node, Root_Node, "language", NULL, NULL, MXML_DESCEND);
}

/**
 * Count the number of child in a node.
 */
unsigned int Language::ChildCount(mxml_node_t *Up_Node, const char *Name)
{
    int Count = 0;

    if(Up_Node)
    {
        mxml_node_t *Message_Node = NULL;
        for(Message_Node = mxmlFindElement(Up_Node, Up_Node, Name, NULL, NULL, MXML_DESCEND);
            Message_Node != NULL;
            Message_Node = mxmlFindElement(Message_Node, Up_Node, Name, NULL, NULL, MXML_DESCEND))
        {
            Count++;
        }
    }
    return Count;
}

/**
 * Get a random message.
 */
const wchar_t *Language::GetRandomMessage(const char *Type, int Count)
{
    char RandNum[4] = "";

    sprintf(RandNum, "%d", rand() % Count + 1);
    mxml_node_t *Up_Node = mxmlFindElement(First_Node, First_Node, Type, NULL, NULL, MXML_DESCEND);
    mxml_node_t *Text_Node = mxmlFindElement(Up_Node, Up_Node, "message", "id", RandNum, MXML_DESCEND);
    return Utf82Unicode(mxmlElementGetAttr(Text_Node, "text")).c_str();
}

/**
 * Get a random winning message.
 */
const wchar_t *Language::GetRandomWinningMessage()
{
    return GetRandomMessage("winning_game", WinningCount);
}
/**
 * Get a random tie message.
 */
const wchar_t *Language::GetRandomTieMessage()
{
    return GetRandomMessage("tie_game", TieCount);
}
/**
 * Get a random turn over message.
 */
const wchar_t *Language::GetRandomTurnOverMessage()
{
    return GetRandomMessage("turn_over", TurnOverCount);
}

/**
 * Convert a utf-8 to Unicode.
 * @param[in] text utf-8 string.
 * @return A Unicode string.
 */
wstring Language::Utf82Unicode(const string &text) {
    const char *inpos = text.c_str();
    int incharsleft = text.size() + 1;

    wstring result;
    wchar_t outbuf[ICONV_BUFSIZE];
    outbuf[ICONV_BUFSIZE - 1] = 0;

    while (incharsleft)
    {
        wchar_t *outpos = outbuf;
        int outcharsleft = ICONV_BUFSIZE - 1;

        while (incharsleft && outcharsleft)
        {
            wchar_t wc = (unsigned char) *inpos++;

            if (wc <= 0x7F)
            {
                incharsleft -= 1;
            }
            else if (wc <= 0xDF)
            {
                wc = (wc & 0x3F) << 6;
                wc |= (*inpos++ & 0x3F);
                incharsleft -= 2;
            }
            else if (wc <= 0xEF)
            {
                wc = (wc & 0x3F) << 12;
                wc |= (*inpos++ & 0x3F) << 6;
                wc |= (*inpos++ & 0x3F);
                incharsleft -= 3;
            }
            else if (wc <= 0xF7)
            {
                wc = (wc & 0x07) << 18;
                wc |= (*inpos++ & 0x3F) << 12;
                wc |= (*inpos++ & 0x3F) << 6;
                wc |= (*inpos++ & 0x3F);
                incharsleft -= 4;
            }
            *outpos++ = wc;
            --outcharsleft;
        }
        result += outbuf;
    }

    return result;
}
