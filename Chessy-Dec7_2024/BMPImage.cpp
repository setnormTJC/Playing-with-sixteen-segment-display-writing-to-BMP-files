#include "BMPImage.h"
#include <iostream>
#include <fstream>

#pragma region Color function implementations
Color::Color()
	: r(0), g(0), b(0)
{
}

Color::Color(float _r, float _g, float _b)
	: r(_r), g(_g), b(_b)
{
}

Color::~Color()
{
}

#pragma endregion

#pragma region BMPimage function implementations
BMPImage::BMPImage(int _width, int _height)
	: width(_width), height(_height), colors(std::vector<Color>(width* height))
{
}

BMPImage::~BMPImage()
{
}

Color BMPImage::getColor(int _x, int _y) const
{
	return colors[_y * width + _x];
}

void BMPImage::setColor(const Color& _color, int _x, int _y)
{
	colors[_y * width + _x].r = _color.r;
	colors[_y * width + _x].g = _color.g;
	colors[_y * width + _x].b = _color.b;
}

/*If curious about this func. def., consult:
https://www.youtube.com/watch?v=vqT5j38bWGg
*/
void BMPImage::ExportBMPFile(const char* _path) const
{
	std::ofstream fout;
	fout.open(_path, std::ios::out | std::ios::binary);

	if (!fout.is_open())
	{
		std::cout << "File could not be opened...\n";
		return;
	}

	unsigned char tempPad[3] = { 0, 0, 0 };
	const int padding = ((4 - (width * 3) % 4) % 4);

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = fileHeaderSize + informationHeaderSize + width * height * 3 + padding * height;

	unsigned char fileHeader[fileHeaderSize];

	// file type
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';

	// file size
	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;

	// reserved 1 (not used)
	fileHeader[6] = 0;
	fileHeader[7] = 0;

	// reserved 1 (not used)
	fileHeader[8] = 0;
	fileHeader[9] = 0;

	// pixel data offset
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];

	// header size
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;

	// image width
	informationHeader[4] = width;
	informationHeader[5] = width >> 8;
	informationHeader[6] = width >> 16;
	informationHeader[7] = width >> 24;

	// image height
	informationHeader[8] = height;
	informationHeader[9] = height >> 8;
	informationHeader[10] = height >> 16;
	informationHeader[11] = height >> 24;

	// planes
	informationHeader[12] = 1;
	informationHeader[13] = 0;

	// bits per pixel 
	informationHeader[14] = 24;
	informationHeader[15] = 0;

	// compression (none)
	informationHeader[16] = 0;
	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;

	// image size (no compression)
	informationHeader[20] = 0;
	informationHeader[21] = 0;
	informationHeader[22] = 0;
	informationHeader[23] = 0;

	// x pixels per meter (not specified)
	informationHeader[24] = 0;
	informationHeader[25] = 0;
	informationHeader[26] = 0;
	informationHeader[27] = 0;

	// y pixels per meter (not specified)
	informationHeader[28] = 0;
	informationHeader[29] = 0;
	informationHeader[30] = 0;
	informationHeader[31] = 0;

	// total colors (color pallete not used)
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;

	// important colors (generally ignored)
	informationHeader[36] = 0;
	informationHeader[37] = 0;
	informationHeader[38] = 0;
	informationHeader[39] = 0;

	fout.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	fout.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char r = static_cast<unsigned char>(getColor(x, y).r * 255.0f);
			unsigned char g = static_cast<unsigned char>(getColor(x, y).g * 255.0f);
			unsigned char b = static_cast<unsigned char>(getColor(x, y).b * 255.0f);

			unsigned char color[] = { b, g, r }; //NOTE the somewhat atypical order

			fout.write(reinterpret_cast<char*>(color), 3);
		}


		fout.write(reinterpret_cast<char*>(tempPad), padding);
	}

	fout.close();

	//std::cout << "File created and closed, it is located in the project repository \n";
}


/*
@param thickness - if 0, then a single pixel is colored, if 1 then pixel and its neigbors are colored,
if 2, then neighbors of neighbors are also colored, etc.
*/
void BMPImage::drawPoint(int x0, int y0, int thickness, const Color& color)
{
	if (thickness < 0)
	{
		std::cout << "negative thickness doesn't make sense!\n";
		return;
	}

	for (int x = x0 - thickness; x <= x0 + thickness; ++x)
	{
		for (int y = y0 - thickness; y <= y0 + thickness; ++y)
		{
			setColor(color, x, y);
		}
	}
}

void BMPImage::drawLine(int x0, int y0, int x1, int y1, const Color& color)
{
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	//handle vertical line (causes infinite slope): 
	if (x0 == x1)
	{
		if (y0 > y1)
		{
			std::swap(y0, y1);
		}

		for (int y = y0; y < y1; ++y)
		{
			setColor(color, x0, y);
		}
	}

	else
	{
		double m = (double)(y1 - y0) / (x1 - x0);

		double b = y0 - m * x0;

		for (int x = x0; x < x1; ++x)
		{
			int y = (int)(m * x + b);
			setColor(color, x, y);
		}
	}

}

