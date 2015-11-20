
#include"Load3DS.h"
#include"human.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;
enum tCollisionTypes		 //��ײ��ö������
{
	NOT_COLLIDING,			 //û����ײ
	PENETRATING,			 //��͸
	COLLIDING				 //��ײ
};

enum tIntegratorTypes		 //��������ö������
{
	EULER_INTEGRATOR,		 //ŷ��������
	MIDPOINT_INTEGRATOR,	 //�е������
	RK4_INTEGRATOR			 //�Ľ�����_����������
};


enum tSpringTypes			 //���ɵ�ö������
{
	MANUAL_SPRING,			 //�ֶ�����
	STRUCTURAL_SPRING,		 //�ṹ����
	SHEAR_SPRING,			 //���е���
	BEND_SPRING				 //��������
};
struct Particle {                 //�ʵ������ṹ
	double	fMass;                //�ʵ�����
	double	fInvMass;             //�ʵ���������
	int     Cylindex;             //���ʵ��ھֲ�����ϵԲ��ӳ���������� 
	CVector3	vprePosition;     //�ʵ�ǰһʱ�̵�λ��
	CVector3	vPosition;        //�ʵ㵱ǰλ��
	CVector3    vPositionCopy;
	CVector3    vprojection;      //�õ��ھֲ�����ϵԲ����������ͶӰ���ȫ������
//	CVector3    vPcollision;      //���ʵ����������ײ��λ��
	CVector3    plocal_xyz;       //���ڴ洢�ʵ�ľֲ�xyz����
	CVector3    plocal_rQz;       //���ڴ洢�ʵ�ľֲ�rQz����
	CVector3	vVelocity;        //�ʵ��ٶ�
	CVector3	vAcceleration;    //�ʵ���ٶ�
	CVector3	vForces;          //�ʵ����ܵĺ���
	CVector3    vNormal;
	CVector3    vTempPC;
	//CVector3    vNormal;          //�ʵ�ķ�����
	bool	bLocked;              //�ʵ��Ƿ�̶��ı��
	bool    isbrief;              //���ʵ��Ƿ��Ǽ��ʵ�
	bool    iscollision;          //���ʵ��Ƿ������ײ

	bool	isSelfCollision;	  //���ʵ��Ƿ��������ײ

	bool    isrotation;           //���ʵ��Ƿ���ת
	int belongtoPart;         //����·��ϵĸõ�����������ĸ���λ
};


struct	Spring {            //���ɽṹ
	int 	index1;			//�뵯��һ�����ӵ��ʵ�������
	int		index2;			//�뵯����һ�����ӵ��ʵ�������
	double	Ks;				//�������쳣��
	double	Kd;				//������������	
	double	restLen;		//������Ȼ����
	double	curLen;		    //���ɵ�ǰ����
	double   maxLen;         //������󳤶�
	int	type;		    //��������
};

struct	Collision {	            //��ײ�ṹ
	int		  particleIndex;    //��ײ�ʵ�������
	CVector3  normal;		    //��ײ�ʵ�ķ�����
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
struct Datapart{     //���������ع�ʱ�����ݶԼ�¼
	int a; //�ʵ��ԭ������
	int b; //���ʵ㸴�ƺ��������
};
struct one_Physic{          //һ�������������Ϣ
	double mass;             //�ö��������
	int particlenumbers;    //����������ʵ����
	int pOriginnumbers;		//����������ʼ�ʵ����
	int pCopynumbers;		 //����������Ƶ��ʵ����
	int collisionnumbers;   //�����������ײ�ʵ����
	int springnumbers;      //���������ĵ��ɸ���
    Particle *pParticles;   //���������������ʵ���Ϣ
	Collision *pCollisions; //����������������ײ�ʵ���Ϣ
	vector<Spring> pSprings; //�������������е�����Ϣ
	Datapart* datacouple[10];//����10��Datapartָ������飬���У���ϥ�Ǵ���Ӧ����6
};

struct all_Physics{       //�������������ж�����Ϣ
	int objectnumbers;    //���������Ķ������
	one_Physic* physics;  //�������������ж���������Ϣ
};
///*-------����Ϊ�����˶���ص����ݽṹ-----------*/
struct direction_angle{   //���岿λ�˶�ʱ�ķ�����Ƕ�
	float angle;       //�Ƕ�
	float x;           //��ת����
	float y;
	float z;          
};
struct PiMotion{    //���岿λ�˶��ķ���ͽǶȼ�¼���ݽṹ
	bool ismotion;   //�ò�λ�Ƿ��˶�
	direction_angle *d_a; //��¼�˶��ķ���ͽǶ�
	bool isfirstSet; //�Ƿ��һ������
};
/*--------------------------------------------------*/
class CMSModel{
public:	

