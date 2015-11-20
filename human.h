#include "3dmath.h"
#include <vector>

using namespace std;
#define POINTNUMEACHPART 20.0
#define SCALESIZE 0.17
#define EPS 10e-6
class Human
{
public:
	struct Vertex{                 //�������ݽṹ
		CVector3 Normalize;        //�õ�ĵ�λ������
		CVector3  Coordinates;     //�õ������
		CVector3  Convexhull;      //͹���������
		int AdjFaceCounts;         //�ڽ���ĸ���
		vector<int> AdjFacesIndex; //�õ���ڽ����������
	};

	struct Triangle{              //������Ƭ�����ݽṹ
		int vert[3];              //�����εĶ������
		CVector3 Normalize;       //�����εĵ�λ������
		CVector3 Anothervetex[4]; //�ֱ��¼�������ε������е����������
		double d;                  //d��ʾ������������ƽ���ƽ�淽�̳���
		double area;               //�����ε����
		CVector3 Normalizehull;      //�������ε����ŷ�����
		double dhull;                  //�����������������ƽ���ƽ�淽�̳���
		double areahull;          // ������������ŵ����

		double slope_xy012[3];    //����������xyƽ���ϵ�ͶӰ������Ƭ�������ߵ�б��
		bool ismax[3];
		double degree012[3];     //��������Ƭ�������ǵĽǶ�
	};
	struct triMapList{          //Բ��ӳ���Ļ�����ʾ
		int numofTri;			//һ����γ�����ڵ�������Ƭ�ĸ���
		vector<int> Triindex;	//�������������Ƭ������
	};
	struct Skeletonchunk
	{        //�����һ����֯��ṹ
		int numofVert;
		int numofTri;
		Vertex* vertex;
		Triangle* triangle;
		Vertex* localVert;		 //�ֲ�����ϵ�ĵ���Ϣ
		M3DMatrix44f Mobb;		 //ȫ������ϵ���ֲ�����ϵ��ת������
		M3DMatrix44f InvertofM;  //M�������
		CVector3 Locenter;		 //�ֲ�����ϵ�����ĵ�
		CVector3 AnglePoint[8];  //�����Χ�е�8���ǵ�
		Triangle Obbface[6];     //�����Χ�еĳ��᷽���ϵ�6����
		int numofCymap;             //Բ��ӳ���Ԫ����
		triMapList* Cylindricalmap; //Բ��ӳ���
		double dQ;              //����Q�Ĳ���
		double dz;              //����z�Ĳ���
		double z_min_max[2];

		CVector3 v;          //����¼����С�۵��Ϻ����ķ�����;
		CVector3 prev;       //���������˶�ʱ�����������λ�ĺ�����ǰһ�εķ�����;
		CVector3 vseN;       //��¼�Ȳ�xy�����ķ�����
		CVector3 vyzN;       //��¼�Ȳ�yz�����ķ�����
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
	int FindSpan(int n, int p, double u, double U[]);//���ֲ��ң�����u
	void BasisFuns(int i, double u, int p, double U[], double N[]);
	void CurvePoint(int n, int p, double U[], double P[][3], double u);
	void SurfacePoint(int n, int p, double U[], int m, int q, double V[], double P[][6][3], double u, double v);
	void SurfacePointgai(int n, int p, double U[], int m, int q, double V[], double P[][15][3], double u, double v);
	void SurfacePointgainew(int n, int p, double U[], int m, int q, double V[], double P[][9][3], double u, double v);
	void Calculation();
	void CalculationXZD();
	void FillSKMesh(Skeletonchunk *skeleton); // Ϊ skeleton�����Ϣ������tempchunk[10]�еĵ����ģ��������������Լ�OBB��Բ��ӳ����
	void TriNormalize_d(Skeletonchunk *skeleton, int i, int a, int b, int c, int triindex);  //Ϊskeleton[i]�еĶ���������Ϊa,b,c��������Ƭ���㵥λ��������ƽ�泣���Լ�A��InvertofA
	void HullTriNormalize_d(Skeletonchunk *skeleton, int i);//����skeleton[i]�����Ϊtriindex��������Ƭ�ĵ�λ��������ƽ�泣���Լ�A��InvertofA
	void CultulateSlope_Degree(Skeletonchunk *skeleton, int i);//����������Ƭ��slpoe[]��degree[]
	void VetexQ(Skeletonchunk *skeleton, int i, int index, M3DVector4d Q[]);
	void CreateOBB(Skeletonchunk *skeleton, int i);//����OBB��Χ��

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
		//��Ϊ������
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
		//��Ϊ�Խ���
		for (i = 2; i>0; i--){
			for (j = i - 1; j >= 0; j--){
				double t = A[j][i] / A[i][i];
				mul(A, j, i, t);
				mul(E, j, i, t);
			}
		}
		//��Ϊ��λ����
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

	int min_eg(M3DVector3d eg[], int length){//���eg�е���Сֵ��������
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

	void quicksort(M3DVector3d*eg, int low, int high){//���������㷨
		if (low<high){
			int key = partion(eg, low, high);
			quicksort(eg, low, key - 1);
			quicksort(eg, key + 1, high);
		}
	}


};
