#include "stdafx.h"
#include <math.h>
#include "resource.h"
#include <iostream>
#include <windows.h>  
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "Bspline_fitting.h"
using namespace std;
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#include "3dsModel.h"
#define SCREEN_WIDTH 800				//窗口默认宽度				
#define SCREEN_HEIGHT 600				//窗口默认高度				
#define SCREEN_DEPTH 16					//窗口默认像素值			
const float pi = 3.1415926f;            
double time;							//测试程序运行时间的变量
ofstream outStream;						//用于输出程序运行时间信息
int per;

//程序路径
char g_sAppPath[512];
//模型路径
char g_sMediaPath[512];
//纹理号表（100个）
UINT g_Texture[100] = {0};						
//全局参数
bool  g_bFullScreen = true;	
UINT g_ViewMode;							
BOOL g_bLighting=FALSE;
HWND  g_hWnd;											
RECT  g_rRect;											
HDC   g_hDC;											
HGLRC g_hRC;											
HINSTANCE g_hInstance;								
float g_fEyeX=0;						//glulookat参数初始化设定
float g_fEyeY=0;
float g_fEyeZ=10;
float g_fEyeX1=0;
float g_fEyeY1=0;
float g_fEyeZ1=0;
float s_angle = -90.0;
Bspline_fitting Bsplin_fit;				//Bspaline_fitting对象，用于型值点拟合
const int m0=10;						//四肢uv方向型值点数量
int dim1=10;
int dim2=10;
int crlp;
double* p;								//存储返回的控制点
float ctrlPoint14[6][6][3];
GLfloat ctrlPoint[9][9][3];				//身体正面控制点
GLfloat ctrlPoint1[9][9][3];			//身体背面控制点
//GLfloat ctrlPoint2[6][6][3];			//头部正面控制点
//GLfloat ctrlPoint3[6][6][3];			//头部背面控制点
GLfloat ctrlPoint4[6][6][3];			//左大腿正面
GLfloat ctrlPoint5[6][6][3];			//左大腿背面
GLfloat ctrlPoint6[6][6][3];			//左小腿正面
GLfloat ctrlPoint7[6][6][3];			//左小腿背面
GLfloat ctrlPoint8[6][6][3];			//右大腿正面
GLfloat ctrlPoint9[6][6][3];			//右大腿背面
GLfloat ctrlPoint10[6][6][3];			//右小腿正面
GLfloat ctrlPoint11[6][6][3];			//右小腿背面
GLfloat ctrlPointys[6][6][3];			//右上臂正面
GLfloat ctrlPointysz[6][6][3];			//右上臂背面
GLfloat ctrlPointyb[6][6][3];			//右小臂正面
GLfloat ctrlPointybz[6][6][3];			//右小臂背面
GLfloat ctrlPointzs[6][6][3];			//左上臂正面
GLfloat ctrlPointzsz[6][6][3];			//左上臂背面
GLfloat ctrlPointzb[6][6][3];			//左小臂正面
GLfloat ctrlPointzbz[6][6][3];			//左小臂背面
int i,j,k;
/*   身体部分初始化参数   */
	double boo1[3]={0,62,-2.6};			//身体上面中心点
	double boo2[3]={0,38,-2.6};			//身体下面中心点
	double bor1=7.5;					//身体正面肩部半径
	double bor2=7.8;					//身体正面胯部半径
	double bor1to2=5.3;					//身体正面胸部半径
	double bor2to1=5.3;					//身体正面腰部半径
	double bor3=2.6;					//身体前侧面肩部半径					
	double bor4=7.0;					//身体前侧面胸部半径
	double bor5=8.0;					//身体前侧面腰部半径
	double bor6=6.6;					//身体前侧面胯部半径
	double bor7=-1.4;					//身体后侧面肩部半径
	double bor8=-3.5;					//身体后侧面胸部半径
	double bor9=-2.2;					//身体后侧面腰部半径
	double bor10=-1.4;					//身体后侧面胯部半径
	double xbrj=3.2;					//乳尖距身体距离3.88
	double xbrg=0.0;					//乳沟距身体距离，默认为0
	double tbtj=-3.23;
	double tbtg=-0.0;

	double bointputx[19][19];			//身体前半部分型值点坐标
    double bointputy[19][19];
	double bointputz[19][19];

	double bointputx1[19][19];			//身体后半部分型值点坐标
    double bointputy1[19][19];
	double bointputz1[19][19];

/*	头部head参数初始化设定	*/
//	导入外部头部模型，暂时不需要
/*	double heo1[3]={0,71,0.0};
	double heo2[3]={0,62,0.0};
	double her1=0.5;
	double her2=2.4;
	double her3=2.2;
	double her4=2.0;
	double her5=0.4;
	double her6=4.8;
	double her7=5.0;
	double her8=3.6;
	double her9=-0.4;
	double her10=-3.9;
	double her11=-3.6;
	double her12=-3.8;
 	double heintputx[10][10];			//头部前半部分型值点
    double heintputy[10][10];
	double heintputz[10][10];
	double heintputx1[10][10];			//头部后半部分型值点
    double heintputy1[10][10];
	double heintputz1[10][10];
*/

/*	左大腿zd参数初始化设定	*/
	double zdo1[3]={-3.9,38,-0.7};		//左大腿上中心点
	double zdo2[3]={-3.9,20,2.4};		//左大腿下中心点
	double zdr1=3.5;					//左大腿上半径
	double zdr2=2.4;					//左大腿下半径
	double zdr3=-3.3;					//左大腿侧面上半径
	double zdr4=-2.9;					//左大腿侧面下半径
	double zdr5=3.3;					//左大腿侧面（同上）（另一侧）
	double zdr6=2.9;
 	double zdintputx[10][10];			//左大腿正面型值点
    double zdintputy[10][10];
	double zdintputz[10][10];
	double zdintputx1[10][10];			//左大腿背面型值点
    double zdintputy1[10][10];
	double zdintputz1[10][10];
/*	左小腿zx参数初始化设定	*/
	double zxo1[3]={-3.9,20,2.4};		//左小腿上中心点
	double zxo2[3]={-3.9,2,-0.7};		//左小腿下中心点
	double zxr1=2.4;					//左小腿上半径
	double zxr2=2.4;					//左小腿下半径
	double zxr3=-2.9;					//左小腿侧面上半径
	double zxr4=-2.7;					//左小腿侧面下半径
	double zxr5=2.9;					//左小腿侧面（同上）（另一侧）
	double zxr6=2.7;
 	double zxintputx[10][10];			//左小腿正面型值点
    double zxintputy[10][10];
	double zxintputz[10][10];
	double zxintputx1[10][10];			//左小腿背面型值点
    double zxintputy1[10][10];
	double zxintputz1[10][10];

/*	右大腿yd参数初始化设定	*/
	double ydo1[3]={3.9,38,-0.7};		//右大腿上中心点
	double ydo2[3]={3.9,20,2.4};		//右大腿下中心点
	double ydr1=3.5;					//右大腿上半径
	double ydr2=2.4;					//右大腿下半径
	double ydr3=-3.3;					//右大腿侧面上半径
	double ydr4=-2.9;					//右大腿侧面下半径
	double ydr5=3.3;					//右大腿侧面（同上）（另一侧）
	double ydr6=2.9;
 	double ydintputx[10][10];			//右大腿正面型值点
    double ydintputy[10][10];
	double ydintputz[10][10];
	double ydintputx1[10][10];			//右大腿背面型值点
    double ydintputy1[10][10];
	double ydintputz1[10][10];

/*	右小腿yx参数初始化设定	*/
	double yxo1[3]={3.9,20,2.4};		//右小腿上中心点
	double yxo2[3]={3.9,2,-0.7};		//右小腿下中心点
	double yxr1=2.4;					//右小腿上半径
	double yxr2=2.4;					//右小腿下半径
	double yxr3=-2.9;					//右小腿侧面上半径
	double yxr4=-2.7;					//右小腿侧面下半径
	double yxr5=2.9;					//右小腿侧面（同上）（另一侧）
	double yxr6=2.7;
 	double yxintputx[10][10];			//右小腿正面型值点
    double yxintputy[10][10];
	double yxintputz[10][10];
	double yxintputx1[10][10];			//右小腿背面型值点
    double yxintputy1[10][10];
	double yxintputz1[10][10];

/*	右上臂ys参数初始化设定	*/
	double yso1[3]={7.0,60.0,-2.6};		//肩部中心点
	double yso22[3]={17.0,50.0,-2.6};	//肘部中心点
	double ysx= yso22[0] - yso1[0];
	double ysxx=ysx*ysx;
	double ysy = yso22[1] - yso1[1];
	double ysyy=ysy*ysy;
	double ysz = yso22[2] - yso1[2];
	double yszz=ysz*ysz;
	double last=pow((ysxx+ysyy+yszz),0.5);
	double yso2[3]={yso1[0],yso1[1]-last,yso1[2]};	//肩部不动，肘部旋转垂直水平面后中心点
	double ysr1=2.6;					//右上臂上半径
	double ysr2=2.1;					//右上臂下半径
	double ysr3=-2.9;					
	double ysr4=-2.5;
	double ysr5=2.9;
	double ysr6=2.5;
 	double ysintputx[10][10];
    double ysintputy[10][10];
	double ysintputz[10][10];
	double ysintputx1[10][10];
    double ysintputy1[10][10];
	double ysintputz1[10][10];
	//右小臂yb
		double ybo1[3]={17.0,50,-2.6};
	//double ybo22[3]={25.0,41.0,-2.6};
		double ybo22[3]={26.65,46,3.4};
	double ybx= ybo22[0] - ybo1[0];
	double ybxx=ybx*ybx;
	double yby = ybo22[1] - ybo1[1];
	double ybyy=yby*yby;
	double ybz = ybo22[2] - ybo1[2];
	double ybzz=ybz*ybz;
	double lastb=pow((ybxx+ybyy+ybzz),0.5);
	double ybo2[3]={ybo1[0],ybo1[1]-lastb,ybo1[2]};
	double ybr1=2.1;
	double ybr2=1.7;
	double ybr3=-2.5;
	double ybr4=-2.3;
	double ybr5=2.5;
	double ybr6=2.3;
 	double ybintputx[10][10];
    double ybintputy[10][10];
	double ybintputz[10][10];
	double ybintputx1[10][10];
    double ybintputy1[10][10];
	double ybintputz1[10][10];

	//左上臂ys
//	double yso1[3]={12.0,38.0,0.0};
//	double yso22[3]={21.0,22.5,0.0};
	double zso1[3]={-7.0,60.0,-2.6};
	double zso22[3]={-17.0,50.0,-2.6};
	double zsx= zso22[0] - zso1[0];
	double zsxx=zsx*zsx;
	double zsy = zso22[1] - zso1[1];
	double zsyy=zsy*zsy;
	double zsz = zso22[2] - zso1[2];
	double zszz=zsz*zsz;
	double zlast=pow((zsxx+zsyy+zszz),0.5);
	double zso2[3]={zso1[0],zso1[1]-zlast,zso1[2]};
	double zsr1=2.6;
	double zsr2=2.1;
	double zsr3=-2.9;
	double zsr4=-2.5;
	double zsr5=2.9;
	double zsr6=2.5;
 	double zsintputx[10][10];
    double zsintputy[10][10];
	double zsintputz[10][10];
	double zsintputx1[10][10];
    double zsintputy1[10][10];
	double zsintputz1[10][10];
	//右小臂yb
		double zbo1[3]={-17.0,50,-2.6};
	double zbo22[3]={-25.0,41.0,-2.6};
	double zbx= zbo22[0] - zbo1[0];
	double zbxx=zbx*zbx;
	double zby = zbo22[1] - zbo1[1];
	double zbyy=zby*zby;
	double zbz = zbo22[2] - zbo1[2];
	double zbzz=zbz*zbz;
	double zlastb=pow((zbxx+zbyy+zbzz),0.5);
	double zbo2[3]={zbo1[0],zbo1[1]-zlastb,zbo1[2]};
	double zbr1=2.1;
	double zbr2=1.7;
	double zbr3=-2.5;
	double zbr4=-2.3;
	double zbr5=2.5;
	double zbr6=2.3;
 	double zbintputx[10][10];
    double zbintputy[10][10];
	double zbintputz[10][10];
	double zbintputx1[10][10];
    double zbintputy1[10][10];
	double zbintputz1[10][10];

	//右手手指向量
	M3DVector3f yhand1={1.0f,-1.0f,0.0f};
	//右手手掌内侧法向量旋转角
	double anglehand1=30.0;
	//左手手指向量
	M3DVector3f zhand2={-1.0f,-1.0f,0.0f};
	//左手手掌内侧法向量旋转角
	double anglehand2=180.0;


double intputx[10][10];
double intputy[10][10];
double intputz[10][10];

//十个模型
C3DSModel g_3dsModel[10];


ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);
void ChangeToFullScreen();
WPARAM MainLoop();

void InitializeOpenGL(int width, int height);
void SizeOpenGLScreen(int width, int height);
bool bSetupPixelFormat(HDC hdc);

void Init(HWND hWnd);
void DeInit();

void RenderScene();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	HWND hWnd;
	//应用程序路径
	GetModuleFileName(hInstance,g_sAppPath,512);
	GetFilePath (g_sAppPath);
	//素材路径
	strcpy(g_sMediaPath,g_sAppPath);
	strcat(g_sMediaPath,"models\\");
	//是否使用全屏幕模式
	if(MessageBox(NULL, "是否使用全屏幕模式?", "窗口选择", MB_YESNO | MB_ICONQUESTION) == IDNO)
		g_bFullScreen = false;
	//生成窗口
	hWnd = CreateMyWindow("www.C3DN.net", SCREEN_WIDTH, SCREEN_HEIGHT, 0, g_bFullScreen, hInstance);
	// 如果没有成功，退出
	if(hWnd == NULL) return false;
	// 初始化OPENGL
	Init(hWnd);		
	void CALLBACK MoveNear(void);
void CALLBACK MoveFar(void);
	// 主循环
	return MainLoop();
}
//生成窗口
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;
	// 注册窗口类
	MyRegisterClass(hInstance);
	//全屏幕判断
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

//注册窗口类
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
//窗口消息处理函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	float aaa=g_fEyeX, bbb = g_fEyeZ;

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

	case WM_LBUTTONDOWN:								// If the left mouse button was clicked
		
		if(g_ViewMode == GL_TRIANGLES) {				// We our drawing mode is at triangles
			g_ViewMode = GL_LINE_STRIP;					// Go to line stips
		} else {
			g_ViewMode = GL_TRIANGLES;					// Go to triangles
		}
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
				PostQuitMessage(0);	
		    break;// Send a QUIT message to the window
			case VK_SPACE:	
				g_fEyeZ -= 0.3f;
				g_fEyeX1=g_fEyeX;
g_fEyeY1=g_fEyeY;
g_fEyeZ1=0;

            break;
			case VK_HOME:
				g_fEyeZ += 0.3f;
				break;
			case VK_LEFT:
	g_fEyeX=(float)(0.985*aaa+0.174*bbb);
		g_fEyeZ=(float)(-0.174*aaa+0.985*bbb);
				break;
			case 	VK_RIGHT:
		g_fEyeX=float(0.985*aaa-0.174*bbb);
		g_fEyeZ=float(0.174*aaa+0.985*bbb);
				break;
				case 	VK_UP:
				g_fEyeY += 0.3f;
g_fEyeY1=g_fEyeY;
				break;
	case 	VK_DOWN:
				g_fEyeY -= 0.3f;
g_fEyeY1=g_fEyeY;
				break;
	case VK_F1:
	g_fEyeY += 0.3f;
		break;

	case VK_F2:
	g_fEyeY -= 0.3f;
		break;
		case VK_F3:
	g_fEyeX += 0.3f;
		break;

	case VK_F4:
	g_fEyeX -= 0.3f;
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
// 关于对话框消息处理函数
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
//主循环
WPARAM MainLoop()
{
	MSG msg;

	while(1)											// Do our infinate loop
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
			RenderScene();								// Update the screen	
        } 
	}

	DeInit();											// Clean up and free all allocated memory
	return(msg.wParam);									// Return from the program
}

void Init(HWND hWnd)
{  //GLbyte *pBytes;
   // GLint iWidth, iHeight, iComponents;
    //GLenum eFormat;


    
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect
//	g_3dsModel[0].LoadModelFromFile("headgai.3ds");
	/*
	GLfloat ambientLight[]={1.0f,1.0f,1.0f,1.0f};

	
    GLfloat  whiteLight[] = {0.7f, 0.7f, 0.7f, 1.0f };
   // GLfloat  specular[] = { 0.7f, 0.7f, 0.7f, 1.0f};
   // GLfloat  shine[] = { 100.0f };
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  //  glEnable(GL_COLOR_MATERIAL);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);
	
 //   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,whiteLight);
  //  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
   // glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	*/
	//光照、颜色
    GLfloat mat_ambient[]={0,0.5,0,2};
	GLfloat mat_below[]={0,0.3,0,0};
	GLfloat mat_specular[]={1.0,1.0,1.0,1.0};
	GLfloat mat_shininess[]={50.0};
	GLfloat light_position[]={1.0,1.0,1.0,0.0};
	GLfloat model_ambient[]={0.5,0.5,0.5,1.0};
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_BACK,GL_AMBIENT,mat_below);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,model_ambient);
	 glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	
    glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	
/*	  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		    pBytes = gltLoadTGA("hand17.tga", &iWidth, &iHeight, &iComponents, &eFormat);
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
     free(pBytes);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	 
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_DECAL); */
	glDisable(GL_TEXTURE_2D);
	g_3dsModel[0].LoadModelFromFile("headgai.3ds");
 //  
}

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
				
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 1.0f ,800.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}
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

