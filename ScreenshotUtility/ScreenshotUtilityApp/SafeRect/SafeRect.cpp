#include "SafeRect.h"


SafeRect::SafeRect()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

SafeRect::SafeRect(int x, int y, int width, int height)
{
	left = x;
	top = y;
	if (width > 0)
		right = x + width;
	else
		right = x + 1;
	if (height > 0)
		bottom = y + height;
	else
		bottom = y + 1;
}

int SafeRect::getLeft()
{
	return left;
}

int SafeRect::getRight()
{
	return right;
}

int SafeRect::getTop()
{
	return top;
}

int SafeRect::getBottom()
{
	return bottom;
}

int SafeRect::getWidth()
{
	return right - left;
}

int SafeRect::getHeight()
{
	return bottom - top;
}

void SafeRect::setLeft(int pos)
{
	left = pos;
	if (left >= right)
		right = left + 50;
}

void SafeRect::setRight(int pos)
{
	right = pos;
	if (left >= right)
		left = right - 50;
}

void SafeRect::setTop(int pos)
{
	top = pos;
	if (top >= bottom)
		bottom = top + 50;
}

void SafeRect::setBottom(int pos)
{
	bottom = pos;
	if (top >= bottom)
		top = bottom - 50;
}

void SafeRect::moveLeft(int amount)
{
	left += amount;
	if (left >= right)
		left = right - 1;
}

void SafeRect::moveRight(int amount)
{
	right += amount;
	if (left >= right)
		right = left + 1;
}

void SafeRect::moveTop(int amount)
{
	top += amount;
	if (top >= bottom)
		top = bottom - 1;
}

void SafeRect::moveBottom(int amount)
{
	bottom += amount;
	if (top >= bottom)
		bottom = top + 1;
}