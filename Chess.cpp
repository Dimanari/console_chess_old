#include "Chess.h"
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <ctime>

#define stalemateAfter 50

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

void gotoxy(int x, int y)
{
	COORD p = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

int isApprox(double a, double b, double margin)
{
	if (a < b + margin)
		return 1;
	else
		if (a > b - margin)
			return -1;
	return 0;
}

bool inrange(char value, char min, char max)
{
	if (value >= min && value <= max)
		return true;
	return false;
}

#define _cls() system("CLS")
HANDLE Chess::hstdout;
bool Chess::sub_move(char c1, char r1, char c2, char r2)
{

	bool isPawn = (board[c1 * 8 + r1] == MyEnumClass::pawn) || (board[c1 * 8 + r1] == MyEnumClass::_pawn);
	bool isKing = (board[c1 * 8 + r1] == MyEnumClass::king) || (board[c1 * 8 + r1] == MyEnumClass::_king);

	if (board[c1 * 8 + r1] == MyEnumClass::empty)
		return false;
	backup();
	if (board[c2 * 8 + r2] == MyEnumClass::en_passant && board[c1 * 8 + r1] == MyEnumClass::_pawn)
		board[(c2 + 1) * 8 + r2] = MyEnumClass::empty;
	if (board[c2 * 8 + r2] == MyEnumClass::_en_passant && board[c1 * 8 + r1] == MyEnumClass::pawn)
		board[(c2 - 1) * 8 + r2] = MyEnumClass::empty;

	//clear En-Passant
	for (int i = 0; i < 8; i++)
	{
		if (board[2 * 8 + i] == MyEnumClass::en_passant)
			board[2 * 8 + i] = MyEnumClass::empty;
		if (board[5 * 8 + i] == MyEnumClass::_en_passant)
			board[5 * 8 + i] = MyEnumClass::empty;
	}
	//add En-Passant
	if ((board[c1 * 8 + r1] == MyEnumClass::pawn) && (abs(c2 - c1) == 2) && (r1 == r2))
	{
		if (board[(c1 + c2) * 4 + r1] == MyEnumClass::empty)
		{
			if (((c1 + c2) / 2) == 2)
				board[2 * 8 + r1] = MyEnumClass::en_passant;
		}
	}
	if ((board[c1 * 8 + r1] == MyEnumClass::_pawn) && (abs(c2 - c1) == 2) && (r1 == r2))
	{
		if (board[(c1 + c2) * 4 + r1] == MyEnumClass::empty)
		{
			if (((c1 + c2) / 2) == 5)
				board[5 * 8 + r1] = MyEnumClass::_en_passant;
		}
	}

	board[c2 * 8 + r2] = board[c1 * 8 + r1];
	board[c1 * 8 + r1] = MyEnumClass::empty;

	if (isCheck())
	{
		restore();
		return false;
	}
	if (isPawn && (c2 == 7 || c2 == 0))
	{
		if (board[c2 * 8 + r2] == MyEnumClass::pawn)
			board[c2 * 8 + r2] = MyEnumClass::queen;

		if (board[c2 * 8 + r2] == MyEnumClass::_pawn)
			board[c2 * 8 + r2] = MyEnumClass::_queen;
	}

	if (isKing && (r2 == 2) && (abs(r2 - r1) == 2))
	{
		if (board[c2 * 8 + r2] == MyEnumClass::king)
			board[c1 * 8 + 3] = MyEnumClass::rook;
		if (board[c2 * 8 + r2] == MyEnumClass::_king)
			board[c1 * 8 + 3] = MyEnumClass::_rook;

		board[c1 * 8 + 0] = MyEnumClass::empty;
	}

	if (isKing && (r2 == 6) && (abs(r2 - r1) == 2))
	{
		if (board[c2 * 8 + r2] == MyEnumClass::king)
			board[c1 * 8 + 5] = MyEnumClass::rook;
		if (board[c2 * 8 + r2] == MyEnumClass::_king)
			board[c1 * 8 + 5] = MyEnumClass::_rook;

		board[c1 * 8 + 7] = MyEnumClass::empty;
	}
	return true;
}
float Chess::inspectPosition(MyEnumClass _board[])
{
	if (stalematecounter > stalemateAfter)
		return NO_KINGS;
	bool hasBlackKing = false;
	bool hasWhiteKing = false;
	float WhiteMaterial = 0;
	float BlackMaterial = 0;
	int n_black = 0;
	int n_white = 0;
	for (int i = 0; i < 8 * 8; i++)
	{
		int c = i / 8;
		switch (_board[i])
		{
		case MyEnumClass::pawn:
			WhiteMaterial += 2 + c * 0.01;
			n_white++;
			break;
		case MyEnumClass::knight:
			WhiteMaterial += 5;
			n_white++;
			break;
		case MyEnumClass::bishop:
			WhiteMaterial += 6;
			n_white++;
			break;
		case MyEnumClass::rook:
			WhiteMaterial += 10;
			n_white++;
			break;
		case MyEnumClass::queen:
			WhiteMaterial += 18;
			n_white++;
			break;
		case MyEnumClass::king:
			WhiteMaterial += 1000;
			n_white++;
			hasWhiteKing = true;
			break;

		case MyEnumClass::_pawn:
			BlackMaterial += 2 + (7 - c) * 0.01;
			n_black++;
			break;
		case MyEnumClass::_knight:
			BlackMaterial += 5;
			n_black++;
			break;
		case MyEnumClass::_bishop:
			BlackMaterial += 6;
			n_black++;
			break;
		case MyEnumClass::_rook:
			BlackMaterial += 10;
			n_black++;
			break;
		case MyEnumClass::_queen:
			BlackMaterial += 18;
			n_black++;
			break;
		case MyEnumClass::_king:
			BlackMaterial += 1000;
			n_black++;
			hasBlackKing = true;
			break;
		}
	}
	if ((n_black > 16) || (n_white > 16))
		throw ERROR;
	if (!hasBlackKing && !hasWhiteKing)
		return NO_KINGS;
	if (!hasBlackKing)
		return BLACK_MATE;
	if (!hasWhiteKing)
		return WHITE_MATE;
	return (WhiteMaterial - BlackMaterial);
}

Chess::Chess()
{
	setInitialBoard();
	moves.clear();
	legalMovesAvailable = false;
}
void Chess::setInitialBoard()
{
	blackCastleL = blackCastleR = whiteCastleL = whiteCastleR = true;
	turn = false;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i * 8 + j] = MyEnumClass::empty;
			if (i == 1)
			{
				board[i * 8 + j] = MyEnumClass::pawn;
			}
			if (i == 6)
			{
				board[i * 8 + j] = MyEnumClass::_pawn;
			}
			if (i == 0)
			{
				switch (j)
				{
				case 0:
				case 7:
					board[i * 8 + j] = MyEnumClass::rook;
					break;

				case 1:
				case 6:
					board[i * 8 + j] = MyEnumClass::knight;
					break;

				case 2:
				case 5:
					board[i * 8 + j] = MyEnumClass::bishop;
					break;

				case 4:
					board[i * 8 + j] = MyEnumClass::king;
					break;
				case 3:
					board[i * 8 + j] = MyEnumClass::queen;
					break;
				}
			}
			if (i == 7)
			{
				switch (j)
				{
				case 0:
				case 7:
					board[i * 8 + j] = MyEnumClass::_rook;
					break;

				case 1:
				case 6:
					board[i * 8 + j] = MyEnumClass::_knight;
					break;

				case 2:
				case 5:
					board[i * 8 + j] = MyEnumClass::_bishop;
					break;

				case 4:
					board[i * 8 + j] = MyEnumClass::_king;
					break;
				case 3:
					board[i * 8 + j] = MyEnumClass::_queen;
					break;
				}
			}
		}
	}
}

