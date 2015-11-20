#include "Octree.h"
#include "Mass-SpringModel.h"
#include "Load3DS.h"

using namespace std;



//bit 0 selects min.x vs max.x
//bit 1 selects min.y vs max.y
//bit 2 selects min.z vs max.z
CVector3 AABB::corner(int i){
	CVector3 conner;
	if (i & 1)	conner.x = max.x;	else conner.x = min.x;
	if (i & 2)	conner.y = max.y;	else conner.y = min.y;
	if (i & 4)	conner.z = max.z;	else conner.z = min.z;
	return conner;
}

void AABB::add(CVector3 point){
	if (point.x < min.x)	min.x = point.x;
	if (point.x > max.x)	max.x = point.x;
	if (point.y < min.y)	min.y = point.y;
	if (point.y > max.y)	max.y = point.y;
	if (point.z < min.z)	min.z = point.z;
	if (point.z > max.z)	max.z = point.z;
}

void AABB::add(AABB box) {
	if (box.min.x < min.x)	min.x = box.min.x;
	if (box.max.x > max.x)	max.x = box.max.x;
	if (box.min.y > min.y)	min.y = box.min.y;
	if (box.max.y < max.y)	max.y = box.max.y;
	if (box.min.z < min.z)	min.z = box.min.z;
	if (box.max.z > max.z)	max.z = box.max.z;
}

bool AABB::isEmpty() {
	return (min.x > max.x) || (min.y > max.y) || (min.z > max.z);
}

bool AABB::contains(CVector3 point) {
	return
		(point.x >= min.x) && (point.x <= max.x) &&
		(point.y >= min.y) && (point.y <= max.y) &&
		(point.z >= min.z) && (point.z <= max.z);
}

CVector3 AABB::clostet_point_to(CVector3 point) {
	CVector3 r;
	if (point.x < min.x)		r.x = min.x; 
	else if (point.x > max.x)	r.x = max.x;
	else						r.x = point.x;

	if (point.y < min.y)		r.y = min.y; 
	else if (point.y > max.y)	r.y = max.y;
	else						r.y = point.y;

	if (point.z < min.z)		r.z = min.z; 
	else if (point.z > max.z)	r.z = max.z;
	else						r.z = point.z;

	return r;
}

void createAABB_clothModel(CLoad3DS::t3DModel *pModel, AABB *clothBox){
	for (int index = 0; index < pModel->numOfObjects; index++) {
		
		CLoad3DS::t3DObject *pObject = &(pModel->vctObjects[index]);
		for (int i = 0; i < pObject->numOfVerts; i++) {
			clothBox->add(pObject->pVerts[i]);
		}
	}
}


//创建八叉树
void Octree::createOctree(OctreeNode * &root, int maxdepth, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) {
	maxdepth = maxdepth - 1; //每递归一次就将最大递归深度-1
	if (maxdepth >= 0) {
		root = new OctreeNode();
		root->data.vertInfcounts = 0;
		root->xmin = xmin; //为节点坐标赋值
		root->xmax = xmax;
		root->ymin = ymin;
		root->ymax = ymax;
		root->zmin = zmin;
		root->zmax = zmax;
		float xm = (xmax - xmin) / 2;//计算节点个维度上的半边长
		float ym = (ymax - ymin) / 2;
		float zm = (ymax - ymin) / 2;
		//递归创建子树，根据每一个节点所处（是几号节点）的位置决定其子结点的坐标。
		createOctree(root->top_left_front, maxdepth, xmin, xmax - xm, ymax - ym, ymax, zmax - zm, zmax);
		createOctree(root->bottom_left_front, maxdepth, xmin, xmax - xm, ymin, ymax - ym, zmax - zm, zmax);
		createOctree(root->top_right_front, maxdepth, xmax - xm, xmax, ymax - ym, ymax, zmax - zm, zmax);
		createOctree(root->bottom_right_front, maxdepth, xmax - xm, xmax, ymin, ymax - ym, zmax - zm, zmax);
		createOctree(root->top_left_back, maxdepth, xmin, xmax - xm, ymax - ym, ymax, zmin, zmax - zm);
		createOctree(root->bottom_left_back, maxdepth, xmin, xmax - xm, ymin, ymax - ym, zmin, zmax - zm);
		createOctree(root->top_right_back, maxdepth, xmax - xm, xmax, ymax - ym, ymax, zmin, zmax - zm);
		createOctree(root->bottom_right_back, maxdepth, xmax - xm, xmax, ymin, ymax - ym, zmin, zmax - zm);
	}
}

