//---------------------------------------------------------------------------
#ifndef PlayerH
#define PlayerH
//---------------------------------------------------------------------------

#define PLAYER_HUMAN 	0
#define PLAYER_CPU 		1

/**
 * This class is used managed players.
 * @author Crayon
 */
class Player
{
public:
	Player();
	//~Player();
	void SetName(const char *);
	char *GetName();
	void SetSign(unsigned char);
	unsigned char GetSign();
	unsigned int GetScore();
	void IncScore();
	void ResetScore();
	void SetType(unsigned char);
	unsigned char GetType();
private:
	unsigned int Score;
	char Name[50];
	unsigned char Sign;
	unsigned char Type;
};
//---------------------------------------------------------------------------
#endif
