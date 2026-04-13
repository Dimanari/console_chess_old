#pragma once
#include <windows.h>
#include <vector>

using std::vector;
void gotoxy(int x, int y);
int isApprox(double a, double b, double margin);

struct _move
{
	char c[4];
};
enum class MyEnumClass
{
	empty, pawn, knight, bishop, rook, queen, king, _pawn, _knight, _bishop, _rook, _queen, _king, en_passant, _en_passant
};
class Chess
{
	bool legalMovesAvailable;

	static HANDLE hstdout;
	int stalematecounter;

	
	MyEnumClass board[8*8];
	MyEnumClass board_test[8 * 8];
	bool turn;
	bool blackCastleL, blackCastleR, whiteCastleL, whiteCastleR;
	vector<_move> moves;
	char moveStr[100];
	bool sub_move(char c1, char r1, char c2, char r2);

	float inspectPosition(MyEnumClass _board[]);
	bool isCheckTest(bool _turn);
	bool isMove(MyEnumClass board[], char c1, char r1);
	bool isEmpty(MyEnumClass p);
	int test_move(char c1, char r1, char c2, char r2);
	bool isLegalMoveCheck(char c1, char r1, char c2, char r2, bool _turn);
	bool isLegalMoveNext(char c1, char r1, char c2, char r2, MyEnumClass board[]);
	void backup();
	void restore();
	float inspectTestPosition();
	bool move2(char c1, char r1, char c2, char r2);
public:
	Chess();
	bool isCheck();
	void setInitialBoard();
	void setBoard(MyEnumClass _board[]);
	bool displayBoard();
	MyEnumClass* getBoard() const;
	MyEnumClass* getTestBoard() const;
	bool move(char c1, char r1, char c2, char r2);
	bool moveIndexed(unsigned indexMove);
	bool isLegalMove(char c1, char r1, char c2, char r2);
	bool isLegalMove(char c1, char r1, char c2, char r2, bool _turn, MyEnumClass board[]);
	bool canCapture(MyEnumClass me, MyEnumClass target);
	float inspectPosition();
	char enumToChar(MyEnumClass e);
	static void setHandle();

	

	int displayLegalMoves(bool display = false);

	char intToString(int Int, bool first);

	int getResult();

	MyEnumClass* exportMoveByindex(unsigned num);
	char* exportMoveStrByindex(unsigned num);
	static void cls();

	bool getTurn();
	void setTurn(bool _turn);
};

void getInput(char& c1, char& r1, char& c2, char& r2);
void announceIlegalMove();
#define NO_KINGS 10000
#define BLACK_MATE 2000
#define WHITE_MATE -2000

bool operator==(_move a, _move b);