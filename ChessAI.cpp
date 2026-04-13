#include "ChessAI.h"
#include <algorithm>

float trunk(float val)
{
	float x = (int)(10000 * (val)) / 10000.0;
	return x;
}

float ChessAI::getMaterialValue(MyEnumClass _board[])
{
	return ((getMaterialValue(_board,true) + getMaterialValue(_board, false))/2);
}

float ChessAI::getMaterialValue(MyEnumClass _board[], bool turn)
{
	chess.setBoard(_board);
	chess.setTurn(turn);
	if (chess.isCheck())
	{
		if (chess.displayLegalMoves() == 0)
		{
			if (turn)
				return 2000;
			else
				return -2000;
		}
		if (turn)
			return trunk(chess.inspectPosition() / 2 - 0.01);
		else
			return trunk(chess.inspectPosition() / 2 + 0.01);
	}
	else
	{
		if (chess.displayLegalMoves() == 0)
		{
			return 0;
		}
		return trunk(chess.inspectPosition() / 2);
	}
}

float ChessAI::subScore(MyEnumClass board[], int gen, bool _turn)
{
	Chess local;
	local.setInitialBoard();
	float scorelocal = 0;
	if (gen == 0)
	{
		int legalmoves = local.displayLegalMoves();
		if (legalmoves)
		{
		}
		else
		{
			local.setTurn(!_turn);
			return trunk(chess.getResult() * 2000);
		}
		return trunk(getMaterialValue(board));
	}
	else
	{
		local.setBoard(board);
		local.setTurn(_turn);
		int legalmoves = local.displayLegalMoves();
		if (legalmoves)
		{
			float temp = Analyze(local.exportMoveByindex(0), gen - 1, !_turn);
			for (int i = 1; i < legalmoves; i++)
			{
				float dat = subScore(local.exportMoveByindex(i), gen - 1, !_turn);
				
				if (_turn)
				{
					if (dat < temp)
						temp = dat;
				}
				else
				{
					if (dat > temp)
						temp = dat;
				}
			}
			return trunk(temp);
		}
		else
		{
			local.setTurn(_turn);
			if (abs(getMaterialValue(board)) < 500)
				return trunk(chess.getResult() * 2000);
			else
				return getMaterialValue(board);
		}
		
	}
}

float ChessAI::score(MyEnumClass board[])
{
	return trunk(getMaterialValue(board));
}

unsigned ChessAI::getBestMove()
{
	unsigned _index = 0;
	for (auto scored : scoreIndex)
	{
		if (scored.score > scoreIndex[_index].score)
		{
			_index = scored.index;
		}
	}
	return _index;
}

void ChessAI::clearMoves()
{
	scoreIndex.clear();
}

void ChessAI::addMove(MyEnumClass board[])
{
	scoreIndex.push_back(set(board , getMaterialValue(board),scoreIndex.size()) );
}

double ChessAI::random(double r)
{
	return r*(((double)rand()) / RAND_MAX);
}

float ChessAI::Analyze(MyEnumClass board[], int gen, bool _turn)
{
	if (gen == 0)
	{
		return trunk(getMaterialValue(board, _turn));
	}
	return trunk(subScore(board, gen, _turn));
}

