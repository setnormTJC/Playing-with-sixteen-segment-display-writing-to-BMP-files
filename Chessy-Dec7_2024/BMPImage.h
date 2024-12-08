#pragma once

#include <vector>
#include <map>

using std::vector;

struct Point
{
	int x, y;

	Point()
	{
		x = 0;
		y = 0;
	}

	Point(int x, int y)
		:x(x), y(y)
	{

	}
};


struct Color
{
	float r;
	float g;
	float b;

	Color();
	Color(float _r, float _g, float _b);
	~Color(); //Destructor!

};

struct Segment
{
	Point startPoint{ 0,0 };
	Point finishPoint{ 0,0 };

	Segment()
	{

	}

	Segment(Point startPoint, Point finishPoint)
		:startPoint(startPoint), finishPoint(finishPoint)
	{

	}
};

/*see: https://en.wikipedia.org/wiki/Sixteen-segment_display
* for map of segments to their positions in alphanumeric display
*/
class  SixteenSegmentDisplay
{
public:
	Point bottomLeft, topRight;


	SixteenSegmentDisplay(const Point& bottomLeft, const Point& topRight);

private: //making private to avoid overwhelming amount of visible members
	int x0 = bottomLeft.x;
	int y0 = bottomLeft.y;

	int xf = topRight.x;
	int yf = topRight.y;

	int xMidpoint = (xf + x0) / 2;
	int yMidpoint = (yf + y0) / 2;

	Segment a1, a2, b, c, d1, d2, e, f;

	Segment g1, g2, h, i, j, k, l, m;

	std::vector<Segment> theSixteenSegments;


public:
	/*requires segments 'b' and 'c' as shown in
	https://en.wikipedia.org/wiki/Sixteen-segment_display
	Note that 1 is "right-aligned"
	*/


	std::vector<Segment>  getAll16Segments() const;

	std::vector<Segment> getSegmentsForLetterA() const;
	std::vector<Segment> getSegmentsForLetterB() const;
	std::vector<Segment> getSegmentsForLetterC() const;



	std::vector<Segment> getSegmentsForNumber0() const;
	std::vector<Segment> getSegmentsForNumber1() const;
	std::vector<Segment> getSegmentsForNumber2() const;
	std::vector<Segment> getSegmentsForNumber3() const;
	std::vector<Segment> getSegmentsForNumber4() const;
	std::vector<Segment> getSegmentsForNumber5() const;
	std::vector<Segment> getSegmentsForNumber6() const;
	std::vector<Segment> getSegmentsForNumber7() const;
	std::vector<Segment> getSegmentsForNumber8() const;
	std::vector<Segment> getSegmentsForNumber9() const;


	std::vector<Segment> getSegmentsForSpaceCharacter() const;
};

class BMPImage
{
public:
	BMPImage(int _width, int _height);
	~BMPImage();

	Color getColor(int _x, int _y) const;
	void setColor(const Color& _color, int _x, int _y);

	void ExportBMPFile(const char* _path) const; //when a function header ends with semicolon
	//that is a function DECLARATION (not a defintion) 


	void setBackgroundColor(const Color& color);

	void drawLine(int x0, int y0, int x1, int y1, const Color& color);

	/*Calls drawLine twice*/
	void drawX(int chosenSpot, const Color& color);

	void drawCircle(int radius, int xCenter, int yCenter, const Color& color);

	void drawPoint(int x, int y, int thickness, const Color& color);

	

private:
	int width = 512;
	int height = 512;
	std::vector<Color> colors; //use of "composition" 
	//BMP image is composed of (at least one) Color 

	/*
	assumes grid numbering as follows:
	===========
	|1 | 2 | 3|
	|4 | 5 | 6|
	|7 | 8 | 9|
	===========
	*/
	std::map<int, Point> gridMap =
	{
		{1, {0, width / 3} },
		{2, {height / 3, width / 3} },
		{3, {2 * height / 3, width / 3}}
		//insert the remaining 6 ...
	};

};