void Chess::setBoard(MyEnumClass _board[])
{
	legalMovesAvailable = false;
	for (int i = 0; i < 64; i++)
	{
		board[i] = _board[i];
	}
}

bool Chess::displayBoard()
{
	bool first = true;
	for (int i = 7*(!turn);;)
	{
		if (!inrange(i, 0, 7))
			break;
		if (!first)
			;// printf("       ---------------\n");
		else
			printf("       Chess  Position\n\n");
		first = false;
		bool second = true;

		for (int j = 7 * (turn);;)
		{
			if (!inrange(j, 0, 7))
				break;
			if(!second)
				printf("|");
			else
				printf("       ");
			second = false;
			switch (board[i * 8 + j])
			{
			case MyEnumClass::pawn:
			case MyEnumClass::knight:
			case MyEnumClass::bishop:
			case MyEnumClass::rook:
			case MyEnumClass::queen:
			case MyEnumClass::king:
				if ((i + j + 1) % 2)
				{
					SetConsoleTextAttribute(hstdout, 0x0b);
				}
				else
				{
					SetConsoleTextAttribute(hstdout, 0x8b);
				}
				break;

			case MyEnumClass::_pawn:
			case MyEnumClass::_knight:
			case MyEnumClass::_bishop:
			case MyEnumClass::_rook:
			case MyEnumClass::_queen:
			case MyEnumClass::_king:
				if ((i + j + 1) % 2)
				{
					SetConsoleTextAttribute(hstdout, 0x0c);
				}
				else
				{
					SetConsoleTextAttribute(hstdout, 0x8c);
				}
				break;
			default:

				if ((i + j + 1) % 2)
				{
					SetConsoleTextAttribute(hstdout, 0x0f);
				}
				else
				{
					SetConsoleTextAttribute(hstdout, 0x80);
				}
			}
			printf("%c", enumToChar(board[i * 8 + j]));

			SetConsoleTextAttribute(hstdout, 0x0f);

			if (turn)
				j--;
			else
				j++;
		}
		printf(" %d\n",i + 1);

		if (!turn)
			i--;
		else
			i++;
	}
	printf("\n");
	first = true;
	for (int i = 7*(turn); ;)
	{
		if (!inrange(i, 0, 7))
			break;
		if (!first)
			printf("|");
		else
			printf("       ");
		first = false;
		printf("%c",i + 'A');
		if (turn)
			i--;
		else
			i++;
	}
	printf("\n");
	bool check = isCheck();
	int legal = displayLegalMoves();
	if (check && (legal != 0))
	{
		printf("CHECK!!! ");
	}
	printf("legal moves: %d\n\n", legal);
	if (legal == 0)
	{
		printf("GAME OVER!");
		if (check)
		{
			printf(" Checkmate!");
		}
		else
		{
			printf(" Stalemate!");
		}
		return false;
	}
	return true;
}