    CMSModel();          //���캯��
	~CMSModel();        //�������� 
	
	//void    AddExtroInfoTri()
	void    surePP(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk);//ȷ���·��ϵ�ÿ���ʵ���������Ĳ�λ�����б��
	void    Initialize(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,int key);      //��ʼ����Ա����pParticle
	void    CreateSpring(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel);    //ΪpParticle�е��ʵ㴴������
	void	ClothAnimation(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,int parti,float angle,float x,float y,float z,const M3DMatrix44f matrix0,const M3DMatrix44f matrix1,const M3DMatrix44f matrix2);
	void	Animation(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,int i,PiMotion*pi_m);
	void	CalcForces(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk, PiMotion*pi_m);			                   //����thisphysic��ÿ���ʵ����ܵĺ���
	void	Verlet(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk,double dt);
    void	StepSimulation(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk, PiMotion*pi_m,double dt);	                   //���㲽��dt֮���ģ��
    void	SetWindVector(double x, double y, double z);	    //���÷���
	void    CollisionCheck(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk);
	void    SingleCheck(Particle *particle,Human::Skeletonchunk *sk_chunk);
	void    CheckSprings(all_Physics *thisphysic);  //�����ʵ�������Ѱ��thisphysic�е�physicindex����������Ӧ�ĵ��ɣ�������Щ���������ȼ�飬�ҳ���ǰ������ĵ��ɱ�Ҫ�Ļ���У������
	void    PreExam(all_Physics *thisphysic, Human::Skeletonchunk *sk_chunk, int key);


public:
	//all_Physics *thisphysic;
	CVector3 WindVector; //����	

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
#define		GRAVITY					-2.0f		//�������ٶ�
#define     DRAGCOEFFICIENT        0.05f		//����ϵ��
#define     WINDFACTOR              0.0			//��������
#define		WINDMAXVECTORX			0			//����x���ϵ������
#define		WINDMAXVECTORY			0			//����y���ϵ������
#define		WINDMAXVECTORZ			0			//����z���ϵ������
#define		STRUCTTENSIONCONSTANT	40.0f		//�ṹ�������쳣��
#define		STRUCTGDAMPINGCONSTANT	2.0f		//�ṹ�������᳣��
#define		SHEARTENSIONCONSTANT	40.0f		//���е������쳣��
#define		SHEARGDAMPINGCONSTANT	2.0f		//���е������᳣��
#define		SPRINGMAXELONGATION		0.01f		//��������쳤��
#define     SPRINGORIGINLENGTH		1.0f		//���ɳ�ʼ����
#define		PHISYCSMASS				200.0f		//����������
#define		FIXYPOSITION			31.0f		//�̶���λ��
#define		LIFTINGFORCE			0.0f		//����ȹ�ӵ���
#define	    PULLFACTOR				0.0f		//����ϵ��
#define		INTEGRATIONSTEP			0.05f		//���ֲ���
#define		ANGULARSPEED			0.0f		//���ٶ�
#define     SYMLIDEGREE				0.3		//�򻯶�
#define     VLAYERNUM				21		//ÿ���ִ�ֱ�������
#define     STRETCHPER				0.5