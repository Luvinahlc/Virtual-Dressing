
#include"Load3DS.h"
#include"human.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;
enum tCollisionTypes		 //碰撞的枚举类型
{
	NOT_COLLIDING,			 //没有碰撞
	PENETRATING,			 //穿透
	COLLIDING				 //碰撞
};

enum tIntegratorTypes		 //积分器的枚举类型
{
	EULER_INTEGRATOR,		 //欧拉积分器
	MIDPOINT_INTEGRATOR,	 //中点积分器
	RK4_INTEGRATOR			 //四阶龙格_库塔积分器
};


enum tSpringTypes			 //弹簧的枚举类型
{
	MANUAL_SPRING,			 //手动弹簧
	STRUCTURAL_SPRING,		 //结构弹簧
	SHEAR_SPRING,			 //剪切弹簧
	BEND_SPRING				 //弯曲弹簧
};
struct Particle {                 //质点的物理结构
	double	fMass;                //质点质量
	double	fInvMass;             //质点质量倒数
	int     Cylindex;             //该质点在局部坐标系圆柱映射表的索引号 
	CVector3	vprePosition;     //质点前一时刻的位置
	CVector3	vPosition;        //质点当前位置
	CVector3    vPositionCopy;
	CVector3    vprojection;      //该点在局部坐标系圆柱中心轴上投影点的全局坐标
//	CVector3    vPcollision;      //该质点与人体的碰撞点位置
	CVector3    plocal_xyz;       //用于存储质点的局部xyz坐标
	CVector3    plocal_rQz;       //用于存储质点的局部rQz坐标
	CVector3	vVelocity;        //质点速度
	CVector3	vAcceleration;    //质点加速度
	CVector3	vForces;          //质点所受的合力
	CVector3    vNormal;
	CVector3    vTempPC;
	//CVector3    vNormal;          //质点的法向量
	bool	bLocked;              //质点是否固定的标记
	bool    isbrief;              //该质点是否是简化质点
	bool    iscollision;          //该质点是否产生碰撞

	bool	isSelfCollision;	  //该质点是否产生自碰撞

	bool    isrotation;           //该质点是否旋转
	int belongtoPart;         //标记衣服上的该点属于人体的哪个部位
};


struct	Spring {            //弹簧结构
	int 	index1;			//与弹簧一端连接的质点索引号
	int		index2;			//与弹簧另一端连接的质点索引号
	double	Ks;				//弹簧拉伸常数
	double	Kd;				//弹簧阻尼因子	
	double	restLen;		//弹簧自然长度
	double	curLen;		    //弹簧当前长度
	double   maxLen;         //弹簧最大长度
	int	type;		    //弹簧类型
};

struct	Collision {	            //碰撞结构
	int		  particleIndex;    //碰撞质点索引号
	CVector3  normal;		    //碰撞质点的法向量
};


/*// TYPE FOR A PLANE THAT THE SYSTEM WILL COLLIDE WITH
struct tCollisionPlane
{
	CVector3 normal;			// inward pointing
    double	d;				// ax + by + cz + d = 0
};


// TYPE FOR CONTACTS THAT ARE FOUND DURING SIM
struct tContact
{
	int		particle;	// Particle Index
    CVector3 normal;		// Normal of Collision plane
};*/
struct Datapart{     //用于网格重构时的数据对记录
	int a; //质点的原索引号
	int b; //该质点复制后的索引号
};
struct one_Physic{          //一个对象的物理信息
	double mass;             //该对象的质量
	int particlenumbers;    //该物理对象质点个数
	int pOriginnumbers;		//该物理对象初始质点个数
	int pCopynumbers;		 //该物理对象复制的质点个数
	int collisionnumbers;   //该物理对象碰撞质点个数
	int springnumbers;      //该物理对象的弹簧个数
    Particle *pParticles;   //该物理对象的所有质点信息
	Collision *pCollisions; //该物理对象的所有碰撞质点信息
	vector<Spring> pSprings; //该物理对象的所有弹簧信息
	Datapart* datacouple[10];//包含10个Datapart指针的数组，其中，右膝盖处对应数组6
};

struct all_Physics{       //整个场景的所有对象信息
	int objectnumbers;    //整个场景的对象个数
	one_Physic* physics;  //整个场景的所有对象物理信息
};
///*-------以下为人体运动相关的数据结构-----------*/
struct direction_angle{   //人体部位运动时的方向与角度
	float angle;       //角度
	float x;           //旋转方向
	float y;
	float z;          
};
struct PiMotion{    //人体部位运动的方向和角度记录数据结构
	bool ismotion;   //该部位是否运动
	direction_angle *d_a; //记录运动的方向和角度
	bool isfirstSet; //是否第一次设置
};
/*--------------------------------------------------*/
class CMSModel{
public:	