MyEnumClass* Chess::getBoard() const
{
	return (MyEnumClass*)board;
}

MyEnumClass* Chess::getTestBoard() const
{
	return (MyEnumClass*)board_test;
}

bool Chess::move(char c1, char r1, char c2, char r2)
{
	if (legalMovesAvailable == false)
		displayLegalMoves();
	_move m = { c1,r1,c2,r2 };
	for (int i=0;i<moves.size();i++)
	{
		if (m == moves[i])
			return moveIndexed(i);
	}
	return false;
}

bool Chess::move2(char c1, char r1, char c2, char r2)
{
	bool isRook = (board[c1 * 8 + r1] == MyEnumClass::rook) || (board[c1 * 8 + r1] == MyEnumClass::_rook);
	bool isKing = (board[c1 * 8 + r1] == MyEnumClass::king) || (board[c1 * 8 + r1] == MyEnumClass::_king);
	bool isPawn = (board[c1 * 8 + r1] == MyEnumClass::pawn) || (board[c1 * 8 + r1] == MyEnumClass::_pawn);
	bool isCapture = board[c2 * 8 + r2] != MyEnumClass::empty;

	if (sub_move(c1, r1, c2, r2) == false)
	{
		restore();
		return false;
	}

	if (isCapture || isPawn)
	{
		stalematecounter = 0;
	}
	else
	{
		stalematecounter++;
	}

	if (isRook & turn & ((r1 == 0) && (c1 == 7) && blackCastleL))
		blackCastleL = false;
	if (isRook & turn & ((r1 == 7) && (c1 == 7) && blackCastleR))
		blackCastleR = false;
	if (isKing & turn & (blackCastleR || blackCastleL))
	{
		blackCastleL = false;
		blackCastleR = false;
	}
	if (isRook & !turn & ((r1 == 0) && (c1 == 0) && whiteCastleL))
		whiteCastleL = false;
	if (isRook & !turn & ((r1 == 7) && (c1 == 0) && whiteCastleR))
		whiteCastleR = false;
	if (isKing & !turn & (whiteCastleR || whiteCastleL))
	{
		whiteCastleL = false;
		whiteCastleR = false;
	}

	if (isCapture && ((r2 % 7) == 0) && ((c2 % 7) == 0))
	{
		if ((r2 == 7) && (c2 == 0) && whiteCastleR)
		{
			whiteCastleR = false;
		}
		if ((r2 == 0) && (c2 == 0) && whiteCastleL)
		{
			whiteCastleL = false;
		}

		if ((r2 == 7) && (c2 == 7) && blackCastleR)
		{
			blackCastleR = false;
		}
		if ((r2 == 0) && (c2 == 7) && blackCastleL)
		{
			blackCastleL = false;
		}
	}

	turn = !turn;
	legalMovesAvailable = false;
	return true;
}

bool Chess::moveIndexed(unsigned indexMove)
{
	return move2(moves[indexMove].c[0], moves[indexMove].c[1], moves[indexMove].c[2], moves[indexMove].c[3]);
}

bool Chess::isEmpty(MyEnumClass p)
{
	if (p == MyEnumClass::empty || p == MyEnumClass::en_passant || p == MyEnumClass::_en_passant)
		return true;
	return false;
}

int Chess::test_move(char c1, char r1, char c2, char r2)
{
	backup();

	if (sub_move(c1, r1, c2, r2) == false)
		return 1;
	MyEnumClass local[8 * 8];
	for (int i = 0; i < 8 * 8; i++)
	{
		local[i] = board[i];
	}
	restore();
	for (int i = 0; i < 8 * 8; i++)
	{
		board_test[i] = local[i];
	}

	return 0;
}


bool Chess::isLegalMove(char c1, char r1, char c2, char r2)
{
	return isLegalMove(c1, r1, c2, r2, turn, board);
}

