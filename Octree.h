#include "3dmath.h"
#include <float.h>
#include <iostream>
#include "Load3DS.h"

class AABB {
public:
	CVector3 min, max;

public:
	AABB() {
		min.x = FLT_MAX;
		min.y = FLT_MAX;
		min.z = FLT_MAX;

		max.x = FLT_MIN;
		max.y = FLT_MIN;
		max.z = FLT_MIN;
	}
	CVector3 center(){ return (min + max)*0.5f; }	//�������ĵ�
	float xSize()	{ return (max.x - min.x); }
	float ySize()	{ return (max.y - min.y); }
	float zSize()	{ return (max.z - min.z); }
	float size()	{ return (max.x - min.x) * (max.y - min.y) * (max.z - min.z); }
	CVector3 corner(int i);			//����8�������е�һ��
	void add(CVector3 point);		//����һ�����㵽��Χ��
	void add(AABB box);				//����һ����Χ�е���Χ��
	bool isEmpty();					//����Χ���Ƿ�Ϊ��
	bool contains(CVector3 point);	//���ĳ�����Ƿ�����AABB��Χ
	CVector3 clostet_point_to(CVector3 point);
};

struct OctreeNodeData {
	int vertInfcounts;				//����ռ��ڵĵ�ĸ���
	vector<int> PointsIndex;		//�ռ��ڵĵ����ڽӱ��е�index
	vector<CVector3> point;			//�������

	OctreeNodeData() {
		vertInfcounts = 0;
	}
};

class Octree {
public:
	struct OctreeNode {
		OctreeNodeData data;			//�ڵ�����
		float xmin, xmax;	//�ڵ�����
		float ymin, ymax;
		float zmin, zmax;
		bool isLeaf;
		OctreeNode *top_left_front, *top_left_back; //�ýڵ�ĸ��ӽ��
		OctreeNode *top_right_front, *top_right_back;
		OctreeNode *bottom_left_front, *bottom_left_back;
		OctreeNode *bottom_right_front, *bottom_right_back;

		OctreeNode //�ڵ���
			(OctreeNodeData nodeValue = OctreeNodeData(),
			float xminValue = float(), float xmaxValue = float(),
			float yminValue = float(), float ymaxValue = float(),
			float zminValue = float(), float zmaxValue = float(),
			bool isLeafValue = bool(false),
			OctreeNode* top_left_front_Node = NULL,
			OctreeNode* top_left_back_Node = NULL,
			OctreeNode* top_right_front_Node = NULL,
			OctreeNode* top_right_back_Node = NULL,
			OctreeNode* bottom_left_front_Node = NULL,
			OctreeNode* bottom_left_back_Node = NULL,
			OctreeNode* bottom_right_front_Node = NULL,
			OctreeNode* bottom_right_back_Node = NULL)
			:data(nodeValue),
			xmin(xminValue), xmax(xmaxValue),
			ymin(yminValue), ymax(ymaxValue),
			zmin(zminValue), zmax(zmaxValue),
			top_left_front(top_left_front_Node),
			top_left_back(top_left_back_Node),
			top_right_front(top_right_front_Node),
			top_right_back(top_right_back_Node),
			bottom_left_front(bottom_left_front_Node),
			bottom_left_back(bottom_left_back_Node),
			bottom_right_front(bottom_right_front_Node),
			bottom_right_back(bottom_right_back_Node){}
	};
public:
	void createOctree(OctreeNode * &root, int maxdepth, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	void preOrder(OctreeNode * & p);
	int depth(OctreeNode *& p);
	//int cal(int num);
	void putIntoOctree(OctreeNode *& p, float x, float y, float z, float txm, float tym, float tzm);
	void putIntoOctreeMeshInfo(OctreeNode *& p, int index, float x, float y, float z, float txm, float tym, float tzm);

};
Octree::OctreeNode * creatOctreeUseModel(CLoad3DS::t3DModel *pModel, AABB *clothBox, int maxDepth);
Octree::OctreeNode * creatOctreeUseMesh(CLoad3DS::t3DModel *pModel, AABB *clothBox, int maxDepth);
void findYMaxPoints(Octree::OctreeNode *& p, CVector3 *& leftYMax, CVector3 *& rightYMax);
void findXminXmaxPoints(Octree::OctreeNode *& p, CVector3 *& Xmin, CVector3 *& Xmax);
void findYminPoints(Octree::OctreeNode *& p, CVector3 *& leftYMin, CVector3 *& rightYMin);
