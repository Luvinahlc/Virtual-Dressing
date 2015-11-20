#include "3dmath.h"
#include <vector>

using namespace std;
#define POINTNUMEACHPART 20.0
#define SCALESIZE 0.17
#define EPS 10e-6
class Human
{
public:
	struct Vertex{                 //顶点数据结构
		CVector3 Normalize;        //该点的单位法向量
		CVector3  Coordinates;     //该点的坐标
		CVector3  Convexhull;      //凸包点的坐标
		int AdjFaceCounts;         //邻接面的个数
		vector<int> AdjFacesIndex; //该点的邻接面的索引号
	};

	struct Triangle{              //三角面片的数据结构
		int vert[3];              //三角形的顶点序号
		CVector3 Normalize;       //三角形的单位法向量
		CVector3 Anothervetex[4]; //分别记录该三角形的三边中点和重心坐标
		double d;                  //d表示该三角面所在平面的平面方程常数
		double area;               //三角形的面积
		CVector3 Normalizehull;      //该三角形的扩张法向量
		double dhull;                  //该三角面的扩张所在平面的平面方程常数
		double areahull;          // 该三角面的扩张的面积

		double slope_xy012[3];    //该三角形在xy平面上的投影三角面片的三条边的斜率
		bool ismax[3];
		double degree012[3];     //该三角面片的三个角的角度
	};
	struct triMapList{          //圆柱映射表的基本表示
		int numofTri;			//一个经纬区域内的三角面片的个数
		vector<int> Triindex;	//该区域的三角面片索引号
	};
	struct Skeletonchunk
	{        //人体的一个组织块结构
		int numofVert;
		int numofTri;
		Vertex* vertex;
		Triangle* triangle;
		Vertex* localVert;		 //局部坐标系的点信息
		M3DMatrix44f Mobb;		 //全局坐标系到局部坐标系的转换矩阵
		M3DMatrix44f InvertofM;  //M的逆矩阵
		CVector3 Locenter;		 //局部坐标系的中心点
		CVector3 AnglePoint[8];  //有向包围盒的8个角点
		Triangle Obbface[6];     //有向包围盒的长轴方向上的6个面
		int numofCymap;             //圆柱映射基元个数
		triMapList* Cylindricalmap; //圆柱映射表
		double dQ;              //柱面Q的步长
		double dz;              //柱面z的步长
		double z_min_max[2];

		CVector3 v;          //即记录左右小臂的上横截面的法向量;
		CVector3 prev;       //用于人体运动时保存身体各部位的横截面的前一次的法向量;
		CVector3 vseN;       //记录腿部xy侧截面的法向量
		CVector3 vyzN;       //记录腿部yz侧截面的法向量
		CVector3 Min;
		CVector3 Max;
	};

public:
	float nRange;
	float nL;
	float nR;
	float nT;
	float nD;

	bool gender; // false == male, true == female;

public:
	void Init(bool gender_new);
	void SetGender(bool gender_new);
	void CalculationXZD_female();

public:
	int FindSpan(int n, int p, double u, double U[]);//二分查找，查找u
	void BasisFuns(int i, double u, int p, double U[], double N[]);
	void CurvePoint(int n, int p, double U[], double P[][3], double u);
	void SurfacePoint(int n, int p, double U[], int m, int q, double V[], double P[][6][3], double u, double v);
	void SurfacePointgai(int n, int p, double U[], int m, int q, double V[], double P[][15][3], double u, double v);
	void SurfacePointgainew(int n, int p, double U[], int m, int q, double V[], double P[][9][3], double u, double v);
	void Calculation();
	void CalculationXZD();
	void FillSKMesh(Skeletonchunk *skeleton); // 为 skeleton添加信息。根据tempchunk[10]中的点信心，生成人体网格，以及OBB和圆柱映射表等
	void TriNormalize_d(Skeletonchunk *skeleton, int i, int a, int b, int c, int triindex);  //为skeleton[i]中的顶点索引号为a,b,c的三角面片计算单位法向量和平面常数以及A和InvertofA
	void HullTriNormalize_d(Skeletonchunk *skeleton, int i);//计算skeleton[i]中序号为triindex的三角面片的单位法向量和平面常数以及A和InvertofA
	void CultulateSlope_Degree(Skeletonchunk *skeleton, int i);//计算三角面片的slpoe[]和degree[]
	void VetexQ(Skeletonchunk *skeleton, int i, int index, M3DVector4d Q[]);
	void CreateOBB(Skeletonchunk *skeleton, int i);//创建OBB包围盒