bool Chess::isLegalMove(char c1, char r1, char c2, char r2, bool _turn, MyEnumClass board[])
{
	if (!inrange(c1, 0, 7) || !inrange(c2, 0, 7) || !inrange(r1, 0, 7) || !inrange(r2, 0, 7))
		return false;
	switch (board[c1 * 8 + r1])
	{
	case MyEnumClass::pawn:
	case MyEnumClass::knight:
	case MyEnumClass::bishop:
	case MyEnumClass::rook:
	case MyEnumClass::queen:
	case MyEnumClass::king:
		//target is white
		if (_turn)
			return false;
		break;

	case MyEnumClass::_pawn:
	case MyEnumClass::_knight:
	case MyEnumClass::_bishop:
	case MyEnumClass::_rook:
	case MyEnumClass::_queen:
	case MyEnumClass::_king:
		//target is black
		if (!_turn)
			return false;
		break;
	}
	int dX = r1 - r2;
	int dY = c1 - c2;
	switch (board[c1 * 8 + r1])
	{
	case MyEnumClass::empty:
		return false;
		break;
	case MyEnumClass::pawn:
		if (dY == -1)
		{
			if (canCapture(board[c1 * 8 + r1], board[c2 * 8 + r2]) && abs(dX) == 1)
			{
				return true;
			}
			if ((isEmpty(board[c2 * 8 + r2])) && dX == 0)
			{
				return true;
			}
		}
		if (dY == -2 && dX == 0 && c1 == 1 && board[c2 * 8 + r2] == MyEnumClass::empty && board[(c1 + 1) * 8 + r2] == MyEnumClass::empty)
		{
			return true;
		}
		return false;
	case MyEnumClass::_pawn:
		if (dY == 1)
		{
			if (canCapture(board[c1 * 8 + r1], board[c2 * 8 + r2]) && abs(dX) == 1)
			{
				return true;
			}
			if ((isEmpty(board[c2 * 8 + r2])) && dX == 0)
			{
				return true;
			}
		}
		if (dY == 2 && dX == 0 && c1 == 6 && board[c2 * 8 + r2] == MyEnumClass::empty && board[(c1 - 1) * 8 + r2] == MyEnumClass::empty)
		{
			return true;
		}
		return false;
	case MyEnumClass::knight:
	case MyEnumClass::_knight:
		if ((abs(dX) == 2 && abs(dY) == 1) || (abs(dX) == 1 && abs(dY) == 2))
		{
			if (canCapture(board[c1 * 8 + r1], board[c2 * 8 + r2]))
			{
				return true;
			}
			if ((isEmpty(board[c2 * 8 + r2])))
			{
				return true;
			}
		}
		return false;
	case MyEnumClass::king:
	case MyEnumClass::_king:
		if (abs(dX) < 2 && abs(dY) < 2)
		{
			if (canCapture(board[c1 * 8 + r1], board[c2 * 8 + r2]))
			{
				return true;
			}
			if ((isEmpty(board[c2 * 8 + r2])))
			{
				return true;
			}
		}
		if (_turn && (blackCastleL || blackCastleR))
		{
			if ((dX == 2) && (dY == 0) && blackCastleL)
			{
				if ((isEmpty(board[c2 * 8 + r2])) && (board[c2 *8 + (r2 + r1)/2] == MyEnumClass::empty) && (board[c2 * 8 + 1] == MyEnumClass::empty) &&  board[c2 * 8] == MyEnumClass::_rook)
				{
					if (!isMove(board, c1, r1) && !isMove(board, c2, r2) && !isMove(board, c1, (r1 + r2) / 2))
					{
						return true;
					}
				}
			}
			if ((dX == -2) && (dY == 0) && blackCastleR)
			{
				if ((isEmpty(board[c2 * 8 + r2])) && (board[c2 * 8 + (r2 + r1) / 2] == MyEnumClass::empty) && board[c2 * 8 + 7] == MyEnumClass::_rook)
				{
					if (!isMove(board, c1, r1) && !isMove(board, c2, r2) && !isMove(board, c1, (r1 + r2) / 2))
					{
						return true;
					}
				}
			}
		}
		if (!_turn && (whiteCastleL || whiteCastleR))
		{
			if ((dX == 2) && (dY == 0) && whiteCastleL)
			{
				if ((isEmpty(board[c2 * 8 + r2])) && (board[c2 * 8 + (r2 + r1) / 2] == MyEnumClass::empty) && (board[c2 * 8 + 1] == MyEnumClass::empty) && board[c2 * 8] == MyEnumClass::rook)
				{
					if (!isMove(board, c1, r1) && !isMove(board, c2, r2) && !isMove(board, c1, (r1 + r2) / 2))
					{
						return true;
					}
				}
			}
			if ((dX == -2) && (dY == 0) && whiteCastleR)
			{
				if ((isEmpty(board[c2 * 8 + r2])) && (board[c2 * 8 + (r2 + r1) / 2] == MyEnumClass::empty) && board[c2 * 8 + 7] == MyEnumClass::rook)
				{
					if (!isMove(board, c1, r1) && !isMove(board, c2, r2) && !isMove(board, c1, (r1 + r2) / 2))
					{
						return true;
					}
				}
			}
		}
		return false;
	case MyEnumClass::rook:
	case MyEnumClass::_rook:
		if (dX == 0 || dY == 0)
		{
			if (isLegalMoveNext(c1, r1, c2 + sgn(dY), r2 + sgn(dX), board))
			{
				if (canCapture(board[c1 * 8 + r1], board[c2 * 8 + r2]))
				{
					return true;
				}
				if ((isEmpty(board[c2 * 8 + r2])))
				{
					return true;
				}
			}
		}
		return false;
	case MyEnumClass::bishop:
	case MyEnumClass::_bishop:
		if (abs(dX) == abs(dY))
		{
			if (isLegalMoveNext(c1, r1, c2 + sgn(dY), r2 + sgn(dX), board))
			{
				if (canCapture(board[c1 * 8 + r1], board[c2 * 8 + r2]))
				{
					return true;
				}
				if ((isEmpty(board[c2 * 8 + r2])))
				{
					return true;
				}
			}
		}
		return false;
	case MyEnumClass::queen:
	case MyEnumClass::_queen:
		if (abs(dX) == abs(dY))
		{
			if (isLegalMoveNext(c1, r1, c2 + sgn(dY), r2 + sgn(dX), board))
			{
				if (canCapture(board[c1 * 8 + r1], board[c2 * 8 + r2]))
				{
					return true;
				}
				if ((isEmpty(board[c2 * 8 + r2])))
				{
					return true;
				}
			}
		}
		if (dX == 0 || dY == 0)
		{
			if (isLegalMoveNext(c1, r1, c2 + sgn(dY), r2 + sgn(dX), board))
			{
				if (canCapture(board[c1 * 8 + r1], board[c2 * 8 + r2]))
				{
					return true;
				}
				if ((isEmpty(board[c2 * 8 + r2])))
				{
					return true;
				}
			}
		}
		return false;
	}
	return false;
}