//先序遍历八叉树
void Octree::preOrder(OctreeNode * & p) {
	if (p) {
		//cout << i << ".当前节点的值为：" << p->data << "\n坐标为：";
		//cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		//cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		//cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		//i += 1;
		preOrder(p->top_left_front);
		preOrder(p->top_left_back);
		preOrder(p->top_right_front);
		preOrder(p->top_right_back);
		preOrder(p->bottom_left_front);
		preOrder(p->bottom_left_back);
		preOrder(p->bottom_right_front);
		preOrder(p->bottom_right_back);
		//cout << endl;
	}
}

//求八叉树的深度
int Octree::depth(OctreeNode *& p) {
	if (p == NULL)
		return -1;
	int h = depth(p->top_left_front);
	return h + 1;
}

//计算单位长度，为查找点做准备
int cal(int num) {
	int result = 1;
	if (1 == num)
		result = 1;
	else {
		for (int i = 1; i<num; i++)
			result = 2 * result;
	}
	return result;
}


void Octree::putIntoOctree(OctreeNode *& p, float x, float y, float z, float txm, float tym, float tzm) {
	if (p == NULL)
		return;
	float xm = (p->xmax - p->xmin) / 2;
	float ym = (p->ymax - p->ymin) / 2;
	float zm = (p->zmax - p->zmin) / 2;
	p->data.vertInfcounts++;

	CVector3 newPoint;
	newPoint.Set(x, y, z);
	p->data.point.push_back(newPoint);
	if (x <= p->xmin + txm && x >= p->xmax - txm && y <= p->ymin + tym && y >= p->ymax - tym && z <= p->zmin + tzm && z >= p->zmax - tzm) {
		//cout << endl << "找到该点！" << "该点位于" << endl;
		//cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		//cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		//cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		//cout << "节点内！" << endl;
		//cout << "共经过" << times << "次递归！" << endl;
		p->isLeaf = true;
		return;
	}
	else if (x<(p->xmax - xm) && y<(p->ymax - ym) && z<(p->zmax - zm)) {
		putIntoOctree(p->bottom_left_back, x, y, z, txm, tym, tzm);
	}
	else if (x<(p->xmax - xm) && y<(p->ymax - ym) && z>(p->zmax - zm)) {
		putIntoOctree(p->bottom_left_front, x, y, z, txm, tym, tzm);
	}
	else if (x>(p->xmax - xm) && y<(p->ymax - ym) && z<(p->zmax - zm)) {
		putIntoOctree(p->bottom_right_back, x, y, z, txm, tym, tzm);
	}
	else if (x>(p->xmax - xm) && y<(p->ymax - ym) && z>(p->zmax - zm)) {
		putIntoOctree(p->bottom_right_front, x, y, z, txm, tym, tzm);
	}
	else if (x<(p->xmax - xm) && y>(p->ymax - ym) && z<(p->zmax - zm)) {
		putIntoOctree(p->top_left_back, x, y, z, txm, tym, tzm);
	}
	else if (x<(p->xmax - xm) && y>(p->ymax - ym) && z>(p->zmax - zm)) {
		putIntoOctree(p->top_left_front, x, y, z, txm, tym, tzm);
	}
	else if (x>(p->xmax - xm) && y>(p->ymax - ym) && z<(p->zmax - zm)) {
		putIntoOctree(p->top_right_back, x, y, z, txm, tym, tzm);
	}
	else if (x>(p->xmax - xm) && y>(p->ymax - ym) && z>(p->zmax - zm)) {
		putIntoOctree(p->top_right_front, x, y, z, txm, tym, tzm);
	}
}