	void SetArmToNewPosition(int leftOrRight, int upOrDown, float newPosition[3]);
	void SetLegToNewPosition(int leftOrRight, int upOrDown, float newPosition[3]);

	void swap(M3DVector3d A[], int t1, int t2){
		double b[3];
		int i;
		for (i = 0; i<3; i++){
			b[i] = A[t1][i];
			A[t1][i] = A[t2][i];
			A[t2][i] = b[i];
		}
	}
	void  mul(M3DVector3d A[], int t1, int t2, double k){
		int i;
		for (i = 0; i<3; i++)A[t1][i] -= (A[t2][i] * k);
	}

	double m3dVectorMulMatrix44(M3DVector4d v, M3DVector4d M[]){
		double sum = 0, s[4] = { 0, 0, 0, 0 };
		int i, j;

		for (i = 0; i<4; i++){
			for (j = 0; j<4; j++)
				s[i] += v[j] * M[j][i];
			sum += s[i] * v[i];
		}
		return sum;
	}

	bool InvertA(M3DVector3d A[], M3DVector3d InertofA[]){
		int i, j, count = 0;
		bool isexist = true;
		M3DVector3d E[3];
		E[0][0] = E[1][1] = E[2][2] = 1;
		E[0][1] = E[0][2] = E[1][0] = E[1][2] = E[2][0] = E[2][1] = 0;
		//化为上三角
		for (i = 0; i<3; i++){
			for (j = i + 1; j<3; j++){
				if (A[0][0] != 0)
					break;
				else{
					swap(A, i, j);
					swap(E, i, j);
					count++;
					if (count == 2 && A[0][0] == 0){
						isexist = false;
						return isexist;
					}
				}
			}
			for (j = i + 1; j<3; j++){
				double t = A[j][i] / A[i][i];
				mul(A, j, i, t);
				mul(E, j, i, t);
			}
		}
		//化为对角阵
		for (i = 2; i>0; i--){
			for (j = i - 1; j >= 0; j--){
				double t = A[j][i] / A[i][i];
				mul(A, j, i, t);
				mul(E, j, i, t);
			}
		}
		//化为单位矩阵
		for (i = 0; i<3; i++)
			for (j = 0; j<3; j++){
			if (A[i][i] == 0)isexist = false;
			else{
				E[i][j] /= A[i][i];
				InertofA[i][j] = E[i][j];
			}
			}
		return isexist;
	}

	int min_eg(M3DVector3d eg[], int length){//获得eg中的最小值的索引号
		int i, k = 0;
		while (k<length&&eg[k][1] != 1)k++;
		for (i = k + 1; i<length; i++){
			if (eg[i][1] == 1 && eg[i][0]<eg[k][0])k = i;
		}
		if (k == 400)return -1;
		else return k;
	}

	int partion(M3DVector3d*eg, int low, int high){
		int k = 0;
		double p = 0;
		double key = eg[low][0];
		while (low<high){
			while ((low<high&&eg[high][0] >= key) || eg[high][2] == 0)--high;
			p = eg[low][0];
			k = eg[low][1];
			eg[low][0] = eg[high][0];
			eg[low][1] = eg[high][1];
			eg[high][0] = p;
			eg[high][1] = k;

			while ((low<high&&eg[low][0] <= key) || eg[low][2] == 0)++low;
			p = eg[low][0];
			k = eg[low][1];
			eg[low][0] = eg[high][0];
			eg[low][1] = eg[high][1];
			eg[high][0] = p;
			eg[high][1] = k;
		}
		return low;
	}

	void quicksort(M3DVector3d*eg, int low, int high){//快速排序算法
		if (low<high){
			int key = partion(eg, low, high);
			quicksort(eg, low, key - 1);
			quicksort(eg, key + 1, high);
		}
	}


};
