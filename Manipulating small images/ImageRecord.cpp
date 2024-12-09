#include "../Messing with image files/ImageRecord.h"

#include "../Messing with image files/stb_image.h"

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

#pragma region Non-member functions that manipulate ImageRecord objects
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

    return arrayOfImageRecords;
}


vector<unsigned char> makeCompositeImage(ImageRecord& backgroundImageRecord, vector<ImageRecord>& foregroundImageRecords)
{
    //    unsigned int BGRD_IMAGE_SIZE = backgroundImageRecord.channelCount * backgroundImageRecord.height * backgroundImageRecord.width;

    unsigned int BGRD_IMAGE_SIZE = backgroundImageRecord.imageSize;

    vector<unsigned char> compositeImageArray(BGRD_IMAGE_SIZE);

    //first, copy the bgrd image to the composite image: 
    std::copy(backgroundImageRecord.imagePtr,
        backgroundImageRecord.imagePtr + (BGRD_IMAGE_SIZE),
        compositeImageArray.begin());


    //for (unsigned int i = 0; i < BGRD_IMAGE_SIZE / 3; ++i)
    //{
    //    compositeImageData.at(i) = 255; //sets to white 
    //    //compositeImageData.at(i) = 0; //sets to black
    //    //cout << (void*)compositeImageData.at(i) << "\n";
    //}

    ImageRecord firstForegroundImage = foregroundImageRecords.at(0);
    ImageRecord secondForegroundImage = foregroundImageRecords.at(1);

    cout << backgroundImageRecord << "\n";
    cout << firstForegroundImage << "\n";
    cout << secondForegroundImage << "\n";

    //assert foreground image size < bgrd image size? 
    for (int col = 0; col < firstForegroundImage.width; ++col)
    {
        for (int row = 0; row < firstForegroundImage.height; ++row)
        {
            for (int channel = 0; channel < firstForegroundImage.channelCount; ++channel)
            {
                int index = col * row * channel;
                compositeImageArray[index] = firstForegroundImage.imagePtr[index];
            }
        }
        //compositeImageArray[i] = firstForegroundImage.imagePtr[i]; 

    }



    return compositeImageArray;


}

#pragma endregion