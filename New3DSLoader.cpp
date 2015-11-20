// New3DSLoader.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <math.h>
#include "resource.h"
#include <fstream>
//#include <ctime>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include "3dsModel.h"
#include "Octree.h"


#define SCREEN_WIDTH 1200								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 800								// We want our screen height 600 pixels
#define SCREEN_DEPTH 16									// We want 16 bits per pixel

//����·��
char g_sAppPath[512];
//ģ��·��
char g_sMediaPath[512];
//����ű�100����
UINT g_Texture[100] = {0};						// This holds the texture info, referenced by an ID

//ȫ�ֲ���
bool  g_bFullScreen = true;	
bool isrotate=false;
bool isrotate2=false;
UINT g_ViewMode;							// Set full screen as default
BOOL g_bLighting=FALSE;
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()

float g_fEyeX=0;
float g_fEyeZ=0;
float g_fEyeY=10;
float g_fRotateAngle=3.14f;
float s_angle = -90.0;//new add
int n=0;   //������ֹ���岿λ��ת��
//�����������ݿ�,�ɵ����������ã�����ò�ͬ�Ĺ���Ч��
GLfloat no_mat[]={0.0,0.0,0.0,1.0};
GLfloat mat_ambient[]={0.4,0.4,0.4,1.0};
GLfloat mat_diffuse[]={0.8,0.8,0.8,1.0};
GLfloat mat_specular[]={1.0,1.0,1.0,1.0};
GLfloat mat_ambient_color[]={0,0,0.2,1.0};//0.245,0.15625,0.03
GLfloat mat_diffuse_color[]={0,0,0.4,1.0};//0.49,0.3125,0.06,
GLfloat mat_specular_color[]={0,0,0.8,1.0};//0.98,0.625,0.12
GLfloat no_shininess[]={0.0};
GLfloat low_shininess[]={5.0};
GLfloat high_shininess[]={300.0};
GLfloat mat_emission[]={0.3,0.2,0.2,0.0};

//3��ģ��
C3DSModel g_3dsModel[3];//Ŀǰֻ�õ�����
CMSModel m_MSModel;
PiMotion pimo[10];//�����ݽṹ������"Mass-SpringModel.h"�У������ÿһ����������Ӧ�Ĳ�λ����10��������i��λ�˶����򽫸ò�λ�˶��ķ���ͽǶ�������pimo[i]��
//Human human;
//
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);
void ChangeToFullScreen();
WPARAM MainLoop();//��ϵͳ����ѭ������

void InitializeOpenGL(int width, int height);//��ʼ��OpenGL
void SizeOpenGLScreen(int width, int height);
bool bSetupPixelFormat(HDC hdc);

void Init(HWND hWnd);
void DeInit();

