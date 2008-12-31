//---------------------------------------------------------------------------
#ifndef LanguageH
#define LanguageH
//---------------------------------------------------------------------------

#include "mxml/mxml.h"

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
	const char *GetRandomWinningMessage();
	const char *GetRandomTieMessage();
	const char *GetRandomTurnOverMessage();
	int TieCount;
	int WinningCount;
	int TurnOverCount;
private:
	mxml_node_t *First_Node;

	void SetLanguage(s32);
	unsigned int ChildCount(mxml_node_t *, const char *);
	const char *GetRandomMessage(const char *, int);
};
//---------------------------------------------------------------------------
#endif
