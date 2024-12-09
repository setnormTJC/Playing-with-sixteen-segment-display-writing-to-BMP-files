// Manipulating small images.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS //stb image uses something like s_printf

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"../Messing with image files/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include"../Messing with image files/stb_image.h"

#include"../Messing with image files/ImageRecord.h"
#include <iostream>

#include<cassert>


void writeSomeImage()
{
    const int IMAGE_WIDTH = 100;
    const int IMAGE_HEIGHT = 1000;
    const int CHANNEL_COUNT = 3;

    unsigned char imageData[IMAGE_WIDTH * IMAGE_HEIGHT * CHANNEL_COUNT];

    int counter = 0;

    for (int col = 0; col < IMAGE_WIDTH; ++col)
    {
        for (int row = 0; row < IMAGE_HEIGHT; ++row)
        {
            for (int channel = 1; channel < CHANNEL_COUNT + 1; ++channel) //note the 1 here (avoid mult by 0)
            {
                int index = col * row * channel;
                if (counter % 2 == 0) {
                    imageData[index] = 0;
                }

                else {
                    imageData[index] = 255;
                }
            }
            counter++;
        }
    }

    unsigned char* ptrToImageData = imageData;

    stbi_write_png("image.png", IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_COUNT, ptrToImageData, IMAGE_WIDTH * CHANNEL_COUNT);

}

//struct MyColor
//{
//    int 
//};

ImageRecord makeBgrdImage()
{
    const unsigned int BACKGROUND_IMAGE_WIDTH = 512; //NOTE: stack overflow will occur if these get too large
    const unsigned int BACKGROUND_IMAGE_HEIGHT = 512;
    const unsigned int BACKGROUND_IMAGE_CHANNELS = 3;

    unsigned char background_image_data[BACKGROUND_IMAGE_WIDTH * BACKGROUND_IMAGE_HEIGHT * BACKGROUND_IMAGE_CHANNELS];

    unsigned char* ptrToBackgroundImageData = background_image_data;
    //uncomment below if reaching large enough image for stack overflow: 
   //     new unsigned char [BACKGROUND_IMAGE_WIDTH * BACKGROUND_IMAGE_HEIGHT * BACKGROUND_IMAGE_CHANNELS];

    //example below of 4 x 3 black and white alterating 
    //unsigned char background_image_data[BACKGROUND_IMAGE_WIDTH * BACKGROUND_IMAGE_HEIGHT * BACKGROUND_IMAGE_CHANNELS];/* =
    //{
    //    0, 0, 0,            255, 255, 255,          0, 0, 0,            255, 255, 255,
    //    255, 255, 255,      0, 0, 0,            255, 255, 255,          0, 0, 0,             
    //    0, 0, 0,            255, 255, 255,          0, 0, 0,            255, 255, 255    
    //};*/

    for (int row = 0; row < BACKGROUND_IMAGE_HEIGHT; ++row)
    {
        for (int col = 0; col < BACKGROUND_IMAGE_WIDTH; ++col)
        {
            for (int channel = 0; channel < BACKGROUND_IMAGE_CHANNELS; ++channel)
            {
                //int index = row * col * channel; 
                int index = ((row * BACKGROUND_IMAGE_WIDTH) + col) * BACKGROUND_IMAGE_CHANNELS + channel;
                //cout << index << "\n";

                if ((row + col) % 2 == 0)
                {
                    background_image_data[index] = 0;
                }

                else
                {
                    background_image_data[index] = 255;
                }
            }
        }
    }

    ImageRecord bgrdImageRecord{ "background image",
        BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, BACKGROUND_IMAGE_CHANNELS,
        background_image_data};
    

    return bgrdImageRecord;
}