bool Chess::isLegalMoveCheck(char c1, char r1, char c2, char r2, bool _turn)
{
	if (isLegalMove(c1, r1, c2, r2, _turn,board))
	{
		return (test_move(c1, r1, c2, r2) == 0);
	}
	return false;
}

bool Chess::isLegalMoveNext(char c1, char r1, char c2, char r2, MyEnumClass board[])
{
	if ((!inrange(c1, 0, 7)) || (!inrange(c2, 0, 7)) || (!inrange(r1, 0, 7)) || (!inrange(r2, 0, 7)))
		return false;

	if (c1 == c2 && r1 == r2)
		return true;

	int dX = r1 - r2;
	int dY = c1 - c2;
	switch (board[c1 * 8 + r1])
	{
	case MyEnumClass::rook:
	case MyEnumClass::_rook:
		if (dX == 0 || dY == 0)
		{
			if (isLegalMoveNext(c1, r1, c2 + sgn(dY), r2 + sgn(dX), board))
			{
				if ((isEmpty(board[c2 * 8 + r2])))
				{
					return true;
				}
			}
		}
		return false;
	case MyEnumClass::bishop:
	case MyEnumClass::_bishop:
		if (abs(dX) == abs(dY))
		{
			if (isLegalMoveNext(c1, r1, c2 + sgn(dY), r2 + sgn(dX), board))
			{
				if ((isEmpty(board[c2 * 8 + r2])))
				{
					return true;
				}
			}
		}
		return false;

	case MyEnumClass::queen:
	case MyEnumClass::_queen:
		if (dX == 0 || dY == 0)
		{
			if (isLegalMoveNext(c1, r1, c2 + sgn(dY), r2 + sgn(dX), board))
			{
				if ((isEmpty(board[c2 * 8 + r2])))
				{
					return true;
				}
			}
		}
		if (abs(dX) == abs(dY))
		{
			if (isLegalMoveNext(c1, r1, c2 + sgn(dY), r2 + sgn(dX), board))
			{
				if ((isEmpty(board[c2 * 8 + r2])))
				{
					return true;
				}
			}
		}
		return false;
	}
}

float Chess::inspectPosition()
{
	return inspectPosition(board);
}

float Chess::inspectTestPosition()
{
	return inspectPosition(board_test);
}

char Chess::enumToChar(MyEnumClass e)
{
	switch (e)
	{
	case MyEnumClass::pawn:
	case MyEnumClass::_pawn:
		return 'p';
		break;
	case MyEnumClass::knight:
	case MyEnumClass::_knight:
		return 'N';
		break;
	case MyEnumClass::bishop:
	case MyEnumClass::_bishop:
		return 'B';
		break;
	case MyEnumClass::rook:
	case MyEnumClass::_rook:
		return 'R';
		break;
	case MyEnumClass::queen:
	case MyEnumClass::_queen:
		return 'Q';
		break;
	case MyEnumClass::king:
	case MyEnumClass::_king:
		return 'K';
		break;
	case MyEnumClass::en_passant:
	case MyEnumClass::_en_passant:
		return '#';
		break;
	default:
		return ' ';
	}
}

void Chess::backup()
{
	for (int i = 0; i < 8 * 8; i++)
	{
		board_test[i] = board[i];
	}
}

void Chess::restore()
{
	for (int i = 0; i < 8 * 8; i++)
	{
		board[i] = board_test[i];
	}
}

void Chess::setHandle()
{
	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
}

