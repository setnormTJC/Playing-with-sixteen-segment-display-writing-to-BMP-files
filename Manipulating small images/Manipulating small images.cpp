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


int main()
{
    //writeSomeImage();

    const unsigned int BACKGROUND_IMAGE_WIDTH = 40; //NOTE: stack overflow will occur if these get too large
    const unsigned int BACKGROUND_IMAGE_HEIGHT = 20;
    const unsigned int BACKGROUND_IMAGE_CHANNELS = 3;

    unsigned char background_image_data[BACKGROUND_IMAGE_WIDTH * BACKGROUND_IMAGE_HEIGHT * BACKGROUND_IMAGE_CHANNELS];/* =
    {
        0, 0, 0,            255, 255, 255,          0, 0, 0,            255, 255, 255,
        255, 255, 255,      0, 0, 0,            255, 255, 255,          0, 0, 0,             
        0, 0, 0,            255, 255, 255,          0, 0, 0,            255, 255, 255    
    };*/

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
    unsigned char* ptrToBackgroundImageData = background_image_data;

    //cout << "here" << (void*)bwData[0] << "\n"; //0000 000FF (255) -> if x86 (16 digits if x64) 
    //cout << "here" << (void*)bwData[3] << "\n"; //FF (255) 

    const int FOREGROUND_IMAGE_WIDTH = 4; 
    const int FORGROUND_IMAGE_HEIGHT = 2; 
    const int FOREGROUND_IMAGE_CHANNELS = 3; 

    unsigned char foreground_image_data[FOREGROUND_IMAGE_WIDTH*FORGROUND_IMAGE_HEIGHT*FOREGROUND_IMAGE_CHANNELS] = 
    {
        255, 0, 0,          0, 255, 0,          0, 0, 255,      128, 128,128, 
        255, 255, 0,        0, 255, 255,        255, 0, 255,    255, 0, 0
    };

    //cout << sizeof(foregroundImage) << "\n";
    unsigned char* ptr_to_foreground_image_data = foreground_image_data;


    assert(sizeof(background_image_data) > sizeof(foreground_image_data));
    int verticalShift = 1; 
    int horizontalShift = 1; 

    for (int row = 0; row < FORGROUND_IMAGE_HEIGHT; ++row)
    {
        for (int col = 0; col < FOREGROUND_IMAGE_WIDTH; ++col)
        {
            for (int channel = 0; channel < FOREGROUND_IMAGE_CHANNELS; ++channel)
            {
                int foregroundIndex = ((row * FOREGROUND_IMAGE_WIDTH) + col) * FOREGROUND_IMAGE_CHANNELS + channel;
                
                //calc below allows for horizontal and vertical shift: 
                int backgroundIndex = 
                             ((row + verticalShift)* BACKGROUND_IMAGE_WIDTH + 
                                    (col+horizontalShift)) * BACKGROUND_IMAGE_CHANNELS + channel; 
                
                assert(backgroundIndex < sizeof(background_image_data));

                ptrToBackgroundImageData[backgroundIndex] = ptr_to_foreground_image_data[foregroundIndex];

            }
        }
    }


    stbi_write_png("modifiedImage.png",
        BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, BACKGROUND_IMAGE_CHANNELS,
        ptrToBackgroundImageData,
        BACKGROUND_IMAGE_WIDTH * BACKGROUND_IMAGE_CHANNELS);

    system("modifiedImage.png"); 

}

