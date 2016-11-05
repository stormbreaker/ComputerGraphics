#include "planet.h"


using namespace std;


Planet::Planet(string name, double radius, string imagePath)
{
    Color tempColor;
    tempColor.red = 0;
    tempColor.green = 0;
    tempColor.blue = 0;
    _Radius = radius;
    _Name = name;
    _TextureImagePath = imagePath;
    _PlanetColor = tempColor;
}

Planet::Planet()
{
    
}

void Planet::setRadius(double radius)
{
    _Radius = radius;
}

void Planet::setName(string name)
{
    _Name = name;
}

void Planet::setTextureImagePath(string filepath)
{
    _TextureImagePath = filepath;
}

void Planet::setPlanetColor(Color planetColor)
{
    _PlanetColor = planetColor;
}
void Planet::setDistance(double distance)
{
    _Distance = distance;
}
void Planet::setYear(double year)
{
    _Year = year;
}
void Planet::setDay(double day)
{
    _Day = day;
}

double Planet::getRadius()
{
    return _Radius;
}

string Planet::getName()
{
    return _Name;
}

string Planet::getTextureImagePath()
{
    return _TextureImagePath;
}

Color Planet::getPlanetColor()
{
    return _PlanetColor;
}
double Planet::getDistance()
{
    return _Distance;
}
double Planet::getYear()
{
    return _Year;
}
double Planet::getDay()
{
    return _Day;
}