void ChessAI::Analyzer_sub(vector<Scorable_2>& sub_scoreIndex, int& checks, bool _turn)
{
	if (checks <= 0)
		return;
	Chess local;
	float scorelocal = 0;

	
	//for each position
	vector<Scorable_2> _scoreIndex;
	_scoreIndex.clear();
	for (int i = 0; i < sub_scoreIndex.size(); i++)
	{
		if (checks > 0)
			checks--;
		else
			break;
		//create subPositions with inverted turn and temp_score them
		local.setBoard(sub_scoreIndex[i].board);
		local.setTurn(_turn);
		int legalmoves = local.displayLegalMoves();

		if (legalmoves)
		{
			float temp = (getMaterialValue(local.exportMoveByindex(0)));
			_scoreIndex.push_back(set(local.exportMoveByindex(0), temp, 0, i));
			for (int j = 1; j < legalmoves; j++)
			{
				float dat = getMaterialValue(local.exportMoveByindex(j));
				_scoreIndex.push_back(set(local.exportMoveByindex(j), dat,j, i));

				if (_turn)
				{
					if (dat < temp)
						temp = dat;
				}
				else
				{
					if (dat > temp)
						temp = dat;
				}
			}
		}
		else
			continue;
	}
	//apply best temp_score to sub_score_index
	vector<bool> wasAltered;
	for (int i = 0; i < sub_scoreIndex.size(); i++)
	{
		wasAltered.push_back(false);
	}
	for (auto a : _scoreIndex)
	{
		if (_turn)
		{
			//if (dat < temp)
			//	temp = dat;
			if (!wasAltered[a.parent_index])
			{
				sub_scoreIndex[a.parent_index].score = a.score;
				wasAltered[a.parent_index] = true;
			}
			if (sub_scoreIndex[a.parent_index].score > a.score)
			{
				sub_scoreIndex[a.parent_index].score = a.score;
			}
		}
		else
		{
			//if (dat > temp)
			//	temp = dat;
			if (sub_scoreIndex[a.parent_index].score < a.score)
			{
				sub_scoreIndex[a.parent_index].score = a.score;
			}
		}
	}
	if (checks > 0)
	{
		//go deeper
		Analyzer_sub(_scoreIndex, checks, !_turn);
		for (auto a : _scoreIndex)
		{
			if (_turn)
			{
				//if (dat < temp)
				//	temp = dat;
				if (sub_scoreIndex[a.parent_index].score > a.score)
				{
					sub_scoreIndex[a.parent_index].score = a.score;
				}
			}
			else
			{
				//if (dat > temp)
				//	temp = dat;
				if (sub_scoreIndex[a.parent_index].score < a.score)
				{
					sub_scoreIndex[a.parent_index].score = a.score;
				}
			}
		}
	}
}

void ChessAI::Analyze(int gen, bool _turn)
{
	for (int i = 0; i < scoreIndex.size(); i++)
	{
		float dat = Analyze(scoreIndex[i].board, gen, _turn);
		scoreIndex[i] = set(scoreIndex[i].board, dat, scoreIndex[i].index);
		if(dat != scoreIndex[i].score)
			scoreIndex[i].score = dat;
	}
}

void ChessAI::Analyzer(int checks, bool _turn)
{
	vector<Scorable_2> temp_scores;
	for (int i = 0; i < scoreIndex.size(); i++)
	{
		float dat = getMaterialValue(scoreIndex[i].board);
		Scorable_2 t_score = set(scoreIndex[i].board, dat, i, 0);
		temp_scores.push_back(t_score);
	}

	Analyzer_sub(temp_scores,checks,_turn);
}

bool myfunction(Scorable i, Scorable j)
{
	if (i.score > (j.score + 0.001))
		return true;
	return false;
}
bool myfunctionR(Scorable i, Scorable j)
{
	if (j.score > (i.score + 0.001))
		return true;
	return false;
}

bool myfunction2(Scorable i, Scorable j) { return (i.index < j.index); }

vector<Scorable> ChessAI::byScore(bool _turn)
{
	vector<Scorable> sortedVector;
	sortedVector.clear();
	std::random_shuffle(scoreIndex.begin(), scoreIndex.end());
	std::sort(scoreIndex.begin(), scoreIndex.end(), (_turn) ? myfunction : myfunctionR);
	for (int i = 0; i < scoreIndex.size(); i++)
	{
		sortedVector.push_back(scoreIndex[i]);
	}
	std::sort(scoreIndex.begin(), scoreIndex.end(), myfunction2);
	return sortedVector;
}

Scorable set(MyEnumClass _board[], float _score, unsigned _index)
{
	Scorable local;
	for (int i = 0; i < 64; i++)
	{
		local.board[i] = _board[i];
	}
	local.score = _score;
	local.index = _index;

	return local;
}

Scorable_2 set(MyEnumClass _board[], float _score, unsigned _index,unsigned p_index)
{
	Scorable_2 local;
	for (int i = 0; i < 64; i++)
	{
		local.board[i] = _board[i];
	}
	local.score = _score;
	local.index = _index;
	local.parent_index = p_index;

	return local;
}
