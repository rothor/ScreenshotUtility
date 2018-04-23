#pragma once
#include <Windows.h>
#include <vector>


class OptionsWindow
{
public:
	OptionsWindow(HWND hwndIn, HINSTANCE hInstIn);
	void show();
	void hide();

private:
	HINSTANCE hInst_;
	HWND mainHwnd_;
	HWND optionsHwnd_;
	HWND lbWindowsHwnd_, bnRefreshHwnd_, ecDimensionsHwnd_;
	std::vector<HWND> windowList;
	
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void createWindow();

	void loadWindowList();
	static BOOL CALLBACK enumWindowsProc(HWND hWnd, LPARAM lParam);
};