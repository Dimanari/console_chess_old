#pragma once
#include "Chess.h"
struct Scorable
{
	MyEnumClass board[8 * 8];
	float score;
	unsigned index;
};
struct Scorable_2
{
	MyEnumClass board[8 * 8];
	float score;
	unsigned index;
	unsigned parent_index;
};

Scorable set(MyEnumClass _board[], float _score, unsigned _index);
Scorable_2 set(MyEnumClass _board[], float _score, unsigned _index, unsigned p_index);
class ChessAI
{
	vector<Scorable> scoreIndex;
	float getMaterialValue(MyEnumClass _board[]);
	float getMaterialValue(MyEnumClass _board[],bool turn);
	Chess chess;
	float subScore(MyEnumClass board[], int gen, bool _turn);
	float Analyze(MyEnumClass board[], int gen, bool _turn);
	void Analyzer_sub(vector<Scorable_2>& sub_scoreIndex, int& checks, bool _turn);
public:
	float score(MyEnumClass board[]);
	unsigned getBestMove();
	void clearMoves();
	void addMove(MyEnumClass board[]);
	double random(double r);
	void Analyze(int gen, bool _turn);
	void Analyzer(int checks,bool _turn);
	vector<Scorable> byScore(bool);
};