void Octree::putIntoOctreeMeshInfo(OctreeNode *& p, int index, float x, float y, float z, float txm, float tym, float tzm) {
	if (p == NULL)
		return;
	float xm = (p->xmax - p->xmin) / 2;
	float ym = (p->ymax - p->ymin) / 2;
	float zm = (p->zmax - p->zmin) / 2;
	p->data.vertInfcounts++;
	p->data.PointsIndex.push_back(index);
	CVector3 newPoint;
	newPoint.Set(x, y, z);
	p->data.point.push_back(newPoint);
	if (x <= p->xmin + txm && x >= p->xmax - txm && y <= p->ymin + tym && y >= p->ymax - tym && z <= p->zmin + tzm && z >= p->zmax - tzm) {
		//cout << endl << "找到该点！" << "该点位于" << endl;
		//cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		//cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		//cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		//cout << "节点内！" << endl;
		//cout << "共经过" << times << "次递归！" << endl;
		p->isLeaf = true;
		return;
	}
	else if (x<(p->xmax - xm) && y<(p->ymax - ym) && z<(p->zmax - zm)) {
		putIntoOctreeMeshInfo(p->bottom_left_back, index, x, y, z, txm, tym, tzm);
	}
	else if (x<(p->xmax - xm) && y<(p->ymax - ym) && z>(p->zmax - zm)) {
		putIntoOctreeMeshInfo(p->bottom_left_front, index, x, y, z, txm, tym, tzm);
	}
	else if (x>(p->xmax - xm) && y<(p->ymax - ym) && z<(p->zmax - zm)) {
		putIntoOctreeMeshInfo(p->bottom_right_back, index, x, y, z, txm, tym, tzm);
	}
	else if (x>(p->xmax - xm) && y<(p->ymax - ym) && z>(p->zmax - zm)) {
		putIntoOctreeMeshInfo(p->bottom_right_front, index, x, y, z, txm, tym, tzm);
	}
	else if (x<(p->xmax - xm) && y>(p->ymax - ym) && z<(p->zmax - zm)) {
		putIntoOctreeMeshInfo(p->top_left_back, index, x, y, z, txm, tym, tzm);
	}
	else if (x<(p->xmax - xm) && y>(p->ymax - ym) && z>(p->zmax - zm)) {
		putIntoOctreeMeshInfo(p->top_left_front, index, x, y, z, txm, tym, tzm);
	}
	else if (x>(p->xmax - xm) && y>(p->ymax - ym) && z<(p->zmax - zm)) {
		putIntoOctreeMeshInfo(p->top_right_back, index, x, y, z, txm, tym, tzm);
	}
	else if (x>(p->xmax - xm) && y>(p->ymax - ym) && z>(p->zmax - zm)) {
		putIntoOctreeMeshInfo(p->top_right_front, index, x, y, z, txm, tym, tzm);
	}
}




Octree::OctreeNode * creatOctreeUseModel(CLoad3DS::t3DModel *pModel, AABB *clothBox, int maxDepth) {
	//AABB *clothBox = new AABB();
	createAABB_clothModel(pModel, clothBox);

	Octree *clothOctree = new Octree();
	Octree::OctreeNode * rootNode = new Octree::OctreeNode();
	float txm, tym, tzm;
	//这里每个树节点不是点，而是一块空间
	if (maxDepth >= 0 || clothBox->max.x > clothBox->min.x || clothBox->max.y > clothBox->min.y || clothBox->max.z > clothBox->min.z || clothBox->min.x > 0 || clothBox->min.y > 0 || clothBox->min.z > 0) {
		int tmaxdepth = cal(maxDepth);
		txm = (clothBox->max.x - clothBox->min.x) / tmaxdepth;
		tym = (clothBox->max.y - clothBox->min.y) / tmaxdepth;
		tzm = (clothBox->max.z - clothBox->min.z) / tmaxdepth;
		clothOctree->createOctree(rootNode, maxDepth, clothBox->min.x, clothBox->max.x, clothBox->min.y, clothBox->max.y, clothBox->min.z, clothBox->max.z);
	}
	//else
	//	return;

	for (int index = 0; index < pModel->numOfObjects; index++) {

		CLoad3DS::t3DObject *pObject = &(pModel->vctObjects[index]);
		for (int i = 0; i < pObject->numOfVerts; i++) {
			//遍历每个点，将点放入八叉树的节点的Data中
			clothOctree->putIntoOctree(rootNode, pObject->pVerts[i].x, pObject->pVerts[i].y, pObject->pVerts[i].z, txm, tym, tzm);
		}
	}
	return rootNode;
}

