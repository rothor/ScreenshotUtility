#pragma once
#include <string>


class Screenshotter
{
public:
	Screenshotter() = delete;
	static void takeScreenshot(int x, int y, int w, int h, std::wstring fileName);
};