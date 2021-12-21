#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h> // Header File For Windows
#include <stdio.h>
#include <iostream>
#include <gl/gl.h>   // Header File For The OpenGL32 Library
#include <gl/glut.h> // Header File For The GLu32 Library
#include <gl/glu.h>
#include <cmath>
#include <time.h>
#include <random>
#include <texture.h>
#include "spaceship.h"
#include "utility.h"

using namespace std;

HDC hDC = NULL;      // Private GDI Device Context
HGLRC hRC = NULL;    // Permanent Rendering Cntext
HWND hWnd = NULL;    // Holds Our Window Handle
HINSTANCE hInstance; // Holds The Instance Of The Application

bool keys[256];          // Array Used For The Keyboard Routine
bool active = TRUE;      // Window Active Flag Set To TRUE By Default
bool fullscreen = FALSE; // Fullscreen Flag Set To Fullscreen Mode By Default


DeltaSpaceShip deltaSpaceShip(0, -0.7);
set<SpaceShip*> zetaSpaceShips;
int score = 0;
time_t startTime = time(0);
int backgroundImg;

int alphaKilled = 0;
int betaKilled = 0;
int gamaKilled = 0;

int stage = 1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Declaration For WndProc

void drawText(double x, double y, double z,  char *string) {
	glColor3f(1, 1, 1);
	glRasterPos3f(x, y, 0);
	int len;
	len = (int)strlen(string);
	for (int i = 0; i < len; i++) {
		// NOT WORKING.
		// glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

GLvoid handleKeyPress() {
	if(keys['W']) {
		deltaSpaceShip.shoot();
	}
	else if(keys['A']) {
		if (deltaSpaceShip.x >= -0.7) {
			deltaSpaceShip.x -= 0.1;
		}
	}
	else if(keys['D']) {
		if (deltaSpaceShip.x <= 0.7) {
			deltaSpaceShip.x += 0.1;
		}
	}
	else if(keys[VK_ESCAPE]) {
		exit(0);
	}
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) { // Resize And Initialize The GL Window
    if (height == 0) { // Prevent A Divide By Zero By
        height = 1; // Making Height Equal One
    }
    glViewport(0, 0, width, height); // Reset The Current Viewport
    glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    glLoadIdentity();            // Reset The Projection Matrix
    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glFrustum(1.0, 1.0, 1.0, 1.0, 1., 100.0);
    glOrtho(1.0, 1.0, 1.0, 1.0, 1., 100.0);
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity();           // Reset The Modelview Matrix
}

int InitGL(GLvoid) { // All Setup For OpenGL Goes Here
    glShadeModel(GL_SMOOTH);                           // Enable Smooth Shading
    glClearDepth(1.0f);                                // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                           // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                            // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations
	backgroundImg = LoadTexture("C:/Users/DIGI TECH/Documents/Visual Studio 2012/Projects/space-invaders/space.bmp", 255);
    return TRUE;
}

GLvoid insertNewZeta() {
	if ((time(0) - startTime) == 3) {
		startTime = time(0);
		GLdouble random = -0.7 + (double)(rand()) / ((double)(RAND_MAX / (0.7 - (-0.7))));
		int random2 = rand() % 6;
		if (random2 == 0 || random2 == 1 || random2 == 2) {
			zetaSpaceShips.insert(new AlphaSpaceShip(random, 0.7));
		}
		else if (random2 == 3 || random2 == 4) {
			zetaSpaceShips.insert(new BetaSpaceShip(random, 0.7));
		}
		else {
			zetaSpaceShips.insert(new GamaSpaceShip(random, 0.7));
		}
	}
}

GLvoid drawBackground() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, backgroundImg);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2d(-1, -1);
	glTexCoord2f(1, 0);
	glVertex2d(1, -1);
	glTexCoord2f(1, 1);
	glVertex2d(1, 1);
	glTexCoord2f(0, 1);
	glVertex2d(-1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawGLScene(GLvoid) { // Here's Where We Do All The Drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	glTranslated(0, 0, -2.5);
	
	drawBackground();

	// drawText(0.5, 0.5, 0, "Hello World");

	handleKeyPress();

	insertNewZeta();
	
	vector<set<SpaceShip*>::iterator> toDeleteZetaShips;
	vector<set<SpaceShot*>::iterator> toDeleteDeltaShots;
	vector<set<SpaceShot*>::iterator> toDeleteZetaShots;

	for (auto zetaShipIt = zetaSpaceShips.begin(); zetaShipIt != zetaSpaceShips.end(); zetaShipIt++) {
		if ((*zetaShipIt)->y <= deltaSpaceShip.y
			&& abs((*zetaShipIt)->x - deltaSpaceShip.x) < 0.4
			&& (*zetaShipIt)->y >= -0.8)
		{
			exit(0);
		}
		for (auto deltaShotIt = deltaSpaceShip.shots.begin(); deltaShotIt != deltaSpaceShip.shots.end(); deltaShotIt++) {
			if ((*deltaShotIt)->y >= (*zetaShipIt)->y
				&& (*deltaShotIt)->x <= (*zetaShipIt)->x + 0.2
				&& (*deltaShotIt)->x >= (*zetaShipIt)->x - 0.2
				&& (*deltaShotIt)->y < 0.7)
			{
				(*zetaShipIt)->health -= (*deltaShotIt)->strength;
				if ((*zetaShipIt)->health <= 0) {
					toDeleteZetaShips.push_back(zetaShipIt);
					if((*zetaShipIt)->type == 2) alphaKilled++;
					else if((*zetaShipIt)->type == 3) betaKilled++;
					else if((*zetaShipIt)->type == 4) gamaKilled++;

					if(betaKilled >= 10 && betaKilled % 10 == 0) {
						deltaSpaceShip.health++;
					}
					if(gamaKilled >= 5 && gamaKilled % 5 == 0) {
						deltaSpaceShip.shotStrength++;
					}
					if(stage == 1 && (alphaKilled + betaKilled + gamaKilled) >= 5 && (alphaKilled + betaKilled + gamaKilled) % 5 == 0) {
						stage++;
					}
					else if(stage == 2 && (alphaKilled + betaKilled + gamaKilled) >= 15 && (alphaKilled + betaKilled + gamaKilled) % 15 == 0) {
						stage++;
					}
					score++;
				}
				toDeleteDeltaShots.push_back(deltaShotIt);
				break;
			}
		}
		double speed;
		if(stage == 1)
			speed = 0.01;
		else if(stage == 2)
			speed = 0.03;
		else speed = 0.05;
		(*zetaShipIt)->y -= speed;
		(*zetaShipIt)->draw();
		if ((*zetaShipIt)->y < -1) {
			toDeleteZetaShips.push_back(zetaShipIt);
		}
	}

	for (auto deltaShotIt = deltaSpaceShip.shots.begin(); deltaShotIt != deltaSpaceShip.shots.end(); deltaShotIt++) {
		double speed;
		if(stage == 1)
			speed = 0.05;
		else if(stage == 2)
			speed = 0.07;
		else speed = 0.09;
		(*deltaShotIt)->y += speed;
		(*deltaShotIt)->draw();
		if ((*deltaShotIt)->y > 1) {
			toDeleteDeltaShots.push_back(deltaShotIt);
		}







	}

	for (auto zetaShipIt = zetaSpaceShips.begin(); zetaShipIt != zetaSpaceShips.end(); zetaShipIt++) {
		(*zetaShipIt)->shoot();
		for (auto zetaShotsIt = (*zetaShipIt)->shots.begin(); zetaShotsIt != (*zetaShipIt)->shots.end(); zetaShotsIt++) {
			if ((*zetaShotsIt)->y <= deltaSpaceShip.y
				&& (*zetaShotsIt)->x <= deltaSpaceShip.x + 0.2
				&& (*zetaShotsIt)->x >= deltaSpaceShip.x - 0.2
				&& (*zetaShotsIt)->y >= -0.8)
			{
				toDeleteZetaShots.push_back(zetaShotsIt);
				deltaSpaceShip.health -= (*zetaShotsIt)->strength;
				if (deltaSpaceShip.health <= 0) {
					exit(0);
				}
			}
			double speed;
			if(stage == 1)
				speed = 0.05;
			else if(stage == 2)
				speed = 0.07;
			else speed = 0.09;
			(*zetaShotsIt)->y -= speed;
			(*zetaShotsIt)->draw();
		}
		for (set<SpaceShot*>::iterator it : toDeleteZetaShots) {
			(*zetaShipIt)->shots.erase(it);
		}
	}

	for (set<SpaceShip*>::iterator it : toDeleteZetaShips) {
		zetaSpaceShips.erase(it);
	}

	for (set<SpaceShot*>::iterator it : toDeleteDeltaShots) {
		deltaSpaceShip.shots.erase(it);
	}

	deltaSpaceShip.draw();

    glFlush(); // DO NOT REMOVE THIS
    SwapBuffers(hDC);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------

GLvoid KillGLWindow(GLvoid) { // Properly Kill The Window
    if (fullscreen) { // Are We In Fullscreen Mode?
        ChangeDisplaySettings(NULL, 0); // If So Switch Back To The Desktop
        ShowCursor(TRUE);               // Show Mouse Pointer
    }
    if (hRC) { // Do We Have A Rendering Context?
        if (!wglMakeCurrent(NULL, NULL)) { // Are We Able To Release The DC And RC Contexts?
            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }
        if (!wglDeleteContext(hRC)) { // Are We Able To Delete The RC?
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }
        hRC = NULL; // Set RC To NULL
    }
    if (hDC && !ReleaseDC(hWnd, hDC)) { // Are We Able To Release The DC
        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hDC = NULL; // Set DC To NULL
    }
    if (hWnd && !DestroyWindow(hWnd)) { // Are We Able To Destroy The Window?
        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hWnd = NULL; // Set hWnd To NULL
    }
    if (!UnregisterClass("OpenGL", hInstance)) { // Are We Able To Unregister Class
        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL; // Set hInstance To NULL
    }
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(const char *title, int width, int height, int bits, bool fullscreenflag) {
    GLuint PixelFormat;               // Holds The Results After Searching For A Match
    WNDCLASS wc;                      // Windows Class Structure
    DWORD dwExStyle;                  // Window Extended Style
    DWORD dwStyle;                    // Window Style
    RECT WindowRect;                  // Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left = (long)0;        // Set Left Value To 0
    WindowRect.right = (long)width;   // Set Right Value To Requested Width
    WindowRect.top = (long)0;         // Set Top Value To 0
    WindowRect.bottom = (long)height; // Set Bottom Value To Requested Height

    fullscreen = fullscreenflag; // Set The Global Fullscreen Flag

    hInstance = GetModuleHandle(NULL);             // Grab An Instance For Our Window
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc = (WNDPROC)WndProc;             // WndProc Handles Messages
    wc.cbClsExtra = 0;                             // No Extra Window Data
    wc.cbWndExtra = 0;                             // No Extra Window Data
    wc.hInstance = hInstance;                      // Set The Instance
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);        // Load The Default Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);      // Load The Arrow Pointer
    wc.hbrBackground = NULL;                       // No Background Required For GL
    wc.lpszMenuName = NULL;                        // We Don't Want A Menu
    wc.lpszClassName = "OpenGL";                   // Set The Class Name

    if (!RegisterClass(&wc)) { // Attempt To Register The Window Class
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }
    if (fullscreen) { // Attempt Fullscreen Mode?
        DEVMODE dmScreenSettings;                               // Device Mode
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);     // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth = width;                   // Selected Screen Width
        dmScreenSettings.dmPelsHeight = height;                 // Selected Screen Height
        dmScreenSettings.dmBitsPerPel = bits;                   // Selected Bits Per Pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
                fullscreen = FALSE; // Windowed Mode Selected.  Fullscreen = FALSE
            }
            else {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                return FALSE; // Return FALSE
            }
        }
    }
    if (fullscreen) { // Are We Still In Fullscreen Mode?
        dwExStyle = WS_EX_APPWINDOW; // Window Extended Style
        dwStyle = WS_POPUP;          // Windows Style
        ShowCursor(FALSE);           // Hide Mouse Pointer
    }
    else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;                  // Windows Style
    }
    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size
    // Create The Window
    if (!(hWnd = CreateWindowEx(dwExStyle,                          // Extended Style For The Window
                                "OpenGL",                           // Class Name
                                title,                              // Window Title
                                dwStyle |                           // Defined Window Style
                                    WS_CLIPSIBLINGS |               // Required Window Style
                                    WS_CLIPCHILDREN,                // Required Window Style
                                0, 0,                               // Window Position
                                WindowRect.right - WindowRect.left, // Calculate Window Width
                                WindowRect.bottom - WindowRect.top, // Calculate Window Height
                                NULL,                               // No Parent Window
                                NULL,                               // No Menu
                                hInstance,                          // Instance
                                NULL)))                             // Dont Pass Anything To WM_CREATE
    {
        KillGLWindow(); // Reset The Display
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }
    static PIXELFORMATDESCRIPTOR pfd = // pfd Tells Windows How We Want Things To Be
        {
            sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
            1,                             // Version Number
            PFD_DRAW_TO_WINDOW |           // Format Must Support Window
                PFD_SUPPORT_OPENGL |       // Format Must Support OpenGL
                PFD_DOUBLEBUFFER,          // Must Support Double Buffering
            PFD_TYPE_RGBA,                 // Request An RGBA Format
            bits,                          // Select Our Color Depth
            0, 0, 0, 0, 0, 0,              // Color Bits Ignored
            0,                             // No Alpha Buffer
            0,                             // Shift Bit Ignored
            0,                             // No Accumulation Buffer
            0, 0, 0, 0,                    // Accumulation Bits Ignored
            16,                            // 16Bit Z-Buffer (Depth Buffer)
            0,                             // No Stencil Buffer
            0,                             // No Auxiliary Buffer
            PFD_MAIN_PLANE,                // Main Drawing Layer
            0,                             // Reserved
            0, 0, 0                        // Layer Masks Ignored
        };
    if (!(hDC = GetDC(hWnd))) { // Did We Get A Device Context?
        KillGLWindow(); // Reset The Display
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }
    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) { // Did Windows Find A Matching Pixel Format?
        KillGLWindow(); // Reset The Display
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }
    if (!SetPixelFormat(hDC, PixelFormat, &pfd)) { // Are We Able To Set The Pixel Format?
        KillGLWindow(); // Reset The Display
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }
    if (!(hRC = wglCreateContext(hDC))) { // Are We Able To Get A Rendering Context?
        KillGLWindow(); // Reset The Display
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }
    if (!wglMakeCurrent(hDC, hRC)) { // Try To Activate The Rendering Context
        KillGLWindow(); // Reset The Display
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }
    ShowWindow(hWnd, SW_SHOW);    // Show The Window
    SetForegroundWindow(hWnd);    // Slightly Higher Priority
    SetFocus(hWnd);               // Sets Keyboard Focus To The Window
    ReSizeGLScene(width, height); // Set Up Our Perspective GL Screen
    if (!InitGL()) { // Initialize Our Newly Created GL Window
        KillGLWindow(); // Reset The Display
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }
    return TRUE; // Success
}

