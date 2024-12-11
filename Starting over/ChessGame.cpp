#include "ChessGame.h"
#define _CRT_SECURE_NO_WARNINGS //stb image uses something like s_printf
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"


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

/*recall - piece names are specific to color AND queen's side v king's side, so this should be okay definition*/
bool Piece::operator==(const Piece& otherPiece) const
{
	return this->pieceName == otherPiece.pieceName; 
	//return false;
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
	if (this->column == other.column) //if columns are the same (ex: A1, A2), compare rows 
	{
		return this->row < other.row; 
	}

	return this->column < other.column;
}

string ChessPosition::convertPositionToString()
{
	string chessPositionAsString{};
	chessPositionAsString += column;
	chessPositionAsString += std::to_string(row);


	return chessPositionAsString;
}

std::ostream& operator<<(ostream& os, const ChessPosition& chessPosition)
{
	os << chessPosition.column << "" << chessPosition.row << "";
	return os;
}

#pragma endregion

#pragma region ChessGame function implementations
ChessGame::ChessGame()
{
	generateBoardPositions();
	generatePieces();
	initializeBoard();

	drawBoard();
}



void ChessGame::generateBoardPositions()
{
	int index = 0;
	for (int row = 1; row < 9; ++row)
	{
		for (char col = 'A'; col < 'I'; ++col)
		{
			boardPositions.at(index) = ChessPosition{ col, row };
			index++; 
		}
	}
}

void ChessGame::generatePieces()
{
	pieces.push_back(Rook{ "black rook q side" });
	pieces.push_back(Knight{ "black knight q side" });
	pieces.push_back(Bishop{ "black bishop q side" });
	pieces.push_back(Queen{ "black queen" });
	pieces.push_back(King{ "black king" });
	pieces.push_back(Bishop{ "black bishop k side" });
	pieces.push_back(Knight{ "black knight k side" });
	pieces.push_back(Rook{ "black rook k side" });

	for (char file = 'A'; file < 'I'; ++file)
	{
		string currentPawnName = "black ";
		currentPawnName += file;
		currentPawnName += string{ "7 pawn" }; //as in rank/row 7 

		pieces.push_back(Pawn{ currentPawnName });
	}

	for (char file = 'A'; file < 'I'; ++file)
	{
		string currentPawnName = "white ";
		currentPawnName += file;
		currentPawnName += string{ "2 pawn" };
		pieces.push_back(Pawn{ currentPawnName });
	}

	pieces.push_back(Rook{ "white rook q side" });
	pieces.push_back(Knight{ "white knight q side" });
	pieces.push_back(Bishop{ "white bishop q side" });
	pieces.push_back(Queen{ "white queen" });
	pieces.push_back(King{ "white king" });
	pieces.push_back(Bishop{ "white bishop k side" });
	pieces.push_back(Knight{ "white knight k side" });
	pieces.push_back(Rook{ "white rook k side" });

}

void ChessGame::initializeBoard()
{
	//ex: theBoardState.at(ChessPosition{1, 'A"'}) = "White rook, q side";
	//boardpositions are ordered by row - ex: {1A, 1B, 1C, ... 1H, 2A, 2B, ...2H ...8H}

	//insert white pieces
	for (int i = 0; i < 8; ++i)
	{
		theBoardState.insert({ boardPositions[i], pieces[24 + i] });
	}

	//insert white PAWNS (piece indices 15 - 23)
	for (int i = 8; i < 16; ++i)
	{
		//theBoardState.insert({ boardPositions[i], pieces[16 + (i - 8)] });
		theBoardState.insert({ boardPositions[i], pieces[8+ i] });
	}

	//insert black pawns (piece indices 8 - 14): 
	for (int i = 48; i < 56; ++i)
	{
		theBoardState.insert({ boardPositions[i], pieces[i - 40] });
	}

	//insert black pieces (indices 0 - 7)
	for (int i = 56; i < 64; ++i)
	{
		theBoardState.insert({ boardPositions[i], pieces[i - 56] });
	}
	//theBoardState.at(boardPositions[0]) = pieces[? ];

}

void ChessGame::drawBoard()
{
	//draw from boardstate 
	auto theBoardImage = getImageRecordFromFile("emptyBoard.jpg");

	vector<ImageRecord> pieceImageRecords = readImageFilesInFolder("/pieceImages");

	map<string, ImageFileCoordinate> imageFileCoordinatesToChessPositions
		= mapImageFileCoordinatesToChessPositions();


	for (const auto& currentPieceImage : pieceImageRecords)
	{
		//ImageRecord currentPieceImage = pieceImageRecords.at(i); 
		//first find piece's chess position: 
		string pieceName = currentPieceImage.name; 
		Piece currentPiece{ pieceName }; 

		//auto it = std::find(theBoardState.begin(), theBoardState.end(), currentPiece); 
		//NO to the line above! cryptic error message results!

		/*!!!!!!!!!!!!!!!!!!!!!!!!!!*/
		auto it = std::find_if(theBoardState.begin(), theBoardState.end(),
			[&currentPiece](const std::pair<ChessPosition, Piece>& pair) //note the "captured" piece :) 
			{
				return pair.second == currentPiece;
			}
		);
		/*!!!!!!!!!!!!!!!!!!!!!!!!!!*/

		if (it != theBoardState.end())
		{
			ChessPosition positionOfCurrentPiece = it->first; 

			//now find x, y, value using ImageRecord's map: 
			string chessPositionAsString = positionOfCurrentPiece.convertPositionToString(); 

			ImageFileCoordinate imageFileCoord = imageFileCoordinatesToChessPositions.at(chessPositionAsString);
			//addForegroundImageToBgrdImage(40 + xShift, 40 + yShift, theBoardImage, pieceImageRecords[i]);
			//xShift += 90;
			//yShift += 90;
			addForegroundImageToBgrdImage(imageFileCoord.x, imageFileCoord.y, theBoardImage, currentPieceImage);
		}
		//theBoardStat
	}



	//now write by passing the updated array (pointer) to stbi_write and display the image using system call:
	const char* filename = "didItWork.png";
	//change current directory to the output images: (readImageFilesInFolder will alter 
	std::filesystem::current_path("outputImages");

	stbi_write_png(filename,
		theBoardImage.width, theBoardImage.height, theBoardImage.channelCount,
		theBoardImage.imagePtr,
		theBoardImage.width * theBoardImage.channelCount);
	

	system(filename);
	//std::cin.get();
}


#pragma endregion