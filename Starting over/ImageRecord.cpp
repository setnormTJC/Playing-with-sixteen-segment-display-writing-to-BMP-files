#include "ImageRecord.h"

#include "stb_image.h"

#include<iostream> 
#include<iomanip> 


using std::setw;


#pragma region member function implementations of ImageRecord struct 
ImageRecord::ImageRecord()
{

}

ImageRecord::ImageRecord(string name, int width, int height, int channelCount, unsigned char* imagePtr)
    :name(name), width(width), height(height), channelCount(channelCount), imagePtr(imagePtr)
{
    imageSize = width * height * channelCount; //added on later 
}

ImageRecord::~ImageRecord()
{
    //if (imagePtr != nullptr)
    //{
    //    cout << "Destructor called\n";
    //    stbi_image_free(imagePtr);
    //}
}

#pragma endregion 

#pragma region Non-member functions that manipulate (or create) ImageRecord objects
/*Non-member functions*/
std::ostream& operator<<(std::ostream& os, const ImageRecord& imageRecord)
{
    os /*<< (void*)imageRecord.imagePtr*/ //cast to void* because cout object interprets char* as C-style string 
        << std::left << std::setw(20) << imageRecord.name
        << std::left << std::setw(5) << imageRecord.width
        << "x"
        << std::left << std::setw(5) << imageRecord.height
        << "x"
        << std::left << std::setw(5) << imageRecord.channelCount
        << std::left << std::setw(10) << imageRecord.imageSize << "\n";
    return os;
}

ImageRecord getImageRecordFromFile(const string& filepath)
{
    int x, y, channelCount;
    unsigned char* ptrToImageFile;

    ptrToImageFile = stbi_load(filepath.c_str(), &x, &y, &channelCount, 0);

    if (ptrToImageFile == nullptr) {
        cout << filepath << " image not found\n";
        std::cin.get();
        return ImageRecord{}; //
    }

    /*Next several lines are just to get only the filename (not the entire filepath)*/
    int indexOfLastFolderSeparator;
    if (filepath.find('/') != string::npos) {
        indexOfLastFolderSeparator = filepath.rfind('/'); //Note the r(everse)find
    }
    else {
        indexOfLastFolderSeparator = filepath.rfind('\\'); //Note the r(everse)find
    }
    string filename = filepath.substr(indexOfLastFolderSeparator + 1/*, filepath.length() - indexOfFileExtensionSeparator*/);
    filename = filename.substr(0, filename.find('.'));


    /*Now fill an image record and return it:*/
    ImageRecord imageRecord{ filename, x, y, channelCount, ptrToImageFile };
    return imageRecord;
}


/*Note: this will change the current directory*/
vector<ImageRecord> readImageFilesInFolder(const std::string& baseFolderName)
{
    using namespace std::filesystem;

    string startingPath = current_path().string(); //gets

    string newPath = startingPath + baseFolderName;

    //cout << currentPath<< "\n";
    //string newPath = "C:/Users/snorm/source/repos/Chessy-Dec7_2024/Messing with image files/chessImageResources";

    std::filesystem::current_path(newPath); //sets 

    vector<ImageRecord> arrayOfImageRecords;
    for (auto& entry : directory_iterator(current_path()))
    {
        //cout << entry.path() << "\n";
        //string pathAsString = (entry.path().generic_string()); 

        ImageRecord imageRecord;
        imageRecord = getImageRecordFromFile(entry.path().string());// = getImageRecord(entry.path().string());

        //insert into arrayOfImageRecords

        arrayOfImageRecords.push_back(imageRecord);
    }

    std::filesystem::current_path(startingPath); //sets back to starting path 
    return arrayOfImageRecords;
}

/*
* @param backGroundImageRecord - will be MODIFIED by this method!
*/
void addForegroundImageToBgrdImage(int horizontalShift, int verticalShift,
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


map<string, ImageFileCoordinate> mapImageFileCoordinatesToChessPositions()
{

    int currentY = 40; 
    int currentX = 40; 

    map<string, ImageFileCoordinate> theMap; 
    
    for (int row = 8; row > 0; --row)
    {
        for (char col = 'A'; col < 'I'; ++col)
        {
            ImageFileCoordinate currentCoordinate{ currentX, currentY };
            //ImageChessPosition currentPosition{ row, col };
            
            string currentPosition{}; 
            currentPosition += col; 
            currentPosition += std::to_string(row); 

            theMap.insert({ currentPosition, currentCoordinate });

            currentX += 90; 
        }
        //RESET x coordinate to correspond to A file (column A) 
        currentX = 40;
        //INCREMENT y coordinate to next rank (row) 
        currentY += 90; 
    }


    return theMap;
}


#pragma endregion