LRESULT CALLBACK WndProc(HWND hWnd,     // Handle For This Window
                         UINT uMsg,     // Message For This Window
                         WPARAM wParam, // Additional Message Information
                         LPARAM lParam) // Additional Message Information
{
    static PAINTSTRUCT ps;

    switch (uMsg) { // Check For Windows Messages
		case WM_PAINT:
			DrawGLScene();
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;

		case WM_TIMER:
			DrawGLScene();
			return 0;

		case WM_ACTIVATE: { // Watch For Window Activate Message
			if (!HIWORD(wParam)) { // Check Minimization State
				active = TRUE; // Program Is Active
			}
			else {
				active = FALSE; // Program Is No Longer Active
			}

			return 0; // Return To The Message Loop
		}

		case WM_SYSCOMMAND: { // Intercept System Commands
			switch (wParam) { // Check System Calls
			case SC_SCREENSAVE:   // Screensaver Trying To Start?
			case SC_MONITORPOWER: // Monitor Trying To Enter Powersave?
				return 0;         // Prevent From Happening
			}
			break; // Exit
		}

		case WM_CLOSE: { // Did We Receive A Close Message?
			PostQuitMessage(0); // Send A Quit Message
			return 0;           // Jump Back
		}

		case WM_KEYDOWN: { // Is A Key Being Held Down?
			keys[wParam] = TRUE; // If So, Mark It As TRUE
			return 0;            // Jump Back
		}

		case WM_KEYUP: { // Has A Key Been Released?
			keys[wParam] = FALSE; // If So, Mark It As FALSE
			return 0;             // Jump Back
		}

		case WM_SIZE: { // Resize The OpenGL Window
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // LoWord=Width, HiWord=Height
			return 0;                                      // Jump Back
		}
    }
    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,     // Instance
                   HINSTANCE hPrevInstance, // Previous Instance
                   LPSTR lpCmdLine,         // Command Line Parameters
                   int nCmdShow)            // Window Show State
{
    MSG msg;           // Windows Message Structure
    BOOL done = FALSE; // Bool Variable To Exit Loop
	srand(time(0));

    // Ask The User Which Screen Mode They Prefer
    // if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
    {
        fullscreen = FALSE; // Windowed Mode
    }

    // Create Our OpenGL Window
    if (!CreateGLWindow("Space Invaders", 500, 500, 16, fullscreen)) {
        return 0; // Quit If Window Was Not Created
    }

    // Set drawing timer to 45 frame per second
    UINT timer = SetTimer(hWnd, 0, 45, (TIMERPROC)NULL);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}