void RenderScene();//��Ⱦ��������������ѭ������MainLoop()�ﱻѭ������
void UpdateScene();//���³�������������ѭ������MainLoop()�ﱻѭ������
void SetMotion(int parti,float angle,float x,float y,float z);//�����˶�����������������pimo[parti]�еĲ�����������Ȼ����ĳ��λ�˶���ʱ����ã�һ�������ﾲ̬�Դ�����ƽ�����ã�
void changeMark(int parti);//�ı�pimo[parti]�е�isfirstSet��ֵtrueΪfalse����һ�ε���SetMotion����������isfirstSetΪtrue��Ȼ�������Ҫ,�����޸�Ϊfalse
void CancelMotion(int parti);//�ı�pimo[parti]�е�ismotion��ֵtrueΪfalse����һ�ε���SetMotion����������ismotionΪtrue����ֹͣ�˶�ʱ����Ϊfalse
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{                                            //��ϵͳΨһ��������
	HWND hWnd;
	//Ӧ�ó���·��
	GetModuleFileName(hInstance,g_sAppPath,512);
	GetFilePath (g_sAppPath);
	//�ز�·��
	strcpy(g_sMediaPath,g_sAppPath);
	//strcat(g_sMediaPath,"������\\");

	//Ůʽ����for test
	//strcat(g_sMediaPath, "����1\\");

	//strcat(g_sMediaPath, "Ů���·�\\");

	strcat(g_sMediaPath, "�����·�\\");


	//�Ƿ�ʹ��ȫ��Ļģʽ
	//if(MessageBox(NULL, "�Ƿ�ʹ��ȫ��Ļģʽ?", "����ѡ��", MB_YESNO | MB_ICONQUESTION) == IDNO)
		g_bFullScreen = false;
	//���ɴ���
	hWnd = CreateMyWindow("www.C3DN.net", SCREEN_WIDTH, SCREEN_HEIGHT, 0, g_bFullScreen, hInstance);
	// ���û�гɹ����˳�
	if(hWnd == NULL) return true;
	// ��ʼ��OPENGL
	Init(hWnd);	//������·���Ԥ�����ڸú������������
	void CALLBACK MoveNear(void);//add new
	void CALLBACK MoveFar(void);//add new

	// ��ѭ��
	return MainLoop();//��ϵͳ��Ҫ��ѭ��������RenderScene()�� UpdateScene()����������Ҫ���֣�������·����Դ�������RenderScene()�����
}
//���ɴ���
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;
	// ע�ᴰ����
	MyRegisterClass(hInstance);
	//ȫ��Ļ�ж�
	if(bFullScreen && !dwStyle) 						// Check if we wanted full screen mode
	{													// Set the window properties for full screen mode
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ChangeToFullScreen();							// Go to full screen
		//ShowCursor(FALSE);								// Hide the cursor
	}
	else if(!dwStyle)									// Assign styles to the window depending on the choice
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	g_hInstance = hInstance;							// Assign our global hInstance to the window's hInstance

	RECT rWindow;
	if(bFullScreen)
	{
		rWindow.left	= 0;
		rWindow.right	= width;						// Set Right Value To Requested Width
		rWindow.top	    = 0;
		rWindow.bottom	= height;
	}
	else
	{
		rWindow.left	= (GetSystemMetrics(SM_CXSCREEN)-width)/2;	// Set Left Value To 0
		rWindow.right	= rWindow.left+width;						// Set Right Value To Requested Width
		rWindow.top	    = (GetSystemMetrics(SM_CYSCREEN)-height)/2;	// Set Top Value To 0
		rWindow.bottom	= rWindow.top+height;						// Set Bottom Value To Requested Height
	}
	AdjustWindowRect( &rWindow, dwStyle, false);		// Adjust Window To True Requested Size

														// Create the window
	hWnd = CreateWindow("bvrain", strWindowName, dwStyle, rWindow.left, rWindow.top,
						width, height, 
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;								// If we could get a handle, return NULL

	ShowWindow(hWnd, SW_SHOWNORMAL);					// Show the window
	UpdateWindow(hWnd);									// Draw the window

	SetFocus(hWnd);										// Sets Keyboard Focus To The Window	

	return hWnd;
}

//ע�ᴰ����
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_NEW3DSLOADER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_NEW3DSLOADER;
	wcex.lpszClassName	= "bvrain";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}