    CMSModel();          //构造函数
	~CMSModel();        //析构函数 
	
	//void    AddExtroInfoTri()
	void    surePP(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk);//确定衣服上的每个质点所属人体的部位，进行标记
	void    Initialize(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,int key);      //初始化成员变量pParticle
	void    CreateSpring(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel);    //为pParticle中的质点创建弹簧
	void	ClothAnimation(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,int parti,float angle,float x,float y,float z,const M3DMatrix44f matrix0,const M3DMatrix44f matrix1,const M3DMatrix44f matrix2);
	void	Animation(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,int i,PiMotion*pi_m);
	void	CalcForces(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk, PiMotion*pi_m);			                   //计算thisphysic中每个质点所受的合力
	void	Verlet(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk,double dt);
    void	StepSimulation(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk, PiMotion*pi_m,double dt);	                   //计算步长dt之后的模型
    void	SetWindVector(double x, double y, double z);	    //设置风向
	void    CollisionCheck(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk);
	void    SingleCheck(Particle *particle,Human::Skeletonchunk *sk_chunk);
	void    CheckSprings(all_Physics *thisphysic);  //根据质点索引号寻找thisphysic中第physicindex个对象中相应的弹簧，并对这些弹簧做长度检查，找出当前长度最长的弹簧必要的话做校正处理
	void    PreExam(all_Physics *thisphysic, Human::Skeletonchunk *sk_chunk, int key);


public:
	//all_Physics *thisphysic;
	CVector3 WindVector; //风力	

	bool   isinside(CVector3 v,Human::Skeletonchunk *sk_chunk,int i,int triindex){
		CVector3 v1,v2,v3;//
		CVector3 vector1,vector2,vector3,vector;
		double tan1,tan2,min,max,Q1,Q2,cos1,cos2,cos3,sin1,sin2,sin3,area1,area2,area3;
		bool b1=false,b2=false;
		v1=sk_chunk[i].vertex[sk_chunk[i].triangle[triindex].vert[0]].Convexhull;
		v2=sk_chunk[i].vertex[sk_chunk[i].triangle[triindex].vert[1]].Convexhull;
		v3=sk_chunk[i].vertex[sk_chunk[i].triangle[triindex].vert[2]].Convexhull;
		vector1=v2-v1;
		vector2=v3-v1;
		vector=v-v1;
		Q1=acos(Dot(vector1,vector)/(Magnitude(vector1)*Magnitude(vector)));
		Q2=acos(Dot(vector2,vector)/(Magnitude(vector2)*Magnitude(vector)));
		if(fabs(Q1+Q2-sk_chunk[i].triangle[triindex].degree012[0])>0.01){
			return false;
		}

		vector1=v1-v2;
		vector2=v3-v2;
		vector=v-v2;
		Q1=acos(Dot(vector1,vector)/(Magnitude(vector1)*Magnitude(vector)));
		Q2=acos(Dot(vector2,vector)/(Magnitude(vector2)*Magnitude(vector)));
		if(fabs(Q1+Q2-sk_chunk[i].triangle[triindex].degree012[1])>0.01){
					return false;
		}
		else {
			return true;
		}
	}
};
#define		GRAVITY					-2.0f		//重力加速度
#define     DRAGCOEFFICIENT        0.05f		//拉力系数
#define     WINDFACTOR              0.0			//风力因子
#define		WINDMAXVECTORX			0			//风力x轴上的最大量
#define		WINDMAXVECTORY			0			//风力y轴上的最大量
#define		WINDMAXVECTORZ			0			//风力z轴上的最大量
#define		STRUCTTENSIONCONSTANT	40.0f		//结构弹簧拉伸常量
#define		STRUCTGDAMPINGCONSTANT	2.0f		//结构弹簧阻尼常量
#define		SHEARTENSIONCONSTANT	40.0f		//剪切弹簧拉伸常量
#define		SHEARGDAMPINGCONSTANT	2.0f		//剪切弹簧阻尼常量
#define		SPRINGMAXELONGATION		0.01f		//弹簧最大伸长率
#define     SPRINGORIGINLENGTH		1.0f		//弹簧初始长度
#define		PHISYCSMASS				200.0f		//对象总质量
#define		FIXYPOSITION			31.0f		//固定点位置
#define		LIFTINGFORCE			0.0f		//提拉裙子的力
#define	    PULLFACTOR				0.0f		//下拉系数
#define		INTEGRATIONSTEP			0.05f		//积分步长
#define		ANGULARSPEED			0.0f		//角速度
#define     SYMLIDEGREE				0.3		//简化度
#define     VLAYERNUM				21		//每部分垂直方向层数
#define     STRETCHPER				0.5