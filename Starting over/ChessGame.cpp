#include "ChessGame.h"

#pragma region Piece function implementations

std::ostream& operator << (ostream& os, const Piece& piece)
{
	os << piece.pieceName;

	return os;
}

Piece::Piece()
{
	pieceName = "";
}

/*NOTE! returns false PERIOD at the moment - might be deleting this method...*/
bool Piece::isLegalMove(const ChessPosition& positionToMoveFrom, const ChessPosition& positionToMoveTo)
{
	return false;
}

/*order by piece name alphabetical*/
bool Piece::operator<(const Piece& otherPiece) const
{
	return this->pieceName < otherPiece.pieceName;
}



#pragma endregion 

#pragma region Knight function implementations

vector<ChessPosition> Knight::generateLegalMoves(const ChessPosition& startPos)
{
	//L patterns ... don't go out of bounds 
	// 
	
	return vector<ChessPosition>();	
}

#pragma endregion 


#pragma region ChessPosition function implementations
bool ChessPosition::operator==(const ChessPosition& otherPosition) const
{
	return (this->column == otherPosition.column && this->row == otherPosition.row);
}

/*Ordered by column (AKA:file) (positions are said A1, B4, etc. (not 1A, 4B, etc.)*/
bool ChessPosition::operator<(const ChessPosition& other) const
{
	return this->column < other.column;
}

std::ostream& operator<<(ostream& os, const ChessPosition& chessPosition)
{
	os << chessPosition.column << "" << chessPosition.row << "";
	return os;
}

#pragma endregion

#pragma region ChessGame function implementations
void ChessGame::generateBoardPositions()
{
	int index = 0;
	for (int row = 1; row < 9; ++row)
	{
		for (char col = 'A'; col < 'I'; ++col)
		{
			boardPositions.at(index) = ChessPosition{ col, row };
		}
	}
}

void ChessGame::generatePieces()
{
	pieces.insert(Rook { "black rook q side" });
	pieces.insert(Knight{ "black knight q side" });
	pieces.insert(Bishop { "black bishop q side" });
	pieces.insert(Queen { "black queen" });
	pieces.insert(King{ "black king" });
	pieces.insert(Bishop { "black bishop k side" });
	pieces.insert(Knight { "black knight k side" });
	pieces.insert(Rook { "black rook k side" });

	for (char file = 'A'; file < 'I'; ++file)
	{
		string currentPawnName = "black " + file + string{ "7 pawn" };
		pieces.insert(Pawn{ currentPawnName });
	}

	pieces.insert(Rook { "white rook q side" });
	pieces.insert(Knight { "white knight q side" });
	pieces.insert(Bishop { "white bishop q side" });
	pieces.insert(Queen { "white queen" });
	pieces.insert(King { "white king" });
	pieces.insert(Bishop { "white bishop k side" });
	pieces.insert(Knight { "white knight k side" });
	pieces.insert(Rook { "white rook k side" });

	for (char file = 'A'; file < 'I'; ++file)
	{
		string currentPawnName = "white " + file + string{ "2 pawn" };
		pieces.insert(Pawn{ currentPawnName });
	}
}

void ChessGame::initializeBoard()
{
	//ex: theBoardState.at(ChessPosition{1, 'A"'}) = "White rook, q side";
}


#pragma endregion