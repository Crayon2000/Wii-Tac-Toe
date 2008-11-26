#include <ogc/conf.h>
#include "language.h"

// Languages
#include "../languages/english.h"
#include "../languages/french.h"

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

	return Convert(mxmlElementGetAttr(Text_Node, "to"));;
}

/**
 * Set the proper language
 * @param[in] Conf_Lang Language ID to set.
 */
void Language::SetLanguage(s32 Conf_Lang)
{
/*
	CONF_LANG_JAPANESE,
	CONF_LANG_ENGLISH,
	CONF_LANG_GERMAN,
	CONF_LANG_FRENCH,
	CONF_LANG_SPANISH,
	CONF_LANG_ITALIAN,
	CONF_LANG_DUTCH
*/
	mxml_node_t *Root_Node;
	switch(Conf_Lang)
	{
		case CONF_LANG_FRENCH:
			Root_Node = mxmlLoadString(NULL, (char*)french, MXML_TEXT_CALLBACK);
			break;
		default:
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
	return Convert(mxmlElementGetAttr(Text_Node, "text"));
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

/**
 * Convert UT8 XML text to proper format
 */
const char *Language::Convert(const char *inchar)
{
	char buffer[512] = "";
	int outlen, inlen, res;

	inlen = strlen(inchar);
	outlen = 512;

	res = UTF8Toascii((unsigned char *)buffer, &outlen, (const unsigned char *)inchar, &inlen);
	//res = UTF8Toisolat1((unsigned char *)buffer, &outlen, (const unsigned char *)inchar, &inlen);

	buffer[outlen] = '\0';

	//sprintf(buffer, "%X %X %X %X %X %X %X", buffer[0], buffer[1],buffer[2], buffer[3], buffer[4], buffer[5], buffer[6]);
	//sprintf(buffer, "%d", LooseCount);
	return buffer;
}

/**
 * UTF8Toascii:
 * @out:  a pointer to an array of bytes to store the result
 * @outlen:  the length of @out
 * @in:  a pointer to an array of UTF-8 chars
 * @inlen:  the length of @in
 *
 * Take a block of UTF-8 chars in and try to convert it to an ASCII
 * block of chars out.
 *
 * Returns 0 if success, -2 if the transcoding fails, or -1 otherwise
 * The value of @inlen after return is the number of octets consumed
 *     if the return value is positive, else unpredictable.
 * The value of @outlen after return is the number of octets consumed.
 */
int Language::UTF8Toascii(unsigned char* out, int *outlen, const unsigned char* in, int *inlen)
{
    const unsigned char* processed = in;
    const unsigned char* outend;
    const unsigned char* outstart = out;
    const unsigned char* instart = in;
    const unsigned char* inend;
    unsigned int c, d;
    int trailing;

    if ((out == NULL) || (outlen == NULL) || (inlen == NULL)) return(-1);
    if (in == NULL) {
			/*
		 * initialization nothing to do
		 */
		*outlen = 0;
		*inlen = 0;
		return(0);
    }
    inend = in + (*inlen);
    outend = out + (*outlen);
    while (in < inend) {
		d = *in++;
		if      (d < 0x80)  { c= d; trailing= 0; }
		else if (d < 0xC0) {
			/* trailing byte in leading position */
			*outlen = out - outstart;
			*inlen = processed - instart;
			return(-2);
		}
		else if (d < 0xE0)  { c= d & 0x1F; trailing= 1; }
		else if (d < 0xF0)  { c= d & 0x0F; trailing= 2; }
		else if (d < 0xF8)  { c= d & 0x07; trailing= 3; }
		else {
			/* no chance for this in Ascii */
			*outlen = out - outstart;
			*inlen = processed - instart;
			return(-2);
		}

		if (inend - in < trailing) {
			break;
		} 

		for ( ; trailing; trailing--) {
			if ((in >= inend) || (((d= *in++) & 0xC0) != 0x80))
				break;
			c <<= 6;
			c |= d & 0x3F;
		}

		/* assertion: c is a single UTF-4 value */
		if (c < 0x80) {
			if (out >= outend)
			break;
			*out++ = c;
		}

else if (c >= 0xE0 && c <= 0xE5) {
	if (out >= outend)
	break;
	*out++ = 'a';
}
else if (c == 0xE7) {
	if (out >= outend)
	break;
	*out++ = 'c';
}
else if (c >= 0xE8 && c <= 0xEB) {
	if (out >= outend)
	break;
	*out++ = 'e';
}
else if (c >= 0xEC && c <= 0xEF) {
	if (out >= outend)
	break;
	*out++ = 'i';
}
else if (c == 0xF1) {
	if (out >= outend)
	break;
	*out++ = 'n';
}
else if (c >= 0xF2 && c <= 0xF6) {
	if (out >= outend)
	break;
	*out++ = 'o';
}
else if (c >= 0xF9 && c <= 0xFC) {
	if (out >= outend)
	break;
	*out++ = 'u';
}

else if (c >= 0xC0 && c <= 0xC5) {
	if (out >= outend)
	break;
	*out++ = 'A';
}
else if (c == 0xC7) {
	if (out >= outend)
	break;
	*out++ = 'C';
}
else if (c >= 0xC8 && c <= 0xCB) {
	if (out >= outend)
	break;
	*out++ = 'E';
}
else if (c >= 0xCC && c <= 0xCF) {
	if (out >= outend)
	break;
	*out++ = 'I';
}
else if (c == 0xD1) {
	if (out >= outend)
	break;
	*out++ = 'N';
}
else if (c >= 0xD2 && c <= 0xD6) {
	if (out >= outend)
	break;
	*out++ = 'O';
}
else if (c >= 0xD9 && c <= 0xDC) {
	if (out >= outend)
	break;
	*out++ = 'U';
}

		else {
			/* no chance for this in Ascii */
			*outlen = out - outstart;
			*inlen = processed - instart;
			return(-2);
		}
		processed = in;
    }
    *outlen = out - outstart;
    *inlen = processed - instart;
    return(*outlen);
}

/**
 * UTF8Toisolat1:
 * @out:  a pointer to an array of bytes to store the result
 * @outlen:  the length of @out
 * @in:  a pointer to an array of UTF-8 chars
 * @inlen:  the length of @in
 *
 * Take a block of UTF-8 chars in and try to convert it to an ISO Latin 1
 * block of chars out.
 *
 * Returns 0 if success, -2 if the transcoding fails, or -1 otherwise
 * The value of @inlen after return is the number of octets consumed
 *     if the return value is positive, else unpredictable.
 * The value of @outlen after return is the number of octets consumed.
 */
int Language::UTF8Toisolat1(unsigned char* out, int *outlen,
              const unsigned char* in, int *inlen) {
    const unsigned char* processed = in;
    const unsigned char* outend;
    const unsigned char* outstart = out;
    const unsigned char* instart = in;
    const unsigned char* inend;
    unsigned int c, d;
    int trailing;

    if ((out == NULL) || (outlen == NULL) || (inlen == NULL)) return(-1);
    if (in == NULL) {
        /*
	 * initialization nothing to do
	 */
	*outlen = 0;
	*inlen = 0;
	return(0);
    }
    inend = in + (*inlen);
    outend = out + (*outlen);
    while (in < inend) {
	d = *in++;
	if      (d < 0x80)  { c= d; trailing= 0; }
	else if (d < 0xC0) {
	    /* trailing byte in leading position */
	    *outlen = out - outstart;
	    *inlen = processed - instart;
	    return(-2);
        } else if (d < 0xE0)  { c= d & 0x1F; trailing= 1; }
        else if (d < 0xF0)  { c= d & 0x0F; trailing= 2; }
        else if (d < 0xF8)  { c= d & 0x07; trailing= 3; }
	else {
	    /* no chance for this in IsoLat1 */
	    *outlen = out - outstart;
	    *inlen = processed - instart;
	    return(-2);
	}

	if (inend - in < trailing) {
	    break;
	} 

	for ( ; trailing; trailing--) {
	    if (in >= inend)
		break;
	    if (((d= *in++) & 0xC0) != 0x80) {
		*outlen = out - outstart;
		*inlen = processed - instart;
		return(-2);
	    }
	    c <<= 6;
	    c |= d & 0x3F;
	}

	/* assertion: c is a single UTF-4 value */
	if (c <= 0xFF) {
	    if (out >= outend)
		break;
	    *out++ = c;
	} else {
	    /* no chance for this in IsoLat1 */
	    *outlen = out - outstart;
	    *inlen = processed - instart;
	    return(-2);
	}
	processed = in;
    }
    *outlen = out - outstart;
    *inlen = processed - instart;
    return(*outlen);
}
