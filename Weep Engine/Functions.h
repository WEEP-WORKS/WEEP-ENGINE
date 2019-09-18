#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <list>
#include <string>

// -----------------------------------------
// -----------------------------------------

// USEFULL FUNCTIONS //

// -----------------------------------------
// -----------------------------------------

// Returns the angle between two points in degrees
float AngleFromTwoPoints(float x1, float y1, float x2, float y2);

// Returns the distance from two points(can be a negative distance)
float DistanceFromTwoPoints(float x1, float y1, float x2, float y2);

// Compares two char*, true if success
bool TextCmp(const char* text1, const char* text2);

//Separate a string into tokens using the indicated iterator
void Tokenize(std::string string, const char separator, std::list<std::string>& tokens);

//Puts al chars to lower case
void ToLowerCase(std::string str);

// Returns a random int given a range;
int GetRandomValue(int range_1, int range_2);

#endif //__FUNCTIONS_H__