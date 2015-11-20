
// OpenGLDlg.cpp : implementation file 
// 
#include "stdafx.h" 
#include "human.h" 
#include <fstream>  

using namespace std;
#define GL_PI 3.1415f
//const float PI = 3.1415926f;
//char g_sAppPath[512];
//模型路径
//char g_sMediaPath[512];
//纹理号表（100个）
//UINT g_Texture[100] = {0};		
//C3DSModel g_3dsModel[10];

float xe = 2.7182818;
//前半部分横方向q；前半部分纵方向w；后半部分横方向e；后半部分纵方向r；

int q[10] = { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 };
int w[10] = { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 };
int e[10] = { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 };
int r[10] = { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 };

//横方向为h，纵方向为v
int h[10] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };
int v[10] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
float B1[13][21];
float B2[13][21];
float B3[13][21];
float C1[9][16];
float C2[9][16];
float C3[9][16];

//臀部控制点及型值点
float B4[10][21];
float B5[10][21];
float B6[10][21];
float C4[6][16];
float C5[6][16];
float C6[6][16];

//大腿控制点及型值点
float B7[10][13];
float B8[10][13];
float B9[10][13];
float C7[6][8];
float C8[6][8];
float C9[6][8];


float B10[10][13];
float B11[10][13];
float B12[10][13];
float C10[6][8];
float C11[6][8];
float C12[6][8];
float C10_2[6][8];
float C11_2[6][8];
float C12_2[6][8];

float B13[10][13];
float B14[10][13];
float B15[10][13];
float C13[6][8];
float C14[6][8];
float C15[6][8];

float B16[10][13];
float B17[10][13];
float B18[10][13];
float C16[6][8];
float C17[6][8];
float C18[6][8];
float C16_2[6][8];
float C17_2[6][8];
float C18_2[6][8];

float B19[8][13];
float B20[8][13];
float B21[8][13];
float C19[4][8];
float C20[4][8];
float C21[4][8];


float B22[8][13];
float B23[8][13];
float B24[8][13];
float C22[4][8];
float C23[4][8];
float C24[4][8];

float B25[8][13];
float B26[8][13];
float B27[8][13];

float B28[8][13];
float B29[8][13];
float B30[8][13];
float B31[8][13];
float B32[8][13];
float B33[8][13];

float B34[8][13];
float B35[8][13];
float B36[8][13];


M3DMatrix44f rotationMatrix1, rotationMatrix2, rotationMatrix3, translationMatrix, transfoormationMatrix1, transfoormationMatrix2;


//右手臂初始型值点
M3DVector3f control_Elbow[8][8];
M3DVector3f control1_Elbow[8][8];

/*右手臂型值点定义，共11层*/
M3DVector3f XZ_Point_Righthand[11][8];
/*右手臂型值点.end*/

//特征点定义
M3DVector3f head_point = { 0.0f, 65.0f, 0.0f };
/*右手臂特征点位置，肘部、腕部*/
M3DVector3f Elbow_point = { 35.4f, 46.5f, 0.0f };
M3DVector3f Elbow_point_2 = { 53.4f, 35.5f, 4.5f };
M3DVector3f L_Elbow_point = { 35.4f, 46.5f, 0.0f };
M3DVector3f L_Elbow_point_2 = { 53.4f, 35.5f, 4.5f };
//腰部特征点
M3DVector3f waist_point = { 0.0f, 19.0f, 0.0f };
//膝盖特征点
M3DVector3f calf_point = { 10.85f, -36.0f, 1.0f };
M3DVector3f calf_point_2 = { 12.85f, -74.0f, -4.0f };
M3DVector3f L_calf_point = { 10.85f, -36.0f, -1.0f };
M3DVector3f L_calf_point_2 = { 12.85f, -74.0f, -4.0f };
M3DVector3f Shoulder_point = { 7.5f, 57.0f, 0.0f };
M3DVector3f L_Shoulder_point = { 7.5f, 57.0f, 0.0f };


//特征尺寸定义

float neck_length = 5.4;
float Shoulder_length = 15.3;
float chest_length = 12.5;
float waist_length = 12.8;	//腰部宽度
float hip_length = 14.7;	//臀部宽度
float Thigh_length = 14.5;	//大腿宽度
//float calf_length = 8.6;
//float arm_length = 3;
float upperarm_length = 4.0;
float upperarm1_length = 3.7;

float side_chest_width = 21;	//胸围
float side_waist_width = 15;	//腰围
float side_boody_width = 23;	//臀围



//float Thigh_length=17.0;
//M3DVector3f calf_point={8.85f,-36.0f,0.0f};
//M3DVector3f calf_point_2={8.85f,-74.0f,0.0f};

//M3DVector3f calf_point={8.85f,-32.0f,16.5f};	
//M3DVector3f calf_point_2={8.85f,-70.0f,16.5f};

//M3DVector3f calf_point={8.85f,-35.0f,8.4f};
//M3DVector3f calf_point_2={8.85f,-73.0f,8.4f};

//M3DVector3f calf_point={8.85f,-34.0f,11.8f};
//M3DVector3f calf_point_2={8.85f,-72.0f,11.8f};
//
//M3DVector3f calf_point={8.85f,-33.0f,14.4f};
//M3DVector3f calf_point_2={8.85f,-71.0f,14.4f};
M3DVector3f calf_temp_point[6];
M3DVector3f calf_temp_point_2[6];
M3DVector3f body_temp_point[9];
//右手臂中心点
M3DVector3f upperarm_temp_point[9];
//M3DVector3f upperarm_temp_point_2[6];

M3DVector3f arm_temp_point[4];


M3DVector3f vNormal;
M3DVector3f vPoints[3];


float asf[6];
//first~tenth 表示人体十个部分，_x，_y是中间结果，_xx，_yy，_z是最终需要用到的结果，即表面点的坐标值
float first_x[21][21];
float  first_y[21][21];
float  first_z[21][21];
float  first_xx[21][21];
float  first_yy[21][21];
float second_x[21][21];
float second_y[21][21];
float second_z[21][21];
float second_xx[21][21];
float second_yy[21][21];
float third_x[21][21];
float third_y[21][21];
float third_z[21][21];
float third_xx[21][21];
float third_yy[21][21];
float fourth_x[21][21];
float fourth_y[21][21];
float fourth_z[21][21];
float fourth_xx[21][21];
float fourth_yy[21][21];
float fifth_x[21][21];
float fifth_y[21][21];
float fifth_z[21][21];
float fifth_xx[21][21];
float fifth_yy[21][21];
float sixth_x[21][21];
float sixth_y[21][21];
float sixth_z[21][21];
float sixth_xx[21][21];
float sixth_yy[21][21];
float seventh_x[21][21];
float seventh_y[21][21];
float seventh_z[21][21];
float seventh_xx[21][21];
float seventh_yy[21][21];
float eighth_x[21][21];
float eighth_y[21][21];
float eighth_z[21][21];
float eighth_xx[21][21];
float eighth_yy[21][21];
float ninth_x[21][21];
float ninth_y[21][21];
float ninth_z[21][21];
float ninth_xx[21][21];
float ninth_yy[21][21];
float tenth_x[21][21];
float tenth_y[21][21];
float tenth_z[21][21];
float tenth_xx[21][21];
float tenth_yy[21][21];
float a_1, a_2, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18;
float b_2, b_1, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10;
float g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, g18, g19, g20;
float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20;
float n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16, n17, n18, n19, n20;

float z, xxxx, kkkk;



float sf = 0.02f;
int i, j, k;
double a[200][50];
double b[10000];
int fram = 0;


M3DMatrix44f   transformationMatrix;     //new
M3DMatrix44f   transformationMatrix1;     //new
M3DMatrix44f   transformationMatrix2;     //new

M3DVector3f objectVertex;				//new
M3DVector3f transformedVertex;			//new
M3DVector3f transformedVertex1;			//new
M3DVector3f transformedVertex2;			//new
int u;

//	double C[3];
//double S[3];


//身体bo
//double boo1[3]={0,62,-2.6};
//double boo2[3]={0,35,-2.6};
double boo1[3] = { 165, 29, -2.6 };
double boo2[3] = { 169, 144, -2.6 };
double bor1 = 24;
double bor2 = 24;
double bor1to2 = 24;
double bor2to1 = 24;
//double bor2=13.0;
double bor3 = 2.6;
//double bor4=6.4;
//double bor4=7.2;
//double bor5=8.1;
double bor4 = 9.0;
double bor5 = 12.0;
double bor6 = 6.6;
double bor7 = -1.4;
double bor8 = -3.5;
double bor9 = -2.2;
double bor10 = -1.4;
double bointputx[10][10];
double bointputy[10][10];
double bointputz[10][10];
double bointputx1[10][10];
double bointputy1[10][10];
double bointputz1[10][10];
//头部he
double heo1[3] = { 165, 1, 0.0 };
double heo2[3] = { 165, 29, 0.0 };
double her1 = 2;
//	double her2=3.3;
//	double her3=3.0;
double her2 = 10;
double her3 = 12;
//	double her4=2.4;
double her4 = 10;
double her5 = 0.4;
double her6 = 4.8;
double her7 = 5.0;
double her8 = 3.6;
double her9 = -0.4;
double her10 = -3.9;
double her11 = -3.6;
double her12 = -3.8;
double heintputx[10][10];
double heintputy[10][10];
double heintputz[10][10];
double heintputx1[10][10];
double heintputy1[10][10];
double heintputz1[10][10];
//左大腿zd
double zdo1[3] = { 156, 144, -0.7 };
double zdo2[3] = { 155, 198, 2.4 };
double zdr1 = 22.5;
double zdr2 = 9.5;
double zdr3 = -3.3;
double zdr4 = -2.9;
double zdr5 = 3.3;
double zdr6 = 2.9;
double zdintputx[10][10];
double zdintputy[10][10];
double zdintputz[10][10];
double zdintputx1[10][10];
double zdintputy1[10][10];
double zdintputz1[10][10];
//左小腿zx
double zxo1[3] = { 155, 198, 2.4 };
double zxo2[3] = { 152, 236, -0.7 };
double zxr1 = 9.5;
double zxr2 = 9.5;
double zxr3 = -2.9;
double zxr4 = -2.7;
double zxr5 = 2.9;
double zxr6 = 2.7;
double zxintputx[10][10];
double zxintputy[10][10];
double zxintputz[10][10];
double zxintputx1[10][10];
double zxintputy1[10][10];
double zxintputz1[10][10];
//右大腿yd
double ydo1[3] = { 182, 144, -0.7 };
double ydo2[3] = { 187, 197, 2.4 };
double ydr1 = 13;
double ydr2 = 9.5;
double ydr3 = -3.3;
double ydr4 = -2.9;
double ydr5 = 3.3;
double ydr6 = 2.9;
double ydintputx[10][10];
double ydintputy[10][10];
double ydintputz[10][10];
double ydintputx1[10][10];
double ydintputy1[10][10];
double ydintputz1[10][10];

//右小腿yx
double yxo1[3] = { 187, 197, 2.4 };
double yxo2[3] = { 196, 232, -0.7 };
double yxr1 = 9.5;
double yxr2 = 9.5;
double yxr3 = -2.9;
//double yxr4=-3.3;
double yxr4 = -2.7;
double yxr5 = 2.9;
//double yxr6=3.3;
double yxr6 = 2.7;
double yxintputx[10][10];
double yxintputy[10][10];
double yxintputz[10][10];
double yxintputx1[10][10];
double yxintputy1[10][10];
double yxintputz1[10][10];

//右上臂ys
//	double yso1[3]={12.0,38.0,0.0};
//	double yso22[3]={21.0,22.5,0.0};
double yso1[3] = { 193, 64, -2.6 };
double yso22[3] = { 219, 92, -2.6 };

double ysr1 = 20;
double ysr2 = 16.5;
double ysr3 = -2.9;
double ysr4 = -2.5;
double ysr5 = 2.9;
double ysr6 = 2.5;
double ysintputx[10][10];
double ysintputy[10][10];
double ysintputz[10][10];
double ysintputx1[10][10];
double ysintputy1[10][10];
double ysintputz1[10][10];
//右小臂yb
double ybo1[3] = { 219, 92, -2.6 };
//double ybo22[3]={25.0,41.0,-2.6};
double ybo22[3] = { 246, 94, 3.4 };

double ybr1 = 16.5;
double ybr2 = 16.5;
double ybr3 = -2.5;
double ybr4 = -2.3;
double ybr5 = 2.5;
double ybr6 = 2.3;
double ybintputx[10][10];
double ybintputy[10][10];
double ybintputz[10][10];
double ybintputx1[10][10];
double ybintputy1[10][10];
double ybintputz1[10][10];

//左上臂ys
//	double yso1[3]={12.0,38.0,0.0};
//	double yso22[3]={21.0,22.5,0.0};
double zso1[3] = { 144, 58, -2.6 };
double zso22[3] = { 118, 90, -2.6 };

double zsr1 = 17;
double zsr2 = 16.5;
double zsr3 = -2.9;
double zsr4 = -2.5;
double zsr5 = 2.9;
double zsr6 = 2.5;
double zsintputx[10][10];
double zsintputy[10][10];
double zsintputz[10][10];
double zsintputx1[10][10];
double zsintputy1[10][10];
double zsintputz1[10][10];
//右小臂yb
double zbo1[3] = { 118, 90, -2.6 };
double zbo22[3] = { 103, 110, -2.6 };

double zbr1 = 16.5;
double zbr2 = 16.5;
double zbr3 = -2.5;
double zbr4 = -2.3;
double zbr5 = 2.5;
double zbr6 = 2.3;
double zbintputx[10][10];
double zbintputy[10][10];
double zbintputz[10][10];
double zbintputx1[10][10];
double zbintputy1[10][10];
double zbintputz1[10][10];

//右手手指向量
M3DVector3f yhand1 = { 1.0f, -1.0f, 0.0f };
//右手手掌内侧法向量旋转角
double anglehand1 = 30.0;
//左手手指向量
M3DVector3f zhand2 = { -1.0f, -1.0f, 0.0f };
//左手手掌内侧法向量旋转角
double anglehand2 = 180.0;


double intputx[10][10];
double intputy[10][10];
double intputz[10][10];


//double zxo1[3]={-3.9,20,2.4};
//double zxo2[3]={-3.9,2,-0.7};
//double zxr1=2.4;
//double zxr2=2.4;
//double zxr3=-2.9;
//double zxr4=-2.7;
//double zxr5=2.9;
//double zxr6=2.7;
//double zxintputx[10][10];
//double zxintputy[10][10];
//double zxintputz[10][10];
//double zxintputx1[10][10];
//double zxintputy1[10][10];
//double zxintputz1[10][10];
/*
float g_fEyeX=0;
float g_fEyeY=0;
float g_fEyeZ=10;
float g_fEyeX1=0;
float g_fEyeY1=0;
float g_fEyeZ1=0;
float s_angle = -90.0;
*/
//const int m0=10;
int dim1 = 10;
int dim2 = 10;
int crlp;
double* p;

GLfloat ctrlPoint6[6][6][3];
double ctrlPoints6[6][6][3];
GLfloat ctrlPoint14[6][6][3];
//	GLfloat ctrlPoint[15][15][3];
GLfloat ctrlPoint[6][6][3];
double ctrlPoints[6][6][3];
GLfloat ctrlPoint1[6][6][3];
double ctrlPoints1[6][6][3];
GLfloat ctrlPoint2[6][6][3];
GLfloat ctrlPoint3[6][6][3];
GLfloat ctrlPoint4[6][6][3];
double ctrlPoints4[6][6][3];
GLfloat ctrlPoint5[6][6][3];
double ctrlPoints5[6][6][3];
GLfloat ctrlPoint7[6][6][3];
double ctrlPoints7[6][6][3];
GLfloat ctrlPoint8[6][6][3];
double ctrlPoints8[6][6][3];
GLfloat ctrlPoint9[6][6][3];
double ctrlPoints9[6][6][3];
GLfloat ctrlPoint10[6][6][3];
double ctrlPoints10[6][6][3];
GLfloat ctrlPoint11[6][6][3];
double ctrlPoints11[6][6][3];

GLfloat ctrlPointys[6][6][3];
double ctrlPointsys[6][6][3];
GLfloat ctrlPointysz[6][6][3];
double ctrlPointsysz[6][6][3];
GLfloat ctrlPointyb[6][6][3];
double ctrlPointsyb[6][6][3];

GLfloat ctrlPointybz[6][6][3];
double ctrlPointsybz[6][6][3];

GLfloat ctrlPointzs[6][6][3];
double ctrlPointszs[6][6][3];

GLfloat ctrlPointzsz[6][6][3];
double ctrlPointszsz[6][6][3];

GLfloat ctrlPointzb[6][6][3];
double ctrlPointszb[6][6][3];

GLfloat ctrlPointzbz[6][6][3];
double ctrlPointszbz[6][6][3];

void Human::Init(bool gender_new)
{
	nRange = 100.0f;
	nL = nR = nT = nD = nRange;
	gender = gender_new;

	/*
	new_body_length = origin_body_length;
	new_boody_length = origin_boody_length;
	new_thigh_length = origin_thigh_length;
	new_calf_length = origin_calf_length;*/

	Calculation();
}

/*性别转换函数
false 为男性，true 为女性
*/
void Human::SetGender(bool gender_new)
{
	if (gender != gender_new)
	{
		gender = gender_new;
		Calculation();
	}
}


void Human::CalculationXZD()
{
	//臀部Y值型值点计算
	for (int i = 0; i<16; i++)
	{
		C5[0][i] = waist_point[1];
		C5[1][i] = waist_point[1] * 4 / 5;
		C5[2][i] = waist_point[1] * 3 / 5;
		C5[3][i] = waist_point[1] * 2 / 5;
		C5[4][i] = waist_point[1] * 1 / 5;
		C5[5][i] = waist_point[1] * 0;
	}

	//臀部X值型值点计算
	C4[0][4] = waist_length;
	C4[5][4] = hip_length;
	C4[1][4] = (hip_length - waist_length)*0.7 / 3.7 + waist_length;
	C4[2][4] = (hip_length - waist_length)*1.9 / 3.7 + waist_length;
	C4[3][4] = (hip_length - waist_length)*3.1 / 3.7 + waist_length;
	C4[4][4] = (hip_length - waist_length)*3.5 / 3.7 + waist_length;
	C4[0][0] = C4[1][0] = C4[2][0] = C4[3][0] = C4[4][0] = C4[5][0] = C4[0][8] = C4[1][8] = C4[2][8] = C4[3][8] = C4[4][8] = C4[5][8] = 0;
	for (int j = 0; j<6; j++)
	{
		C4[j][1] = C4[j][4] * 0.35;
		C4[j][2] = C4[j][4] * 0.7;
		C4[j][3] = C4[j][4] * 0.9;
		C4[j][5] = C4[j][4] * 0.9;
		C4[j][6] = C4[j][4] * 0.7;
		C4[j][7] = C4[j][4] * 0.35;
		for (int i = 9; i<16; i++)
		{
			C4[j][i] = -C4[j][16 - i];
		}
	}

	//臀部Z值型值点计算
	C6[0][4] = C6[1][4] = C6[2][4] = C6[3][4] = C6[4][4] = C6[5][4] = 0;

	side_boody_width = side_boody_width;

	//C6[5][0] = 3.8*C4[5][4] / 16.1;//C6[5]为大腿一开始
	//C6[5][8] = -9.4*C4[5][4] / 16.1;
	C6[5][0] = (3.8 / (3.8*2.25 + 9.4*1.13)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 12.052 / 17.437);
	C6[5][8] = -(9.4 / (3.8*2.25 + 9.4*1.13)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 12.052 / 17.437);
	C6[5][1] = 1.11*C6[5][0];
	C6[5][2] = 2.25*C6[5][0];
	C6[5][3] = 1.46*C6[5][0];
	C6[5][5] = 0.74*C6[5][8];
	C6[5][6] = 1.03*C6[5][8];
	C6[5][7] = 1.13*C6[5][8];

	//C6[4][0] = 7.1*C4[4][4] / 15.9;
	//C6[4][8] = -9.6*C4[4][4] / 15.9;
	C6[4][0] = (7.1 / (7.1*1.1 + 9.6*1.23)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 15.331 / 17.437);
	C6[4][8] = -(9.6 / (7.1*1.1 + 9.6*1.23)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 15.331 / 17.437);
	C6[4][1] = 0.78*C6[4][0];
	C6[4][2] = 1.1*C6[4][0];
	C6[4][3] = 0.68*C6[4][0];
	C6[4][5] = 0.67*C6[4][8];
	C6[4][6] = 1.04*C6[4][8];
	C6[4][7] = 1.23*C6[4][8];

	//C6[3][0] = 8.5*C4[3][4] / 15.5;
	//C6[3][8] = -10 * C4[3][4] / 15.5;	
	C6[3][0] = (8.5 / (8.5 + 10 * 1.23)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 17.25 / 17.437);
	C6[3][8] = -(10 / (8.5 + 10 * 1.23)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 17.25 / 17.437);
	C6[3][1] = 0.78*C6[3][0];
	C6[3][2] = 0.9*C6[3][0];
	C6[3][3] = 0.60*C6[3][0];
	C6[3][5] = 0.61*C6[3][8];
	C6[3][6] = 0.9*C6[3][8];
	C6[3][7] = 1.23*C6[3][8];

	//C6[2][0] = 8.1*C4[2][4] / 14.3;
	//C6[2][8] = -10 * C4[2][4] / 14.3;
	C6[2][0] = side_boody_width / (8.1 + 10 * 1.2) * 7.5;
	C6[2][8] = -side_boody_width / (8.1 + 10 * 1.2) * 9;
	C6[2][1] = 0.83*C6[2][0];
	C6[2][2] = 0.69*C6[2][0];
	C6[2][3] = 0.58*C6[2][0];
	C6[2][5] = 0.62*C6[2][8];
	C6[2][6] = 1 * C6[2][8];
	C6[2][7] = 1.2*C6[2][8];



	//C6[1][0] = 8 * C4[1][4] / 13.1;
	//C6[1][8] = -8.1*C4[1][4] / 13.1;
	//C6[1][0] = (8 / (8 + 8.1*1.19)) *  (C6[2][0] - C6[2][8]）*0.8 + （C6[0][0] - C6[0][8]）*0.6 ）;
	//C6[1][8] = -(8.1 / (8 + 8.1*1.19)) * ((C6[2][0] - C6[2][8] + C6[0][0] - C6[0][8]) / 2);
	C6[1][0] = (8 / (8 + 8.1*1.19))  * side_waist_width / (8.1 + 10 * 1.2) * (8.1 + 10) * 16.173 / 14.555;
	C6[1][8] = -(8.1 / (8 + 8.1*1.19)) * side_waist_width / (8.1 + 10 * 1.2) * (8.1 + 10) * 16.173 / 14.555;
	C6[1][1] = 0.94*C6[1][0];
	C6[1][2] = 0.69*C6[1][0];
	C6[1][3] = 0.54*C6[1][0];
	C6[1][5] = 0.56*C6[1][8];
	C6[1][6] = 1.04*C6[1][8];
	C6[1][7] = 1.19*C6[1][8];

	//C6[0][0] = 7.2*C4[0][4] / 12.4;
	//C6[0][8] = -6.9*C4[0][4] / 12.4;
	C6[0][0] = side_waist_width / (7.2 * 1.06 + 6.9) * 7.2;
	C6[0][8] = -side_waist_width / (7.2 * 1.06 + 6.9) * 6.9;
	C6[0][1] = 1.06*C6[0][0];
	C6[0][2] = 0.75*C6[0][0];
	C6[0][3] = 0.57*C6[0][0];
	C6[0][5] = 0.41*C6[0][8];
	C6[0][6] = 0.81*C6[0][8];
	C6[0][7] = 1 * C6[0][8];

	for (int i = 0; i<6; i++)
	{
		for (int j = 9; j<16; j++)
		{
			C6[i][j] = C6[i][16 - j];
		}
	}

	//右腿大腿Y值型值点
	for (int i = 0; i<8; i++)
	{
		C8[0][i] = 0;
		C8[5][i] = calf_point[1];
		C8[1][i] = calf_point[1] * 1 / 5;
		C8[2][i] = calf_point[1] * 2 / 5;
		C8[3][i] = calf_point[1] * 3 / 5;
		C8[4][i] = calf_point[1] * 4 / 5;
	}

	//右腿大腿X型值点
	for (int i = 0; i<3; i++)
	{
		calf_temp_point[5][i] = calf_point[i];
	}
	calf_temp_point[0][0] = hip_length - Thigh_length / 2;
	calf_temp_point[0][1] = 0.0f;
	calf_temp_point[0][2] = 0.0f;
	for (int i = 1; i<5; i++)
	{
		calf_temp_point[i][0] = (calf_temp_point[5][0] - calf_temp_point[0][0])*i / 5 + calf_temp_point[0][0];
		calf_temp_point[i][1] = (calf_temp_point[5][1] - calf_temp_point[0][1])*i / 5 + calf_temp_point[0][1];
		calf_temp_point[i][2] = (calf_temp_point[5][2] - calf_temp_point[0][2])*i / 5 + calf_temp_point[0][2];

	}

	asf[0] = Thigh_length / 14.5;
	asf[1] = (asf[0] - Thigh_length / 14.5) * 4 / 5 + Thigh_length / 14.5;
	asf[2] = (asf[0] - Thigh_length / 14.5) * 3 / 5 + Thigh_length / 14.5;
	asf[3] = (asf[0] - Thigh_length / 14.5) * 2 / 5 + Thigh_length / 14.5;
	asf[4] = (asf[0] - Thigh_length / 14.5) * 1 / 5 + Thigh_length / 14.5;
	asf[5] = Thigh_length / 14.5;

	for (int i = 0; i<6; i++)
	{
		C7[0][i] = C4[5][i + 2];
	}
	C7[0][6] = hip_length - Thigh_length;
	C7[0][7] = C4[5][1];

	C7[1][0] = C7[1][4] = calf_temp_point[1][0];
	C7[2][0] = C7[2][4] = calf_temp_point[2][0];
	C7[3][0] = C7[3][4] = calf_temp_point[3][0];
	C7[4][0] = C7[4][4] = calf_temp_point[4][0];
	C7[5][0] = C7[5][4] = calf_temp_point[5][0];

	C7[1][2] = calf_temp_point[1][0] + 7.5*asf[1];
	C7[1][6] = calf_temp_point[1][0] - 7.5*asf[1];
	C7[2][2] = calf_temp_point[2][0] + 7.1*asf[2];
	C7[2][6] = calf_temp_point[2][0] - 7.1*asf[2];
	C7[3][2] = calf_temp_point[3][0] + 6.1*asf[3];
	C7[3][6] = calf_temp_point[3][0] - 6.1*asf[3];
	C7[4][2] = calf_temp_point[4][0] + 5.4*asf[4];
	C7[4][6] = calf_temp_point[4][0] - 5.4*asf[4];
	C7[5][2] = calf_temp_point[5][0] + 4.3*asf[5];
	C7[5][6] = calf_temp_point[5][0] - 4.3*asf[5];

	for (int i = 1; i<6; i++)
	{
		C7[i][1] = (C7[i][2] - C7[i][0])*0.7 + C7[i][0];
		C7[i][3] = (C7[i][2] - C7[i][0])*0.7 + C7[i][0];
		C7[i][5] = (C7[i][6] - C7[i][0])*0.7 + C7[i][0];
		C7[i][7] = (C7[i][6] - C7[i][0])*0.7 + C7[i][0];
	}

	//右大腿Z值型值点
	for (int i = 0; i<6; i++)
	{
		C9[0][i] = C6[5][i + 2];
	}
	C9[0][6] = 0.0f;
	C9[0][7] = C6[5][1];

	C9[1][2] = C9[1][6] = calf_temp_point[1][2];
	C9[2][2] = C9[2][6] = calf_temp_point[2][2];
	C9[3][2] = C9[3][6] = calf_temp_point[3][2];
	C9[4][2] = C9[4][6] = calf_temp_point[4][2];
	C9[5][2] = C9[5][6] = calf_temp_point[5][2];

	C9[1][0] = calf_temp_point[1][2] + 8.0*asf[1];
	C9[1][4] = calf_temp_point[1][2] - 8.2*asf[1];
	C9[2][0] = calf_temp_point[2][2] + 7.6*asf[2];
	C9[2][4] = calf_temp_point[2][2] - 7.5*asf[2];
	C9[3][0] = calf_temp_point[3][2] + 7.2*asf[3];
	C9[3][4] = calf_temp_point[3][2] - 6.0*asf[3];
	C9[4][0] = calf_temp_point[4][2] + 6.5*asf[4];
	C9[4][4] = calf_temp_point[4][2] - 5.1*asf[4];
	C9[5][0] = calf_temp_point[5][2] + 6.4*asf[5];
	C9[5][4] = calf_temp_point[5][2] - 4.4*asf[5];

	C9[1][1] = (C9[1][0] - C9[1][2])*0.78 + C9[1][2];
	C9[1][3] = (C9[1][4] - C9[1][2])*0.72 + C9[1][2];
	C9[1][5] = (C9[1][4] - C9[1][2])*0.75 + C9[1][2];
	C9[1][7] = (C9[1][0] - C9[1][2])*0.56 + C9[1][2];

	C9[2][1] = (C9[2][0] - C9[2][2])*0.74 + C9[2][2];
	C9[2][3] = (C9[2][4] - C9[2][2])*0.71 + C9[2][2];
	C9[2][5] = (C9[2][4] - C9[2][2])*0.71 + C9[2][2];
	C9[2][7] = (C9[2][0] - C9[2][2])*0.68 + C9[2][2];

	C9[3][1] = (C9[3][0] - C9[3][2])*0.74 + C9[3][2];
	C9[3][3] = (C9[3][4] - C9[3][2])*0.71 + C9[3][2];
	C9[3][5] = (C9[3][4] - C9[3][2])*0.71 + C9[3][2];
	C9[3][7] = (C9[3][0] - C9[3][2])*0.70 + C9[3][2];

	C9[4][1] = (C9[4][0] - C9[4][2])*0.65 + C9[4][2];
	C9[4][3] = (C9[4][4] - C9[4][2])*0.65 + C9[4][2];
	C9[4][5] = (C9[4][4] - C9[4][2])*0.65 + C9[4][2];
	C9[4][7] = (C9[4][0] - C9[4][2])*0.6 + C9[4][2];

	C9[5][1] = (C9[5][0] - C9[5][2])*0.5 + C9[5][2];
	C9[5][3] = (C9[5][4] - C9[5][2])*0.5 + C9[5][2];
	C9[5][5] = (C9[5][4] - C9[5][2])*0.5 + C9[5][2];
	C9[5][7] = (C9[5][0] - C9[5][2])*0.5 + C9[5][2];

	//左小腿Y值型值点
	for (int i = 0; i<8; i++)
	{
		C14[0][i] = C8[5][i];
		C14[5][i] = calf_point_2[1];
		C14[1][i] = (calf_point_2[1] - calf_point[1]) * 1 / 5 + calf_point[1];
		C14[2][i] = (calf_point_2[1] - calf_point[1]) * 2 / 5 + calf_point[1];
		C14[3][i] = (calf_point_2[1] - calf_point[1]) * 3 / 5 + calf_point[1];
		C14[4][i] = (calf_point_2[1] - calf_point[1]) * 4 / 5 + calf_point[1];
	}
	for (int i = 0; i<3; i++)
	{
		calf_temp_point_2[5][i] = calf_point_2[i];
		calf_temp_point_2[0][i] = calf_point[i];
	}
	for (int i = 1; i<5; i++)
	{
		calf_temp_point_2[i][0] = (calf_temp_point_2[5][0] - calf_temp_point_2[0][0])*i / 5 + calf_temp_point_2[0][0];
		calf_temp_point_2[i][1] = (calf_temp_point_2[5][1] - calf_temp_point_2[0][1])*i / 5 + calf_temp_point_2[0][1];
		calf_temp_point_2[i][2] = (calf_temp_point_2[5][2] - calf_temp_point_2[0][2])*i / 5 + calf_temp_point_2[0][2];

	}
	asf[0] = Thigh_length / 14.5;
	asf[1] = Thigh_length / 14.5;
	asf[2] = Thigh_length / 14.5;
	asf[3] = Thigh_length / 14.5;
	asf[4] = Thigh_length / 14.5;
	asf[5] = Thigh_length / 14.5;

	//左小腿X值型值点
	for (int i = 0; i<8; i++)
	{

		C13[0][i] = C7[5][i];
	}
	C13[1][0] = C13[1][4] = calf_temp_point_2[1][0];
	C13[2][0] = C13[2][4] = calf_temp_point_2[2][0];
	C13[3][0] = C13[3][4] = calf_temp_point_2[3][0];
	C13[4][0] = C13[4][4] = calf_temp_point_2[4][0];
	C13[5][0] = C13[5][4] = calf_temp_point_2[5][0];
	C13[1][2] = calf_temp_point_2[1][0] + 4.9*asf[1];
	C13[1][6] = calf_temp_point_2[1][0] - 4.9*asf[1];
	C13[2][2] = calf_temp_point_2[2][0] + 5.0*asf[2];
	C13[2][6] = calf_temp_point_2[2][0] - 5.0*asf[2];
	C13[3][2] = calf_temp_point_2[3][0] + 4.1*asf[3];
	C13[3][6] = calf_temp_point_2[3][0] - 4.1*asf[3];
	C13[4][2] = calf_temp_point_2[4][0] + 3.5*asf[4];
	C13[4][6] = calf_temp_point_2[4][0] - 3.5*asf[4];
	C13[5][2] = calf_temp_point_2[5][0] + 2.5*asf[5];
	C13[5][6] = calf_temp_point_2[5][0] - 2.5*asf[5];
	for (int i = 1; i<6; i++)
	{
		C13[i][1] = (C13[i][2] - C13[i][0])*0.7 + C13[i][0];
		C13[i][3] = (C13[i][2] - C13[i][0])*0.7 + C13[i][0];
		C13[i][5] = (C13[i][6] - C13[i][0])*0.7 + C13[i][0];
		C13[i][7] = (C13[i][6] - C13[i][0])*0.7 + C13[i][0];
	}

	//左小腿Z值型值点
	for (int i = 0; i<8; i++)
	{
		C15[0][i] = C9[5][i];

	}

	C15[1][2] = C15[1][6] = calf_temp_point_2[1][2];
	C15[2][2] = C15[2][6] = calf_temp_point_2[2][2];
	C15[3][2] = C15[3][6] = calf_temp_point_2[3][2];
	C15[4][2] = C15[4][6] = calf_temp_point_2[4][2];
	C15[5][2] = C15[5][6] = calf_temp_point_2[5][2];
	C15[1][0] = calf_temp_point_2[1][2] + 5.0*asf[1];
	C15[1][4] = calf_temp_point_2[1][2] - 6 * asf[1];
	C15[2][0] = calf_temp_point_2[2][2] + 5 * asf[2];
	C15[2][4] = calf_temp_point_2[2][2] - 6 * asf[2];
	C15[3][0] = calf_temp_point_2[3][2] + 4.1*asf[3];
	C15[3][4] = calf_temp_point_2[3][2] - 4.5*asf[3];
	C15[4][0] = calf_temp_point_2[4][2] + 3 * asf[4];
	C15[4][4] = calf_temp_point_2[4][2] - 3 * asf[4];
	C15[5][0] = calf_temp_point_2[5][2] + 2.5*asf[5];
	C15[5][4] = calf_temp_point_2[5][2] - 2.5*asf[5];

	C15[1][1] = (C15[1][0] - C15[1][2])*0.7 + C15[1][2];
	C15[1][3] = (C15[1][4] - C15[1][2])*0.7 + C15[1][2];
	C15[1][5] = (C15[1][4] - C15[1][2])*0.7 + C15[1][2];
	C15[1][7] = (C15[1][0] - C15[1][2])*0.7 + C15[1][2];

	C15[2][1] = (C15[2][0] - C15[2][2])*0.7 + C15[2][2];
	C15[2][3] = (C15[2][4] - C15[2][2])*0.71 + C15[2][2];
	C15[2][5] = (C15[2][4] - C15[2][2])*0.71 + C15[2][2];
	C15[2][7] = (C15[2][0] - C15[2][2])*0.68 + C15[2][2];

	C15[3][1] = (C15[3][0] - C15[3][2])*0.7 + C15[3][2];
	C15[3][3] = (C15[3][4] - C15[3][2])*0.71 + C15[3][2];
	C15[3][5] = (C15[3][4] - C15[3][2])*0.71 + C15[3][2];
	C15[3][7] = (C15[3][0] - C15[3][2])*0.70 + C15[3][2];

	C15[4][1] = (C15[4][0] - C15[4][2])*0.7 + C15[4][2];
	C15[4][3] = (C15[4][4] - C15[4][2])*0.7 + C15[4][2];
	C15[4][5] = (C15[4][4] - C15[4][2])*0.7 + C15[4][2];
	C15[4][7] = (C15[4][0] - C15[4][2])*0.7 + C15[4][2];

	C15[5][1] = (C15[5][0] - C15[5][2])*0.7 + C15[5][2];
	C15[5][3] = (C15[5][4] - C15[5][2])*0.7 + C15[5][2];
	C15[5][5] = (C15[5][4] - C15[5][2])*0.7 + C15[5][2];
	C15[5][7] = (C15[5][0] - C15[5][2])*0.7 + C15[5][2];

	//右大腿Y值型值点
	//L_calf_point[0] = -L_calf_point[0];
	for (int i = 0; i<8; i++)
	{
		C11_2[0][i] = 0;
		C11_2[5][i] = L_calf_point[1];
		C11_2[1][i] = L_calf_point[1] * 1 / 5;
		C11_2[2][i] = L_calf_point[1] * 2 / 5;
		C11_2[3][i] = L_calf_point[1] * 3 / 5;
		C11_2[4][i] = L_calf_point[1] * 4 / 5;
	}

	//右腿大腿X型值点
	for (int i = 0; i<3; i++)
	{
		calf_temp_point[5][i] = L_calf_point[i];
	}
	calf_temp_point[0][0] = hip_length - Thigh_length / 2;
	calf_temp_point[0][1] = 0.0f;
	calf_temp_point[0][2] = 0.0f;
	for (int i = 1; i<5; i++)
	{
		calf_temp_point[i][0] = (calf_temp_point[5][0] - calf_temp_point[0][0])*i / 5 + calf_temp_point[0][0];
		calf_temp_point[i][1] = (calf_temp_point[5][1] - calf_temp_point[0][1])*i / 5 + calf_temp_point[0][1];
		calf_temp_point[i][2] = (calf_temp_point[5][2] - calf_temp_point[0][2])*i / 5 + calf_temp_point[0][2];

	}
	/*asf[0] = Thigh_length / 14.5;
	asf[1] = (asf[0] - 1) * 4 / 5 + 1;
	asf[2] = (asf[0] - 1) * 3 / 5 + 1;
	asf[3] = (asf[0] - 1) * 2 / 5 + 1;
	asf[4] = (asf[0] - 1) * 1 / 5 + 1;
	asf[5] = 1;*/

	asf[0] = Thigh_length / 14.5;
	asf[1] = (asf[0] - Thigh_length / 14.5) * 4 / 5 + Thigh_length / 14.5;
	asf[2] = (asf[0] - Thigh_length / 14.5) * 3 / 5 + Thigh_length / 14.5;
	asf[3] = (asf[0] - Thigh_length / 14.5) * 2 / 5 + Thigh_length / 14.5;
	asf[4] = (asf[0] - Thigh_length / 14.5) * 1 / 5 + Thigh_length / 14.5;
	asf[5] = Thigh_length / 14.5;

	for (int i = 0; i<6; i++)
	{
		C10_2[0][i] = C4[5][i + 2];
	}
	C10_2[0][6] = hip_length - Thigh_length;
	C10_2[0][7] = C4[5][1];

	C10_2[1][0] = C10_2[1][4] = calf_temp_point[1][0];
	C10_2[2][0] = C10_2[2][4] = calf_temp_point[2][0];
	C10_2[3][0] = C10_2[3][4] = calf_temp_point[3][0];
	C10_2[4][0] = C10_2[4][4] = calf_temp_point[4][0];
	C10_2[5][0] = C10_2[5][4] = calf_temp_point[5][0];

	C10_2[1][2] = calf_temp_point[1][0] + 7.5*asf[1];
	C10_2[1][6] = calf_temp_point[1][0] - 7.5*asf[1];
	C10_2[2][2] = calf_temp_point[2][0] + 7.1*asf[2];
	C10_2[2][6] = calf_temp_point[2][0] - 7.1*asf[2];
	C10_2[3][2] = calf_temp_point[3][0] + 6.1*asf[3];
	C10_2[3][6] = calf_temp_point[3][0] - 6.1*asf[3];
	C10_2[4][2] = calf_temp_point[4][0] + 5.4*asf[4];
	C10_2[4][6] = calf_temp_point[4][0] - 5.4*asf[4];
	C10_2[5][2] = calf_temp_point[5][0] + 4.3*asf[5];
	C10_2[5][6] = calf_temp_point[5][0] - 4.3*asf[5];

	for (int i = 1; i<6; i++)
	{
		C10_2[i][1] = (C10_2[i][2] - C10_2[i][0])*0.7 + C10_2[i][0];
		C10_2[i][3] = (C10_2[i][2] - C10_2[i][0])*0.7 + C10_2[i][0];
		C10_2[i][5] = (C10_2[i][6] - C10_2[i][0])*0.7 + C10_2[i][0];
		C10_2[i][7] = (C10_2[i][6] - C10_2[i][0])*0.7 + C10_2[i][0];
	}

	//右大腿Z值型值点
	for (int i = 0; i<6; i++)
	{
		C12_2[0][i] = C6[5][i + 2];
	}
	C12_2[0][6] = 0.0f;
	C12_2[0][7] = C6[5][1];

	C12_2[1][2] = C12_2[1][6] = calf_temp_point[1][2];
	C12_2[2][2] = C12_2[2][6] = calf_temp_point[2][2];
	C12_2[3][2] = C12_2[3][6] = calf_temp_point[3][2];
	C12_2[4][2] = C12_2[4][6] = calf_temp_point[4][2];
	C12_2[5][2] = C12_2[5][6] = calf_temp_point[5][2];

	C12_2[1][0] = calf_temp_point[1][2] + 8.0*asf[1];
	C12_2[1][4] = calf_temp_point[1][2] - 8.2*asf[1];
	C12_2[2][0] = calf_temp_point[2][2] + 7.6*asf[2];
	C12_2[2][4] = calf_temp_point[2][2] - 7.5*asf[2];
	C12_2[3][0] = calf_temp_point[3][2] + 7.2*asf[3];
	C12_2[3][4] = calf_temp_point[3][2] - 6.0*asf[3];
	C12_2[4][0] = calf_temp_point[4][2] + 6.5*asf[4];
	C12_2[4][4] = calf_temp_point[4][2] - 5.1*asf[4];
	C12_2[5][0] = calf_temp_point[5][2] + 6.4*asf[5];
	C12_2[5][4] = calf_temp_point[5][2] - 4.4*asf[5];

	C12_2[1][1] = (C12_2[1][0] - C12_2[1][2])*0.78 + C12_2[1][2];
	C12_2[1][3] = (C12_2[1][4] - C12_2[1][2])*0.72 + C12_2[1][2];
	C12_2[1][5] = (C12_2[1][4] - C12_2[1][2])*0.75 + C12_2[1][2];
	C12_2[1][7] = (C12_2[1][0] - C12_2[1][2])*0.56 + C12_2[1][2];

	C12_2[2][1] = (C12_2[2][0] - C12_2[2][2])*0.74 + C12_2[2][2];
	C12_2[2][3] = (C12_2[2][4] - C12_2[2][2])*0.71 + C12_2[2][2];
	C12_2[2][5] = (C12_2[2][4] - C12_2[2][2])*0.71 + C12_2[2][2];
	C12_2[2][7] = (C12_2[2][0] - C12_2[2][2])*0.68 + C12_2[2][2];

	C12_2[3][1] = (C12_2[3][0] - C12_2[3][2])*0.74 + C12_2[3][2];
	C12_2[3][3] = (C12_2[3][4] - C12_2[3][2])*0.71 + C12_2[3][2];
	C12_2[3][5] = (C12_2[3][4] - C12_2[3][2])*0.71 + C12_2[3][2];
	C12_2[3][7] = (C12_2[3][0] - C12_2[3][2])*0.70 + C12_2[3][2];

	C12_2[4][1] = (C12_2[4][0] - C12_2[4][2])*0.65 + C12_2[4][2];
	C12_2[4][3] = (C12_2[4][4] - C12_2[4][2])*0.65 + C12_2[4][2];
	C12_2[4][5] = (C12_2[4][4] - C12_2[4][2])*0.65 + C12_2[4][2];
	C12_2[4][7] = (C12_2[4][0] - C12_2[4][2])*0.6 + C12_2[4][2];

	C12_2[5][1] = (C12_2[5][0] - C12_2[5][2])*0.5 + C12_2[5][2];
	C12_2[5][3] = (C12_2[5][4] - C12_2[5][2])*0.5 + C12_2[5][2];
	C12_2[5][5] = (C12_2[5][4] - C12_2[5][2])*0.5 + C12_2[5][2];
	C12_2[5][7] = (C12_2[5][0] - C12_2[5][2])*0.5 + C12_2[5][2];

	for (int i = 0; i<6; i++)
	{
		for (int j = 0; j<8; j++)
		{
			C10[i][j] = -C10_2[i][(8 - j) % 8];
			C11[i][j] = C11_2[i][(8 - j) % 8];
			C12[i][j] = C12_2[i][(8 - j) % 8];

		}
	}

	//	L_calf_point[0]=-L_calf_point[0];
	//L_calf_point_2[0] = -L_calf_point_2[0];


	//右小腿Y值型值点
	for (int i = 0; i<8; i++)
	{
		C17_2[0][i] = C11_2[5][i];
		C17_2[5][i] = L_calf_point_2[1];
		C17_2[1][i] = (L_calf_point_2[1] - L_calf_point[1]) * 1 / 5 + L_calf_point[1];
		C17_2[2][i] = (L_calf_point_2[1] - L_calf_point[1]) * 2 / 5 + L_calf_point[1];
		C17_2[3][i] = (L_calf_point_2[1] - L_calf_point[1]) * 3 / 5 + L_calf_point[1];
		C17_2[4][i] = (L_calf_point_2[1] - L_calf_point[1]) * 4 / 5 + L_calf_point[1];
	}
	for (int i = 0; i<3; i++)
	{
		calf_temp_point_2[5][i] = L_calf_point_2[i];
		calf_temp_point_2[0][i] = L_calf_point[i];
	}
	for (int i = 1; i<5; i++)
	{
		calf_temp_point_2[i][0] = (calf_temp_point_2[5][0] - calf_temp_point_2[0][0])*i / 5 + calf_temp_point_2[0][0];
		calf_temp_point_2[i][1] = (calf_temp_point_2[5][1] - calf_temp_point_2[0][1])*i / 5 + calf_temp_point_2[0][1];
		calf_temp_point_2[i][2] = (calf_temp_point_2[5][2] - calf_temp_point_2[0][2])*i / 5 + calf_temp_point_2[0][2];

	}

	asf[0] = Thigh_length / 14.5;
	asf[1] = Thigh_length / 14.5;
	asf[2] = Thigh_length / 14.5;
	asf[3] = Thigh_length / 14.5;
	asf[4] = Thigh_length / 14.5;
	asf[5] = Thigh_length / 14.5;

	//右小腿X值型值点
	for (int i = 0; i<8; i++)
	{

		C16_2[0][i] = C10_2[5][i];
	}
	C16_2[1][0] = C16_2[1][4] = calf_temp_point_2[1][0];
	C16_2[2][0] = C16_2[2][4] = calf_temp_point_2[2][0];
	C16_2[3][0] = C16_2[3][4] = calf_temp_point_2[3][0];
	C16_2[4][0] = C16_2[4][4] = calf_temp_point_2[4][0];
	C16_2[5][0] = C16_2[5][4] = calf_temp_point_2[5][0];
	C16_2[1][2] = calf_temp_point_2[1][0] + 4.9*asf[1];
	C16_2[1][6] = calf_temp_point_2[1][0] - 4.9*asf[1];
	C16_2[2][2] = calf_temp_point_2[2][0] + 5.0*asf[2];
	C16_2[2][6] = calf_temp_point_2[2][0] - 5.0*asf[2];
	C16_2[3][2] = calf_temp_point_2[3][0] + 4.1*asf[3];
	C16_2[3][6] = calf_temp_point_2[3][0] - 4.1*asf[3];
	C16_2[4][2] = calf_temp_point_2[4][0] + 3.5*asf[4];
	C16_2[4][6] = calf_temp_point_2[4][0] - 3.5*asf[4];
	C16_2[5][2] = calf_temp_point_2[5][0] + 2.5*asf[5];
	C16_2[5][6] = calf_temp_point_2[5][0] - 2.5*asf[5];
	for (int i = 1; i<6; i++)
	{
		C16_2[i][1] = (C16_2[i][2] - C16_2[i][0])*0.7 + C16_2[i][0];
		C16_2[i][3] = (C16_2[i][2] - C16_2[i][0])*0.7 + C16_2[i][0];
		C16_2[i][5] = (C16_2[i][6] - C16_2[i][0])*0.7 + C16_2[i][0];
		C16_2[i][7] = (C16_2[i][6] - C16_2[i][0])*0.7 + C16_2[i][0];
	}

	//右小腿Z值型值点
	for (int i = 0; i<8; i++)
	{
		C18_2[0][i] = C12_2[5][i];

	}

	C18_2[1][2] = C18_2[1][6] = calf_temp_point_2[1][2];
	C18_2[2][2] = C18_2[2][6] = calf_temp_point_2[2][2];
	C18_2[3][2] = C18_2[3][6] = calf_temp_point_2[3][2];
	C18_2[4][2] = C18_2[4][6] = calf_temp_point_2[4][2];
	C18_2[5][2] = C18_2[5][6] = calf_temp_point_2[5][2];
	C18_2[1][0] = calf_temp_point_2[1][2] + 5.0*asf[1];
	C18_2[1][4] = calf_temp_point_2[1][2] - 6 * asf[1];
	C18_2[2][0] = calf_temp_point_2[2][2] + 5 * asf[2];
	C18_2[2][4] = calf_temp_point_2[2][2] - 6 * asf[2];
	C18_2[3][0] = calf_temp_point_2[3][2] + 4.1*asf[3];
	C18_2[3][4] = calf_temp_point_2[3][2] - 4.5*asf[3];
	C18_2[4][0] = calf_temp_point_2[4][2] + 3 * asf[4];
	C18_2[4][4] = calf_temp_point_2[4][2] - 3 * asf[4];
	C18_2[5][0] = calf_temp_point_2[5][2] + 2.5*asf[5];
	C18_2[5][4] = calf_temp_point_2[5][2] - 2.5*asf[5];

	C18_2[1][1] = (C18_2[1][0] - C18_2[1][2])*0.7 + C18_2[1][2];
	C18_2[1][3] = (C18_2[1][4] - C18_2[1][2])*0.7 + C18_2[1][2];
	C18_2[1][5] = (C18_2[1][4] - C18_2[1][2])*0.7 + C18_2[1][2];
	C18_2[1][7] = (C18_2[1][0] - C18_2[1][2])*0.7 + C18_2[1][2];

	C18_2[2][1] = (C18_2[2][0] - C18_2[2][2])*0.7 + C18_2[2][2];
	C18_2[2][3] = (C18_2[2][4] - C18_2[2][2])*0.71 + C18_2[2][2];
	C18_2[2][5] = (C18_2[2][4] - C18_2[2][2])*0.71 + C18_2[2][2];
	C18_2[2][7] = (C18_2[2][0] - C18_2[2][2])*0.68 + C18_2[2][2];

	C18_2[3][1] = (C18_2[3][0] - C18_2[3][2])*0.7 + C18_2[3][2];
	C18_2[3][3] = (C18_2[3][4] - C18_2[3][2])*0.71 + C18_2[3][2];
	C18_2[3][5] = (C18_2[3][4] - C18_2[3][2])*0.71 + C18_2[3][2];
	C18_2[3][7] = (C18_2[3][0] - C18_2[3][2])*0.70 + C18_2[3][2];

	C18_2[4][1] = (C18_2[4][0] - C18_2[4][2])*0.7 + C18_2[4][2];
	C18_2[4][3] = (C18_2[4][4] - C18_2[4][2])*0.7 + C18_2[4][2];
	C18_2[4][5] = (C18_2[4][4] - C18_2[4][2])*0.7 + C18_2[4][2];
	C18_2[4][7] = (C18_2[4][0] - C18_2[4][2])*0.7 + C18_2[4][2];

	C18_2[5][1] = (C18_2[5][0] - C18_2[5][2])*0.7 + C18_2[5][2];
	C18_2[5][3] = (C18_2[5][4] - C18_2[5][2])*0.7 + C18_2[5][2];
	C18_2[5][5] = (C18_2[5][4] - C18_2[5][2])*0.7 + C18_2[5][2];
	C18_2[5][7] = (C18_2[5][0] - C18_2[5][2])*0.7 + C18_2[5][2];

	for (int i = 0; i<6; i++)
	{
		for (int j = 0; j<8; j++)
		{
			C16[i][j] = -C16_2[i][(8 - j) % 8];
			C17[i][j] = C17_2[i][(8 - j) % 8];
			C18[i][j] = C18_2[i][(8 - j) % 8];

		}
	}

	body_temp_point[0][0] = head_point[0];
	body_temp_point[0][1] = head_point[1];
	body_temp_point[8][0] = waist_point[0];
	body_temp_point[8][1] = waist_point[1];
	body_temp_point[8][2] = waist_point[2];

	for (int i = 0; i<3; i++)
	{
		body_temp_point[1][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 43 / 46 + body_temp_point[8][i];
		body_temp_point[2][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 40 / 46 + body_temp_point[8][i];
		body_temp_point[3][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 34 / 46 + body_temp_point[8][i];
		body_temp_point[4][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 28 / 46 + body_temp_point[8][i];
		body_temp_point[5][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 22 / 46 + body_temp_point[8][i];
		body_temp_point[6][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 16 / 46 + body_temp_point[8][i];
		body_temp_point[7][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 8 / 46 + body_temp_point[8][i];

	}

	// 身体Y值型值点
	for (int i = 0; i<9; i++)
	{
		for (int j = 0; j<16; j++)
		{
			C2[i][j] = body_temp_point[i][1];

		}

	}

	// 身体X值型值点
	for (int i = 0; i<8; i++)
	{
		C1[i][0] = C1[i][8] = 0;
	}

	C1[0][4] = C1[1][4] = neck_length;
	C1[0][12] = C1[1][12] = -neck_length;
	C1[0][1] = C1[0][7] = C1[1][1] = C1[1][7] = neck_length*0.35;
	C1[0][2] = C1[0][6] = C1[1][2] = C1[1][6] = neck_length*0.7;
	C1[0][3] = C1[0][5] = C1[1][3] = C1[1][5] = neck_length*0.9;
	C1[0][9] = C1[0][15] = C1[1][9] = C1[1][15] = -neck_length*0.35;
	C1[0][10] = C1[0][14] = C1[1][10] = C1[1][14] = -neck_length*0.7;
	C1[0][11] = C1[0][13] = C1[1][11] = C1[1][13] = -neck_length*0.9;

	C1[2][4] = Shoulder_length;
	C1[2][12] = -Shoulder_length;
	C1[2][1] = C1[2][7] = Shoulder_length*0.35;
	C1[2][2] = C1[2][6] = Shoulder_length*0.7;
	C1[2][3] = C1[2][5] = Shoulder_length*0.9;
	C1[2][9] = C1[2][15] = -Shoulder_length*0.35;
	C1[2][10] = C1[2][14] = -Shoulder_length*0.7;
	C1[2][11] = C1[2][13] = -Shoulder_length*0.9;

	C1[3][4] = chest_length * 14 / 12.5;
	C1[3][12] = -chest_length * 14 / 12.5;
	C1[3][1] = C1[3][7] = C1[3][4] * 0.35;
	C1[3][2] = C1[3][6] = C1[3][4] * 0.7;
	C1[3][3] = C1[3][5] = C1[3][4] * 0.9;
	C1[3][9] = C1[3][15] = -C1[3][4] * 0.35;
	C1[3][10] = C1[3][14] = -C1[3][4] * 0.7;
	C1[3][11] = C1[3][13] = -C1[3][4] * 0.9;

	C1[4][4] = chest_length;
	C1[4][12] = -chest_length;
	C1[4][1] = C1[4][7] = C1[4][4] * 0.35;
	C1[4][2] = C1[4][6] = C1[4][4] * 0.7;
	C1[4][3] = C1[4][5] = C1[4][4] * 0.9;
	C1[4][9] = C1[4][15] = -C1[4][4] * 0.35;
	C1[4][10] = C1[4][14] = -C1[4][4] * 0.7;
	C1[4][11] = C1[4][13] = -C1[4][4] * 0.9;

	C1[5][4] = chest_length*12.1 / 12.5;
	C1[5][12] = -chest_length*12.1 / 12.5;
	C1[5][1] = C1[5][7] = C1[5][4] * 0.35;
	C1[5][2] = C1[5][6] = C1[5][4] * 0.7;
	C1[5][3] = C1[5][5] = C1[5][4] * 0.9;
	C1[5][9] = C1[5][15] = -C1[5][4] * 0.35;
	C1[5][10] = C1[5][14] = -C1[5][4] * 0.7;
	C1[5][11] = C1[5][13] = -C1[5][4] * 0.9;


	C1[6][4] = chest_length*11.3 / 12.5;
	C1[6][12] = -chest_length*11.3 / 12.5;
	C1[6][1] = C1[6][7] = C1[6][4] * 0.35;
	C1[6][2] = C1[6][6] = C1[6][4] * 0.7;
	C1[6][3] = C1[6][5] = C1[6][4] * 0.9;
	C1[6][9] = C1[6][15] = -C1[6][4] * 0.35;
	C1[6][10] = C1[6][14] = -C1[6][4] * 0.7;
	C1[6][11] = C1[6][13] = -C1[6][4] * 0.9;

	for (int j = 0; j<16; j++)
	{
		C1[8][j] = C4[0][j];
	}

	C1[7][4] = C1[8][4] * 12 / 12.8;
	C1[7][12] = C1[8][12] * 12 / 12.8;
	C1[7][1] = C1[7][7] = C1[7][4] * 0.35;
	C1[7][2] = C1[7][6] = C1[7][4] * 0.7;
	C1[7][3] = C1[7][5] = C1[7][4] * 0.9;
	C1[7][9] = C1[7][15] = -C1[7][4] * 0.35;
	C1[7][10] = C1[7][14] = -C1[7][4] * 0.7;
	C1[7][11] = C1[7][13] = -C1[7][4] * 0.9;

	// 身体Z值型值点
	//C3[0][0] = 4.75*neck_length / 5.4;
	//C3[0][8] = -1 * neck_length / 5.4;
	C3[0][0] = neck_length;
	C3[0][8] = -neck_length;
	C3[0][4] = C3[0][12] = 0;
	C3[0][15] = C3[0][1] = C3[0][0] * 0.89;
	C3[0][14] = C3[0][2] = C3[0][0] * 0.52;
	C3[0][13] = C3[0][3] = C3[0][0] * 0.1;
	C3[0][11] = C3[0][5] = C3[0][8] * 0.78;
	C3[0][10] = C3[0][6] = C3[0][8] * 0.89;
	C3[0][9] = C3[0][7] = C3[0][8] * 1;
	for (int i = 0; i<16; i++)
	{
		C3[1][i] = C3[0][i];
	}

	C3[2][0] = 5.5*Shoulder_length / 15.3;
	//C3[2][8] = -5.5*neck_length / 15.3;
	C3[2][8] = -neck_length;
	C3[2][4] = C3[2][12] = 0;
	C3[2][15] = C3[2][1] = C3[2][0] * 0.58;
	C3[2][14] = C3[2][2] = C3[2][0] * 0.45;
	C3[2][13] = C3[2][3] = C3[2][0] * 0.2;
	C3[2][11] = C3[2][5] = C3[2][8] * 0.42;
	C3[2][10] = C3[2][6] = C3[2][8] * 0.54;
	C3[2][9] = C3[2][7] = C3[2][8] * 1;

	//C3[3][0] = 7.6*chest_length / 12.5;
	//C3[3][8] = -7.6*chest_length / 12.5;
	C3[3][0] = (side_chest_width / (1.2 + 1.2)) * 7.6 / 8.75;
	C3[3][8] = -(side_chest_width / (1.2 + 1.2)) * 7.6 / 8.75;
	C3[3][4] = C3[3][12] = 0;
	C3[3][15] = C3[3][1] = C3[3][0] * 1;
	C3[3][14] = C3[3][2] = C3[3][0] * 0.71;
	C3[3][13] = C3[3][3] = C3[3][0] * 0.61;
	C3[3][11] = C3[3][5] = C3[3][8] * 0.88;
	C3[3][10] = C3[3][6] = C3[3][8] * 0.95;
	C3[3][9] = C3[3][7] = C3[3][8] * 1.04;

	//C3[4][0] = 8.75*chest_length / 12.5;
	//C3[4][8] = -8.75*chest_length / 12.5;
	C3[4][0] = side_chest_width / (1.2 + 1.2);
	C3[4][8] = -side_chest_width / (1.2 + 1.2) * 0.9;
	C3[4][4] = C3[4][12] = 0;
	C3[4][15] = C3[4][1] = C3[4][0] * 1.2;
	C3[4][14] = C3[4][2] = C3[4][0] * 0.88;
	C3[4][13] = C3[4][3] = C3[4][0] * 0.79;
	C3[4][11] = C3[4][5] = C3[4][8] * 0.99;
	C3[4][10] = C3[4][6] = C3[4][8] * 1.1;
	C3[4][9] = C3[4][7] = C3[4][8] * 1.2;

	//C3[5][0] = 7.6*chest_length / 12.5;
	//C3[5][8] = -7.6*chest_length / 12.5;
	C3[5][0] = (side_chest_width / (1.2 + 1.2)) * 7.6 / 8.75;
	C3[5][8] = -(side_chest_width / (1.2 + 1.2)) * 7.6 / 8.75;
	C3[5][4] = C3[5][12] = 0;
	C3[5][15] = C3[5][1] = C3[5][0] * 1.2;
	C3[5][14] = C3[5][2] = C3[5][0] * 0.88;
	C3[5][13] = C3[5][3] = C3[5][0] * 0.79;
	C3[5][11] = C3[5][5] = C3[5][8] * 0.99;
	C3[5][10] = C3[5][6] = C3[5][8] * 1.1;
	C3[5][9] = C3[5][7] = C3[5][8] * 1.2;

	//C3[6][0] = 8.3*chest_length / 12.5;
	//C3[6][8] = -8.3*chest_length / 12.5;
	C3[6][0] = (side_chest_width / (1.2 + 1.2)) * 8.3 / 8.75;
	C3[6][8] = -(side_chest_width / (1.2 + 1.2)) * 8.3 / 8.75;
	C3[6][4] = C3[6][12] = 0;
	C3[6][15] = C3[6][1] = C3[6][0] * 0.94;
	C3[6][14] = C3[6][2] = C3[6][0] * 0.67;
	C3[6][13] = C3[6][3] = C3[6][0] * 0.34;
	C3[6][11] = C3[6][5] = C3[6][8] * 0.46;
	C3[6][10] = C3[6][6] = C3[6][8] * 0.8;
	C3[6][9] = C3[6][7] = C3[6][8] * 1;


	for (int j = 0; j<16; j++)
	{
		C3[8][j] = C6[0][j];
	}

	//C3[7][0] = 9.05 * C6[0][0] / 7.2;
	//C3[7][8] = -8.05 * C6[0][0] / 7.2;
	//C3[7][0] = 7 * C6[0][0] / 7.2;
	//C3[7][8] = - 7 * C6[0][0] / 7.2;

	C3[7][0] = C3[8][0];
	C3[7][8] = C3[8][8];
	C3[7][4] = C3[7][12] = 0;
	C3[7][15] = C3[7][1] = C3[7][0] * 0.89;
	C3[7][14] = C3[7][2] = C3[7][0] * 0.6;
	C3[7][13] = C3[7][3] = C3[7][0] * 0.4;
	C3[7][11] = C3[7][5] = C3[7][8] * 0.58;
	C3[7][10] = C3[7][6] = C3[7][8] * 0.87;
	C3[7][9] = C3[7][7] = C3[7][8] * 1;
	for (int j = 0; j<16; j++)
	{
		C3[8][j] = C6[0][j];
	}

}


void Human::CalculationXZD_female()
{
	//臀部Y值型值点计算
	for (int i = 0; i<16; i++)
	{
		C5[0][i] = waist_point[1];
		C5[1][i] = waist_point[1] * 4 / 5;
		C5[2][i] = waist_point[1] * 3 / 5;
		C5[3][i] = waist_point[1] * 2 / 5;
		C5[4][i] = waist_point[1] * 1 / 5;
		C5[5][i] = waist_point[1] * 0;
	}

	//臀部X值型值点计算
	C4[0][4] = waist_length;
	C4[5][4] = hip_length;
	C4[1][4] = (hip_length - waist_length)*0.7 / 3.7 + waist_length;
	C4[2][4] = (hip_length - waist_length)*1.9 / 3.7 + waist_length;
	C4[3][4] = (hip_length - waist_length)*3.1 / 3.7 + waist_length;
	C4[4][4] = (hip_length - waist_length)*3.5 / 3.7 + waist_length;
	C4[0][0] = C4[1][0] = C4[2][0] = C4[3][0] = C4[4][0] = C4[5][0] = C4[0][8] = C4[1][8] = C4[2][8] = C4[3][8] = C4[4][8] = C4[5][8] = 0;
	for (int j = 0; j<6; j++)
	{
		C4[j][1] = C4[j][4] * 0.35;
		C4[j][2] = C4[j][4] * 0.7;
		C4[j][3] = C4[j][4] * 0.9;
		C4[j][5] = C4[j][4] * 0.9;
		C4[j][6] = C4[j][4] * 0.7;
		C4[j][7] = C4[j][4] * 0.35;
		for (int i = 9; i<16; i++)
		{
			C4[j][i] = -C4[j][16 - i];
		}
	}

	//臀部Z值型值点计算
	C6[0][4] = C6[1][4] = C6[2][4] = C6[3][4] = C6[4][4] = C6[5][4] = 0;
	//C6[5][0] = 3.8*C4[5][4] / 16.1;//C6[5]为大腿一开始
	//C6[5][8] = -9.4*C4[5][4] / 16.1;
	C6[5][0] = (3.8 / (3.8*2.25 + 9.4*1.13)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 12.052 / 17.437);
	C6[5][8] = -(9.4 / (3.8*2.25 + 9.4*1.13)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 12.052 / 17.437);
	C6[5][1] = 1.11*C6[5][0];
	C6[5][2] = 2.25*C6[5][0];
	C6[5][3] = 1.46*C6[5][0];
	C6[5][5] = 0.74*C6[5][8];
	C6[5][6] = 1.03*C6[5][8];
	C6[5][7] = 1.13*C6[5][8];

	//C6[4][0] = 7.1*C4[4][4] / 15.9;
	//C6[4][8] = -9.6*C4[4][4] / 15.9;
	C6[4][0] = (7.1 / (5.4*1.1 + 10.6*1.23)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 15.331 / 17.437);
	C6[4][8] = -(9.6 / (5.4*1.1 + 10.6*1.23)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 15.331 / 17.437);
	C6[4][1] = 0.78*C6[4][0];
	C6[4][2] = 1.1*C6[4][0];
	C6[4][3] = 0.68*C6[4][0];
	C6[4][5] = 0.67*C6[4][8];
	C6[4][6] = 1.04*C6[4][8];
	C6[4][7] = 1.23*C6[4][8];

	//C6[3][0] = 8.5*C4[3][4] / 15.5;
	//C6[3][8] = -10 * C4[3][4] / 15.5;	
	C6[3][0] = (8.5 / (8.5 + 10 * 1.23)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 17.178 / 17.437);
	C6[3][8] = -(10 / (8.5 + 10 * 1.23)) * ((side_boody_width / (8.1 + 10 * 1.2)) * (8.1 + 10) * 17.178 / 17.437);
	C6[3][1] = 0.78*C6[3][0];
	C6[3][2] = 0.9*C6[3][0];
	C6[3][3] = 0.60*C6[3][0];
	C6[3][5] = 0.61*C6[3][8];
	C6[3][6] = 0.9*C6[3][8];
	C6[3][7] = 1.23*C6[3][8];

	//C6[2][0] = 8.1*C4[2][4] / 14.3;
	//C6[2][8] = -10 * C4[2][4] / 14.3;
	//C6[2][0] = side_boody_width / (8.1 + 10 * 1.2) * 8.1;
	//C6[2][8] = -side_boody_width / (8.1 + 10 * 1.2) * 10;
	C6[2][0] = side_boody_width / (8.1 + 10 * 1.2) * 7.5;
	C6[2][8] = -side_boody_width / (8.1 + 10 * 1.2) * 9;
	C6[2][1] = 0.83*C6[2][0];
	C6[2][2] = 0.69*C6[2][0];
	C6[2][3] = 0.58*C6[2][0];
	C6[2][5] = 0.62*C6[2][8];
	C6[2][6] = 1 * C6[2][8];
	C6[2][7] = 1.2*C6[2][8];

	//C6[0][0] = 7.2*C4[0][4] / 12.4;
	//C6[0][8] = -6.9*C4[0][4] / 12.4;
	C6[0][0] = side_waist_width / (7.2 * 1.06 + 6.9) * 7.2;
	C6[0][8] = -side_waist_width / (7.2 * 1.06 + 6.9) * 6.9;
	C6[0][1] = 1.06*C6[0][0];
	C6[0][2] = 0.75*C6[0][0];
	C6[0][3] = 0.57*C6[0][0];
	C6[0][5] = 0.41*C6[0][8];
	C6[0][6] = 0.81*C6[0][8];
	C6[0][7] = 1 * C6[0][8];

	//C6[1][0] = 8 * C4[1][4] / 13.1;
	//C6[1][8] = -8.1*C4[1][4] / 13.1;
	//C6[1][0] = (8 / (8 + 8.1)) * ((C6[2][0] - C6[2][8] + C6[0][0] - C6[0][8]) / 2);
	//C6[1][8] = -(8.1 / (8 + 8.1)) * ((C6[2][0] - C6[2][8] + C6[0][0] - C6[0][8]) / 2);
	C6[1][0] = (C6[2][0] + C6[0][0]) / 2;
	C6[1][8] = (C6[2][8] + C6[0][8]) / 2;
	C6[1][1] = 0.94*C6[1][0];
	C6[1][2] = 0.69*C6[1][0];
	C6[1][3] = 0.54*C6[1][0];
	C6[1][5] = 0.56*C6[1][8];
	C6[1][6] = 1.04*C6[1][8];
	C6[1][7] = 1.19*C6[1][8];


	for (int i = 0; i<6; i++)
	{
		for (int j = 9; j<16; j++)
		{
			C6[i][j] = C6[i][16 - j];
		}
	}

	//右腿大腿Y值型值点
	for (int i = 0; i<8; i++)
	{
		C8[0][i] = 0;
		C8[5][i] = calf_point[1];
		C8[1][i] = calf_point[1] * 1 / 5;
		C8[2][i] = calf_point[1] * 2 / 5;
		C8[3][i] = calf_point[1] * 3 / 5;
		C8[4][i] = calf_point[1] * 4 / 5;
	}
	//右腿大腿X型值点
	for (int i = 0; i<3; i++)
	{
		calf_temp_point[5][i] = calf_point[i];
	}
	calf_temp_point[0][0] = hip_length - Thigh_length / 2;
	calf_temp_point[0][1] = 0.0f;
	calf_temp_point[0][2] = 0.0f;
	for (int i = 1; i<5; i++)
	{
		calf_temp_point[i][0] = (calf_temp_point[5][0] - calf_temp_point[0][0])*i / 5 + calf_temp_point[0][0];
		calf_temp_point[i][1] = (calf_temp_point[5][1] - calf_temp_point[0][1])*i / 5 + calf_temp_point[0][1];
		calf_temp_point[i][2] = (calf_temp_point[5][2] - calf_temp_point[0][2])*i / 5 + calf_temp_point[0][2];

	}

	/*
	asf[0] = Thigh_length / 14.5;
	asf[1] = (asf[0] - 1) * 4 / 5 + 1;
	asf[2] = (asf[0] - 1) * 3 / 5 + 1;
	asf[3] = (asf[0] - 1) * 2 / 5 + 1;
	asf[4] = (asf[0] - 1) * 1 / 5 + 1;
	asf[5] = 1;*/

	asf[0] = Thigh_length / 14.5;
	asf[1] = (asf[0] - Thigh_length / 14.5) * 4 / 5 + Thigh_length / 14.5;
	asf[2] = (asf[0] - Thigh_length / 14.5) * 3 / 5 + Thigh_length / 14.5;
	asf[3] = (asf[0] - Thigh_length / 14.5) * 2 / 5 + Thigh_length / 14.5;
	asf[4] = (asf[0] - Thigh_length / 14.5) * 1 / 5 + Thigh_length / 14.5;
	asf[5] = Thigh_length / 14.5;


	for (int i = 0; i<6; i++)
	{
		C7[0][i] = C4[5][i + 2];
	}
	C7[0][6] = hip_length - Thigh_length;
	C7[0][7] = C4[5][1];

	C7[1][0] = C7[1][4] = calf_temp_point[1][0];
	C7[2][0] = C7[2][4] = calf_temp_point[2][0];
	C7[3][0] = C7[3][4] = calf_temp_point[3][0];
	C7[4][0] = C7[4][4] = calf_temp_point[4][0];
	C7[5][0] = C7[5][4] = calf_temp_point[5][0];

	C7[1][2] = calf_temp_point[1][0] + 7.5*asf[1];
	C7[1][6] = calf_temp_point[1][0] - 7.5*asf[1];
	C7[2][2] = calf_temp_point[2][0] + 7.1*asf[2];
	C7[2][6] = calf_temp_point[2][0] - 7.1*asf[2];
	C7[3][2] = calf_temp_point[3][0] + 6.1*asf[3];
	C7[3][6] = calf_temp_point[3][0] - 6.1*asf[3];
	C7[4][2] = calf_temp_point[4][0] + 5.4*asf[4];
	C7[4][6] = calf_temp_point[4][0] - 5.4*asf[4];
	C7[5][2] = calf_temp_point[5][0] + 4.3*asf[5];
	C7[5][6] = calf_temp_point[5][0] - 4.3*asf[5];

	for (int i = 1; i<6; i++)
	{
		C7[i][1] = (C7[i][2] - C7[i][0])*0.7 + C7[i][0];
		C7[i][3] = (C7[i][2] - C7[i][0])*0.7 + C7[i][0];
		C7[i][5] = (C7[i][6] - C7[i][0])*0.7 + C7[i][0];
		C7[i][7] = (C7[i][6] - C7[i][0])*0.7 + C7[i][0];
	}
	//右大腿Z值型值点
	for (int i = 0; i<6; i++)
	{
		C9[0][i] = C6[5][i + 2];
	}
	C9[0][6] = 0.0f;
	C9[0][7] = C6[5][1];

	C9[1][2] = C9[1][6] = calf_temp_point[1][2];
	C9[2][2] = C9[2][6] = calf_temp_point[2][2];
	C9[3][2] = C9[3][6] = calf_temp_point[3][2];
	C9[4][2] = C9[4][6] = calf_temp_point[4][2];
	C9[5][2] = C9[5][6] = calf_temp_point[5][2];

	C9[1][0] = calf_temp_point[1][2] + 8.0*asf[1];
	C9[1][4] = calf_temp_point[1][2] - 8.2*asf[1];
	C9[2][0] = calf_temp_point[2][2] + 7.6*asf[2];
	C9[2][4] = calf_temp_point[2][2] - 7.5*asf[2];
	C9[3][0] = calf_temp_point[3][2] + 7.2*asf[3];
	C9[3][4] = calf_temp_point[3][2] - 6.0*asf[3];
	C9[4][0] = calf_temp_point[4][2] + 6.5*asf[4];
	C9[4][4] = calf_temp_point[4][2] - 5.1*asf[4];
	C9[5][0] = calf_temp_point[5][2] + 6.4*asf[5];
	C9[5][4] = calf_temp_point[5][2] - 4.4*asf[5];

	C9[1][1] = (C9[1][0] - C9[1][2])*0.78 + C9[1][2];
	C9[1][3] = (C9[1][4] - C9[1][2])*0.72 + C9[1][2];
	C9[1][5] = (C9[1][4] - C9[1][2])*0.75 + C9[1][2];
	C9[1][7] = (C9[1][0] - C9[1][2])*0.56 + C9[1][2];

	C9[2][1] = (C9[2][0] - C9[2][2])*0.74 + C9[2][2];
	C9[2][3] = (C9[2][4] - C9[2][2])*0.71 + C9[2][2];
	C9[2][5] = (C9[2][4] - C9[2][2])*0.71 + C9[2][2];
	C9[2][7] = (C9[2][0] - C9[2][2])*0.68 + C9[2][2];

	C9[3][1] = (C9[3][0] - C9[3][2])*0.74 + C9[3][2];
	C9[3][3] = (C9[3][4] - C9[3][2])*0.71 + C9[3][2];
	C9[3][5] = (C9[3][4] - C9[3][2])*0.71 + C9[3][2];
	C9[3][7] = (C9[3][0] - C9[3][2])*0.70 + C9[3][2];

	C9[4][1] = (C9[4][0] - C9[4][2])*0.65 + C9[4][2];
	C9[4][3] = (C9[4][4] - C9[4][2])*0.65 + C9[4][2];
	C9[4][5] = (C9[4][4] - C9[4][2])*0.65 + C9[4][2];
	C9[4][7] = (C9[4][0] - C9[4][2])*0.6 + C9[4][2];

	C9[5][1] = (C9[5][0] - C9[5][2])*0.5 + C9[5][2];
	C9[5][3] = (C9[5][4] - C9[5][2])*0.5 + C9[5][2];
	C9[5][5] = (C9[5][4] - C9[5][2])*0.5 + C9[5][2];
	C9[5][7] = (C9[5][0] - C9[5][2])*0.5 + C9[5][2];

	//右小腿 Y?
	for (int i = 0; i<8; i++)
	{
		C14[0][i] = C8[5][i];
		C14[5][i] = calf_point_2[1];
		C14[1][i] = (calf_point_2[1] - calf_point[1]) * 1 / 5 + calf_point[1];
		C14[2][i] = (calf_point_2[1] - calf_point[1]) * 2 / 5 + calf_point[1];
		C14[3][i] = (calf_point_2[1] - calf_point[1]) * 3 / 5 + calf_point[1];
		C14[4][i] = (calf_point_2[1] - calf_point[1]) * 4 / 5 + calf_point[1];
	}
	for (int i = 0; i<3; i++)
	{
		calf_temp_point_2[5][i] = calf_point_2[i];
		calf_temp_point_2[0][i] = calf_point[i];
	}
	for (int i = 1; i<5; i++)
	{
		calf_temp_point_2[i][0] = (calf_temp_point_2[5][0] - calf_temp_point_2[0][0])*i / 5 + calf_temp_point_2[0][0];
		calf_temp_point_2[i][1] = (calf_temp_point_2[5][1] - calf_temp_point_2[0][1])*i / 5 + calf_temp_point_2[0][1];
		calf_temp_point_2[i][2] = (calf_temp_point_2[5][2] - calf_temp_point_2[0][2])*i / 5 + calf_temp_point_2[0][2];

	}
	/*
	asf[0] = 1;
	asf[1] = 1;
	asf[2] = 1;
	asf[3] = 1;
	asf[4] = 1;
	asf[5] = 1;*/

	asf[0] = Thigh_length / 14.5;
	asf[1] = Thigh_length / 14.5;
	asf[2] = Thigh_length / 14.5;
	asf[3] = Thigh_length / 14.5;
	asf[4] = Thigh_length / 14.5;
	asf[5] = Thigh_length / 14.5;

	//右小腿 X?

	for (int i = 0; i<8; i++)
	{

		C13[0][i] = C7[5][i];
	}
	C13[1][0] = C13[1][4] = calf_temp_point_2[1][0];
	C13[2][0] = C13[2][4] = calf_temp_point_2[2][0];
	C13[3][0] = C13[3][4] = calf_temp_point_2[3][0];
	C13[4][0] = C13[4][4] = calf_temp_point_2[4][0];
	C13[5][0] = C13[5][4] = calf_temp_point_2[5][0];
	C13[1][2] = calf_temp_point_2[1][0] + 4.9*asf[1];
	C13[1][6] = calf_temp_point_2[1][0] - 4.9*asf[1];
	C13[2][2] = calf_temp_point_2[2][0] + 5.0*asf[2];
	C13[2][6] = calf_temp_point_2[2][0] - 5.0*asf[2];
	C13[3][2] = calf_temp_point_2[3][0] + 4.1*asf[3];
	C13[3][6] = calf_temp_point_2[3][0] - 4.1*asf[3];
	C13[4][2] = calf_temp_point_2[4][0] + 3.5*asf[4];
	C13[4][6] = calf_temp_point_2[4][0] - 3.5*asf[4];
	C13[5][2] = calf_temp_point_2[5][0] + 2.5*asf[5];
	C13[5][6] = calf_temp_point_2[5][0] - 2.5*asf[5];
	for (int i = 1; i<6; i++)
	{
		C13[i][1] = (C13[i][2] - C13[i][0])*0.7 + C13[i][0];
		C13[i][3] = (C13[i][2] - C13[i][0])*0.7 + C13[i][0];
		C13[i][5] = (C13[i][6] - C13[i][0])*0.7 + C13[i][0];
		C13[i][7] = (C13[i][6] - C13[i][0])*0.7 + C13[i][0];
	}

	//右小腿 Z?

	for (int i = 0; i<8; i++)
	{
		C15[0][i] = C9[5][i];

	}

	C15[1][2] = C15[1][6] = calf_temp_point_2[1][2];
	C15[2][2] = C15[2][6] = calf_temp_point_2[2][2];
	C15[3][2] = C15[3][6] = calf_temp_point_2[3][2];
	C15[4][2] = C15[4][6] = calf_temp_point_2[4][2];
	C15[5][2] = C15[5][6] = calf_temp_point_2[5][2];
	C15[1][0] = calf_temp_point_2[1][2] + 5.0*asf[1];
	C15[1][4] = calf_temp_point_2[1][2] - 6 * asf[1];
	C15[2][0] = calf_temp_point_2[2][2] + 5 * asf[2];
	C15[2][4] = calf_temp_point_2[2][2] - 6 * asf[2];
	C15[3][0] = calf_temp_point_2[3][2] + 4.1*asf[3];
	C15[3][4] = calf_temp_point_2[3][2] - 4.5*asf[3];
	C15[4][0] = calf_temp_point_2[4][2] + 3 * asf[4];
	C15[4][4] = calf_temp_point_2[4][2] - 3 * asf[4];
	C15[5][0] = calf_temp_point_2[5][2] + 2.5*asf[5];
	C15[5][4] = calf_temp_point_2[5][2] - 2.5*asf[5];

	C15[1][1] = (C15[1][0] - C15[1][2])*0.7 + C15[1][2];
	C15[1][3] = (C15[1][4] - C15[1][2])*0.7 + C15[1][2];
	C15[1][5] = (C15[1][4] - C15[1][2])*0.7 + C15[1][2];
	C15[1][7] = (C15[1][0] - C15[1][2])*0.7 + C15[1][2];

	C15[2][1] = (C15[2][0] - C15[2][2])*0.7 + C15[2][2];
	C15[2][3] = (C15[2][4] - C15[2][2])*0.71 + C15[2][2];
	C15[2][5] = (C15[2][4] - C15[2][2])*0.71 + C15[2][2];
	C15[2][7] = (C15[2][0] - C15[2][2])*0.68 + C15[2][2];

	C15[3][1] = (C15[3][0] - C15[3][2])*0.7 + C15[3][2];
	C15[3][3] = (C15[3][4] - C15[3][2])*0.71 + C15[3][2];
	C15[3][5] = (C15[3][4] - C15[3][2])*0.71 + C15[3][2];
	C15[3][7] = (C15[3][0] - C15[3][2])*0.70 + C15[3][2];

	C15[4][1] = (C15[4][0] - C15[4][2])*0.7 + C15[4][2];
	C15[4][3] = (C15[4][4] - C15[4][2])*0.7 + C15[4][2];
	C15[4][5] = (C15[4][4] - C15[4][2])*0.7 + C15[4][2];
	C15[4][7] = (C15[4][0] - C15[4][2])*0.7 + C15[4][2];

	C15[5][1] = (C15[5][0] - C15[5][2])*0.7 + C15[5][2];
	C15[5][3] = (C15[5][4] - C15[5][2])*0.7 + C15[5][2];
	C15[5][5] = (C15[5][4] - C15[5][2])*0.7 + C15[5][2];
	C15[5][7] = (C15[5][0] - C15[5][2])*0.7 + C15[5][2];

	//L_calf_point[0]=-L_calf_point[0];
	for (int i = 0; i<8; i++)
	{
		C11_2[0][i] = 0;
		C11_2[5][i] = L_calf_point[1];
		C11_2[1][i] = L_calf_point[1] * 1 / 5;
		C11_2[2][i] = L_calf_point[1] * 2 / 5;
		C11_2[3][i] = L_calf_point[1] * 3 / 5;
		C11_2[4][i] = L_calf_point[1] * 4 / 5;
	}
	//右腿大腿X型值点
	for (int i = 0; i<3; i++)
	{
		calf_temp_point[5][i] = L_calf_point[i];
	}
	calf_temp_point[0][0] = hip_length - Thigh_length / 2;
	calf_temp_point[0][1] = 0.0f;
	calf_temp_point[0][2] = 0.0f;
	for (int i = 1; i<5; i++)
	{
		calf_temp_point[i][0] = (calf_temp_point[5][0] - calf_temp_point[0][0])*i / 5 + calf_temp_point[0][0];
		calf_temp_point[i][1] = (calf_temp_point[5][1] - calf_temp_point[0][1])*i / 5 + calf_temp_point[0][1];
		calf_temp_point[i][2] = (calf_temp_point[5][2] - calf_temp_point[0][2])*i / 5 + calf_temp_point[0][2];

	}
	/*asf[0] = Thigh_length / 14.5;
	asf[1] = (asf[0] - 1) * 4 / 5 + 1;
	asf[2] = (asf[0] - 1) * 3 / 5 + 1;
	asf[3] = (asf[0] - 1) * 2 / 5 + 1;
	asf[4] = (asf[0] - 1) * 1 / 5 + 1;
	asf[5] = 1;*/

	asf[0] = Thigh_length / 14.5;
	asf[1] = (asf[0] - Thigh_length / 14.5) * 4 / 5 + Thigh_length / 14.5;
	asf[2] = (asf[0] - Thigh_length / 14.5) * 3 / 5 + Thigh_length / 14.5;
	asf[3] = (asf[0] - Thigh_length / 14.5) * 2 / 5 + Thigh_length / 14.5;
	asf[4] = (asf[0] - Thigh_length / 14.5) * 1 / 5 + Thigh_length / 14.5;
	asf[5] = Thigh_length / 14.5;

	for (int i = 0; i<6; i++)
	{
		C10_2[0][i] = C4[5][i + 2];
	}
	C10_2[0][6] = hip_length - Thigh_length;
	C10_2[0][7] = C4[5][1];

	C10_2[1][0] = C10_2[1][4] = calf_temp_point[1][0];
	C10_2[2][0] = C10_2[2][4] = calf_temp_point[2][0];
	C10_2[3][0] = C10_2[3][4] = calf_temp_point[3][0];
	C10_2[4][0] = C10_2[4][4] = calf_temp_point[4][0];
	C10_2[5][0] = C10_2[5][4] = calf_temp_point[5][0];

	C10_2[1][2] = calf_temp_point[1][0] + 7.5*asf[1];
	C10_2[1][6] = calf_temp_point[1][0] - 7.5*asf[1];
	C10_2[2][2] = calf_temp_point[2][0] + 7.1*asf[2];
	C10_2[2][6] = calf_temp_point[2][0] - 7.1*asf[2];
	C10_2[3][2] = calf_temp_point[3][0] + 6.1*asf[3];
	C10_2[3][6] = calf_temp_point[3][0] - 6.1*asf[3];
	C10_2[4][2] = calf_temp_point[4][0] + 5.4*asf[4];
	C10_2[4][6] = calf_temp_point[4][0] - 5.4*asf[4];
	C10_2[5][2] = calf_temp_point[5][0] + 4.3*asf[5];
	C10_2[5][6] = calf_temp_point[5][0] - 4.3*asf[5];

	for (int i = 1; i<6; i++)
	{
		C10_2[i][1] = (C10_2[i][2] - C10_2[i][0])*0.7 + C10_2[i][0];
		C10_2[i][3] = (C10_2[i][2] - C10_2[i][0])*0.7 + C10_2[i][0];
		C10_2[i][5] = (C10_2[i][6] - C10_2[i][0])*0.7 + C10_2[i][0];
		C10_2[i][7] = (C10_2[i][6] - C10_2[i][0])*0.7 + C10_2[i][0];
	}
	//右大腿Z值型值点
	for (int i = 0; i<6; i++)
	{
		C12_2[0][i] = C6[5][i + 2];
	}
	C12_2[0][6] = 0.0f;
	C12_2[0][7] = C6[5][1];

	C12_2[1][2] = C12_2[1][6] = calf_temp_point[1][2];
	C12_2[2][2] = C12_2[2][6] = calf_temp_point[2][2];
	C12_2[3][2] = C12_2[3][6] = calf_temp_point[3][2];
	C12_2[4][2] = C12_2[4][6] = calf_temp_point[4][2];
	C12_2[5][2] = C12_2[5][6] = calf_temp_point[5][2];

	C12_2[1][0] = calf_temp_point[1][2] + 8.0*asf[1];
	C12_2[1][4] = calf_temp_point[1][2] - 8.2*asf[1];
	C12_2[2][0] = calf_temp_point[2][2] + 7.6*asf[2];
	C12_2[2][4] = calf_temp_point[2][2] - 7.5*asf[2];
	C12_2[3][0] = calf_temp_point[3][2] + 7.2*asf[3];
	C12_2[3][4] = calf_temp_point[3][2] - 6.0*asf[3];
	C12_2[4][0] = calf_temp_point[4][2] + 6.5*asf[4];
	C12_2[4][4] = calf_temp_point[4][2] - 5.1*asf[4];
	C12_2[5][0] = calf_temp_point[5][2] + 6.4*asf[5];
	C12_2[5][4] = calf_temp_point[5][2] - 4.4*asf[5];

	C12_2[1][1] = (C12_2[1][0] - C12_2[1][2])*0.78 + C12_2[1][2];
	C12_2[1][3] = (C12_2[1][4] - C12_2[1][2])*0.72 + C12_2[1][2];
	C12_2[1][5] = (C12_2[1][4] - C12_2[1][2])*0.75 + C12_2[1][2];
	C12_2[1][7] = (C12_2[1][0] - C12_2[1][2])*0.56 + C12_2[1][2];

	C12_2[2][1] = (C12_2[2][0] - C12_2[2][2])*0.74 + C12_2[2][2];
	C12_2[2][3] = (C12_2[2][4] - C12_2[2][2])*0.71 + C12_2[2][2];
	C12_2[2][5] = (C12_2[2][4] - C12_2[2][2])*0.71 + C12_2[2][2];
	C12_2[2][7] = (C12_2[2][0] - C12_2[2][2])*0.68 + C12_2[2][2];

	C12_2[3][1] = (C12_2[3][0] - C12_2[3][2])*0.74 + C12_2[3][2];
	C12_2[3][3] = (C12_2[3][4] - C12_2[3][2])*0.71 + C12_2[3][2];
	C12_2[3][5] = (C12_2[3][4] - C12_2[3][2])*0.71 + C12_2[3][2];
	C12_2[3][7] = (C12_2[3][0] - C12_2[3][2])*0.70 + C12_2[3][2];

	C12_2[4][1] = (C12_2[4][0] - C12_2[4][2])*0.65 + C12_2[4][2];
	C12_2[4][3] = (C12_2[4][4] - C12_2[4][2])*0.65 + C12_2[4][2];
	C12_2[4][5] = (C12_2[4][4] - C12_2[4][2])*0.65 + C12_2[4][2];
	C12_2[4][7] = (C12_2[4][0] - C12_2[4][2])*0.6 + C12_2[4][2];

	C12_2[5][1] = (C12_2[5][0] - C12_2[5][2])*0.5 + C12_2[5][2];
	C12_2[5][3] = (C12_2[5][4] - C12_2[5][2])*0.5 + C12_2[5][2];
	C12_2[5][5] = (C12_2[5][4] - C12_2[5][2])*0.5 + C12_2[5][2];
	C12_2[5][7] = (C12_2[5][0] - C12_2[5][2])*0.5 + C12_2[5][2];

	for (int i = 0; i<6; i++)
	{
		for (int j = 0; j<8; j++)
		{
			C10[i][j] = -C10_2[i][(8 - j) % 8];
			C11[i][j] = C11_2[i][(8 - j) % 8];
			C12[i][j] = C12_2[i][(8 - j) % 8];

		}
	}

	//L_calf_point_2[0]=-L_calf_point_2[0];


	for (int i = 0; i<8; i++)
	{
		C17_2[0][i] = C11_2[5][i];
		C17_2[5][i] = L_calf_point_2[1];
		C17_2[1][i] = (L_calf_point_2[1] - L_calf_point[1]) * 1 / 5 + L_calf_point[1];
		C17_2[2][i] = (L_calf_point_2[1] - L_calf_point[1]) * 2 / 5 + L_calf_point[1];
		C17_2[3][i] = (L_calf_point_2[1] - L_calf_point[1]) * 3 / 5 + L_calf_point[1];
		C17_2[4][i] = (L_calf_point_2[1] - L_calf_point[1]) * 4 / 5 + L_calf_point[1];
	}
	for (int i = 0; i<3; i++)
	{
		calf_temp_point_2[5][i] = L_calf_point_2[i];
		calf_temp_point_2[0][i] = L_calf_point[i];
	}
	for (int i = 1; i<5; i++)
	{
		calf_temp_point_2[i][0] = (calf_temp_point_2[5][0] - calf_temp_point_2[0][0])*i / 5 + calf_temp_point_2[0][0];
		calf_temp_point_2[i][1] = (calf_temp_point_2[5][1] - calf_temp_point_2[0][1])*i / 5 + calf_temp_point_2[0][1];
		calf_temp_point_2[i][2] = (calf_temp_point_2[5][2] - calf_temp_point_2[0][2])*i / 5 + calf_temp_point_2[0][2];

	}

	asf[0] = Thigh_length / 14.5;
	asf[1] = Thigh_length / 14.5;
	asf[2] = Thigh_length / 14.5;
	asf[3] = Thigh_length / 14.5;
	asf[4] = Thigh_length / 14.5;
	asf[5] = Thigh_length / 14.5;

	for (int i = 0; i<8; i++)
	{

		C16_2[0][i] = C10_2[5][i];
	}
	C16_2[1][0] = C16_2[1][4] = calf_temp_point_2[1][0];
	C16_2[2][0] = C16_2[2][4] = calf_temp_point_2[2][0];
	C16_2[3][0] = C16_2[3][4] = calf_temp_point_2[3][0];
	C16_2[4][0] = C16_2[4][4] = calf_temp_point_2[4][0];
	C16_2[5][0] = C16_2[5][4] = calf_temp_point_2[5][0];
	C16_2[1][2] = calf_temp_point_2[1][0] + 4.9*asf[1];
	C16_2[1][6] = calf_temp_point_2[1][0] - 4.9*asf[1];
	C16_2[2][2] = calf_temp_point_2[2][0] + 5.0*asf[2];
	C16_2[2][6] = calf_temp_point_2[2][0] - 5.0*asf[2];
	C16_2[3][2] = calf_temp_point_2[3][0] + 4.1*asf[3];
	C16_2[3][6] = calf_temp_point_2[3][0] - 4.1*asf[3];
	C16_2[4][2] = calf_temp_point_2[4][0] + 3.5*asf[4];
	C16_2[4][6] = calf_temp_point_2[4][0] - 3.5*asf[4];
	C16_2[5][2] = calf_temp_point_2[5][0] + 2.5*asf[5];
	C16_2[5][6] = calf_temp_point_2[5][0] - 2.5*asf[5];
	for (int i = 1; i<6; i++)
	{
		C16_2[i][1] = (C16_2[i][2] - C16_2[i][0])*0.7 + C16_2[i][0];
		C16_2[i][3] = (C16_2[i][2] - C16_2[i][0])*0.7 + C16_2[i][0];
		C16_2[i][5] = (C16_2[i][6] - C16_2[i][0])*0.7 + C16_2[i][0];
		C16_2[i][7] = (C16_2[i][6] - C16_2[i][0])*0.7 + C16_2[i][0];
	}

	for (int i = 0; i<8; i++)
	{
		C18_2[0][i] = C12_2[5][i];

	}

	C18_2[1][2] = C18_2[1][6] = calf_temp_point_2[1][2];
	C18_2[2][2] = C18_2[2][6] = calf_temp_point_2[2][2];
	C18_2[3][2] = C18_2[3][6] = calf_temp_point_2[3][2];
	C18_2[4][2] = C18_2[4][6] = calf_temp_point_2[4][2];
	C18_2[5][2] = C18_2[5][6] = calf_temp_point_2[5][2];
	C18_2[1][0] = calf_temp_point_2[1][2] + 5.0*asf[1];
	C18_2[1][4] = calf_temp_point_2[1][2] - 6 * asf[1];
	C18_2[2][0] = calf_temp_point_2[2][2] + 5 * asf[2];
	C18_2[2][4] = calf_temp_point_2[2][2] - 6 * asf[2];
	C18_2[3][0] = calf_temp_point_2[3][2] + 4.1*asf[3];
	C18_2[3][4] = calf_temp_point_2[3][2] - 4.5*asf[3];
	C18_2[4][0] = calf_temp_point_2[4][2] + 3 * asf[4];
	C18_2[4][4] = calf_temp_point_2[4][2] - 3 * asf[4];
	C18_2[5][0] = calf_temp_point_2[5][2] + 2.5*asf[5];
	C18_2[5][4] = calf_temp_point_2[5][2] - 2.5*asf[5];

	C18_2[1][1] = (C18_2[1][0] - C18_2[1][2])*0.7 + C18_2[1][2];
	C18_2[1][3] = (C18_2[1][4] - C18_2[1][2])*0.7 + C18_2[1][2];
	C18_2[1][5] = (C18_2[1][4] - C18_2[1][2])*0.7 + C18_2[1][2];
	C18_2[1][7] = (C18_2[1][0] - C18_2[1][2])*0.7 + C18_2[1][2];

	C18_2[2][1] = (C18_2[2][0] - C18_2[2][2])*0.7 + C18_2[2][2];
	C18_2[2][3] = (C18_2[2][4] - C18_2[2][2])*0.71 + C18_2[2][2];
	C18_2[2][5] = (C18_2[2][4] - C18_2[2][2])*0.71 + C18_2[2][2];
	C18_2[2][7] = (C18_2[2][0] - C18_2[2][2])*0.68 + C18_2[2][2];

	C18_2[3][1] = (C18_2[3][0] - C18_2[3][2])*0.7 + C18_2[3][2];
	C18_2[3][3] = (C18_2[3][4] - C18_2[3][2])*0.71 + C18_2[3][2];
	C18_2[3][5] = (C18_2[3][4] - C18_2[3][2])*0.71 + C18_2[3][2];
	C18_2[3][7] = (C18_2[3][0] - C18_2[3][2])*0.70 + C18_2[3][2];

	C18_2[4][1] = (C18_2[4][0] - C18_2[4][2])*0.7 + C18_2[4][2];
	C18_2[4][3] = (C18_2[4][4] - C18_2[4][2])*0.7 + C18_2[4][2];
	C18_2[4][5] = (C18_2[4][4] - C18_2[4][2])*0.7 + C18_2[4][2];
	C18_2[4][7] = (C18_2[4][0] - C18_2[4][2])*0.7 + C18_2[4][2];

	C18_2[5][1] = (C18_2[5][0] - C18_2[5][2])*0.7 + C18_2[5][2];
	C18_2[5][3] = (C18_2[5][4] - C18_2[5][2])*0.7 + C18_2[5][2];
	C18_2[5][5] = (C18_2[5][4] - C18_2[5][2])*0.7 + C18_2[5][2];
	C18_2[5][7] = (C18_2[5][0] - C18_2[5][2])*0.7 + C18_2[5][2];

	for (int i = 0; i<6; i++)
	{
		for (int j = 0; j<8; j++)
		{
			C16[i][j] = -C16_2[i][(8 - j) % 8];
			C17[i][j] = C17_2[i][(8 - j) % 8];
			C18[i][j] = C18_2[i][(8 - j) % 8];

		}
	}

	body_temp_point[0][0] = head_point[0];
	body_temp_point[0][1] = head_point[1];
	body_temp_point[0][2] = head_point[2];
	body_temp_point[8][0] = waist_point[0];
	body_temp_point[8][1] = waist_point[1];
	body_temp_point[8][2] = waist_point[2];

	for (int i = 0; i<3; i++)
	{
		body_temp_point[1][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 43 / 46 + body_temp_point[8][i];
		body_temp_point[2][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 40 / 46 + body_temp_point[8][i];
		body_temp_point[3][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 34 / 46 + body_temp_point[8][i];
		body_temp_point[4][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 28 / 46 + body_temp_point[8][i];
		body_temp_point[5][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 22 / 46 + body_temp_point[8][i];
		body_temp_point[6][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 16 / 46 + body_temp_point[8][i];
		body_temp_point[7][i] = (body_temp_point[0][i] - body_temp_point[8][i]) * 8 / 46 + body_temp_point[8][i];

	}
	for (int i = 0; i<9; i++)
	{
		for (int j = 0; j<16; j++)
		{
			C2[i][j] = body_temp_point[i][1];

		}

	}
	for (int i = 0; i<8; i++)
	{
		C1[i][0] = C1[i][8] = 0;
	}

	C1[0][4] = C1[1][4] = neck_length;
	C1[0][12] = C1[1][12] = -neck_length;
	C1[0][1] = C1[0][7] = C1[1][1] = C1[1][7] = neck_length*0.35;
	C1[0][2] = C1[0][6] = C1[1][2] = C1[1][6] = neck_length*0.7;
	C1[0][3] = C1[0][5] = C1[1][3] = C1[1][5] = neck_length*0.9;
	C1[0][9] = C1[0][15] = C1[1][9] = C1[1][15] = -neck_length*0.35;
	C1[0][10] = C1[0][14] = C1[1][10] = C1[1][14] = -neck_length*0.7;
	C1[0][11] = C1[0][13] = C1[1][11] = C1[1][13] = -neck_length*0.9;

	C1[2][4] = Shoulder_length;
	C1[2][12] = -Shoulder_length;
	C1[2][1] = C1[2][7] = Shoulder_length*0.35;
	C1[2][2] = C1[2][6] = Shoulder_length*0.7;
	C1[2][3] = C1[2][5] = Shoulder_length*0.9;
	C1[2][9] = C1[2][15] = -Shoulder_length*0.35;
	C1[2][10] = C1[2][14] = -Shoulder_length*0.7;
	C1[2][11] = C1[2][13] = -Shoulder_length*0.9;

	C1[3][4] = chest_length * 14 / 12.5;
	C1[3][12] = -chest_length * 14 / 12.5;
	C1[3][1] = C1[3][7] = C1[3][4] * 0.35;
	C1[3][2] = C1[3][6] = C1[3][4] * 0.7;
	C1[3][3] = C1[3][5] = C1[3][4] * 0.9;
	C1[3][9] = C1[3][15] = -C1[3][4] * 0.35;
	C1[3][10] = C1[3][14] = -C1[3][4] * 0.7;
	C1[3][11] = C1[3][13] = -C1[3][4] * 0.9;

	C1[4][4] = chest_length;
	C1[4][12] = -chest_length;
	C1[4][1] = C1[4][7] = C1[4][4] * 0.35;
	C1[4][2] = C1[4][6] = C1[4][4] * 0.7;
	C1[4][3] = C1[4][5] = C1[4][4] * 0.9;
	C1[4][9] = C1[4][15] = -C1[4][4] * 0.35;
	C1[4][10] = C1[4][14] = -C1[4][4] * 0.7;
	C1[4][11] = C1[4][13] = -C1[4][4] * 0.9;

	C1[5][4] = chest_length*12.1 / 12.5;
	C1[5][12] = -chest_length*12.1 / 12.5;
	C1[5][1] = C1[5][7] = C1[5][4] * 0.35;
	C1[5][2] = C1[5][6] = C1[5][4] * 0.7;
	C1[5][3] = C1[5][5] = C1[5][4] * 0.9;
	C1[5][9] = C1[5][15] = -C1[5][4] * 0.35;
	C1[5][10] = C1[5][14] = -C1[5][4] * 0.7;
	C1[5][11] = C1[5][13] = -C1[5][4] * 0.9;


	C1[6][4] = chest_length*11.3 / 12.5;
	C1[6][12] = -chest_length*11.3 / 12.5;
	C1[6][1] = C1[6][7] = C1[6][4] * 0.35;
	C1[6][2] = C1[6][6] = C1[6][4] * 0.7;
	C1[6][3] = C1[6][5] = C1[6][4] * 0.9;
	C1[6][9] = C1[6][15] = -C1[6][4] * 0.35;
	C1[6][10] = C1[6][14] = -C1[6][4] * 0.7;
	C1[6][11] = C1[6][13] = -C1[6][4] * 0.9;

	for (int j = 0; j<16; j++)
	{
		C1[8][j] = C4[0][j];
	}

	C1[7][4] = C1[8][4] * 11.5 / 12.8;
	C1[7][12] = C1[8][12] * 11.5 / 12.8;
	C1[7][1] = C1[7][7] = C1[7][4] * 0.35;
	C1[7][2] = C1[7][6] = C1[7][4] * 0.7;
	C1[7][3] = C1[7][5] = C1[7][4] * 0.9;
	C1[7][9] = C1[7][15] = -C1[7][4] * 0.35;
	C1[7][10] = C1[7][14] = -C1[7][4] * 0.7;
	C1[7][11] = C1[7][13] = -C1[7][4] * 0.9;


	//C3[0][0] = 4.75*neck_length / 5.4;
	//C3[0][8] = -1 * neck_length / 5.4;
	C3[0][0] = 4.75*neck_length / 5.4;
	C3[0][8] = -neck_length;
	C3[0][4] = C3[0][12] = 0;
	C3[0][15] = C3[0][1] = C3[0][0] * 0.89;
	C3[0][14] = C3[0][2] = C3[0][0] * 0.52;
	C3[0][13] = C3[0][3] = C3[0][0] * 0.1;
	C3[0][11] = C3[0][5] = C3[0][8] * 0.78;
	C3[0][10] = C3[0][6] = C3[0][8] * 0.89;
	C3[0][9] = C3[0][7] = C3[0][8] * 1;
	for (int i = 0; i<16; i++)
	{
		C3[1][i] = C3[0][i];
	}

	C3[2][0] = 5.5*Shoulder_length / 15.3;
	//C3[2][8] = -5.5*neck_length / 15.3;
	C3[2][8] = -neck_length;
	C3[2][4] = C3[2][12] = 0;
	C3[2][15] = C3[2][1] = C3[2][0] * 0.58;
	C3[2][14] = C3[2][2] = C3[2][0] * 0.45;
	C3[2][13] = C3[2][3] = C3[2][0] * 0.2;
	C3[2][11] = C3[2][5] = C3[2][8] * 0.42;
	C3[2][10] = C3[2][6] = C3[2][8] * 0.54;
	C3[2][9] = C3[2][7] = C3[2][8] * 1;

	//C3[3][0] = 7.6*chest_length / 12.5;
	//C3[3][8] = -7.6*chest_length / 12.5;
	C3[3][0] = (side_chest_width / (1.39 + 1.07)) * 7.6 / 8.75;
	C3[3][8] = -(side_chest_width / (1.39 + 1.07)) * 7.6 / 8.75 * 0.9;
	C3[3][4] = C3[3][12] = 0;
	C3[3][15] = C3[3][1] = C3[3][0] * 1;
	C3[3][14] = C3[3][2] = C3[3][0] * 0.71;
	C3[3][13] = C3[3][3] = C3[3][0] * 0.61;
	C3[3][11] = C3[3][5] = C3[3][8] * 0.88;
	C3[3][10] = C3[3][6] = C3[3][8] * 0.95;
	C3[3][9] = C3[3][7] = C3[3][8] * 1.04;

	//C3[4][0] = 8.75*chest_length / 12.5;
	//C3[4][8] = -8.75*chest_length / 12.5;
	C3[4][0] = side_chest_width / (1.39 + 1.07);
	C3[4][8] = -side_chest_width / (1.39 + 1.07) * 0.9;
	C3[4][15] = C3[4][1] = C3[4][0] * 1.34;
	C3[4][14] = C3[4][2] = C3[4][0] * 1.39;
	C3[4][13] = C3[4][3] = C3[4][0] * 1.15;
	C3[4][11] = C3[4][5] = C3[4][8] * 0.8;
	C3[4][10] = C3[4][6] = C3[4][8] * 0.97;
	C3[4][9] = C3[4][7] = C3[4][8] * 1.07;

	//C3[5][0] = 7.6*chest_length / 12.5;
	//C3[5][8] = -7.6*chest_length / 12.5;
	C3[5][0] = (side_chest_width / (1.39 + 1.07)) * 7.6 / 8.75;
	C3[5][8] = -(side_chest_width / (1.39 + 1.07)) * 7.6 / 8.75;
	C3[5][4] = C3[5][12] = 0;
	C3[5][15] = C3[5][1] = C3[5][0] * 1.4;
	C3[5][14] = C3[5][2] = C3[5][0] * 1.45;
	C3[5][13] = C3[5][3] = C3[5][0] * 1.23;
	C3[5][11] = C3[5][5] = C3[5][8] * 0.53;
	C3[5][10] = C3[5][6] = C3[5][8] * 0.82;
	C3[5][9] = C3[5][7] = C3[5][8] * 1.03;

	//C3[6][0] = 8.3*((chest_length / 12.5 - 1)*0.4 + 1);
	//C3[6][8] = -8.3*((chest_length / 12.5 - 1)*0.4 + 1);
	C3[6][0] = 8.3*((C3[5][0] / 7.6 - 1)*0.4 + 1);
	C3[6][8] = -8.3*((C3[5][0] / 7.6 - 1)*0.4 + 1);
	C3[6][4] = C3[6][12] = 0;
	C3[6][15] = C3[6][1] = C3[6][0] * 0.94;
	C3[6][14] = C3[6][2] = C3[6][0] * 0.67;
	C3[6][13] = C3[6][3] = C3[6][0] * 0.34;
	C3[6][11] = C3[6][5] = C3[6][8] * 0.46;
	C3[6][10] = C3[6][6] = C3[6][8] * 0.8;
	C3[6][9] = C3[6][7] = C3[6][8] * 1;


	C3[7][0] = 7.05 * C6[0][0] / 7.2;
	C3[7][8] = -7.05 * C6[0][0] / 7.2;
	C3[7][4] = C3[7][12] = 0;
	C3[7][15] = C3[7][1] = C3[7][0] * 0.89;
	C3[7][14] = C3[7][2] = C3[7][0] * 0.6;
	C3[7][13] = C3[7][3] = C3[7][0] * 0.4;
	C3[7][11] = C3[7][5] = C3[7][8] * 0.58;
	C3[7][10] = C3[7][6] = C3[7][8] * 0.87;
	C3[7][9] = C3[7][7] = C3[7][8] * 1;
	for (int j = 0; j<16; j++)
	{
		C3[8][j] = C6[0][j];
	}


}


void Human::Calculation()
{
	if (gender == false)
		CalculationXZD();
	else if (gender == true)
		CalculationXZD_female();

	for (int i = 0; i<6; i++)
	{
		for (int j = 0; j<16; j++)
		{
			B4[i + 2][j] = C4[i][j];
			B5[i + 2][j] = C5[i][j];
			B6[i + 2][j] = C6[i][j];
		}
	}
	for (int j = 0; j<16; j++)
	{
		B4[0][j] = C1[6][j];
		B4[1][j] = C1[7][j];
		B5[0][j] = C2[6][j];
		B5[1][j] = C2[7][j];
		B6[0][j] = C3[6][j];
		B6[1][j] = C3[7][j];
	}
	B4[8][0] = B4[9][0] = 0;
	B4[8][1] = C7[1][7];
	for (int i = 2; i<8; i++)
	{
		B4[8][i] = C7[1][i - 2];
	}
	B4[8][8] = 0;
	B4[8][9] = C10[1][3];
	B4[8][10] = C10[1][4];
	B4[8][11] = C10[1][5];
	B4[8][12] = C10[1][6];
	B4[8][13] = C10[1][7];
	B4[8][14] = C10[1][0];
	B4[8][15] = C10[1][1];

	B4[9][1] = C7[2][7];
	for (int i = 2; i<8; i++)
	{
		B4[9][i] = C7[2][i - 2];
	}
	B4[9][8] = 0;
	B4[9][9] = C10[2][3];
	B4[9][10] = C10[2][4];
	B4[9][11] = C10[2][5];
	B4[9][12] = C10[2][6];
	B4[9][13] = C10[2][7];
	B4[9][14] = C10[2][0];
	B4[9][15] = C10[2][1];

	//
	B5[8][0] = C8[1][1];
	B5[9][0] = C8[2][1];
	B5[8][1] = C8[1][7];
	for (int i = 2; i<8; i++)
	{
		B5[8][i] = C8[1][i - 2];
	}
	B5[8][8] = 0;
	B5[8][9] = C11[1][3];
	B5[8][10] = C11[1][4];
	B5[8][11] = C11[1][5];
	B5[8][12] = C11[1][6];
	B5[8][13] = C11[1][7];
	B5[8][14] = C11[1][0];
	B5[8][15] = C11[1][1];

	B5[9][1] = C8[2][7];
	for (int i = 2; i<8; i++)
	{
		B5[9][i] = C8[2][i - 2];
	}
	B5[9][8] = 0;
	B5[9][9] = C11[2][3];
	B5[9][10] = C11[2][4];
	B5[9][11] = C11[2][5];
	B5[9][12] = C11[2][6];
	B5[9][13] = C11[2][7];
	B5[9][14] = C11[2][0];
	B5[9][15] = C11[2][1];
	//
	B6[8][0] = B6[7][0];
	B6[9][0] = B6[7][0];
	B6[8][1] = C9[1][7];
	for (int i = 2; i<8; i++)
	{
		B6[8][i] = C9[1][i - 2];
	}
	B6[8][8] = 0;
	B6[8][9] = C12[1][3];
	B6[8][10] = C12[1][4];
	B6[8][11] = C12[1][5];
	B6[8][12] = C12[1][6];
	B6[8][13] = C12[1][7];
	B6[8][14] = C12[1][0];
	B6[8][15] = C12[1][1];

	B6[9][1] = C9[2][7];
	for (int i = 2; i<8; i++)
	{
		B6[9][i] = C9[2][i - 2];
	}
	B6[9][8] = 0;
	B6[9][9] = C12[2][3];
	B6[9][10] = C12[2][4];
	B6[9][11] = C12[2][5];
	B6[9][12] = C12[2][6];
	B6[9][13] = C12[2][7];
	B6[9][14] = C12[2][0];
	B6[9][15] = C12[2][1];
	for (int i = 0; i<10; i++)
	{
		for (int j = 16; j<21; j++)
		{
			B4[i][j] = B4[i][j - 16];
			B5[i][j] = B5[i][j - 16];
			B6[i][j] = B6[i][j - 16];

		}
	}

	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			second_x[i][j] = j * 16 / POINTNUMEACHPART;
			second_y[i][j] = i * 5 / POINTNUMEACHPART;
			if ((second_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((second_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] + 1);
			}

			if ((second_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((second_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] + 2);
			}

			if (second_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((second_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] + 0);
			}

			if ((second_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((second_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 1);
			}

			if ((second_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((second_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 2);
			}

			if ((second_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((second_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 3);
			}

			if ((second_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((second_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 4);
			}

			if ((second_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((second_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 5);
			}

			if ((second_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((second_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 6);
			}

			if ((second_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((second_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 7);
			}

			if ((second_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((second_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 8);
			}

			if ((second_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((second_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 9);
			}

			if ((second_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((second_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 10);
			}

			if ((second_x[i][j] - 11) == 0)
			{
				a11 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 11), 2.0f) / (-3);
				a11 = sin((second_x[i][j] - 11)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 11);
			}

			if ((second_x[i][j] - 12) == 0)
			{
				a12 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 12), 2.0f) / (-3);
				a12 = sin((second_x[i][j] - 12)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 12);
			}
			if ((second_x[i][j] - 13) == 0)
			{
				a13 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 13), 2.0f) / (-3);
				a13 = sin((second_x[i][j] - 13)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 13);
			}
			if ((second_x[i][j] - 14) == 0)
			{
				a14 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 14), 2.0f) / (-3);
				a14 = sin((second_x[i][j] - 14)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 14);
			}
			if ((second_x[i][j] - 15) == 0)
			{
				a15 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 15), 2.0f) / (-3);
				a15 = sin((second_x[i][j] - 15)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 15);
			}
			if ((second_x[i][j] - 16) == 0)
			{
				a16 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 16), 2.0f) / (-3);
				a16 = sin((second_x[i][j] - 16)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 16);
			}
			if ((second_x[i][j] - 17) == 0)
			{
				a17 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 17), 2.0f) / (-3);
				a17 = sin((second_x[i][j] - 17)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 17);
			}
			if ((second_x[i][j] - 18) == 0)
			{
				a18 = 1;
			}
			else
			{
				float temp = pow((second_x[i][j] - 18), 2.0f) / (-3);
				a18 = sin((second_x[i][j] - 18)*GL_PI)*pow(xe, temp) / GL_PI / (second_x[i][j] - 18);
			}
			if ((second_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((second_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] + 1);
			}

			if ((second_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((second_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] + 2);
			}

			if ((second_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((second_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] + 0);
			}

			if ((second_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((second_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] - 1);
			}

			if ((second_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((second_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] - 2);
			}

			if ((second_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((second_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] - 3);
			}

			if ((second_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((second_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] - 4);
			}

			if ((second_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((second_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] - 5);
			}

			if ((second_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((second_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] - 6);
			}

			if ((second_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((second_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((second_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (second_y[i][j] - 7);
			}
			g0 = b_2*B6[0][0] + b_1*B6[1][0] + b0*B6[2][0] + b1*B6[3][0] + b2*B6[4][0] + b3*B6[5][0] + b4*B6[6][0] + b5*B6[7][0] + b6*B6[8][0] + b7*B6[9][0];
			g1 = b_2*B6[0][1] + b_1*B6[1][1] + b0*B6[2][1] + b1*B6[3][1] + b2*B6[4][1] + b3*B6[5][1] + b4*B6[6][1] + b5*B6[7][1] + b6*B6[8][1] + b7*B6[9][1];
			g2 = b_2*B6[0][2] + b_1*B6[1][2] + b0*B6[2][2] + b1*B6[3][2] + b2*B6[4][2] + b3*B6[5][2] + b4*B6[6][2] + b5*B6[7][2] + b6*B6[8][2] + b7*B6[9][2];
			g3 = b_2*B6[0][3] + b_1*B6[1][3] + b0*B6[2][3] + b1*B6[3][3] + b2*B6[4][3] + b3*B6[5][3] + b4*B6[6][3] + b5*B6[7][3] + b6*B6[8][3] + b7*B6[9][3];
			g4 = b_2*B6[0][4] + b_1*B6[1][4] + b0*B6[2][4] + b1*B6[3][4] + b2*B6[4][4] + b3*B6[5][4] + b4*B6[6][4] + b5*B6[7][4] + b6*B6[8][4] + b7*B6[9][4];
			g5 = b_2*B6[0][5] + b_1*B6[1][5] + b0*B6[2][5] + b1*B6[3][5] + b2*B6[4][5] + b3*B6[5][5] + b4*B6[6][5] + b5*B6[7][5] + b6*B6[8][5] + b7*B6[9][5];
			g6 = b_2*B6[0][6] + b_1*B6[1][6] + b0*B6[2][6] + b1*B6[3][6] + b2*B6[4][6] + b3*B6[5][6] + b4*B6[6][6] + b5*B6[7][6] + b6*B6[8][6] + b7*B6[9][6];
			g7 = b_2*B6[0][7] + b_1*B6[1][7] + b0*B6[2][7] + b1*B6[3][7] + b2*B6[4][7] + b3*B6[5][7] + b4*B6[6][7] + b5*B6[7][7] + b6*B6[8][7] + b7*B6[9][7];
			g8 = b_2*B6[0][8] + b_1*B6[1][8] + b0*B6[2][8] + b1*B6[3][8] + b2*B6[4][8] + b3*B6[5][8] + b4*B6[6][8] + b5*B6[7][8] + b6*B6[8][8] + b7*B6[9][8];
			g9 = b_2*B6[0][9] + b_1*B6[1][9] + b0*B6[2][9] + b1*B6[3][9] + b2*B6[4][9] + b3*B6[5][9] + b4*B6[6][9] + b5*B6[7][9] + b6*B6[8][9] + b7*B6[9][9];
			g10 = b_2*B6[0][10] + b_1*B6[1][10] + b0*B6[2][10] + b1*B6[3][10] + b2*B6[4][10] + b3*B6[5][10] + b4*B6[6][10] + b5*B6[7][10] + b6*B6[8][10] + b7*B6[9][10];
			g11 = b_2*B6[0][11] + b_1*B6[1][11] + b0*B6[2][11] + b1*B6[3][11] + b2*B6[4][11] + b3*B6[5][11] + b4*B6[6][11] + b5*B6[7][11] + b6*B6[8][11] + b7*B6[9][11];
			g12 = b_2*B6[0][12] + b_1*B6[1][12] + b0*B6[2][12] + b1*B6[3][12] + b2*B6[4][12] + b3*B6[5][12] + b4*B6[6][12] + b5*B6[7][12] + b6*B6[8][12] + b7*B6[9][12];
			g13 = b_2*B6[0][13] + b_1*B6[1][13] + b0*B6[2][13] + b1*B6[3][13] + b2*B6[4][13] + b3*B6[5][13] + b4*B6[6][13] + b5*B6[7][13] + b6*B6[8][13] + b7*B6[9][13];
			g14 = b_2*B6[0][14] + b_1*B6[1][14] + b0*B6[2][14] + b1*B6[3][14] + b2*B6[4][14] + b3*B6[5][14] + b4*B6[6][14] + b5*B6[7][14] + b6*B6[8][14] + b7*B6[9][14];
			g15 = b_2*B6[0][15] + b_1*B6[1][15] + b0*B6[2][15] + b1*B6[3][15] + b2*B6[4][15] + b3*B6[5][15] + b4*B6[6][15] + b5*B6[7][15] + b6*B6[8][15] + b7*B6[9][15];
			g16 = b_2*B6[0][16] + b_1*B6[1][16] + b0*B6[2][16] + b1*B6[3][16] + b2*B6[4][16] + b3*B6[5][16] + b4*B6[6][16] + b5*B6[7][16] + b6*B6[8][16] + b7*B6[9][16];
			g17 = b_2*B6[0][17] + b_1*B6[1][17] + b0*B6[2][17] + b1*B6[3][17] + b2*B6[4][17] + b3*B6[5][17] + b4*B6[6][17] + b5*B6[7][17] + b6*B6[8][17] + b7*B6[9][17];
			g18 = b_2*B6[0][18] + b_1*B6[1][18] + b0*B6[2][18] + b1*B6[3][18] + b2*B6[4][18] + b3*B6[5][18] + b4*B6[6][18] + b5*B6[7][18] + b6*B6[8][18] + b7*B6[9][18];
			g19 = b_2*B6[0][19] + b_1*B6[1][19] + b0*B6[2][19] + b1*B6[3][19] + b2*B6[4][19] + b3*B6[5][19] + b4*B6[6][19] + b5*B6[7][19] + b6*B6[8][19] + b7*B6[9][19];
			g20 = b_2*B6[0][20] + b_1*B6[1][20] + b0*B6[2][20] + b1*B6[3][20] + b2*B6[4][20] + b3*B6[5][20] + b4*B6[6][20] + b5*B6[7][20] + b6*B6[8][20] + b7*B6[9][20];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10 + g13*a11 + g14*a12 + g15*a13 + g16*a14 + g17*a15 + g18*a16 + g19*a17 + g20*a18;
			second_z[i][j] = z;

			m0 = b_2*B5[0][0] + b_1*B5[1][0] + b0*B5[2][0] + b1*B5[3][0] + b2*B5[4][0] + b3*B5[5][0] + b4*B5[6][0] + b5*B5[7][0] + b6*B5[8][0] + b7*B5[9][0];
			m1 = b_2*B5[0][1] + b_1*B5[1][1] + b0*B5[2][1] + b1*B5[3][1] + b2*B5[4][1] + b3*B5[5][1] + b4*B5[6][1] + b5*B5[7][1] + b6*B5[8][1] + b7*B5[9][1];
			m2 = b_2*B5[0][2] + b_1*B5[1][2] + b0*B5[2][2] + b1*B5[3][2] + b2*B5[4][2] + b3*B5[5][2] + b4*B5[6][2] + b5*B5[7][2] + b6*B5[8][2] + b7*B5[9][2];
			m3 = b_2*B5[0][3] + b_1*B5[1][3] + b0*B5[2][3] + b1*B5[3][3] + b2*B5[4][3] + b3*B5[5][3] + b4*B5[6][3] + b5*B5[7][3] + b6*B5[8][3] + b7*B5[9][3];
			m4 = b_2*B5[0][4] + b_1*B5[1][4] + b0*B5[2][4] + b1*B5[3][4] + b2*B5[4][4] + b3*B5[5][4] + b4*B5[6][4] + b5*B5[7][4] + b6*B5[8][4] + b7*B5[9][4];
			m5 = b_2*B5[0][5] + b_1*B5[1][5] + b0*B5[2][5] + b1*B5[3][5] + b2*B5[4][5] + b3*B5[5][5] + b4*B5[6][5] + b5*B5[7][5] + b6*B5[8][5] + b7*B5[9][5];
			m6 = b_2*B5[0][6] + b_1*B5[1][6] + b0*B5[2][6] + b1*B5[3][6] + b2*B5[4][6] + b3*B5[5][6] + b4*B5[6][6] + b5*B5[7][6] + b6*B5[8][6] + b7*B5[9][6];
			m7 = b_2*B5[0][7] + b_1*B5[1][7] + b0*B5[2][7] + b1*B5[3][7] + b2*B5[4][7] + b3*B5[5][7] + b4*B5[6][7] + b5*B5[7][7] + b6*B5[8][7] + b7*B5[9][7];
			m8 = b_2*B5[0][8] + b_1*B5[1][8] + b0*B5[2][8] + b1*B5[3][8] + b2*B5[4][8] + b3*B5[5][8] + b4*B5[6][8] + b5*B5[7][8] + b6*B5[8][8] + b7*B5[9][8];
			m9 = b_2*B5[0][9] + b_1*B5[1][9] + b0*B5[2][9] + b1*B5[3][9] + b2*B5[4][9] + b3*B5[5][9] + b4*B5[6][9] + b5*B5[7][9] + b6*B5[8][9] + b7*B5[9][9];
			m10 = b_2*B5[0][10] + b_1*B5[1][10] + b0*B5[2][10] + b1*B5[3][10] + b2*B5[4][10] + b3*B5[5][10] + b4*B5[6][10] + b5*B5[7][10] + b6*B5[8][10] + b7*B5[9][10];
			m11 = b_2*B5[0][11] + b_1*B5[1][11] + b0*B5[2][11] + b1*B5[3][11] + b2*B5[4][11] + b3*B5[5][11] + b4*B5[6][11] + b5*B5[7][11] + b6*B5[8][11] + b7*B5[9][11];
			m12 = b_2*B5[0][12] + b_1*B5[1][12] + b0*B5[2][12] + b1*B5[3][12] + b2*B5[4][12] + b3*B5[5][12] + b4*B5[6][12] + b5*B5[7][12] + b6*B5[8][12] + b7*B5[9][12];
			m13 = b_2*B5[0][13] + b_1*B5[1][13] + b0*B5[2][13] + b1*B5[3][13] + b2*B5[4][13] + b3*B5[5][13] + b4*B5[6][13] + b5*B5[7][13] + b6*B5[8][13] + b7*B5[9][13];
			m14 = b_2*B5[0][14] + b_1*B5[1][14] + b0*B5[2][14] + b1*B5[3][14] + b2*B5[4][14] + b3*B5[5][14] + b4*B5[6][14] + b5*B5[7][14] + b6*B5[8][14] + b7*B5[9][14];
			m15 = b_2*B5[0][15] + b_1*B5[1][15] + b0*B5[2][15] + b1*B5[3][15] + b2*B5[4][15] + b3*B5[5][15] + b4*B5[6][15] + b5*B5[7][15] + b6*B5[8][15] + b7*B5[9][15];
			m16 = b_2*B5[0][16] + b_1*B5[1][16] + b0*B5[2][16] + b1*B5[3][16] + b2*B5[4][16] + b3*B5[5][16] + b4*B5[6][16] + b5*B5[7][16] + b6*B5[8][16] + b7*B5[9][16];
			m17 = b_2*B5[0][17] + b_1*B5[1][17] + b0*B5[2][17] + b1*B5[3][17] + b2*B5[4][17] + b3*B5[5][17] + b4*B5[6][17] + b5*B5[7][17] + b6*B5[8][17] + b7*B5[9][17];
			m18 = b_2*B5[0][18] + b_1*B5[1][18] + b0*B5[2][18] + b1*B5[3][18] + b2*B5[4][18] + b3*B5[5][18] + b4*B5[6][18] + b5*B5[7][18] + b6*B5[8][18] + b7*B5[9][18];
			m19 = b_2*B5[0][19] + b_1*B5[1][19] + b0*B5[2][19] + b1*B5[3][19] + b2*B5[4][19] + b3*B5[5][19] + b4*B5[6][19] + b5*B5[7][19] + b6*B5[8][19] + b7*B5[9][19];
			m20 = b_2*B5[0][20] + b_1*B5[1][20] + b0*B5[2][20] + b1*B5[3][20] + b2*B5[4][20] + b3*B5[5][20] + b4*B5[6][20] + b5*B5[7][20] + b6*B5[8][20] + b7*B5[9][20];
			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10 + m13*a11 + m14*a12 + m15*a13 + m16*a14 + m17*a15 + m18*a16 + m19*a17 + m20*a18;
			second_yy[i][j] = xxxx;

			n0 = b_2*B4[0][0] + b_1*B4[1][0] + b0*B4[2][0] + b1*B4[3][0] + b2*B4[4][0] + b3*B4[5][0] + b4*B4[6][0] + b5*B4[7][0] + b6*B4[8][0] + b7*B4[9][0];
			n1 = b_2*B4[0][1] + b_1*B4[1][1] + b0*B4[2][1] + b1*B4[3][1] + b2*B4[4][1] + b3*B4[5][1] + b4*B4[6][1] + b5*B4[7][1] + b6*B4[8][1] + b7*B4[9][1];
			n2 = b_2*B4[0][2] + b_1*B4[1][2] + b0*B4[2][2] + b1*B4[3][2] + b2*B4[4][2] + b3*B4[5][2] + b4*B4[6][2] + b5*B4[7][2] + b6*B4[8][2] + b7*B4[9][2];
			n3 = b_2*B4[0][3] + b_1*B4[1][3] + b0*B4[2][3] + b1*B4[3][3] + b2*B4[4][3] + b3*B4[5][3] + b4*B4[6][3] + b5*B4[7][3] + b6*B4[8][3] + b7*B4[9][3];
			n4 = b_2*B4[0][4] + b_1*B4[1][4] + b0*B4[2][4] + b1*B4[3][4] + b2*B4[4][4] + b3*B4[5][4] + b4*B4[6][4] + b5*B4[7][4] + b6*B4[8][4] + b7*B4[9][4];
			n5 = b_2*B4[0][5] + b_1*B4[1][5] + b0*B4[2][5] + b1*B4[3][5] + b2*B4[4][5] + b3*B4[5][5] + b4*B4[6][5] + b5*B4[7][5] + b6*B4[8][5] + b7*B4[9][5];
			n6 = b_2*B4[0][6] + b_1*B4[1][6] + b0*B4[2][6] + b1*B4[3][6] + b2*B4[4][6] + b3*B4[5][6] + b4*B4[6][6] + b5*B4[7][6] + b6*B4[8][6] + b7*B4[9][6];
			n7 = b_2*B4[0][7] + b_1*B4[1][7] + b0*B4[2][7] + b1*B4[3][7] + b2*B4[4][7] + b3*B4[5][7] + b4*B4[6][7] + b5*B4[7][7] + b6*B4[8][7] + b7*B4[9][7];
			n8 = b_2*B4[0][8] + b_1*B4[1][8] + b0*B4[2][8] + b1*B4[3][8] + b2*B4[4][8] + b3*B4[5][8] + b4*B4[6][8] + b5*B4[7][8] + b6*B4[8][8] + b7*B4[9][8];
			n9 = b_2*B4[0][9] + b_1*B4[1][9] + b0*B4[2][9] + b1*B4[3][9] + b2*B4[4][9] + b3*B4[5][9] + b4*B4[6][9] + b5*B4[7][9] + b6*B4[8][9] + b7*B4[9][9];
			n10 = b_2*B4[0][10] + b_1*B4[1][10] + b0*B4[2][10] + b1*B4[3][10] + b2*B4[4][10] + b3*B4[5][10] + b4*B4[6][10] + b5*B4[7][10] + b6*B4[8][10] + b7*B4[9][10];
			n11 = b_2*B4[0][11] + b_1*B4[1][11] + b0*B4[2][11] + b1*B4[3][11] + b2*B4[4][11] + b3*B4[5][11] + b4*B4[6][11] + b5*B4[7][11] + b6*B4[8][11] + b7*B4[9][11];
			n12 = b_2*B4[0][12] + b_1*B4[1][12] + b0*B4[2][12] + b1*B4[3][12] + b2*B4[4][12] + b3*B4[5][12] + b4*B4[6][12] + b5*B4[7][12] + b6*B4[8][12] + b7*B4[9][12];
			n13 = b_2*B4[0][13] + b_1*B4[1][13] + b0*B4[2][13] + b1*B4[3][13] + b2*B4[4][13] + b3*B4[5][13] + b4*B4[6][13] + b5*B4[7][13] + b6*B4[8][13] + b7*B4[9][13];
			n14 = b_2*B4[0][14] + b_1*B4[1][14] + b0*B4[2][14] + b1*B4[3][14] + b2*B4[4][14] + b3*B4[5][14] + b4*B4[6][14] + b5*B4[7][14] + b6*B4[8][14] + b7*B4[9][14];
			n15 = b_2*B4[0][15] + b_1*B4[1][15] + b0*B4[2][15] + b1*B4[3][15] + b2*B4[4][15] + b3*B4[5][15] + b4*B4[6][15] + b5*B4[7][15] + b6*B4[8][15] + b7*B4[9][15];
			n16 = b_2*B4[0][16] + b_1*B4[1][16] + b0*B4[2][16] + b1*B4[3][16] + b2*B4[4][16] + b3*B4[5][16] + b4*B4[6][16] + b5*B4[7][16] + b6*B4[8][16] + b7*B4[9][16];
			n17 = b_2*B4[0][17] + b_1*B4[1][17] + b0*B4[2][17] + b1*B4[3][17] + b2*B4[4][17] + b3*B4[5][17] + b4*B4[6][17] + b5*B4[7][17] + b6*B4[8][17] + b7*B4[9][17];
			n18 = b_2*B4[0][18] + b_1*B4[1][18] + b0*B4[2][18] + b1*B4[3][18] + b2*B4[4][18] + b3*B4[5][18] + b4*B4[6][18] + b5*B4[7][18] + b6*B4[8][18] + b7*B4[9][18];
			n19 = b_2*B4[0][19] + b_1*B4[1][19] + b0*B4[2][19] + b1*B4[3][19] + b2*B4[4][19] + b3*B4[5][19] + b4*B4[6][19] + b5*B4[7][19] + b6*B4[8][19] + b7*B4[9][19];
			n20 = b_2*B4[0][20] + b_1*B4[1][20] + b0*B4[2][20] + b1*B4[3][20] + b2*B4[4][20] + b3*B4[5][20] + b4*B4[6][20] + b5*B4[7][20] + b6*B4[8][20] + b7*B4[9][20];
			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10 + n13*a11 + n14*a12 + n15*a13 + n16*a14 + n17*a15 + n18*a16 + n19*a17 + n20*a18;
			second_xx[i][j] = kkkk;
		}
	}


	//右大腿
	for (int i = 2; i<8; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B7[i][j] = C7[i - 2][j];
			B8[i][j] = C8[i - 2][j];
			B9[i][j] = C9[i - 2][j];
		}
	}
	for (int i = 0; i<6; i++)
	{
		B7[0][i] = C4[3][i + 2];
		B7[1][i] = C4[4][i + 2];
		B8[0][i] = C5[3][i + 2];
		B8[1][i] = C5[4][i + 2];
		B9[0][i] = C6[3][i + 2];
		B9[1][i] = C6[4][i + 2];
	}
	B7[0][6] = B7[1][6] = B7[2][6];
	B8[0][6] = C5[3][1];
	B8[1][6] = C5[4][1];
	B9[0][6] = B9[1][6] = 0;
	B7[0][7] = C4[3][1];
	B7[1][7] = C4[4][1];
	B8[0][7] = C5[3][1];
	B8[1][7] = C5[4][1];
	B9[0][7] = C6[3][1];
	B9[1][7] = C6[4][1];

	for (int i = 8; i<10; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B7[i][j] = C13[i - 7][j];
			B8[i][j] = C14[i - 7][j];
			B9[i][j] = C15[i - 7][j];
		}
	}

	for (int i = 0; i<10; i++)
	{
		for (int j = 8; j<13; j++)
		{
			B7[i][j] = B7[i][j - 8];
			B8[i][j] = B8[i][j - 8];
			B9[i][j] = B9[i][j - 8];

		}
	}

	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			third_x[i][j] = j * 8 / POINTNUMEACHPART;
			third_y[i][j] = i * 5 / POINTNUMEACHPART;
			if ((third_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((third_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] + 1);
			}

			if ((third_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((third_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] + 2);
			}

			if (third_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((third_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] + 0);
			}

			if ((third_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((third_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 1);
			}

			if ((third_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((third_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 2);
			}

			if ((third_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((third_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 3);
			}

			if ((third_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((third_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 4);
			}

			if ((third_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((third_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 5);
			}

			if ((third_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((third_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 6);
			}

			if ((third_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((third_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 7);
			}

			if ((third_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((third_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 8);
			}

			if ((third_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((third_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 9);
			}

			if ((third_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((third_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((third_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (third_x[i][j] - 10);
			}


			if ((third_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((third_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] + 1);
			}

			if ((third_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((third_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] + 2);
			}

			if ((third_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((third_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] + 0);
			}

			if ((third_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((third_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] - 1);
			}

			if ((third_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((third_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] - 2);
			}

			if ((third_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((third_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] - 3);
			}

			if ((third_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((third_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] - 4);
			}

			if ((third_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((third_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] - 5);
			}

			if ((third_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((third_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] - 6);
			}

			if ((third_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((third_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((third_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (third_y[i][j] - 7);
			}
			g0 = b_2*B9[0][0] + b_1*B9[1][0] + b0*B9[2][0] + b1*B9[3][0] + b2*B9[4][0] + b3*B9[5][0] + b4*B9[6][0] + b5*B9[7][0] + b6*B9[8][0] + b7*B9[9][0];
			g1 = b_2*B9[0][1] + b_1*B9[1][1] + b0*B9[2][1] + b1*B9[3][1] + b2*B9[4][1] + b3*B9[5][1] + b4*B9[6][1] + b5*B9[7][1] + b6*B9[8][1] + b7*B9[9][1];
			g2 = b_2*B9[0][2] + b_1*B9[1][2] + b0*B9[2][2] + b1*B9[3][2] + b2*B9[4][2] + b3*B9[5][2] + b4*B9[6][2] + b5*B9[7][2] + b6*B9[8][2] + b7*B9[9][2];
			g3 = b_2*B9[0][3] + b_1*B9[1][3] + b0*B9[2][3] + b1*B9[3][3] + b2*B9[4][3] + b3*B9[5][3] + b4*B9[6][3] + b5*B9[7][3] + b6*B9[8][3] + b7*B9[9][3];
			g4 = b_2*B9[0][4] + b_1*B9[1][4] + b0*B9[2][4] + b1*B9[3][4] + b2*B9[4][4] + b3*B9[5][4] + b4*B9[6][4] + b5*B9[7][4] + b6*B9[8][4] + b7*B9[9][4];
			g5 = b_2*B9[0][5] + b_1*B9[1][5] + b0*B9[2][5] + b1*B9[3][5] + b2*B9[4][5] + b3*B9[5][5] + b4*B9[6][5] + b5*B9[7][5] + b6*B9[8][5] + b7*B9[9][5];
			g6 = b_2*B9[0][6] + b_1*B9[1][6] + b0*B9[2][6] + b1*B9[3][6] + b2*B9[4][6] + b3*B9[5][6] + b4*B9[6][6] + b5*B9[7][6] + b6*B9[8][6] + b7*B9[9][6];
			g7 = b_2*B9[0][7] + b_1*B9[1][7] + b0*B9[2][7] + b1*B9[3][7] + b2*B9[4][7] + b3*B9[5][7] + b4*B9[6][7] + b5*B9[7][7] + b6*B9[8][7] + b7*B9[9][7];
			g8 = b_2*B9[0][8] + b_1*B9[1][8] + b0*B9[2][8] + b1*B9[3][8] + b2*B9[4][8] + b3*B9[5][8] + b4*B9[6][8] + b5*B9[7][8] + b6*B9[8][8] + b7*B9[9][8];
			g9 = b_2*B9[0][9] + b_1*B9[1][9] + b0*B9[2][9] + b1*B9[3][9] + b2*B9[4][9] + b3*B9[5][9] + b4*B9[6][9] + b5*B9[7][9] + b6*B9[8][9] + b7*B9[9][9];
			g10 = b_2*B9[0][10] + b_1*B9[1][10] + b0*B9[2][10] + b1*B9[3][10] + b2*B9[4][10] + b3*B9[5][10] + b4*B9[6][10] + b5*B9[7][10] + b6*B9[8][10] + b7*B9[9][10];
			g11 = b_2*B9[0][11] + b_1*B9[1][11] + b0*B9[2][11] + b1*B9[3][11] + b2*B9[4][11] + b3*B9[5][11] + b4*B9[6][11] + b5*B9[7][11] + b6*B9[8][11] + b7*B9[9][11];
			g12 = b_2*B9[0][12] + b_1*B9[1][12] + b0*B9[2][12] + b1*B9[3][12] + b2*B9[4][12] + b3*B9[5][12] + b4*B9[6][12] + b5*B9[7][12] + b6*B9[8][12] + b7*B9[9][12];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10;
			third_z[i][j] = z;

			m0 = b_2*B8[0][0] + b_1*B8[1][0] + b0*B8[2][0] + b1*B8[3][0] + b2*B8[4][0] + b3*B8[5][0] + b4*B8[6][0] + b5*B8[7][0] + b6*B8[8][0] + b7*B8[9][0];
			m1 = b_2*B8[0][1] + b_1*B8[1][1] + b0*B8[2][1] + b1*B8[3][1] + b2*B8[4][1] + b3*B8[5][1] + b4*B8[6][1] + b5*B8[7][1] + b6*B8[8][1] + b7*B8[9][1];
			m2 = b_2*B8[0][2] + b_1*B8[1][2] + b0*B8[2][2] + b1*B8[3][2] + b2*B8[4][2] + b3*B8[5][2] + b4*B8[6][2] + b5*B8[7][2] + b6*B8[8][2] + b7*B8[9][2];
			m3 = b_2*B8[0][3] + b_1*B8[1][3] + b0*B8[2][3] + b1*B8[3][3] + b2*B8[4][3] + b3*B8[5][3] + b4*B8[6][3] + b5*B8[7][3] + b6*B8[8][3] + b7*B8[9][3];
			m4 = b_2*B8[0][4] + b_1*B8[1][4] + b0*B8[2][4] + b1*B8[3][4] + b2*B8[4][4] + b3*B8[5][4] + b4*B8[6][4] + b5*B8[7][4] + b6*B8[8][4] + b7*B8[9][4];
			m5 = b_2*B8[0][5] + b_1*B8[1][5] + b0*B8[2][5] + b1*B8[3][5] + b2*B8[4][5] + b3*B8[5][5] + b4*B8[6][5] + b5*B8[7][5] + b6*B8[8][5] + b7*B8[9][5];
			m6 = b_2*B8[0][6] + b_1*B8[1][6] + b0*B8[2][6] + b1*B8[3][6] + b2*B8[4][6] + b3*B8[5][6] + b4*B8[6][6] + b5*B8[7][6] + b6*B8[8][6] + b7*B8[9][6];
			m7 = b_2*B8[0][7] + b_1*B8[1][7] + b0*B8[2][7] + b1*B8[3][7] + b2*B8[4][7] + b3*B8[5][7] + b4*B8[6][7] + b5*B8[7][7] + b6*B8[8][7] + b7*B8[9][7];
			m8 = b_2*B8[0][8] + b_1*B8[1][8] + b0*B8[2][8] + b1*B8[3][8] + b2*B8[4][8] + b3*B8[5][8] + b4*B8[6][8] + b5*B8[7][8] + b6*B8[8][8] + b7*B8[9][8];
			m9 = b_2*B8[0][9] + b_1*B8[1][9] + b0*B8[2][9] + b1*B8[3][9] + b2*B8[4][9] + b3*B8[5][9] + b4*B8[6][9] + b5*B8[7][9] + b6*B8[8][9] + b7*B8[9][9];
			m10 = b_2*B8[0][10] + b_1*B8[1][10] + b0*B8[2][10] + b1*B8[3][10] + b2*B8[4][10] + b3*B8[5][10] + b4*B8[6][10] + b5*B8[7][10] + b6*B8[8][10] + b7*B8[9][10];
			m11 = b_2*B8[0][11] + b_1*B8[1][11] + b0*B8[2][11] + b1*B8[3][11] + b2*B8[4][11] + b3*B8[5][11] + b4*B8[6][11] + b5*B8[7][11] + b6*B8[8][11] + b7*B8[9][11];
			m12 = b_2*B8[0][12] + b_1*B8[1][12] + b0*B8[2][12] + b1*B8[3][12] + b2*B8[4][12] + b3*B8[5][12] + b4*B8[6][12] + b5*B8[7][12] + b6*B8[8][12] + b7*B8[9][12];

			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10;
			third_yy[i][j] = xxxx;

			n0 = b_2*B7[0][0] + b_1*B7[1][0] + b0*B7[2][0] + b1*B7[3][0] + b2*B7[4][0] + b3*B7[5][0] + b4*B7[6][0] + b5*B7[7][0] + b6*B7[8][0] + b7*B7[9][0];
			n1 = b_2*B7[0][1] + b_1*B7[1][1] + b0*B7[2][1] + b1*B7[3][1] + b2*B7[4][1] + b3*B7[5][1] + b4*B7[6][1] + b5*B7[7][1] + b6*B7[8][1] + b7*B7[9][1];
			n2 = b_2*B7[0][2] + b_1*B7[1][2] + b0*B7[2][2] + b1*B7[3][2] + b2*B7[4][2] + b3*B7[5][2] + b4*B7[6][2] + b5*B7[7][2] + b6*B7[8][2] + b7*B7[9][2];
			n3 = b_2*B7[0][3] + b_1*B7[1][3] + b0*B7[2][3] + b1*B7[3][3] + b2*B7[4][3] + b3*B7[5][3] + b4*B7[6][3] + b5*B7[7][3] + b6*B7[8][3] + b7*B7[9][3];
			n4 = b_2*B7[0][4] + b_1*B7[1][4] + b0*B7[2][4] + b1*B7[3][4] + b2*B7[4][4] + b3*B7[5][4] + b4*B7[6][4] + b5*B7[7][4] + b6*B7[8][4] + b7*B7[9][4];
			n5 = b_2*B7[0][5] + b_1*B7[1][5] + b0*B7[2][5] + b1*B7[3][5] + b2*B7[4][5] + b3*B7[5][5] + b4*B7[6][5] + b5*B7[7][5] + b6*B7[8][5] + b7*B7[9][5];
			n6 = b_2*B7[0][6] + b_1*B7[1][6] + b0*B7[2][6] + b1*B7[3][6] + b2*B7[4][6] + b3*B7[5][6] + b4*B7[6][6] + b5*B7[7][6] + b6*B7[8][6] + b7*B7[9][6];
			n7 = b_2*B7[0][7] + b_1*B7[1][7] + b0*B7[2][7] + b1*B7[3][7] + b2*B7[4][7] + b3*B7[5][7] + b4*B7[6][7] + b5*B7[7][7] + b6*B7[8][7] + b7*B7[9][7];
			n8 = b_2*B7[0][8] + b_1*B7[1][8] + b0*B7[2][8] + b1*B7[3][8] + b2*B7[4][8] + b3*B7[5][8] + b4*B7[6][8] + b5*B7[7][8] + b6*B7[8][8] + b7*B7[9][8];
			n9 = b_2*B7[0][9] + b_1*B7[1][9] + b0*B7[2][9] + b1*B7[3][9] + b2*B7[4][9] + b3*B7[5][9] + b4*B7[6][9] + b5*B7[7][9] + b6*B7[8][9] + b7*B7[9][9];
			n10 = b_2*B7[0][10] + b_1*B7[1][10] + b0*B7[2][10] + b1*B7[3][10] + b2*B7[4][10] + b3*B7[5][10] + b4*B7[6][10] + b5*B7[7][10] + b6*B7[8][10] + b7*B7[9][10];
			n11 = b_2*B7[0][11] + b_1*B7[1][11] + b0*B7[2][11] + b1*B7[3][11] + b2*B7[4][11] + b3*B7[5][11] + b4*B7[6][11] + b5*B7[7][11] + b6*B7[8][11] + b7*B7[9][11];
			n12 = b_2*B7[0][12] + b_1*B7[1][12] + b0*B7[2][12] + b1*B7[3][12] + b2*B7[4][12] + b3*B7[5][12] + b4*B7[6][12] + b5*B7[7][12] + b6*B7[8][12] + b7*B7[9][12];

			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10;
			third_xx[i][j] = kkkk;
		}
	}

	//xiaotui
	for (int i = 2; i<8; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B13[i][j] = C13[i - 2][j];
			B14[i][j] = C14[i - 2][j];
			B15[i][j] = C15[i - 2][j];

		}
	}
	for (int i = 0; i<2; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B13[i][j] = C7[i + 3][j];
			B14[i][j] = C8[i + 3][j];
			B15[i][j] = C9[i + 3][j];
		}
	}
	for (int j = 0; j<8; j++)
	{
		B14[8][j] = calf_point_2[1] - 1;
		B14[9][j] = calf_point_2[1] - 2;
	}
	for (int i = 8; i<10; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B13[i][j] = B13[7][j];
			B15[i][j] = B15[7][j];

		}
	}
	for (int i = 0; i<10; i++)
	{
		for (int j = 8; j<13; j++)
		{
			B13[i][j] = B13[i][j - 8];
			B14[i][j] = B14[i][j - 8];
			B15[i][j] = B15[i][j - 8];

		}
	}

	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			fifth_x[i][j] = j * 8 / POINTNUMEACHPART;
			fifth_y[i][j] = i * 5 / POINTNUMEACHPART;
			if ((fifth_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((fifth_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] + 1);
			}

			if ((fifth_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((fifth_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] + 2);
			}

			if (fifth_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((fifth_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] + 0);
			}

			if ((fifth_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((fifth_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 1);
			}

			if ((fifth_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((fifth_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 2);
			}

			if ((fifth_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((fifth_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 3);
			}

			if ((fifth_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((fifth_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 4);
			}

			if ((fifth_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((fifth_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 5);
			}

			if ((fifth_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((fifth_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 6);
			}

			if ((fifth_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((fifth_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 7);
			}

			if ((fifth_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((fifth_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 8);
			}

			if ((fifth_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((fifth_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 9);
			}

			if ((fifth_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((fifth_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((fifth_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_x[i][j] - 10);
			}


			if ((fifth_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((fifth_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] + 1);
			}

			if ((fifth_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((fifth_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] + 2);
			}

			if ((fifth_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((fifth_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] + 0);
			}

			if ((fifth_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((fifth_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] - 1);
			}

			if ((fifth_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((fifth_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] - 2);
			}

			if ((fifth_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((fifth_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] - 3);
			}

			if ((fifth_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((fifth_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] - 4);
			}

			if ((fifth_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((fifth_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] - 5);
			}

			if ((fifth_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((fifth_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] - 6);
			}

			if ((fifth_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((fifth_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((fifth_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (fifth_y[i][j] - 7);
			}
			g0 = b_2*B15[0][0] + b_1*B15[1][0] + b0*B15[2][0] + b1*B15[3][0] + b2*B15[4][0] + b3*B15[5][0] + b4*B15[6][0] + b5*B15[7][0] + b6*B15[8][0] + b7*B15[9][0];
			g1 = b_2*B15[0][1] + b_1*B15[1][1] + b0*B15[2][1] + b1*B15[3][1] + b2*B15[4][1] + b3*B15[5][1] + b4*B15[6][1] + b5*B15[7][1] + b6*B15[8][1] + b7*B15[9][1];
			g2 = b_2*B15[0][2] + b_1*B15[1][2] + b0*B15[2][2] + b1*B15[3][2] + b2*B15[4][2] + b3*B15[5][2] + b4*B15[6][2] + b5*B15[7][2] + b6*B15[8][2] + b7*B15[9][2];
			g3 = b_2*B15[0][3] + b_1*B15[1][3] + b0*B15[2][3] + b1*B15[3][3] + b2*B15[4][3] + b3*B15[5][3] + b4*B15[6][3] + b5*B15[7][3] + b6*B15[8][3] + b7*B15[9][3];
			g4 = b_2*B15[0][4] + b_1*B15[1][4] + b0*B15[2][4] + b1*B15[3][4] + b2*B15[4][4] + b3*B15[5][4] + b4*B15[6][4] + b5*B15[7][4] + b6*B15[8][4] + b7*B15[9][4];
			g5 = b_2*B15[0][5] + b_1*B15[1][5] + b0*B15[2][5] + b1*B15[3][5] + b2*B15[4][5] + b3*B15[5][5] + b4*B15[6][5] + b5*B15[7][5] + b6*B15[8][5] + b7*B15[9][5];
			g6 = b_2*B15[0][6] + b_1*B15[1][6] + b0*B15[2][6] + b1*B15[3][6] + b2*B15[4][6] + b3*B15[5][6] + b4*B15[6][6] + b5*B15[7][6] + b6*B15[8][6] + b7*B15[9][6];
			g7 = b_2*B15[0][7] + b_1*B15[1][7] + b0*B15[2][7] + b1*B15[3][7] + b2*B15[4][7] + b3*B15[5][7] + b4*B15[6][7] + b5*B15[7][7] + b6*B15[8][7] + b7*B15[9][7];
			g8 = b_2*B15[0][8] + b_1*B15[1][8] + b0*B15[2][8] + b1*B15[3][8] + b2*B15[4][8] + b3*B15[5][8] + b4*B15[6][8] + b5*B15[7][8] + b6*B15[8][8] + b7*B15[9][8];
			g9 = b_2*B15[0][9] + b_1*B15[1][9] + b0*B15[2][9] + b1*B15[3][9] + b2*B15[4][9] + b3*B15[5][9] + b4*B15[6][9] + b5*B15[7][9] + b6*B15[8][9] + b7*B15[9][9];
			g10 = b_2*B15[0][10] + b_1*B15[1][10] + b0*B15[2][10] + b1*B15[3][10] + b2*B15[4][10] + b3*B15[5][10] + b4*B15[6][10] + b5*B15[7][10] + b6*B15[8][10] + b7*B15[9][10];
			g11 = b_2*B15[0][11] + b_1*B15[1][11] + b0*B15[2][11] + b1*B15[3][11] + b2*B15[4][11] + b3*B15[5][11] + b4*B15[6][11] + b5*B15[7][11] + b6*B15[8][11] + b7*B15[9][11];
			g12 = b_2*B15[0][12] + b_1*B15[1][12] + b0*B15[2][12] + b1*B15[3][12] + b2*B15[4][12] + b3*B15[5][12] + b4*B15[6][12] + b5*B15[7][12] + b6*B15[8][12] + b7*B15[9][12];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10;
			fifth_z[i][j] = z;

			m0 = b_2*B14[0][0] + b_1*B14[1][0] + b0*B14[2][0] + b1*B14[3][0] + b2*B14[4][0] + b3*B14[5][0] + b4*B14[6][0] + b5*B14[7][0] + b6*B14[8][0] + b7*B14[9][0];
			m1 = b_2*B14[0][1] + b_1*B14[1][1] + b0*B14[2][1] + b1*B14[3][1] + b2*B14[4][1] + b3*B14[5][1] + b4*B14[6][1] + b5*B14[7][1] + b6*B14[8][1] + b7*B14[9][1];
			m2 = b_2*B14[0][2] + b_1*B14[1][2] + b0*B14[2][2] + b1*B14[3][2] + b2*B14[4][2] + b3*B14[5][2] + b4*B14[6][2] + b5*B14[7][2] + b6*B14[8][2] + b7*B14[9][2];
			m3 = b_2*B14[0][3] + b_1*B14[1][3] + b0*B14[2][3] + b1*B14[3][3] + b2*B14[4][3] + b3*B14[5][3] + b4*B14[6][3] + b5*B14[7][3] + b6*B14[8][3] + b7*B14[9][3];
			m4 = b_2*B14[0][4] + b_1*B14[1][4] + b0*B14[2][4] + b1*B14[3][4] + b2*B14[4][4] + b3*B14[5][4] + b4*B14[6][4] + b5*B14[7][4] + b6*B14[8][4] + b7*B14[9][4];
			m5 = b_2*B14[0][5] + b_1*B14[1][5] + b0*B14[2][5] + b1*B14[3][5] + b2*B14[4][5] + b3*B14[5][5] + b4*B14[6][5] + b5*B14[7][5] + b6*B14[8][5] + b7*B14[9][5];
			m6 = b_2*B14[0][6] + b_1*B14[1][6] + b0*B14[2][6] + b1*B14[3][6] + b2*B14[4][6] + b3*B14[5][6] + b4*B14[6][6] + b5*B14[7][6] + b6*B14[8][6] + b7*B14[9][6];
			m7 = b_2*B14[0][7] + b_1*B14[1][7] + b0*B14[2][7] + b1*B14[3][7] + b2*B14[4][7] + b3*B14[5][7] + b4*B14[6][7] + b5*B14[7][7] + b6*B14[8][7] + b7*B14[9][7];
			m8 = b_2*B14[0][8] + b_1*B14[1][8] + b0*B14[2][8] + b1*B14[3][8] + b2*B14[4][8] + b3*B14[5][8] + b4*B14[6][8] + b5*B14[7][8] + b6*B14[8][8] + b7*B14[9][8];
			m9 = b_2*B14[0][9] + b_1*B14[1][9] + b0*B14[2][9] + b1*B14[3][9] + b2*B14[4][9] + b3*B14[5][9] + b4*B14[6][9] + b5*B14[7][9] + b6*B14[8][9] + b7*B14[9][9];
			m10 = b_2*B14[0][10] + b_1*B14[1][10] + b0*B14[2][10] + b1*B14[3][10] + b2*B14[4][10] + b3*B14[5][10] + b4*B14[6][10] + b5*B14[7][10] + b6*B14[8][10] + b7*B14[9][10];
			m11 = b_2*B14[0][11] + b_1*B14[1][11] + b0*B14[2][11] + b1*B14[3][11] + b2*B14[4][11] + b3*B14[5][11] + b4*B14[6][11] + b5*B14[7][11] + b6*B14[8][11] + b7*B14[9][11];
			m12 = b_2*B14[0][12] + b_1*B14[1][12] + b0*B14[2][12] + b1*B14[3][12] + b2*B14[4][12] + b3*B14[5][12] + b4*B14[6][12] + b5*B14[7][12] + b6*B14[8][12] + b7*B14[9][12];

			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10;
			fifth_yy[i][j] = xxxx;

			n0 = b_2*B13[0][0] + b_1*B13[1][0] + b0*B13[2][0] + b1*B13[3][0] + b2*B13[4][0] + b3*B13[5][0] + b4*B13[6][0] + b5*B13[7][0] + b6*B13[8][0] + b7*B13[9][0];
			n1 = b_2*B13[0][1] + b_1*B13[1][1] + b0*B13[2][1] + b1*B13[3][1] + b2*B13[4][1] + b3*B13[5][1] + b4*B13[6][1] + b5*B13[7][1] + b6*B13[8][1] + b7*B13[9][1];
			n2 = b_2*B13[0][2] + b_1*B13[1][2] + b0*B13[2][2] + b1*B13[3][2] + b2*B13[4][2] + b3*B13[5][2] + b4*B13[6][2] + b5*B13[7][2] + b6*B13[8][2] + b7*B13[9][2];
			n3 = b_2*B13[0][3] + b_1*B13[1][3] + b0*B13[2][3] + b1*B13[3][3] + b2*B13[4][3] + b3*B13[5][3] + b4*B13[6][3] + b5*B13[7][3] + b6*B13[8][3] + b7*B13[9][3];
			n4 = b_2*B13[0][4] + b_1*B13[1][4] + b0*B13[2][4] + b1*B13[3][4] + b2*B13[4][4] + b3*B13[5][4] + b4*B13[6][4] + b5*B13[7][4] + b6*B13[8][4] + b7*B13[9][4];
			n5 = b_2*B13[0][5] + b_1*B13[1][5] + b0*B13[2][5] + b1*B13[3][5] + b2*B13[4][5] + b3*B13[5][5] + b4*B13[6][5] + b5*B13[7][5] + b6*B13[8][5] + b7*B13[9][5];
			n6 = b_2*B13[0][6] + b_1*B13[1][6] + b0*B13[2][6] + b1*B13[3][6] + b2*B13[4][6] + b3*B13[5][6] + b4*B13[6][6] + b5*B13[7][6] + b6*B13[8][6] + b7*B13[9][6];
			n7 = b_2*B13[0][7] + b_1*B13[1][7] + b0*B13[2][7] + b1*B13[3][7] + b2*B13[4][7] + b3*B13[5][7] + b4*B13[6][7] + b5*B13[7][7] + b6*B13[8][7] + b7*B13[9][7];
			n8 = b_2*B13[0][8] + b_1*B13[1][8] + b0*B13[2][8] + b1*B13[3][8] + b2*B13[4][8] + b3*B13[5][8] + b4*B13[6][8] + b5*B13[7][8] + b6*B13[8][8] + b7*B13[9][8];
			n9 = b_2*B13[0][9] + b_1*B13[1][9] + b0*B13[2][9] + b1*B13[3][9] + b2*B13[4][9] + b3*B13[5][9] + b4*B13[6][9] + b5*B13[7][9] + b6*B13[8][9] + b7*B13[9][9];
			n10 = b_2*B13[0][10] + b_1*B13[1][10] + b0*B13[2][10] + b1*B13[3][10] + b2*B13[4][10] + b3*B13[5][10] + b4*B13[6][10] + b5*B13[7][10] + b6*B13[8][10] + b7*B13[9][10];
			n11 = b_2*B13[0][11] + b_1*B13[1][11] + b0*B13[2][11] + b1*B13[3][11] + b2*B13[4][11] + b3*B13[5][11] + b4*B13[6][11] + b5*B13[7][11] + b6*B13[8][11] + b7*B13[9][11];
			n12 = b_2*B13[0][12] + b_1*B13[1][12] + b0*B13[2][12] + b1*B13[3][12] + b2*B13[4][12] + b3*B13[5][12] + b4*B13[6][12] + b5*B13[7][12] + b6*B13[8][12] + b7*B13[9][12];

			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10;
			fifth_xx[i][j] = kkkk;
		}
	}
	//zuodatui
	for (int i = 2; i<8; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B10[i][j] = C10[i - 2][j];
			B11[i][j] = C11[i - 2][j];
			B12[i][j] = C12[i - 2][j];
		}
	}
	for (int i = 0; i<2; i++)
	{
		for (int j = 3; j<8; j++)
		{
			B10[i][j] = C4[i + 3][j + 6];
			B11[i][j] = C5[i + 3][j + 6];
			B12[i][j] = C6[i + 3][j + 6];

		}
	}
	for (int i = 0; i<2; i++)
	{
		for (int j = 0; j<2; j++)
		{
			B10[i][j] = C4[i + 3][j + 14];
			B11[i][j] = C5[i + 3][j + 14];
			B12[i][j] = C6[i + 3][j + 14];

		}
	}
	B10[0][2] = B10[1][2] = B10[2][2];
	B11[0][2] = C5[3][1];
	B11[1][2] = C5[4][1];
	B12[0][2] = B12[1][2] = B12[2][2];




	for (int i = 8; i<10; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B10[i][j] = C16[i - 7][j];
			B11[i][j] = C17[i - 7][j];
			B12[i][j] = C18[i - 7][j];
		}
	}

	for (int i = 0; i<10; i++)
	{
		for (int j = 8; j<13; j++)
		{
			B10[i][j] = B10[i][j - 8];
			B11[i][j] = B11[i][j - 8];
			B12[i][j] = B12[i][j - 8];

		}
	}
	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			fourth_x[i][j] = j * 8 / POINTNUMEACHPART;
			fourth_y[i][j] = i * 5 / POINTNUMEACHPART;
			if ((fourth_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((fourth_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] + 1);
			}

			if ((fourth_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((fourth_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] + 2);
			}

			if (fourth_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((fourth_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] + 0);
			}

			if ((fourth_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((fourth_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 1);
			}

			if ((fourth_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((fourth_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 2);
			}

			if ((fourth_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((fourth_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 3);
			}

			if ((fourth_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((fourth_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 4);
			}

			if ((fourth_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((fourth_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 5);
			}

			if ((fourth_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((fourth_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 6);
			}

			if ((fourth_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((fourth_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 7);
			}

			if ((fourth_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((fourth_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 8);
			}

			if ((fourth_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((fourth_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 9);
			}

			if ((fourth_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((fourth_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((fourth_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_x[i][j] - 10);
			}


			if ((fourth_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((fourth_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] + 1);
			}

			if ((fourth_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((fourth_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] + 2);
			}

			if ((fourth_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((fourth_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] + 0);
			}

			if ((fourth_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((fourth_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] - 1);
			}

			if ((fourth_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((fourth_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] - 2);
			}

			if ((fourth_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((fourth_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] - 3);
			}

			if ((fourth_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((fourth_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] - 4);
			}

			if ((fourth_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((fourth_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] - 5);
			}

			if ((fourth_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((fourth_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] - 6);
			}

			if ((fourth_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((fourth_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((fourth_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (fourth_y[i][j] - 7);
			}
			g0 = b_2*B12[0][0] + b_1*B12[1][0] + b0*B12[2][0] + b1*B12[3][0] + b2*B12[4][0] + b3*B12[5][0] + b4*B12[6][0] + b5*B12[7][0] + b6*B12[8][0] + b7*B12[9][0];
			g1 = b_2*B12[0][1] + b_1*B12[1][1] + b0*B12[2][1] + b1*B12[3][1] + b2*B12[4][1] + b3*B12[5][1] + b4*B12[6][1] + b5*B12[7][1] + b6*B12[8][1] + b7*B12[9][1];
			g2 = b_2*B12[0][2] + b_1*B12[1][2] + b0*B12[2][2] + b1*B12[3][2] + b2*B12[4][2] + b3*B12[5][2] + b4*B12[6][2] + b5*B12[7][2] + b6*B12[8][2] + b7*B12[9][2];
			g3 = b_2*B12[0][3] + b_1*B12[1][3] + b0*B12[2][3] + b1*B12[3][3] + b2*B12[4][3] + b3*B12[5][3] + b4*B12[6][3] + b5*B12[7][3] + b6*B12[8][3] + b7*B12[9][3];
			g4 = b_2*B12[0][4] + b_1*B12[1][4] + b0*B12[2][4] + b1*B12[3][4] + b2*B12[4][4] + b3*B12[5][4] + b4*B12[6][4] + b5*B12[7][4] + b6*B12[8][4] + b7*B12[9][4];
			g5 = b_2*B12[0][5] + b_1*B12[1][5] + b0*B12[2][5] + b1*B12[3][5] + b2*B12[4][5] + b3*B12[5][5] + b4*B12[6][5] + b5*B12[7][5] + b6*B12[8][5] + b7*B12[9][5];
			g6 = b_2*B12[0][6] + b_1*B12[1][6] + b0*B12[2][6] + b1*B12[3][6] + b2*B12[4][6] + b3*B12[5][6] + b4*B12[6][6] + b5*B12[7][6] + b6*B12[8][6] + b7*B12[9][6];
			g7 = b_2*B12[0][7] + b_1*B12[1][7] + b0*B12[2][7] + b1*B12[3][7] + b2*B12[4][7] + b3*B12[5][7] + b4*B12[6][7] + b5*B12[7][7] + b6*B12[8][7] + b7*B12[9][7];
			g8 = b_2*B12[0][8] + b_1*B12[1][8] + b0*B12[2][8] + b1*B12[3][8] + b2*B12[4][8] + b3*B12[5][8] + b4*B12[6][8] + b5*B12[7][8] + b6*B12[8][8] + b7*B12[9][8];
			g9 = b_2*B12[0][9] + b_1*B12[1][9] + b0*B12[2][9] + b1*B12[3][9] + b2*B12[4][9] + b3*B12[5][9] + b4*B12[6][9] + b5*B12[7][9] + b6*B12[8][9] + b7*B12[9][9];
			g10 = b_2*B12[0][10] + b_1*B12[1][10] + b0*B12[2][10] + b1*B12[3][10] + b2*B12[4][10] + b3*B12[5][10] + b4*B12[6][10] + b5*B12[7][10] + b6*B12[8][10] + b7*B12[9][10];
			g11 = b_2*B12[0][11] + b_1*B12[1][11] + b0*B12[2][11] + b1*B12[3][11] + b2*B12[4][11] + b3*B12[5][11] + b4*B12[6][11] + b5*B12[7][11] + b6*B12[8][11] + b7*B12[9][11];
			g12 = b_2*B12[0][12] + b_1*B12[1][12] + b0*B12[2][12] + b1*B12[3][12] + b2*B12[4][12] + b3*B12[5][12] + b4*B12[6][12] + b5*B12[7][12] + b6*B12[8][12] + b7*B12[9][12];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10;
			fourth_z[i][j] = z;

			m0 = b_2*B11[0][0] + b_1*B11[1][0] + b0*B11[2][0] + b1*B11[3][0] + b2*B11[4][0] + b3*B11[5][0] + b4*B11[6][0] + b5*B11[7][0] + b6*B11[8][0] + b7*B11[9][0];
			m1 = b_2*B11[0][1] + b_1*B11[1][1] + b0*B11[2][1] + b1*B11[3][1] + b2*B11[4][1] + b3*B11[5][1] + b4*B11[6][1] + b5*B11[7][1] + b6*B11[8][1] + b7*B11[9][1];
			m2 = b_2*B11[0][2] + b_1*B11[1][2] + b0*B11[2][2] + b1*B11[3][2] + b2*B11[4][2] + b3*B11[5][2] + b4*B11[6][2] + b5*B11[7][2] + b6*B11[8][2] + b7*B11[9][2];
			m3 = b_2*B11[0][3] + b_1*B11[1][3] + b0*B11[2][3] + b1*B11[3][3] + b2*B11[4][3] + b3*B11[5][3] + b4*B11[6][3] + b5*B11[7][3] + b6*B11[8][3] + b7*B11[9][3];
			m4 = b_2*B11[0][4] + b_1*B11[1][4] + b0*B11[2][4] + b1*B11[3][4] + b2*B11[4][4] + b3*B11[5][4] + b4*B11[6][4] + b5*B11[7][4] + b6*B11[8][4] + b7*B11[9][4];
			m5 = b_2*B11[0][5] + b_1*B11[1][5] + b0*B11[2][5] + b1*B11[3][5] + b2*B11[4][5] + b3*B11[5][5] + b4*B11[6][5] + b5*B11[7][5] + b6*B11[8][5] + b7*B11[9][5];
			m6 = b_2*B11[0][6] + b_1*B11[1][6] + b0*B11[2][6] + b1*B11[3][6] + b2*B11[4][6] + b3*B11[5][6] + b4*B11[6][6] + b5*B11[7][6] + b6*B11[8][6] + b7*B11[9][6];
			m7 = b_2*B11[0][7] + b_1*B11[1][7] + b0*B11[2][7] + b1*B11[3][7] + b2*B11[4][7] + b3*B11[5][7] + b4*B11[6][7] + b5*B11[7][7] + b6*B11[8][7] + b7*B11[9][7];
			m8 = b_2*B11[0][8] + b_1*B11[1][8] + b0*B11[2][8] + b1*B11[3][8] + b2*B11[4][8] + b3*B11[5][8] + b4*B11[6][8] + b5*B11[7][8] + b6*B11[8][8] + b7*B11[9][8];
			m9 = b_2*B11[0][9] + b_1*B11[1][9] + b0*B11[2][9] + b1*B11[3][9] + b2*B11[4][9] + b3*B11[5][9] + b4*B11[6][9] + b5*B11[7][9] + b6*B11[8][9] + b7*B11[9][9];
			m10 = b_2*B11[0][10] + b_1*B11[1][10] + b0*B11[2][10] + b1*B11[3][10] + b2*B11[4][10] + b3*B11[5][10] + b4*B11[6][10] + b5*B11[7][10] + b6*B11[8][10] + b7*B11[9][10];
			m11 = b_2*B11[0][11] + b_1*B11[1][11] + b0*B11[2][11] + b1*B11[3][11] + b2*B11[4][11] + b3*B11[5][11] + b4*B11[6][11] + b5*B11[7][11] + b6*B11[8][11] + b7*B11[9][11];
			m12 = b_2*B11[0][12] + b_1*B11[1][12] + b0*B11[2][12] + b1*B11[3][12] + b2*B11[4][12] + b3*B11[5][12] + b4*B11[6][12] + b5*B11[7][12] + b6*B11[8][12] + b7*B11[9][12];

			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10;
			fourth_yy[i][j] = xxxx;

			n0 = b_2*B10[0][0] + b_1*B10[1][0] + b0*B10[2][0] + b1*B10[3][0] + b2*B10[4][0] + b3*B10[5][0] + b4*B10[6][0] + b5*B10[7][0] + b6*B10[8][0] + b7*B10[9][0];
			n1 = b_2*B10[0][1] + b_1*B10[1][1] + b0*B10[2][1] + b1*B10[3][1] + b2*B10[4][1] + b3*B10[5][1] + b4*B10[6][1] + b5*B10[7][1] + b6*B10[8][1] + b7*B10[9][1];
			n2 = b_2*B10[0][2] + b_1*B10[1][2] + b0*B10[2][2] + b1*B10[3][2] + b2*B10[4][2] + b3*B10[5][2] + b4*B10[6][2] + b5*B10[7][2] + b6*B10[8][2] + b7*B10[9][2];
			n3 = b_2*B10[0][3] + b_1*B10[1][3] + b0*B10[2][3] + b1*B10[3][3] + b2*B10[4][3] + b3*B10[5][3] + b4*B10[6][3] + b5*B10[7][3] + b6*B10[8][3] + b7*B10[9][3];
			n4 = b_2*B10[0][4] + b_1*B10[1][4] + b0*B10[2][4] + b1*B10[3][4] + b2*B10[4][4] + b3*B10[5][4] + b4*B10[6][4] + b5*B10[7][4] + b6*B10[8][4] + b7*B10[9][4];
			n5 = b_2*B10[0][5] + b_1*B10[1][5] + b0*B10[2][5] + b1*B10[3][5] + b2*B10[4][5] + b3*B10[5][5] + b4*B10[6][5] + b5*B10[7][5] + b6*B10[8][5] + b7*B10[9][5];
			n6 = b_2*B10[0][6] + b_1*B10[1][6] + b0*B10[2][6] + b1*B10[3][6] + b2*B10[4][6] + b3*B10[5][6] + b4*B10[6][6] + b5*B10[7][6] + b6*B10[8][6] + b7*B10[9][6];
			n7 = b_2*B10[0][7] + b_1*B10[1][7] + b0*B10[2][7] + b1*B10[3][7] + b2*B10[4][7] + b3*B10[5][7] + b4*B10[6][7] + b5*B10[7][7] + b6*B10[8][7] + b7*B10[9][7];
			n8 = b_2*B10[0][8] + b_1*B10[1][8] + b0*B10[2][8] + b1*B10[3][8] + b2*B10[4][8] + b3*B10[5][8] + b4*B10[6][8] + b5*B10[7][8] + b6*B10[8][8] + b7*B10[9][8];
			n9 = b_2*B10[0][9] + b_1*B10[1][9] + b0*B10[2][9] + b1*B10[3][9] + b2*B10[4][9] + b3*B10[5][9] + b4*B10[6][9] + b5*B10[7][9] + b6*B10[8][9] + b7*B10[9][9];
			n10 = b_2*B10[0][10] + b_1*B10[1][10] + b0*B10[2][10] + b1*B10[3][10] + b2*B10[4][10] + b3*B10[5][10] + b4*B10[6][10] + b5*B10[7][10] + b6*B10[8][10] + b7*B10[9][10];
			n11 = b_2*B10[0][11] + b_1*B10[1][11] + b0*B10[2][11] + b1*B10[3][11] + b2*B10[4][11] + b3*B10[5][11] + b4*B10[6][11] + b5*B10[7][11] + b6*B10[8][11] + b7*B10[9][11];
			n12 = b_2*B10[0][12] + b_1*B10[1][12] + b0*B10[2][12] + b1*B10[3][12] + b2*B10[4][12] + b3*B10[5][12] + b4*B10[6][12] + b5*B10[7][12] + b6*B10[8][12] + b7*B10[9][12];

			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10;
			fourth_xx[i][j] = kkkk;
		}
	}

	//zuoxiaotui
	for (int i = 2; i<8; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B16[i][j] = C16[i - 2][j];
			B17[i][j] = C17[i - 2][j];
			B18[i][j] = C18[i - 2][j];

		}
	}
	for (int i = 0; i<2; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B16[i][j] = C10[i + 3][j];
			B17[i][j] = C11[i + 3][j];
			B18[i][j] = C12[i + 3][j];
		}
	}
	for (int j = 0; j<8; j++)
	{
		B17[8][j] = L_calf_point_2[1] - 1;
		B17[9][j] = L_calf_point_2[1] - 2;
	}
	for (int i = 8; i<10; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B16[i][j] = B16[7][j];
			B18[i][j] = B18[7][j];

		}
	}
	for (int i = 0; i<10; i++)
	{
		for (int j = 8; j<13; j++)
		{
			B16[i][j] = B16[i][j - 8];
			B17[i][j] = B17[i][j - 8];
			B18[i][j] = B18[i][j - 8];

		}
	}
	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			sixth_x[i][j] = j * 8 / POINTNUMEACHPART;
			sixth_y[i][j] = i * 5 / POINTNUMEACHPART;
			if ((sixth_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((sixth_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] + 1);
			}

			if ((sixth_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((sixth_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] + 2);
			}

			if (sixth_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((sixth_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] + 0);
			}

			if ((sixth_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((sixth_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 1);
			}

			if ((sixth_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((sixth_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 2);
			}

			if ((sixth_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((sixth_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 3);
			}

			if ((sixth_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((sixth_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 4);
			}

			if ((sixth_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((sixth_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 5);
			}

			if ((sixth_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((sixth_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 6);
			}

			if ((sixth_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((sixth_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 7);
			}

			if ((sixth_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((sixth_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 8);
			}

			if ((sixth_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((sixth_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 9);
			}

			if ((sixth_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((sixth_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((sixth_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_x[i][j] - 10);
			}


			if ((sixth_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((sixth_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] + 1);
			}

			if ((sixth_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((sixth_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] + 2);
			}

			if ((sixth_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((sixth_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] + 0);
			}

			if ((sixth_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((sixth_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] - 1);
			}

			if ((sixth_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((sixth_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] - 2);
			}

			if ((sixth_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((sixth_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] - 3);
			}

			if ((sixth_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((sixth_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] - 4);
			}

			if ((sixth_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((sixth_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] - 5);
			}

			if ((sixth_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((sixth_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] - 6);
			}

			if ((sixth_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((sixth_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((sixth_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (sixth_y[i][j] - 7);
			}
			g0 = b_2*B18[0][0] + b_1*B18[1][0] + b0*B18[2][0] + b1*B18[3][0] + b2*B18[4][0] + b3*B18[5][0] + b4*B18[6][0] + b5*B18[7][0] + b6*B18[8][0] + b7*B18[9][0];
			g1 = b_2*B18[0][1] + b_1*B18[1][1] + b0*B18[2][1] + b1*B18[3][1] + b2*B18[4][1] + b3*B18[5][1] + b4*B18[6][1] + b5*B18[7][1] + b6*B18[8][1] + b7*B18[9][1];
			g2 = b_2*B18[0][2] + b_1*B18[1][2] + b0*B18[2][2] + b1*B18[3][2] + b2*B18[4][2] + b3*B18[5][2] + b4*B18[6][2] + b5*B18[7][2] + b6*B18[8][2] + b7*B18[9][2];
			g3 = b_2*B18[0][3] + b_1*B18[1][3] + b0*B18[2][3] + b1*B18[3][3] + b2*B18[4][3] + b3*B18[5][3] + b4*B18[6][3] + b5*B18[7][3] + b6*B18[8][3] + b7*B18[9][3];
			g4 = b_2*B18[0][4] + b_1*B18[1][4] + b0*B18[2][4] + b1*B18[3][4] + b2*B18[4][4] + b3*B18[5][4] + b4*B18[6][4] + b5*B18[7][4] + b6*B18[8][4] + b7*B18[9][4];
			g5 = b_2*B18[0][5] + b_1*B18[1][5] + b0*B18[2][5] + b1*B18[3][5] + b2*B18[4][5] + b3*B18[5][5] + b4*B18[6][5] + b5*B18[7][5] + b6*B18[8][5] + b7*B18[9][5];
			g6 = b_2*B18[0][6] + b_1*B18[1][6] + b0*B18[2][6] + b1*B18[3][6] + b2*B18[4][6] + b3*B18[5][6] + b4*B18[6][6] + b5*B18[7][6] + b6*B18[8][6] + b7*B18[9][6];
			g7 = b_2*B18[0][7] + b_1*B18[1][7] + b0*B18[2][7] + b1*B18[3][7] + b2*B18[4][7] + b3*B18[5][7] + b4*B18[6][7] + b5*B18[7][7] + b6*B18[8][7] + b7*B18[9][7];
			g8 = b_2*B18[0][8] + b_1*B18[1][8] + b0*B18[2][8] + b1*B18[3][8] + b2*B18[4][8] + b3*B18[5][8] + b4*B18[6][8] + b5*B18[7][8] + b6*B18[8][8] + b7*B18[9][8];
			g9 = b_2*B18[0][9] + b_1*B18[1][9] + b0*B18[2][9] + b1*B18[3][9] + b2*B18[4][9] + b3*B18[5][9] + b4*B18[6][9] + b5*B18[7][9] + b6*B18[8][9] + b7*B18[9][9];
			g10 = b_2*B18[0][10] + b_1*B18[1][10] + b0*B18[2][10] + b1*B18[3][10] + b2*B18[4][10] + b3*B18[5][10] + b4*B18[6][10] + b5*B18[7][10] + b6*B18[8][10] + b7*B18[9][10];
			g11 = b_2*B18[0][11] + b_1*B18[1][11] + b0*B18[2][11] + b1*B18[3][11] + b2*B18[4][11] + b3*B18[5][11] + b4*B18[6][11] + b5*B18[7][11] + b6*B18[8][11] + b7*B18[9][11];
			g12 = b_2*B18[0][12] + b_1*B18[1][12] + b0*B18[2][12] + b1*B18[3][12] + b2*B18[4][12] + b3*B18[5][12] + b4*B18[6][12] + b5*B18[7][12] + b6*B18[8][12] + b7*B18[9][12];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10;
			sixth_z[i][j] = z;

			m0 = b_2*B17[0][0] + b_1*B17[1][0] + b0*B17[2][0] + b1*B17[3][0] + b2*B17[4][0] + b3*B17[5][0] + b4*B17[6][0] + b5*B17[7][0] + b6*B17[8][0] + b7*B17[9][0];
			m1 = b_2*B17[0][1] + b_1*B17[1][1] + b0*B17[2][1] + b1*B17[3][1] + b2*B17[4][1] + b3*B17[5][1] + b4*B17[6][1] + b5*B17[7][1] + b6*B17[8][1] + b7*B17[9][1];
			m2 = b_2*B17[0][2] + b_1*B17[1][2] + b0*B17[2][2] + b1*B17[3][2] + b2*B17[4][2] + b3*B17[5][2] + b4*B17[6][2] + b5*B17[7][2] + b6*B17[8][2] + b7*B17[9][2];
			m3 = b_2*B17[0][3] + b_1*B17[1][3] + b0*B17[2][3] + b1*B17[3][3] + b2*B17[4][3] + b3*B17[5][3] + b4*B17[6][3] + b5*B17[7][3] + b6*B17[8][3] + b7*B17[9][3];
			m4 = b_2*B17[0][4] + b_1*B17[1][4] + b0*B17[2][4] + b1*B17[3][4] + b2*B17[4][4] + b3*B17[5][4] + b4*B17[6][4] + b5*B17[7][4] + b6*B17[8][4] + b7*B17[9][4];
			m5 = b_2*B17[0][5] + b_1*B17[1][5] + b0*B17[2][5] + b1*B17[3][5] + b2*B17[4][5] + b3*B17[5][5] + b4*B17[6][5] + b5*B17[7][5] + b6*B17[8][5] + b7*B17[9][5];
			m6 = b_2*B17[0][6] + b_1*B17[1][6] + b0*B17[2][6] + b1*B17[3][6] + b2*B17[4][6] + b3*B17[5][6] + b4*B17[6][6] + b5*B17[7][6] + b6*B17[8][6] + b7*B17[9][6];
			m7 = b_2*B17[0][7] + b_1*B17[1][7] + b0*B17[2][7] + b1*B17[3][7] + b2*B17[4][7] + b3*B17[5][7] + b4*B17[6][7] + b5*B17[7][7] + b6*B17[8][7] + b7*B17[9][7];
			m8 = b_2*B17[0][8] + b_1*B17[1][8] + b0*B17[2][8] + b1*B17[3][8] + b2*B17[4][8] + b3*B17[5][8] + b4*B17[6][8] + b5*B17[7][8] + b6*B17[8][8] + b7*B17[9][8];
			m9 = b_2*B17[0][9] + b_1*B17[1][9] + b0*B17[2][9] + b1*B17[3][9] + b2*B17[4][9] + b3*B17[5][9] + b4*B17[6][9] + b5*B17[7][9] + b6*B17[8][9] + b7*B17[9][9];
			m10 = b_2*B17[0][10] + b_1*B17[1][10] + b0*B17[2][10] + b1*B17[3][10] + b2*B17[4][10] + b3*B17[5][10] + b4*B17[6][10] + b5*B17[7][10] + b6*B17[8][10] + b7*B17[9][10];
			m11 = b_2*B17[0][11] + b_1*B17[1][11] + b0*B17[2][11] + b1*B17[3][11] + b2*B17[4][11] + b3*B17[5][11] + b4*B17[6][11] + b5*B17[7][11] + b6*B17[8][11] + b7*B17[9][11];
			m12 = b_2*B17[0][12] + b_1*B17[1][12] + b0*B17[2][12] + b1*B17[3][12] + b2*B17[4][12] + b3*B17[5][12] + b4*B17[6][12] + b5*B17[7][12] + b6*B17[8][12] + b7*B17[9][12];

			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10;
			sixth_yy[i][j] = xxxx;

			n0 = b_2*B16[0][0] + b_1*B16[1][0] + b0*B16[2][0] + b1*B16[3][0] + b2*B16[4][0] + b3*B16[5][0] + b4*B16[6][0] + b5*B16[7][0] + b6*B16[8][0] + b7*B16[9][0];
			n1 = b_2*B16[0][1] + b_1*B16[1][1] + b0*B16[2][1] + b1*B16[3][1] + b2*B16[4][1] + b3*B16[5][1] + b4*B16[6][1] + b5*B16[7][1] + b6*B16[8][1] + b7*B16[9][1];
			n2 = b_2*B16[0][2] + b_1*B16[1][2] + b0*B16[2][2] + b1*B16[3][2] + b2*B16[4][2] + b3*B16[5][2] + b4*B16[6][2] + b5*B16[7][2] + b6*B16[8][2] + b7*B16[9][2];
			n3 = b_2*B16[0][3] + b_1*B16[1][3] + b0*B16[2][3] + b1*B16[3][3] + b2*B16[4][3] + b3*B16[5][3] + b4*B16[6][3] + b5*B16[7][3] + b6*B16[8][3] + b7*B16[9][3];
			n4 = b_2*B16[0][4] + b_1*B16[1][4] + b0*B16[2][4] + b1*B16[3][4] + b2*B16[4][4] + b3*B16[5][4] + b4*B16[6][4] + b5*B16[7][4] + b6*B16[8][4] + b7*B16[9][4];
			n5 = b_2*B16[0][5] + b_1*B16[1][5] + b0*B16[2][5] + b1*B16[3][5] + b2*B16[4][5] + b3*B16[5][5] + b4*B16[6][5] + b5*B16[7][5] + b6*B16[8][5] + b7*B16[9][5];
			n6 = b_2*B16[0][6] + b_1*B16[1][6] + b0*B16[2][6] + b1*B16[3][6] + b2*B16[4][6] + b3*B16[5][6] + b4*B16[6][6] + b5*B16[7][6] + b6*B16[8][6] + b7*B16[9][6];
			n7 = b_2*B16[0][7] + b_1*B16[1][7] + b0*B16[2][7] + b1*B16[3][7] + b2*B16[4][7] + b3*B16[5][7] + b4*B16[6][7] + b5*B16[7][7] + b6*B16[8][7] + b7*B16[9][7];
			n8 = b_2*B16[0][8] + b_1*B16[1][8] + b0*B16[2][8] + b1*B16[3][8] + b2*B16[4][8] + b3*B16[5][8] + b4*B16[6][8] + b5*B16[7][8] + b6*B16[8][8] + b7*B16[9][8];
			n9 = b_2*B16[0][9] + b_1*B16[1][9] + b0*B16[2][9] + b1*B16[3][9] + b2*B16[4][9] + b3*B16[5][9] + b4*B16[6][9] + b5*B16[7][9] + b6*B16[8][9] + b7*B16[9][9];
			n10 = b_2*B16[0][10] + b_1*B16[1][10] + b0*B16[2][10] + b1*B16[3][10] + b2*B16[4][10] + b3*B16[5][10] + b4*B16[6][10] + b5*B16[7][10] + b6*B16[8][10] + b7*B16[9][10];
			n11 = b_2*B16[0][11] + b_1*B16[1][11] + b0*B16[2][11] + b1*B16[3][11] + b2*B16[4][11] + b3*B16[5][11] + b4*B16[6][11] + b5*B16[7][11] + b6*B16[8][11] + b7*B16[9][11];
			n12 = b_2*B16[0][12] + b_1*B16[1][12] + b0*B16[2][12] + b1*B16[3][12] + b2*B16[4][12] + b3*B16[5][12] + b4*B16[6][12] + b5*B16[7][12] + b6*B16[8][12] + b7*B16[9][12];

			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10;
			sixth_xx[i][j] = kkkk;
		}
	}
	//shenti
	for (int i = 2; i<10; i++)
	{
		for (int j = 0; j<16; j++)
		{
			B1[i][j] = C1[i - 2][j];
			B2[i][j] = C2[i - 2][j];
			B3[i][j] = C3[i - 2][j];

		}
	}
	for (int i = 10; i<13; i++)
	{
		for (int j = 0; j<16; j++)
		{
			B1[i][j] = C4[i - 10][j];
			B2[i][j] = C5[i - 10][j];
			B3[i][j] = C6[i - 10][j];

		}
	}
	for (int i = 0; i<16; i++)
	{
		B1[0][i] = B1[1][i] = B1[2][i];
		B3[0][i] = B3[1][i] = B3[2][i];
		B2[0][i] = B2[2][1] + 2;
		B2[1][i] = B2[2][1] + 1;


	}
	for (int i = 0; i<13; i++)
	{
		for (int j = 16; j<21; j++)
		{
			B1[i][j] = B1[i][j - 16];
			B2[i][j] = B2[i][j - 16];
			B3[i][j] = B3[i][j - 16];

		}
	}
	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			first_x[i][j] = j * 16 / POINTNUMEACHPART;
			first_y[i][j] = i * 8 / POINTNUMEACHPART;
			if ((first_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((first_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] + 1);
			}

			if ((first_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((first_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] + 2);
			}

			if (first_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((first_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] + 0);
			}

			if ((first_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((first_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 1);
			}

			if ((first_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((first_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 2);
			}

			if ((first_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((first_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 3);
			}

			if ((first_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((first_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 4);
			}

			if ((first_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((first_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 5);
			}

			if ((first_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((first_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 6);
			}

			if ((first_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((first_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 7);
			}

			if ((first_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((first_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 8);
			}

			if ((first_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((first_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 9);
			}

			if ((first_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((first_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 10);
			}

			if ((first_x[i][j] - 11) == 0)
			{
				a11 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 11), 2.0f) / (-3);
				a11 = sin((first_x[i][j] - 11)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 11);
			}

			if ((first_x[i][j] - 12) == 0)
			{
				a12 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 12), 2.0f) / (-3);
				a12 = sin((first_x[i][j] - 12)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 12);
			}
			if ((first_x[i][j] - 13) == 0)
			{
				a13 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 13), 2.0f) / (-3);
				a13 = sin((first_x[i][j] - 13)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 13);
			}
			if ((first_x[i][j] - 14) == 0)
			{
				a14 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 14), 2.0f) / (-3);
				a14 = sin((first_x[i][j] - 14)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 14);
			}
			if ((first_x[i][j] - 15) == 0)
			{
				a15 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 15), 2.0f) / (-3);
				a15 = sin((first_x[i][j] - 15)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 15);
			}
			if ((first_x[i][j] - 16) == 0)
			{
				a16 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 16), 2.0f) / (-3);
				a16 = sin((first_x[i][j] - 16)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 16);
			}
			if ((first_x[i][j] - 17) == 0)
			{
				a17 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 17), 2.0f) / (-3);
				a17 = sin((first_x[i][j] - 17)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 17);
			}
			if ((first_x[i][j] - 18) == 0)
			{
				a18 = 1;
			}
			else
			{
				float temp = pow((first_x[i][j] - 18), 2.0f) / (-3);
				a18 = sin((first_x[i][j] - 18)*GL_PI)*pow(xe, temp) / GL_PI / (first_x[i][j] - 18);
			}
			if ((first_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((first_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] + 1);
			}

			if ((first_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((first_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] + 2);
			}

			if ((first_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((first_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] + 0);
			}

			if ((first_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((first_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 1);
			}

			if ((first_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((first_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 2);
			}

			if ((first_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((first_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 3);
			}

			if ((first_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((first_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 4);
			}

			if ((first_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((first_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 5);
			}

			if ((first_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((first_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 6);
			}

			if ((first_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((first_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 7);
			}

			if ((first_y[i][j] - 8) == 0)
			{
				b8 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 8), 2.0f) / (-3);
				b8 = sin((first_y[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 8);
			}
			if ((first_y[i][j] - 9) == 0)
			{
				b9 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 9), 2.0f) / (-3);
				b9 = sin((first_y[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 9);
			}
			if ((first_y[i][j] - 10) == 0)
			{
				b10 = 1;
			}
			else
			{
				float temp = pow((first_y[i][j] - 10), 2.0f) / (-3);
				b10 = sin((first_y[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (first_y[i][j] - 10);
			}
			g0 = b_2*B3[0][0] + b_1*B3[1][0] + b0*B3[2][0] + b1*B3[3][0] + b2*B3[4][0] + b3*B3[5][0] + b4*B3[6][0] + b5*B3[7][0] + b6*B3[8][0] + b7*B3[9][0] + b8*B3[10][0] + b9*B3[11][0] + b10*B3[12][0];
			g1 = b_2*B3[0][1] + b_1*B3[1][1] + b0*B3[2][1] + b1*B3[3][1] + b2*B3[4][1] + b3*B3[5][1] + b4*B3[6][1] + b5*B3[7][1] + b6*B3[8][1] + b7*B3[9][1] + b8*B3[10][1] + b9*B3[11][1] + b10*B3[12][1];
			g2 = b_2*B3[0][2] + b_1*B3[1][2] + b0*B3[2][2] + b1*B3[3][2] + b2*B3[4][2] + b3*B3[5][2] + b4*B3[6][2] + b5*B3[7][2] + b6*B3[8][2] + b7*B3[9][2] + b8*B3[10][2] + b9*B3[11][2] + b10*B3[12][2];
			g3 = b_2*B3[0][3] + b_1*B3[1][3] + b0*B3[2][3] + b1*B3[3][3] + b2*B3[4][3] + b3*B3[5][3] + b4*B3[6][3] + b5*B3[7][3] + b6*B3[8][3] + b7*B3[9][3] + b8*B3[10][3] + b9*B3[11][3] + b10*B3[12][3];
			g4 = b_2*B3[0][4] + b_1*B3[1][4] + b0*B3[2][4] + b1*B3[3][4] + b2*B3[4][4] + b3*B3[5][4] + b4*B3[6][4] + b5*B3[7][4] + b6*B3[8][4] + b7*B3[9][4] + b8*B3[10][4] + b9*B3[11][4] + b10*B3[12][4];
			g5 = b_2*B3[0][5] + b_1*B3[1][5] + b0*B3[2][5] + b1*B3[3][5] + b2*B3[4][5] + b3*B3[5][5] + b4*B3[6][5] + b5*B3[7][5] + b6*B3[8][5] + b7*B3[9][5] + b8*B3[10][5] + b9*B3[11][5] + b10*B3[12][5];
			g6 = b_2*B3[0][6] + b_1*B3[1][6] + b0*B3[2][6] + b1*B3[3][6] + b2*B3[4][6] + b3*B3[5][6] + b4*B3[6][6] + b5*B3[7][6] + b6*B3[8][6] + b7*B3[9][6] + b8*B3[10][6] + b9*B3[11][6] + b10*B3[12][6];
			g7 = b_2*B3[0][7] + b_1*B3[1][7] + b0*B3[2][7] + b1*B3[3][7] + b2*B3[4][7] + b3*B3[5][7] + b4*B3[6][7] + b5*B3[7][7] + b6*B3[8][7] + b7*B3[9][7] + b8*B3[10][7] + b9*B3[11][7] + b10*B3[12][7];
			g8 = b_2*B3[0][8] + b_1*B3[1][8] + b0*B3[2][8] + b1*B3[3][8] + b2*B3[4][8] + b3*B3[5][8] + b4*B3[6][8] + b5*B3[7][8] + b6*B3[8][8] + b7*B3[9][8] + b8*B3[10][8] + b9*B3[11][8] + b10*B3[12][8];
			g9 = b_2*B3[0][9] + b_1*B3[1][9] + b0*B3[2][9] + b1*B3[3][9] + b2*B3[4][9] + b3*B3[5][9] + b4*B3[6][9] + b5*B3[7][9] + b6*B3[8][9] + b7*B3[9][9] + b8*B3[10][9] + b9*B3[11][9] + b10*B3[12][9];
			g10 = b_2*B3[0][10] + b_1*B3[1][10] + b0*B3[2][10] + b1*B3[3][10] + b2*B3[4][10] + b3*B3[5][10] + b4*B3[6][10] + b5*B3[7][10] + b6*B3[8][10] + b7*B3[9][10] + b8*B3[10][10] + b9*B3[11][10] + b10*B3[12][10];
			g11 = b_2*B3[0][11] + b_1*B3[1][11] + b0*B3[2][11] + b1*B3[3][11] + b2*B3[4][11] + b3*B3[5][11] + b4*B3[6][11] + b5*B3[7][11] + b6*B3[8][11] + b7*B3[9][11] + b8*B3[10][11] + b9*B3[11][11] + b10*B3[12][11];
			g12 = b_2*B3[0][12] + b_1*B3[1][12] + b0*B3[2][12] + b1*B3[3][12] + b2*B3[4][12] + b3*B3[5][12] + b4*B3[6][12] + b5*B3[7][12] + b6*B3[8][12] + b7*B3[9][12] + b8*B3[10][12] + b9*B3[11][12] + b10*B3[12][12];
			g13 = b_2*B3[0][13] + b_1*B3[1][13] + b0*B3[2][13] + b1*B3[3][13] + b2*B3[4][13] + b3*B3[5][13] + b4*B3[6][13] + b5*B3[7][13] + b6*B3[8][13] + b7*B3[9][13] + b8*B3[10][13] + b9*B3[11][13] + b10*B3[12][13];
			g14 = b_2*B3[0][14] + b_1*B3[1][14] + b0*B3[2][14] + b1*B3[3][14] + b2*B3[4][14] + b3*B3[5][14] + b4*B3[6][14] + b5*B3[7][14] + b6*B3[8][14] + b7*B3[9][14] + b8*B3[10][14] + b9*B3[11][14] + b10*B3[12][14];
			g15 = b_2*B3[0][15] + b_1*B3[1][15] + b0*B3[2][15] + b1*B3[3][15] + b2*B3[4][15] + b3*B3[5][15] + b4*B3[6][15] + b5*B3[7][15] + b6*B3[8][15] + b7*B3[9][15] + b8*B3[10][15] + b9*B3[11][15] + b10*B3[12][15];
			g16 = b_2*B3[0][16] + b_1*B3[1][16] + b0*B3[2][16] + b1*B3[3][16] + b2*B3[4][16] + b3*B3[5][16] + b4*B3[6][16] + b5*B3[7][16] + b6*B3[8][16] + b7*B3[9][16] + b8*B3[10][16] + b9*B3[11][16] + b10*B3[12][16];
			g17 = b_2*B3[0][17] + b_1*B3[1][17] + b0*B3[2][17] + b1*B3[3][17] + b2*B3[4][17] + b3*B3[5][17] + b4*B3[6][17] + b5*B3[7][17] + b6*B3[8][17] + b7*B3[9][17] + b8*B3[10][17] + b9*B3[11][17] + b10*B3[12][17];
			g18 = b_2*B3[0][18] + b_1*B3[1][18] + b0*B3[2][18] + b1*B3[3][18] + b2*B3[4][18] + b3*B3[5][18] + b4*B3[6][18] + b5*B3[7][18] + b6*B3[8][18] + b7*B3[9][18] + b8*B3[10][18] + b9*B3[11][18] + b10*B3[12][18];
			g19 = b_2*B3[0][19] + b_1*B3[1][19] + b0*B3[2][19] + b1*B3[3][19] + b2*B3[4][19] + b3*B3[5][19] + b4*B3[6][19] + b5*B3[7][19] + b6*B3[8][19] + b7*B3[9][19] + b8*B3[10][19] + b9*B3[11][19] + b10*B3[12][19];
			g20 = b_2*B3[0][20] + b_1*B3[1][20] + b0*B3[2][20] + b1*B3[3][20] + b2*B3[4][20] + b3*B3[5][20] + b4*B3[6][20] + b5*B3[7][20] + b6*B3[8][20] + b7*B3[9][20] + b8*B3[10][20] + b9*B3[11][20] + b10*B3[12][20];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10 + g13*a11 + g14*a12 + g15*a13 + g16*a14 + g17*a15 + g18*a16 + g19*a17 + g20*a18;
			first_z[i][j] = z;

			m0 = b_2*B2[0][0] + b_1*B2[1][0] + b0*B2[2][0] + b1*B2[3][0] + b2*B2[4][0] + b3*B2[5][0] + b4*B2[6][0] + b5*B2[7][0] + b6*B2[8][0] + b7*B2[9][0] + b8*B2[10][0] + b9*B2[11][0] + b10*B2[12][0];
			m1 = b_2*B2[0][1] + b_1*B2[1][1] + b0*B2[2][1] + b1*B2[3][1] + b2*B2[4][1] + b3*B2[5][1] + b4*B2[6][1] + b5*B2[7][1] + b6*B2[8][1] + b7*B2[9][1] + b8*B2[10][1] + b9*B2[11][1] + b10*B2[12][1];
			m2 = b_2*B2[0][2] + b_1*B2[1][2] + b0*B2[2][2] + b1*B2[3][2] + b2*B2[4][2] + b3*B2[5][2] + b4*B2[6][2] + b5*B2[7][2] + b6*B2[8][2] + b7*B2[9][2] + b8*B2[10][2] + b9*B2[11][2] + b10*B2[12][2];
			m3 = b_2*B2[0][3] + b_1*B2[1][3] + b0*B2[2][3] + b1*B2[3][3] + b2*B2[4][3] + b3*B2[5][3] + b4*B2[6][3] + b5*B2[7][3] + b6*B2[8][3] + b7*B2[9][3] + b8*B2[10][3] + b9*B2[11][3] + b10*B2[12][3];
			m4 = b_2*B2[0][4] + b_1*B2[1][4] + b0*B2[2][4] + b1*B2[3][4] + b2*B2[4][4] + b3*B2[5][4] + b4*B2[6][4] + b5*B2[7][4] + b6*B2[8][4] + b7*B2[9][4] + b8*B2[10][4] + b9*B2[11][4] + b10*B2[12][4];
			m5 = b_2*B2[0][5] + b_1*B2[1][5] + b0*B2[2][5] + b1*B2[3][5] + b2*B2[4][5] + b3*B2[5][5] + b4*B2[6][5] + b5*B2[7][5] + b6*B2[8][5] + b7*B2[9][5] + b8*B2[10][5] + b9*B2[11][5] + b10*B2[12][5];
			m6 = b_2*B2[0][6] + b_1*B2[1][6] + b0*B2[2][6] + b1*B2[3][6] + b2*B2[4][6] + b3*B2[5][6] + b4*B2[6][6] + b5*B2[7][6] + b6*B2[8][6] + b7*B2[9][6] + b8*B2[10][6] + b9*B2[11][6] + b10*B2[12][6];
			m7 = b_2*B2[0][7] + b_1*B2[1][7] + b0*B2[2][7] + b1*B2[3][7] + b2*B2[4][7] + b3*B2[5][7] + b4*B2[6][7] + b5*B2[7][7] + b6*B2[8][7] + b7*B2[9][7] + b8*B2[10][7] + b9*B2[11][7] + b10*B2[12][7];
			m8 = b_2*B2[0][8] + b_1*B2[1][8] + b0*B2[2][8] + b1*B2[3][8] + b2*B2[4][8] + b3*B2[5][8] + b4*B2[6][8] + b5*B2[7][8] + b6*B2[8][8] + b7*B2[9][8] + b8*B2[10][8] + b9*B2[11][8] + b10*B2[12][8];
			m9 = b_2*B2[0][9] + b_1*B2[1][9] + b0*B2[2][9] + b1*B2[3][9] + b2*B2[4][9] + b3*B2[5][9] + b4*B2[6][9] + b5*B2[7][9] + b6*B2[8][9] + b7*B2[9][9] + b8*B2[10][9] + b9*B2[11][9] + b10*B2[12][9];
			m10 = b_2*B2[0][10] + b_1*B2[1][10] + b0*B2[2][10] + b1*B2[3][10] + b2*B2[4][10] + b3*B2[5][10] + b4*B2[6][10] + b5*B2[7][10] + b6*B2[8][10] + b7*B2[9][10] + b8*B2[10][10] + b9*B2[11][10] + b10*B2[12][10];
			m11 = b_2*B2[0][11] + b_1*B2[1][11] + b0*B2[2][11] + b1*B2[3][11] + b2*B2[4][11] + b3*B2[5][11] + b4*B2[6][11] + b5*B2[7][11] + b6*B2[8][11] + b7*B2[9][11] + b8*B2[10][11] + b9*B2[11][11] + b10*B2[12][11];
			m12 = b_2*B2[0][12] + b_1*B2[1][12] + b0*B2[2][12] + b1*B2[3][12] + b2*B2[4][12] + b3*B2[5][12] + b4*B2[6][12] + b5*B2[7][12] + b6*B2[8][12] + b7*B2[9][12] + b8*B2[10][12] + b9*B2[11][12] + b10*B2[12][12];
			m13 = b_2*B2[0][13] + b_1*B2[1][13] + b0*B2[2][13] + b1*B2[3][13] + b2*B2[4][13] + b3*B2[5][13] + b4*B2[6][13] + b5*B2[7][13] + b6*B2[8][13] + b7*B2[9][13] + b8*B2[10][13] + b9*B2[11][13] + b10*B2[12][13];
			m14 = b_2*B2[0][14] + b_1*B2[1][14] + b0*B2[2][14] + b1*B2[3][14] + b2*B2[4][14] + b3*B2[5][14] + b4*B2[6][14] + b5*B2[7][14] + b6*B2[8][14] + b7*B2[9][14] + b8*B2[10][14] + b9*B2[11][14] + b10*B2[12][14];
			m15 = b_2*B2[0][15] + b_1*B2[1][15] + b0*B2[2][15] + b1*B2[3][15] + b2*B2[4][15] + b3*B2[5][15] + b4*B2[6][15] + b5*B2[7][15] + b6*B2[8][15] + b7*B2[9][15] + b8*B2[10][15] + b9*B2[11][15] + b10*B2[12][15];
			m16 = b_2*B2[0][16] + b_1*B2[1][16] + b0*B2[2][16] + b1*B2[3][16] + b2*B2[4][16] + b3*B2[5][16] + b4*B2[6][16] + b5*B2[7][16] + b6*B2[8][16] + b7*B2[9][16] + b8*B2[10][16] + b9*B2[11][16] + b10*B2[12][16];
			m17 = b_2*B2[0][17] + b_1*B2[1][17] + b0*B2[2][17] + b1*B2[3][17] + b2*B2[4][17] + b3*B2[5][17] + b4*B2[6][17] + b5*B2[7][17] + b6*B2[8][17] + b7*B2[9][17] + b8*B2[10][17] + b9*B2[11][17] + b10*B2[12][17];
			m18 = b_2*B2[0][18] + b_1*B2[1][18] + b0*B2[2][18] + b1*B2[3][18] + b2*B2[4][18] + b3*B2[5][18] + b4*B2[6][18] + b5*B2[7][18] + b6*B2[8][18] + b7*B2[9][18] + b8*B2[10][18] + b9*B2[11][18] + b10*B2[12][18];
			m19 = b_2*B2[0][19] + b_1*B2[1][19] + b0*B2[2][19] + b1*B2[3][19] + b2*B2[4][19] + b3*B2[5][19] + b4*B2[6][19] + b5*B2[7][19] + b6*B2[8][19] + b7*B2[9][19] + b8*B2[10][19] + b9*B2[11][19] + b10*B2[12][19];
			m20 = b_2*B2[0][20] + b_1*B2[1][20] + b0*B2[2][20] + b1*B2[3][20] + b2*B2[4][20] + b3*B2[5][20] + b4*B2[6][20] + b5*B2[7][20] + b6*B2[8][20] + b7*B2[9][20] + b8*B2[10][20] + b9*B2[11][20] + b10*B2[12][20];
			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10 + m13*a11 + m14*a12 + m15*a13 + m16*a14 + m17*a15 + m18*a16 + m19*a17 + m20*a18;
			first_yy[i][j] = xxxx;

			n0 = b_2*B1[0][0] + b_1*B1[1][0] + b0*B1[2][0] + b1*B1[3][0] + b2*B1[4][0] + b3*B1[5][0] + b4*B1[6][0] + b5*B1[7][0] + b6*B1[8][0] + b7*B1[9][0] + b8*B1[10][0] + b9*B1[11][0] + b10*B1[12][0];
			n1 = b_2*B1[0][1] + b_1*B1[1][1] + b0*B1[2][1] + b1*B1[3][1] + b2*B1[4][1] + b3*B1[5][1] + b4*B1[6][1] + b5*B1[7][1] + b6*B1[8][1] + b7*B1[9][1] + b8*B1[10][1] + b9*B1[11][1] + b10*B1[12][1];
			n2 = b_2*B1[0][2] + b_1*B1[1][2] + b0*B1[2][2] + b1*B1[3][2] + b2*B1[4][2] + b3*B1[5][2] + b4*B1[6][2] + b5*B1[7][2] + b6*B1[8][2] + b7*B1[9][2] + b8*B1[10][2] + b9*B1[11][2] + b10*B1[12][2];
			n3 = b_2*B1[0][3] + b_1*B1[1][3] + b0*B1[2][3] + b1*B1[3][3] + b2*B1[4][3] + b3*B1[5][3] + b4*B1[6][3] + b5*B1[7][3] + b6*B1[8][3] + b7*B1[9][3] + b8*B1[10][3] + b9*B1[11][3] + b10*B1[12][3];
			n4 = b_2*B1[0][4] + b_1*B1[1][4] + b0*B1[2][4] + b1*B1[3][4] + b2*B1[4][4] + b3*B1[5][4] + b4*B1[6][4] + b5*B1[7][4] + b6*B1[8][4] + b7*B1[9][4] + b8*B1[10][4] + b9*B1[11][4] + b10*B1[12][4];
			n5 = b_2*B1[0][5] + b_1*B1[1][5] + b0*B1[2][5] + b1*B1[3][5] + b2*B1[4][5] + b3*B1[5][5] + b4*B1[6][5] + b5*B1[7][5] + b6*B1[8][5] + b7*B1[9][5] + b8*B1[10][5] + b9*B1[11][5] + b10*B1[12][5];
			n6 = b_2*B1[0][6] + b_1*B1[1][6] + b0*B1[2][6] + b1*B1[3][6] + b2*B1[4][6] + b3*B1[5][6] + b4*B1[6][6] + b5*B1[7][6] + b6*B1[8][6] + b7*B1[9][6] + b8*B1[10][6] + b9*B1[11][6] + b10*B1[12][6];
			n7 = b_2*B1[0][7] + b_1*B1[1][7] + b0*B1[2][7] + b1*B1[3][7] + b2*B1[4][7] + b3*B1[5][7] + b4*B1[6][7] + b5*B1[7][7] + b6*B1[8][7] + b7*B1[9][7] + b8*B1[10][7] + b9*B1[11][7] + b10*B1[12][7];
			n8 = b_2*B1[0][8] + b_1*B1[1][8] + b0*B1[2][8] + b1*B1[3][8] + b2*B1[4][8] + b3*B1[5][8] + b4*B1[6][8] + b5*B1[7][8] + b6*B1[8][8] + b7*B1[9][8] + b8*B1[10][8] + b9*B1[11][8] + b10*B1[12][8];
			n9 = b_2*B1[0][9] + b_1*B1[1][9] + b0*B1[2][9] + b1*B1[3][9] + b2*B1[4][9] + b3*B1[5][9] + b4*B1[6][9] + b5*B1[7][9] + b6*B1[8][9] + b7*B1[9][9] + b8*B1[10][9] + b9*B1[11][9] + b10*B1[12][9];
			n10 = b_2*B1[0][10] + b_1*B1[1][10] + b0*B1[2][10] + b1*B1[3][10] + b2*B1[4][10] + b3*B1[5][10] + b4*B1[6][10] + b5*B1[7][10] + b6*B1[8][10] + b7*B1[9][10] + b8*B1[10][10] + b9*B1[11][10] + b10*B1[12][10];
			n11 = b_2*B1[0][11] + b_1*B1[1][11] + b0*B1[2][11] + b1*B1[3][11] + b2*B1[4][11] + b3*B1[5][11] + b4*B1[6][11] + b5*B1[7][11] + b6*B1[8][11] + b7*B1[9][11] + b8*B1[10][11] + b9*B1[11][11] + b10*B1[12][11];
			n12 = b_2*B1[0][12] + b_1*B1[1][12] + b0*B1[2][12] + b1*B1[3][12] + b2*B1[4][12] + b3*B1[5][12] + b4*B1[6][12] + b5*B1[7][12] + b6*B1[8][12] + b7*B1[9][12] + b8*B1[10][12] + b9*B1[11][12] + b10*B1[12][12];
			n13 = b_2*B1[0][13] + b_1*B1[1][13] + b0*B1[2][13] + b1*B1[3][13] + b2*B1[4][13] + b3*B1[5][13] + b4*B1[6][13] + b5*B1[7][13] + b6*B1[8][13] + b7*B1[9][13] + b8*B1[10][13] + b9*B1[11][13] + b10*B1[12][13];
			n14 = b_2*B1[0][14] + b_1*B1[1][14] + b0*B1[2][14] + b1*B1[3][14] + b2*B1[4][14] + b3*B1[5][14] + b4*B1[6][14] + b5*B1[7][14] + b6*B1[8][14] + b7*B1[9][14] + b8*B1[10][14] + b9*B1[11][14] + b10*B1[12][14];
			n15 = b_2*B1[0][15] + b_1*B1[1][15] + b0*B1[2][15] + b1*B1[3][15] + b2*B1[4][15] + b3*B1[5][15] + b4*B1[6][15] + b5*B1[7][15] + b6*B1[8][15] + b7*B1[9][15] + b8*B1[10][15] + b9*B1[11][15] + b10*B1[12][15];
			n16 = b_2*B1[0][16] + b_1*B1[1][16] + b0*B1[2][16] + b1*B1[3][16] + b2*B1[4][16] + b3*B1[5][16] + b4*B1[6][16] + b5*B1[7][16] + b6*B1[8][16] + b7*B1[9][16] + b8*B1[10][16] + b9*B1[11][16] + b10*B1[12][16];
			n17 = b_2*B1[0][17] + b_1*B1[1][17] + b0*B1[2][17] + b1*B1[3][17] + b2*B1[4][17] + b3*B1[5][17] + b4*B1[6][17] + b5*B1[7][17] + b6*B1[8][17] + b7*B1[9][17] + b8*B1[10][17] + b9*B1[11][17] + b10*B1[12][17];
			n18 = b_2*B1[0][18] + b_1*B1[1][18] + b0*B1[2][18] + b1*B1[3][18] + b2*B1[4][18] + b3*B1[5][18] + b4*B1[6][18] + b5*B1[7][18] + b6*B1[8][18] + b7*B1[9][18] + b8*B1[10][18] + b9*B1[11][18] + b10*B1[12][18];
			n19 = b_2*B1[0][19] + b_1*B1[1][19] + b0*B1[2][19] + b1*B1[3][19] + b2*B1[4][19] + b3*B1[5][19] + b4*B1[6][19] + b5*B1[7][19] + b6*B1[8][19] + b7*B1[9][19] + b8*B1[10][19] + b9*B1[11][19] + b10*B1[12][19];
			n20 = b_2*B1[0][20] + b_1*B1[1][20] + b0*B1[2][20] + b1*B1[3][20] + b2*B1[4][20] + b3*B1[5][20] + b4*B1[6][20] + b5*B1[7][20] + b6*B1[8][20] + b7*B1[9][20] + b8*B1[10][20] + b9*B1[11][20] + b10*B1[12][20];
			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10 + n13*a11 + n14*a12 + n15*a13 + n16*a14 + n17*a15 + n18*a16 + n19*a17 + n20*a18;
			first_xx[i][j] = kkkk;

		}
	}

	//
	upperarm_temp_point[3][0] = Elbow_point[0];
	upperarm_temp_point[3][1] = Elbow_point[1];
	upperarm_temp_point[3][2] = Elbow_point[2];

	upperarm_temp_point[0][0] = 15.430;
	upperarm_temp_point[0][1] = 55.826;
	upperarm_temp_point[0][2] = -0.327;

	upperarm_temp_point[1][0] = (upperarm_temp_point[0][0] - upperarm_temp_point[3][0]) * 2 / 3 + upperarm_temp_point[3][0];
	upperarm_temp_point[1][1] = (upperarm_temp_point[0][1] - upperarm_temp_point[3][1]) * 2 / 3 + upperarm_temp_point[3][1];
	upperarm_temp_point[1][2] = (upperarm_temp_point[0][2] - upperarm_temp_point[3][2]) * 2 / 3 + upperarm_temp_point[3][2];

	upperarm_temp_point[2][0] = (upperarm_temp_point[0][0] - upperarm_temp_point[3][0]) * 1 / 3 + upperarm_temp_point[3][0];
	upperarm_temp_point[2][1] = (upperarm_temp_point[0][1] - upperarm_temp_point[3][1]) * 1 / 3 + upperarm_temp_point[3][1];
	upperarm_temp_point[2][2] = (upperarm_temp_point[0][2] - upperarm_temp_point[3][2]) * 1 / 3 + upperarm_temp_point[3][2];


	upperarm_temp_point[6][0] = Elbow_point_2[0];
	upperarm_temp_point[6][1] = Elbow_point_2[1];
	upperarm_temp_point[6][2] = Elbow_point_2[2];

	upperarm_temp_point[4][0] = (upperarm_temp_point[3][0] - upperarm_temp_point[6][0]) * 2 / 3 + upperarm_temp_point[6][0];
	upperarm_temp_point[4][1] = (upperarm_temp_point[3][1] - upperarm_temp_point[6][1]) * 2 / 3 + upperarm_temp_point[6][1];
	upperarm_temp_point[4][2] = (upperarm_temp_point[3][2] - upperarm_temp_point[6][2]) * 2 / 3 + upperarm_temp_point[6][2];
	upperarm_temp_point[5][0] = (upperarm_temp_point[3][0] - upperarm_temp_point[6][0]) * 1 / 3 + upperarm_temp_point[6][0];
	upperarm_temp_point[5][1] = (upperarm_temp_point[3][1] - upperarm_temp_point[6][1]) * 1 / 3 + upperarm_temp_point[6][1];
	upperarm_temp_point[5][2] = (upperarm_temp_point[3][2] - upperarm_temp_point[6][2]) * 1 / 3 + upperarm_temp_point[6][2];
	upperarm_temp_point[7][0] = (upperarm_temp_point[3][0] - upperarm_temp_point[6][0])*(-0.1) + upperarm_temp_point[6][0];
	upperarm_temp_point[7][1] = (upperarm_temp_point[3][1] - upperarm_temp_point[6][1])*(-0.1) + upperarm_temp_point[6][1];
	upperarm_temp_point[7][2] = (upperarm_temp_point[3][2] - upperarm_temp_point[6][2])*(-0.1) + upperarm_temp_point[6][2];
	upperarm_temp_point[8][0] = (upperarm_temp_point[3][0] - upperarm_temp_point[6][0])*(-0.2) + upperarm_temp_point[6][0];
	upperarm_temp_point[8][1] = (upperarm_temp_point[3][1] - upperarm_temp_point[6][1])*(-0.2) + upperarm_temp_point[6][1];
	upperarm_temp_point[8][2] = (upperarm_temp_point[3][2] - upperarm_temp_point[6][2])*(-0.2) + upperarm_temp_point[6][2];

	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[2][i][0] = upperarm_length*cos(ang);
		control_Elbow[2][i][1] = 0;
		control_Elbow[2][i][2] = -upperarm_length*sin(ang);
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[0][i][0] = upperarm_length*cos(ang)*1.15;
		control_Elbow[0][i][1] = 0;
		control_Elbow[0][i][2] = -upperarm_length*sin(ang)*1.15;
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[1][i][0] = upperarm_length*cos(ang)*1.08;
		control_Elbow[1][i][1] = 0;
		control_Elbow[1][i][2] = -upperarm_length*sin(ang)*1.08;
	}

	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[5][i][0] = upperarm1_length*cos(ang);
		control_Elbow[5][i][1] = 0;
		control_Elbow[5][i][2] = -upperarm1_length*sin(ang);
	}



	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[3][i][0] = ((upperarm_length - upperarm1_length)*0.67 + upperarm1_length)*cos(ang);
		control_Elbow[3][i][1] = 0;
		control_Elbow[3][i][2] = -((upperarm_length - upperarm1_length)*0.67 + upperarm1_length)*sin(ang);
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[4][i][0] = ((upperarm_length - upperarm1_length)*0.33 + upperarm1_length)*cos(ang);
		control_Elbow[4][i][1] = 0;
		control_Elbow[4][i][2] = -((upperarm_length - upperarm1_length)*0.33 + upperarm1_length)*sin(ang);
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[6][i][0] = upperarm1_length*cos(ang)*0.95;
		control_Elbow[6][i][1] = 0;
		control_Elbow[6][i][2] = -upperarm1_length*sin(ang)*0.95;
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[7][i][0] = upperarm1_length*cos(ang)*0.9;
		control_Elbow[7][i][1] = 0;
		control_Elbow[7][i][2] = -upperarm1_length*sin(ang)*0.9;
	}



	CVector3 shoubi1(0.0f, 1.0f, 0.0f);
	CVector3 shoubi2((upperarm_temp_point[0][0] - upperarm_temp_point[3][0]), (upperarm_temp_point[0][1] - upperarm_temp_point[3][1]), (upperarm_temp_point[0][2] - upperarm_temp_point[3][2]));
	CVector3 faxiangliang1 = Cross(shoubi1, shoubi2);
	double jiajiao = acos((shoubi1.x*shoubi2.x + shoubi1.y*shoubi2.y + shoubi1.z*shoubi2.z) / (pow((pow(shoubi1.x, 2.0) + pow(shoubi1.y, 2.0) + pow(shoubi1.z, 2.0)), 0.5)*pow((pow(shoubi2.x, 2.0) + pow(shoubi2.y, 2.0) + pow(shoubi2.z, 2.0)), 0.5)));

	float ggg = upperarm_temp_point[0][1] - upperarm_temp_point[3][1];
	float gggg = upperarm_temp_point[3][0] - upperarm_temp_point[0][0];
	double ang_temp1 = (upperarm_temp_point[3][0] - upperarm_temp_point[0][0]) / (upperarm_temp_point[0][1] - upperarm_temp_point[3][1]);
	double ang_temp2 = (upperarm_temp_point[3][2] - upperarm_temp_point[0][2]) / pow((pow(ggg, 2.0) + pow(gggg, 2.0)), 0.5);



	//		m3dRotationMatrix44(rotationMatrix1,atan(ang_temp1),0.0f,0.0f,1.0f);
	m3dRotationMatrix44(rotationMatrix1, jiajiao, faxiangliang1.x, faxiangliang1.y, faxiangliang1.z);
	m3dRotationMatrix44(rotationMatrix2, atan(ang_temp2), 0.0f, 0.0f, 1.0f);
	m3dTranslateMatrix44(translationMatrix, upperarm_temp_point[3][0], upperarm_temp_point[3][1], upperarm_temp_point[3][2]);

	rotationMatrix1[12] = upperarm_temp_point[3][0];
	rotationMatrix1[13] = upperarm_temp_point[3][1];
	rotationMatrix1[14] = upperarm_temp_point[3][2];





	B20[2][0] = 55.303;
	B20[2][1] = 58.623;
	B20[2][2] = 59.925;
	B20[2][3] = 58.624;
	B20[2][4] = 55.302;
	B20[2][5] = 51.751;
	B20[2][6] = 49.835;
	B20[2][7] = 51.755;
	B20[2][8] = 55.303;
	B20[2][9] = 58.623;
	B20[2][10] = 59.925;
	B20[2][11] = 58.624;
	B20[2][12] = 55.302;
	for (int i = 0; i<13; i++)
	{
		B20[0][i] = B20[1][i] = B20[2][i];
	}

	B19[2][0] = 13.936;
	B19[2][1] = 14.375;
	B19[2][2] = 13.417;
	B19[2][3] = 15.535;
	B19[2][4] = 14.282;
	B19[2][5] = 13.234;
	B19[2][6] = 12.980;
	B19[2][7] = 13.410;
	B19[2][8] = 13.936;
	B19[2][9] = 14.375;
	B19[2][10] = 13.417;
	B19[2][11] = 15.535;
	B19[2][12] = 14.282;
	for (int i = 0; i<13; i++)
	{
		B19[0][i] = B19[2][i] - 4;
		B19[1][i] = B19[2][i] - 2;

	}
	B21[2][0] = 2.772;
	B21[2][1] = 1.100;
	B21[2][2] = -0.034;
	B21[2][3] = -0.311;
	B21[2][4] = -3.657;
	B21[2][5] = -3.262;
	B21[2][6] = -0.715;
	B21[2][7] = 1.757;
	B21[2][8] = 2.772;
	B21[2][9] = 1.100;
	B21[2][10] = -0.034;
	B21[2][11] = -0.311;
	B21[2][12] = -3.657;
	for (int i = 0; i<13; i++)
	{
		B21[0][i] = B21[1][i] = B19[2][i];
	}





	rotationMatrix1[12] = upperarm_temp_point[2][0];
	rotationMatrix1[13] = upperarm_temp_point[2][1];
	rotationMatrix1[14] = upperarm_temp_point[2][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[1][i], control_Elbow[1][i], rotationMatrix1);

	}
	for (int i = 0; i<8; i++)
	{
		B20[4][i] = control1_Elbow[1][i][1];
		B19[4][i] = control1_Elbow[1][i][0];
		B21[4][i] = control1_Elbow[1][i][2];
		C19[2][i] = B19[4][i];
		C20[2][i] = B20[4][i];
		C21[2][i] = B21[4][i];

	}
	for (int i = 8; i<13; i++)
	{
		B20[4][i] = B20[4][i - 8];
		B19[4][i] = B19[4][i - 8];
		B21[4][i] = B21[4][i - 8];

	}
	rotationMatrix1[12] = upperarm_temp_point[1][0];
	rotationMatrix1[13] = upperarm_temp_point[1][1];
	rotationMatrix1[14] = upperarm_temp_point[1][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[0][i], control_Elbow[0][i], rotationMatrix1);

	}
	for (int i = 0; i<8; i++)
	{
		B20[3][i] = control1_Elbow[0][i][1];
		B19[3][i] = control1_Elbow[0][i][0];
		B21[3][i] = control1_Elbow[0][i][2];
		C19[1][i] = B19[3][i];
		C20[1][i] = B20[3][i];
		C21[1][i] = B21[3][i];
	}
	for (int i = 8; i<13; i++)
	{
		B20[3][i] = B20[3][i - 8];
		B19[3][i] = B19[3][i - 8];
		B21[3][i] = B21[3][i - 8];

	}
	//guanjiedian
	shoubi2.x = upperarm_temp_point[0][0] - upperarm_temp_point[6][0];
	shoubi2.y = upperarm_temp_point[0][1] - upperarm_temp_point[6][1];
	shoubi2.z = upperarm_temp_point[0][2] - upperarm_temp_point[6][2];

	//		CVector3 shoubi2((upperarm_temp_point[0][0]-upperarm_temp_point[3][0]),(upperarm_temp_point[0][1]-upperarm_temp_point[3][1]),(upperarm_temp_point[0][2]-upperarm_temp_point[3][2]));
	CVector3 faxiangliang2 = Cross(shoubi1, shoubi2);
	jiajiao = acos((shoubi1.x*shoubi2.x + shoubi1.y*shoubi2.y + shoubi1.z*shoubi2.z) / (pow((pow(shoubi1.x, 2.0) + pow(shoubi1.y, 2.0) + pow(shoubi1.z, 2.0)), 0.5)*pow((pow(shoubi2.x, 2.0) + pow(shoubi2.y, 2.0) + pow(shoubi2.z, 2.0)), 0.5)));
	m3dRotationMatrix44(rotationMatrix2, jiajiao, faxiangliang2.x, faxiangliang2.y, faxiangliang2.z);

	rotationMatrix2[12] = upperarm_temp_point[3][0];
	rotationMatrix2[13] = upperarm_temp_point[3][1];
	rotationMatrix2[14] = upperarm_temp_point[3][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[2][i], control_Elbow[2][i], rotationMatrix2);

	}
	for (int i = 0; i<8; i++)
	{
		B20[5][i] = control1_Elbow[2][i][1];
		B19[5][i] = control1_Elbow[2][i][0];
		B21[5][i] = control1_Elbow[2][i][2];
		C19[3][i] = B19[5][i];
		C20[3][i] = B20[5][i];
		C21[3][i] = B21[5][i];
	}
	for (int i = 8; i<13; i++)
	{
		B20[5][i] = B20[5][i - 8];
		B19[5][i] = B19[5][i - 8];
		B21[5][i] = B21[5][i - 8];

	}

	shoubi2.x = upperarm_temp_point[3][0] - upperarm_temp_point[6][0];
	shoubi2.y = upperarm_temp_point[3][1] - upperarm_temp_point[6][1];
	shoubi2.z = upperarm_temp_point[3][2] - upperarm_temp_point[6][2];

	//		CVector3 shoubi2((upperarm_temp_point[0][0]-upperarm_temp_point[3][0]),(upperarm_temp_point[0][1]-upperarm_temp_point[3][1]),(upperarm_temp_point[0][2]-upperarm_temp_point[3][2]));
	CVector3 faxiangliang3 = Cross(shoubi1, shoubi2);
	jiajiao = acos((shoubi1.x*shoubi2.x + shoubi1.y*shoubi2.y + shoubi1.z*shoubi2.z) / (pow((pow(shoubi1.x, 2.0) + pow(shoubi1.y, 2.0) + pow(shoubi1.z, 2.0)), 0.5)*pow((pow(shoubi2.x, 2.0) + pow(shoubi2.y, 2.0) + pow(shoubi2.z, 2.0)), 0.5)));
	m3dRotationMatrix44(rotationMatrix3, jiajiao, faxiangliang3.x, faxiangliang3.y, faxiangliang3.z);

	rotationMatrix3[12] = upperarm_temp_point[4][0];
	rotationMatrix3[13] = upperarm_temp_point[4][1];
	rotationMatrix3[14] = upperarm_temp_point[4][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[3][i], control_Elbow[3][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B20[6][i] = control1_Elbow[3][i][1];
		B19[6][i] = control1_Elbow[3][i][0];
		B21[6][i] = control1_Elbow[3][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B20[6][i] = B20[6][i - 8];
		B19[6][i] = B19[6][i - 8];
		B21[6][i] = B21[6][i - 8];

	}
	rotationMatrix3[12] = upperarm_temp_point[5][0];
	rotationMatrix3[13] = upperarm_temp_point[5][1];
	rotationMatrix3[14] = upperarm_temp_point[5][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[4][i], control_Elbow[4][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B20[7][i] = control1_Elbow[4][i][1];
		B19[7][i] = control1_Elbow[4][i][0];
		B21[7][i] = control1_Elbow[4][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B20[7][i] = B20[7][i - 8];
		B19[7][i] = B19[7][i - 8];
		B21[7][i] = B21[7][i - 8];

	}

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<13; j++)
		{
			B22[i][j] = B19[i + 3][j];
			B23[i][j] = B20[i + 3][j];
			B24[i][j] = B21[i + 3][j];

		}
	}

	rotationMatrix3[12] = upperarm_temp_point[6][0];
	rotationMatrix3[13] = upperarm_temp_point[6][1];
	rotationMatrix3[14] = upperarm_temp_point[6][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[5][i], control_Elbow[5][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B23[5][i] = control1_Elbow[5][i][1];
		B22[5][i] = control1_Elbow[5][i][0];
		B24[5][i] = control1_Elbow[5][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B23[5][i] = B23[5][i - 8];
		B22[5][i] = B22[5][i - 8];
		B24[5][i] = B24[5][i - 8];

	}
	rotationMatrix3[12] = upperarm_temp_point[7][0];
	rotationMatrix3[13] = upperarm_temp_point[7][1];
	rotationMatrix3[14] = upperarm_temp_point[7][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[6][i], control_Elbow[6][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B23[6][i] = control1_Elbow[6][i][1];
		B22[6][i] = control1_Elbow[6][i][0];
		B24[6][i] = control1_Elbow[6][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B23[6][i] = B23[6][i - 8];
		B22[6][i] = B22[6][i - 8];
		B24[6][i] = B24[6][i - 8];

	}
	rotationMatrix3[12] = upperarm_temp_point[8][0];
	rotationMatrix3[13] = upperarm_temp_point[8][1];
	rotationMatrix3[14] = upperarm_temp_point[8][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[7][i], control_Elbow[7][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B23[7][i] = control1_Elbow[7][i][1];
		B22[7][i] = control1_Elbow[7][i][0];
		B24[7][i] = control1_Elbow[7][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B23[7][i] = B23[7][i - 8];
		B22[7][i] = B22[7][i - 8];
		B24[7][i] = B24[7][i - 8];

	}
	for (int i = 3; i<6; i++)
	{
		for (int j = 0; j<8; j++)
		{
			C22[i - 2][j] = B22[i][j];
			C23[i - 2][j] = B23[i][j];
			C24[i - 2][j] = B24[i][j];

		}
	}

	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			seventh_x[i][j] = j * 8 / POINTNUMEACHPART;
			seventh_y[i][j] = i * 3 / POINTNUMEACHPART;
			if ((seventh_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((seventh_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] + 1);
			}

			if ((seventh_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((seventh_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] + 2);
			}

			if (seventh_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((seventh_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] + 0);
			}

			if ((seventh_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((seventh_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 1);
			}

			if ((seventh_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((seventh_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 2);
			}

			if ((seventh_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((seventh_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 3);
			}

			if ((seventh_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((seventh_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 4);
			}

			if ((seventh_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((seventh_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 5);
			}

			if ((seventh_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((seventh_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 6);
			}

			if ((seventh_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((seventh_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 7);
			}

			if ((seventh_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((seventh_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 8);
			}

			if ((seventh_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((seventh_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 9);
			}

			if ((seventh_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((seventh_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((seventh_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_x[i][j] - 10);
			}


			if ((seventh_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((seventh_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] + 1);
			}

			if ((seventh_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((seventh_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] + 2);
			}

			if ((seventh_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((seventh_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] + 0);
			}

			if ((seventh_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((seventh_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] - 1);
			}

			if ((seventh_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((seventh_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] - 2);
			}

			if ((seventh_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((seventh_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] - 3);
			}

			if ((seventh_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((seventh_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] - 4);
			}

			if ((seventh_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((seventh_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] - 5);
			}

			if ((seventh_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((seventh_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] - 6);
			}

			if ((seventh_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((seventh_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((seventh_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (seventh_y[i][j] - 7);
			}
			g0 = b_2*B21[0][0] + b_1*B21[1][0] + b0*B21[2][0] + b1*B21[3][0] + b2*B21[4][0] + b3*B21[5][0] + b4*B21[6][0] + b5*B21[7][0];
			g1 = b_2*B21[0][1] + b_1*B21[1][1] + b0*B21[2][1] + b1*B21[3][1] + b2*B21[4][1] + b3*B21[5][1] + b4*B21[6][1] + b5*B21[7][1];
			g2 = b_2*B21[0][2] + b_1*B21[1][2] + b0*B21[2][2] + b1*B21[3][2] + b2*B21[4][2] + b3*B21[5][2] + b4*B21[6][2] + b5*B21[7][2];
			g3 = b_2*B21[0][3] + b_1*B21[1][3] + b0*B21[2][3] + b1*B21[3][3] + b2*B21[4][3] + b3*B21[5][3] + b4*B21[6][3] + b5*B21[7][3];
			g4 = b_2*B21[0][4] + b_1*B21[1][4] + b0*B21[2][4] + b1*B21[3][4] + b2*B21[4][4] + b3*B21[5][4] + b4*B21[6][4] + b5*B21[7][4];
			g5 = b_2*B21[0][5] + b_1*B21[1][5] + b0*B21[2][5] + b1*B21[3][5] + b2*B21[4][5] + b3*B21[5][5] + b4*B21[6][5] + b5*B21[7][5];
			g6 = b_2*B21[0][6] + b_1*B21[1][6] + b0*B21[2][6] + b1*B21[3][6] + b2*B21[4][6] + b3*B21[5][6] + b4*B21[6][6] + b5*B21[7][6];
			g7 = b_2*B21[0][7] + b_1*B21[1][7] + b0*B21[2][7] + b1*B21[3][7] + b2*B21[4][7] + b3*B21[5][7] + b4*B21[6][7] + b5*B21[7][7];
			g8 = b_2*B21[0][8] + b_1*B21[1][8] + b0*B21[2][8] + b1*B21[3][8] + b2*B21[4][8] + b3*B21[5][8] + b4*B21[6][8] + b5*B21[7][8];
			g9 = b_2*B21[0][9] + b_1*B21[1][9] + b0*B21[2][9] + b1*B21[3][9] + b2*B21[4][9] + b3*B21[5][9] + b4*B21[6][9] + b5*B21[7][9];
			g10 = b_2*B21[0][10] + b_1*B21[1][10] + b0*B21[2][10] + b1*B21[3][10] + b2*B21[4][10] + b3*B21[5][10] + b4*B21[6][10] + b5*B21[7][10];
			g11 = b_2*B21[0][11] + b_1*B21[1][11] + b0*B21[2][11] + b1*B21[3][11] + b2*B21[4][11] + b3*B21[5][11] + b4*B21[6][11] + b5*B21[7][11];
			g12 = b_2*B21[0][12] + b_1*B21[1][12] + b0*B21[2][12] + b1*B21[3][12] + b2*B21[4][12] + b3*B21[5][12] + b4*B21[6][12] + b5*B21[7][12];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10;
			seventh_z[i][j] = z;

			m0 = b_2*B20[0][0] + b_1*B20[1][0] + b0*B20[2][0] + b1*B20[3][0] + b2*B20[4][0] + b3*B20[5][0] + b4*B20[6][0] + b5*B20[7][0];
			m1 = b_2*B20[0][1] + b_1*B20[1][1] + b0*B20[2][1] + b1*B20[3][1] + b2*B20[4][1] + b3*B20[5][1] + b4*B20[6][1] + b5*B20[7][1];
			m2 = b_2*B20[0][2] + b_1*B20[1][2] + b0*B20[2][2] + b1*B20[3][2] + b2*B20[4][2] + b3*B20[5][2] + b4*B20[6][2] + b5*B20[7][2];
			m3 = b_2*B20[0][3] + b_1*B20[1][3] + b0*B20[2][3] + b1*B20[3][3] + b2*B20[4][3] + b3*B20[5][3] + b4*B20[6][3] + b5*B20[7][3];
			m4 = b_2*B20[0][4] + b_1*B20[1][4] + b0*B20[2][4] + b1*B20[3][4] + b2*B20[4][4] + b3*B20[5][4] + b4*B20[6][4] + b5*B20[7][4];
			m5 = b_2*B20[0][5] + b_1*B20[1][5] + b0*B20[2][5] + b1*B20[3][5] + b2*B20[4][5] + b3*B20[5][5] + b4*B20[6][5] + b5*B20[7][5];
			m6 = b_2*B20[0][6] + b_1*B20[1][6] + b0*B20[2][6] + b1*B20[3][6] + b2*B20[4][6] + b3*B20[5][6] + b4*B20[6][6] + b5*B20[7][6];
			m7 = b_2*B20[0][7] + b_1*B20[1][7] + b0*B20[2][7] + b1*B20[3][7] + b2*B20[4][7] + b3*B20[5][7] + b4*B20[6][7] + b5*B20[7][7];
			m8 = b_2*B20[0][8] + b_1*B20[1][8] + b0*B20[2][8] + b1*B20[3][8] + b2*B20[4][8] + b3*B20[5][8] + b4*B20[6][8] + b5*B20[7][8];
			m9 = b_2*B20[0][9] + b_1*B20[1][9] + b0*B20[2][9] + b1*B20[3][9] + b2*B20[4][9] + b3*B20[5][9] + b4*B20[6][9] + b5*B20[7][9];
			m10 = b_2*B20[0][10] + b_1*B20[1][10] + b0*B20[2][10] + b1*B20[3][10] + b2*B20[4][10] + b3*B20[5][10] + b4*B20[6][10] + b5*B20[7][10];
			m11 = b_2*B20[0][11] + b_1*B20[1][11] + b0*B20[2][11] + b1*B20[3][11] + b2*B20[4][11] + b3*B20[5][11] + b4*B20[6][11] + b5*B20[7][11];
			m12 = b_2*B20[0][12] + b_1*B20[1][12] + b0*B20[2][12] + b1*B20[3][12] + b2*B20[4][12] + b3*B20[5][12] + b4*B20[6][12] + b5*B20[7][12];

			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10;
			seventh_yy[i][j] = xxxx;

			n0 = b_2*B19[0][0] + b_1*B19[1][0] + b0*B19[2][0] + b1*B19[3][0] + b2*B19[4][0] + b3*B19[5][0] + b4*B19[6][0] + b5*B19[7][0];
			n1 = b_2*B19[0][1] + b_1*B19[1][1] + b0*B19[2][1] + b1*B19[3][1] + b2*B19[4][1] + b3*B19[5][1] + b4*B19[6][1] + b5*B19[7][1];
			n2 = b_2*B19[0][2] + b_1*B19[1][2] + b0*B19[2][2] + b1*B19[3][2] + b2*B19[4][2] + b3*B19[5][2] + b4*B19[6][2] + b5*B19[7][2];
			n3 = b_2*B19[0][3] + b_1*B19[1][3] + b0*B19[2][3] + b1*B19[3][3] + b2*B19[4][3] + b3*B19[5][3] + b4*B19[6][3] + b5*B19[7][3];
			n4 = b_2*B19[0][4] + b_1*B19[1][4] + b0*B19[2][4] + b1*B19[3][4] + b2*B19[4][4] + b3*B19[5][4] + b4*B19[6][4] + b5*B19[7][4];
			n5 = b_2*B19[0][5] + b_1*B19[1][5] + b0*B19[2][5] + b1*B19[3][5] + b2*B19[4][5] + b3*B19[5][5] + b4*B19[6][5] + b5*B19[7][5];
			n6 = b_2*B19[0][6] + b_1*B19[1][6] + b0*B19[2][6] + b1*B19[3][6] + b2*B19[4][6] + b3*B19[5][6] + b4*B19[6][6] + b5*B19[7][6];
			n7 = b_2*B19[0][7] + b_1*B19[1][7] + b0*B19[2][7] + b1*B19[3][7] + b2*B19[4][7] + b3*B19[5][7] + b4*B19[6][7] + b5*B19[7][7];
			n8 = b_2*B19[0][8] + b_1*B19[1][8] + b0*B19[2][8] + b1*B19[3][8] + b2*B19[4][8] + b3*B19[5][8] + b4*B19[6][8] + b5*B19[7][8];
			n9 = b_2*B19[0][9] + b_1*B19[1][9] + b0*B19[2][9] + b1*B19[3][9] + b2*B19[4][9] + b3*B19[5][9] + b4*B19[6][9] + b5*B19[7][9];
			n10 = b_2*B19[0][10] + b_1*B19[1][10] + b0*B19[2][10] + b1*B19[3][10] + b2*B19[4][10] + b3*B19[5][10] + b4*B19[6][10] + b5*B19[7][10];
			n11 = b_2*B19[0][11] + b_1*B19[1][11] + b0*B19[2][11] + b1*B19[3][11] + b2*B19[4][11] + b3*B19[5][11] + b4*B19[6][11] + b5*B19[7][11];
			n12 = b_2*B19[0][12] + b_1*B19[1][12] + b0*B19[2][12] + b1*B19[3][12] + b2*B19[4][12] + b3*B19[5][12] + b4*B19[6][12] + b5*B19[7][12];

			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10;
			seventh_xx[i][j] = kkkk;
		}
	}
	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			eighth_x[i][j] = j * 8 / POINTNUMEACHPART;
			eighth_y[i][j] = i * 3 / POINTNUMEACHPART;
			if ((eighth_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((eighth_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] + 1);
			}

			if ((eighth_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((eighth_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] + 2);
			}

			if (eighth_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((eighth_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] + 0);
			}

			if ((eighth_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((eighth_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 1);
			}

			if ((eighth_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((eighth_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 2);
			}

			if ((eighth_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((eighth_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 3);
			}

			if ((eighth_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((eighth_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 4);
			}

			if ((eighth_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((eighth_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 5);
			}

			if ((eighth_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((eighth_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 6);
			}

			if ((eighth_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((eighth_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 7);
			}

			if ((eighth_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((eighth_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 8);
			}

			if ((eighth_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((eighth_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 9);
			}

			if ((eighth_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((eighth_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((eighth_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_x[i][j] - 10);
			}


			if ((eighth_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((eighth_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] + 1);
			}

			if ((eighth_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((eighth_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] + 2);
			}

			if ((eighth_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((eighth_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] + 0);
			}

			if ((eighth_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((eighth_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] - 1);
			}

			if ((eighth_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((eighth_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] - 2);
			}

			if ((eighth_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((eighth_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] - 3);
			}

			if ((eighth_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((eighth_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] - 4);
			}

			if ((eighth_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((eighth_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] - 5);
			}

			if ((eighth_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((eighth_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] - 6);
			}

			if ((eighth_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((eighth_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((eighth_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (eighth_y[i][j] - 7);
			}
			g0 = b_2*B24[0][0] + b_1*B24[1][0] + b0*B24[2][0] + b1*B24[3][0] + b2*B24[4][0] + b3*B24[5][0] + b4*B24[6][0] + b5*B24[7][0];
			g1 = b_2*B24[0][1] + b_1*B24[1][1] + b0*B24[2][1] + b1*B24[3][1] + b2*B24[4][1] + b3*B24[5][1] + b4*B24[6][1] + b5*B24[7][1];
			g2 = b_2*B24[0][2] + b_1*B24[1][2] + b0*B24[2][2] + b1*B24[3][2] + b2*B24[4][2] + b3*B24[5][2] + b4*B24[6][2] + b5*B24[7][2];
			g3 = b_2*B24[0][3] + b_1*B24[1][3] + b0*B24[2][3] + b1*B24[3][3] + b2*B24[4][3] + b3*B24[5][3] + b4*B24[6][3] + b5*B24[7][3];
			g4 = b_2*B24[0][4] + b_1*B24[1][4] + b0*B24[2][4] + b1*B24[3][4] + b2*B24[4][4] + b3*B24[5][4] + b4*B24[6][4] + b5*B24[7][4];
			g5 = b_2*B24[0][5] + b_1*B24[1][5] + b0*B24[2][5] + b1*B24[3][5] + b2*B24[4][5] + b3*B24[5][5] + b4*B24[6][5] + b5*B24[7][5];
			g6 = b_2*B24[0][6] + b_1*B24[1][6] + b0*B24[2][6] + b1*B24[3][6] + b2*B24[4][6] + b3*B24[5][6] + b4*B24[6][6] + b5*B24[7][6];
			g7 = b_2*B24[0][7] + b_1*B24[1][7] + b0*B24[2][7] + b1*B24[3][7] + b2*B24[4][7] + b3*B24[5][7] + b4*B24[6][7] + b5*B24[7][7];
			g8 = b_2*B24[0][8] + b_1*B24[1][8] + b0*B24[2][8] + b1*B24[3][8] + b2*B24[4][8] + b3*B24[5][8] + b4*B24[6][8] + b5*B24[7][8];
			g9 = b_2*B24[0][9] + b_1*B24[1][9] + b0*B24[2][9] + b1*B24[3][9] + b2*B24[4][9] + b3*B24[5][9] + b4*B24[6][9] + b5*B24[7][9];
			g10 = b_2*B24[0][10] + b_1*B24[1][10] + b0*B24[2][10] + b1*B24[3][10] + b2*B24[4][10] + b3*B24[5][10] + b4*B24[6][10] + b5*B24[7][10];
			g11 = b_2*B24[0][11] + b_1*B24[1][11] + b0*B24[2][11] + b1*B24[3][11] + b2*B24[4][11] + b3*B24[5][11] + b4*B24[6][11] + b5*B24[7][11];
			g12 = b_2*B24[0][12] + b_1*B24[1][12] + b0*B24[2][12] + b1*B24[3][12] + b2*B24[4][12] + b3*B24[5][12] + b4*B24[6][12] + b5*B24[7][12];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10;
			eighth_z[i][j] = z;

			m0 = b_2*B23[0][0] + b_1*B23[1][0] + b0*B23[2][0] + b1*B23[3][0] + b2*B23[4][0] + b3*B23[5][0] + b4*B23[6][0] + b5*B23[7][0];
			m1 = b_2*B23[0][1] + b_1*B23[1][1] + b0*B23[2][1] + b1*B23[3][1] + b2*B23[4][1] + b3*B23[5][1] + b4*B23[6][1] + b5*B23[7][1];
			m2 = b_2*B23[0][2] + b_1*B23[1][2] + b0*B23[2][2] + b1*B23[3][2] + b2*B23[4][2] + b3*B23[5][2] + b4*B23[6][2] + b5*B23[7][2];
			m3 = b_2*B23[0][3] + b_1*B23[1][3] + b0*B23[2][3] + b1*B23[3][3] + b2*B23[4][3] + b3*B23[5][3] + b4*B23[6][3] + b5*B23[7][3];
			m4 = b_2*B23[0][4] + b_1*B23[1][4] + b0*B23[2][4] + b1*B23[3][4] + b2*B23[4][4] + b3*B23[5][4] + b4*B23[6][4] + b5*B23[7][4];
			m5 = b_2*B23[0][5] + b_1*B23[1][5] + b0*B23[2][5] + b1*B23[3][5] + b2*B23[4][5] + b3*B23[5][5] + b4*B23[6][5] + b5*B23[7][5];
			m6 = b_2*B23[0][6] + b_1*B23[1][6] + b0*B23[2][6] + b1*B23[3][6] + b2*B23[4][6] + b3*B23[5][6] + b4*B23[6][6] + b5*B23[7][6];
			m7 = b_2*B23[0][7] + b_1*B23[1][7] + b0*B23[2][7] + b1*B23[3][7] + b2*B23[4][7] + b3*B23[5][7] + b4*B23[6][7] + b5*B23[7][7];
			m8 = b_2*B23[0][8] + b_1*B23[1][8] + b0*B23[2][8] + b1*B23[3][8] + b2*B23[4][8] + b3*B23[5][8] + b4*B23[6][8] + b5*B23[7][8];
			m9 = b_2*B23[0][9] + b_1*B23[1][9] + b0*B23[2][9] + b1*B23[3][9] + b2*B23[4][9] + b3*B23[5][9] + b4*B23[6][9] + b5*B23[7][9];
			m10 = b_2*B23[0][10] + b_1*B23[1][10] + b0*B23[2][10] + b1*B23[3][10] + b2*B23[4][10] + b3*B23[5][10] + b4*B23[6][10] + b5*B23[7][10];
			m11 = b_2*B23[0][11] + b_1*B23[1][11] + b0*B23[2][11] + b1*B23[3][11] + b2*B23[4][11] + b3*B23[5][11] + b4*B23[6][11] + b5*B23[7][11];
			m12 = b_2*B23[0][12] + b_1*B23[1][12] + b0*B23[2][12] + b1*B23[3][12] + b2*B23[4][12] + b3*B23[5][12] + b4*B23[6][12] + b5*B23[7][12];

			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10;
			eighth_yy[i][j] = xxxx;

			n0 = b_2*B22[0][0] + b_1*B22[1][0] + b0*B22[2][0] + b1*B22[3][0] + b2*B22[4][0] + b3*B22[5][0] + b4*B22[6][0] + b5*B22[7][0];
			n1 = b_2*B22[0][1] + b_1*B22[1][1] + b0*B22[2][1] + b1*B22[3][1] + b2*B22[4][1] + b3*B22[5][1] + b4*B22[6][1] + b5*B22[7][1];
			n2 = b_2*B22[0][2] + b_1*B22[1][2] + b0*B22[2][2] + b1*B22[3][2] + b2*B22[4][2] + b3*B22[5][2] + b4*B22[6][2] + b5*B22[7][2];
			n3 = b_2*B22[0][3] + b_1*B22[1][3] + b0*B22[2][3] + b1*B22[3][3] + b2*B22[4][3] + b3*B22[5][3] + b4*B22[6][3] + b5*B22[7][3];
			n4 = b_2*B22[0][4] + b_1*B22[1][4] + b0*B22[2][4] + b1*B22[3][4] + b2*B22[4][4] + b3*B22[5][4] + b4*B22[6][4] + b5*B22[7][4];
			n5 = b_2*B22[0][5] + b_1*B22[1][5] + b0*B22[2][5] + b1*B22[3][5] + b2*B22[4][5] + b3*B22[5][5] + b4*B22[6][5] + b5*B22[7][5];
			n6 = b_2*B22[0][6] + b_1*B22[1][6] + b0*B22[2][6] + b1*B22[3][6] + b2*B22[4][6] + b3*B22[5][6] + b4*B22[6][6] + b5*B22[7][6];
			n7 = b_2*B22[0][7] + b_1*B22[1][7] + b0*B22[2][7] + b1*B22[3][7] + b2*B22[4][7] + b3*B22[5][7] + b4*B22[6][7] + b5*B22[7][7];
			n8 = b_2*B22[0][8] + b_1*B22[1][8] + b0*B22[2][8] + b1*B22[3][8] + b2*B22[4][8] + b3*B22[5][8] + b4*B22[6][8] + b5*B22[7][8];
			n9 = b_2*B22[0][9] + b_1*B22[1][9] + b0*B22[2][9] + b1*B22[3][9] + b2*B22[4][9] + b3*B22[5][9] + b4*B22[6][9] + b5*B22[7][9];
			n10 = b_2*B22[0][10] + b_1*B22[1][10] + b0*B22[2][10] + b1*B22[3][10] + b2*B22[4][10] + b3*B22[5][10] + b4*B22[6][10] + b5*B22[7][10];
			n11 = b_2*B22[0][11] + b_1*B22[1][11] + b0*B22[2][11] + b1*B22[3][11] + b2*B22[4][11] + b3*B22[5][11] + b4*B22[6][11] + b5*B22[7][11];
			n12 = b_2*B22[0][12] + b_1*B22[1][12] + b0*B22[2][12] + b1*B22[3][12] + b2*B22[4][12] + b3*B22[5][12] + b4*B22[6][12] + b5*B22[7][12];

			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10;
			eighth_xx[i][j] = kkkk;
		}
	}

	//zuoshou
	L_Elbow_point[0] = -L_Elbow_point[0];
	L_Elbow_point_2[0] = -L_Elbow_point_2[0];

	upperarm_temp_point[3][0] = L_Elbow_point[0];
	upperarm_temp_point[3][1] = L_Elbow_point[1];
	upperarm_temp_point[3][2] = L_Elbow_point[2];

	upperarm_temp_point[0][0] = 15.430;
	upperarm_temp_point[0][1] = 55.826;
	upperarm_temp_point[0][2] = -0.327;

	upperarm_temp_point[1][0] = (upperarm_temp_point[0][0] - upperarm_temp_point[3][0]) * 2 / 3 + upperarm_temp_point[3][0];
	upperarm_temp_point[1][1] = (upperarm_temp_point[0][1] - upperarm_temp_point[3][1]) * 2 / 3 + upperarm_temp_point[3][1];
	upperarm_temp_point[1][2] = (upperarm_temp_point[0][2] - upperarm_temp_point[3][2]) * 2 / 3 + upperarm_temp_point[3][2];

	upperarm_temp_point[2][0] = (upperarm_temp_point[0][0] - upperarm_temp_point[3][0]) * 1 / 3 + upperarm_temp_point[3][0];
	upperarm_temp_point[2][1] = (upperarm_temp_point[0][1] - upperarm_temp_point[3][1]) * 1 / 3 + upperarm_temp_point[3][1];
	upperarm_temp_point[2][2] = (upperarm_temp_point[0][2] - upperarm_temp_point[3][2]) * 1 / 3 + upperarm_temp_point[3][2];



	upperarm_temp_point[6][0] = L_Elbow_point_2[0];
	upperarm_temp_point[6][1] = L_Elbow_point_2[1];
	upperarm_temp_point[6][2] = L_Elbow_point_2[2];

	upperarm_temp_point[4][0] = (upperarm_temp_point[3][0] - upperarm_temp_point[6][0]) * 2 / 3 + upperarm_temp_point[6][0];
	upperarm_temp_point[4][1] = (upperarm_temp_point[3][1] - upperarm_temp_point[6][1]) * 2 / 3 + upperarm_temp_point[6][1];
	upperarm_temp_point[4][2] = (upperarm_temp_point[3][2] - upperarm_temp_point[6][2]) * 2 / 3 + upperarm_temp_point[6][2];
	upperarm_temp_point[5][0] = (upperarm_temp_point[3][0] - upperarm_temp_point[6][0]) * 1 / 3 + upperarm_temp_point[6][0];
	upperarm_temp_point[5][1] = (upperarm_temp_point[3][1] - upperarm_temp_point[6][1]) * 1 / 3 + upperarm_temp_point[6][1];
	upperarm_temp_point[5][2] = (upperarm_temp_point[3][2] - upperarm_temp_point[6][2]) * 1 / 3 + upperarm_temp_point[6][2];
	upperarm_temp_point[7][0] = (upperarm_temp_point[3][0] - upperarm_temp_point[6][0])*(-0.1) + upperarm_temp_point[6][0];
	upperarm_temp_point[7][1] = (upperarm_temp_point[3][1] - upperarm_temp_point[6][1])*(-0.1) + upperarm_temp_point[6][1];
	upperarm_temp_point[7][2] = (upperarm_temp_point[3][2] - upperarm_temp_point[6][2])*(-0.1) + upperarm_temp_point[6][2];
	upperarm_temp_point[8][0] = (upperarm_temp_point[3][0] - upperarm_temp_point[6][0])*(-0.2) + upperarm_temp_point[6][0];
	upperarm_temp_point[8][1] = (upperarm_temp_point[3][1] - upperarm_temp_point[6][1])*(-0.2) + upperarm_temp_point[6][1];
	upperarm_temp_point[8][2] = (upperarm_temp_point[3][2] - upperarm_temp_point[6][2])*(-0.2) + upperarm_temp_point[6][2];

	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[2][i][0] = upperarm_length*cos(ang);
		control_Elbow[2][i][1] = 0;
		control_Elbow[2][i][2] = -upperarm_length*sin(ang);
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[0][i][0] = upperarm_length*cos(ang)*1.15;
		control_Elbow[0][i][1] = 0;
		control_Elbow[0][i][2] = -upperarm_length*sin(ang)*1.15;
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[1][i][0] = upperarm_length*cos(ang)*1.08;
		control_Elbow[1][i][1] = 0;
		control_Elbow[1][i][2] = -upperarm_length*sin(ang)*1.08;
	}

	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[5][i][0] = upperarm1_length*cos(ang);
		control_Elbow[5][i][1] = 0;
		control_Elbow[5][i][2] = -upperarm1_length*sin(ang);
	}

	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[3][i][0] = ((upperarm_length - upperarm1_length)*0.67 + upperarm1_length)*cos(ang);
		control_Elbow[3][i][1] = 0;
		control_Elbow[3][i][2] = -((upperarm_length - upperarm1_length)*0.67 + upperarm1_length)*sin(ang);
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[4][i][0] = ((upperarm_length - upperarm1_length)*0.33 + upperarm1_length)*cos(ang);
		control_Elbow[4][i][1] = 0;
		control_Elbow[4][i][2] = -((upperarm_length - upperarm1_length)*0.33 + upperarm1_length)*sin(ang);
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[6][i][0] = upperarm1_length*cos(ang)*0.95;
		control_Elbow[6][i][1] = 0;
		control_Elbow[6][i][2] = -upperarm1_length*sin(ang)*0.95;
	}
	for (int i = 0; i<8; i++)
	{
		double ang = 3 * PI / 2 + PI*i / 4;
		control_Elbow[7][i][0] = upperarm1_length*cos(ang)*0.9;
		control_Elbow[7][i][1] = 0;
		control_Elbow[7][i][2] = -upperarm1_length*sin(ang)*0.9;
	}



	CVector3 L_shoubi1(0.0f, 1.0f, 0.0f);
	CVector3 L_shoubi2((upperarm_temp_point[0][0] - upperarm_temp_point[3][0]), (upperarm_temp_point[0][1] - upperarm_temp_point[3][1]), (upperarm_temp_point[0][2] - upperarm_temp_point[3][2]));
	CVector3 L_faxiangliang1 = Cross(L_shoubi1, L_shoubi2);
	double L_jiajiao = acos((L_shoubi1.x*L_shoubi2.x + L_shoubi1.y*L_shoubi2.y + L_shoubi1.z*L_shoubi2.z) / (pow((pow(L_shoubi1.x, 2.0) + pow(L_shoubi1.y, 2.0) + pow(L_shoubi1.z, 2.0)), 0.5)*pow((pow(L_shoubi2.x, 2.0) + pow(L_shoubi2.y, 2.0) + pow(L_shoubi2.z, 2.0)), 0.5)));

	ggg = upperarm_temp_point[0][1] - upperarm_temp_point[3][1];
	gggg = upperarm_temp_point[3][0] - upperarm_temp_point[0][0];
	ang_temp1 = (upperarm_temp_point[3][0] - upperarm_temp_point[0][0]) / (upperarm_temp_point[0][1] - upperarm_temp_point[3][1]);
	ang_temp2 = (upperarm_temp_point[3][2] - upperarm_temp_point[0][2]) / pow((pow(ggg, 2.0) + pow(gggg, 2.0)), 0.5);



	//		m3dRotationMatrix44(rotationMatrix1,atan(ang_temp1),0.0f,0.0f,1.0f);
	m3dRotationMatrix44(rotationMatrix1, L_jiajiao, L_faxiangliang1.x, L_faxiangliang1.y, L_faxiangliang1.z);
	m3dRotationMatrix44(rotationMatrix2, atan(ang_temp2), 0.0f, 0.0f, 1.0f);
	m3dTranslateMatrix44(translationMatrix, upperarm_temp_point[3][0], upperarm_temp_point[3][1], upperarm_temp_point[3][2]);

	rotationMatrix1[12] = upperarm_temp_point[3][0];
	rotationMatrix1[13] = upperarm_temp_point[3][1];
	rotationMatrix1[14] = upperarm_temp_point[3][2];


	B26[2][0] = 55.303;
	B26[2][1] = 58.623;
	B26[2][2] = 59.925;
	B26[2][3] = 58.624;
	B26[2][4] = 55.302;
	B26[2][5] = 51.751;
	B26[2][6] = 49.835;
	B26[2][7] = 51.755;
	B26[2][8] = 55.303;
	B26[2][9] = 58.623;
	B26[2][10] = 59.925;
	B26[2][11] = 58.624;
	B26[2][12] = 55.302;
	for (int i = 0; i<13; i++)
	{
		B26[0][i] = B26[1][i] = B26[2][i];
	}

	B25[2][0] = 13.936;
	B25[2][1] = 14.375;
	B25[2][2] = 13.417;
	B25[2][3] = 15.535;
	B25[2][4] = 14.282;
	B25[2][5] = 13.234;
	B25[2][6] = 12.980;
	B25[2][7] = 13.410;
	B25[2][8] = 13.936;
	B25[2][9] = 14.375;
	B25[2][10] = 13.417;
	B25[2][11] = 15.535;
	B25[2][12] = 14.282;
	for (int i = 0; i<13; i++)
	{
		B25[0][i] = B25[2][i] - 4;
		B25[1][i] = B25[2][i] - 2;

	}
	B27[2][0] = 2.772;
	B27[2][1] = 1.100;
	B27[2][2] = -0.034;
	B27[2][3] = -0.311;
	B27[2][4] = -3.657;
	B27[2][5] = -3.262;
	B27[2][6] = -0.715;
	B27[2][7] = 1.757;
	B27[2][8] = 2.772;
	B27[2][9] = 1.100;
	B27[2][10] = -0.034;
	B27[2][11] = -0.311;
	B27[2][12] = -3.657;
	for (int i = 0; i<13; i++)
	{
		B27[0][i] = B27[1][i] = B25[2][i];
	}





	rotationMatrix1[12] = upperarm_temp_point[2][0];
	rotationMatrix1[13] = upperarm_temp_point[2][1];
	rotationMatrix1[14] = upperarm_temp_point[2][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[1][i], control_Elbow[1][i], rotationMatrix1);

	}
	for (int i = 0; i<8; i++)
	{
		B26[4][i] = control1_Elbow[1][i][1];
		B25[4][i] = control1_Elbow[1][i][0];
		B27[4][i] = control1_Elbow[1][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B26[4][i] = B26[4][i - 8];
		B25[4][i] = B25[4][i - 8];
		B27[4][i] = B27[4][i - 8];

	}
	rotationMatrix1[12] = upperarm_temp_point[1][0];
	rotationMatrix1[13] = upperarm_temp_point[1][1];
	rotationMatrix1[14] = upperarm_temp_point[1][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[0][i], control_Elbow[0][i], rotationMatrix1);

	}
	for (int i = 0; i<8; i++)
	{
		B26[3][i] = control1_Elbow[0][i][1];
		B25[3][i] = control1_Elbow[0][i][0];
		B27[3][i] = control1_Elbow[0][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B26[3][i] = B26[3][i - 8];
		B25[3][i] = B25[3][i - 8];
		B27[3][i] = B27[3][i - 8];

	}
	//guanjiedian
	L_shoubi2.x = upperarm_temp_point[0][0] - upperarm_temp_point[6][0];
	L_shoubi2.y = upperarm_temp_point[0][1] - upperarm_temp_point[6][1];
	L_shoubi2.z = upperarm_temp_point[0][2] - upperarm_temp_point[6][2];

	//		CVector3 shoubi2((upperarm_temp_point[0][0]-upperarm_temp_point[3][0]),(upperarm_temp_point[0][1]-upperarm_temp_point[3][1]),(upperarm_temp_point[0][2]-upperarm_temp_point[3][2]));
	CVector3 L_faxiangliang2 = Cross(shoubi1, shoubi2);
	jiajiao = acos((L_shoubi1.x*L_shoubi2.x + L_shoubi1.y*L_shoubi2.y + L_shoubi1.z*L_shoubi2.z) / (pow((pow(L_shoubi1.x, 2.0) + pow(L_shoubi1.y, 2.0) + pow(L_shoubi1.z, 2.0)), 0.5)*pow((pow(L_shoubi2.x, 2.0) + pow(L_shoubi2.y, 2.0) + pow(L_shoubi2.z, 2.0)), 0.5)));
	m3dRotationMatrix44(rotationMatrix2, jiajiao, L_faxiangliang2.x, L_faxiangliang2.y, L_faxiangliang2.z);

	rotationMatrix2[12] = upperarm_temp_point[3][0];
	rotationMatrix2[13] = upperarm_temp_point[3][1];
	rotationMatrix2[14] = upperarm_temp_point[3][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[2][i], control_Elbow[2][i], rotationMatrix2);

	}
	for (int i = 0; i<8; i++)
	{
		B26[5][i] = control1_Elbow[2][i][1];
		B25[5][i] = control1_Elbow[2][i][0];
		B27[5][i] = control1_Elbow[2][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B26[5][i] = B26[5][i - 8];
		B25[5][i] = B25[5][i - 8];
		B27[5][i] = B27[5][i - 8];

	}

	shoubi2.x = upperarm_temp_point[3][0] - upperarm_temp_point[6][0];
	shoubi2.y = upperarm_temp_point[3][1] - upperarm_temp_point[6][1];
	shoubi2.z = upperarm_temp_point[3][2] - upperarm_temp_point[6][2];

	//		CVector3 shoubi2((upperarm_temp_point[0][0]-upperarm_temp_point[3][0]),(upperarm_temp_point[0][1]-upperarm_temp_point[3][1]),(upperarm_temp_point[0][2]-upperarm_temp_point[3][2]));
	CVector3 L_faxiangliang3 = Cross(shoubi1, shoubi2);
	jiajiao = acos((L_shoubi1.x*L_shoubi2.x + L_shoubi1.y*L_shoubi2.y + L_shoubi1.z*L_shoubi2.z) / (pow((pow(L_shoubi1.x, 2.0) + pow(L_shoubi1.y, 2.0) + pow(L_shoubi1.z, 2.0)), 0.5)*pow((pow(L_shoubi2.x, 2.0) + pow(L_shoubi2.y, 2.0) + pow(L_shoubi2.z, 2.0)), 0.5)));
	m3dRotationMatrix44(rotationMatrix3, jiajiao, L_faxiangliang3.x, L_faxiangliang3.y, L_faxiangliang3.z);

	rotationMatrix3[12] = upperarm_temp_point[4][0];
	rotationMatrix3[13] = upperarm_temp_point[4][1];
	rotationMatrix3[14] = upperarm_temp_point[4][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[3][i], control_Elbow[3][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B26[6][i] = control1_Elbow[3][i][1];
		B25[6][i] = control1_Elbow[3][i][0];
		B27[6][i] = control1_Elbow[3][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B26[6][i] = B26[6][i - 8];
		B25[6][i] = B25[6][i - 8];
		B27[6][i] = B27[6][i - 8];

	}
	rotationMatrix3[12] = upperarm_temp_point[5][0];
	rotationMatrix3[13] = upperarm_temp_point[5][1];
	rotationMatrix3[14] = upperarm_temp_point[5][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[4][i], control_Elbow[4][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B26[7][i] = control1_Elbow[4][i][1];
		B25[7][i] = control1_Elbow[4][i][0];
		B27[7][i] = control1_Elbow[4][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B26[7][i] = B26[7][i - 8];
		B25[7][i] = B25[7][i - 8];
		B27[7][i] = B27[7][i - 8];

	}

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<13; j++)
		{
			B28[i][j] = B25[i + 3][j];
			B29[i][j] = B26[i + 3][j];
			B30[i][j] = B27[i + 3][j];

		}
	}
	rotationMatrix3[12] = upperarm_temp_point[6][0];
	rotationMatrix3[13] = upperarm_temp_point[6][1];
	rotationMatrix3[14] = upperarm_temp_point[6][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[5][i], control_Elbow[5][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B29[5][i] = control1_Elbow[5][i][1];
		B28[5][i] = control1_Elbow[5][i][0];
		B30[5][i] = control1_Elbow[5][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B29[5][i] = B29[5][i - 8];
		B28[5][i] = B28[5][i - 8];
		B30[5][i] = B30[5][i - 8];

	}
	rotationMatrix3[12] = upperarm_temp_point[7][0];
	rotationMatrix3[13] = upperarm_temp_point[7][1];
	rotationMatrix3[14] = upperarm_temp_point[7][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[6][i], control_Elbow[6][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B29[6][i] = control1_Elbow[6][i][1];
		B28[6][i] = control1_Elbow[6][i][0];
		B30[6][i] = control1_Elbow[6][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B29[6][i] = B29[6][i - 8];
		B28[6][i] = B28[6][i - 8];
		B30[6][i] = B30[6][i - 8];

	}
	rotationMatrix3[12] = upperarm_temp_point[8][0];
	rotationMatrix3[13] = upperarm_temp_point[8][1];
	rotationMatrix3[14] = upperarm_temp_point[8][2];
	for (int i = 0; i<8; i++)
	{
		m3dTransformVector3(control1_Elbow[7][i], control_Elbow[7][i], rotationMatrix3);

	}
	for (int i = 0; i<8; i++)
	{
		B29[7][i] = control1_Elbow[7][i][1];
		B28[7][i] = control1_Elbow[7][i][0];
		B30[7][i] = control1_Elbow[7][i][2];
	}
	for (int i = 8; i<13; i++)
	{
		B29[7][i] = B29[7][i - 8];
		B28[7][i] = B28[7][i - 8];
		B30[7][i] = B30[7][i - 8];

	}
	for (int i = 0; i<8; i++)
	{
		for (int j = 0; j<8; j++)
		{
			B31[i][j] = -B25[i][(8 - j) % 8];
			B32[i][j] = B26[i][(8 - j) % 8];
			B33[i][j] = B27[i][(8 - j) % 8];
			B34[i][j] = -B28[i][(8 - j) % 8];
			B35[i][j] = B29[i][(8 - j) % 8];
			B36[i][j] = B30[i][(8 - j) % 8];

		}
	}
	for (int i = 0; i<8; i++)
	{
		for (int j = 8; j<13; j++)
		{
			B31[i][j] = B31[i][j - 8];
			B32[i][j] = B32[i][j - 8];
			B33[i][j] = B33[i][j - 8];
			B34[i][j] = B34[i][j - 8];
			B35[i][j] = B35[i][j - 8];
			B36[i][j] = B36[i][j - 8];

		}
	}

	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			ninth_x[i][j] = j * 8 / POINTNUMEACHPART;
			ninth_y[i][j] = i * 3 / POINTNUMEACHPART;
			if ((ninth_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((ninth_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] + 1);
			}

			if ((ninth_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((ninth_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] + 2);
			}

			if (ninth_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((ninth_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] + 0);
			}

			if ((ninth_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((ninth_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 1);
			}

			if ((ninth_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((ninth_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 2);
			}

			if ((ninth_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((ninth_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 3);
			}

			if ((ninth_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((ninth_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 4);
			}

			if ((ninth_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((ninth_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 5);
			}

			if ((ninth_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((ninth_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 6);
			}

			if ((ninth_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((ninth_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 7);
			}

			if ((ninth_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((ninth_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 8);
			}

			if ((ninth_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((ninth_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 9);
			}

			if ((ninth_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((ninth_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((ninth_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_x[i][j] - 10);
			}


			if ((ninth_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((ninth_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] + 1);
			}

			if ((ninth_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((ninth_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] + 2);
			}

			if ((ninth_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((ninth_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] + 0);
			}

			if ((ninth_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((ninth_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] - 1);
			}

			if ((ninth_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((ninth_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] - 2);
			}

			if ((ninth_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((ninth_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] - 3);
			}

			if ((ninth_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((ninth_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] - 4);
			}

			if ((ninth_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((ninth_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] - 5);
			}

			if ((ninth_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((ninth_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] - 6);
			}

			if ((ninth_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((ninth_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((ninth_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (ninth_y[i][j] - 7);
			}
			g0 = b_2*B33[0][0] + b_1*B33[1][0] + b0*B33[2][0] + b1*B33[3][0] + b2*B33[4][0] + b3*B33[5][0] + b4*B33[6][0] + b5*B33[7][0];
			g1 = b_2*B33[0][1] + b_1*B33[1][1] + b0*B33[2][1] + b1*B33[3][1] + b2*B33[4][1] + b3*B33[5][1] + b4*B33[6][1] + b5*B33[7][1];
			g2 = b_2*B33[0][2] + b_1*B33[1][2] + b0*B33[2][2] + b1*B33[3][2] + b2*B33[4][2] + b3*B33[5][2] + b4*B33[6][2] + b5*B33[7][2];
			g3 = b_2*B33[0][3] + b_1*B33[1][3] + b0*B33[2][3] + b1*B33[3][3] + b2*B33[4][3] + b3*B33[5][3] + b4*B33[6][3] + b5*B33[7][3];
			g4 = b_2*B33[0][4] + b_1*B33[1][4] + b0*B33[2][4] + b1*B33[3][4] + b2*B33[4][4] + b3*B33[5][4] + b4*B33[6][4] + b5*B33[7][4];
			g5 = b_2*B33[0][5] + b_1*B33[1][5] + b0*B33[2][5] + b1*B33[3][5] + b2*B33[4][5] + b3*B33[5][5] + b4*B33[6][5] + b5*B33[7][5];
			g6 = b_2*B33[0][6] + b_1*B33[1][6] + b0*B33[2][6] + b1*B33[3][6] + b2*B33[4][6] + b3*B33[5][6] + b4*B33[6][6] + b5*B33[7][6];
			g7 = b_2*B33[0][7] + b_1*B33[1][7] + b0*B33[2][7] + b1*B33[3][7] + b2*B33[4][7] + b3*B33[5][7] + b4*B33[6][7] + b5*B33[7][7];
			g8 = b_2*B33[0][8] + b_1*B33[1][8] + b0*B33[2][8] + b1*B33[3][8] + b2*B33[4][8] + b3*B33[5][8] + b4*B33[6][8] + b5*B33[7][8];
			g9 = b_2*B33[0][9] + b_1*B33[1][9] + b0*B33[2][9] + b1*B33[3][9] + b2*B33[4][9] + b3*B33[5][9] + b4*B33[6][9] + b5*B33[7][9];
			g10 = b_2*B33[0][10] + b_1*B33[1][10] + b0*B33[2][10] + b1*B33[3][10] + b2*B33[4][10] + b3*B33[5][10] + b4*B33[6][10] + b5*B33[7][10];
			g11 = b_2*B33[0][11] + b_1*B33[1][11] + b0*B33[2][11] + b1*B33[3][11] + b2*B33[4][11] + b3*B33[5][11] + b4*B33[6][11] + b5*B33[7][11];
			g12 = b_2*B33[0][12] + b_1*B33[1][12] + b0*B33[2][12] + b1*B33[3][12] + b2*B33[4][12] + b3*B33[5][12] + b4*B33[6][12] + b5*B33[7][12];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10;
			ninth_z[i][j] = z;

			m0 = b_2*B32[0][0] + b_1*B32[1][0] + b0*B32[2][0] + b1*B32[3][0] + b2*B32[4][0] + b3*B32[5][0] + b4*B32[6][0] + b5*B32[7][0];
			m1 = b_2*B32[0][1] + b_1*B32[1][1] + b0*B32[2][1] + b1*B32[3][1] + b2*B32[4][1] + b3*B32[5][1] + b4*B32[6][1] + b5*B32[7][1];
			m2 = b_2*B32[0][2] + b_1*B32[1][2] + b0*B32[2][2] + b1*B32[3][2] + b2*B32[4][2] + b3*B32[5][2] + b4*B32[6][2] + b5*B32[7][2];
			m3 = b_2*B32[0][3] + b_1*B32[1][3] + b0*B32[2][3] + b1*B32[3][3] + b2*B32[4][3] + b3*B32[5][3] + b4*B32[6][3] + b5*B32[7][3];
			m4 = b_2*B32[0][4] + b_1*B32[1][4] + b0*B32[2][4] + b1*B32[3][4] + b2*B32[4][4] + b3*B32[5][4] + b4*B32[6][4] + b5*B32[7][4];
			m5 = b_2*B32[0][5] + b_1*B32[1][5] + b0*B32[2][5] + b1*B32[3][5] + b2*B32[4][5] + b3*B32[5][5] + b4*B32[6][5] + b5*B32[7][5];
			m6 = b_2*B32[0][6] + b_1*B32[1][6] + b0*B32[2][6] + b1*B32[3][6] + b2*B32[4][6] + b3*B32[5][6] + b4*B32[6][6] + b5*B32[7][6];
			m7 = b_2*B32[0][7] + b_1*B32[1][7] + b0*B32[2][7] + b1*B32[3][7] + b2*B32[4][7] + b3*B32[5][7] + b4*B32[6][7] + b5*B32[7][7];
			m8 = b_2*B32[0][8] + b_1*B32[1][8] + b0*B32[2][8] + b1*B32[3][8] + b2*B32[4][8] + b3*B32[5][8] + b4*B32[6][8] + b5*B32[7][8];
			m9 = b_2*B32[0][9] + b_1*B32[1][9] + b0*B32[2][9] + b1*B32[3][9] + b2*B32[4][9] + b3*B32[5][9] + b4*B32[6][9] + b5*B32[7][9];
			m10 = b_2*B32[0][10] + b_1*B32[1][10] + b0*B32[2][10] + b1*B32[3][10] + b2*B32[4][10] + b3*B32[5][10] + b4*B32[6][10] + b5*B32[7][10];
			m11 = b_2*B32[0][11] + b_1*B32[1][11] + b0*B32[2][11] + b1*B32[3][11] + b2*B32[4][11] + b3*B32[5][11] + b4*B32[6][11] + b5*B32[7][11];
			m12 = b_2*B32[0][12] + b_1*B32[1][12] + b0*B32[2][12] + b1*B32[3][12] + b2*B32[4][12] + b3*B32[5][12] + b4*B32[6][12] + b5*B32[7][12];

			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10;
			ninth_yy[i][j] = xxxx;

			n0 = b_2*B31[0][0] + b_1*B31[1][0] + b0*B31[2][0] + b1*B31[3][0] + b2*B31[4][0] + b3*B31[5][0] + b4*B31[6][0] + b5*B31[7][0];
			n1 = b_2*B31[0][1] + b_1*B31[1][1] + b0*B31[2][1] + b1*B31[3][1] + b2*B31[4][1] + b3*B31[5][1] + b4*B31[6][1] + b5*B31[7][1];
			n2 = b_2*B31[0][2] + b_1*B31[1][2] + b0*B31[2][2] + b1*B31[3][2] + b2*B31[4][2] + b3*B31[5][2] + b4*B31[6][2] + b5*B31[7][2];
			n3 = b_2*B31[0][3] + b_1*B31[1][3] + b0*B31[2][3] + b1*B31[3][3] + b2*B31[4][3] + b3*B31[5][3] + b4*B31[6][3] + b5*B31[7][3];
			n4 = b_2*B31[0][4] + b_1*B31[1][4] + b0*B31[2][4] + b1*B31[3][4] + b2*B31[4][4] + b3*B31[5][4] + b4*B31[6][4] + b5*B31[7][4];
			n5 = b_2*B31[0][5] + b_1*B31[1][5] + b0*B31[2][5] + b1*B31[3][5] + b2*B31[4][5] + b3*B31[5][5] + b4*B31[6][5] + b5*B31[7][5];
			n6 = b_2*B31[0][6] + b_1*B31[1][6] + b0*B31[2][6] + b1*B31[3][6] + b2*B31[4][6] + b3*B31[5][6] + b4*B31[6][6] + b5*B31[7][6];
			n7 = b_2*B31[0][7] + b_1*B31[1][7] + b0*B31[2][7] + b1*B31[3][7] + b2*B31[4][7] + b3*B31[5][7] + b4*B31[6][7] + b5*B31[7][7];
			n8 = b_2*B31[0][8] + b_1*B31[1][8] + b0*B31[2][8] + b1*B31[3][8] + b2*B31[4][8] + b3*B31[5][8] + b4*B31[6][8] + b5*B31[7][8];
			n9 = b_2*B31[0][9] + b_1*B31[1][9] + b0*B31[2][9] + b1*B31[3][9] + b2*B31[4][9] + b3*B31[5][9] + b4*B31[6][9] + b5*B31[7][9];
			n10 = b_2*B31[0][10] + b_1*B31[1][10] + b0*B31[2][10] + b1*B31[3][10] + b2*B31[4][10] + b3*B31[5][10] + b4*B31[6][10] + b5*B31[7][10];
			n11 = b_2*B31[0][11] + b_1*B31[1][11] + b0*B31[2][11] + b1*B31[3][11] + b2*B31[4][11] + b3*B31[5][11] + b4*B31[6][11] + b5*B31[7][11];
			n12 = b_2*B31[0][12] + b_1*B31[1][12] + b0*B31[2][12] + b1*B31[3][12] + b2*B31[4][12] + b3*B31[5][12] + b4*B31[6][12] + b5*B31[7][12];

			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10;
			ninth_xx[i][j] = kkkk;
		}
	}
	for (int i = 0; i<POINTNUMEACHPART + 1; i++)
	{
		for (int j = 0; j<POINTNUMEACHPART + 1; j++)
		{
			tenth_x[i][j] = j * 8 / POINTNUMEACHPART;
			tenth_y[i][j] = i * 3 / POINTNUMEACHPART;
			if ((tenth_x[i][j] + 1) == 0)
			{
				a_1 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] + 1), 2.0f) / (-3);
				a_1 = sin((tenth_x[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] + 1);
			}

			if ((tenth_x[i][j] + 2) == 0)
			{
				a_2 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] + 2), 2.0f) / (-3);
				a_2 = sin((tenth_x[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] + 2);
			}

			if (tenth_x[i][j] == 0)
			{
				a0 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] + 0), 2.0f) / (-3);
				a0 = sin((tenth_x[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] + 0);
			}

			if ((tenth_x[i][j] - 1) == 0)
			{
				a1 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 1), 2.0f) / (-3);
				a1 = sin((tenth_x[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 1);
			}

			if ((tenth_x[i][j] - 2) == 0)
			{
				a2 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 2), 2.0f) / (-3);
				a2 = sin((tenth_x[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 2);
			}

			if ((tenth_x[i][j] - 3) == 0)
			{
				a3 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 3), 2.0f) / (-3);
				a3 = sin((tenth_x[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 3);
			}

			if ((tenth_x[i][j] - 4) == 0)
			{
				a4 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 4), 2.0f) / (-3);
				a4 = sin((tenth_x[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 4);
			}

			if ((tenth_x[i][j] - 5) == 0)
			{
				a5 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 5), 2.0f) / (-3);
				a5 = sin((tenth_x[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 5);
			}

			if ((tenth_x[i][j] - 6) == 0)
			{
				a6 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 6), 2.0f) / (-3);
				a6 = sin((tenth_x[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 6);
			}

			if ((tenth_x[i][j] - 7) == 0)
			{
				a7 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 7), 2.0f) / (-3);
				a7 = sin((tenth_x[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 7);
			}

			if ((tenth_x[i][j] - 8) == 0)
			{
				a8 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 8), 2.0f) / (-3);
				a8 = sin((tenth_x[i][j] - 8)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 8);
			}

			if ((tenth_x[i][j] - 9) == 0)
			{
				a9 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 9), 2.0f) / (-3);
				a9 = sin((tenth_x[i][j] - 9)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 9);
			}

			if ((tenth_x[i][j] - 10) == 0)
			{
				a10 = 1;
			}
			else
			{
				float temp = pow((tenth_x[i][j] - 10), 2.0f) / (-3);
				a10 = sin((tenth_x[i][j] - 10)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_x[i][j] - 10);
			}


			if ((tenth_y[i][j] + 1) == 0)
			{
				b_1 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] + 1), 2.0f) / (-3);
				b_1 = sin((tenth_y[i][j] + 1)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] + 1);
			}

			if ((tenth_y[i][j] + 2) == 0)
			{
				b_2 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] + 2), 2.0f) / (-3);
				b_2 = sin((tenth_y[i][j] + 2)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] + 2);
			}

			if ((tenth_y[i][j] + 0) == 0)
			{
				b0 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] + 0), 2.0f) / (-3);
				b0 = sin((tenth_y[i][j] + 0)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] + 0);
			}

			if ((tenth_y[i][j] - 1) == 0)
			{
				b1 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] - 1), 2.0f) / (-3);
				b1 = sin((tenth_y[i][j] - 1)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] - 1);
			}

			if ((tenth_y[i][j] - 2) == 0)
			{
				b2 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] - 2), 2.0f) / (-3);
				b2 = sin((tenth_y[i][j] - 2)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] - 2);
			}

			if ((tenth_y[i][j] - 3) == 0)
			{
				b3 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] - 3), 2.0f) / (-3);
				b3 = sin((tenth_y[i][j] - 3)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] - 3);
			}

			if ((tenth_y[i][j] - 4) == 0)
			{
				b4 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] - 4), 2.0f) / (-3);
				b4 = sin((tenth_y[i][j] - 4)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] - 4);
			}

			if ((tenth_y[i][j] - 5) == 0)
			{
				b5 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] - 5), 2.0f) / (-3);
				b5 = sin((tenth_y[i][j] - 5)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] - 5);
			}

			if ((tenth_y[i][j] - 6) == 0)
			{
				b6 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] - 6), 2.0f) / (-3);
				b6 = sin((tenth_y[i][j] - 6)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] - 6);
			}

			if ((tenth_y[i][j] - 7) == 0)
			{
				b7 = 1;
			}
			else
			{
				float temp = pow((tenth_y[i][j] - 7), 2.0f) / (-3);
				b7 = sin((tenth_y[i][j] - 7)*GL_PI)*pow(xe, temp) / GL_PI / (tenth_y[i][j] - 7);
			}
			g0 = b_2*B36[0][0] + b_1*B36[1][0] + b0*B36[2][0] + b1*B36[3][0] + b2*B36[4][0] + b3*B36[5][0] + b4*B36[6][0] + b5*B36[7][0];
			g1 = b_2*B36[0][1] + b_1*B36[1][1] + b0*B36[2][1] + b1*B36[3][1] + b2*B36[4][1] + b3*B36[5][1] + b4*B36[6][1] + b5*B36[7][1];
			g2 = b_2*B36[0][2] + b_1*B36[1][2] + b0*B36[2][2] + b1*B36[3][2] + b2*B36[4][2] + b3*B36[5][2] + b4*B36[6][2] + b5*B36[7][2];
			g3 = b_2*B36[0][3] + b_1*B36[1][3] + b0*B36[2][3] + b1*B36[3][3] + b2*B36[4][3] + b3*B36[5][3] + b4*B36[6][3] + b5*B36[7][3];
			g4 = b_2*B36[0][4] + b_1*B36[1][4] + b0*B36[2][4] + b1*B36[3][4] + b2*B36[4][4] + b3*B36[5][4] + b4*B36[6][4] + b5*B36[7][4];
			g5 = b_2*B36[0][5] + b_1*B36[1][5] + b0*B36[2][5] + b1*B36[3][5] + b2*B36[4][5] + b3*B36[5][5] + b4*B36[6][5] + b5*B36[7][5];
			g6 = b_2*B36[0][6] + b_1*B36[1][6] + b0*B36[2][6] + b1*B36[3][6] + b2*B36[4][6] + b3*B36[5][6] + b4*B36[6][6] + b5*B36[7][6];
			g7 = b_2*B36[0][7] + b_1*B36[1][7] + b0*B36[2][7] + b1*B36[3][7] + b2*B36[4][7] + b3*B36[5][7] + b4*B36[6][7] + b5*B36[7][7];
			g8 = b_2*B36[0][8] + b_1*B36[1][8] + b0*B36[2][8] + b1*B36[3][8] + b2*B36[4][8] + b3*B36[5][8] + b4*B36[6][8] + b5*B36[7][8];
			g9 = b_2*B36[0][9] + b_1*B36[1][9] + b0*B36[2][9] + b1*B36[3][9] + b2*B36[4][9] + b3*B36[5][9] + b4*B36[6][9] + b5*B36[7][9];
			g10 = b_2*B36[0][10] + b_1*B36[1][10] + b0*B36[2][10] + b1*B36[3][10] + b2*B36[4][10] + b3*B36[5][10] + b4*B36[6][10] + b5*B36[7][10];
			g11 = b_2*B36[0][11] + b_1*B36[1][11] + b0*B36[2][11] + b1*B36[3][11] + b2*B36[4][11] + b3*B36[5][11] + b4*B36[6][11] + b5*B36[7][11];
			g12 = b_2*B36[0][12] + b_1*B36[1][12] + b0*B36[2][12] + b1*B36[3][12] + b2*B36[4][12] + b3*B36[5][12] + b4*B36[6][12] + b5*B36[7][12];


			z = g0*a_2 + g1*a_1 + g2*a0 + g3*a1 + g4*a2 + g5*a3 + g6*a4 + g7*a5 + g8*a6 + g9*a7 + g10*a8 + g11*a9 + g12*a10;
			tenth_z[i][j] = z;

			m0 = b_2*B35[0][0] + b_1*B35[1][0] + b0*B35[2][0] + b1*B35[3][0] + b2*B35[4][0] + b3*B35[5][0] + b4*B35[6][0] + b5*B35[7][0];
			m1 = b_2*B35[0][1] + b_1*B35[1][1] + b0*B35[2][1] + b1*B35[3][1] + b2*B35[4][1] + b3*B35[5][1] + b4*B35[6][1] + b5*B35[7][1];
			m2 = b_2*B35[0][2] + b_1*B35[1][2] + b0*B35[2][2] + b1*B35[3][2] + b2*B35[4][2] + b3*B35[5][2] + b4*B35[6][2] + b5*B35[7][2];
			m3 = b_2*B35[0][3] + b_1*B35[1][3] + b0*B35[2][3] + b1*B35[3][3] + b2*B35[4][3] + b3*B35[5][3] + b4*B35[6][3] + b5*B35[7][3];
			m4 = b_2*B35[0][4] + b_1*B35[1][4] + b0*B35[2][4] + b1*B35[3][4] + b2*B35[4][4] + b3*B35[5][4] + b4*B35[6][4] + b5*B35[7][4];
			m5 = b_2*B35[0][5] + b_1*B35[1][5] + b0*B35[2][5] + b1*B35[3][5] + b2*B35[4][5] + b3*B35[5][5] + b4*B35[6][5] + b5*B35[7][5];
			m6 = b_2*B35[0][6] + b_1*B35[1][6] + b0*B35[2][6] + b1*B35[3][6] + b2*B35[4][6] + b3*B35[5][6] + b4*B35[6][6] + b5*B35[7][6];
			m7 = b_2*B35[0][7] + b_1*B35[1][7] + b0*B35[2][7] + b1*B35[3][7] + b2*B35[4][7] + b3*B35[5][7] + b4*B35[6][7] + b5*B35[7][7];
			m8 = b_2*B35[0][8] + b_1*B35[1][8] + b0*B35[2][8] + b1*B35[3][8] + b2*B35[4][8] + b3*B35[5][8] + b4*B35[6][8] + b5*B35[7][8];
			m9 = b_2*B35[0][9] + b_1*B35[1][9] + b0*B35[2][9] + b1*B35[3][9] + b2*B35[4][9] + b3*B35[5][9] + b4*B35[6][9] + b5*B35[7][9];
			m10 = b_2*B35[0][10] + b_1*B35[1][10] + b0*B35[2][10] + b1*B35[3][10] + b2*B35[4][10] + b3*B35[5][10] + b4*B35[6][10] + b5*B35[7][10];
			m11 = b_2*B35[0][11] + b_1*B35[1][11] + b0*B35[2][11] + b1*B35[3][11] + b2*B35[4][11] + b3*B35[5][11] + b4*B35[6][11] + b5*B35[7][11];
			m12 = b_2*B35[0][12] + b_1*B35[1][12] + b0*B35[2][12] + b1*B35[3][12] + b2*B35[4][12] + b3*B35[5][12] + b4*B35[6][12] + b5*B35[7][12];

			xxxx = m0*a_2 + m1*a_1 + m2*a0 + m3*a1 + m4*a2 + m5*a3 + m6*a4 + m7*a5 + m8*a6 + m9*a7 + m10*a8 + m11*a9 + m12*a10;
			tenth_yy[i][j] = xxxx;

			n0 = b_2*B34[0][0] + b_1*B34[1][0] + b0*B34[2][0] + b1*B34[3][0] + b2*B34[4][0] + b3*B34[5][0] + b4*B34[6][0] + b5*B34[7][0];
			n1 = b_2*B34[0][1] + b_1*B34[1][1] + b0*B34[2][1] + b1*B34[3][1] + b2*B34[4][1] + b3*B34[5][1] + b4*B34[6][1] + b5*B34[7][1];
			n2 = b_2*B34[0][2] + b_1*B34[1][2] + b0*B34[2][2] + b1*B34[3][2] + b2*B34[4][2] + b3*B34[5][2] + b4*B34[6][2] + b5*B34[7][2];
			n3 = b_2*B34[0][3] + b_1*B34[1][3] + b0*B34[2][3] + b1*B34[3][3] + b2*B34[4][3] + b3*B34[5][3] + b4*B34[6][3] + b5*B34[7][3];
			n4 = b_2*B34[0][4] + b_1*B34[1][4] + b0*B34[2][4] + b1*B34[3][4] + b2*B34[4][4] + b3*B34[5][4] + b4*B34[6][4] + b5*B34[7][4];
			n5 = b_2*B34[0][5] + b_1*B34[1][5] + b0*B34[2][5] + b1*B34[3][5] + b2*B34[4][5] + b3*B34[5][5] + b4*B34[6][5] + b5*B34[7][5];
			n6 = b_2*B34[0][6] + b_1*B34[1][6] + b0*B34[2][6] + b1*B34[3][6] + b2*B34[4][6] + b3*B34[5][6] + b4*B34[6][6] + b5*B34[7][6];
			n7 = b_2*B34[0][7] + b_1*B34[1][7] + b0*B34[2][7] + b1*B34[3][7] + b2*B34[4][7] + b3*B34[5][7] + b4*B34[6][7] + b5*B34[7][7];
			n8 = b_2*B34[0][8] + b_1*B34[1][8] + b0*B34[2][8] + b1*B34[3][8] + b2*B34[4][8] + b3*B34[5][8] + b4*B34[6][8] + b5*B34[7][8];
			n9 = b_2*B34[0][9] + b_1*B34[1][9] + b0*B34[2][9] + b1*B34[3][9] + b2*B34[4][9] + b3*B34[5][9] + b4*B34[6][9] + b5*B34[7][9];
			n10 = b_2*B34[0][10] + b_1*B34[1][10] + b0*B34[2][10] + b1*B34[3][10] + b2*B34[4][10] + b3*B34[5][10] + b4*B34[6][10] + b5*B34[7][10];
			n11 = b_2*B34[0][11] + b_1*B34[1][11] + b0*B34[2][11] + b1*B34[3][11] + b2*B34[4][11] + b3*B34[5][11] + b4*B34[6][11] + b5*B34[7][11];
			n12 = b_2*B34[0][12] + b_1*B34[1][12] + b0*B34[2][12] + b1*B34[3][12] + b2*B34[4][12] + b3*B34[5][12] + b4*B34[6][12] + b5*B34[7][12];

			kkkk = n0*a_2 + n1*a_1 + n2*a0 + n3*a1 + n4*a2 + n5*a3 + n6*a4 + n7*a5 + n8*a6 + n9*a7 + n10*a8 + n11*a9 + n12*a10;
			tenth_xx[i][j] = kkkk;
		}
	}

};


int FindSpan(int n, int p, double u, double U[])
{
	int low;
	int high;
	int mid;
	if (u == U[n + 1])
	{
		return(n);
	}
	low = p;
	high = n + 1;
	mid = (low + high) / 2;
	while (u<U[mid] || u >= U[mid + 1])
	{
		if (u<U[mid])
		{
			high = mid;
		}
		else
		{
			low = mid;
		}
		mid = (low + high) / 2;
	}
	return(mid);
}

void BasisFuns(int i, double u, int p, double U[], double N[])
{
	int j;
	int k;
	double saved;
	double temp;
	double left[20];
	double right[20];
	N[0] = 1.0;
	for (j = 1; j <= p; j++)
	{
		left[j] = u - U[i + 1 - j];
		right[j] = U[i + j] - u;
		saved = 0.0;
		for (k = 0; k<j; k++)
		{
			temp = N[k] / (right[k + 1] + left[j - k]);
			N[k] = saved + right[k + 1] * temp;
			saved = left[j - k] * temp;
		}
		N[j] = saved;
	}
}

double C[3];
double S[3];
void CurvePoint(int n, int p, double U[], double P[][3], double u)
{
	int span = FindSpan(n, p, u, U);
	double N[10];
	BasisFuns(span, u, p, U, N);
	C[0] = 0.0;
	C[1] = 0.0;
	C[2] = 0.0;
	for (int i = 0; i <= p; i++)
	{
		C[0] = C[0] + N[i] * P[span - p + i][0];
		C[1] = C[1] + N[i] * P[span - p + i][1];
		C[2] = C[2] + N[i] * P[span - p + i][2];

	}

}

void Human::FillSKMesh(Skeletonchunk *skeleton)
{
	int newindex = 0;
	int i, k, j, count;

	for (i = 0; i<10; i++)
	{
		skeleton[i].numofVert = ((h[i] + 1)*(v[i] + 1));
		skeleton[i].vertex = new Vertex[skeleton[i].numofVert];
		memset(skeleton[i].vertex, 0, sizeof(Vertex) * (skeleton[i].numofVert));

		skeleton[i].numofTri = 2 * (h[i] + 1)*(v[i] + 1);
		skeleton[i].triangle = new Triangle[skeleton[i].numofTri];
		memset(skeleton[i].triangle, 0, sizeof(Triangle) * (skeleton[i].numofTri));
		//添加顶点坐标信息
		if (i == 1)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = first_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = first_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = first_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 2)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = second_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = second_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = second_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 3)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = third_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = third_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = third_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 4)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = fourth_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = fourth_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = fourth_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 5)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = fifth_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = fifth_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = fifth_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 6)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = sixth_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = sixth_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = sixth_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 7)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = seventh_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = seventh_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = seventh_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 8)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = eighth_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = eighth_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = eighth_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 9)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = ninth_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = ninth_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = ninth_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		if (i == 0)
		{
			k = 0;
			for (int m = 0; m<POINTNUMEACHPART + 1; m++)
			{
				for (j = 0; j<POINTNUMEACHPART + 1; j++)
				{
					skeleton[i].vertex[k].Coordinates.x = tenth_xx[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.y = tenth_yy[m][j] * SCALESIZE;
					skeleton[i].vertex[k].Coordinates.z = tenth_z[m][j] * SCALESIZE;
					k++;
					if (k >= (h[i] + 1)*(v[i] + 1))
						break;
				}
			}
		}
		int a, b, c;
		count = 0;
		//f<<"三角面片信息："<<endl;
		for (k = 0; k<h[i]; k++)
		{
			for (j = 0; j<v[i]; j++)
			{

				//添加三角面片count的顶点索引号信息和单位法向量
				a = (k + 1)*(v[i] + 1) + j;
				b = k*(v[i] + 1) + j;
				c = k*(v[i] + 1) + (j + 1);
				skeleton[i].triangle[count].vert[0] = a;
				skeleton[i].triangle[count].vert[1] = b;
				skeleton[i].triangle[count].vert[2] = c;
				TriNormalize_d(skeleton, i, a, b, c, count);
				//为相应的点添加邻接面片的信息
				skeleton[i].vertex[a].AdjFaceCounts++;
				skeleton[i].vertex[a].AdjFacesIndex.push_back(newindex);
				skeleton[i].vertex[a].AdjFacesIndex[skeleton[i].vertex[a].AdjFaceCounts - 1] = count;
				skeleton[i].vertex[b].AdjFaceCounts++;
				skeleton[i].vertex[b].AdjFacesIndex.push_back(newindex);
				skeleton[i].vertex[b].AdjFacesIndex[skeleton[i].vertex[b].AdjFaceCounts - 1] = count;
				skeleton[i].vertex[c].AdjFaceCounts++;
				skeleton[i].vertex[c].AdjFacesIndex.push_back(newindex);
				skeleton[i].vertex[c].AdjFacesIndex[skeleton[i].vertex[c].AdjFaceCounts - 1] = count;
				//添加三角面片count+1的顶点索引号信息
				a = k*(v[i] + 1) + (j + 1);
				b = (k + 1)*(v[i] + 1) + (j + 1);
				c = (k + 1)*(v[i] + 1) + j;
				skeleton[i].triangle[count + 1].vert[0] = a;
				skeleton[i].triangle[count + 1].vert[1] = b;
				skeleton[i].triangle[count + 1].vert[2] = c;
				TriNormalize_d(skeleton, i, a, b, c, count + 1);
				//为相应的点添加邻接面片的信息
				skeleton[i].vertex[a].AdjFaceCounts++;
				skeleton[i].vertex[a].AdjFacesIndex.push_back(newindex);
				skeleton[i].vertex[a].AdjFacesIndex[skeleton[i].vertex[a].AdjFaceCounts - 1] = count + 1;
				skeleton[i].vertex[b].AdjFaceCounts++;
				skeleton[i].vertex[b].AdjFacesIndex.push_back(newindex);
				skeleton[i].vertex[b].AdjFacesIndex[skeleton[i].vertex[b].AdjFaceCounts - 1] = count + 1;
				skeleton[i].vertex[c].AdjFaceCounts++;
				skeleton[i].vertex[c].AdjFacesIndex.push_back(newindex);
				skeleton[i].vertex[c].AdjFacesIndex[skeleton[i].vertex[c].AdjFaceCounts - 1] = count + 1;

				count = count + 2;
			}
		}
		CVector3 sum;
		double area;
		for (k = 0; k<skeleton[i].numofVert; k++)
		{
			sum.Set(0, 0, 0);
			area = 0;
			for (j = 0; j<skeleton[i].vertex[k].AdjFaceCounts; j++)
			{
				sum = sum + skeleton[i].triangle[skeleton[i].vertex[k].AdjFacesIndex[j]].Normalize;//*skeleton[i].triangle[skeleton[i].vertex[k].AdjFacesIndex[j]].area
				area += skeleton[i].triangle[skeleton[i].vertex[k].AdjFacesIndex[j]].area;
			}
			skeleton[i].vertex[k].Normalize = Normalize(sum / area);
			skeleton[i].vertex[k].Convexhull = skeleton[i].vertex[k].Coordinates + skeleton[i].vertex[k].Normalize*0.18;//*1.02
		}
		//为凸起来的人体骨骼三角面片计算其法向量和三个角的角度
		HullTriNormalize_d(skeleton, i);
		CultulateSlope_Degree(skeleton, i);
		//构造有向包围盒
		CreateOBB(skeleton, i);

	}
}


void Human::TriNormalize_d(Skeletonchunk *skeleton, int i, int a, int b, int c, int triindex)
{
	CVector3 v1, v2, vNormal;
	double cos, sin;
	v1 = Vector(skeleton[i].vertex[b].Coordinates, skeleton[i].vertex[a].Coordinates);
	v2 = Vector(skeleton[i].vertex[c].Coordinates, skeleton[i].vertex[b].Coordinates);
	//v2 = Vector(skeleton[i].vertex[b].Coordinates, skeleton[i].vertex[c].Coordinates);
	//计算面积
	cos = Dot(v1, v2) / (Magnitude(v1)*Magnitude(v2));
	sin = sqrt(1 - cos*cos);
	skeleton[i].triangle[triindex].area = Magnitude(v1)*Magnitude(v2)*sin;
	//计算法向量和平面方程常量
	vNormal = Cross(v2, v1);
	/*
	if(i==2||i==5||i==6)vNormal  = Cross(v2, v1);
	else vNormal  = Cross(v1, v2);
	if(i<=6&&i>=3) vNormal=vNormal*(-1);*/
	skeleton[i].triangle[triindex].Normalize = Normalize(vNormal);
	skeleton[i].triangle[triindex].d = Dot(skeleton[i].vertex[a].Coordinates, skeleton[i].triangle[triindex].Normalize)*(-1);
}
void Human::HullTriNormalize_d(Skeletonchunk *skeleton, int i)
{
	CVector3 vl1, vl2;//用于记录左右小臂/（大、小）腿的横截面法向量，进而进行衣服质点的身体部位检测判断
	if (i == 1 || i == 2)
	{
		vl1 = Vector(skeleton[i].vertex[2].Convexhull, skeleton[i].vertex[12].Convexhull);
		/*vl2=Vector(skeleton[i].vertex[17].Convexhull, skeleton[i].vertex[7].Convexhull);*/
		vl2 = Vector(skeleton[i].vertex[7].Convexhull, skeleton[i].vertex[17].Convexhull);
		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
	else
	{
		vl1 = Vector(skeleton[i].vertex[0].Convexhull, skeleton[i].vertex[10].Convexhull);
		vl2 = Vector(skeleton[i].vertex[5].Convexhull, skeleton[i].vertex[15].Convexhull);
	}
	skeleton[i].v = Cross(vl1, vl2);

	CVector3 v1, v2, vNormal;
	int a, b, c;
	double cos, sin;
	for (int k = 0; k<skeleton[i].numofTri; k++){
		a = skeleton[i].triangle[k].vert[0];
		b = skeleton[i].triangle[k].vert[1];
		c = skeleton[i].triangle[k].vert[2];

		v1 = Vector(skeleton[i].vertex[b].Convexhull, skeleton[i].vertex[a].Convexhull);
		v2 = Vector(skeleton[i].vertex[c].Convexhull, skeleton[i].vertex[b].Convexhull);
		//v2 = Vector(skeleton[i].vertex[c].Convexhull, skeleton[i].vertex[b].Convexhull);

		//计算面积
		cos = Dot(v1, v2) / (Magnitude(v1)*Magnitude(v2));
		sin = sqrt(1 - cos*cos);
		skeleton[i].triangle[k].areahull = Magnitude(v1)*Magnitude(v2)*sin;

		//计算法向量和平面方程常量
		vNormal = Cross(v2, v1);
		skeleton[i].triangle[k].Normalizehull = Normalize(vNormal);
		skeleton[i].triangle[k].dhull = Dot(skeleton[i].vertex[b].Convexhull, skeleton[i].triangle[k].Normalizehull)*(-1);
	}
}
void Human::CultulateSlope_Degree(Skeletonchunk *skeleton, int i){
	CVector3 v1, v2, v3, vector1, vector2;
	double cos;
	for (int k = 0; k<skeleton[i].numofTri; k++){
		for (int j = 0; j<3; j++)
			skeleton[i].triangle[k].ismax[j] = false;
		v1 = skeleton[i].vertex[skeleton[i].triangle[k].vert[0]].Convexhull;
		v2 = skeleton[i].vertex[skeleton[i].triangle[k].vert[1]].Convexhull;
		v3 = skeleton[i].vertex[skeleton[i].triangle[k].vert[2]].Convexhull;

		vector1 = v2 - v1;
		vector2 = v3 - v1;
		cos = Dot(vector1, vector2) / (Magnitude(vector1)*Magnitude(vector2));
		skeleton[i].triangle[k].degree012[0] = acos(cos);

		vector1 = v1 - v2;
		vector2 = v3 - v2;
		cos = Dot(vector1, vector2) / (Magnitude(vector1)*Magnitude(vector2));
		skeleton[i].triangle[k].degree012[1] = acos(cos);

		vector1 = v1 - v3;
		vector2 = v2 - v3;
		cos = Dot(vector1, vector2) / (Magnitude(vector1)*Magnitude(vector2));
		skeleton[i].triangle[k].degree012[2] = acos(cos);

	}
}
void Human::CreateOBB(Skeletonchunk *skeleton, int i){
	CVector3 Vx, tempx, Vy, Vz;
	int index1, index2, j, k;
	//计算局部坐标系的中心点
	j = v[i] / 2;
	//对于身体或臀部
	if (i == 1 || i == 2)
	{
		index1 = j*(h[i] + 1) + 2;
		index2 = j*(h[i] + 1) + 12;
	}
	else
	{
		index1 = j*(h[i] + 1);
		index2 = j*(h[i] + 1) + 10;
	}
	skeleton[i].Locenter = (skeleton[i].vertex[index1].Coordinates + skeleton[i].vertex[index2].Coordinates) / 2.0;
	//if(i==2)skeleton[i].Locenter.z+=0.5;

	skeleton[i].Mobb[12] = skeleton[i].Locenter.x;
	skeleton[i].Mobb[13] = skeleton[i].Locenter.y;
	skeleton[i].Mobb[14] = skeleton[i].Locenter.z;
	skeleton[i].Mobb[15] = 1;
	//构造Mobb和其逆矩阵InvertofM，注意将手臂与身体的其他部分区别开来，节省计算时间
	if (i != 1 && i != 2){
		//临时改变以得到正确的局部坐标系，后面再想统一解决方案
		if (i == 7 || i == 9)
			Vy = Normalize(skeleton[i].vertex[index1 + 21 * 1].Coordinates - skeleton[i].vertex[index2].Coordinates);
		else
			if (i == 0)
				Vy = Normalize(skeleton[i].vertex[index1 - 21].Coordinates - skeleton[i].vertex[index2].Coordinates);
			else
				Vy = Normalize(skeleton[i].vertex[index1].Coordinates - skeleton[i].vertex[index2].Coordinates);
		skeleton[i].Mobb[4] = Vy.x;
		skeleton[i].Mobb[5] = Vy.y;
		skeleton[i].Mobb[6] = Vy.z;
		skeleton[i].Mobb[7] = 0;

		tempx = Normalize(skeleton[i].vertex[15].Coordinates - skeleton[i].vertex[5].Coordinates);
		Vz = Normalize(Cross(tempx, Vy));
		skeleton[i].Mobb[8] = Vz.x;
		skeleton[i].Mobb[9] = Vz.y;
		skeleton[i].Mobb[10] = Vz.z;
		skeleton[i].Mobb[11] = 0;

		Vx = Normalize(Cross(Vy, Vz));
		skeleton[i].Mobb[0] = Vx.x;
		skeleton[i].Mobb[1] = Vx.y;
		skeleton[i].Mobb[2] = Vx.z;
		skeleton[i].Mobb[3] = 0;
		//double test=Dot(Vx,Vy);

		m3dInvertMatrix44(skeleton[i].InvertofM, skeleton[i].Mobb);

	}
	else{
		for (k = 0; k<16; k++){
			if (k<12){
				if (k % 5 == 0)skeleton[i].Mobb[k] = 1;
				else skeleton[i].Mobb[k] = 0;

				skeleton[i].InvertofM[k] = skeleton[i].Mobb[k];
			}
			else{
				if (k != 15)skeleton[i].InvertofM[k] = skeleton[i].Mobb[k] * (-1);
				else skeleton[i].InvertofM[k] = 1;
			}
		}
	}

	//为身体的各个部分计算点在局部坐标系中的xyz坐标和柱面rQz坐标
	//CVector3 Max,Min;
	skeleton[i].localVert = new Vertex[skeleton[i].numofVert];
	memset(skeleton[i].localVert, 0, sizeof(Vertex) * (skeleton[i].numofVert));
	skeleton[i].Max.Set(-10000, -10000, -10000);
	skeleton[i].Min.Set(10000, 10000, 10000);
	for (k = 0; k<skeleton[i].numofVert; k++){

		if (i != 1 && i != 2){
			skeleton[i].localVert[k].Convexhull.x = skeleton[i].InvertofM[0] * skeleton[i].vertex[k].Convexhull.x + skeleton[i].InvertofM[4] * skeleton[i].vertex[k].Convexhull.y + skeleton[i].InvertofM[8] * skeleton[i].vertex[k].Convexhull.z + skeleton[i].InvertofM[12];
			skeleton[i].localVert[k].Convexhull.y = skeleton[i].InvertofM[1] * skeleton[i].vertex[k].Convexhull.x + skeleton[i].InvertofM[5] * skeleton[i].vertex[k].Convexhull.y + skeleton[i].InvertofM[9] * skeleton[i].vertex[k].Convexhull.z + skeleton[i].InvertofM[13];
			skeleton[i].localVert[k].Convexhull.z = skeleton[i].InvertofM[2] * skeleton[i].vertex[k].Convexhull.x + skeleton[i].InvertofM[6] * skeleton[i].vertex[k].Convexhull.y + skeleton[i].InvertofM[10] * skeleton[i].vertex[k].Convexhull.z + skeleton[i].InvertofM[14];
			//以下成员属性Coordinate在这里作为柱面坐标,x,y,z分别为r,Q,z.需要注意局部坐标系的方向问题
			skeleton[i].localVert[k].Coordinates.x = sqrt(skeleton[i].localVert[k].Convexhull.y*skeleton[i].localVert[k].Convexhull.y + skeleton[i].localVert[k].Convexhull.z*skeleton[i].localVert[k].Convexhull.z);
			double z, y;
			z = skeleton[i].localVert[k].Convexhull.z;
			y = skeleton[i].localVert[k].Convexhull.y;
			if (z >= 0 && y>0)skeleton[i].localVert[k].Coordinates.y = atan(z / y);
			if (z>0 && y == 0)skeleton[i].localVert[k].Coordinates.y = PI*0.5;
			if (z<0 && y == 0)skeleton[i].localVert[k].Coordinates.y = PI*1.5;
			if (y<0)skeleton[i].localVert[k].Coordinates.y = PI + atan(z / y);
			if (z<0 && y>0)skeleton[i].localVert[k].Coordinates.y = 2 * PI + atan(z / y);
			skeleton[i].localVert[k].Coordinates.z = skeleton[i].localVert[k].Convexhull.x;
		}
		else{
			skeleton[i].localVert[k].Convexhull.x = skeleton[i].vertex[k].Convexhull.x + skeleton[i].InvertofM[12];
			skeleton[i].localVert[k].Convexhull.y = skeleton[i].vertex[k].Convexhull.y + skeleton[i].InvertofM[13];
			skeleton[i].localVert[k].Convexhull.z = skeleton[i].vertex[k].Convexhull.z + skeleton[i].InvertofM[14];
			//以下成员属性Coordinate在这里作为柱面坐标,x,y,z分别为r,Q,z.需要注意局部坐标系的方向问题
			skeleton[i].localVert[k].Coordinates.x = sqrt(skeleton[i].localVert[k].Convexhull.x*skeleton[i].localVert[k].Convexhull.x + skeleton[i].localVert[k].Convexhull.z*skeleton[i].localVert[k].Convexhull.z);
			double z, x;
			z = skeleton[i].localVert[k].Convexhull.z;
			x = skeleton[i].localVert[k].Convexhull.x;
			if (z >= 0 && x>0)skeleton[i].localVert[k].Coordinates.y = atan(z / x);
			if (z>0 && x == 0)skeleton[i].localVert[k].Coordinates.y = PI*0.5;
			if (z<0 && x == 0)skeleton[i].localVert[k].Coordinates.y = PI*1.5;
			if (x<0)skeleton[i].localVert[k].Coordinates.y = PI + atan(z / x);
			if (z<0 && x>0)skeleton[i].localVert[k].Coordinates.y = 2 * PI + atan(z / x);
			skeleton[i].localVert[k].Coordinates.z = skeleton[i].localVert[k].Convexhull.y;
		}
		//用于构造局部有向包围盒AABB

		if (skeleton[i].localVert[k].Convexhull.x>skeleton[i].Max.x)skeleton[i].Max.x = skeleton[i].localVert[k].Convexhull.x;
		if (skeleton[i].localVert[k].Convexhull.x<skeleton[i].Min.x)skeleton[i].Min.x = skeleton[i].localVert[k].Convexhull.x;

		if (skeleton[i].localVert[k].Convexhull.y>skeleton[i].Max.y)skeleton[i].Max.y = skeleton[i].localVert[k].Convexhull.y;
		if (skeleton[i].localVert[k].Convexhull.y<skeleton[i].Min.y)skeleton[i].Min.y = skeleton[i].localVert[k].Convexhull.y;

		if (skeleton[i].localVert[k].Convexhull.z>skeleton[i].Max.z)skeleton[i].Max.z = skeleton[i].localVert[k].Convexhull.z;
		if (skeleton[i].localVert[k].Convexhull.z<skeleton[i].Min.z)skeleton[i].Min.z = skeleton[i].localVert[k].Convexhull.z;
	}
	//构造在全局坐标系中的有向包围盒OBB
	CVector3 APoint[6];
	APoint[0].x = skeleton[i].Min.x; APoint[0].z = skeleton[i].Min.z; APoint[0].y = skeleton[i].Max.y;
	APoint[1].x = skeleton[i].Min.x; APoint[1].z = skeleton[i].Max.z; APoint[1].y = skeleton[i].Max.y;
	APoint[2].x = skeleton[i].Min.x; APoint[2].z = skeleton[i].Max.z; APoint[2].y = skeleton[i].Min.y;
	APoint[3].x = skeleton[i].Max.x; APoint[3].z = skeleton[i].Max.z; APoint[3].y = skeleton[i].Min.y;
	APoint[4].x = skeleton[i].Max.x; APoint[4].z = skeleton[i].Min.z; APoint[4].y = skeleton[i].Min.y;
	APoint[5].x = skeleton[i].Max.x; APoint[5].z = skeleton[i].Min.z; APoint[5].y = skeleton[i].Max.y;
	skeleton[i].AnglePoint[7].x = skeleton[i].Mobb[0] * skeleton[i].Max.x + skeleton[i].Mobb[4] * skeleton[i].Max.y + skeleton[i].Mobb[8] * skeleton[i].Max.z + skeleton[i].Mobb[12];
	skeleton[i].AnglePoint[7].y = skeleton[i].Mobb[1] * skeleton[i].Max.x + skeleton[i].Mobb[5] * skeleton[i].Max.y + skeleton[i].Mobb[9] * skeleton[i].Max.z + skeleton[i].Mobb[13];
	skeleton[i].AnglePoint[7].z = skeleton[i].Mobb[2] * skeleton[i].Max.x + skeleton[i].Mobb[6] * skeleton[i].Max.y + skeleton[i].Mobb[10] * skeleton[i].Max.z + skeleton[i].Mobb[14];
	skeleton[i].AnglePoint[0].x = skeleton[i].Mobb[0] * skeleton[i].Min.x + skeleton[i].Mobb[4] * skeleton[i].Min.y + skeleton[i].Mobb[8] * skeleton[i].Min.z + skeleton[i].Mobb[12];
	skeleton[i].AnglePoint[0].y = skeleton[i].Mobb[1] * skeleton[i].Min.x + skeleton[i].Mobb[5] * skeleton[i].Min.y + skeleton[i].Mobb[9] * skeleton[i].Min.z + skeleton[i].Mobb[13];
	skeleton[i].AnglePoint[0].z = skeleton[i].Mobb[2] * skeleton[i].Min.x + skeleton[i].Mobb[6] * skeleton[i].Min.y + skeleton[i].Mobb[10] * skeleton[i].Min.z + skeleton[i].Mobb[14];
	for (j = 0; j<6; j++){
		skeleton[i].AnglePoint[j + 1].x = skeleton[i].Mobb[0] * APoint[j].x + skeleton[i].Mobb[4] * APoint[j].y + skeleton[i].Mobb[8] * APoint[j].z + skeleton[i].Mobb[12];
		skeleton[i].AnglePoint[j + 1].y = skeleton[i].Mobb[1] * APoint[j].x + skeleton[i].Mobb[5] * APoint[j].y + skeleton[i].Mobb[9] * APoint[j].z + skeleton[i].Mobb[13];
		skeleton[i].AnglePoint[j + 1].z = skeleton[i].Mobb[2] * APoint[j].x + skeleton[i].Mobb[6] * APoint[j].y + skeleton[i].Mobb[10] * APoint[j].z + skeleton[i].Mobb[14];
	}
	skeleton[i].Obbface[0].vert[0] = 1; skeleton[i].Obbface[0].vert[1] = 2; skeleton[i].Obbface[0].vert[2] = 6;
	skeleton[i].Obbface[1].vert[0] = 2; skeleton[i].Obbface[1].vert[1] = 3; skeleton[i].Obbface[1].vert[2] = 7;
	skeleton[i].Obbface[2].vert[0] = 3; skeleton[i].Obbface[2].vert[1] = 0; skeleton[i].Obbface[2].vert[2] = 4;
	skeleton[i].Obbface[3].vert[0] = 0; skeleton[i].Obbface[3].vert[1] = 1; skeleton[i].Obbface[3].vert[2] = 5;
	skeleton[i].Obbface[4].vert[0] = 0; skeleton[i].Obbface[4].vert[1] = 3; skeleton[i].Obbface[4].vert[2] = 1;
	skeleton[i].Obbface[5].vert[0] = 7; skeleton[i].Obbface[5].vert[1] = 4; skeleton[i].Obbface[5].vert[2] = 6;
	for (j = 0; j<6; j++){
		int p1, p2, p3;
		p1 = skeleton[i].Obbface[j].vert[0];
		p2 = skeleton[i].Obbface[j].vert[1];
		p3 = skeleton[i].Obbface[j].vert[2];
		APoint[0] = skeleton[i].AnglePoint[p1] - skeleton[i].AnglePoint[p2];
		APoint[1] = skeleton[i].AnglePoint[p1] - skeleton[i].AnglePoint[p3];
		APoint[2] = Cross(APoint[0], APoint[1]);
		skeleton[i].Obbface[j].Normalize = Normalize(APoint[2]);
		skeleton[i].Obbface[j].d = Dot(skeleton[i].AnglePoint[p1], skeleton[i].Obbface[j].Normalize)*(-1);
		if (j == 1)skeleton[i].vseN = APoint[2];
		if (j == 2)skeleton[i].vyzN = APoint[2];//皆指向右	
	}
	//构造柱面映射列表
	skeleton[i].numofCymap = 21 * 21;
	skeleton[i].dQ = (double)((2 * PI) / 21);
	if (i != 1 && i != 2){
		skeleton[i].z_min_max[0] = skeleton[i].Min.x;
		skeleton[i].z_min_max[1] = skeleton[i].Max.x;
	}
	else {
		skeleton[i].z_min_max[0] = skeleton[i].Min.y;
		skeleton[i].z_min_max[1] = skeleton[i].Max.y;
	}
	skeleton[i].dz = (skeleton[i].z_min_max[1] - skeleton[i].z_min_max[0]) / (double)(21);

	skeleton[i].Cylindricalmap = new triMapList[skeleton[i].numofCymap];
	memset(skeleton[i].Cylindricalmap, 0, sizeof(triMapList) * (skeleton[i].numofCymap));

	//对每个三角面片进行搜索，判断其属于柱面的哪几个区域
	int p, m, n, index, newindex = 0;
	double minQ, maxQ, minz, maxz;
	int baseminz, basemaxz, baseminQ, basemaxQ, temp;
	for (k = 0; k<skeleton[i].numofTri; k++){
		minQ = 8; maxQ = -1; minz = 10000; maxz = -10000;
		for (j = 0; j<3; j++){
			p = skeleton[i].triangle[k].vert[j];
			if (skeleton[i].localVert[p].Coordinates.y>maxQ)maxQ = skeleton[i].localVert[p].Coordinates.y;
			if (skeleton[i].localVert[p].Coordinates.y<minQ)minQ = skeleton[i].localVert[p].Coordinates.y;
			if (skeleton[i].localVert[p].Coordinates.z>maxz)maxz = skeleton[i].localVert[p].Coordinates.z;
			if (skeleton[i].localVert[p].Coordinates.z<minz)minz = skeleton[i].localVert[p].Coordinates.z;
		}
		//对于柱面子区域，由上向下，由前向后记录，因而要将minz与maxz进行调整

		minz -= skeleton[i].z_min_max[1];
		maxz -= skeleton[i].z_min_max[1];
		baseminz = (int)(-maxz / skeleton[i].dz);
		basemaxz = (int)(-minz / skeleton[i].dz);
		if (basemaxz == 21)basemaxz = 21 - 1;
		baseminQ = (int)(minQ / skeleton[i].dQ);
		basemaxQ = (int)(maxQ / skeleton[i].dQ);
		for (m = baseminz; m <= basemaxz; m++){
			temp = m * 21;
			if (maxQ - minQ>PI){
				for (n = 0; n <= baseminQ; n++){
					index = temp + n;
					skeleton[i].Cylindricalmap[index].numofTri++;
					skeleton[i].Cylindricalmap[index].Triindex.push_back(newindex);
					skeleton[i].Cylindricalmap[index].Triindex[skeleton[i].Cylindricalmap[index].numofTri - 1] = k;
				}
				int QQ;
				QQ = 21;
				for (n = basemaxQ; n<QQ; n++){
					index = temp + n;
					skeleton[i].Cylindricalmap[index].numofTri++;
					skeleton[i].Cylindricalmap[index].Triindex.push_back(newindex);
					skeleton[i].Cylindricalmap[index].Triindex[skeleton[i].Cylindricalmap[index].numofTri - 1] = k;
				}
			}
			else{
				for (n = baseminQ; n <= basemaxQ; n++){
					index = temp + n;
					skeleton[i].Cylindricalmap[index].numofTri++;
					skeleton[i].Cylindricalmap[index].Triindex.push_back(newindex);
					skeleton[i].Cylindricalmap[index].Triindex[skeleton[i].Cylindricalmap[index].numofTri - 1] = k;
				}
			}
		}
	}
}


/*将手臂调整到需要的位置
leftOrRight 左侧 = -1， 右侧 = 1
upOrDown 上臂 = 1， 小臂 = -1
newPosition 新的坐标位置
*/
void Human::SetArmToNewPosition(int leftOrRight, int upOrDown, float newPosition[3])
{
	if (leftOrRight == -1 && upOrDown == 1) {	//左上臂
		Elbow_point[0] = -newPosition[0]; //这里是对称的，左上臂动了则右臂也动
		Elbow_point[1] = newPosition[1];
		Elbow_point[2] = newPosition[2];
		L_Elbow_point[0] = newPosition[0];
		L_Elbow_point[1] = newPosition[1];
		L_Elbow_point[2] = newPosition[2];
		Elbow_point_2[0] = 53.4f;
		Elbow_point_2[1] = 35.5f;
		Elbow_point_2[2] = 4.5f;
		L_Elbow_point_2[0] = -53.4f;
		L_Elbow_point_2[1] = 35.5f;
		L_Elbow_point_2[2] = 4.5f;
	}
	else if (leftOrRight == -1 && upOrDown == -1) {	//左小臂
		Elbow_point_2[0] = -newPosition[0];
		Elbow_point_2[1] = newPosition[1];
		Elbow_point_2[2] = newPosition[2];
		L_Elbow_point_2[0] = newPosition[0];
		L_Elbow_point_2[1] = newPosition[1];
		L_Elbow_point_2[2] = newPosition[2];
	}
	else if (leftOrRight == 1 && upOrDown == 1) {	//右上臂
		Elbow_point[0] = newPosition[0];
		Elbow_point[1] = newPosition[1];
		Elbow_point[2] = newPosition[2];
		Elbow_point_2[0] = 53.4f;
		Elbow_point_2[1] = 35.5f;
		Elbow_point_2[2] = 4.5f;
		L_Elbow_point_2[0] = -53.4f;
		L_Elbow_point_2[1] = 35.5f;
		L_Elbow_point_2[2] = 4.5f;
	}
	else if (leftOrRight == 1 && upOrDown == -1) {	//右小臂
		Elbow_point_2[0] = newPosition[0];
		Elbow_point_2[1] = newPosition[1];
		Elbow_point_2[2] = newPosition[2];
		L_Elbow_point_2[0] = -newPosition[0];
		L_Elbow_point_2[1] = newPosition[1];
		L_Elbow_point_2[2] = newPosition[2];
	}
	Calculation();
}


/*将腿调整到需要的位置
leftOrRight 左侧 = -1， 右侧 = 1
upOrDown 大腿 = 1， 小腿 = -1
newPosition 新的坐标位置
*/
void Human::SetLegToNewPosition(int leftOrRight, int upOrDown, float newPosition[3]) {
	if (leftOrRight == -1 && upOrDown == 1) {	//左大腿
		calf_point[0] = -newPosition[0];
		calf_point[1] = newPosition[1];
		calf_point[2] = newPosition[2];
		L_calf_point[0] = -newPosition[0];
		L_calf_point[1] = newPosition[1];
		L_calf_point[2] = newPosition[2];


		//calf_point[0] = 9.5f;
		//calf_point[1] = -36.1f;
		//calf_point[2] = -1.0f;
		//L_calf_point[0] =9.5f;
		//L_calf_point[1] = -36.1f;
		//L_calf_point[2] = -1.0f;


		//calf_point_2[0] = 11.5f;
		//calf_point_2[1] = -74.1f;
		//calf_point_2[2] = -6.0f;
		//L_calf_point_2[0] = 11.5f;
		//L_calf_point_2[1] = -74.1f;
		//L_calf_point_2[2] = -4.0f;
		calf_point_2[0] = -newPosition[0] + 2.0f;
		calf_point_2[0] = newPosition[0] - 38.0f;
		calf_point_2[0] = newPosition[0] - 5.0f;
		L_calf_point_2[0] = newPosition[0] + 2.0f;
		L_calf_point_2[1] = newPosition[1] - 38.0f;
		L_calf_point_2[2] = newPosition[2] - 3.0f;

	}
	else if (leftOrRight == -1 && upOrDown == -1) {	//左小腿

	}
	else if (leftOrRight == 1 && upOrDown == 1) {	//右大腿

	}
	else if (leftOrRight == 1 && upOrDown == -1) {	//右小腿

	}
	Calculation();
}