Octree::OctreeNode * creatOctreeUseMesh(CLoad3DS::t3DModel *pModel, AABB *clothBox, int maxDepth) {
	createAABB_clothModel(pModel, clothBox);
	Octree *clothOctree = new Octree();
	Octree::OctreeNode * rootNode = new Octree::OctreeNode();
	float txm, tym, tzm;
	//这里每个树节点不是点，而是一块空间
	if (maxDepth >= 0 || clothBox->max.x > clothBox->min.x || clothBox->max.y > clothBox->min.y || clothBox->max.z > clothBox->min.z || clothBox->min.x > 0 || clothBox->min.y > 0 || clothBox->min.z > 0) {
		int tmaxdepth = cal(maxDepth);
		txm = (clothBox->max.x - clothBox->min.x) / tmaxdepth;
		tym = (clothBox->max.y - clothBox->min.y) / tmaxdepth;
		tzm = (clothBox->max.z - clothBox->min.z) / tmaxdepth;
		clothOctree->createOctree(rootNode, maxDepth, clothBox->min.x, clothBox->max.x, clothBox->min.y, clothBox->max.y, clothBox->min.z, clothBox->max.z);
	}
	//else
	//	return;

	for (int index = 0; index < pModel->numOfMeshInfos; index++) {

		CLoad3DS::MeshInfo *pMeshInfo = &(pModel->vctMeshInfos[index]);
		for (int i = 0; i < pMeshInfo->vertInfcounts; i++) {
			//遍历每个点，将点放入八叉树的节点的Data中
			clothOctree->putIntoOctreeMeshInfo(rootNode, i, pMeshInfo->vertInfos[i].Coordinates.x, pMeshInfo->vertInfos[i].Coordinates.y, pMeshInfo->vertInfos[i].Coordinates.z, txm, tym, tzm);
		}
	}
	return rootNode;
}

void findYMaxPoints(Octree::OctreeNode *& p, CVector3 *& leftYMax, CVector3 *& rightYMax){

	float yMaxLeft = FLT_MIN;
	float yMaxRight = FLT_MIN;
	for (int i = 0; i < p->top_left_back->data.vertInfcounts; i++) {
		if (p->top_left_back->data.point[i].y > yMaxLeft) {
			leftYMax->Set(p->top_left_back->data.point[i].x, p->top_left_back->data.point[i].y, p->top_left_back->data.point[i].z);
			yMaxLeft = p->top_left_back->data.point[i].y;
		}
	}
	for (int i = 0; i < p->top_left_front->data.vertInfcounts; i++) {
		if (p->top_left_front->data.point[i].y > yMaxLeft) {
			leftYMax->Set(p->top_left_front->data.point[i].x, p->top_left_front->data.point[i].y, p->top_left_front->data.point[i].z);
			yMaxLeft = p->top_left_front->data.point[i].y;
		}
	}

	for (int i = 0; i < p->top_right_back->data.vertInfcounts; i++) {
		if (p->top_right_back->data.point[i].y > yMaxRight) {
			rightYMax->Set(p->top_right_back->data.point[i].x, p->top_right_back->data.point[i].y, p->top_right_back->data.point[i].z);
			yMaxRight = p->top_right_back->data.point[i].y;
		}
	}
	for (int i = 0; i < p->top_right_front->data.vertInfcounts; i++) {
		if (p->top_right_front->data.point[i].y > yMaxRight) {
			rightYMax->Set(p->top_right_front->data.point[i].x, p->top_right_front->data.point[i].y, p->top_right_front->data.point[i].z);
			yMaxRight = p->top_right_front->data.point[i].y;
		}
	}

}

