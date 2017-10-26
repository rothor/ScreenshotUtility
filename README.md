# Screenshot Utility

Version: 1.0  
Last updated: Oct 26, 2017

## About this project

This project is a Windows desktop application, written in C++, that allows you to easily take screenshots and save them as a file. An overlay on the screen can be altered with the mouse to control the cropping area, and pressing P, J, G, or B on the keyboard will save the image as a PNG, JPG, GIF, or BMP to your computer. This project was made for and tested on Windows 10.

## Building this project with MS Visual Studio 2017

1. Clone this project to a local folder.
2. Open "ScreenshotUtility.sln" in Visual Studio by going to
	```
	File > Open > Project/Solution
	```
3. Once opened, turn off precompiled headers by going to
	```
	Project > Properties > C/C++ > Precompiled Headers > Precompiled Header
	```
	and choosing "Not Using Precompiled Headers" from the dropdown list.
4. Build and run the project.

## Instructions for using application

- Use **left click** and **right click** to control the cropping of the image.
- Use **arrow keys** to also control the cropping of the image.
- Use the **mouse wheel** and **middle click** to change the crop mask's appearance.
- Press **J** to save image as JPG.
- Press **G** to save image as GIF.
- Press **P** to save image as PNG.
- Press **B** to save image as BMP.
- Press **Escape** to exit.

Note: Images are saved in 'pics' folder where the application is located.

## Author

Author: Robert Thorsberg  
Contact: jowarp@yahoo.com  
Git: rothor

## License

This project is licensed under the MIT License - see the LICENSE.txt file for details