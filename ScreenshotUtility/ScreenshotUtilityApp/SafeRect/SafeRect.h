#pragma once


class SafeRect
{
public:
	SafeRect();
	SafeRect(int x, int y, int width, int height);
	int getLeft();
	int getRight();
	int getTop();
	int getBottom();
	int getWidth();
	int getHeight();
	void setLeft(int pos);
	void setRight(int pos);
	void setTop(int pos);
	void setBottom(int pos);
	void moveLeft(int amount);
	void moveRight(int amount);
	void moveTop(int amount);
	void moveBottom(int amount);

private:
	int left;
	int top;
	int right;
	int bottom;
};