//找的是衣服下围的X最大和最小
void findXminXmaxPoints(Octree::OctreeNode *& p, CVector3 *& Xmin, CVector3 *& Xmax) {
	float xMin = FLT_MAX;
	float xMax = FLT_MIN;
	//找X最小
	for (int i = 0; i < p->bottom_left_back->data.vertInfcounts; i++) {
		if (p->bottom_left_back->data.point[i].x < xMin) {
			Xmin->Set(p->bottom_left_back->data.point[i].x, p->bottom_left_back->data.point[i].y, p->bottom_left_back->data.point[i].z);
			xMin = p->bottom_left_back->data.point[i].x;
		}
	}

	for (int i = 0; i < p->bottom_left_front->data.vertInfcounts; i++) {
		if (p->bottom_left_front->data.point[i].x < xMin) {
			Xmin->Set(p->bottom_left_front->data.point[i].x, p->bottom_left_front->data.point[i].y, p->bottom_left_front->data.point[i].z);
			xMin = p->bottom_left_front->data.point[i].x;
		}
	}

	//找X最大
	for (int i = 0; i < p->bottom_right_back->data.vertInfcounts; i++) {
		if (p->bottom_right_back->data.point[i].x > xMax) {
			Xmax->Set(p->bottom_right_back->data.point[i].x, p->bottom_right_back->data.point[i].y, p->bottom_right_back->data.point[i].z);
			xMax = p->bottom_right_back->data.point[i].x;
		}
	}

	for (int i = 0; i < p->bottom_right_front->data.vertInfcounts; i++) {
		if (p->bottom_right_front->data.point[i].x > xMax) {
			Xmax->Set(p->bottom_right_front->data.point[i].x, p->bottom_right_front->data.point[i].y, p->bottom_right_front->data.point[i].z);
			xMax = p->bottom_right_front->data.point[i].x;
		}
	}
}


void findYminPoints(Octree::OctreeNode *& p, CVector3 *& leftYMin, CVector3 *& rightYMin) {
	float yMinLeft = FLT_MAX;
	float yMinRight = FLT_MAX;
	for (int i = 0; i < p->bottom_left_back->data.vertInfcounts; i++) {
		if (p->bottom_left_back->data.point[i].y < yMinLeft) {
			leftYMin->Set(p->bottom_left_back->data.point[i].x, p->bottom_left_back->data.point[i].y, p->bottom_left_back->data.point[i].z);
			yMinLeft = p->bottom_left_back->data.point[i].y;
		}
	}
	for (int i = 0; i < p->bottom_left_front->data.vertInfcounts; i++) {
		if (p->bottom_left_front->data.point[i].y < yMinLeft) {
			leftYMin->Set(p->bottom_left_front->data.point[i].x, p->bottom_left_front->data.point[i].y, p->bottom_left_front->data.point[i].z);
			yMinLeft = p->bottom_left_front->data.point[i].y;
		}
	}

	for (int i = 0; i < p->bottom_left_front->data.vertInfcounts; i++) {
		if (p->bottom_right_back->data.point[i].y < yMinRight) {
			rightYMin->Set(p->bottom_right_back->data.point[i].x, p->bottom_right_back->data.point[i].y, p->bottom_right_back->data.point[i].z);
			yMinRight = p->bottom_left_front->data.point[i].y;
		}
	}
	for (int i = 0; i < p->bottom_right_front->data.vertInfcounts; i++) {
		if (p->bottom_right_front->data.point[i].y < yMinRight) {
			rightYMin->Set(p->bottom_right_front->data.point[i].x, p->bottom_right_front->data.point[i].y, p->bottom_right_front->data.point[i].z);
			yMinRight = p->bottom_right_front->data.point[i].y;
		}
	}
}

//Y方向上的对齐
float alignY(Octree::OctreeNode *& p, Human::Skeletonchunk *sk_chunk) {
	float moveY = 0;


	return moveY;
}