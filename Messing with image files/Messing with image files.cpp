// Messing with image files.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include"ImageRecord.h"

#include<vector> 

#include<string> 

#include<thread> 

#include<chrono> 



using std::cout; 
using std::vector; 
using std::string; 
using std::to_string; 



void overlayImage(unsigned char* baseImg, int baseWidth, int baseHeight, int baseChannels,
    unsigned char* overlayImg, int overlayWidth, int overlayHeight, int overlayChannels,
    int posX, int posY) {
    for (int y = 0; y < overlayHeight; ++y) {
        for (int x = 0; x < overlayWidth; ++x) {
            for (int c = 0; c < overlayChannels; ++c) {
                int baseIndex = ((y + posY) * baseWidth + (x + posX)) * baseChannels + c;
                int overlayIndex = (y * overlayWidth + x) * overlayChannels + c;
                if (baseIndex < baseWidth * baseHeight * baseChannels) {
                    baseImg[baseIndex] = overlayImg[overlayIndex];
                }
            }
        }
    }
}

unsigned char* scaleImage(unsigned char* img, int width, int height, int channels, int newWidth, int newHeight) {
    unsigned char* scaledImg = new unsigned char[newWidth * newHeight * channels];
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            for (int c = 0; c < channels; ++c) {
                int srcX = x * width / newWidth;
                int srcY = y * height / newHeight;

                int srcIndex = (srcY * width + srcX) * channels + c;
                int dstIndex = (y * newWidth + x) * channels + c;
                scaledImg[dstIndex] = img[srcIndex];
            }
        }
    }
    return scaledImg;
}

void movePawnAbout(unsigned char* emptyBoardImagePointer, 
                    int emptyBoardChannelCount, int emptyBoardImageHeight, int emptyBoardImageWidth,
                    vector<unsigned char>& newImageArray, 
                    unsigned char* scaledBlackPawnImagePointer, 
                    int blackPawnImageWidth, int SCALING_FACTOR, int blackPawnImageHeight, 
                    int blackPawnChannelCount
    )
{
    const int SQUARE_STEP_SIZE = 30;

    for (int i = 0; i < 100; i = i + SQUARE_STEP_SIZE)
    {
        //reset to empty board
        std::copy(emptyBoardImagePointer,
            emptyBoardImagePointer + (emptyBoardChannelCount * emptyBoardImageHeight * emptyBoardImageWidth),
            newImageArray.begin());

        overlayImage(newImageArray.data(), emptyBoardImageWidth, emptyBoardImageHeight, emptyBoardChannelCount,
            scaledBlackPawnImagePointer, blackPawnImageWidth / SCALING_FACTOR, blackPawnImageHeight / SCALING_FACTOR, blackPawnChannelCount,
            20 + i, 20 + i);


        std::string filename = "composite" + std::to_string(i) + ".png";

        stbi_write_png(filename.c_str(), emptyBoardImageWidth, emptyBoardImageHeight, emptyBoardChannelCount,
            newImageArray.data(), emptyBoardImageWidth * emptyBoardChannelCount);

        system(filename.c_str());

        std::this_thread::sleep_for(std::chrono::seconds{ 1 });


    }
}

void moveKnightAbout(unsigned char* emptyBoardImagePointer,
                    int emptyBoardChannelCount, int emptyBoardImageHeight, int emptyBoardImageWidth,
                    vector<unsigned char>& newImageArray,
                    unsigned char* scaledBlackPawnImagePointer,
                    int blackPawnImageWidth, int blackPawnImageHeight,
                    int blackPawnChannelCount)
{
    const int SQUARE_STEP_SIZE = 30;

    for (int i = 0; i < 500; i = i + SQUARE_STEP_SIZE)
    {
        //reset to empty board
        std::copy(emptyBoardImagePointer,
            emptyBoardImagePointer + (emptyBoardChannelCount * emptyBoardImageHeight * emptyBoardImageWidth),
            newImageArray.begin());

        overlayImage(newImageArray.data(), emptyBoardImageWidth, emptyBoardImageHeight, emptyBoardChannelCount,
            scaledBlackPawnImagePointer, blackPawnImageWidth, blackPawnImageHeight, blackPawnChannelCount,
            20 + i, 20 + i/2); //div by 2 to simulate L pattern walk of knight 


        std::string filename = "compositeWithKnight" + std::to_string(i) + ".png";

        stbi_write_png(filename.c_str(), emptyBoardImageWidth, emptyBoardImageHeight, emptyBoardChannelCount,
            newImageArray.data(), emptyBoardImageWidth * emptyBoardChannelCount);

        system(filename.c_str());

        std::this_thread::sleep_for(std::chrono::seconds{ 1 });

    }
}



void freeImages(ImageRecord& theBoardImageRecord, vector<ImageRecord>& thePiecesImageRecords)
{
    stbi_image_free(theBoardImageRecord.imagePtr);
    for (const auto& currentImageRecord : thePiecesImageRecords)
    {
        stbi_image_free(currentImageRecord.imagePtr);
    }
}

void writeCompositeImage(const char* compositeFilePath, const ImageRecord& backgroundImageRecord, 
    const vector<unsigned char>& compositeImageArray)
{
    stbi_write_png(compositeFilePath,
        backgroundImageRecord.width, backgroundImageRecord.height, backgroundImageRecord.channelCount,
        compositeImageArray.data(),
        (backgroundImageRecord.width * backgroundImageRecord.channelCount));

    //const char* bmpFileName = "composite.bmp"; 

    //stbi_write_bmp(bmpFileName,
    //    backgroundImageRecord.width, backgroundImageRecord.height, backgroundImageRecord.channelCount,
    //    compositeImageArray.data());
    //no obvious diff between result for bmp and png here 
}

int main()
{
    string filePathOfTheBoard = "C:/Users/snorm/source/repos/Chessy-Dec7_2024/Messing with image files/chessImageResources/emptyChessBoard.jpg";

    auto theBoardImageRecord = getImageRecordFromFile(filePathOfTheBoard);

    auto thePiecesImageRecords = readImageFilesInFolder("/chessImageResources/chessPieceImages");
    
    
    auto compositeImageArray = makeCompositeImage(theBoardImageRecord, thePiecesImageRecords); 

    const char* compositeFilePath = "composite.png";
    writeCompositeImage(compositeFilePath, theBoardImageRecord, compositeImageArray);


    system(compositeFilePath); 


    freeImages(theBoardImageRecord, thePiecesImageRecords); 
    //free willies:


}