bool Chess::isCheck()
{
	
	if (!turn)
	{
		int kingIndex = -1;
		for (int i = 0; i < 8 * 8; i++)
		{
			if (board[i] == MyEnumClass::king)
			{
				kingIndex = i;
				break;
			}
		}

		for (int i = 0; i < 8 * 8; i++)
		{
			switch (board[i])
			{
			case MyEnumClass::_pawn:
			case MyEnumClass::_knight:
			case MyEnumClass::_bishop:
			case MyEnumClass::_rook:
			case MyEnumClass::_queen:
			case MyEnumClass::_king:
				if (isLegalMove(i / 8, i % 8, kingIndex / 8, kingIndex % 8, !turn,board))
				{
					return true;
				}
			default:
				;
			}
		}
	}
	else
	{
		int kingIndex = -1;
		for (int i = 0; i < 8 * 8; i++)
		{
			if (board[i] == MyEnumClass::_king)
			{
				kingIndex = i;
				break;
			}
		}

		for (int i = 0; i < 8 * 8; i++)
		{
			switch (board[i])
			{
			case MyEnumClass::pawn:
			case MyEnumClass::knight:
			case MyEnumClass::bishop:
			case MyEnumClass::rook:
			case MyEnumClass::queen:
			case MyEnumClass::king:
				if (isLegalMove(i / 8, i % 8, kingIndex / 8, kingIndex % 8, !turn,board))
				{
					return true;
				}
			default:
				;
			}
		}
	}

	return false;
}

bool Chess::isCheckTest(bool _turn)
{
	if (!_turn)
	{
		int kingIndex = -1;
		for (int i = 0; i < 8 * 8; i++)
		{
			if (board_test[i] == MyEnumClass::king)
			{
				kingIndex = i;
				break;
			}
		}

		for (int i = 0; i < 8 * 8; i++)
		{
			switch (board_test[i])
			{
			case MyEnumClass::_pawn:
			case MyEnumClass::_knight:
			case MyEnumClass::_bishop:
			case MyEnumClass::_rook:
			case MyEnumClass::_queen:
			case MyEnumClass::_king:
				if (isLegalMove(i / 8, i % 8, kingIndex / 8, kingIndex % 8, !_turn, board_test))
				{
					return true;
				}
			default:
				;
			}
		}
	}
	else
	{
		int kingIndex = -1;
		for (int i = 0; i < 8 * 8; i++)
		{
			if (board_test[i] == MyEnumClass::_king)
			{
				kingIndex = i;
				break;
			}
		}

		for (int i = 0; i < 8 * 8; i++)
		{
			switch (board_test[i])
			{
			case MyEnumClass::pawn:
			case MyEnumClass::knight:
			case MyEnumClass::bishop:
			case MyEnumClass::rook:
			case MyEnumClass::queen:
			case MyEnumClass::king:
				if (isLegalMove(i / 8, i % 8, kingIndex / 8, kingIndex % 8, !_turn, board_test))
				{
					return true;
				}
			default:
				;
			}
		}
	}

	return false;
}

bool Chess::isMove(MyEnumClass board[], char c1, char r1)
{
	if (!turn)
	{
		int kingIndex = c1 * 8 + r1;

		for (int i = 0; i < 8 * 8; i++)
		{
			switch (board[i])
			{
			case MyEnumClass::_pawn:
			case MyEnumClass::_knight:
			case MyEnumClass::_bishop:
			case MyEnumClass::_rook:
			case MyEnumClass::_queen:
			case MyEnumClass::_king:
				if (isLegalMove(i / 8, i % 8, kingIndex / 8, kingIndex % 8, !turn, board))
				{
					return true;
				}
			default:
				;
			}
		}
	}
	else
	{
		int kingIndex = -1;
		for (int i = 0; i < 8 * 8; i++)
		{
			if (board[i] == MyEnumClass::_king)
			{
				kingIndex = i;
				break;
			}
		}

		for (int i = 0; i < 8 * 8; i++)
		{
			switch (board[i])
			{
			case MyEnumClass::pawn:
			case MyEnumClass::knight:
			case MyEnumClass::bishop:
			case MyEnumClass::rook:
			case MyEnumClass::queen:
			case MyEnumClass::king:
				if (isLegalMove(i / 8, i % 8, kingIndex / 8, kingIndex % 8, !turn, board))
				{
					return true;
				}
			default:
				;
			}
		}
	}

	return false;
}