/*Calls drawline twice - relies on a map<int, Point> for tic tac toe grid*/
void BMPImage::drawX(int chosenSpot, const Color& color)
{
	int x0 = gridMap[chosenSpot].x;
	int y0 = gridMap[chosenSpot].y;

	int x1 = width / 3 + x0;
	int y1 = height / 3 + y0;

	//auto x0 = gridMap.find(chosenSpot); 

	drawLine(x0, y0, x1, y1, color);
	drawLine(x0, x1, y1, y0, color);
}

void BMPImage::drawCircle(int radius, int xCenter, int yCenter, const Color& color)
{
	//equation of a circle: x^2 + y^2 = r^2 
	for (int x = -radius; x <= radius; ++x)
	{
		int y = (int)std::sqrt(radius * radius - x * x);
		setColor(color, xCenter + x, y); //one octant of circle? 
	}
}

#pragma endregion

#pragma region SixteenSegmentDisplay function implementations

SixteenSegmentDisplay::SixteenSegmentDisplay(const ImageFileCoordinate& bottomLeft, const ImageFileCoordinate& topRight)
	:bottomLeft(bottomLeft), topRight(topRight)
{
	a1 = Segment
	{
		{x0, yf},
		{xMidpoint, yf}
	};

	a2 = Segment
	{
		{xMidpoint, yf},
		{xf, yf}
	};

	b = Segment
	{
		{xf, yf},
		{xf, yMidpoint}
	};

	c = Segment
	{
		{xf, yMidpoint},
		{xf, y0}
	};

	d2 = Segment
	{
		{xf, y0},
		{xMidpoint, y0}
	};

	d1 = Segment
	{
		{xMidpoint, y0},
		{x0, y0}
	};

	e = Segment
	{
		{x0, y0},
		{x0, yMidpoint}
	};

	f = Segment
	{
		{x0, yMidpoint},
		{x0, yf}
	};

	g1 = Segment
	{
		{x0, yMidpoint},
		{xMidpoint, yMidpoint}
	};

	g2 = Segment
	{
		{xMidpoint, yMidpoint},
		{xf, yMidpoint}
	};

	h = Segment
	{
		{x0, yf},
		{xMidpoint, yMidpoint}
	};

	i = Segment
	{
		{xMidpoint, yf},
		{xMidpoint, yMidpoint}
	};

	j = Segment
	{
		{xf, yf},
		{xMidpoint, yMidpoint}
	};

	k = Segment
	{
		{x0, y0},
		{xMidpoint, yMidpoint}
	};

	l = Segment
	{
		{xMidpoint, yMidpoint },
		{xMidpoint, y0}
	};

	m = Segment
	{
		{xMidpoint, yMidpoint},
		{xf, y0}
	};

	theSixteenSegments = { a1, a2, b, c, d2, d1, e, f,
							g1, g2, h, i, j, k, l, m };
}


std::vector<Segment>  SixteenSegmentDisplay::getAll16Segments() const
{
	return theSixteenSegments;
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber0() const
{
	return vector<Segment>{e, f, a1, a2, b, c, d1, d2, k, j};
}

std::vector<Segment>  SixteenSegmentDisplay::getSegmentsForNumber1() const
{
	//return theSixteenSegments; 
	return vector<Segment>{b, c};
};

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber2() const
{
	return std::vector<Segment>{ a1, a2, b, g1, g2, e, d1, d2 };
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber3() const
{
	return std::vector<Segment>{ a1, a2, b, g1, g2, c, d1, d2 };
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber4() const
{
	return std::vector<Segment>{ f, g1, g2, b, c };
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber5() const
{
	return std::vector<Segment>{ a1, a2, f, g1, g2, c, d1, d2 };
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber6() const
{
	return std::vector<Segment>{ a1, a2, f, g1, g2, e, c, d1, d2 };
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber7() const
{
	return std::vector<Segment>{ a1, a2, b, c };
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber8() const
{
	return std::vector<Segment>{ a1, a2, b, c, d1, d2, e, f, g1, g2 };
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForNumber9() const
{
	return std::vector<Segment>{ a1, a2, b, c, d1, d2, f, g1, g2 };
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForLetterA() const
{
	return vector<Segment>{f, e, a1, a2, b, c, g1, g2};
}

/*lowercase b here - capital B is ambiguous with 8*/
std::vector<Segment> SixteenSegmentDisplay::getSegmentsForLetterB() const
{
	return vector<Segment>{f, e, d1, d2, c, g2, g1};
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForLetterC() const
{
	return vector<Segment>{a1, a2, d2, d1, f, e};
	//note the "mixed up" order (okay since line segments are drawn "separately")
}

std::vector<Segment> SixteenSegmentDisplay::getSegmentsForSpaceCharacter() const
{
	return vector<Segment>{d1, d2};
}
#pragma endregion