//������Ϣ������
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   float aaa=g_fEyeX, bbb = g_fEyeZ;
	int wmId, wmEvent;
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

	switch (uMsg) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(g_hInstance, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				    PostQuitMessage(0);	
				   break;
				default:
				   return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			break;
    case WM_SIZE:										// If the window is resized
		if(!g_bFullScreen)								// Do this only if we are NOT in full screen
		{
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
			GetClientRect(hWnd, &g_rRect);				// Get the window rectangle
		}
        break; 
 
	case WM_PAINT:										// If we need to repaint the scene
		BeginPaint(hWnd, &ps);							// Init the paint struct		
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
		break;

	//case WM_LBUTTONDOWN:								// If the left mouse button was clicked
	//	
	//	if(g_ViewMode == GL_TRIANGLES) {				// We our drawing mode is at triangles
	//		g_ViewMode = GL_LINE_STRIP;					// Go to line stips
	//	} else {
	//		g_ViewMode = GL_TRIANGLES;					// Go to triangles
	//	}
	//	break;

	case WM_LBUTTONUP:
		isrotate=false;
		break;
	case WM_RBUTTONDOWN:								// If the right mouse button was clicked.
		
		g_bLighting = !g_bLighting;						// Turn lighting ON/OFF

		if(g_bLighting) {								// If lighting is ON
			glEnable(GL_LIGHTING);						// Enable OpenGL lighting
		} else {
			glDisable(GL_LIGHTING);						// Disable OpenGL lighting
		}
		break;

	case WM_KEYDOWN:									// If we pressed a key

		switch(wParam) {								// Check if we hit a key
			case VK_ESCAPE:								// If we hit the escape key
				PostQuitMessage(0);						// Send a QUIT message to the window
				break;
			case VK_LEFT:
				isrotate=true;
				break;
			case VK_RIGHT:
				isrotate2=true;
				break;
			case VK_DOWN:
				n=0;
				break;
		}
		break;
	case WM_KEYUP:
		switch(wParam) {								// Check if we hit a key
			case VK_LEFT:
				isrotate=false;
				break;
			case VK_RIGHT:
				isrotate2=false;
				break;
		}
		break;
    case WM_CLOSE:										// If the window is being closed
        PostQuitMessage(0);								// Send a QUIT Message to the window
        break; 
   
    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet;										// Return by default
}
// ���ڶԻ�����Ϣ������
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
//��ѭ��
WPARAM MainLoop()
{
	MSG msg;
	while(true)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
			//�����������Դ��ĸ�������Ⱦ��������
			UpdateScene();
			RenderScene();
			
		}
    } 
	DeInit();											// Clean up and free all allocated memory
	return(msg.wParam);									// Return from the program
}
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	��ʼ������
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
void Init(HWND hWnd)
{
	//��ʼ��pimo
	for(int i=0;i<10;i++){
		pimo[i].ismotion=false;
		pimo[i].isfirstSet=false;
		pimo[i].d_a=new direction_angle;
		memset(pimo[i].d_a, 0, sizeof(direction_angle));
	}
	GLfloat glDarkColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat glfLightAmbient[] = { 1.0f, 1.0f, 1.0f,1.0f };
    GLfloat glfLightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat glfLightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat glfLingtPosition1[] = {50.0f, 50.0f,0.0f, 1.0f };
	GLfloat glfLingtPosition2[] = { 10.0f, 0.0f,10.0f, 1.0f };
	GLfloat glfLingtPosition3[] = { -10.0f, -100.0f, -100.0f, 1.0f };
    
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	//��ȡ3��ģ��
	//----------------------------------------------------------
	//g_3dsModel[0].LoadModelFromFile("shirt_02.3DS",0);//g_3dsModel[]ΪC3DSModel�Ķ���3������ֻ����������������LoadModelFromFileʵ����3DSModel.cpp�ļ���

	//g_3dsModel[0].LoadModelFromFile("Ůʽ����1new.3DS", 0);
	//g_3dsModel[0].LoadModelFromFile("T��2new.3DS", 0);
	g_3dsModel[0].LoadModelFromFile("����1new.3DS", 0);
	//g_3dsModel[1].LoadModelFromFile("��ȹ3new.3DS",1);
	//g_3dsModel[1].LoadModelFromFile("����1new.3ds",1);

	glLightfv (GL_LIGHT0, GL_AMBIENT, glfLightAmbient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, glfLightDiffuse);
    glLightfv (GL_LIGHT0, GL_SPECULAR, glfLightSpecular);
	glLightfv (GL_LIGHT0, GL_POSITION, glfLingtPosition1);

	glLightfv (GL_LIGHT1, GL_AMBIENT, glDarkColor);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, glfLightDiffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, glfLightSpecular);
	glLightfv (GL_LIGHT1, GL_POSITION, glfLingtPosition2);

	glLightfv (GL_LIGHT2, GL_AMBIENT, glDarkColor);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, glfLightDiffuse);
    glLightfv (GL_LIGHT2, GL_SPECULAR, glfLightSpecular);
	glLightfv (GL_LIGHT2, GL_POSITION, glfLingtPosition3);

    //glEnable(GL_LIGHTING);								// Turn on lighting
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);								// Turn on a light with defaults set
	glEnable(GL_LIGHT1);
//	glEnable(GL_LIGHT2);

	//glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT);
	//glEnable(GL_COLOR_MATERIAL);
}

//��ʼ��OpenGL
void InitializeOpenGL(int width, int height) 
{  
    g_hDC = GetDC(g_hWnd);								// This sets our global HDC
														// We don't free this hdc until the end of our program
    if (!bSetupPixelFormat(g_hDC))						// This sets our pixel format/information
        PostQuitMessage (0);							// If there's an error, quit

    g_hRC = wglCreateContext(g_hDC);					// This creates a rendering context from our hdc
    wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use

	glEnable(GL_TEXTURE_2D);							// Enables Texture Mapping
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing

	SizeOpenGLScreen(width, height);					// Setup the screen translations and viewport
}

void CALLBACK MoveNear(void)
{
    g_fEyeZ -= 0.3f;
}
void CALLBACK MoveFar(void)
{
    g_fEyeZ += 0.3f;
}

