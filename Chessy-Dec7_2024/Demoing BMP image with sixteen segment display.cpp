// Chessy-Dec7_2024.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include"BMPImage.h"

//#include"C:/Users/Work/Desktop/myCppLibrary/utils/writing image files/writing bmp format/BMPImage.cpp"
//#include"bmpDrawingFunctions.h"

void drawSegments(const vector<Segment>& all16Segments, BMPImage& theImage)
{
	for (int i = 0; i < all16Segments.size(); ++i)
	{
		theImage.drawLine(
			all16Segments[i].startPoint.x,
			all16Segments[i].startPoint.y,
			all16Segments[i].finishPoint.x,
			all16Segments[i].finishPoint.y,
			Color
			{ 
				1.0f * (i % 2), //i mod 2 for color alternation (makes segments more obvious) 
				1.0f * (i % 2),
				1.0f}
		);
	}

	//for (const auto& currentSegment : all16Segments)
	//{
	//	if (counterForAlternatingColors % 2 == 0)
	//	{
	//		theImage.drawLine
	//		(
	//			currentSegment.startPoint.x,
	//			currentSegment.startPoint.y,
	//			currentSegment.finishPoint.x,
	//			currentSegment.finishPoint.y,
	//			Color{ 0.0f, 0.0f, 1.0f } //blue
	//		);
	//	}

	//	else
	//	{
	//		theImage.drawLine
	//		(
	//			currentSegment.startPoint.x,
	//			currentSegment.startPoint.y,
	//			currentSegment.finishPoint.x,
	//			currentSegment.finishPoint.y,
	//			Color{ 1.0f, 0.0f, 0.0f } //red
	//		);
	//	}

	//	counterForAlternatingColors++;

	//}

}



auto makeASixteenSegmentDisplay(const ImageFileCoordinate& bottomLeft, const ImageFileCoordinate& topRight, char chosenChar)
{
	SixteenSegmentDisplay sixteenSegmentDisplay{ bottomLeft, topRight };


	//std::cout << "Which character do you want (0 - 9, A - Z)? \n";
	//char chosenChar; 
	//std::cin >> chosenChar; 

	vector<Segment> segments;

	switch (chosenChar)
	{
	case 'A':
		segments = sixteenSegmentDisplay.getSegmentsForLetterA(); 
		break; 

	case 'B':
		segments = sixteenSegmentDisplay.getSegmentsForLetterB();
		break;

	case 'C':
		segments = sixteenSegmentDisplay.getSegmentsForLetterC();
		break;
	
	case ' ':
		segments = sixteenSegmentDisplay.getSegmentsForSpaceCharacter(); 
		break; 

	case '0':
		segments = sixteenSegmentDisplay.getSegmentsForNumber0(); 
		break; 

	case '1':
		segments = sixteenSegmentDisplay.getSegmentsForNumber1(); 
		break; 

	    case '2':
        segments = sixteenSegmentDisplay.getSegmentsForNumber2(); 
        break; 

    case '3':
        segments = sixteenSegmentDisplay.getSegmentsForNumber3(); 
        break; 

    case '4':
        segments = sixteenSegmentDisplay.getSegmentsForNumber4(); 
        break; 

    case '5':
        segments = sixteenSegmentDisplay.getSegmentsForNumber5(); 
        break; 

    case '6':
        segments = sixteenSegmentDisplay.getSegmentsForNumber6(); 
        break; 

    case '7':
        segments = sixteenSegmentDisplay.getSegmentsForNumber7(); 
        break; 

    case '8':
        segments = sixteenSegmentDisplay.getSegmentsForNumber8(); 
        break; 

    case '9':
        segments = sixteenSegmentDisplay.getSegmentsForNumber9(); 
        break; 
	default: 
		std::cout << "Unknown character entered - defaulting to full sixteen segments\n";
		segments = sixteenSegmentDisplay.getAll16Segments();
		break; 
	}
	return segments;

}

int main()
{
	const int IMAGE_WIDTH = 512;
	const int IMAGE_HEIGHT = 512;

	BMPImage theImage{ IMAGE_WIDTH, IMAGE_HEIGHT };

	char continueResponse = 'y'; 

	const int FONT_HEIGHT = IMAGE_HEIGHT/10;
	const int FONT_WIDTH = FONT_HEIGHT/2;
	const int PADDING = 10; 

	int currentXPosition = 0 + PADDING; 
	int currentYPosition = IMAGE_HEIGHT - FONT_HEIGHT - PADDING; 

	int counter = 0; 

	vector<char> characterSet;

	for (char currentChar = '0'; currentChar < 'Z'; ++currentChar)
	{
		characterSet.push_back(currentChar);
	}


	while(counter < characterSet.size())
	//while (continueResponse != 'n')
	{
		//if (currentXPosition > IMAGE_WIDTH || currentYPosition < IMAGE_HEIGHT)
		//{
		//	std::cout << "Went out of image bounds with x = "
		//		<< currentXPosition << " and y = " << currentYPosition <<"\n";
		//	break; 
		//}

		auto segments = makeASixteenSegmentDisplay
		(
			ImageFileCoordinate{ currentXPosition, currentYPosition },
			ImageFileCoordinate{ currentXPosition + FONT_WIDTH, currentYPosition + FONT_HEIGHT },
			characterSet.at(counter)
		);

		drawSegments(segments, theImage); 

		//move one FONT_WIDTH to the right: 
		currentXPosition += FONT_WIDTH + PADDING; 
		
		//move to next "line" once currentXPosition gets large enough: 
		if (currentXPosition > IMAGE_WIDTH)
		{
			currentXPosition = 0;
			currentYPosition -= (FONT_HEIGHT + PADDING); //drop down
		}

		if (currentYPosition < 0)
		{
			std::cout << "Hit bottom of image (out of bounds):\n";
			break; 
		}

		//std::cout << "Continue (y/n)? \n";
		//std::cin >> continueResponse;

		counter++; 

	} //end while continueResponse != 'n'
	

	theImage.drawPoint(10, 10, 3, Color{ 1.0f, 1.0f, 1.0f }); //white pixel and 3 layers of surrounding neighbors at 10, 10
	theImage.drawPoint(100, 100, 4, Color{ 0.0f, 1.0f, 0.0f }); //4 thick green pixel at 100, 100 

	const char* filename = "sixteenSegmentDisplay.bmp";

	std::cout << "Writing image to file " << filename << "\n";
	
	theImage.ExportBMPFile(filename); 

	system(filename);
}

