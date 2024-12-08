// Messing with image files.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

#include <iostream>

#include<vector> 

#include<string> 

#include<thread> 

#include<chrono> 

using std::cout; 
using std::vector; 
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


int main()
{
    int emptyBoardImageWidth, emptyBoardImageHeight, emptyBoardChannelCount; 
    int blackPawnImageWidth, blackPawnImageHeight, blackPawnChannelCount;
    int whiteKnightImageWidth, whiteKnightImageHeight, whiteKnightChannelCount; 

    unsigned char* emptyBoardImagePointer = stbi_load("chessImageResources/emptyChessBoard.jpg", &emptyBoardImageWidth, 
                                                &emptyBoardImageHeight, &emptyBoardChannelCount, 0);

    auto blackPawnImagePointer = stbi_load("chessImageResources/blackPawn.jpg", &blackPawnImageWidth, &blackPawnImageHeight,
        &blackPawnChannelCount, 0);

    auto whiteKnightImagePointer = stbi_load("chessImageResources/whiteKnight.jpg", &whiteKnightImageWidth, &whiteKnightImageHeight,
        &whiteKnightChannelCount, 0);

    if (emptyBoardImagePointer == nullptr 
        ||
        blackPawnImagePointer == nullptr
        ||
        whiteKnightImagePointer == nullptr)
    {
        std::cout << "Couldn't file one of the files\n";
        return 1;
    }
  

    //begin making new (composite) image: 
    vector<unsigned char> newImageArray(emptyBoardImageWidth * emptyBoardImageHeight * emptyBoardChannelCount);

    //write first (background) image into newImage 
    std::copy(emptyBoardImagePointer,
        emptyBoardImagePointer + (emptyBoardChannelCount * emptyBoardImageHeight * emptyBoardImageWidth),
        newImageArray.begin());

    //scale the black pawn image (hardcoding scaling for now): 
    const unsigned int SCALING_FACTOR = 3;

    auto scaledBlackPawnImagePointer = scaleImage(blackPawnImagePointer, blackPawnImageWidth, blackPawnImageHeight, blackPawnChannelCount,
        blackPawnImageWidth / SCALING_FACTOR, blackPawnImageHeight / SCALING_FACTOR);


    //write second (foreground) image:
    movePawnAbout(emptyBoardImagePointer, emptyBoardChannelCount, emptyBoardImageHeight, emptyBoardImageWidth,
        newImageArray, scaledBlackPawnImagePointer, blackPawnImageWidth, SCALING_FACTOR, blackPawnImageHeight, 
        blackPawnChannelCount);


    //write third image (scaling later): 
    moveKnightAbout(emptyBoardImagePointer, emptyBoardChannelCount, emptyBoardImageHeight, emptyBoardImageWidth,
        newImageArray, whiteKnightImagePointer, whiteKnightImageWidth, whiteKnightImageHeight, whiteKnightChannelCount);




    stbi_image_free(emptyBoardImagePointer);
    stbi_image_free(blackPawnImagePointer);
    stbi_image_free(whiteKnightImagePointer);

    //cout << x << "x" << y << " with channel count = " << channels << "\n";
    


    //std::cout << "Hello World!\n";
}
