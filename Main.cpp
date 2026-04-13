#include "ChessAI.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void playerVSplayer()
{
	Chess chess;
	chess.setInitialBoard();
	ChessAI assistant;
	while (abs(chess.inspectPosition()) < 1500)
	{
		chess.cls();
		if (!chess.displayBoard())
			break;
		int legalmoves = chess.displayLegalMoves();
		if (legalmoves)
		{
			assistant.clearMoves();
			for (int i = 0; i < legalmoves; i++)
			{
				assistant.addMove(chess.exportMoveByindex(i));
			}
			assistant.Analyze(1, !chess.getTurn());
			vector<Scorable> indexed = assistant.byScore(!chess.getTurn());
			for (auto a : indexed)
			{
				printf("%d : %s %f\n", a.index, chess.exportMoveStrByindex(a.index), a.score);
			}
		}

		char c1, r1, c2, r2;

		getInput(c1, r1, c2, r2);

		if (chess.isLegalMove(c1, r1, c2, r2))
		{
			if (!chess.move(c1, r1, c2, r2))
			{
				announceIlegalMove();
				//system("PAUSE");
			}
		}
		else
		{
			announceIlegalMove();
			//system("PAUSE");
		}
		//system("PAUSE");
	}

	printf("game result %d\n", chess.getResult());
	system("PAUSE");
}

void playerVSAI()
{
	bool AITurn = rand() % 2;
	Chess chess;
	chess.setInitialBoard();
	ChessAI assistant;
	bool wasLegal = true;
	vector<Scorable> indexed;
	while (abs(chess.inspectPosition()) < 1500)
	{
		chess.cls();
		if (!chess.displayBoard())
			break;
		int legalmoves = chess.displayLegalMoves();
		if (legalmoves && wasLegal)
		{
			assistant.clearMoves();
			for (int i = 0; i < legalmoves; i++)
			{
				assistant.addMove(chess.exportMoveByindex(i));
			}
			assistant.Analyze(1, !chess.getTurn());
			indexed = assistant.byScore(!chess.getTurn());
			for (auto a : indexed)
			{
				printf("%d : %s %f\n", a.index, chess.exportMoveStrByindex(a.index), a.score);
			}
		}
		else
		{
			for (auto a : indexed)
			{
				printf("%d : %s %f\n", a.index, chess.exportMoveStrByindex(a.index), a.score);
			}
		}

		if (chess.getTurn() == AITurn)
		{
			assistant.Analyze(3, !chess.getTurn());
			indexed = assistant.byScore(!chess.getTurn());
			//assistant.Analyze(2, !chess.getTurn());
			indexed = assistant.byScore(!chess.getTurn());
			chess.moveIndexed(indexed[0].index);
		}
		else
		{
			char c1, r1, c2, r2;
			getInput(c1, r1, c2, r2);
			if (chess.move(c1, r1, c2, r2))
			{
				wasLegal = true;
			}
			else
			{
				announceIlegalMove();
				wasLegal = false;
				//system("PAUSE");
			}
		}
		//system("PAUSE");
	}

	printf("game result %d\n", chess.getResult());
	system("PAUSE");
}

void AIVSAI()
{
	bool AITurn = true;
	Chess chess;
	chess.setInitialBoard();
	ChessAI assistant;
	while (abs(chess.inspectPosition()) < 1500)
	{
		chess.cls();
		if (!chess.displayBoard())
			break;
		vector<Scorable> indexed;
		int legalmoves = chess.displayLegalMoves();
		if (legalmoves)
		{
			assistant.clearMoves();
			for (int i = 0; i < legalmoves; i++)
			{
				assistant.addMove(chess.exportMoveByindex(i));
			}
			//Sleep(1000);
		}
		
		gotoxy(0, 0);
		gotoxy(0, 13);
		if (chess.getTurn() == AITurn)
		{
			printf("Running Branch Analyze ");
			assistant.Analyze(2, !chess.getTurn());
			indexed = assistant.byScore(!chess.getTurn());
			//assistant.Analyze(2, !chess.getTurn());
			//indexed = assistant.byScore(!chess.getTurn());
		}
		else
		{
			printf("being stupid ");
			assistant.Analyzer(3000, !chess.getTurn());
			indexed = assistant.byScore(!chess.getTurn());
		}
		printf("\n\n");
		for (auto a : indexed)
		{
			printf("%d : %s %f\n", a.index, chess.exportMoveStrByindex(a.index), a.score);
		}
		printf("%d : %s %f\n", indexed[0].index, chess.exportMoveStrByindex(indexed[0].index), indexed[0].score);
		chess.moveIndexed(indexed[0].index);
		//Sleep(500);
		//system("PAUSE");
	}

	printf("game result %d\n", chess.getResult());
	system("PAUSE");
}

void AIVSRand()
{
	bool AITurn = false;
	Chess chess;
	chess.setInitialBoard();
	ChessAI assistant;
	while (abs(chess.inspectPosition()) < 1500)
	{
		chess.cls();
		if (!chess.displayBoard())
			break;
		vector<Scorable> indexed;
		int legalmoves = chess.displayLegalMoves();
		if (legalmoves)
		{
			assistant.clearMoves();
			for (int i = 0; i < legalmoves; i++)
			{
				assistant.addMove(chess.exportMoveByindex(i));
			}
			//Sleep(1000);
		}

		gotoxy(0, 0);
		gotoxy(0, 13);
		if (chess.getTurn() == AITurn)
		{
			printf("Running Branch Analyze ");
			assistant.Analyze(0, !chess.getTurn());
			indexed = assistant.byScore(!chess.getTurn());
			//assistant.Analyze(2, !chess.getTurn());
			//indexed = assistant.byScore(!chess.getTurn());
			printf("\n\n");
			for (auto a : indexed)
			{
				printf("%d : %s %f\n", a.index, chess.exportMoveStrByindex(a.index), a.score);
			}
			printf("%d : %s %f\n", indexed[0].index, chess.exportMoveStrByindex(indexed[0].index), indexed[0].score);
			chess.moveIndexed(indexed[0].index);
		}
		else
		{
			printf("being stupid ");
			assistant.Analyze(0, !chess.getTurn());
			indexed = assistant.byScore(!chess.getTurn());
			printf("\n\n");
			for (auto a : indexed)
			{
				printf("%d : %s %f\n", a.index, chess.exportMoveStrByindex(a.index), a.score);
			}
			unsigned choose = rand() % legalmoves;
			printf("%d : %s %f\n", indexed[choose].index, chess.exportMoveStrByindex(indexed[choose].index), indexed[choose].score);
			chess.moveIndexed(indexed[choose].index);
		}
		
		//Sleep(500);
		//system("PAUSE");
	}

	printf("game result %d\n", chess.getResult());
	system("PAUSE");
}

void main()
{
	srand(time(0));
	Chess::setHandle();

	playerVSAI();
}