void RenderScene() 
{
	GLbyte *pBytes;
    GLint iWidth, iHeight, iComponents;
    GLenum eFormat;
 LARGE_INTEGER BegainTime ;   
    LARGE_INTEGER EndTime ;   
    LARGE_INTEGER Frequency ; 
	QueryPerformanceFrequency(&Frequency);   
    QueryPerformanceCounter(&BegainTime) ; 

		glClearColor(0.75f,0.75f,0.75f,1);
 
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix
	gluLookAt(g_fEyeX,g_fEyeY,g_fEyeZ,g_fEyeX1,g_fEyeY1,g_fEyeZ1,0,1,0);
//	glRotated(90.0,1.0,0.0,0.0);
	glScalef(0.10f,0.10f,0.10f);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
	GLfloat ku[10]={0.0f,0.0f,0.0f,0.0f,0.256f,0.62f,1.0f,1.0f,1.0f,1.0f};
	GLfloat kv[10]={0.0f,0.0f,0.0f,0.0f,0.262f,0.63f,1.0f,1.0f,1.0f,1.0f};
	GLfloat U1[19],V1[19];
	GLfloat U2[13],V2[13];

	GLfloat tpsgai3[9][9][2] = {{{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0}},
						   {{0,1},{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1}},
						   {{0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{6,2},{7,2},{8,2}},
                           {{0,3},{1,3},{2,3},{3,3},{4,3},{5,3},{6,3},{7,3},{8,3}},
						   {{0,4},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4},{7,4},{8,4}},
						   {{0,5},{1,5},{2,5},{3,5},{4,5},{5,5},{6,5},{7,5},{8,5}},
						   {{0,6},{1,6},{2,6},{3,6},{4,6},{5,6},{6,6},{7,6},{8,6}},
						   {{0,7},{1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7},{8,7}},
						   {{0,8},{1,8},{2,8},{3,8},{4,8},{5,8},{6,8},{7,8},{8,8}}
	
	};


	GLfloat tps[6][6][2] = {{{0,0},{1,0},{2,0},{3,0},{4,0},{5,0}},
						   {{0,1},{1,1},{2,1},{3,1},{4,1},{5,1}},
						   {{0,2},{1,2},{2,2},{3,2},{4,2},{5,2}},
                          {{0,3},{1,3},{2,3},{3,3},{4,3},{5,3}},
						   {{0,4},{1,4},{2,4},{3,4},{4,4},{5,4}},
						    {{0,5},{1,5},{2,5},{3,5},{4,5},{5,5}}};

	GLfloat tpsgai[15][15][2] = {{{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0}},
						   {{0,1},{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},{11,1},{12,1},{13,1},{14,1}},
						   {{0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{6,2},{7,2},{8,2},{9,2},{10,2},{11,2},{12,2},{13,2},{14,2}},
                           {{0,3},{1,3},{2,3},{3,3},{4,3},{5,3},{6,3},{7,3},{8,3},{9,3},{10,3},{11,3},{12,3},{13,3},{14,3}},
						   {{0,4},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4},{7,4},{8,4},{9,4},{10,4},{11,4},{12,4},{13,4},{14,4}},
						   {{0,5},{1,5},{2,5},{3,5},{4,5},{5,5},{6,5},{7,5},{8,5},{9,5},{10,5},{11,5},{12,5},{13,5},{14,5}},
						   {{0,6},{1,6},{2,6},{3,6},{4,6},{5,6},{6,6},{7,6},{8,6},{9,6},{10,6},{11,6},{12,6},{13,6},{14,6}},
						   {{0,7},{1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7},{8,7},{9,7},{10,7},{11,7},{12,7},{13,7},{14,7}},
						   {{0,8},{1,8},{2,8},{3,8},{4,8},{5,8},{6,8},{7,8},{8,8},{9,8},{10,8},{11,8},{12,8},{13,8},{14,8}},
						   {{0,9},{1,9},{2,9},{3,9},{4,9},{5,9},{6,9},{7,9},{8,9},{9,9},{10,9},{11,9},{12,9},{13,9},{14,9}},
						   {{0,10},{1,10},{2,10},{3,10},{4,10},{5,10},{6,10},{7,10},{8,10},{9,10},{10,10},{11,10},{12,10},{13,10},{14,10}},
						   {{0,11},{1,11},{2,11},{3,11},{4,11},{5,11},{6,11},{7,11},{8,11},{9,11},{10,11},{11,11},{12,11},{13,11},{14,11}},
						   {{0,12},{1,12},{2,12},{3,12},{4,12},{5,12},{6,12},{7,12},{8,12},{9,12},{10,12},{11,12},{12,12},{13,12},{14,12}},
						   {{0,13},{1,13},{2,13},{3,13},{4,13},{5,13},{6,13},{7,13},{8,13},{9,13},{10,13},{11,13},{12,13},{13,13},{14,13}},
						   {{0,14},{1,14},{2,14},{3,14},{4,14},{5,14},{6,14},{7,14},{8,14},{9,14},{10,14},{11,14},{12,14},{13,14},{14,14}},
	
	
	
	};
	
//	glDisable(GL_TEXTURE_2D);
	  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		    pBytes = gltLoadTGA("head14.tga", &iWidth, &iHeight, &iComponents, &eFormat);
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	 
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE); 
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glTranslated(0.0,64.0,0.0);
	glScalef(0.033f,0.033f,0.033f);
	
	g_3dsModel[0].Draw();
	glPopMatrix();
	  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		    pBytes = gltLoadTGA("hand17.tga", &iWidth, &iHeight, &iComponents, &eFormat);
//	   pBytes = gltLoadTGA("xiong.tga", &iWidth, &iHeight, &iComponents, &eFormat);
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
//     free(pBytes);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	 
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_DECAL); 
	glEnable(GL_TEXTURE_2D);
	

GLUnurbsObj *surfName2;
surfName2 = gluNewNurbsRenderer ( );
gluNurbsProperty(surfName2,GLU_SAMPLING_TOLERANCE,50);
//gluNurbsProperty(surfName2,GLU_SAMPLING_TOLERANCE,1);
gluNurbsProperty(surfName2,GLU_DISPLAY_MODE,(GLfloat)GLU_FILL);
/*
gluBeginSurface (surfName2);
//gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint2[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);
glEnable(GL_MAP2_VERTEX_3);

gluBeginSurface (surfName2);
//gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint3[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);

*/
//身体正面19*19 正常
//y
/*bointputy[0][0]=boo1[1];
bointputy[18][0]=boo2[1];
for(i=1;i<18;i++)
{
	bointputy[i][0]=boo1[1]-(boo1[1]-boo2[1])*i/18;
}

for(i=0;i<=18;i++)
{
	for(j=1;j<=18;j++)
	{
		bointputy[i][j]=bointputy[i][0];
	}
	
}
//x
bointputx[0][0]=boo1[0]+bor1;
bointputx[0][18]=boo1[0]-bor1;
bointputx[6][0]=(boo2[0]-boo1[0])/3+boo1[0]+bor1to2;
bointputx[6][18]=(boo2[0]-boo1[0])/3+boo1[0]-bor1to2;
bointputx[12][0]=(boo2[0]-boo1[0])*2/3+boo1[0]+bor2to1;
bointputx[12][18]=(boo2[0]-boo1[0])*2/3+boo1[0]-bor2to1;
bointputx[18][0]=boo2[0]+bor2;
bointputx[18][18]=boo2[0]-bor2;
for(i=1;i<6;i++)
{
	bointputx[i][0]=(bointputx[6][0]-bointputx[0][0])*i/6+bointputx[0][0];
	bointputx[i][18]=(bointputx[6][18]-bointputx[0][18])*i/6+bointputx[0][18];
}

for(i=7;i<12;i++)
{
	bointputx[i][0]=(bointputx[12][0]-bointputx[6][0])*(i-6)/6+bointputx[6][0];
	bointputx[i][18]=(bointputx[12][18]-bointputx[6][18])*(i-6)/6+bointputx[6][18];
}

//for(i=7;i<9;i++)
//{
	//bointputx[i][0]=(bointputx[9][0]-bointputx[6][0])*(i-6)/3+bointputx[6][0];
	//bointputx[i][9]=(bointputx[9][9]-bointputx[6][9])*(i-6)/3+bointputx[6][9];
	bointputx[13][0]=(bointputx[18][0]-bointputx[12][0])/3+bointputx[12][0];
	bointputx[13][18]=(bointputx[18][18]-bointputx[12][18])/3+bointputx[12][18];
	bointputx[14][0]=(bointputx[18][0]-bointputx[12][0])*5/9+bointputx[12][0];
	bointputx[14][18]=(bointputx[18][18]-bointputx[12][18])*5/9+bointputx[12][18];
	bointputx[15][0]=(bointputx[18][0]-bointputx[12][0])*6/9+bointputx[12][0];
	bointputx[15][18]=(bointputx[18][18]-bointputx[12][18])*6/9+bointputx[12][18];
	bointputx[16][0]=(bointputx[18][0]-bointputx[12][0])*7/9+bointputx[12][0];
	bointputx[16][18]=(bointputx[18][18]-bointputx[12][18])*7/9+bointputx[12][18];
	bointputx[17][0]=(bointputx[18][0]-bointputx[12][0])*8/9+bointputx[12][0];
	bointputx[17][18]=(bointputx[18][18]-bointputx[12][18])*8/9+bointputx[12][18];
	
//}

for(i=0;i<=18;i++)
{
	for(j=1;j<18;j++)
	{
		bointputx[i][j]=(bointputx[i][18]-bointputx[i][0])*j/18+bointputx[i][0];
	}
}

//z
bointputz[0][0]=boo1[2];
bointputz[0][18]=boo1[2];
bointputz[18][0]=boo2[2];
bointputz[18][18]=boo2[2];
bointputz[0][9]=boo1[2]+bor3;
bointputz[0][9]=boo1[2]+bor3;
bointputz[18][9]=boo2[2]+bor6;
bointputz[18][9]=boo2[2]+bor6;
for(i=1;i<18;i++)
{
	bointputz[i][0]=bointputz[0][0]-(bointputz[0][0]-bointputz[18][0])*i/18;
	bointputz[i][18]=bointputz[0][18]-(bointputz[0][18]-bointputz[18][18])*i/18;
}
bointputz[6][9]=(bointputz[6][18]+bointputz[6][0])/2+bor4;
bointputz[12][9]=(bointputz[12][18]+bointputz[12][0])/2+bor5;


bointputz[1][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*10/23;
bointputz[2][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*16/23;
bointputz[3][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*19/23;
bointputz[4][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*21/23;
bointputz[5][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*22/23;


bointputz[7][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*1/6;
bointputz[8][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*2/6;
bointputz[9][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*3/6;
bointputz[10][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*4/6;
bointputz[11][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*5/6;

bointputz[13][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*1/23;
bointputz[14][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*2/23;
bointputz[15][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*4/23;
bointputz[16][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*7/23;
bointputz[17][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*13/23;




for(i=0;i<19;i++)
{
	bointputz[i][1]=(bointputz[i][9]-bointputz[i][0])/2+bointputz[i][0];
	bointputz[i][2]=(bointputz[i][9]-bointputz[i][0])*21/32+bointputz[i][0];
	bointputz[i][3]=(bointputz[i][9]-bointputz[i][0])*25/32+bointputz[i][0];
	bointputz[i][4]=(bointputz[i][9]-bointputz[i][0])*28/32+bointputz[i][0];
	bointputz[i][5]=(bointputz[i][9]-bointputz[i][0])*29/32+bointputz[i][0];
	bointputz[i][6]=(bointputz[i][9]-bointputz[i][0])*30/32+bointputz[i][0];
	bointputz[i][7]=(bointputz[i][9]-bointputz[i][0])*30.5/32+bointputz[i][0];
	bointputz[i][8]=(bointputz[i][9]-bointputz[i][0])*31/32+bointputz[i][0];
	
	bointputz[i][17]=(bointputz[i][9]-bointputz[i][18])/2+bointputz[i][18];
	bointputz[i][16]=(bointputz[i][9]-bointputz[i][18])*21/32+bointputz[i][18];
	bointputz[i][15]=(bointputz[i][9]-bointputz[i][18])*25/32+bointputz[i][18];
	bointputz[i][14]=(bointputz[i][9]-bointputz[i][18])*28/32+bointputz[i][18];
	bointputz[i][13]=(bointputz[i][9]-bointputz[i][18])*29/32+bointputz[i][18];
	bointputz[i][12]=(bointputz[i][9]-bointputz[i][18])*30/32+bointputz[i][18];
	bointputz[i][11]=(bointputz[i][9]-bointputz[i][18])*30.5/32+bointputz[i][18];
	bointputz[i][10]=(bointputz[i][9]-bointputz[i][18])*31/32+bointputz[i][18];
	
}

p=Bsplin_fit.GlobalSurApproxFixednmgai (19,19,bointputx,bointputy,bointputz);
crlp=0;
for(i=0;i<15;i++)
{
	for(j=0;j<15;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}

for(i=0;i<19;i++)
{
	U1[i]=(float)Bsplin_fit.U[i];
}
for(i=0;i<19;i++)
{
	V1[i]=(float)Bsplin_fit.V[i];
}

gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,19,U1,19,V1,2,30,&tpsgai[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 19, U1, 
 19, V1, 3, 45, 
 &ctrlPoint[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);
*/

	//身体正面 胸部造型
	//身体正面19*19
//y
bointputy[0][0]=boo1[1];
bointputy[18][0]=boo2[1];
for(i=1;i<18;i++)
{
	bointputy[i][0]=boo1[1]-(boo1[1]-boo2[1])*i/18;
}

for(i=0;i<=18;i++)
{
	for(j=1;j<=18;j++)
	{
		bointputy[i][j]=bointputy[i][0];
	}
	
}
//x
bointputx[0][0]=boo1[0]+bor1;
bointputx[0][18]=boo1[0]-bor1;
bointputx[6][0]=(boo2[0]-boo1[0])/3+boo1[0]+bor1to2;
bointputx[6][18]=(boo2[0]-boo1[0])/3+boo1[0]-bor1to2;
bointputx[12][0]=(boo2[0]-boo1[0])*2/3+boo1[0]+bor2to1;
bointputx[12][18]=(boo2[0]-boo1[0])*2/3+boo1[0]-bor2to1;
bointputx[18][0]=boo2[0]+bor2;
bointputx[18][18]=boo2[0]-bor2;
for(i=1;i<6;i++)
{
	bointputx[i][0]=(bointputx[6][0]-bointputx[0][0])*i/6+bointputx[0][0];
	bointputx[i][18]=(bointputx[6][18]-bointputx[0][18])*i/6+bointputx[0][18];
}

for(i=7;i<12;i++)
{
	bointputx[i][0]=(bointputx[12][0]-bointputx[6][0])*(i-6)/6+bointputx[6][0];
	bointputx[i][18]=(bointputx[12][18]-bointputx[6][18])*(i-6)/6+bointputx[6][18];
}

//for(i=7;i<9;i++)
//{
	//bointputx[i][0]=(bointputx[9][0]-bointputx[6][0])*(i-6)/3+bointputx[6][0];
	//bointputx[i][9]=(bointputx[9][9]-bointputx[6][9])*(i-6)/3+bointputx[6][9];
	bointputx[13][0]=(bointputx[18][0]-bointputx[12][0])/3+bointputx[12][0];
	bointputx[13][18]=(bointputx[18][18]-bointputx[12][18])/3+bointputx[12][18];
	bointputx[14][0]=(bointputx[18][0]-bointputx[12][0])*5/9+bointputx[12][0];
	bointputx[14][18]=(bointputx[18][18]-bointputx[12][18])*5/9+bointputx[12][18];
	bointputx[15][0]=(bointputx[18][0]-bointputx[12][0])*6/9+bointputx[12][0];
	bointputx[15][18]=(bointputx[18][18]-bointputx[12][18])*6/9+bointputx[12][18];
	bointputx[16][0]=(bointputx[18][0]-bointputx[12][0])*7/9+bointputx[12][0];
	bointputx[16][18]=(bointputx[18][18]-bointputx[12][18])*7/9+bointputx[12][18];
	bointputx[17][0]=(bointputx[18][0]-bointputx[12][0])*8/9+bointputx[12][0];
	bointputx[17][18]=(bointputx[18][18]-bointputx[12][18])*8/9+bointputx[12][18];
	
//}

for(i=0;i<=18;i++)
{
	for(j=1;j<18;j++)
	{
		bointputx[i][j]=(bointputx[i][18]-bointputx[i][0])*j/18+bointputx[i][0];
	}
}
bointputx[18][9]=0;
bointputx[1][9]=0;
//z
bointputz[0][0]=boo1[2];
bointputz[0][18]=boo1[2];
bointputz[18][0]=boo2[2];
bointputz[18][18]=boo2[2];
bointputz[0][9]=boo1[2]+bor3;
bointputz[0][9]=boo1[2]+bor3;
bointputz[18][9]=boo2[2]+bor6;
bointputz[18][9]=boo2[2]+bor6;
for(i=1;i<18;i++)
{
	bointputz[i][0]=bointputz[0][0]-(bointputz[0][0]-bointputz[18][0])*i/18;
	bointputz[i][18]=bointputz[0][18]-(bointputz[0][18]-bointputz[18][18])*i/18;
}
bointputz[6][9]=(bointputz[6][18]+bointputz[6][0])/2+bor4;
bointputz[12][9]=(bointputz[12][18]+bointputz[12][0])/2+bor5;