//�ı䴰�ڴ�С
void SizeOpenGLScreen(int width, int height)			// Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero error
	{
		height=1;										// Make the Height Equal One
	}

	glViewport(0,0,width,height);						// Make our viewport the whole window
														// We could make the view smaller inside
														// Our window if we wanted too.
														// The glViewport takes (x, y, width, height)
														// This basically means, what our our drawing boundries

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
														// The parameters are:
														// (view angle, aspect ration of the width to the height, 
														//  The closest distance to the camera before it clips, 
				  // FOV		// Ratio				//  The farthest distance before it stops drawing)
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 1.0f ,800.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}
//��������
void DeInit()
{
	if (g_hRC)											
	{
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_hRC);						// Delete our OpenGL Rendering Context	
	}
	
	if (g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);						// Release our HDC from memory
		
	if(g_bFullScreen)									// If we were in full screen
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}
	UnregisterClass("bvrain", g_hInstance);		// Free the window class
	PostQuitMessage (0);								// Post a QUIT message to the window
}
//�����˶�
void SetMotion(int parti,float angle,float x,float y,float z){
	pimo[parti].ismotion=true;
	pimo[parti].isfirstSet=true;
	pimo[parti].d_a->angle=angle;
	pimo[parti].d_a->x=x;
	pimo[parti].d_a->y=y;
	pimo[parti].d_a->z=z;
}
void changeMark(int parti){
	pimo[parti].isfirstSet=false;
}
void CancelMotion(int parti){
	pimo[parti].ismotion=false;
}
//���³���
void UpdateScene()
{
	static DWORD elapsedTime   = 0;
	static DWORD lastTime	  = 0;

	DWORD time = GetTickCount();

	elapsedTime = time - lastTime;

	////�Զ���ת
	//if (elapsedTime >= 80) //40 ����
	//{
	//	g_fRotateAngle += 2 * float(PI / 360);
	//	if (g_fRotateAngle >= 2 * PI)g_fRotateAngle = 0.0f;

	//	g_fEyeX = 30 * sinf(g_fRotateAngle);
	//	g_fEyeZ = 30 * cosf(g_fRotateAngle);

	//	lastTime = time;
	//}


	//�������ϵ��������Ҽ�����ת�ӽ�
	if (elapsedTime >=40&&isrotate) //40 ����
	{
		g_fRotateAngle+=8*float(PI/360);
		if(g_fRotateAngle>=2*PI)g_fRotateAngle=0.0f;

		g_fEyeX=30*sinf(g_fRotateAngle);
		g_fEyeZ=30*cosf(g_fRotateAngle);			
		
		lastTime = time;
	}
	if(elapsedTime >=40&&isrotate2)
	{
		g_fRotateAngle-=8*float(PI/360);
		if(g_fRotateAngle>=2*PI)g_fRotateAngle=0.0f;

		g_fEyeX=30*sinf(g_fRotateAngle);
		g_fEyeZ=30*cosf(g_fRotateAngle);			
		
		lastTime = time;
	}
	if(n==0)
	{
		g_fEyeX=0.0;
		g_fEyeY=10.0;
		g_fEyeZ=-30.0;
	}
	n++;
}

//���Ƴ���
void RenderScene() 
{
	//int i,j;
	glClearColor(0.8f, 0.8f, 1.0f, 1);//0.8f,0.8f,1.0f     0.8f, 0.8f, 1.0f, 1      1.0,1.0,1.0,1.0   1.0, 1.0, 1.0, 0.0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix

   gluLookAt(-g_fEyeX,g_fEyeY,-g_fEyeZ,0,0,0,0,1,0);//��ת�ӽ�
	//gluLookAt(0,10,30,0,0,0,0,1,0);//����ͼ
	//gluLookAt(30,10,0,0,0,0,0,1,0);//����ͼ
	//gluLookAt(0,30,0,0,0,0,0,0,1);//����ͼ
	//-------------------------------------------------

	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient_color);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_color);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular_color);
	glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);
	glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);//mat_emission

	g_3dsModel[0].Drawbody();//��ʾ����
	
	
	//glPointSize(8);
	//glBegin(GL_POINTS);
	//glColor3f(0.0,1.0,0.0); //��Сֵ������ɫΪ��ɫ
	//glVertex3f(-0.0830257013, -0.962369919 ,-0.360967964 );
	//glVertex3f(-1.45875549, -0.961950302, 1.28398442);
	//glEnd();

//	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);

//	g_3dsModel[0].Drawclothpointbypoint();
	g_3dsModel[0].Drawcloth_V3();
//	g_3dsModel[0].Drawcloth(0);//��ʾ����


	glDisable(GL_LIGHTING);

	//m_MSModel.StepSimulation(&(g_3dsModel[0].thisphysic),g_3dsModel[0].m_3DModel,g_3dsModel[0].sk_chunk,pimo,0.050f);

	//----------------------------------������


	//glEnable(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	//glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	//glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);
	//glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);//mat_emission
	//g_3dsModel[1].Drawcloth(1);

	//m_MSModel.StepSimulation(&(g_3dsModel[1].thisphysic),g_3dsModel[1].m_3DModel,g_3dsModel[1].sk_chunk,pimo,0.050f);
	
	SwapBuffers(g_hDC);									// Swap the backbuffers to the foreground
}