ImageRecord makeFirstForegroundImage()
{
    const int FOREGROUND_IMAGE_WIDTH = 4;
    const int FOREGROUND_IMAGE_HEIGHT = 2;
    const int FOREGROUND_IMAGE_CHANNELS = 3;


    unsigned char foreground_image_data[FOREGROUND_IMAGE_WIDTH * FOREGROUND_IMAGE_HEIGHT * FOREGROUND_IMAGE_CHANNELS] =
    {
        255, 0, 0,          0, 255, 0,          0, 0, 255,      128, 128,128,
        255, 255, 0,        0, 255, 255,        255, 0, 255,    255, 0, 0
    };

    //cout << sizeof(foregroundImage) << "\n";
    unsigned char* ptr_to_foreground_image_data = foreground_image_data;


    ImageRecord foregroundImageRecord{ "foreground image",
        FOREGROUND_IMAGE_WIDTH, FOREGROUND_IMAGE_WIDTH, FOREGROUND_IMAGE_CHANNELS,
        ptr_to_foreground_image_data };

    return foregroundImageRecord;

}

ImageRecord makeSmileyForegroundImage()
{

    unsigned char smileyData[4 * 5 * 3] =
    {
        255, 255, 0,        255, 255, 0,        255, 255, 0,        255, 255, 0, //yellow forehead
        255, 255, 0,        255, 0, 255,        255, 0, 255,        255, 255, 0, //purple (magenta) eyes (and lips on last row)
        255, 255, 0,        255, 255, 0,        255, 255, 0,        255, 255, 0,
        255, 0, 255,        255, 255, 0,        255, 255, 0,        255,  0, 255,
        255, 0, 255,        255, 0, 255,        255, 0, 255,        255, 0, 255,
    };

    unsigned char* ptrToSmileyData = smileyData; 

    ImageRecord smilyForegroundImageRecord{ "smiley foreground image",
        4, 5, 3, ptrToSmileyData };

    return smilyForegroundImageRecord;
}



void addForegroundImageToBgrdImage(int verticalShift, int horizontalShift, 
    ImageRecord& backgroundImageRecord, 
    const ImageRecord& foregroundImageRecord)
{


    assert(backgroundImageRecord.imageSize > foregroundImageRecord.imageSize);
    //int verticalShift = 1;
    //int horizontalShift = 1;

    for (int row = 0; row < foregroundImageRecord.height; ++row)
    {
        for (int col = 0; col < foregroundImageRecord.width; ++col)
        {
            for (int channel = 0; channel < foregroundImageRecord.channelCount; ++channel)
            {
                int foregroundIndex = ((row * foregroundImageRecord.width) + col) * foregroundImageRecord.channelCount + channel;

                //calc below allows for horizontal and vertical shift: 
                int backgroundIndex =
                    ((row + verticalShift) * backgroundImageRecord.width +
                        (col + horizontalShift)) * backgroundImageRecord.channelCount + channel;
                //shifts may result in going out of bounds ...
                assert(backgroundIndex < backgroundImageRecord.imageSize);
                

                backgroundImageRecord.imagePtr[backgroundIndex] = foregroundImageRecord.imagePtr[foregroundIndex];

                //ptrToBackgroundImageData[backgroundIndex] = ptr_to_foreground_image_data[foregroundIndex];

            }
        }
    }

}

int main()
{
    //writeSomeImage();

    ImageRecord backgroundImageRecord = makeBgrdImage(); 

    ImageRecord forgroundImageRecord = makeFirstForegroundImage(); 

    //shift 1 pixel down and 1 pixel right: 
    addForegroundImageToBgrdImage(1, 1, backgroundImageRecord, forgroundImageRecord);

    ImageRecord smileyForegroundImageRecord = makeSmileyForegroundImage(); 
    
    ImageRecord chessboardImageRecord = getImageRecordFromFile("../Messing with image files/chessImageResources/emptyChessBoard.jpg");


    addForegroundImageToBgrdImage(100, 100, backgroundImageRecord, smileyForegroundImageRecord); 

    addForegroundImageToBgrdImage(400, 400, backgroundImageRecord, smileyForegroundImageRecord);

    addForegroundImageToBgrdImage(100, 100, backgroundImageRecord, chessboardImageRecord);

    stbi_write_png("modifiedImage.png",
        backgroundImageRecord.width, backgroundImageRecord.height, backgroundImageRecord.channelCount,
        backgroundImageRecord.imagePtr,
        backgroundImageRecord.width * backgroundImageRecord.channelCount);
    
    //delete[] doubledSmiley.imagePtr;

    system("modifiedImage.png"); 


    //delete [] ptrToBackgroundImageData;
}