bointputz[1][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*10/23;
bointputz[2][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*16/23;
bointputz[3][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*19/23;
bointputz[4][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*21/23;
bointputz[5][9]=bointputz[0][9]+(bointputz[6][9]-bointputz[0][9])*22/23;


bointputz[7][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*1/6;
bointputz[8][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*2/6;
bointputz[9][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*3/6;
bointputz[10][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*4/6;
bointputz[11][9]=bointputz[6][9]+(bointputz[12][9]-bointputz[6][9])*5/6;

bointputz[13][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*1/23;
bointputz[14][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*2/23;
bointputz[15][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*4/23;
bointputz[16][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*7/23;
bointputz[17][9]=bointputz[12][9]+(bointputz[18][9]-bointputz[12][9])*13/23;




for(i=0;i<19;i++)
{
	bointputz[i][1]=(bointputz[i][9]-bointputz[i][0])/2+bointputz[i][0];
	bointputz[i][2]=(bointputz[i][9]-bointputz[i][0])*21/32+bointputz[i][0];
	bointputz[i][3]=(bointputz[i][9]-bointputz[i][0])*25/32+bointputz[i][0];
	bointputz[i][4]=(bointputz[i][9]-bointputz[i][0])*28/32+bointputz[i][0];
	bointputz[i][5]=(bointputz[i][9]-bointputz[i][0])*29/32+bointputz[i][0];
	bointputz[i][6]=(bointputz[i][9]-bointputz[i][0])*30/32+bointputz[i][0];
	bointputz[i][7]=(bointputz[i][9]-bointputz[i][0])*30.5/32+bointputz[i][0];
	bointputz[i][8]=(bointputz[i][9]-bointputz[i][0])*31/32+bointputz[i][0];
	
	bointputz[i][17]=(bointputz[i][9]-bointputz[i][18])/2+bointputz[i][18];
	bointputz[i][16]=(bointputz[i][9]-bointputz[i][18])*21/32+bointputz[i][18];
	bointputz[i][15]=(bointputz[i][9]-bointputz[i][18])*25/32+bointputz[i][18];
	bointputz[i][14]=(bointputz[i][9]-bointputz[i][18])*28/32+bointputz[i][18];
	bointputz[i][13]=(bointputz[i][9]-bointputz[i][18])*29/32+bointputz[i][18];
	bointputz[i][12]=(bointputz[i][9]-bointputz[i][18])*30/32+bointputz[i][18];
	bointputz[i][11]=(bointputz[i][9]-bointputz[i][18])*30.5/32+bointputz[i][18];
	bointputz[i][10]=(bointputz[i][9]-bointputz[i][18])*31/32+bointputz[i][18];
	
}
//初始设定值
/*
bointputz[6][1]=4.4;
bointputz[6][2]=6;
bointputz[6][3]=7.1;
bointputz[6][4]=7.4;
bointputz[6][5]=7.4;
bointputz[6][6]=6.9;
bointputz[6][7]=5.9;
bointputz[6][8]=4.9;
bointputz[6][10]=4.9;
bointputz[6][11]=5.9;
bointputz[6][12]=6.9;
bointputz[6][13]=7.4;
bointputz[6][14]=7.4;
bointputz[6][15]=7.1;
bointputz[6][16]=6;
bointputz[6][17]=4.4;
*/
bointputz[6][4]=bointputz[6][4]+xbrj;
bointputz[6][9]=bointputz[6][9]+xbrg;
bointputz[6][1]=(bointputz[6][4]-bointputz[6][0])*7.0/10+bointputz[6][0];
bointputz[6][2]=(bointputz[6][4]-bointputz[6][0])*8.6/10+bointputz[6][0];
bointputz[6][3]=(bointputz[6][4]-bointputz[6][0])*9.7/10+bointputz[6][0];
bointputz[6][5]=(bointputz[6][4]-bointputz[6][0])*9.9/10+bointputz[6][0];
bointputz[6][6]=(bointputz[6][4]-bointputz[6][0])*9.5/10+bointputz[6][0];
bointputz[6][7]=(bointputz[6][4]-bointputz[6][0])*8.5/10+bointputz[6][0];
bointputz[6][8]=(bointputz[6][4]-bointputz[6][0])*7.5/10+bointputz[6][0];

bointputz[6][10]=bointputz[6][8];
bointputz[6][11]=bointputz[6][7];
bointputz[6][12]=bointputz[6][6];
bointputz[6][13]=bointputz[6][5];
bointputz[6][14]=bointputz[6][4];
bointputz[6][15]=bointputz[6][3];
bointputz[6][16]=bointputz[6][2];
bointputz[6][17]=bointputz[6][1];
for(i=0;i<19;i++)
{
	bointputz[3][i]=(bointputz[6][i]-bointputz[2][i])*11/18+bointputz[2][i];
	bointputz[4][i]=(bointputz[6][i]-bointputz[2][i])*15/18+bointputz[2][i];
	bointputz[5][i]=(bointputz[6][i]-bointputz[2][i])*17/18+bointputz[2][i];

	bointputz[7][i]=(bointputz[6][i]-bointputz[9][i])*16/18+bointputz[9][i];
	bointputz[8][i]=(bointputz[6][i]-bointputz[9][i])*12/18+bointputz[9][i];
	
}
/*
for(i=0;i<19;i++)
	for(j=0;j<19;j++)
		bointputz[i][j]=0.0;
		*/

p=Bsplin_fit.GlobalSurApproxFixednmgai3(19,19,bointputx,bointputy,bointputz);
crlp=0;
for(i=0;i<9;i++)
{
	for(j=0;j<9;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}

for(i=0;i<13;i++)
{
	U2[i]=(float)Bsplin_fit.Ug[i];
}
for(i=0;i<13;i++)
{
	V2[i]=(float)Bsplin_fit.Vg[i];
}



gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,13,U2,13,V2,2,18,&tpsgai3[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 13, U2, 
 13, V2, 3, 27, 
 &ctrlPoint[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);
//身体背面
bointputy1[0][0]=boo1[1];
bointputy1[18][0]=boo2[1];
for(i=1;i<18;i++)
{
	bointputy1[i][0]=boo1[1]-(boo1[1]-boo2[1])*i/18;
}

for(i=0;i<=18;i++)
{
	for(j=1;j<=18;j++)
	{
		bointputy1[i][j]=bointputy1[i][0];
	}
	
}
//x

bointputx1[0][0]=boo1[0]-bor1;
bointputx1[0][18]=boo1[0]+bor1;
bointputx1[6][0]=(boo2[0]-boo1[0])/3+boo1[0]-bor1to2;
bointputx1[6][18]=(boo2[0]-boo1[0])/3+boo1[0]+bor1to2;
bointputx1[12][0]=(boo2[0]-boo1[0])*2/3+boo1[0]-bor2to1;
bointputx1[12][18]=(boo2[0]-boo1[0])*2/3+boo1[0]+bor2to1;
bointputx1[18][0]=boo2[0]-bor2;
bointputx1[18][18]=boo2[0]+bor2;
for(i=1;i<6;i++)
{
	bointputx1[i][0]=(bointputx1[6][0]-bointputx1[0][0])*i/6+bointputx1[0][0];
	bointputx1[i][18]=(bointputx1[6][18]-bointputx1[0][18])*i/6+bointputx1[0][18];
}

for(i=7;i<12;i++)
{
	bointputx1[i][0]=(bointputx1[12][0]-bointputx1[6][0])*(i-6)/6+bointputx1[6][0];
	bointputx1[i][18]=(bointputx1[12][18]-bointputx1[6][18])*(i-6)/6+bointputx1[6][18];
}

//for(i=7;i<9;i++)
//{
	//bointputx[i][0]=(bointputx[9][0]-bointputx[6][0])*(i-6)/3+bointputx[6][0];
	//bointputx[i][9]=(bointputx[9][9]-bointputx[6][9])*(i-6)/3+bointputx[6][9];
	bointputx1[13][0]=(bointputx1[18][0]-bointputx1[12][0])/3+bointputx1[12][0];
	bointputx1[13][18]=(bointputx1[18][18]-bointputx1[12][18])/3+bointputx1[12][18];
	bointputx1[14][0]=(bointputx1[18][0]-bointputx1[12][0])*5/9+bointputx1[12][0];
	bointputx1[14][18]=(bointputx1[18][18]-bointputx1[12][18])*5/9+bointputx1[12][18];
	bointputx1[15][0]=(bointputx1[18][0]-bointputx1[12][0])*6/9+bointputx1[12][0];
	bointputx1[15][18]=(bointputx1[18][18]-bointputx1[12][18])*6/9+bointputx1[12][18];
	bointputx1[16][0]=(bointputx1[18][0]-bointputx1[12][0])*7/9+bointputx1[12][0];
	bointputx1[16][18]=(bointputx1[18][18]-bointputx1[12][18])*7/9+bointputx1[12][18];
	bointputx1[17][0]=(bointputx1[18][0]-bointputx1[12][0])*8/9+bointputx1[12][0];
	bointputx1[17][18]=(bointputx1[18][18]-bointputx1[12][18])*8/9+bointputx1[12][18];
	
//}

for(i=0;i<=18;i++)
{
	for(j=1;j<18;j++)
	{
		bointputx1[i][j]=(bointputx1[i][18]-bointputx1[i][0])*j/18+bointputx1[i][0];
	}
}

//z
bointputz1[0][0]=boo1[2];
bointputz1[0][18]=boo1[2];
bointputz1[18][0]=boo2[2];
bointputz1[18][18]=boo2[2];
bointputz1[0][9]=boo1[2]+bor7;
bointputz1[18][9]=boo2[2]+bor10;
for(i=1;i<18;i++)
{
	bointputz1[i][0]=bointputz1[0][0]-(bointputz1[0][0]-bointputz1[18][0])*i/18;
	bointputz1[i][18]=bointputz1[0][18]-(bointputz1[0][18]-bointputz1[18][18])*i/18;
}
bointputz1[6][9]=(bointputz1[6][18]+bointputz1[6][0])/2+bor8;
bointputz1[12][9]=(bointputz1[12][18]+bointputz1[12][0])/2+bor9;


bointputz1[1][9]=bointputz1[0][9]+(bointputz1[6][9]-bointputz1[0][9])*10/23;
bointputz1[2][9]=bointputz1[0][9]+(bointputz1[6][9]-bointputz1[0][9])*16/23;
bointputz1[3][9]=bointputz1[0][9]+(bointputz1[6][9]-bointputz1[0][9])*19/23;
bointputz1[4][9]=bointputz1[0][9]+(bointputz1[6][9]-bointputz1[0][9])*21/23;
bointputz1[5][9]=bointputz1[0][9]+(bointputz1[6][9]-bointputz1[0][9])*22/23;


bointputz1[7][9]=bointputz1[6][9]+(bointputz1[12][9]-bointputz1[6][9])*1/6;
bointputz1[8][9]=bointputz1[6][9]+(bointputz1[12][9]-bointputz1[6][9])*2/6;
bointputz1[9][9]=bointputz1[6][9]+(bointputz1[12][9]-bointputz1[6][9])*3/6;
bointputz1[10][9]=bointputz1[6][9]+(bointputz1[12][9]-bointputz1[6][9])*4/6;
bointputz1[11][9]=bointputz1[6][9]+(bointputz1[12][9]-bointputz1[6][9])*5/6;

bointputz1[13][9]=bointputz1[12][9]+(bointputz1[18][9]-bointputz1[12][9])*1/23;
bointputz1[14][9]=bointputz1[12][9]+(bointputz1[18][9]-bointputz1[12][9])*2/23;
bointputz1[15][9]=bointputz1[12][9]+(bointputz1[18][9]-bointputz1[12][9])*4/23;
bointputz1[16][9]=bointputz1[12][9]+(bointputz1[18][9]-bointputz1[12][9])*7/23;
bointputz1[17][9]=bointputz1[12][9]+(bointputz1[18][9]-bointputz1[12][9])*13/23;




for(i=0;i<19;i++)
{
	bointputz1[i][1]=(bointputz1[i][9]-bointputz1[i][0])/2+bointputz1[i][0];
	bointputz1[i][2]=(bointputz1[i][9]-bointputz1[i][0])*21/32+bointputz1[i][0];
	bointputz1[i][3]=(bointputz1[i][9]-bointputz1[i][0])*25/32+bointputz1[i][0];
	bointputz1[i][4]=(bointputz1[i][9]-bointputz1[i][0])*28/32+bointputz1[i][0];
	bointputz1[i][5]=(bointputz1[i][9]-bointputz1[i][0])*29/32+bointputz1[i][0];
	bointputz1[i][6]=(bointputz1[i][9]-bointputz1[i][0])*30/32+bointputz1[i][0];
	bointputz1[i][7]=(bointputz1[i][9]-bointputz1[i][0])*30.5/32+bointputz1[i][0];
	bointputz1[i][8]=(bointputz1[i][9]-bointputz1[i][0])*31/32+bointputz1[i][0];
	
	bointputz1[i][17]=(bointputz1[i][9]-bointputz1[i][18])/2+bointputz1[i][18];
	bointputz1[i][16]=(bointputz1[i][9]-bointputz1[i][18])*21/32+bointputz1[i][18];
	bointputz1[i][15]=(bointputz1[i][9]-bointputz1[i][18])*25/32+bointputz1[i][18];
	bointputz1[i][14]=(bointputz1[i][9]-bointputz1[i][18])*28/32+bointputz1[i][18];
	bointputz1[i][13]=(bointputz1[i][9]-bointputz1[i][18])*29/32+bointputz1[i][18];
	bointputz1[i][12]=(bointputz1[i][9]-bointputz1[i][18])*30/32+bointputz1[i][18];
	bointputz1[i][11]=(bointputz1[i][9]-bointputz1[i][18])*30.5/32+bointputz1[i][18];
	bointputz1[i][10]=(bointputz1[i][9]-bointputz1[i][18])*31/32+bointputz1[i][18];
	
}
/*
//初始值
bointputz1[16][1]=-6.0;
bointputz1[16][2]=-6.9;
bointputz1[16][3]=-7.2;
bointputz1[16][4]=-7.4;
bointputz1[16][5]=-7.6;
bointputz1[16][6]=-7.5;
bointputz1[16][7]=-7.2;
bointputz1[16][8]=-6.7;
bointputz1[16][9]=-4.56;
bointputz1[16][10]=-6.7;
bointputz1[16][11]=-7.2;
bointputz1[16][12]=-7.5;
bointputz1[16][13]=-7.6;
bointputz1[16][14]=-7.4;
bointputz1[16][15]=-7.2;
bointputz1[16][16]=-6.9;
bointputz1[16][17]=-6.0;
*/

bointputz1[16][5]=bointputz1[16][5]+tbtj;
bointputz1[16][9]=bointputz1[16][9]+tbtg;
bointputz1[16][1]=(bointputz1[16][5]-bointputz1[16][0])*3.4/5+bointputz1[16][0];
bointputz1[16][2]=(bointputz1[16][5]-bointputz1[16][0])*4.3/5+bointputz1[16][0];
bointputz1[16][3]=(bointputz1[16][5]-bointputz1[16][0])*4.6/5+bointputz1[16][0];
bointputz1[16][4]=(bointputz1[16][5]-bointputz1[16][0])*4.8/5+bointputz1[16][0];

bointputz1[16][6]=(bointputz1[16][5]-bointputz1[16][9])*2.94/3.04+bointputz1[16][9];
bointputz1[16][7]=(bointputz1[16][5]-bointputz1[16][9])*2.64/3.04+bointputz1[16][9];
bointputz1[16][8]=(bointputz1[16][5]-bointputz1[16][9])*2.14/3.04+bointputz1[16][9];


bointputz1[16][10]=bointputz1[16][8];
bointputz1[16][11]=bointputz1[16][7];
bointputz1[16][12]=bointputz1[16][6];
bointputz1[16][13]=bointputz1[16][5];
bointputz1[16][14]=bointputz1[16][4];
bointputz1[16][15]=bointputz1[16][3];
bointputz1[16][16]=bointputz1[16][2];
bointputz1[16][17]=bointputz1[16][1];
/*for(i=0;i<19;i++)
{
bointputz1[18][]
}
*/
for(i=0;i<19;i++)
{
	bointputz1[13][i]=(bointputz1[14][i]-bointputz1[10][i])*3.5/18+bointputz1[12][i];
	bointputz1[14][i]=(bointputz1[14][i]-bointputz1[10][i])*9/18+bointputz1[12][i];
	bointputz1[15][i]=(bointputz1[14][i]-bointputz1[10][i])*13.5/18+bointputz1[12][i];
	
	bointputz1[17][i]=(bointputz1[14][i]-bointputz1[18][i])*15.5/18+bointputz[18][i];
	//bointputz1[17][i]=(bointputz1[14][i]-bointputz1[18][i])*3.5/18+bointputz[18][i];
	
}
p=Bsplin_fit.GlobalSurApproxFixednmgai3(19,19,bointputx1,bointputy1,bointputz1);

crlp=0;
for(i=0;i<9;i++)
{
	for(j=0;j<9;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint1[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}
ofstream outStream;
	outStream.open("line1.txt");
	for(i=0;i<=14;i++)
	{
		
		outStream.setf(ios::fixed);
		outStream.setf(ios::showpoint);
		outStream.precision(8);	
		outStream <<"{"<<ctrlPoint1[0][i][0] <<"," << ctrlPoint1[0][i][2] <<", "<<0.0<<"},"<<endl;
//	outStream<<endl;
	}
	

	
	//cin >> dim1;
	outStream.close();
for(i=0;i<13;i++)
{
	U2[i]=(float)Bsplin_fit.Ug[i];
}
for(i=0;i<13;i++)
{
	V2[i]=(float)Bsplin_fit.Vg[i];
}




gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,13,U2,13,V2,2,18,&tpsgai3[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 13, U2, 
 13, V2, 3, 27, 
 &ctrlPoint1[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);


//左大腿正面
//y
zdintputy[0][0]=zdo1[1];
zdintputy[9][0]=zdo2[1];
for(i=1;i<9;i++)
{
	zdintputy[i][0]=zdo1[1]-(zdo1[1]-zdo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		zdintputy[i][j]=zdintputy[i][0];
	}
	
}
//x
zdintputx[0][0]=zdo1[0]+zdr1;
zdintputx[0][9]=zdo1[0]-zdr1;
zdintputx[9][0]=zdo2[0]+zdr2;
zdintputx[9][9]=zdo2[0]-zdr2;
for(i=1;i<9;i++)
{
	zdintputx[i][0]=(zdintputx[9][0]-zdintputx[0][0])*i/9+zdintputx[0][0];
	zdintputx[i][9]=zdintputx[0][9]-(zdintputx[0][9]-zdintputx[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		zdintputx[i][j]=(zdintputx[i][9]-zdintputx[i][0])*j/9+zdintputx[i][0];
	}
}
//z
zdintputz[0][0]=zdo1[2];
zdintputz[0][9]=zdo1[2];
zdintputz[9][0]=zdo2[2];
zdintputz[9][9]=zdo2[2];
zdintputz[0][4]=zdo1[2]+zdr5;
zdintputz[0][5]=zdo1[2]+zdr5;
zdintputz[9][4]=zdo2[2]+zdr6;
zdintputz[9][5]=zdo2[2]+zdr6;
for(i=1;i<9;i++)
{
	zdintputz[i][0]=zdintputz[0][0]-(zdintputz[0][0]-zdintputz[9][0])*i/9;
	zdintputz[i][4]=zdintputz[0][4]-(zdintputz[0][4]-zdintputz[9][4])*i/9;
	zdintputz[i][5]=zdintputz[0][4]-(zdintputz[0][4]-zdintputz[9][4])*i/9;
	zdintputz[i][9]=zdintputz[0][9]-(zdintputz[0][9]-zdintputz[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	zdintputz[i][1]=(zdintputz[i][4]-zdintputz[i][0])/2+zdintputz[i][0];
	zdintputz[i][2]=(zdintputz[i][4]-zdintputz[i][1])/2+zdintputz[i][1];
	zdintputz[i][3]=(zdintputz[i][4]-zdintputz[i][2])/2+zdintputz[i][2];
	zdintputz[i][8]=(zdintputz[i][5]-zdintputz[i][9])/2+zdintputz[i][9];
	zdintputz[i][7]=(zdintputz[i][5]-zdintputz[i][8])/2+zdintputz[i][8];
	zdintputz[i][6]=(zdintputz[i][5]-zdintputz[i][7])/2+zdintputz[i][7];
}


p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,zdintputx,zdintputy,zdintputz);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint4[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}


gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint4[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);


//左大腿背面
//y
zdintputy1[0][0]=zdo1[1];
zdintputy1[9][0]=zdo2[1];
for(i=1;i<9;i++)
{
	zdintputy1[i][0]=zdo1[1]-(zdo1[1]-zdo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		zdintputy1[i][j]=zdintputy1[i][0];
	}
	
}
//x
zdintputx1[0][0]=zdo1[0]+zdr1;
zdintputx1[0][9]=zdo1[0]-zdr1;
zdintputx1[9][0]=zdo2[0]+zdr2;
zdintputx1[9][9]=zdo2[0]-zdr2;
for(i=1;i<9;i++)
{
	zdintputx1[i][0]=(zdintputx1[9][0]-zdintputx1[0][0])*i/9+zdintputx1[0][0];
	zdintputx1[i][9]=zdintputx1[0][9]-(zdintputx1[0][9]-zdintputx1[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		zdintputx1[i][j]=(zdintputx1[i][9]-zdintputx1[i][0])*j/9+zdintputx1[i][0];
	}
}
//z
zdintputz1[0][0]=zdo1[2];
zdintputz1[0][9]=zdo1[2];
zdintputz1[9][0]=zdo2[2];
zdintputz1[9][9]=zdo2[2];
zdintputz1[0][4]=zdo1[2]+zdr3;
zdintputz1[0][5]=zdo1[2]+zdr3;
zdintputz1[9][4]=zdo2[2]+zdr4;
zdintputz1[9][5]=zdo2[2]+zdr4;
for(i=1;i<9;i++)
{
	zdintputz1[i][0]=zdintputz1[0][0]-(zdintputz1[0][0]-zdintputz1[9][0])*i/9;
	zdintputz1[i][4]=zdintputz1[0][4]-(zdintputz1[0][4]-zdintputz1[9][4])*i/9;
	zdintputz1[i][5]=zdintputz1[0][4]-(zdintputz1[0][4]-zdintputz1[9][4])*i/9;
	zdintputz1[i][9]=zdintputz1[0][9]-(zdintputz1[0][9]-zdintputz1[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	zdintputz1[i][1]=(zdintputz1[i][4]-zdintputz1[i][0])/2+zdintputz1[i][0];
	zdintputz1[i][2]=(zdintputz1[i][4]-zdintputz1[i][1])/2+zdintputz1[i][1];
	zdintputz1[i][3]=(zdintputz1[i][4]-zdintputz1[i][2])/2+zdintputz1[i][2];
	zdintputz1[i][8]=(zdintputz1[i][5]-zdintputz1[i][9])/2+zdintputz1[i][9];
	zdintputz1[i][7]=(zdintputz1[i][5]-zdintputz1[i][8])/2+zdintputz1[i][8];
	zdintputz1[i][6]=(zdintputz1[i][5]-zdintputz1[i][7])/2+zdintputz1[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,zdintputx1,zdintputy1,zdintputz1);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=5;j>=0;j--)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint5[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}


gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint5[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);


//左小腿正面
//y
zxintputy[0][0]=zxo1[1];
zxintputy[9][0]=zxo2[1];
for(i=1;i<9;i++)
{
	zxintputy[i][0]=zxo1[1]-(zxo1[1]-zxo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		zxintputy[i][j]=zxintputy[i][0];
	}
	
}
//x
zxintputx[0][0]=zxo1[0]+zxr1;
zxintputx[0][9]=zxo1[0]-zxr1;
zxintputx[9][0]=zxo2[0]+zxr2;
zxintputx[9][9]=zxo2[0]-zxr2;
for(i=1;i<9;i++)
{
	zxintputx[i][0]=(zxintputx[9][0]-zxintputx[0][0])*i/9+zxintputx[0][0];
	zxintputx[i][9]=zxintputx[0][9]-(zxintputx[0][9]-zxintputx[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		zxintputx[i][j]=(zxintputx[i][9]-zxintputx[i][0])*j/9+zxintputx[i][0];
	}
}
//z
zxintputz[0][0]=zxo1[2];
zxintputz[0][9]=zxo1[2];
zxintputz[9][0]=zxo2[2];
zxintputz[9][9]=zxo2[2];
zxintputz[0][4]=zxo1[2]+zxr5;
zxintputz[0][5]=zxo1[2]+zxr5;
zxintputz[9][4]=zxo2[2]+zxr6;
zxintputz[9][5]=zxo2[2]+zxr6;
for(i=1;i<9;i++)
{
	zxintputz[i][0]=zxintputz[0][0]-(zxintputz[0][0]-zxintputz[9][0])*i/9;
	zxintputz[i][4]=zxintputz[0][4]-(zxintputz[0][4]-zxintputz[9][4])*i/9;
	zxintputz[i][5]=zxintputz[0][4]-(zxintputz[0][4]-zxintputz[9][4])*i/9;
	zxintputz[i][9]=zxintputz[0][9]-(zxintputz[0][9]-zxintputz[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	zxintputz[i][1]=(zxintputz[i][4]-zxintputz[i][0])/2+zxintputz[i][0];
	zxintputz[i][2]=(zxintputz[i][4]-zxintputz[i][1])/2+zxintputz[i][1];
	zxintputz[i][3]=(zxintputz[i][4]-zxintputz[i][2])/2+zxintputz[i][2];

	zxintputz[i][8]=(zxintputz[i][5]-zxintputz[i][9])/2+zxintputz[i][9];
	zxintputz[i][7]=(zxintputz[i][5]-zxintputz[i][8])/2+zxintputz[i][8];
	zxintputz[i][6]=(zxintputz[i][5]-zxintputz[i][7])/2+zxintputz[i][7];
}


p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,zxintputx,zxintputy,zxintputz);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint6[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}



gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint6[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);

//左小腿背面
//y
zxintputy1[0][0]=zxo1[1];
zxintputy1[9][0]=zxo2[1];
for(i=1;i<9;i++)
{
	zxintputy1[i][0]=zxo1[1]-(zxo1[1]-zxo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		zxintputy1[i][j]=zxintputy1[i][0];
	}
	
}
//x
zxintputx1[0][0]=zxo1[0]+zxr1;
zxintputx1[0][9]=zxo1[0]-zxr1;
zxintputx1[9][0]=zxo2[0]+zxr2;
zxintputx1[9][9]=zxo2[0]-zxr2;
for(i=1;i<9;i++)
{
	zxintputx1[i][0]=(zxintputx1[9][0]-zxintputx1[0][0])*i/9+zxintputx1[0][0];
	zxintputx1[i][9]=zxintputx1[0][9]-(zxintputx1[0][9]-zxintputx1[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		zxintputx1[i][j]=(zxintputx1[i][9]-zxintputx1[i][0])*j/9+zxintputx1[i][0];
	}
}
//z
zxintputz1[0][0]=zxo1[2];
zxintputz1[0][9]=zxo1[2];
zxintputz1[9][0]=zxo2[2];
zxintputz1[9][9]=zxo2[2];
zxintputz1[0][4]=zxo1[2]+zxr3;
zxintputz1[0][5]=zxo1[2]+zxr3;
zxintputz1[9][4]=zxo2[2]+zxr4;
zxintputz1[9][5]=zxo2[2]+zxr4;
for(i=1;i<9;i++)
{
	zxintputz1[i][0]=zxintputz1[0][0]-(zxintputz1[0][0]-zxintputz1[9][0])*i/9;
	zxintputz1[i][4]=zxintputz1[0][4]-(zxintputz1[0][4]-zxintputz1[9][4])*i/9;
	zxintputz1[i][5]=zxintputz1[0][4]-(zxintputz1[0][4]-zxintputz1[9][4])*i/9;
	zxintputz1[i][9]=zxintputz1[0][9]-(zxintputz1[0][9]-zxintputz1[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	zxintputz1[i][1]=(zxintputz1[i][4]-zxintputz1[i][0])/2+zxintputz1[i][0];
	zxintputz1[i][2]=(zxintputz1[i][4]-zxintputz1[i][1])/2+zxintputz1[i][1];
	zxintputz1[i][3]=(zxintputz1[i][4]-zxintputz1[i][2])/2+zxintputz1[i][2];

	zxintputz1[i][8]=(zxintputz1[i][5]-zxintputz1[i][9])/2+zxintputz1[i][9];
	zxintputz1[i][7]=(zxintputz1[i][5]-zxintputz1[i][8])/2+zxintputz1[i][8];
	zxintputz1[i][6]=(zxintputz1[i][5]-zxintputz1[i][7])/2+zxintputz1[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,zxintputx1,zxintputy1,zxintputz1);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=5;j>=0;j--)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint7[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}


gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint7[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);

//右大腿正面
//y
ydintputy1[0][0]=ydo1[1];
ydintputy1[9][0]=ydo2[1];
for(i=1;i<9;i++)
{
	ydintputy1[i][0]=ydo1[1]-(ydo1[1]-ydo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		ydintputy1[i][j]=ydintputy1[i][0];
	}
	
}
//x
ydintputx1[0][0]=ydo1[0]+ydr1;
ydintputx1[0][9]=ydo1[0]-ydr1;
ydintputx1[9][0]=ydo2[0]+ydr2;
ydintputx1[9][9]=ydo2[0]-ydr2;
for(i=1;i<9;i++)
{
	ydintputx1[i][0]=(ydintputx1[9][0]-ydintputx1[0][0])*i/9+ydintputx1[0][0];
	ydintputx1[i][9]=ydintputx1[0][9]-(ydintputx1[0][9]-ydintputx1[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		ydintputx1[i][j]=(ydintputx1[i][9]-ydintputx1[i][0])*j/9+ydintputx1[i][0];
	}
}
//z
ydintputz1[0][0]=ydo1[2];
ydintputz1[0][9]=ydo1[2];
ydintputz1[9][0]=ydo2[2];
ydintputz1[9][9]=ydo2[2];
ydintputz1[0][4]=ydo1[2]+ydr5;
ydintputz1[0][5]=ydo1[2]+ydr5;
ydintputz1[9][4]=ydo2[2]+ydr6;
ydintputz1[9][5]=ydo2[2]+ydr6;
for(i=1;i<9;i++)
{
	ydintputz1[i][0]=ydintputz1[0][0]-(ydintputz1[0][0]-ydintputz1[9][0])*i/9;
	ydintputz1[i][4]=ydintputz1[0][4]-(ydintputz1[0][4]-ydintputz1[9][4])*i/9;
	ydintputz1[i][5]=ydintputz1[0][4]-(ydintputz1[0][4]-ydintputz1[9][4])*i/9;
	ydintputz1[i][9]=ydintputz1[0][9]-(ydintputz1[0][9]-ydintputz1[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	ydintputz1[i][1]=(ydintputz1[i][4]-ydintputz1[i][0])/2+ydintputz1[i][0];
	ydintputz1[i][2]=(ydintputz1[i][4]-ydintputz1[i][1])/2+ydintputz1[i][1];
	ydintputz1[i][3]=(ydintputz1[i][4]-ydintputz1[i][2])/2+ydintputz1[i][2];

	ydintputz1[i][8]=(ydintputz1[i][5]-ydintputz1[i][9])/2+ydintputz1[i][9];
	ydintputz1[i][7]=(ydintputz1[i][5]-ydintputz1[i][8])/2+ydintputz1[i][8];
	ydintputz1[i][6]=(ydintputz1[i][5]-ydintputz1[i][7])/2+ydintputz1[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,ydintputx1,ydintputy1,ydintputz1);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=5;j>=0;j--)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint8[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}


gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint8[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);

//右大腿背面
//y
ydintputy[0][0]=ydo1[1];
ydintputy[9][0]=ydo2[1];
for(i=1;i<9;i++)
{
	ydintputy[i][0]=ydo1[1]-(ydo1[1]-ydo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		ydintputy[i][j]=ydintputy[i][0];
	}
	
}
//x
ydintputx[0][0]=ydo1[0]+ydr1;
ydintputx[0][9]=ydo1[0]-ydr1;
ydintputx[9][0]=ydo2[0]+ydr2;
ydintputx[9][9]=ydo2[0]-ydr2;
for(i=1;i<9;i++)
{
	ydintputx[i][0]=(ydintputx[9][0]-ydintputx[0][0])*i/9+ydintputx[0][0];
	ydintputx[i][9]=ydintputx[0][9]-(ydintputx[0][9]-ydintputx[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		ydintputx[i][j]=(ydintputx[i][9]-ydintputx[i][0])*j/9+ydintputx[i][0];
	}
}
//z
ydintputz[0][0]=ydo1[2];
ydintputz[0][9]=ydo1[2];
ydintputz[9][0]=ydo2[2];
ydintputz[9][9]=ydo2[2];
ydintputz[0][4]=ydo1[2]+ydr3;
ydintputz[0][5]=ydo1[2]+ydr3;
ydintputz[9][4]=ydo2[2]+ydr4;
ydintputz[9][5]=ydo2[2]+ydr4;
for(i=1;i<9;i++)
{
	ydintputz[i][0]=ydintputz[0][0]-(ydintputz[0][0]-ydintputz[9][0])*i/9;
	ydintputz[i][4]=ydintputz[0][4]-(ydintputz[0][4]-ydintputz[9][4])*i/9;
	ydintputz[i][5]=ydintputz[0][4]-(ydintputz[0][4]-ydintputz[9][4])*i/9;
	ydintputz[i][9]=ydintputz[0][9]-(ydintputz[0][9]-ydintputz[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	ydintputz[i][1]=(ydintputz[i][4]-ydintputz[i][0])/2+ydintputz[i][0];
	ydintputz[i][2]=(ydintputz[i][4]-ydintputz[i][1])/2+ydintputz[i][1];
	ydintputz[i][3]=(ydintputz[i][4]-ydintputz[i][2])/2+ydintputz[i][2];

	ydintputz[i][8]=(ydintputz[i][5]-ydintputz[i][9])/2+ydintputz[i][9];
	ydintputz[i][7]=(ydintputz[i][5]-ydintputz[i][8])/2+ydintputz[i][8];
	ydintputz[i][6]=(ydintputz[i][5]-ydintputz[i][7])/2+ydintputz[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,ydintputx,ydintputy,ydintputz);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint9[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}



gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint9[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);

//右小腿正面
//y
yxintputy1[0][0]=yxo1[1];
yxintputy1[9][0]=yxo2[1];
for(i=1;i<9;i++)
{
	yxintputy1[i][0]=yxo1[1]-(yxo1[1]-yxo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		yxintputy1[i][j]=yxintputy1[i][0];
	}
	
}
//x
yxintputx1[0][0]=yxo1[0]+yxr1;
yxintputx1[0][9]=yxo1[0]-yxr1;
yxintputx1[9][0]=yxo2[0]+yxr1;
yxintputx1[9][9]=yxo2[0]-yxr1;
for(i=1;i<9;i++)
{
	yxintputx1[i][0]=(yxintputx1[9][0]-yxintputx1[0][0])*i/9+yxintputx1[0][0];
	yxintputx1[i][9]=yxintputx1[0][9]-(yxintputx1[0][9]-yxintputx1[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		yxintputx1[i][j]=(yxintputx1[i][9]-yxintputx1[i][0])*j/9+yxintputx1[i][0];
	}
}
//z
yxintputz1[0][0]=yxo1[2];
yxintputz1[0][9]=yxo1[2];
yxintputz1[9][0]=yxo2[2];
yxintputz1[9][9]=yxo2[2];
yxintputz1[0][4]=yxo1[2]+yxr5;
yxintputz1[0][5]=yxo1[2]+yxr5;
yxintputz1[9][4]=yxo2[2]+yxr6;
yxintputz1[9][5]=yxo2[2]+yxr6;
for(i=1;i<9;i++)
{
	yxintputz1[i][0]=yxintputz1[0][0]-(yxintputz1[0][0]-yxintputz1[9][0])*i/9;
	yxintputz1[i][4]=yxintputz1[0][4]-(yxintputz1[0][4]-yxintputz1[9][4])*i/9;
	yxintputz1[i][5]=yxintputz1[0][4]-(yxintputz1[0][4]-yxintputz1[9][4])*i/9;
	yxintputz1[i][9]=yxintputz1[0][9]-(yxintputz1[0][9]-yxintputz1[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	yxintputz1[i][1]=(yxintputz1[i][4]-yxintputz1[i][0])/2+yxintputz1[i][0];
	yxintputz1[i][2]=(yxintputz1[i][4]-yxintputz1[i][1])/2+yxintputz1[i][1];
	yxintputz1[i][3]=(yxintputz1[i][4]-yxintputz1[i][2])/2+yxintputz1[i][2];

	yxintputz1[i][8]=(yxintputz1[i][5]-yxintputz1[i][9])/2+yxintputz1[i][9];
	yxintputz1[i][7]=(yxintputz1[i][5]-yxintputz1[i][8])/2+yxintputz1[i][8];
	yxintputz1[i][6]=(yxintputz1[i][5]-yxintputz1[i][7])/2+yxintputz1[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,yxintputx1,yxintputy1,yxintputz1);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=5;j>=0;j--)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint10[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}


gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint10[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);


//右小腿背面
//y
yxintputy[0][0]=yxo1[1];
yxintputy[9][0]=yxo2[1];
for(i=1;i<9;i++)
{
	yxintputy[i][0]=yxo1[1]-(yxo1[1]-yxo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		yxintputy[i][j]=yxintputy[i][0];
	}
	
}
//x
yxintputx[0][0]=yxo1[0]+yxr1;
yxintputx[0][9]=yxo1[0]-yxr1;
yxintputx[9][0]=yxo2[0]+yxr1;
yxintputx[9][9]=yxo2[0]-yxr1;
for(i=1;i<9;i++)
{
	yxintputx[i][0]=(yxintputx[9][0]-yxintputx[0][0])*i/9+yxintputx[0][0];
	yxintputx[i][9]=yxintputx[0][9]-(yxintputx[0][9]-yxintputx[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		yxintputx[i][j]=(yxintputx[i][9]-yxintputx[i][0])*j/9+yxintputx[i][0];
	}
}
//z
yxintputz[0][0]=yxo1[2];
yxintputz[0][9]=yxo1[2];
yxintputz[9][0]=yxo2[2];
yxintputz[9][9]=yxo2[2];
yxintputz[0][4]=yxo1[2]+yxr3;
yxintputz[0][5]=yxo1[2]+yxr3;
yxintputz[9][4]=yxo2[2]+yxr4;
yxintputz[9][5]=yxo2[2]+yxr4;
for(i=1;i<9;i++)
{
	yxintputz[i][0]=yxintputz[0][0]-(yxintputz[0][0]-yxintputz[9][0])*i/9;
	yxintputz[i][4]=yxintputz[0][4]-(yxintputz[0][4]-yxintputz[9][4])*i/9;
	yxintputz[i][5]=yxintputz[0][4]-(yxintputz[0][4]-yxintputz[9][4])*i/9;
	yxintputz[i][9]=yxintputz[0][9]-(yxintputz[0][9]-yxintputz[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	yxintputz[i][1]=(yxintputz[i][4]-yxintputz[i][0])/2+yxintputz[i][0];
	yxintputz[i][2]=(yxintputz[i][4]-yxintputz[i][1])/2+yxintputz[i][1];
	yxintputz[i][3]=(yxintputz[i][4]-yxintputz[i][2])/2+yxintputz[i][2];

	yxintputz[i][8]=(yxintputz[i][5]-yxintputz[i][9])/2+yxintputz[i][9];
	yxintputz[i][7]=(yxintputz[i][5]-yxintputz[i][8])/2+yxintputz[i][8];
	yxintputz[i][6]=(yxintputz[i][5]-yxintputz[i][7])/2+yxintputz[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,yxintputx,yxintputy,yxintputz);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPoint11[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}

gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPoint11[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);

//右上臂正面
//y
ysintputy1[0][0]=yso1[1];
ysintputy1[9][0]=yso2[1];
for(i=1;i<9;i++)
{
	ysintputy1[i][0]=yso1[1]-(yso1[1]-yso2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		ysintputy1[i][j]=ysintputy1[i][0];
	}
	
}
//x
ysintputx1[0][0]=yso1[0]+ysr1;
ysintputx1[0][9]=yso1[0]-ysr1;
ysintputx1[9][0]=yso2[0]+ysr2;
ysintputx1[9][9]=yso2[0]-ysr2;
for(i=1;i<9;i++)
{
	ysintputx1[i][0]=(ysintputx1[9][0]-ysintputx1[0][0])*i/9+ysintputx1[0][0];
	ysintputx1[i][9]=ysintputx1[0][9]-(ysintputx1[0][9]-ysintputx1[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		ysintputx1[i][j]=(ysintputx1[i][9]-ysintputx1[i][0])*j/9+ysintputx1[i][0];
	}
}
//z
ysintputz1[0][0]=yso1[2];
ysintputz1[0][9]=yso1[2];
ysintputz1[9][0]=yso2[2];
ysintputz1[9][9]=yso2[2];
ysintputz1[0][4]=yso1[2]+ysr5;
ysintputz1[0][5]=yso1[2]+ysr5;
ysintputz1[9][4]=yso2[2]+ysr6;
ysintputz1[9][5]=yso2[2]+ysr6;
for(i=1;i<9;i++)
{
	ysintputz1[i][0]=ysintputz1[0][0]-(ysintputz1[0][0]-ysintputz1[9][0])*i/9;
	ysintputz1[i][4]=ysintputz1[0][4]-(ysintputz1[0][4]-ysintputz1[9][4])*i/9;
	ysintputz1[i][5]=ysintputz1[0][4]-(ysintputz1[0][4]-ysintputz1[9][4])*i/9;
	ysintputz1[i][9]=ysintputz1[0][9]-(ysintputz1[0][9]-ysintputz1[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	ysintputz1[i][1]=(ysintputz1[i][4]-ysintputz1[i][0])/2+ysintputz1[i][0];
	ysintputz1[i][2]=(ysintputz1[i][4]-ysintputz1[i][1])/2+ysintputz1[i][1];
	ysintputz1[i][3]=(ysintputz1[i][4]-ysintputz1[i][2])/2+ysintputz1[i][2];
	ysintputz1[i][8]=(ysintputz1[i][5]-ysintputz1[i][9])/2+ysintputz1[i][9];
	ysintputz1[i][7]=(ysintputz1[i][5]-ysintputz1[i][8])/2+ysintputz1[i][8];
	ysintputz1[i][6]=(ysintputz1[i][5]-ysintputz1[i][7])/2+ysintputz1[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,ysintputx1,ysintputy1,ysintputz1);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=5;j>=0;j--)
	{
		for(k=0;k<3;k++)
		{
			ctrlPointysz[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}
glPushMatrix();
glTranslatef((float)yso1[0],(float)yso1[1],(float)yso1[2]);
//glRotatef(30.0f,0.0f,0.0f,1.0f);
M3DVector3f uu={(float)(yso22[0]-yso1[0]),(float)(yso22[1]-yso1[1]),(float)(yso22[2]-yso1[2])};
M3DVector3f vv={0.0f,-1.0f,0.0f};
M3DVector3f result;
m3dCrossProduct(result,vv,uu);
//M3DVector3d uu={9.0,-15.5,0.0};
//M3DVector3d vv={0.0,-10.0,0.0};
//M3DVector3d uu={0.0,-1.0,0.0};
//M3DVector3d vv={1.0,-1.0,0.0};
double allu=sqrt(uu[1]*uu[1] + uu[2]*uu[2]+ uu[0]*uu[0]);
double allv=sqrt(vv[1]*vv[1] + vv[2]*vv[2]+ vv[0]*vv[0]);
uu[0]/=(float)allu;
uu[1]/=(float)allu;
uu[2]/=(float)allu;
vv[0]/=(float)allv;
vv[1]/=(float)allv;
vv[2]/=(float)allv;

double angle=m3dGetAngleBetweenVectors( uu,vv);
double angle1=angle*180/PI;
glRotatef((float)angle1,result[0],result[1],result[2]);
glTranslatef((float)-yso1[0],(float)-yso1[1],(float)-yso1[2]);


gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPointysz[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);


//右上臂背面
//y
ysintputy[0][0]=yso1[1];
ysintputy[9][0]=yso2[1];
for(i=1;i<9;i++)
{
	ysintputy[i][0]=yso1[1]-(yso1[1]-yso2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		ysintputy[i][j]=ysintputy[i][0];
	}
	
}
//x
ysintputx[0][0]=yso1[0]+ysr1;
ysintputx[0][9]=yso1[0]-ysr1;
ysintputx[9][0]=yso2[0]+ysr2;
ysintputx[9][9]=yso2[0]-ysr2;
for(i=1;i<9;i++)
{
	ysintputx[i][0]=(ysintputx[9][0]-ysintputx[0][0])*i/9+ysintputx[0][0];
	ysintputx[i][9]=ysintputx[0][9]-(ysintputx[0][9]-ysintputx[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		ysintputx[i][j]=(ysintputx[i][9]-ysintputx[i][0])*j/9+ysintputx[i][0];
	}
}
//z
ysintputz[0][0]=yso1[2];
ysintputz[0][9]=yso1[2];
ysintputz[9][0]=yso2[2];
ysintputz[9][9]=yso2[2];
ysintputz[0][4]=yso1[2]+ysr3;
ysintputz[0][5]=yso1[2]+ysr3;
ysintputz[9][4]=yso2[2]+ysr4;
ysintputz[9][5]=yso2[2]+ysr4;
for(i=1;i<9;i++)
{
	ysintputz[i][0]=ysintputz[0][0]-(ysintputz[0][0]-ysintputz[9][0])*i/9;
	ysintputz[i][4]=ysintputz[0][4]-(ysintputz[0][4]-ysintputz[9][4])*i/9;
	ysintputz[i][5]=ysintputz[0][4]-(ysintputz[0][4]-ysintputz[9][4])*i/9;
	ysintputz[i][9]=ysintputz[0][9]-(ysintputz[0][9]-ysintputz[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	ysintputz[i][1]=(ysintputz[i][4]-ysintputz[i][0])/2+ysintputz[i][0];
	ysintputz[i][2]=(ysintputz[i][4]-ysintputz[i][1])/2+ysintputz[i][1];
	ysintputz[i][3]=(ysintputz[i][4]-ysintputz[i][2])/2+ysintputz[i][2];
	ysintputz[i][8]=(ysintputz[i][5]-ysintputz[i][9])/2+ysintputz[i][9];
	ysintputz[i][7]=(ysintputz[i][5]-ysintputz[i][8])/2+ysintputz[i][8];
	ysintputz[i][6]=(ysintputz[i][5]-ysintputz[i][7])/2+ysintputz[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,ysintputx,ysintputy,ysintputz);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPointys[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}



gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPointys[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);
glPopMatrix();


//右小臂正面
//y
ybintputy1[0][0]=ybo1[1];
ybintputy1[9][0]=ybo2[1];
for(i=1;i<9;i++)
{
	ybintputy1[i][0]=ybo1[1]-(ybo1[1]-ybo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		ybintputy1[i][j]=ybintputy1[i][0];
	}
	
}
//x
ybintputx1[0][0]=ybo1[0]+ybr1;
ybintputx1[0][9]=ybo1[0]-ybr1;
ybintputx1[9][0]=ybo2[0]+ybr2;
ybintputx1[9][9]=ybo2[0]-ybr2;
for(i=1;i<9;i++)
{
	ybintputx1[i][0]=(ybintputx1[9][0]-ybintputx1[0][0])*i/9+ybintputx1[0][0];
	ybintputx1[i][9]=ybintputx1[0][9]-(ybintputx1[0][9]-ybintputx1[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		ybintputx1[i][j]=(ybintputx1[i][9]-ybintputx1[i][0])*j/9+ybintputx1[i][0];
	}
}
//z
ybintputz1[0][0]=ybo1[2];
ybintputz1[0][9]=ybo1[2];
ybintputz1[9][0]=ybo2[2];
ybintputz1[9][9]=ybo2[2];
ybintputz1[0][4]=ybo1[2]+ybr5;
ybintputz1[0][5]=ybo1[2]+ybr5;
ybintputz1[9][4]=ybo2[2]+ybr6;
ybintputz1[9][5]=ybo2[2]+ybr6;
for(i=1;i<9;i++)
{
	ybintputz1[i][0]=ybintputz1[0][0]-(ybintputz1[0][0]-ybintputz1[9][0])*i/9;
	ybintputz1[i][4]=ybintputz1[0][4]-(ybintputz1[0][4]-ybintputz1[9][4])*i/9;
	ybintputz1[i][5]=ybintputz1[0][4]-(ybintputz1[0][4]-ybintputz1[9][4])*i/9;
	ybintputz1[i][9]=ybintputz1[0][9]-(ybintputz1[0][9]-ybintputz1[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	ybintputz1[i][1]=(ybintputz1[i][4]-ybintputz1[i][0])/2+ybintputz1[i][0];
	ybintputz1[i][2]=(ybintputz1[i][4]-ybintputz1[i][1])/2+ybintputz1[i][1];
	ybintputz1[i][3]=(ybintputz1[i][4]-ybintputz1[i][2])/2+ybintputz1[i][2];
	ybintputz1[i][8]=(ybintputz1[i][5]-ybintputz1[i][9])/2+ybintputz1[i][9];
	ybintputz1[i][7]=(ybintputz1[i][5]-ybintputz1[i][8])/2+ybintputz1[i][8];
	ybintputz1[i][6]=(ybintputz1[i][5]-ybintputz1[i][7])/2+ybintputz1[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,ybintputx1,ybintputy1,ybintputz1);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=5;j>=0;j--)
	{
		for(k=0;k<3;k++)
		{
			ctrlPointybz[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}

glPushMatrix();
glTranslatef((float)ybo1[0],(float)ybo1[1],(float)ybo1[2]);
uu[0]=(float)(ybo22[0]-ybo1[0]);
uu[1]=(float)(ybo22[1]-ybo1[1]);
uu[2]=(float)(ybo22[2]-ybo1[2]);
m3dCrossProduct(result,vv,uu);

allu=sqrt(uu[1]*uu[1] + uu[2]*uu[2]+ uu[0]*uu[0]);
allv=sqrt(vv[1]*vv[1] + vv[2]*vv[2]+ vv[0]*vv[0]);
uu[0]/=(float)allu;
uu[1]/=(float)allu;
uu[2]/=(float)allu;
vv[0]/=(float)allv;
vv[1]/=(float)allv;
vv[2]/=(float)allv;

angle=m3dGetAngleBetweenVectors( uu,vv);
angle1=angle*180/PI;
glRotatef((float)angle1,result[0],result[1],result[2]);
glTranslatef((float)-ybo1[0],(float)-ybo1[1],(float)-ybo1[2]);



gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPointybz[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);


ybintputy[0][0]=ybo1[1];
ybintputy[9][0]=ybo2[1];
for(i=1;i<9;i++)
{
	ybintputy[i][0]=ybo1[1]-(ybo1[1]-ybo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		ybintputy[i][j]=ybintputy[i][0];
	}
	
}
//x
ybintputx[0][0]=ybo1[0]+ybr1;
ybintputx[0][9]=ybo1[0]-ybr1;
ybintputx[9][0]=ybo2[0]+ybr2;
ybintputx[9][9]=ybo2[0]-ybr2;
for(i=1;i<9;i++)
{
	ybintputx[i][0]=(ybintputx[9][0]-ybintputx[0][0])*i/9+ybintputx[0][0];
	ybintputx[i][9]=ybintputx[0][9]-(ybintputx[0][9]-ybintputx[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		ybintputx[i][j]=(ybintputx[i][9]-ybintputx[i][0])*j/9+ybintputx[i][0];
	}
}

ybintputz[0][0]=ybo1[2];
ybintputz[0][9]=ybo1[2];
ybintputz[9][0]=ybo2[2];
ybintputz[9][9]=ybo2[2];
ybintputz[0][4]=ybo1[2]+ybr3;
ybintputz[0][5]=ybo1[2]+ybr3;
ybintputz[9][4]=ybo2[2]+ybr4;
ybintputz[9][5]=ybo2[2]+ybr4;
for(i=1;i<9;i++)
{
	ybintputz[i][0]=ybintputz[0][0]-(ybintputz[0][0]-ybintputz[9][0])*i/9;
	ybintputz[i][4]=ybintputz[0][4]-(ybintputz[0][4]-ybintputz[9][4])*i/9;
	ybintputz[i][5]=ybintputz[0][4]-(ybintputz[0][4]-ybintputz[9][4])*i/9;
	ybintputz[i][9]=ybintputz[0][9]-(ybintputz[0][9]-ybintputz[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	ybintputz[i][1]=(ybintputz[i][4]-ybintputz[i][0])/2+ybintputz[i][0];
	ybintputz[i][2]=(ybintputz[i][4]-ybintputz[i][1])/2+ybintputz[i][1];
	ybintputz[i][3]=(ybintputz[i][4]-ybintputz[i][2])/2+ybintputz[i][2];
	ybintputz[i][8]=(ybintputz[i][5]-ybintputz[i][9])/2+ybintputz[i][9];
	ybintputz[i][7]=(ybintputz[i][5]-ybintputz[i][8])/2+ybintputz[i][8];
	ybintputz[i][6]=(ybintputz[i][5]-ybintputz[i][7])/2+ybintputz[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,ybintputx,ybintputy,ybintputz);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPointyb[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}


gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPointyb[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);
glPopMatrix();



//左上臂正面
//y
zsintputy1[0][0]=zso1[1];
zsintputy1[9][0]=zso2[1];
for(i=1;i<9;i++)
{
	zsintputy1[i][0]=zso1[1]-(zso1[1]-zso2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		zsintputy1[i][j]=zsintputy1[i][0];
	}
	
}
//x
zsintputx1[0][0]=zso1[0]+zsr1;
zsintputx1[0][9]=zso1[0]-zsr1;
zsintputx1[9][0]=zso2[0]+zsr2;
zsintputx1[9][9]=zso2[0]-zsr2;
for(i=1;i<9;i++)
{
	zsintputx1[i][0]=(zsintputx1[9][0]-zsintputx1[0][0])*i/9+zsintputx1[0][0];
	zsintputx1[i][9]=zsintputx1[0][9]-(zsintputx1[0][9]-zsintputx1[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		zsintputx1[i][j]=(zsintputx1[i][9]-zsintputx1[i][0])*j/9+zsintputx1[i][0];
	}
}
//z
zsintputz1[0][0]=zso1[2];
zsintputz1[0][9]=zso1[2];
zsintputz1[9][0]=zso2[2];
zsintputz1[9][9]=zso2[2];
zsintputz1[0][4]=zso1[2]+zsr5;
zsintputz1[0][5]=zso1[2]+zsr5;
zsintputz1[9][4]=zso2[2]+zsr6;
zsintputz1[9][5]=zso2[2]+zsr6;
for(i=1;i<9;i++)
{
	zsintputz1[i][0]=zsintputz1[0][0]-(zsintputz1[0][0]-zsintputz1[9][0])*i/9;
	zsintputz1[i][4]=zsintputz1[0][4]-(zsintputz1[0][4]-zsintputz1[9][4])*i/9;
	zsintputz1[i][5]=zsintputz1[0][4]-(zsintputz1[0][4]-zsintputz1[9][4])*i/9;
	zsintputz1[i][9]=zsintputz1[0][9]-(zsintputz1[0][9]-zsintputz1[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	zsintputz1[i][1]=(zsintputz1[i][4]-zsintputz1[i][0])/2+zsintputz1[i][0];
	zsintputz1[i][2]=(zsintputz1[i][4]-zsintputz1[i][1])/2+zsintputz1[i][1];
	zsintputz1[i][3]=(zsintputz1[i][4]-zsintputz1[i][2])/2+zsintputz1[i][2];
	zsintputz1[i][8]=(zsintputz1[i][5]-zsintputz1[i][9])/2+zsintputz1[i][9];
	zsintputz1[i][7]=(zsintputz1[i][5]-zsintputz1[i][8])/2+zsintputz1[i][8];
	zsintputz1[i][6]=(zsintputz1[i][5]-zsintputz1[i][7])/2+zsintputz1[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,zsintputx1,zsintputy1,zsintputz1);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=5;j>=0;j--)
	{
		for(k=0;k<3;k++)
		{
			ctrlPointzsz[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}
/*

M3DVector3f u={0.0f,-1.0f,0.0f};
M3DVector3f v={1.3f,0.0f,1.3f};
M3DVector3f result;
m3dCrossProduct(result,u,v);
cin>>dim2;*/
/*
M3DVector3d u={0.0,-1.0,0.0};
M3DVector3d v={1.3,0.0,1.3};
double angle=m3dGetAngleBetweenVectors( u, v);
double anlge1=angle*180/PI;
cin>>dim2;*/
glPushMatrix();
glTranslatef((float)zso1[0],(float)zso1[1],(float)zso1[2]);
//glRotatef(30.0f,0.0f,0.0f,1.0f);
uu[0]=(float)(zso22[0]-zso1[0]);
uu[1]=(float)(zso22[1]-zso1[1]);
uu[2]=(float)(zso22[2]-zso1[2]);
m3dCrossProduct(result,vv,uu);
//M3DVector3d uu={9.0,-15.5,0.0};
//M3DVector3d vv={0.0,-10.0,0.0};
//M3DVector3d uu={0.0,-1.0,0.0};
//M3DVector3d vv={1.0,-1.0,0.0};
allu=sqrt(uu[1]*uu[1] + uu[2]*uu[2]+ uu[0]*uu[0]);
allv=sqrt(vv[1]*vv[1] + vv[2]*vv[2]+ vv[0]*vv[0]);
uu[0]/=(float)allu;
uu[1]/=(float)allu;
uu[2]/=(float)allu;
vv[0]/=(float)allv;
vv[1]/=(float)allv;
vv[2]/=(float)allv;

angle=m3dGetAngleBetweenVectors( uu,vv);
angle1=angle*180/PI;
glRotatef((float)angle1,result[0],result[1],result[2]);
glTranslatef((float)-zso1[0],(float)-zso1[1],(float)-zso1[2]);

gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPointzsz[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);

//右上臂背面
//y
zsintputy[0][0]=zso1[1];
zsintputy[9][0]=zso2[1];
for(i=1;i<9;i++)
{
	zsintputy[i][0]=zso1[1]-(zso1[1]-zso2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		zsintputy[i][j]=zsintputy[i][0];
	}
	
}
//x
zsintputx[0][0]=zso1[0]+zsr1;
zsintputx[0][9]=zso1[0]-zsr1;
zsintputx[9][0]=zso2[0]+zsr2;
zsintputx[9][9]=zso2[0]-zsr2;
for(i=1;i<9;i++)
{
	zsintputx[i][0]=(zsintputx[9][0]-zsintputx[0][0])*i/9+zsintputx[0][0];
	zsintputx[i][9]=zsintputx[0][9]-(zsintputx[0][9]-zsintputx[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		zsintputx[i][j]=(zsintputx[i][9]-zsintputx[i][0])*j/9+zsintputx[i][0];
	}
}
//z
zsintputz[0][0]=zso1[2];
zsintputz[0][9]=zso1[2];
zsintputz[9][0]=zso2[2];
zsintputz[9][9]=zso2[2];
zsintputz[0][4]=zso1[2]+zsr3;
zsintputz[0][5]=zso1[2]+zsr3;
zsintputz[9][4]=zso2[2]+zsr4;
zsintputz[9][5]=zso2[2]+zsr4;
for(i=1;i<9;i++)
{
	zsintputz[i][0]=zsintputz[0][0]-(zsintputz[0][0]-zsintputz[9][0])*i/9;
	zsintputz[i][4]=zsintputz[0][4]-(zsintputz[0][4]-zsintputz[9][4])*i/9;
	zsintputz[i][5]=zsintputz[0][4]-(zsintputz[0][4]-zsintputz[9][4])*i/9;
	zsintputz[i][9]=zsintputz[0][9]-(zsintputz[0][9]-zsintputz[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	zsintputz[i][1]=(zsintputz[i][4]-zsintputz[i][0])/2+zsintputz[i][0];
	zsintputz[i][2]=(zsintputz[i][4]-zsintputz[i][1])/2+zsintputz[i][1];
	zsintputz[i][3]=(zsintputz[i][4]-zsintputz[i][2])/2+zsintputz[i][2];
	zsintputz[i][8]=(zsintputz[i][5]-zsintputz[i][9])/2+zsintputz[i][9];
	zsintputz[i][7]=(zsintputz[i][5]-zsintputz[i][8])/2+zsintputz[i][8];
	zsintputz[i][6]=(zsintputz[i][5]-zsintputz[i][7])/2+zsintputz[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,zsintputx,zsintputy,zsintputz);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPointzs[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}
gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPointzs[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);
glPopMatrix();



//右小臂正面
//y
zbintputy1[0][0]=zbo1[1];
zbintputy1[9][0]=zbo2[1];
for(i=1;i<9;i++)
{
	zbintputy1[i][0]=zbo1[1]-(zbo1[1]-zbo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		zbintputy1[i][j]=zbintputy1[i][0];
	}
	
}
//x
zbintputx1[0][0]=zbo1[0]+zbr1;
zbintputx1[0][9]=zbo1[0]-zbr1;
zbintputx1[9][0]=zbo2[0]+zbr2;
zbintputx1[9][9]=zbo2[0]-zbr2;
for(i=1;i<9;i++)
{
	zbintputx1[i][0]=(zbintputx1[9][0]-zbintputx1[0][0])*i/9+zbintputx1[0][0];
	zbintputx1[i][9]=zbintputx1[0][9]-(zbintputx1[0][9]-zbintputx1[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		zbintputx1[i][j]=(zbintputx1[i][9]-zbintputx1[i][0])*j/9+zbintputx1[i][0];
	}
}
//z
zbintputz1[0][0]=zbo1[2];
zbintputz1[0][9]=zbo1[2];
zbintputz1[9][0]=zbo2[2];
zbintputz1[9][9]=zbo2[2];
zbintputz1[0][4]=zbo1[2]+zbr5;
zbintputz1[0][5]=zbo1[2]+zbr5;
zbintputz1[9][4]=zbo2[2]+zbr6;
zbintputz1[9][5]=zbo2[2]+zbr6;
for(i=1;i<9;i++)
{
	zbintputz1[i][0]=zbintputz1[0][0]-(zbintputz1[0][0]-zbintputz1[9][0])*i/9;
	zbintputz1[i][4]=zbintputz1[0][4]-(zbintputz1[0][4]-zbintputz1[9][4])*i/9;
	zbintputz1[i][5]=zbintputz1[0][4]-(zbintputz1[0][4]-zbintputz1[9][4])*i/9;
	zbintputz1[i][9]=zbintputz1[0][9]-(zbintputz1[0][9]-zbintputz1[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	zbintputz1[i][1]=(zbintputz1[i][4]-zbintputz1[i][0])/2+zbintputz1[i][0];
	zbintputz1[i][2]=(zbintputz1[i][4]-zbintputz1[i][1])/2+zbintputz1[i][1];
	zbintputz1[i][3]=(zbintputz1[i][4]-zbintputz1[i][2])/2+zbintputz1[i][2];
	zbintputz1[i][8]=(zbintputz1[i][5]-zbintputz1[i][9])/2+zbintputz1[i][9];
	zbintputz1[i][7]=(zbintputz1[i][5]-zbintputz1[i][8])/2+zbintputz1[i][8];
	zbintputz1[i][6]=(zbintputz1[i][5]-zbintputz1[i][7])/2+zbintputz1[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,zbintputx1,zbintputy1,zbintputz1);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=5;j>=0;j--)
	{
		for(k=0;k<3;k++)
		{
			ctrlPointzbz[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}
/*

M3DVector3f u={0.0f,-1.0f,0.0f};
M3DVector3f v={1.3f,0.0f,1.3f};
M3DVector3f result;
m3dCrossProduct(result,u,v);
cin>>dim2;*/
/*
M3DVector3d u={0.0,-1.0,0.0};
M3DVector3d v={1.3,0.0,1.3};
double angle=m3dGetAngleBetweenVectors( u, v);
double anlge1=angle*180/PI;
cin>>dim2;*/
glPushMatrix();
glTranslatef((float)zbo1[0],(float)zbo1[1],(float)zbo1[2]);
uu[0]=(float)(zbo22[0]-zbo1[0]);
uu[1]=(float)(zbo22[1]-zbo1[1]);
uu[2]=(float)(zbo22[2]-zbo1[2]);
m3dCrossProduct(result,vv,uu);
//M3DVector3d uu={9.0,-15.5,0.0};
//M3DVector3d vv={0.0,-10.0,0.0};
//M3DVector3d uu={0.0,-1.0,0.0};
//M3DVector3d vv={1.0,-1.0,0.0};
allu=sqrt(uu[1]*uu[1] + uu[2]*uu[2]+ uu[0]*uu[0]);
allv=sqrt(vv[1]*vv[1] + vv[2]*vv[2]+ vv[0]*vv[0]);
uu[0]/=(float)allu;
uu[1]/=(float)allu;
uu[2]/=(float)allu;
vv[0]/=(float)allv;
vv[1]/=(float)allv;
vv[2]/=(float)allv;

angle=m3dGetAngleBetweenVectors( uu,vv);
angle1=angle*180/PI;
glRotatef((float)angle1,result[0],result[1],result[2]);
glTranslatef((float)-zbo1[0],(float)-zbo1[1],(float)-zbo1[2]);

gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPointzbz[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);

//右上臂背面
//y
zbintputy[0][0]=zbo1[1];
zbintputy[9][0]=zbo2[1];
for(i=1;i<9;i++)
{
	zbintputy[i][0]=zbo1[1]-(zbo1[1]-zbo2[1])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<=9;j++)
	{
		zbintputy[i][j]=zbintputy[i][0];
	}
	
}
//x
zbintputx[0][0]=zbo1[0]+zbr1;
zbintputx[0][9]=zbo1[0]-zbr1;
zbintputx[9][0]=zbo2[0]+zbr2;
zbintputx[9][9]=zbo2[0]-zbr2;
for(i=1;i<9;i++)
{
	zbintputx[i][0]=(zbintputx[9][0]-zbintputx[0][0])*i/9+zbintputx[0][0];
	zbintputx[i][9]=zbintputx[0][9]-(zbintputx[0][9]-zbintputx[9][9])*i/9;
}

for(i=0;i<=9;i++)
{
	for(j=1;j<9;j++)
	{
		zbintputx[i][j]=(zbintputx[i][9]-zbintputx[i][0])*j/9+zbintputx[i][0];
	}
}
//z
zbintputz[0][0]=zbo1[2];
zbintputz[0][9]=zbo1[2];
zbintputz[9][0]=zbo2[2];
zbintputz[9][9]=zbo2[2];
zbintputz[0][4]=zbo1[2]+zbr3;
zbintputz[0][5]=zbo1[2]+zbr3;
zbintputz[9][4]=zbo2[2]+zbr4;
zbintputz[9][5]=zbo2[2]+zbr4;
for(i=1;i<9;i++)
{
	zbintputz[i][0]=zbintputz[0][0]-(zbintputz[0][0]-zbintputz[9][0])*i/9;
	zbintputz[i][4]=zbintputz[0][4]-(zbintputz[0][4]-zbintputz[9][4])*i/9;
	zbintputz[i][5]=zbintputz[0][4]-(zbintputz[0][4]-zbintputz[9][4])*i/9;
	zbintputz[i][9]=zbintputz[0][9]-(zbintputz[0][9]-zbintputz[9][9])*i/9;
}
for(i=0;i<10;i++)
{
	zbintputz[i][1]=(zbintputz[i][4]-zbintputz[i][0])/2+zbintputz[i][0];
	zbintputz[i][2]=(zbintputz[i][4]-zbintputz[i][1])/2+zbintputz[i][1];
	zbintputz[i][3]=(zbintputz[i][4]-zbintputz[i][2])/2+zbintputz[i][2];
	zbintputz[i][8]=(zbintputz[i][5]-zbintputz[i][9])/2+zbintputz[i][9];
	zbintputz[i][7]=(zbintputz[i][5]-zbintputz[i][8])/2+zbintputz[i][8];
	zbintputz[i][6]=(zbintputz[i][5]-zbintputz[i][7])/2+zbintputz[i][7];
}

p=Bsplin_fit.GlobalSurApproxFixednm (dim1,dim2,zbintputx,zbintputy,zbintputz);
crlp=0;
for(i=0;i<6;i++)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<3;k++)
		{
			ctrlPointzb[i][j][k]=(float)*(p+crlp);
			crlp++;
		}
	}
}
gluBeginSurface (surfName2);
gluNurbsSurface(surfName2,10,ku,10,kv,2,12,&tps[0][0][0],4,4, GL_MAP2_TEXTURE_COORD_2);
gluNurbsSurface (surfName2,
 10, ku, 
 10, kv, 3, 18, 
 &ctrlPointzb[0][0][0], 4, 4, 
GL_MAP2_VERTEX_3);
gluEndSurface (surfName2);
glPopMatrix();


GLUquadricObj *pObj0;
pObj0=gluNewQuadric();
	gluQuadricNormals(pObj0,GLU_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	gluQuadricDrawStyle(pObj0,GLU_FILL);
	gluQuadricOrientation(pObj0,GLU_OUTSIDE);
	gluQuadricTexture(pObj0, GL_TRUE);


		glPushMatrix();
	glTranslatef((float)zbo22[0],(float)zbo22[1],(float)zbo22[2]); 
		gluSphere(pObj0,1.78f,30,15);
		glPopMatrix();

			glPushMatrix();
	glTranslatef((float)zbo1[0],(float)zbo1[1],(float)zbo1[2]); 
		gluSphere(pObj0,1.94f,30,15);
		glPopMatrix();
		
			glPushMatrix();
	glTranslatef((float)zso1[0],(float)zso1[1],(float)zso1[2]); 
		gluSphere(pObj0,2.10f,30,15);
		glPopMatrix();

			glPushMatrix();
	glTranslatef((float)ybo1[0],(float)ybo1[1],(float)ybo1[2]); 
		gluSphere(pObj0,1.94f,30,15);
		glPopMatrix();
		
			glPushMatrix();
	glTranslatef((float)yso1[0],(float)yso1[1],(float)yso1[2]); 
		gluSphere(pObj0,2.10f,30,15);
		glPopMatrix();


		

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
		GLfloat x[5]={0.0f,0.0f,0.0f,0.0f,0.0f};
GLfloat y[5]={0.0f,0.0f,0.0f,0.0f,0.0f};
GLfloat z[2]={0.0f,0.0f};

GLfloat a=10.0f;
GLfloat b=0.0f;
GLfloat c=10.0f;
M3DVector3f v={1.0f,1.0f,1.0f};
		GLUquadricObj *pObj;//二次方程对象

	M3DVector3f vNormal;
	M3DVector3f vNode[7]={{-33.6f,18.0f,3.0f},{-30.0f,40.0f,0.0f},{4.4f,40.0f,0.0f},
	{30.5f,30.0f,2.0f},{32.5f,3.0f,4.0f},{20.0f,-30.0f,7.0f},{-2.0f,-30.0f,7.0f}};
	M3DVector3f vNodeb[7]={{-33.6f,18.0f,-17.0f},{-30.0f,40.0f,-15.0f},{4.4f,40.0f,-15.0f},
	{30.5f,30.0f,-16.0f},{32.5f,3.0f,-18.0f},{20.0f,-30.0f,-20.0f},{-2.0f,-30.0f,-20.0f}};
	// Clear the window with current clearing color
//	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
   // glTranslatef(0.0f+a,-10.0f+b,-250.0f+c);
	glTranslated(ybo22[0],ybo22[1],ybo22[2]);
	glRotated(anglehand1,yhand1[0],yhand1[1],yhand1[2]);
	M3DVector3f vvv={0.0f,1.0f,0.0f};
	m3dCrossProduct(result,vvv,yhand1);
allu=sqrt(yhand1[1]*yhand1[1] + yhand1[2]*yhand1[2]+ yhand1[0]*yhand1[0]);
allv=sqrt(vvv[1]*vvv[1] + vvv[2]*vvv[2]+ vvv[0]*vvv[0]);
yhand1[0]/=(float)allu;
yhand1[1]/=(float)allu;
yhand1[2]/=(float)allu;
vvv[0]/=(float)allv;
vvv[1]/=(float)allv;
vvv[2]/=(float)allv;

angle=m3dGetAngleBetweenVectors( yhand1,vvv);
angle1=angle*180/PI;
glRotatef((float)angle1,result[0],result[1],result[2]);
	glScaled(0.05,0.05,0.05);

	// Draw a filled polygon with current color
	//画前手掌(顺时针)
	// Set current drawing color to yellow
	//		   R	 G	   B  
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	 
	m3dFindNormal(vNormal, vNode[2],vNode[1],vNode[0]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNode[0]);
	glTexCoord2f(0.071f,1.0f);
	glVertex3fv(vNode[1]);
	glTexCoord2f(0.563f,1.0f);
	glVertex3fv(vNode[2]);

	m3dFindNormal(vNormal, vNode[4],vNode[3],vNode[2]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.563f,1.0f);
	glVertex3fv(vNode[2]);
	glTexCoord2f(0.936f,0.857f);
	glVertex3fv(vNode[3]);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNode[4]);

	m3dFindNormal(vNormal, vNode[6],vNode[5],vNode[4]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNode[4]);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNode[5]);
	glTexCoord2f(0.471f,0.0f);
	glVertex3fv(vNode[6]);

	m3dFindNormal(vNormal, vNode[4],vNode[0],vNode[6]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.471f,0.0f);
	glVertex3fv(vNode[6]);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNode[0]);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNode[4]);

	m3dFindNormal(vNormal, vNode[4],vNode[2],vNode[0]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNode[0]);
	glTexCoord2f(0.563f,1.0f);
	glVertex3fv(vNode[2]);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNode[4]);
	//glTexCoord2f(0.1f,0.125f);
	//glVertex3f(-32.0f,-20.0f,0.0f);
	glEnd();
	//画后手掌（顺时针）
	glBegin(GL_TRIANGLES);
	m3dFindNormal(vNormal, vNodeb[5],vNodeb[6],vNodeb[0]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNodeb[0]);
	glTexCoord2f(0.471f,0.0f);
	glVertex3fv(vNodeb[6]);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNodeb[5]);

	m3dFindNormal(vNormal, vNodeb[3],vNodeb[4],vNodeb[5]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNodeb[5]);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNodeb[4]);
	glTexCoord2f(0.936f,0.857f);
	glVertex3fv(vNodeb[3]);

	m3dFindNormal(vNormal, vNodeb[1],vNodeb[2],vNodeb[3]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.936f,0.857f);
	glVertex3fv(vNodeb[3]);
	glTexCoord2f(0.563f,1.0f);
	glVertex3fv(vNodeb[2]);
	glTexCoord2f(0.071f,1.0f);
	glVertex3fv(vNodeb[1]);

	m3dFindNormal(vNormal, vNodeb[5],vNodeb[0],vNodeb[1]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.071f,1.0f);
	glVertex3fv(vNodeb[1]);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNodeb[0]);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNodeb[5]);

	m3dFindNormal(vNormal, vNodeb[1],vNodeb[3],vNodeb[5]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNodeb[5]);
	glTexCoord2f(0.936f,0.857f);
	glVertex3fv(vNodeb[3]);
	glTexCoord2f(0.071f,1.0f);
	glVertex3fv(vNodeb[1]);

	glEnd();

	//使手掌封闭（顺时针）
	glBegin(GL_QUADS);
	//glNormal3i(-4,1,0);
	m3dFindNormal(vNormal, vNodeb[1],vNodeb[0],vNode[0]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[0]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[0]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[1]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[1]);
	
	m3dFindNormal(vNormal, vNodeb[2],vNodeb[1],vNode[1]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[1]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[1]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[2]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[2]);

	m3dFindNormal(vNormal, vNodeb[3],vNodeb[2],vNode[2]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[2]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[2]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[3]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[3]);

	m3dFindNormal(vNormal, vNodeb[4],vNodeb[3],vNode[3]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[3]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[3]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[4]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[4]);

	m3dFindNormal(vNormal, vNodeb[5],vNodeb[4],vNode[4]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[4]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[4]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[5]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[5]);

	m3dFindNormal(vNormal, vNodeb[6],vNodeb[5],vNode[5]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[5]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[5]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[6]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[6]);

	m3dFindNormal(vNormal, vNodeb[0],vNodeb[6],vNode[6]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[6]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[6]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[0]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[0]);

	glEnd();
	
	//画手指
	pObj=gluNewQuadric();
	gluQuadricNormals(pObj,GLU_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	gluQuadricDrawStyle(pObj,GLU_FILL);
	gluQuadricOrientation(pObj,GLU_OUTSIDE);
	gluQuadricTexture(pObj, GL_TRUE);
   //
    //画大拇指
	//glBindTexture(GL_TEXTURE_2D, textureObjects[0]);
	//glDisable(GL_CULL_FACE);
	glPushMatrix();
		//glColor3ub(227,207,87);
	    //该圆柱体作为手掌的一部分
	    glPushMatrix();
		glTranslatef(-8.5f,-37.0f,-6.5f);
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		glRotatef(-31.0f,0.0f,1.0f,0.0f);
		//gluSphere(pObj,15.0f,50,25);
		gluCylinder(pObj,16.0f,10.3f,53.0f,50,25);
		glPopMatrix();

		glTranslatef(-37.3f,10.0f,-7.0f); 
		gluSphere(pObj,10.5f,30,15);
		
		//glColor3ub(245, 222, 179);
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		glRotatef(-30.0f,0.0f,1.0f,0.0f);
		glRotatef(x[0],0.5f,0.0f,0.866f);
		gluCylinder(pObj,10.5f,8.8f,21.0f,50,25);
	
		glTranslatef(0.0f,0.0f,21.0f);
		gluSphere(pObj,9.0f,50,25);

       if(x[0]<=135){
		    gluCylinder(pObj,9.0f,6.0f,18.0f,50,25);

			glTranslatef(0.0f,0.0f,18.0f);
			gluSphere(pObj,6.0f,50,25);
	   }else{
			glRotatef(y[0],1.0f,-1.0f,-1.0f);	
			gluCylinder(pObj,9.5f,6.0f,18.0f,50,25);

		    glTranslatef(0.0f,0.0f,18.0f);
		    gluSphere(pObj,6.0f,50,25);
		}
        if(x[0]<=135)x[0]+=5.0f;
		else
			if(y[0]<=60)y[0]=y[0]+5;
		
	glPopMatrix();

	//画食指
	glPushMatrix();
		//glColor3ub(227,207,87);
		glTranslatef(-22.5f,40.0f,-7.5f);
		gluSphere(pObj,7.5f,50,25);

		glPushMatrix();
			//glColor3ub(245, 222, 179);
			glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
			gluCylinder(pObj,7.5f,7.5f,18.0f,50,25);
		glPopMatrix();

		//glColor3ub(227,207,87);
		glPushMatrix();
			glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
			glTranslatef(0.0f,0.0f,18.0f);
			gluSphere(pObj,7.5f,50,25);
		glPopMatrix();
		if(x[1]<90){
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,18.0f);
				gluCylinder(pObj,7.5f,6.0f,16.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
				glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,34.0f);
				gluSphere(pObj,6.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,34.0f);
				gluCylinder(pObj,6.0f,4.0f,14.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				 glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f); 
				 glTranslatef(0.0f,0.0f,48.0f);
				 gluSphere(pObj,4.0f,50,25);
			glPopMatrix();
		}else{
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,18.0f);
				glRotatef(y[1],1.0f,0.0f,0.0f);
				gluCylinder(pObj,7.5f,6.0f,16.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
			    glTranslatef(0.0f,0.0f,18.0f);
				glRotatef(y[1],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,16.0f);
				gluSphere(pObj,6.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,18.0f);
				glRotatef(y[1],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,16.0f);
				gluCylinder(pObj,6.0f,4.0f,14.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
		    	glTranslatef(0.0f,0.0f,18.0f);
				glRotatef(y[1],1.0f,0.0f,0.0f); 
				glTranslatef(0.0f,0.0f,30.0f);
				gluSphere(pObj,4.0f,50,25);
			glPopMatrix();
		}

		if(y[0]>60){
			if(x[1]<90)x[1]+=5.0f;
			else
				if(y[1]<90)y[1]+=5.0f;
		}
		//else
			//if(y[0]<60)y[0]=y[0]+5;

	glPopMatrix();
	
	//画中指
	glPushMatrix();
		//glColor3ub(227,207,87);
		glTranslatef(-5.4f,40.0f,-7.5f);
		gluSphere(pObj,8.8f,50,25);

		glPushMatrix();
			//glColor3ub(245, 222, 179);
			glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
			gluCylinder(pObj,8.8f,8.8f,20.0f,50,25);
		glPopMatrix();

		glPushMatrix();
		//glColor3ub(227,207,87);
			glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
			glTranslatef(0.0f,0.0f,20.0f);
			gluSphere(pObj,8.8f,50,25);
		glPopMatrix();

		if(x[2]<90){
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,20.0f);
				gluCylinder(pObj,8.8f,7.5f,18.0f,50,25);
			glPopMatrix();

			//glColor3ub(227,207,87);
			glPushMatrix();
				glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,38.0f);
				gluSphere(pObj,7.5f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,38.0f);
				gluCylinder(pObj,7.5f,5.2f,16.0f,50,25);
			glPopMatrix();

			//glColor3ub(245,222,179);
			glPushMatrix();
				glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,54.0f);
				gluSphere(pObj,5.2f,50,25);
			glPopMatrix();
		}
		else{
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[2],1.0f,0.0f,0.0f);
				gluCylinder(pObj,8.8f,7.5f,18.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
			    glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[2],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,18.0f);
				gluSphere(pObj,7.5f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[2],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,18.0f);
				gluCylinder(pObj,7.5f,5.2f,16.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
		    	glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[2],1.0f,0.0f,0.0f); 
				glTranslatef(0.0f,0.0f,34.0f);
				gluSphere(pObj,5.2f,50,25);
			glPopMatrix();
		}


		if(x[1]>=90){
			if(x[2]<90)x[2]+=5.0f;
			else
				if(y[2]<90)y[2]+=5.0f;
		}

	glPopMatrix();
		
	//画无名指
	glPushMatrix();
		//glColor3ub(227,207,87);
		glTranslatef(12.2f,35.62f,-7.5f);
		gluSphere(pObj,7.8f,50,25);

		glPushMatrix();
			//glColor3ub(245, 222, 179);
			glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
			gluCylinder(pObj,7.8f,7.8f,19.0f,50,25);
		glPopMatrix();

		glPushMatrix();
		//glColor3ub(227,207,87);
			glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
			glTranslatef(0.0f,0.0f,20.0f);
			gluSphere(pObj,7.8f,50,25);
		glPopMatrix();
		
		if(x[3]<90){
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,20.0f);
				gluCylinder(pObj,7.8f,6.6f,17.0f,50,25);
			glPopMatrix();

			//glColor3ub(227,207,87);
			glPushMatrix();
				glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,38.0f);
				gluSphere(pObj,6.6f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,38.0f);
				gluCylinder(pObj,6.6f,4.5f,15.0f,50,25);
			glPopMatrix();

			//glColor3ub(245,222,179);
			glPushMatrix();
				glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,54.0f);
				gluSphere(pObj,4.5f,50,25);
			glPopMatrix();
		}
		else{
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[3],1.0f,0.0f,0.0f);
				gluCylinder(pObj,7.8f,6.6f,17.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
			    glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[3],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,18.0f);
				gluSphere(pObj,6.6f,50,25);
			glPopMatrix();
		    if(y[3]<90){
				glPushMatrix();
					//glColor3ub(245, 222, 179);
					glTranslatef(0.0f,0.0f,20.0f);
					glRotatef(y[3],1.0f,0.0f,0.0f);
					glTranslatef(0.0f,0.0f,18.0f);
					gluCylinder(pObj,6.6f,4.5f,16.0f,50,25);
				glPopMatrix();

				glPushMatrix();
					//glColor3ub(245, 222, 179);
		    		glTranslatef(0.0f,0.0f,20.0f);
					glRotatef(y[3],1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,34.0f);
					gluSphere(pObj,4.5f,50,25);
				glPopMatrix();
			}
			else{
				glPushMatrix();
					//glColor3ub(245, 222, 179);
					glTranslatef(0.0f,0.0f,20.0f);
					glRotatef(90,1.0f,0.0f,0.0f);
					glTranslatef(0.0f,0.0f,18.0f);
					glRotatef(z[0],1.0f,0.0f,0.0f);
					gluCylinder(pObj,6.6f,4.5f,16.0f,50,25);
				glPopMatrix();

				glPushMatrix();
					//glColor3ub(245, 222, 179);
		    		glTranslatef(0.0f,0.0f,20.0f);
					glRotatef(90,1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,18.0f);
					glRotatef(z[0],1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,16.0f);
					gluSphere(pObj,4.5f,50,25);
				glPopMatrix();
			}
		}

		if(x[2]>=90){
			if(x[3]<90)x[3]+=5.0f;
			else
				if(y[3]<90)y[3]+=5.0f;
				else
					if(z[0]<20)z[0]+=3.0f;
		}
	glPopMatrix();

	//画小指
	glPushMatrix();
		//glColor3ub(227,207,87);
		glTranslatef(26.0f,33.0f,-7.5f);
		gluSphere(pObj,5.5f,50,25);

		glPushMatrix();
			//glColor3ub(245, 222, 179);
			glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
			gluCylinder(pObj,5.5f,5.5f,16.0f,50,25);
		glPopMatrix();

		glPushMatrix();
		//glColor3ub(227,207,87);
			glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
			glTranslatef(0.0f,0.0f,16.0f);
			gluSphere(pObj,5.5f,50,25);
		glPopMatrix();
		
		if(x[4]<90){
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,16.0f);
				gluCylinder(pObj,5.5f,4.5f,15.0f,50,25);
			glPopMatrix();

			//glColor3ub(227,207,87);
			glPushMatrix();
				glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,31.0f);
				gluSphere(pObj,4.5f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,31.0f);
				gluCylinder(pObj,4.5f,3.2f,11.0f,50,25);
			glPopMatrix();

			//glColor3ub(245,222,179);
			glPushMatrix();
				glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,42.0f);
				gluSphere(pObj,3.2f,50,25);
			glPopMatrix();
		}
		else{
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,16.0f);
				glRotatef(y[4],1.0f,0.0f,0.0f);
				gluCylinder(pObj,5.5f,4.5f,15.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
			    glTranslatef(0.0f,0.0f,16.0f);
				glRotatef(y[4],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,15.0f);
				gluSphere(pObj,4.5f,50,25);
			glPopMatrix();
			if(y[4]<90){
				glPushMatrix();
					//glColor3ub(245, 222, 179);
					glTranslatef(0.0f,0.0f,16.0f);
					glRotatef(y[4],1.0f,0.0f,0.0f);
					glTranslatef(0.0f,0.0f,15.0f);
					gluCylinder(pObj,4.5f,3.2f,11.0f,50,25);
				glPopMatrix();

				glPushMatrix();
					//glColor3ub(245, 222, 179);
		    		glTranslatef(0.0f,0.0f,16.0f);
					glRotatef(y[4],1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,26.0f);
					gluSphere(pObj,3.2f,50,25);
				glPopMatrix();
			}
			else{
				glPushMatrix();
					//glColor3ub(245, 222, 179);
					glTranslatef(0.0f,0.0f,16.0f);
					glRotatef(90,1.0f,0.0f,0.0f);
					glTranslatef(0.0f,0.0f,15.0f);
					glRotatef(z[1],1.0f,0.0f,0.0f);
					gluCylinder(pObj,4.5f,3.2f,11.0f,50,25);
				glPopMatrix();

				glPushMatrix();
					//glColor3ub(245, 222, 179);
		    		glTranslatef(0.0f,0.0f,16.0f);
					glRotatef(90,1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,15.0f);
					glRotatef(z[1],1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,11.0f);
					gluSphere(pObj,3.2f,50,25);
				glPopMatrix();

			}
		}

		if(z[0]>=20){
			if(x[4]<90)x[4]+=5.0f;
			else
				if(y[4]<90)y[4]+=5.0f;
				else
					if(z[1]<25)z[1]+=3.0f;
		}

	glPopMatrix();

	glPopMatrix();

	
	//左手
xRot = 0.0f;
 yRot = 0.0f;


a=10.0f;
 b=0.0f;
c=10.0f;

	glPushMatrix();
	glTranslated(zbo22[0],zbo22[1],zbo22[2]);
	glRotated(anglehand2,zhand2[0],zhand2[1],zhand2[2]);
	vvv[0]=0.0f;
	vvv[1]=1.0f;
	vvv[2]=0.0f;
	
	m3dCrossProduct(result,vvv,zhand2);
allu=sqrt(zhand2[1]*zhand2[1] + zhand2[2]*zhand2[2]+ zhand2[0]*zhand2[0]);
allv=sqrt(vvv[1]*vvv[1] + vvv[2]*vvv[2]+ vvv[0]*vvv[0]);
zhand2[0]/=(float)allu;
zhand2[1]/=(float)allu;
zhand2[2]/=(float)allu;
vvv[0]/=(float)allv;
vvv[1]/=(float)allv;
vvv[2]/=(float)allv;

angle=m3dGetAngleBetweenVectors( zhand2,vvv);
angle1=angle*180/PI;
glRotatef((float)angle1,result[0],result[1],result[2]);
	glScaled(0.05,0.05,0.05);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	 
	m3dFindNormal(vNormal, vNode[2],vNode[1],vNode[0]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNode[0]);
	glTexCoord2f(0.071f,1.0f);
	glVertex3fv(vNode[1]);
	glTexCoord2f(0.563f,1.0f);
	glVertex3fv(vNode[2]);

	m3dFindNormal(vNormal, vNode[4],vNode[3],vNode[2]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.563f,1.0f);
	glVertex3fv(vNode[2]);
	glTexCoord2f(0.936f,0.857f);
	glVertex3fv(vNode[3]);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNode[4]);

	m3dFindNormal(vNormal, vNode[6],vNode[5],vNode[4]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNode[4]);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNode[5]);
	glTexCoord2f(0.471f,0.0f);
	glVertex3fv(vNode[6]);

	m3dFindNormal(vNormal, vNode[4],vNode[0],vNode[6]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.471f,0.0f);
	glVertex3fv(vNode[6]);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNode[0]);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNode[4]);

	m3dFindNormal(vNormal, vNode[4],vNode[2],vNode[0]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNode[0]);
	glTexCoord2f(0.563f,1.0f);
	glVertex3fv(vNode[2]);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNode[4]);
	//glTexCoord2f(0.1f,0.125f);
	//glVertex3f(-32.0f,-20.0f,0.0f);
	glEnd();
	//画后手掌（顺时针）
	glBegin(GL_TRIANGLES);
	m3dFindNormal(vNormal, vNodeb[5],vNodeb[6],vNodeb[0]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNodeb[0]);
	glTexCoord2f(0.471f,0.0f);
	glVertex3fv(vNodeb[6]);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNodeb[5]);

	m3dFindNormal(vNormal, vNodeb[3],vNodeb[4],vNodeb[5]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNodeb[5]);
	glTexCoord2f(0.964f,0.471f);
	glVertex3fv(vNodeb[4]);
	glTexCoord2f(0.936f,0.857f);
	glVertex3fv(vNodeb[3]);

	m3dFindNormal(vNormal, vNodeb[1],vNodeb[2],vNodeb[3]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.936f,0.857f);
	glVertex3fv(vNodeb[3]);
	glTexCoord2f(0.563f,1.0f);
	glVertex3fv(vNodeb[2]);
	glTexCoord2f(0.071f,1.0f);
	glVertex3fv(vNodeb[1]);

	m3dFindNormal(vNormal, vNodeb[5],vNodeb[0],vNodeb[1]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.071f,1.0f);
	glVertex3fv(vNodeb[1]);
	glTexCoord2f(0.02f,0.686f);
	glVertex3fv(vNodeb[0]);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNodeb[5]);

	m3dFindNormal(vNormal, vNodeb[1],vNodeb[3],vNodeb[5]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.786f,0.0f);
	glVertex3fv(vNodeb[5]);
	glTexCoord2f(0.936f,0.857f);
	glVertex3fv(vNodeb[3]);
	glTexCoord2f(0.071f,1.0f);
	glVertex3fv(vNodeb[1]);

	glEnd();

	//使手掌封闭（顺时针）
	glBegin(GL_QUADS);
	//glNormal3i(-4,1,0);
	m3dFindNormal(vNormal, vNodeb[1],vNodeb[0],vNode[0]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[0]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[0]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[1]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[1]);
	
	m3dFindNormal(vNormal, vNodeb[2],vNodeb[1],vNode[1]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[1]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[1]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[2]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[2]);

	m3dFindNormal(vNormal, vNodeb[3],vNodeb[2],vNode[2]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[2]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[2]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[3]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[3]);

	m3dFindNormal(vNormal, vNodeb[4],vNodeb[3],vNode[3]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[3]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[3]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[4]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[4]);

	m3dFindNormal(vNormal, vNodeb[5],vNodeb[4],vNode[4]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[4]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[4]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[5]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[5]);

	m3dFindNormal(vNormal, vNodeb[6],vNodeb[5],vNode[5]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[5]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[5]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[6]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[6]);

	m3dFindNormal(vNormal, vNodeb[0],vNodeb[6],vNode[6]);
	glNormal3fv(vNormal);
	glTexCoord2f(0.0f,0.0f);
	glVertex3fv(vNode[6]);
    glTexCoord2f(0.0f,1.0f);
	glVertex3fv(vNodeb[6]);
	glTexCoord2f(0.5f,1.0f);
	glVertex3fv(vNodeb[0]);
	glTexCoord2f(0.5f,0.0f);
	glVertex3fv(vNode[0]);

	glEnd();
	
	//画手指
	pObj=gluNewQuadric();
	gluQuadricNormals(pObj,GLU_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	gluQuadricDrawStyle(pObj,GLU_FILL);
	gluQuadricOrientation(pObj,GLU_OUTSIDE);
	gluQuadricTexture(pObj, GL_TRUE);
   //
    //画大拇指
	//glBindTexture(GL_TEXTURE_2D, textureObjects[0]);
	//glDisable(GL_CULL_FACE);
	glPushMatrix();
		//glColor3ub(227,207,87);
	    //该圆柱体作为手掌的一部分
	    glPushMatrix();
		glTranslatef(-8.5f,-37.0f,-6.5f);
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		glRotatef(-31.0f,0.0f,1.0f,0.0f);
		//gluSphere(pObj,15.0f,50,25);
		gluCylinder(pObj,16.0f,10.3f,53.0f,50,25);
		glPopMatrix();

		glTranslatef(-37.3f,10.0f,-7.0f); 
		gluSphere(pObj,10.5f,30,15);
		
		//glColor3ub(245, 222, 179);
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		glRotatef(-30.0f,0.0f,1.0f,0.0f);
		glRotatef(x[0],0.5f,0.0f,0.866f);
		gluCylinder(pObj,10.5f,8.8f,21.0f,50,25);
	
		glTranslatef(0.0f,0.0f,21.0f);
		gluSphere(pObj,9.0f,50,25);

       if(x[0]<=135){
		    gluCylinder(pObj,9.0f,6.0f,18.0f,50,25);

			glTranslatef(0.0f,0.0f,18.0f);
			gluSphere(pObj,6.0f,50,25);
	   }else{
			glRotatef(y[0],1.0f,-1.0f,-1.0f);	
			gluCylinder(pObj,9.5f,6.0f,18.0f,50,25);

		    glTranslatef(0.0f,0.0f,18.0f);
		    gluSphere(pObj,6.0f,50,25);
		}
        if(x[0]<=135)x[0]+=5.0f;
		else
			if(y[0]<=60)y[0]=y[0]+5;
		
	glPopMatrix();

	//画食指
	glPushMatrix();
		//glColor3ub(227,207,87);
		glTranslatef(-22.5f,40.0f,-7.5f);
		gluSphere(pObj,7.5f,50,25);

		glPushMatrix();
			//glColor3ub(245, 222, 179);
			glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
			gluCylinder(pObj,7.5f,7.5f,18.0f,50,25);
		glPopMatrix();

		//glColor3ub(227,207,87);
		glPushMatrix();
			glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
			glTranslatef(0.0f,0.0f,18.0f);
			gluSphere(pObj,7.5f,50,25);
		glPopMatrix();
		if(x[1]<90){
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,18.0f);
				gluCylinder(pObj,7.5f,6.0f,16.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
				glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,34.0f);
				gluSphere(pObj,6.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,34.0f);
				gluCylinder(pObj,6.0f,4.0f,14.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				 glRotatef(-(90.0f-x[1]),1.0f,0.0f,0.0f); 
				 glTranslatef(0.0f,0.0f,48.0f);
				 gluSphere(pObj,4.0f,50,25);
			glPopMatrix();
		}else{
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,18.0f);
				glRotatef(y[1],1.0f,0.0f,0.0f);
				gluCylinder(pObj,7.5f,6.0f,16.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
			    glTranslatef(0.0f,0.0f,18.0f);
				glRotatef(y[1],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,16.0f);
				gluSphere(pObj,6.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,18.0f);
				glRotatef(y[1],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,16.0f);
				gluCylinder(pObj,6.0f,4.0f,14.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
		    	glTranslatef(0.0f,0.0f,18.0f);
				glRotatef(y[1],1.0f,0.0f,0.0f); 
				glTranslatef(0.0f,0.0f,30.0f);
				gluSphere(pObj,4.0f,50,25);
			glPopMatrix();
		}

		if(y[0]>60){
			if(x[1]<90)x[1]+=5.0f;
			else
				if(y[1]<90)y[1]+=5.0f;
		}
		//else
			//if(y[0]<60)y[0]=y[0]+5;

	glPopMatrix();
	
	//画中指
	glPushMatrix();
		//glColor3ub(227,207,87);
		glTranslatef(-5.4f,40.0f,-7.5f);
		gluSphere(pObj,8.8f,50,25);

		glPushMatrix();
			//glColor3ub(245, 222, 179);
			glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
			gluCylinder(pObj,8.8f,8.8f,20.0f,50,25);
		glPopMatrix();

		glPushMatrix();
		//glColor3ub(227,207,87);
			glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
			glTranslatef(0.0f,0.0f,20.0f);
			gluSphere(pObj,8.8f,50,25);
		glPopMatrix();

		if(x[2]<90){
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,20.0f);
				gluCylinder(pObj,8.8f,7.5f,18.0f,50,25);
			glPopMatrix();

			//glColor3ub(227,207,87);
			glPushMatrix();
				glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,38.0f);
				gluSphere(pObj,7.5f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,38.0f);
				gluCylinder(pObj,7.5f,5.2f,16.0f,50,25);
			glPopMatrix();

			//glColor3ub(245,222,179);
			glPushMatrix();
				glRotatef(-(90.0f-x[2]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,54.0f);
				gluSphere(pObj,5.2f,50,25);
			glPopMatrix();
		}
		else{
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[2],1.0f,0.0f,0.0f);
				gluCylinder(pObj,8.8f,7.5f,18.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
			    glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[2],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,18.0f);
				gluSphere(pObj,7.5f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[2],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,18.0f);
				gluCylinder(pObj,7.5f,5.2f,16.0f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
		    	glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[2],1.0f,0.0f,0.0f); 
				glTranslatef(0.0f,0.0f,34.0f);
				gluSphere(pObj,5.2f,50,25);
			glPopMatrix();
		}


		if(x[1]>=90){
			if(x[2]<90)x[2]+=5.0f;
			else
				if(y[2]<90)y[2]+=5.0f;
		}

	glPopMatrix();
		
	//画无名指
	glPushMatrix();
		//glColor3ub(227,207,87);
		glTranslatef(12.2f,35.62f,-7.5f);
		gluSphere(pObj,7.8f,50,25);

		glPushMatrix();
			//glColor3ub(245, 222, 179);
			glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
			gluCylinder(pObj,7.8f,7.8f,19.0f,50,25);
		glPopMatrix();

		glPushMatrix();
		//glColor3ub(227,207,87);
			glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
			glTranslatef(0.0f,0.0f,20.0f);
			gluSphere(pObj,7.8f,50,25);
		glPopMatrix();
		
		if(x[3]<90){
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,20.0f);
				gluCylinder(pObj,7.8f,6.6f,17.0f,50,25);
			glPopMatrix();

			//glColor3ub(227,207,87);
			glPushMatrix();
				glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,38.0f);
				gluSphere(pObj,6.6f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,38.0f);
				gluCylinder(pObj,6.6f,4.5f,15.0f,50,25);
			glPopMatrix();

			//glColor3ub(245,222,179);
			glPushMatrix();
				glRotatef(-(90.0f-x[3]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,54.0f);
				gluSphere(pObj,4.5f,50,25);
			glPopMatrix();
		}
		else{
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[3],1.0f,0.0f,0.0f);
				gluCylinder(pObj,7.8f,6.6f,17.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
			    glTranslatef(0.0f,0.0f,20.0f);
				glRotatef(y[3],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,18.0f);
				gluSphere(pObj,6.6f,50,25);
			glPopMatrix();
		    if(y[3]<90){
				glPushMatrix();
					//glColor3ub(245, 222, 179);
					glTranslatef(0.0f,0.0f,20.0f);
					glRotatef(y[3],1.0f,0.0f,0.0f);
					glTranslatef(0.0f,0.0f,18.0f);
					gluCylinder(pObj,6.6f,4.5f,16.0f,50,25);
				glPopMatrix();

				glPushMatrix();
					//glColor3ub(245, 222, 179);
		    		glTranslatef(0.0f,0.0f,20.0f);
					glRotatef(y[3],1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,34.0f);
					gluSphere(pObj,4.5f,50,25);
				glPopMatrix();
			}
			else{
				glPushMatrix();
					//glColor3ub(245, 222, 179);
					glTranslatef(0.0f,0.0f,20.0f);
					glRotatef(90,1.0f,0.0f,0.0f);
					glTranslatef(0.0f,0.0f,18.0f);
					glRotatef(z[0],1.0f,0.0f,0.0f);
					gluCylinder(pObj,6.6f,4.5f,16.0f,50,25);
				glPopMatrix();

				glPushMatrix();
					//glColor3ub(245, 222, 179);
		    		glTranslatef(0.0f,0.0f,20.0f);
					glRotatef(90,1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,18.0f);
					glRotatef(z[0],1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,16.0f);
					gluSphere(pObj,4.5f,50,25);
				glPopMatrix();
			}
		}

		if(x[2]>=90){
			if(x[3]<90)x[3]+=5.0f;
			else
				if(y[3]<90)y[3]+=5.0f;
				else
					if(z[0]<20)z[0]+=3.0f;
		}
	glPopMatrix();

	//画小指
	glPushMatrix();
		//glColor3ub(227,207,87);
		glTranslatef(26.0f,33.0f,-7.5f);
		gluSphere(pObj,5.5f,50,25);

		glPushMatrix();
			//glColor3ub(245, 222, 179);
			glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
			gluCylinder(pObj,5.5f,5.5f,16.0f,50,25);
		glPopMatrix();

		glPushMatrix();
		//glColor3ub(227,207,87);
			glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
			glTranslatef(0.0f,0.0f,16.0f);
			gluSphere(pObj,5.5f,50,25);
		glPopMatrix();
		
		if(x[4]<90){
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,16.0f);
				gluCylinder(pObj,5.5f,4.5f,15.0f,50,25);
			glPopMatrix();

			//glColor3ub(227,207,87);
			glPushMatrix();
				glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,31.0f);
				gluSphere(pObj,4.5f,50,25);
			glPopMatrix();

			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,31.0f);
				gluCylinder(pObj,4.5f,3.2f,11.0f,50,25);
			glPopMatrix();

			//glColor3ub(245,222,179);
			glPushMatrix();
				glRotatef(-(90.0f-x[4]),1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,42.0f);
				gluSphere(pObj,3.2f,50,25);
			glPopMatrix();
		}
		else{
			glPushMatrix();
				//glColor3ub(245, 222, 179);
				glTranslatef(0.0f,0.0f,16.0f);
				glRotatef(y[4],1.0f,0.0f,0.0f);
				gluCylinder(pObj,5.5f,4.5f,15.0f,50,25);
			glPopMatrix();

			glPushMatrix();
			//glColor3ub(227,207,87);
			    glTranslatef(0.0f,0.0f,16.0f);
				glRotatef(y[4],1.0f,0.0f,0.0f);
				glTranslatef(0.0f,0.0f,15.0f);
				gluSphere(pObj,4.5f,50,25);
			glPopMatrix();
			if(y[4]<90){
				glPushMatrix();
					//glColor3ub(245, 222, 179);
					glTranslatef(0.0f,0.0f,16.0f);
					glRotatef(y[4],1.0f,0.0f,0.0f);
					glTranslatef(0.0f,0.0f,15.0f);
					gluCylinder(pObj,4.5f,3.2f,11.0f,50,25);
				glPopMatrix();

				glPushMatrix();
					//glColor3ub(245, 222, 179);
		    		glTranslatef(0.0f,0.0f,16.0f);
					glRotatef(y[4],1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,26.0f);
					gluSphere(pObj,3.2f,50,25);
				glPopMatrix();
			}
			else{
				glPushMatrix();
					//glColor3ub(245, 222, 179);
					glTranslatef(0.0f,0.0f,16.0f);
					glRotatef(90,1.0f,0.0f,0.0f);
					glTranslatef(0.0f,0.0f,15.0f);
					glRotatef(z[1],1.0f,0.0f,0.0f);
					gluCylinder(pObj,4.5f,3.2f,11.0f,50,25);
				glPopMatrix();

				glPushMatrix();
					//glColor3ub(245, 222, 179);
		    		glTranslatef(0.0f,0.0f,16.0f);
					glRotatef(90,1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,15.0f);
					glRotatef(z[1],1.0f,0.0f,0.0f); 
					glTranslatef(0.0f,0.0f,11.0f);
					gluSphere(pObj,3.2f,50,25);
				glPopMatrix();

			}
		}

		if(z[0]>=20){
			if(x[4]<90)x[4]+=5.0f;
			else
				if(y[4]<90)y[4]+=5.0f;
				else
					if(z[1]<25)z[1]+=3.0f;
		}

	glPopMatrix();

	glPopMatrix();


 // if(surfName2)
 // gluDeleteNurbsRenderer(surfName2);
	SwapBuffers(g_hDC);									// Swap the backbuffers to the foreground
	 QueryPerformanceCounter(&EndTime); 
	outStream.open("time4.txt",ios::in|ios::app);
	//outStream.seekp(per,ios::cur);
	outStream.setf(ios::fixed);
		outStream.setf(ios::showpoint);
		outStream.precision(8);	
		time=(double)( EndTime.QuadPart - BegainTime.QuadPart )/ Frequency.QuadPart;
		outStream << time/9<<endl;  
		
		outStream.close();

	//	per+=10;
}