int Chess::displayLegalMoves(bool display)
{
	if (legalMovesAvailable == true)
		return moves.size();
	int legalmoves = 0;
	moves.clear();

	for (int index = 0; index < 8 * 8; index++)
	{
		int c1 = index / 8, r1 = index % 8;
		int c2 = index / 8, r2 = index % 8;
		bool legal = false;
		//locate pieces
		switch (board[index])
		{
		case MyEnumClass::pawn:
			if (isLegalMoveCheck(c1, r1, c1 + 1, r1, turn))
			{
				legal = true;
				c2 = c1 + 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 + 2, r1, turn))
			{
				legal = true;
				c2 = c1 + 2;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 + 1, r1 + 1, turn))
			{
				legal = true;
				c2 = c1 + 1;
				r2 = r1 + 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 + 1, r1 - 1, turn))
			{
				legal = true;
				c2 = c1 + 1;
				r2 = r1 - 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			break;
		case MyEnumClass::_pawn:
			if (isLegalMoveCheck(c1, r1, c1 - 1, r1, turn))
			{
				legal = true;
				c2 = c1 - 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 - 2, r1, turn))
			{
				legal = true;
				c2 = c1 - 2;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 - 1, r1 + 1, turn))
			{
				legal = true;
				c2 = c1 - 1;
				r2 = r1 + 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 - 1, r1 - 1, turn))
			{
				legal = true;
				c2 = c1 - 1;
				r2 = r1 - 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			break;
		case MyEnumClass::_knight:
		case MyEnumClass::knight:
			if (isLegalMoveCheck(c1, r1, c1 - 1, r1 + 2, turn))
			{
				legal = true;
				c2 = c1 - 1;
				r2 = r1 + 2;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 - 1, r1 - 2, turn))
			{
				legal = true;
				c2 = c1 - 1;
				r2 = r1 - 2;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 - 2, r1 + 1, turn))
			{
				legal = true;
				c2 = c1 - 2;
				r2 = r1 + 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 - 2, r1 - 1, turn))
			{
				legal = true;
				c2 = c1 - 2;
				r2 = r1 - 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 + 1, r1 + 2, turn))
			{
				legal = true;
				c2 = c1 + 1;
				r2 = r1 + 2;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 + 1, r1 - 2, turn))
			{
				legal = true;
				c2 = c1 + 1;
				r2 = r1 - 2;
				legalmoves++;
				_move m = { c1,r1,c2,r2 };
				moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 + 2, r1 + 1, turn))
			{
				legal = true;
				c2 = c1 + 2;
				r2 = r1 + 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			if (isLegalMoveCheck(c1, r1, c1 + 2, r1 - 1, turn))
			{
				legal = true;
				c2 = c1 + 2;
				r2 = r1 - 1;
				legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
			}
			break;
		case MyEnumClass::_bishop:
		case MyEnumClass::bishop:
			for (int i = 1; i < 8; i++)
			{
				if (isLegalMoveCheck(c1, r1, c1 - i, r1 - i, turn))
				{
					legal = true;
					c2 = c1 - i;
					r2 = r1 - i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 - i, r1 + i, turn))
				{
					legal = true;
					c2 = c1 - i;
					r2 = r1 + i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 + i, r1 - i, turn))
				{
					legal = true;
					c2 = c1 + i;
					r2 = r1 - i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 + i, r1 + i, turn))
				{
					legal = true;
					c2 = c1 + i;
					r2 = r1 + i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
			}
			break;
		case MyEnumClass::_rook:
		case MyEnumClass::rook:
			for (int i = 1; i < 8; i++)
			{
				if (isLegalMoveCheck(c1, r1, c1, r1 - i, turn))
				{
					legal = true;
					c2 = c1;
					r2 = r1 - i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1, r1 + i, turn))
				{
					legal = true;
					c2 = c1;
					r2 = r1 + i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 - i, r1, turn))
				{
					legal = true;
					c2 = c1 - i;
					r2 = r1;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 + i, r1, turn))
				{
					legal = true;
					c2 = c1 + i;
					r2 = r1;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
			}
			break;
		case MyEnumClass::_queen:
		case MyEnumClass::queen:
			for (int i = 1; i < 8; i++)
			{
				if (isLegalMoveCheck(c1, r1, c1 - i, r1 - i, turn))
				{
					legal = true;
					c2 = c1 - i;
					r2 = r1 - i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 - i, r1 + i, turn))
				{
					legal = true;
					c2 = c1 - i;
					r2 = r1 + i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 + i, r1 - i, turn))
				{
					legal = true;
					c2 = c1 + i;
					r2 = r1 - i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 + i, r1 + i, turn))
				{
					legal = true;
					c2 = c1 + i;
					r2 = r1 + i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1, r1 - i, turn))
				{
					legal = true;
					c2 = c1;
					r2 = r1 - i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1, r1 + i, turn))
				{
					legal = true;
					c2 = c1;
					r2 = r1 + i;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 - i, r1, turn))
				{
					legal = true;
					c2 = c1 - i;
					r2 = r1;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
				if (isLegalMoveCheck(c1, r1, c1 + i, r1, turn))
				{
					legal = true;
					c2 = c1 + i;
					r2 = r1;
					legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
				}
			}
			break;
		case MyEnumClass::_king:
		case MyEnumClass::king:
			for (int i = -2; i < 3; i++)
			{
				for (int j = -2; j < 3; j++)
				{
					if ((i == j) && (i == 0))
						continue;
					if (isLegalMoveCheck(c1, r1, c1 + j, r1 + i, turn))
					{
						c2 = c1 + j;
						r2 = r1 + i;
						legalmoves++; _move m = { c1,r1,c2,r2 }; moves.push_back(m);
					}
				}
			}
			break;
			//my pieces
		}
	}
	if (display)
		for (int i = 0; i < moves.size(); i++)
		{
			if (board[moves[i].c[2] * 8 + moves[i].c[3]] == MyEnumClass::empty)
				printf("%d: %c%c%c-%c%c\n", i,
					(enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]]) >= 'A' && enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]]) <= 'Z') ? (enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]])) : ' ',
					intToString(moves[i].c[0] * 8 + moves[i].c[1], true),
					intToString(moves[i].c[0] * 8 + moves[i].c[1], false),
					intToString(moves[i].c[2] * 8 + moves[i].c[3], true),
					intToString(moves[i].c[2] * 8 + moves[i].c[3], false));
			else
				printf("%d: %c%c%cx%c%c\n", i,
					(enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]]) >= 'A' && enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]]) <= 'Z') ? (enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]])) : ' ',
					intToString(moves[i].c[0] * 8 + moves[i].c[1], true),
					intToString(moves[i].c[0] * 8 + moves[i].c[1], false),
					intToString(moves[i].c[2] * 8 + moves[i].c[3], true),
					intToString(moves[i].c[2] * 8 + moves[i].c[3], false));
		}

	legalMovesAvailable = true;

	return legalmoves;
}

