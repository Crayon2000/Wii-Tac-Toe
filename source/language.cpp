#include <ogc/conf.h>
#include "language.h"

// Languages
#include "../languages/english.h"
#include "../languages/french.h"
#include "../languages/german.h"
#include "../languages/dutch.h"
#include "../languages/spanish.h"

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
 * Load a text from an XML file in memory
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
 * Set the proper language
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
		case CONF_LANG_JAPANESE:
		default:	// LANG_ENGLISH
			Root_Node = mxmlLoadString(NULL, (char*)english, MXML_TEXT_CALLBACK);
	}
	First_Node = mxmlFindElement(Root_Node, Root_Node, "language", NULL, NULL, MXML_DESCEND);
}

/**
 * Count the number of child in a node
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
 * Get a random message
 */
const char *Language::GetRandomMessage(const char *Type, int Count)
{
	char RandNum[4] = "";

	sprintf(RandNum, "%d", rand() % Count + 1);
	mxml_node_t *Up_Node = mxmlFindElement(First_Node, First_Node, Type, NULL, NULL, MXML_DESCEND);
	mxml_node_t *Text_Node = mxmlFindElement(Up_Node, Up_Node, "message", "id", RandNum, MXML_DESCEND);
	return mxmlElementGetAttr(Text_Node, "text");
}

const char *Language::GetRandomWinningMessage()
{
	return GetRandomMessage("winning_game", WinningCount);
}
const char *Language::GetRandomTieMessage()
{
	return GetRandomMessage("tie_game", TieCount);
}
const char *Language::GetRandomTurnOverMessage()
{
	return GetRandomMessage("turn_over", TurnOverCount);
}
