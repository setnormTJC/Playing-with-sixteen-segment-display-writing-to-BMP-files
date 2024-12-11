#pragma once


#include<iostream> 
#include<string> 
#include<vector> 
#include<filesystem>

#include<cassert> 

#include<map> 



using std::vector;
using std::string;
using std::cout;
using std::map; 

struct ImageRecord
{
    std::string name{};

    int width{}, height{}, channelCount{};
    int imageSize{};// = width * height * channelCount;

    unsigned char* imagePtr{}; //initializes to null pointer  
    //NOTE -> cast to (void*) if printing the pointer 

    ImageRecord();
    ImageRecord(std::string name, int width, int height, int channelCount, unsigned char* imagePtr);


    friend std::ostream& operator << (std::ostream& os, const ImageRecord& imageRecord);


    ~ImageRecord();
};

ImageRecord getImageRecordFromFile(const string& filepath);


vector<ImageRecord> readImageFilesInFolder(const std::string& baseFolderName);


void addForegroundImageToBgrdImage(int verticalShift, int horizontalShift,
    ImageRecord& backgroundImageRecord,
    const ImageRecord& foregroundImageRecord);

/*x and y -> {0, 0} is TOP LEFT */
struct ImageFileCoordinate
{
    int x = 0, y = 0;

    ImageFileCoordinate() {}; 
    ImageFileCoordinate(int x, int y)
        :x(x), y(y)
    {

    };
};

/*VERY specific method - only works for the current (Dec 9, 2024) chessboard image I am using*/
map<string, ImageFileCoordinate> mapImageFileCoordinatesToChessPositions();