char Chess::intToString(int Int, bool first)
{
	if (first)
	{
		return Int % 8 + 'a';
	}
	else
	{
		return (Int / 8) + '1';
	}
}

int Chess::getResult()
{
	if (stalematecounter > stalemateAfter)
		return 2;
	if (!isCheck())
		return 0;
	else
		if (turn)
			return 1;
		else
			return -1;
}

MyEnumClass* Chess::exportMoveByindex(unsigned num)
{
	if (num < displayLegalMoves())
	{
		_move m = moves[num];

		if (int val = test_move(m.c[0], m.c[1], m.c[2], m.c[3]))
		{
			val++;
			throw ERROR;
		}
	}
	return getTestBoard();
}

void Chess::cls()
{
	_cls();
}

bool Chess::getTurn()
{
	return turn;
}

void Chess::setTurn(bool _turn)
{
	turn = _turn;
}

bool Chess::canCapture(MyEnumClass me, MyEnumClass target)
{
	if (me == MyEnumClass::pawn && target == MyEnumClass::_en_passant)
		return true;
	if (me == MyEnumClass::_pawn && target == MyEnumClass::en_passant)
		return true;
	switch (me)
	{
	case MyEnumClass::pawn:
	case MyEnumClass::knight:
	case MyEnumClass::bishop:
	case MyEnumClass::rook:
	case MyEnumClass::queen:
	case MyEnumClass::king:
		//me is white
		switch (target)
		{
		case MyEnumClass::_pawn:
		case MyEnumClass::_knight:
		case MyEnumClass::_bishop:
		case MyEnumClass::_rook:
		case MyEnumClass::_queen:
		case MyEnumClass::_king:
			//target is black
			return true;
			break;
		}
		break;

	case MyEnumClass::_pawn:
	case MyEnumClass::_knight:
	case MyEnumClass::_bishop:
	case MyEnumClass::_rook:
	case MyEnumClass::_queen:
	case MyEnumClass::_king:
		//me is black
		switch (target)
		{
		case MyEnumClass::pawn:
		case MyEnumClass::knight:
		case MyEnumClass::bishop:
		case MyEnumClass::rook:
		case MyEnumClass::queen:
		case MyEnumClass::king:
			//target is white
			return true;
			break;
		}
		break;
	}
	return false;
}

void getInput(char& c1, char& r1, char& c2, char& r2)
{
	gotoxy(0, 0);
	gotoxy(0, 13);

	r1 = _getch();
	printf("%c", r1);
	c1 = _getch();
	printf("%c-", c1);
	r2 = _getch();
	printf("%c", r2);
	c2 = _getch();
	printf("%c", c2);
	r1 -= 'a';
	r2 -= 'a';
	c1 -= '1';
	c2 -= '1';

}

void announceIlegalMove()
{
	printf("Ilegal Move!!!!!!\n");
}

bool operator==(_move a, _move b)
{
	for (int i = 0; i < 4; i++)
	{
		if (a.c[i] != b.c[i])
			return false;
	}
	return true;
}

char* Chess::exportMoveStrByindex(unsigned i)
{
	if (board[moves[i].c[2] * 8 + moves[i].c[3]] == MyEnumClass::empty)
		sprintf_s(moveStr, 100, "%c%c%c-%c%c, ",
			(enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]]) >= 'A' && enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]]) <= 'Z') ? (enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]])) : ' ',

			intToString(moves[i].c[0] * 8 + moves[i].c[1], true),
			intToString(moves[i].c[0] * 8 + moves[i].c[1], false),

			intToString(moves[i].c[2] * 8 + moves[i].c[3], true),
			intToString(moves[i].c[2] * 8 + moves[i].c[3], false));
	else
		sprintf_s(moveStr, 100, "%c%c%cx%c%c, ", (enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]]) >= 'A' && enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]]) <= 'Z') ? (enumToChar(board[moves[i].c[0] * 8 + moves[i].c[1]])) : ' ',

			intToString(moves[i].c[0] * 8 + moves[i].c[1], true),
			intToString(moves[i].c[0] * 8 + moves[i].c[1], false),

			intToString(moves[i].c[2] * 8 + moves[i].c[3], true),
			intToString(moves[i].c[2] * 8 + moves[i].c[3], false));

	return (moveStr);
}