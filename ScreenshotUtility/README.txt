Summary of this program's structure:

ScreenshotUtility.cpp
	This is where WinMain is located (the entry point for the
	application). In WinMain, GDI+ is initialized, then an instance of
	PrintScreen is created, and then the message-loop is started. The
	PrintScreen class creates the application's window, and is
	responsible for most of the application's functionality.
	
PrintScreen.cpp and PrintScreen.h
	These contains the PrintScreen class. It is referenced by the
	WinMain function.
	
BasicWindowMaker
	This folder contains the files for the BasicWindowMaker class. It
	has a static method that creates a window using the Windows API. It
	is used by the PrintScreen class to create the application's window.
	
GraphicsInterface:
	This folder contains the files for the GraphicsInterface class.
	Rather than dealing with GDI+ directly, the PrintScreen class uses
	this class as an interface to draw to the window using double
	buffering.
	