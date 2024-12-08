#pragma once


#include<iostream> 
#include<string> 
#include<vector> 
#include<filesystem>


using std::vector; 
using std::string; 
using std::cout; 

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

vector<unsigned char> makeCompositeImage(ImageRecord& backgroundImageRecord, vector<ImageRecord>& foregroundImageRecords);
