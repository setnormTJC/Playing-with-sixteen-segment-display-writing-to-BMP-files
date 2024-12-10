// Starting over.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS //stb image uses something like s_printf

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include"ImageRecord.h"
#include <iostream>

#include<cassert>

#include<map> 
#include "ChessGame.h"

using std::map; 
using std::cout; 
using std::cin; 


//void addForegroundImageToBgrdImage(int verticalShift, int horizontalShift,
//    ImageRecord& backgroundImageRecord,
//    const ImageRecord& foregroundImageRecord)
//{
//
//
//    assert(backgroundImageRecord.imageSize > foregroundImageRecord.imageSize);
//    //int verticalShift = 1;
//    //int horizontalShift = 1;
//
//    for (int row = 0; row < foregroundImageRecord.height; ++row)
//    {
//        for (int col = 0; col < foregroundImageRecord.width; ++col)
//        {
//            for (int channel = 0; channel < foregroundImageRecord.channelCount; ++channel)
//            {
//                int foregroundIndex = ((row * foregroundImageRecord.width) + col) * foregroundImageRecord.channelCount + channel;
//
//                //calc below allows for horizontal and vertical shift: 
//                int backgroundIndex =
//                    ((row + verticalShift) * backgroundImageRecord.width +
//                        (col + horizontalShift)) * backgroundImageRecord.channelCount + channel;
//                //shifts may result in going out of bounds ...
//                assert(backgroundIndex < backgroundImageRecord.imageSize);
//
//
//                backgroundImageRecord.imagePtr[backgroundIndex] = foregroundImageRecord.imagePtr[foregroundIndex];
//
//                //ptrToBackgroundImageData[backgroundIndex] = ptr_to_foreground_image_data[foregroundIndex];
//
//            }
//        }
//    }
//
//}





//to be deleted...
void moveKnightToE4(ImageRecord& theBoard, const ImageRecord& theKnight)
{
    //clear prev. pos (later)


    addForegroundImageToBgrdImage(400, 400, theBoard, theKnight); 

}

int main()
{
    ChessGame theGame{}; 
    //theGame.drawBoard();

    auto imageFileChessBoardMap = mapImageFileCoordinatesToChessPositions(); 

    auto theBoardImage = getImageRecordFromFile("emptyBoard.jpg");

    auto arrayOfImages = readImageFilesInFolder("/pieceImages/");
    
    //1st, read all piece images into array of images - make sure to name files same as names in Pieces array in ChessGame.h

    //2nd, position the images using both 
    // 1) the map of positions to coordinates in ImageRecord.h (imageFileChessBoardMap variable above) and 
    //2) the map of (starting) positions to pieces in ChessGame.h
    

    cin.get(); 


    //auto blackRook = getImageRecordFromFile("blackRook.jpg");
    //load lightSquare to cover up a piece that moves from its previous position: 
    //auto lightSquare = getImageRecordFromFile("lightSquare.jpg");

    int verticalShift = 40; // emptyBoard.height / 8; excessive
    int horizontalShift = 40; // = emptyBoard.width / 8;



    const char* filename = "asdfsad.png";
    //change current directory to the output images: 
    std::filesystem::current_path("../outputImages");

    for (const auto& image : arrayOfImages)
    {
        addForegroundImageToBgrdImage(verticalShift, horizontalShift, theBoardImage, image);

        verticalShift += 90;
        horizontalShift += 90;


        stbi_write_png(
            filename,
            theBoardImage.width, theBoardImage.height, theBoardImage.channelCount,
            theBoardImage.imagePtr,
            theBoardImage.width * theBoardImage.channelCount
        );

        system(filename);


        //std::cin.get();
    };

    //for (auto& image : arrayOfImages)
    //{
    //    if (image.name == "whiteKnight") //will need to specify king's side v queen's side later 
    //    {
    //        moveKnightToE4(theBoardImage, image);
    //         break; 
    //    }

    //    else
    //    {
    //        cout << "there is no white knight!\n";
    //    }
    //}

    stbi_write_png(
        filename,
        theBoardImage.width, theBoardImage.height, theBoardImage.channelCount,
        theBoardImage.imagePtr,
        theBoardImage.width * theBoardImage.channelCount
    );

    system(filename);


    //std::cin.get();

}
