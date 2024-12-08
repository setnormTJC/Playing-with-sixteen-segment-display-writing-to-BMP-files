#pragma once
#include<vector> 
#include<array> 

#include<memory>

#include<string> 

#include<iostream> 

#include<map> 

#include<set> //pieces will be unique (ex: white, queen side knight) 

using std::vector;
using std::string;
using std::set;
using std::array;
using std::map;

using std::ostream;

using std::cout;



class Piece
{
protected:

public:
	string pieceName;

	Piece()
	{
		pieceName = "";
	}

	friend std::ostream& operator << (ostream& os, const Piece& piece)
	{
		os << piece.pieceName;

		return os;
	}

};

class Knight : public Piece
{
private:

};

struct ChessPosition
{
	char rank;
	char file;

	bool operator == (const ChessPosition& other)
	{
		return (this->file == other.file && this->rank == other.rank);
	}


	bool operator < (const ChessPosition& other) const
	{
		return this->file < other.file;
	};

	friend std::ostream& operator << (ostream& os, const ChessPosition& chessPosition)
	{
		os << chessPosition.file << "" << chessPosition.rank << "";
		return os;
	}

};

class ChessGame
{
	set<Piece*> pieces;/* =
	{
		std::make_unique<Knight> whiteQueenSideKnight;
	};*/

	array<array<ChessPosition, 8>, 8> boardPositions; /*=
	{
		{},
		{},
		{},
		{},
		{},
	};*/

	map<ChessPosition, Piece*> theBoard;/* =
	{

	}*/


public:
	void initializeBoard()
	{
		//put pawns and pieces in their place - ex:
		//theBoard[ChessPosition{"A1"}] = pieces[0]; 
	}


	bool isValidMove(const Piece& piece, const ChessPosition& previousPosition, const ChessPosition& newPosition)
	{
		return true; //placeholder for very complicated logic ...
	}
	/*
	@param pieceName - ex: whiteQueenSideKnight
	@param newPosition - ex: A1, B3, etc.
	*/
	void movePiece(const Piece& piece, const ChessPosition& previousPosition, const ChessPosition& newPosition)
	{
		if (!isValidMove(piece, previousPosition, newPosition))
		{
			cout << piece << " not allowed to move to " << newPosition << "\n";
			return;
		}

		else
		{
			//auto previousPositionIterator = theBoard.find(previousPosition); 

			if (theBoard.find(previousPosition) != theBoard.end())
			{
				//remove piece from previous position by setting an "empty piece" there
				Piece emptyPiece;
				theBoard.at(previousPosition) = &emptyPiece;

				//move piece to new position: 
				//theBoard.at(newPosition) = 
				//theBoard[newPosition] = &pieceName;
			}

			else
			{
				cout << "Piece " << piece << " not found on board\n";
			}

		}

	};

	/*
	This should write a BMP image file
	*/
	void drawBoard()
	{

	};
};

