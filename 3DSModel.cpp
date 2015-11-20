#include "stdafx.h"
#include "3DSModel.h"

#include "Octree.h"

#include <fstream>
#include <windows.h>  
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>

using namespace std;

#define SCALESIZE 0.17	//比例尺——人体模型

double time;

//只是为了只改变一次邻接表
bool flag_change_MeshInfo = true;


ofstream outStream;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
C3DSModel::C3DSModel()
{
	m_sFileName[0]=0;
	m_bModelOK=FALSE;

	m_3DModel.numOfMaterials=0;
	m_3DModel.numOfObjects=0;
	m_3DModel.numOfMeshInfos=0;
	m_3DModel.vctMaterials.clear();
	m_3DModel.vctObjects.clear();
	m_3DModel.vctMeshInfos.clear();

	thisphysic.objectnumbers=0;
	thisphysic.physics=0;

}
C3DSModel::~C3DSModel()
{

	for(int i = 0; i < m_3DModel.numOfObjects; i++)
	{
		// Free the faces, normals, vertices, and texture coordinates.
		delete [] m_3DModel.vctObjects[i].pFaces;
		delete [] m_3DModel.vctObjects[i].pNormals;
		delete [] m_3DModel.vctObjects[i].pVerts;
		delete [] m_3DModel.vctObjects[i].pTexVerts;
		for(int j=0;j<m_3DModel.vctObjects[i].numOfMaterials;j++)
			delete[] m_3DModel.vctObjects[i].pMaterialREFS[j].pFaceIndexs;
		delete [] m_3DModel.vctObjects[i].pMaterialREFS;
		delete [] m_3DModel.vctMeshInfos[i].edgeInfos;
		delete [] m_3DModel.vctMeshInfos[i].faceInfos;
		delete [] m_3DModel.vctMeshInfos[i].vertInfos;
	}
	delete [] thisphysic.physics;
	delete [] sk_chunk;
}
//绘制模型
bool C3DSModel::Drawcloth(int key)//显示衣服模型
{
	CLoad3DS::tMatREF *pmatref;
	CLoad3DS::t3DObject *pObject;
	CLoad3DS::MeshInfo *pMeshInfo;

	for(int i = 0; i <1; i++)// m_3DModel.numOfObjects
	{
		//for(int index=0;index<thisphysic.physics[i].particlenumbers;index++)
		// Get the current object that we are displaying
		pObject = &m_3DModel.vctObjects[i];
		pMeshInfo=&m_3DModel.vctMeshInfos[i];
		//对所有的子材料
		for(int imat=0;imat<pObject->numOfMaterials;imat++)
		{
			pmatref=&pObject->pMaterialREFS[imat];;
			if(pmatref->bHasTexture) 
			{
				glEnable(GL_TEXTURE_2D);
				glColor3ub(255, 255, 255);
				glBindTexture(GL_TEXTURE_2D, m_3DModel.vctMaterials[pmatref->nMaterialID].texureId);//g_Texture[pObject->materialID]);
			} 
			else 
			{
				glDisable(GL_TEXTURE_2D);
				glColor3ub(255, 255, 255);
			}	
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);//显示填充

//			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);//显示网格
			glPointSize(2.0f);

			glBegin(GL_TRIANGLES);//GL_POINTS GL_LINE_STRIP// Begin drawing with our selected mode (triangles or lines)
			for(int nfindex = 0,j=0; nfindex < pmatref->nFaceNum; nfindex++)
			{
				j=int(pmatref->pFaceIndexs[nfindex]);
				
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					// Get the index for each point of the face
					int index = pObject->pFaces[j].vertIndex[whichVertex];
					
					// Give OpenGL the normal for this vertex.
					
					thisphysic.physics[i].pParticles[ index ].vNormal=pObject->pNormals[ index ];
					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
					
					// If the object has a texture associated with it, give it a texture coordinate.
					if(pmatref->bHasTexture)// pObject->bHasTexture) {
					{
						// Make sure there was a UVW map applied to the object or else it won't have tex coords.
						if(pObject->pTexVerts) {
							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[index].y);
						}
					} 
					else 
					{
						if(m_3DModel.numOfMaterials>0 && pmatref->nMaterialID>=0)//pObject->materialID >= 0) 
						{
							// Get and set the color that the object is, since it must not have a texture
							BYTE *pColor = m_3DModel.vctMaterials[pmatref->nMaterialID].color;
							// Assign the current color to this model
							glColor3ub(pColor[0], pColor[1], pColor[2]);
						}
					}
					glVertex3f(thisphysic.physics[i].pParticles[ index ].vPosition.x,thisphysic.physics[i].pParticles[ index ].vPosition.y,thisphysic.physics[i].pParticles[ index ].vPosition.z);
				}
			}
			glEnd();				
			
		}
		
	}//glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	return TRUE;
}


void C3DSModel::AlignmentClothesAndHuman() {
	AABB *clothBox = new AABB();
	//深度定为2
	Octree::OctreeNode * rootNode = creatOctreeUseMesh(&m_3DModel, clothBox, 2);
	const float ZOOM =20;	//放大倍数 背心--1	女式短袖--24   T恤1、2--20  衬衫1--20

	//for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
	//	for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {
	//		m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates = m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates * ZOOM;
	//		m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y -= 20;
	//	}
	//}

	CVector3 *Xmin = new CVector3;
	CVector3 *Xmax = new CVector3;
	findXminXmaxPoints(rootNode, Xmin, Xmax);

	Xmin->x = Xmin->x * ZOOM;
	Xmin->y = Xmin->y * ZOOM;
	Xmin->z = Xmin->z * ZOOM;

	Xmax->x = Xmax->x * ZOOM;
	Xmax->y = Xmax->y * ZOOM;
	Xmax->z = Xmax->z * ZOOM;

	CVector3 *leftYMax = new CVector3;
	CVector3 *rightYMax = new CVector3;
	findYMaxPoints(rootNode, leftYMax, rightYMax);

	leftYMax->x = leftYMax->x * ZOOM;
	leftYMax->y = leftYMax->y * ZOOM;
	leftYMax->z = leftYMax->z * ZOOM;

	rightYMax->x = rightYMax->x * ZOOM;
	rightYMax->y = rightYMax->y * ZOOM;
	rightYMax->z = rightYMax->z * ZOOM;

	CVector3 top, bottom;
	top.x = (leftYMax->x + rightYMax->x) / 2;
	top.y = (leftYMax->y + rightYMax->y) / 2;
	top.z = (leftYMax->z + rightYMax->z) / 2;
	bottom.x = (Xmin->x + Xmax->x) / 2;
	bottom.y = (Xmin->y + Xmax->y) / 2;
	bottom.z = (Xmin->z + Xmax->z) / 2;

	float sinTheta = abs(top.z - bottom.z) / sqrt((top.y - bottom.y) * (top.y - bottom.y) + (top.z - bottom.z) * (top.z - bottom.z));
	float cosTheta = abs(top.y - bottom.y) / sqrt((top.y - bottom.y) * (top.y - bottom.y) + (top.z - bottom.z) * (top.z - bottom.z));

	float DeltaY = -bottom.y * cosTheta + bottom.z * sinTheta;
	float DeltaZ = -bottom.y * sinTheta - bottom.z * cosTheta;

	for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
		for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {

			CVector3 tempvertex;
			tempvertex.x = m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x;
			tempvertex.y = m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y;
			tempvertex.z = m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z;

			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x = tempvertex.x * ZOOM - bottom.x;
			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y = tempvertex.y * ZOOM;
			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z = tempvertex.z * ZOOM - bottom.z +0.4;
			//m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y = tempvertex.y * ZOOM * cosTheta - tempvertex.z * ZOOM * sinTheta + DeltaY;
			//m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z = tempvertex.y * ZOOM * sinTheta + tempvertex.z * ZOOM * cosTheta + DeltaZ;

		}
	}

	//Y方向上的对齐，寻找衣服模型上的肩的位置
	CVector3 Shoulder_Cloth;
	Shoulder_Cloth.x = 0;
	Shoulder_Cloth.y = -FLT_MAX;
	Shoulder_Cloth.z = 0;
	for (int index = 0; index < rootNode->top_left_front->data.vertInfcounts; index++) {
		if (m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.x < Xmin->x) {
			if (m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.y > Shoulder_Cloth.y) {
				Shoulder_Cloth.x = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.x;
				Shoulder_Cloth.y = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.y;
				Shoulder_Cloth.z = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.z;
			}
			else if (m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.y == Shoulder_Cloth.y && m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.x > Shoulder_Cloth.x) {
				Shoulder_Cloth.x = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.x;
				Shoulder_Cloth.y = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.y;
				Shoulder_Cloth.z = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_front->data.PointsIndex[index]].Coordinates.z;
			}
		}
	}

	for (int index = 0; index < rootNode->top_left_back->data.vertInfcounts; index++) {
		if (m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.x < Xmin->x) {
			if (m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.y > Shoulder_Cloth.y) {
				Shoulder_Cloth.x = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.x;
				Shoulder_Cloth.y = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.y;
				Shoulder_Cloth.z = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.z;
			}
			else if (m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.y == Shoulder_Cloth.y && m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.x > Shoulder_Cloth.x) {
				Shoulder_Cloth.x = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.x;
				Shoulder_Cloth.y = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.y;
				Shoulder_Cloth.z = m_3DModel.vctMeshInfos[0].vertInfos[rootNode->top_left_back->data.PointsIndex[index]].Coordinates.z;
			}
		}
	}

	float YDIS = sk_chunk[9].vertex[10].Coordinates.y - Shoulder_Cloth.y;

	for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
		for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {

			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y += YDIS;
			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z -= 0.5;
		}
	}
}

CVector3 C3DSModel::AlignmentTrousesAndHuman() {
	const double ZOOM = 0.86;//男性裤子2 0.86 牛仔裤0.9
	//X和Z方向上的对齐
	CVector3 Xmin, Xmax, Ymax, Zmin, Zmax;
	Xmin.x = FLT_MAX;
	Xmax.x = -FLT_MAX;
	Ymax.y = -FLT_MAX;
	Zmin.z = FLT_MAX;
	Zmax.z = -FLT_MAX;
	for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
		for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {
			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates = m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates * ZOOM;
			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x < Xmin.x) {
				Xmin.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
			}
			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x > Xmax.x) {
				Xmax.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
			}
			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z < Zmin.z) {
				Zmin.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
			}
			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z > Zmax.z) {
				Zmax.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
			}
			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y > Ymax.y) {
				Ymax.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
			}
		}
	}

	//CVector3 waistZmin, waistXmin, waistXmax;
	//waistZmin.z = FLT_MAX;
	//waistXmin.x = FLT_MAX;
	//waistXmax.x = -FLT_MAX;
	//for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
	//	for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {
	//		if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y - Zmax.y < 0.08 && Zmax.y - m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y < 0.08) {	//0.08是个经验值
	//			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z < waistZmin.z) {
	//				waistZmin.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
	//			}
	//			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x < waistXmin.x) {
	//				waistXmin.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
	//			}
	//			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x > waistXmax.x) {
	//				waistXmax.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
	//			}
	//		}
	//	}
	//}

	//Z轴方向上对齐
	CVector3 crotch, leftLegYmin;	//裤子的裆部点和左腿Y最小的点
	crotch.y = FLT_MAX;
	leftLegYmin.y = FLT_MAX;
	float Xmiddle = (Xmin.x + Xmax.x) / 2;
	//float Zmiddle = (waistZmin.z + Zmax.z) / 2;
	for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
		for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {
			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x -= Xmiddle;
			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y -= Ymax.y - sk_chunk[2].vertex[0].Coordinates.y;
			//m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z -= Zmiddle;
			if ((m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x - Xmiddle < 0.1) && (Xmiddle - m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x < 0.1)) {
				if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y < crotch.y) {
					crotch.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
				}
			}
			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x < Xmiddle) {	//表示是左腿
				if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y < leftLegYmin.y) {
					leftLegYmin.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
				}
			}
		}
	}
	CVector3 thighLeftXmin;//左裤腿X最小的点
	CVector3 leftBottomXmin, leftBottomXmax;	//分别是做裤腿裤脚的X最小的点和X最大的点
	thighLeftXmin.x = FLT_MAX;
	leftBottomXmin.x = FLT_MAX;
	leftBottomXmax.x = -FLT_MAX;
	int count = 0;
	for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
		for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {
			if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x < Xmiddle) {	//比裤裆点的x值小，表明是左裤腿
				if ((m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y - crotch.y < 0.01) && (crotch.y - m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y < 0.01)) {
					count++;
					if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x < thighLeftXmin.x) {
						thighLeftXmin.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
					}
				}

				if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y - leftLegYmin.y < 0.5) {
					if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x < leftBottomXmin.x) {
						leftBottomXmin.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
					}
					if (m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.x > leftBottomXmax.x) {
						leftBottomXmax.Set(m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates);
					}
				}
			}
		}
	}

	//找到左裤腿的轴
	CVector3 leftTrouserAxisTop, leftTrouserAxisBottom;
	leftTrouserAxisTop.x = (thighLeftXmin.x + crotch.x) / 2;
	leftTrouserAxisTop.y = (thighLeftXmin.y + crotch.y) / 2;
	leftTrouserAxisTop.z = (thighLeftXmin.z + crotch.z) / 2;
	leftTrouserAxisBottom.x = (leftBottomXmin.x + leftBottomXmax.x) / 2;
	leftTrouserAxisBottom.y = (leftBottomXmin.y + leftBottomXmax.y) / 2;
	leftTrouserAxisBottom.z = (leftBottomXmin.z + leftBottomXmax.z) / 2;

	//找到左腿的轴
	CVector3 leftLegAxisTop, leftLegAxisBottom;
	leftLegAxisTop.x = (sk_chunk[4].vertex[0].Coordinates.x + sk_chunk[4].vertex[10].Coordinates.x) / 2;
	leftLegAxisTop.y = (sk_chunk[4].vertex[0].Coordinates.y + sk_chunk[4].vertex[10].Coordinates.y) / 2;
	leftLegAxisTop.z = (sk_chunk[4].vertex[0].Coordinates.z + sk_chunk[4].vertex[10].Coordinates.z) / 2;
	leftLegAxisBottom.x = (sk_chunk[6].vertex[0].Coordinates.x + sk_chunk[6].vertex[10].Coordinates.x) / 2;
	leftLegAxisBottom.y = (sk_chunk[6].vertex[0].Coordinates.y + sk_chunk[6].vertex[10].Coordinates.y) / 2;
	leftLegAxisBottom.z = (sk_chunk[6].vertex[0].Coordinates.z + sk_chunk[6].vertex[10].Coordinates.z) / 2;

	//微调裤子的位置，前后（Z轴方向）微调，左右不需要微调
	//for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
	//	for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {
	//		m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z += leftLegAxisTop.z - leftTrouserAxisTop.z;
	//	}
	//}

	float leftLegAxisLength = Distance(leftLegAxisTop, leftLegAxisBottom);
	CVector3 kneeNewPosition;
	kneeNewPosition.z = leftLegAxisBottom.z;
	kneeNewPosition.x = leftLegAxisTop.x + (leftTrouserAxisBottom.x - leftTrouserAxisTop.x) * sqrt((leftLegAxisTop.x - leftLegAxisBottom.x)*(leftLegAxisTop.x - leftLegAxisBottom.x) + (leftLegAxisTop.y - leftLegAxisBottom.y)*(leftLegAxisTop.y - leftLegAxisBottom.y)) / sqrt((leftTrouserAxisTop.x - leftTrouserAxisBottom.x)*(leftTrouserAxisTop.x - leftTrouserAxisBottom.x) + (leftTrouserAxisTop.y - leftTrouserAxisBottom.y)*(leftTrouserAxisTop.y - leftTrouserAxisBottom.y));
	kneeNewPosition.y = leftLegAxisTop.y - sqrt(leftLegAxisLength*leftLegAxisLength - (kneeNewPosition.x - leftLegAxisTop.x)*(kneeNewPosition.x - leftLegAxisTop.x) - (kneeNewPosition.z - leftLegAxisTop.z)*(kneeNewPosition.z - leftLegAxisTop.z));
	return kneeNewPosition;
}

bool C3DSModel::Drawcloth_V3() {
	//以点的形式显示
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
		for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {
			if (thisphysic.physics[i].pParticles[j].iscollision) {	//碰撞的点
				glVertex3f(thisphysic.physics[i].pParticles[j].vPosition.x, thisphysic.physics[i].pParticles[j].vPosition.y, thisphysic.physics[i].pParticles[j].vPosition.z);
			}
		}
	}
	glEnd();
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//显示网格
	glBegin(GL_QUADS);
		glColor3f(0.0, 1.0, 0.0);

	for (int i = 0; i < 10; i++) {
		glVertex3f(sk_chunk[i].AnglePoint[0].x, sk_chunk[i].AnglePoint[0].y, sk_chunk[i].AnglePoint[0].z);
		glVertex3f(sk_chunk[i].AnglePoint[1].x, sk_chunk[i].AnglePoint[1].y, sk_chunk[i].AnglePoint[1].z);
		glVertex3f(sk_chunk[i].AnglePoint[2].x, sk_chunk[i].AnglePoint[2].y, sk_chunk[i].AnglePoint[2].z);
		glVertex3f(sk_chunk[i].AnglePoint[3].x, sk_chunk[i].AnglePoint[3].y, sk_chunk[i].AnglePoint[3].z);

		glVertex3f(sk_chunk[i].AnglePoint[4].x, sk_chunk[i].AnglePoint[4].y, sk_chunk[i].AnglePoint[4].z);
		glVertex3f(sk_chunk[i].AnglePoint[5].x, sk_chunk[i].AnglePoint[5].y, sk_chunk[i].AnglePoint[5].z);
		glVertex3f(sk_chunk[i].AnglePoint[6].x, sk_chunk[i].AnglePoint[6].y, sk_chunk[i].AnglePoint[6].z);
		glVertex3f(sk_chunk[i].AnglePoint[7].x, sk_chunk[i].AnglePoint[7].y, sk_chunk[i].AnglePoint[7].z);

		glVertex3f(sk_chunk[i].AnglePoint[1].x, sk_chunk[i].AnglePoint[1].y, sk_chunk[i].AnglePoint[1].z);
		glVertex3f(sk_chunk[i].AnglePoint[2].x, sk_chunk[i].AnglePoint[2].y, sk_chunk[i].AnglePoint[2].z);
		glVertex3f(sk_chunk[i].AnglePoint[7].x, sk_chunk[i].AnglePoint[7].y, sk_chunk[i].AnglePoint[7].z);
		glVertex3f(sk_chunk[i].AnglePoint[6].x, sk_chunk[i].AnglePoint[6].y, sk_chunk[i].AnglePoint[6].z);

		glVertex3f(sk_chunk[i].AnglePoint[4].x, sk_chunk[i].AnglePoint[4].y, sk_chunk[i].AnglePoint[4].z);
		glVertex3f(sk_chunk[i].AnglePoint[5].x, sk_chunk[i].AnglePoint[5].y, sk_chunk[i].AnglePoint[5].z);
		glVertex3f(sk_chunk[i].AnglePoint[0].x, sk_chunk[i].AnglePoint[0].y, sk_chunk[i].AnglePoint[0].z);
		glVertex3f(sk_chunk[i].AnglePoint[3].x, sk_chunk[i].AnglePoint[3].y, sk_chunk[i].AnglePoint[3].z);

		glVertex3f(sk_chunk[i].AnglePoint[4].x, sk_chunk[i].AnglePoint[4].y, sk_chunk[i].AnglePoint[4].z);
		glVertex3f(sk_chunk[i].AnglePoint[7].x, sk_chunk[i].AnglePoint[7].y, sk_chunk[i].AnglePoint[7].z);
		glVertex3f(sk_chunk[i].AnglePoint[2].x, sk_chunk[i].AnglePoint[2].y, sk_chunk[i].AnglePoint[2].z);
		glVertex3f(sk_chunk[i].AnglePoint[3].x, sk_chunk[i].AnglePoint[3].y, sk_chunk[i].AnglePoint[3].z);

		glVertex3f(sk_chunk[i].AnglePoint[0].x, sk_chunk[i].AnglePoint[0].y, sk_chunk[i].AnglePoint[0].z);
		glVertex3f(sk_chunk[i].AnglePoint[1].x, sk_chunk[i].AnglePoint[1].y, sk_chunk[i].AnglePoint[1].z);
		glVertex3f(sk_chunk[i].AnglePoint[6].x, sk_chunk[i].AnglePoint[6].y, sk_chunk[i].AnglePoint[6].z);
		glVertex3f(sk_chunk[i].AnglePoint[5].x, sk_chunk[i].AnglePoint[5].y, sk_chunk[i].AnglePoint[5].z);
		/*
		glVertex3f(sk_chunk[1].AnglePoint[0].x, sk_chunk[1].AnglePoint[0].y, sk_chunk[1].AnglePoint[0].z);
		glVertex3f(sk_chunk[1].AnglePoint[1].x, sk_chunk[1].AnglePoint[1].y, sk_chunk[1].AnglePoint[1].z);
		glVertex3f(sk_chunk[1].AnglePoint[2].x, sk_chunk[1].AnglePoint[2].y, sk_chunk[1].AnglePoint[2].z);
		glVertex3f(sk_chunk[1].AnglePoint[3].x, sk_chunk[1].AnglePoint[3].y, sk_chunk[1].AnglePoint[3].z);

		glVertex3f(sk_chunk[1].AnglePoint[4].x, sk_chunk[1].AnglePoint[4].y, sk_chunk[1].AnglePoint[4].z);
		glVertex3f(sk_chunk[1].AnglePoint[5].x, sk_chunk[1].AnglePoint[5].y, sk_chunk[1].AnglePoint[5].z);
		glVertex3f(sk_chunk[1].AnglePoint[6].x, sk_chunk[1].AnglePoint[6].y, sk_chunk[1].AnglePoint[6].z);
		glVertex3f(sk_chunk[1].AnglePoint[7].x, sk_chunk[1].AnglePoint[7].y, sk_chunk[1].AnglePoint[7].z);

		glVertex3f(sk_chunk[1].AnglePoint[1].x, sk_chunk[1].AnglePoint[1].y, sk_chunk[1].AnglePoint[1].z);
		glVertex3f(sk_chunk[1].AnglePoint[2].x, sk_chunk[1].AnglePoint[2].y, sk_chunk[1].AnglePoint[2].z);
		glVertex3f(sk_chunk[1].AnglePoint[7].x, sk_chunk[1].AnglePoint[7].y, sk_chunk[1].AnglePoint[7].z);
		glVertex3f(sk_chunk[1].AnglePoint[6].x, sk_chunk[1].AnglePoint[6].y, sk_chunk[1].AnglePoint[6].z);

		glVertex3f(sk_chunk[1].AnglePoint[4].x, sk_chunk[1].AnglePoint[4].y, sk_chunk[1].AnglePoint[4].z);
		glVertex3f(sk_chunk[1].AnglePoint[5].x, sk_chunk[1].AnglePoint[5].y, sk_chunk[1].AnglePoint[5].z);
		glVertex3f(sk_chunk[1].AnglePoint[0].x, sk_chunk[1].AnglePoint[0].y, sk_chunk[1].AnglePoint[0].z);
		glVertex3f(sk_chunk[1].AnglePoint[3].x, sk_chunk[1].AnglePoint[3].y, sk_chunk[1].AnglePoint[3].z);

		glVertex3f(sk_chunk[1].AnglePoint[4].x, sk_chunk[1].AnglePoint[4].y, sk_chunk[1].AnglePoint[4].z);
		glVertex3f(sk_chunk[1].AnglePoint[7].x, sk_chunk[1].AnglePoint[7].y, sk_chunk[1].AnglePoint[7].z);
		glVertex3f(sk_chunk[1].AnglePoint[2].x, sk_chunk[1].AnglePoint[2].y, sk_chunk[1].AnglePoint[2].z);
		glVertex3f(sk_chunk[1].AnglePoint[3].x, sk_chunk[1].AnglePoint[3].y, sk_chunk[1].AnglePoint[3].z);

		glVertex3f(sk_chunk[1].AnglePoint[0].x, sk_chunk[1].AnglePoint[0].y, sk_chunk[1].AnglePoint[0].z);
		glVertex3f(sk_chunk[1].AnglePoint[1].x, sk_chunk[1].AnglePoint[1].y, sk_chunk[1].AnglePoint[1].z);
		glVertex3f(sk_chunk[1].AnglePoint[6].x, sk_chunk[1].AnglePoint[6].y, sk_chunk[1].AnglePoint[6].z);
		glVertex3f(sk_chunk[1].AnglePoint[5].x, sk_chunk[1].AnglePoint[5].y, sk_chunk[1].AnglePoint[5].z);
		*/
	}

	glEnd();
	



	//以三角面片的形式显示
	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0, 0.0, 0.0);
	//for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
	//	for (int j = 0; j < m_3DModel.vctMeshInfos[i].faceInfcounts; j++) {
	//		for (int k = 0; k < 3; k++) {
	//			glVertex3f(	m_3DModel.vctMeshInfos[i].vertInfos[m_3DModel.vctMeshInfos[i].faceInfos[j].triangleinfo->vertIndex[k]].Coordinates.x,
	//						m_3DModel.vctMeshInfos[i].vertInfos[m_3DModel.vctMeshInfos[i].faceInfos[j].triangleinfo->vertIndex[k]].Coordinates.y,
	//						m_3DModel.vctMeshInfos[i].vertInfos[m_3DModel.vctMeshInfos[i].faceInfos[j].triangleinfo->vertIndex[k]].Coordinates.z );
	//		}		
	//	}
	//}

	//for (int i = 0; i < 1; i++) {
	//	for (int j = 0; j < thisphysic.physics[i].particlenumbers; j++) {
	//		if (thisphysic.physics[i].pParticles[j].belongtoPart == 9) {	//属于左大臂
	//			glVertex3f(thisphysic.physics[i].pParticles[j].vPosition.x, thisphysic.physics[i].pParticles[j].vPosition.y, thisphysic.physics[i].pParticles[j].vPosition.z);
	//		}
	//	}
	//}
	//glEnd();

	return TRUE;

}

//袖子的对齐
CVector3 C3DSModel::SleeveAlign() {
	//只使用左胳膊
	CVector3 LeftArmTop, LeftArmBottom;
	LeftArmTop.x = (sk_chunk[9].vertex[0].Coordinates.x + sk_chunk[9].vertex[10].Coordinates.x) / 2;
	LeftArmTop.y = (sk_chunk[9].vertex[0].Coordinates.y + sk_chunk[9].vertex[10].Coordinates.y) / 2;
	LeftArmTop.z = (sk_chunk[9].vertex[0].Coordinates.z + sk_chunk[9].vertex[10].Coordinates.z) / 2;
	LeftArmBottom.x = (sk_chunk[0].vertex[0].Coordinates.x + sk_chunk[0].vertex[10].Coordinates.x) / 2;
	LeftArmBottom.y = (sk_chunk[0].vertex[0].Coordinates.y + sk_chunk[0].vertex[10].Coordinates.y) / 2;
	LeftArmBottom.z = (sk_chunk[0].vertex[0].Coordinates.z + sk_chunk[0].vertex[10].Coordinates.z) / 2;

	//float LeftArmLength = sqrt((LeftArmTop.x - LeftArmBottom.x)*(LeftArmTop.x - LeftArmBottom.x) + (LeftArmTop.y - LeftArmBottom.y)*(LeftArmTop.y - LeftArmBottom.y) + (LeftArmTop.z - LeftArmBottom.z)*(LeftArmTop.z - LeftArmBottom.z));
	//glLineWidth(5);
	//glBegin(GL_LINES);
	//glColor3f(0.0, 1.0, 0.0);
	//glVertex3f(LeftArmTop.x, LeftArmTop.y, LeftArmTop.z);
	//glVertex3f(LeftArmBottom.x, LeftArmBottom.y, LeftArmBottom.z);
	//glEnd();
	//glLineWidth(1);

	float ymin = FLT_MAX, ymax = -FLT_MAX;
	CVector3 Ymin, Ymax;
	CVector3 LeftSleeveTop, LeftSleeveBottom;
	CVector3 lst0, lst1, lsb0, lsb1;
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < thisphysic.physics[i].particlenumbers; j++) {
			if (thisphysic.physics[i].pParticles[j].belongtoPart == 9) {	//属于左大臂
				if (thisphysic.physics[i].pParticles[j].vPosition.y < ymin) {
					ymin = thisphysic.physics[i].pParticles[j].vPosition.y;
					Ymin.Set(thisphysic.physics[i].pParticles[j].vPosition.x, thisphysic.physics[i].pParticles[j].vPosition.y, thisphysic.physics[i].pParticles[j].vPosition.z);
				}
				if (thisphysic.physics[i].pParticles[j].vPosition.y > ymax) {
					ymax = thisphysic.physics[i].pParticles[j].vPosition.y;
					Ymax.Set(thisphysic.physics[i].pParticles[j].vPosition.x, thisphysic.physics[i].pParticles[j].vPosition.y, thisphysic.physics[i].pParticles[j].vPosition.z);
				}
			}
		}
	}
	lsb0.y = Ymin.y;
	lsb1.Set(Ymin.x, Ymin.y, Ymin.z);
	lst0.Set(Ymax.x, Ymax.y, Ymax.z);
	lst1.y = Ymax.y;
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < thisphysic.physics[i].particlenumbers; j++) {
			if (thisphysic.physics[i].pParticles[j].belongtoPart == 9) {	//属于左大臂
				//这里允许有一个误差值，这里的这个误差值对于其他衣服是否适用？？？
				if (abs(thisphysic.physics[i].pParticles[j].vPosition.x - Ymin.x) <= 0.08) {
					if (thisphysic.physics[i].pParticles[j].vPosition.y > lsb0.y) {
						lsb0.Set(thisphysic.physics[i].pParticles[j].vPosition.x, thisphysic.physics[i].pParticles[j].vPosition.y, thisphysic.physics[i].pParticles[j].vPosition.z);
					}
				}
				if (abs(thisphysic.physics[i].pParticles[j].vPosition.x - Ymax.x) <= 0.08) {
					if (thisphysic.physics[i].pParticles[j].vPosition.y < lst1.y) {
						lst1.Set(thisphysic.physics[i].pParticles[j].vPosition.x, thisphysic.physics[i].pParticles[j].vPosition.y, thisphysic.physics[i].pParticles[j].vPosition.z);
					}
				}
			}
		}
	}
	LeftSleeveTop.x = (lst0.x + lst1.x) / 2;
	LeftSleeveTop.y = (lst0.y + lst1.y) / 2;
	LeftSleeveTop.z = (lst0.z + lst1.z) / 2;
	LeftSleeveBottom.x = (lsb0.x +lsb1.x) / 2;
	LeftSleeveBottom.y = (lsb0.y + lsb1.y) / 2;
	LeftSleeveBottom.z = (lsb0.z + lsb1.z) / 2;
	float LeftSleeveLength = Distance(LeftSleeveTop, LeftSleeveBottom);//sqrt((LeftSleeveTop.x - LeftSleeveBottom.x)*(LeftSleeveTop.x - LeftSleeveBottom.x) + (LeftSleeveTop.y - LeftSleeveBottom.y)*(LeftSleeveTop.y - LeftSleeveBottom.y) + (LeftSleeveTop.z - LeftSleeveBottom.z)*(LeftSleeveTop.z - LeftSleeveBottom.z));
	//glLineWidth(5);
	//glBegin(GL_LINES);
	//glColor3f(1.0, 1.0, 0.0);
	//glVertex3f(LeftSleeveTop.x, LeftSleeveTop.y, LeftSleeveTop.z);
	//glVertex3f(LeftSleeveBottom.x, LeftSleeveBottom.y, LeftSleeveBottom.z);
	//glEnd();
	//glLineWidth(1);

	//Y和Z方向上的微调
	float adjust_Y, adjust_Z;
	CVector3 crossPoint;
	crossPoint.x = LeftSleeveTop.x;
	crossPoint.y = (LeftSleeveTop.x - LeftArmTop.x) * (LeftArmBottom.y - LeftArmTop.y) / (LeftArmBottom.x - LeftArmTop.x) + LeftArmTop.y;
	crossPoint.z = (LeftSleeveTop.x - LeftArmTop.x) * (LeftArmBottom.z - LeftArmTop.z) / (LeftArmBottom.x - LeftArmTop.x) + LeftArmTop.z;
	adjust_Y = crossPoint.y - LeftSleeveTop.y;
	adjust_Z = crossPoint.z - LeftSleeveTop.z;
	for (int i = 0; i < m_3DModel.numOfMeshInfos; i++) {
		for (int j = 0; j < m_3DModel.vctMeshInfos[i].vertInfcounts; j++) {
			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.y += adjust_Y;
			m_3DModel.vctMeshInfos[i].vertInfos[j].Coordinates.z += adjust_Z;
		}
	}
	float newLeftArmLength = Distance(crossPoint, LeftArmBottom);//sqrt((crossPoint.x - LeftArmBottom.x)*(crossPoint.x - LeftArmBottom.x) + (crossPoint.y - LeftArmBottom.y)*(crossPoint.y - LeftArmBottom.y) + (crossPoint.z - LeftArmBottom.z)*(crossPoint.z - LeftArmBottom.z));
	CVector3 destPoint;	//胳膊旋转后的目的坐标
	destPoint.x = LeftSleeveTop.x - (LeftSleeveTop.x - LeftSleeveBottom.x) * newLeftArmLength / LeftSleeveLength;
	destPoint.y = LeftSleeveTop.y - (LeftSleeveTop.y - LeftSleeveBottom.y) * newLeftArmLength / LeftSleeveLength-0.2;
	destPoint.z = LeftSleeveTop.z - (LeftSleeveTop.z - LeftSleeveBottom.z) * newLeftArmLength / LeftSleeveLength;

	return destPoint;
}

//通过碰撞检测将人体模型内的衣服“拉”出来
void C3DSModel::CollisionCheckForAlign(all_Physics *thisphysic, Human::Skeletonchunk *sk_chunk) {
	int i, j;
	int vert, parti;
	bool isinternal;
	CVector3 nNormal, vector, v, tempPC;
	Particle*particle;
	int temp = 21;
	for (i = 0; i<thisphysic->objectnumbers; i++){
		for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
			particle = &(thisphysic->physics[i].pParticles[j]);
			if (particle->bLocked == 0)
			{
				if (particle->isbrief)
				{//首先是简化的衣服模型的碰撞检测
					parti = particle->belongtoPart;
					isinternal = true;
					for (int k = 0; k<6; k++)
					{//先与OBB包围盒进行碰撞检测
						vert = sk_chunk[parti].Obbface[k].vert[0];
						vector = particle->vPosition - sk_chunk[parti].AnglePoint[vert];
						nNormal = sk_chunk[parti].Obbface[k].Normalize;
						if (Dot(vector, nNormal)>0){
							isinternal = false;
							break;
						}
					}
					if (isinternal)
					{
						particle->iscollision = true;//在OBB内，记为碰撞
					}

				}
			}
		}
	}

	int indexz, indexQ, index;
	bool needamplify[10];
	for (i = 0; i<10; i++)needamplify[i] = false;
	for (i = 0; i<thisphysic->objectnumbers; i++){
		for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
			particle = &(thisphysic->physics[i].pParticles[j]);
			if (particle->bLocked == 0)
			{
				if (particle->isbrief&&particle->iscollision)
				{//记为碰撞的点进一步与人体网格进行碰撞
					parti = particle->belongtoPart;
					particle->plocal_xyz.x = sk_chunk[parti].InvertofM[0] * particle->vPosition.x + sk_chunk[parti].InvertofM[4] * particle->vPosition.y + sk_chunk[parti].InvertofM[8] * particle->vPosition.z + sk_chunk[parti].InvertofM[12];
					particle->plocal_xyz.y = sk_chunk[parti].InvertofM[1] * particle->vPosition.x + sk_chunk[parti].InvertofM[5] * particle->vPosition.y + sk_chunk[parti].InvertofM[9] * particle->vPosition.z + sk_chunk[parti].InvertofM[13];
					particle->plocal_xyz.z = sk_chunk[parti].InvertofM[2] * particle->vPosition.x + sk_chunk[parti].InvertofM[6] * particle->vPosition.y + sk_chunk[parti].InvertofM[10] * particle->vPosition.z + sk_chunk[parti].InvertofM[14];
					particle->plocal_rQz.x = sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y + particle->plocal_xyz.z*particle->plocal_xyz.z);
					double z, y;
					z = particle->plocal_xyz.z;
					y = particle->plocal_xyz.y;
					if (z >= 0 && y>0)particle->plocal_rQz.y = atan(z / y);
					if (z>0 && y == 0)particle->plocal_rQz.y = PI*0.5;
					if (z<0 && y == 0)particle->plocal_rQz.y = PI*1.5;
					if (y<0)particle->plocal_rQz.y = PI + atan(z / y);
					if (z<0 && y>0)particle->plocal_rQz.y = 2 * PI + atan(z / y);
					particle->plocal_rQz.z = particle->plocal_xyz.x;

					indexz = (sk_chunk[parti].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[parti].dz;
					if (indexz == 21)indexz = 21 - 1;

					int bound;
					bound = 21;
					indexQ = particle->plocal_rQz.y / sk_chunk[parti].dQ;
					if (indexQ == bound)indexQ = 0;
					index = indexz*bound + indexQ;
					if (index<21 * bound&&index >= 0)
					{
						isinternal = true;
						for (int k = 0; k<sk_chunk[parti].Cylindricalmap[index].numofTri; k++)
						{
							int a = sk_chunk[parti].Cylindricalmap[index].Triindex[k];
							vert = sk_chunk[parti].triangle[a].vert[0];
							vector = particle->vPosition - sk_chunk[parti].vertex[vert].Convexhull;
							nNormal = sk_chunk[parti].triangle[a].Normalizehull;
							if (Dot(vector, nNormal)>0)
							{
								isinternal = false;
								break;
							}
						}
						if (isinternal){
							needamplify[parti] = true;//若某个部位中，衣服上有质点与人体碰撞了，则需进一步检测非简化点
							//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
							CVector3 p;
							if (parti == 1 || parti == 2){
								p.Set(0, particle->plocal_xyz.y, 0);
								particle->vprojection.x = sk_chunk[parti].Mobb[12];
								particle->vprojection.y = p.y + sk_chunk[parti].Mobb[13];
								particle->vprojection.z = sk_chunk[parti].Mobb[14];
							}
							else{
								p.Set(particle->plocal_xyz.x, 0, 0);
								particle->vprojection.x = sk_chunk[parti].Mobb[0] * p.x + sk_chunk[parti].Mobb[12];
								particle->vprojection.y = sk_chunk[parti].Mobb[1] * p.x + sk_chunk[parti].Mobb[13];
								particle->vprojection.z = sk_chunk[parti].Mobb[2] * p.x + sk_chunk[parti].Mobb[14];
							}
							particle->Cylindex = index;
						}
						else particle->iscollision = false;
					}
					else particle->iscollision = false;
				}

			}
		}
	}
	bool needall = false;
	i = 0;
	while (i<10){
		if (needamplify[i]){
			needall = true;
			break;
		}
		i++;
	}
	if (needall)
	{
		for (i = 0; i<thisphysic->objectnumbers; i++){
			for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
				particle = &(thisphysic->physics[i].pParticles[j]);
				if (particle->bLocked == 0)
				{
					if (!particle->isbrief){//非简化点与OBB包围盒的碰撞检测
						parti = particle->belongtoPart;
						if (needamplify[parti]){
							isinternal = true;
							for (int k = 0; k<6; k++){
								vert = sk_chunk[parti].Obbface[k].vert[0];
								vector = particle->vPosition - sk_chunk[parti].AnglePoint[vert];
								nNormal = sk_chunk[parti].Obbface[k].Normalize;
								if (Dot(vector, nNormal)>0){
									isinternal = false;
									break;
								}
							}
							if (isinternal)
							{
								particle->iscollision = true;

							}
						}
					}
				}
			}
		}
		for (i = 0; i<thisphysic->objectnumbers; i++)
		{
			for (j = 0; j<thisphysic->physics[i].particlenumbers; j++)
			{
				particle = &(thisphysic->physics[i].pParticles[j]);
				if (particle->bLocked == 0)
				{
					//if(!particle->isbrief&&particle->iscollision)
					if (particle->iscollision)
					{//非简化点与人体的碰撞检测
						for (int k = 0; k<10; k++)
						{
							if ((particle->belongtoPart) == k && (needamplify[k]))
							{
								float mindist = 10000.0;
								float tempdist;
								int triindex;
								for (int m = 0; m<sk_chunk[k].numofTri; m++)
								{
									CVector3 trian;
									trian = sk_chunk[k].vertex[sk_chunk[k].triangle[m].vert[0]].Convexhull;
									tempdist = Magnitude(particle->vPosition - trian);
									if (tempdist<mindist)
									{
										mindist = tempdist;
										triindex = m;
									}
								}
								vert = sk_chunk[k].triangle[triindex].vert[0];
								vector = particle->vPosition - sk_chunk[k].vertex[vert].Convexhull;
								nNormal = sk_chunk[k].triangle[triindex].Normalizehull;
								if (Dot(vector, nNormal)>0)
								{
									isinternal = false;
									break;
								}
								//if(isinternal)
								{
									particle->iscollision = true;
									//if(isinside(tempPC,sk_chunk,k,triindex))
									{
										particle->vPosition = sk_chunk[k].vertex[sk_chunk[k].triangle[triindex].vert[0]].Convexhull;
										particle->vprePosition = particle->vPosition;
										particle->iscollision = false;
										particle->bLocked = 1;
										break;
									}
									//else particle->iscollision=false;

								}
								//else particle->iscollision=false;
								//temp=21;
								//particle->plocal_xyz.x=particle->vPosition.x+sk_chunk[k].InvertofM[12];
								//particle->plocal_xyz.y=particle->vPosition.y+sk_chunk[k].InvertofM[13];
								//particle->plocal_xyz.z=particle->vPosition.z+sk_chunk[k].InvertofM[14];
								//particle->plocal_rQz.x=sqrt(particle->plocal_xyz.x*particle->plocal_xyz.x+particle->plocal_xyz.z*particle->plocal_xyz.z);
								//double z,x;
								//z=particle->plocal_xyz.z;
								//x=particle->plocal_xyz.x;
								//if(z>=0&&x>0)particle->plocal_rQz.y=atan(z/x);
								//if(z>0&&x==0)particle->plocal_rQz.y=PI*0.5;
								//if(z<0&&x==0)particle->plocal_rQz.y=PI*1.5;
								//if(x<0)particle->plocal_rQz.y=PI+atan(z/x);
								//if(z<0&&x>0)particle->plocal_rQz.y=2*PI+atan(z/x);
								//particle->plocal_rQz.z=particle->plocal_xyz.y;

								//indexz=(sk_chunk[k].z_min_max[1]-particle->plocal_rQz.z)/sk_chunk[k].dz;
								//if(indexz==21)indexz=20;
								//indexQ=particle->plocal_rQz.y/sk_chunk[k].dQ;
								//if(indexQ==temp)indexQ=0;
								//index=indexz*temp+indexQ;
								//if(index<21*temp&&index>=0)
								//{
								//	isinternal=true;
								//	for(int m=0;m<sk_chunk[k].Cylindricalmap[index].numofTri;m++)
								//	{
								//		int a=sk_chunk[k].Cylindricalmap[index].Triindex[m];
								//		vert=sk_chunk[k].triangle[a].vert[0];
								//		vector=particle->vPosition-sk_chunk[k].vertex[vert].Convexhull;
								//		nNormal=sk_chunk[k].triangle[a].Normalizehull;
								//		if(Dot(vector,nNormal)>0)
								//		{
								//			isinternal=false;
								//			break;
								//		}
								//		if(isinternal)
								//		{
								//			particle->iscollision=true;
								//			CVector3 p;
								//			p.Set(0,particle->plocal_xyz.y,0);
								//			particle->vprojection.x=sk_chunk[k].Mobb[12];
								//			particle->vprojection.y=p.y+sk_chunk[k].Mobb[13];
								//			particle->vprojection.z=sk_chunk[k].Mobb[14];
								//			particle->Cylindex=index;
								//			nNormal=sk_chunk[k].triangle[a].Normalizehull;
								//			double t=(particle->vPosition.x*nNormal.x+particle->vPosition.y*nNormal.y+particle->vPosition.z*nNormal.z+sk_chunk[k].triangle[a].dhull)/(vector.x*nNormal.x+vector.y*nNormal.y+vector.z*nNormal.z);
								//			tempPC.x=particle->vPosition.x-t*vector.x;
								//			tempPC.y=particle->vPosition.y-t*vector.y;
								//			tempPC.z=particle->vPosition.z-t*vector.z;
								//			//if(isinside(tempPC,sk_chunk,k,a))
								//			{

								//				//particle->vPosition=tempPC+nNormal*0.05;//线衫1： *0.3
								//				//particle->vPosition=tempPC;
								//				particle->vPosition=particle->vprePosition;
								//				particle->vprePosition=particle->vPosition;
								//				particle->iscollision=false;
								//				particle->bLocked=1;
								//				break;
								//			}
								//		}
								//		particle->iscollision=false;
								//	}
								//}else particle->iscollision=false;
							}
						}
					}
				}
			}
		}
	}
}

bool C3DSModel::Drawbody(){//显示人体网格模型
	//*--------------
	CVector3 temp,temp2;
	Particle *particle;

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glColor3ub(0,0,0);//205,186,150---244 164 96--210 180 140 -- 245,222,179   0,0,0
	int j;
	for(int i=0;i<10;i++)
	{
		
		glBegin(GL_TRIANGLES);
		glColor3ub(0, 0, 0);
		for(j=0;j<sk_chunk[i].numofTri;j++)
		{
				for(int s=0;s<3;s++)
				{
					int index=sk_chunk[i].triangle[j].vert[s];
					glNormal3f(sk_chunk[i].vertex[ index ].Normalize.x, sk_chunk[i].vertex[ index ].Normalize.y, sk_chunk[i].vertex[ index ].Normalize.z);
					glVertex3f(sk_chunk[i].vertex[ index ].Coordinates.x,sk_chunk[i].vertex[ index ].Coordinates.y,sk_chunk[i].vertex[ index ].Coordinates.z);
				}
		}
		glEnd();

		
		//glPointSize(8);
		//glBegin(GL_POINTS);
		//
		//glColor3f(0.0,1.0,0.0); //最小值设置颜色为绿色
		//glVertex3f(sk_chunk[i].Min.x,sk_chunk[i].Min.y,sk_chunk[i].Min.z);
		//glColor3f(1.0,0.0,0.0);//最大值颜色为红色
		//glVertex3f(sk_chunk[i].Max.x,sk_chunk[i].Max.y,sk_chunk[i].Max.z);

		//glEnd();
		
		//glBegin(GL_LINES);
		//glLineWidth(6);//设置线的宽度为6
		//glColor3f(1.0,0.0,0.0); //设置颜色为绿色
		//if(i==3)
		//{
		//	glVertex3f(sk_chunk[i].Mobb[12],sk_chunk[i].Mobb[13],sk_chunk[i].Mobb[14]);
		//	glVertex3f(sk_chunk[i].Mobb[4],sk_chunk[i].Mobb[5],sk_chunk[i].Mobb[6]);
		//}
		//glEnd();
		//glPointSize(10);//设置点的大小为10
		//glBegin(GL_POINTS);
		////if(i!=1&&i!=2)
		//{
		//	glColor3ub(255,0,0);
		//	//glVertex3f(sk_chunk[i].Locenter.x,sk_chunk[i].Locenter.y,sk_chunk[i].Locenter.z);
		//	glVertex3f(sk_chunk[i].Mobb[12],sk_chunk[i].Mobb[13],sk_chunk[i].Mobb[14]);
		//	glVertex3f(sk_chunk[i].Mobb[12]+sk_chunk[i].Mobb[0],sk_chunk[i].Mobb[13]+sk_chunk[i].Mobb[1],sk_chunk[i].Mobb[14]+sk_chunk[i].Mobb[2]);
		//	//glVertex3f(sk_chunk[i].Mobb[12]+sk_chunk[i].Mobb[4],sk_chunk[i].Mobb[13]+sk_chunk[i].Mobb[5],sk_chunk[i].Mobb[14]+sk_chunk[i].Mobb[6]);
		//	//glVertex3f(sk_chunk[i].Mobb[12]+sk_chunk[i].Mobb[8],sk_chunk[i].Mobb[13]+sk_chunk[i].Mobb[9],sk_chunk[i].Mobb[14]+sk_chunk[i].Mobb[10]);
		//}
		//glEnd();
		//glBegin(GL_TRIANGLES);
		//glColor3ub(0,0,0);
		//for(j=0;j<sk_chunk[i].numofTri;j++){//
		//		for(int s=0;s<3;s++){
		//			int index=sk_chunk[i].triangle[j].vert[s];
		//			glNormal3f(sk_chunk[i].vertex[ index ].Normalize.x, sk_chunk[i].vertex[ index ].Normalize.y, sk_chunk[i].vertex[ index ].Normalize.z);
		//			glVertex3f(sk_chunk[i].vertex[ index ].Convexhull.x,sk_chunk[i].vertex[ index ].Convexhull.y,sk_chunk[i].vertex[ index ].Convexhull.z);
		//		}
		//}
		//glEnd();
		//i=1;
		//glColor3f(0.0,1.0,0.0); //设置颜色为绿色
		//glLineWidth(1);//设置线的宽度为6
		//glBegin(GL_LINES);

		//for(int k=0;k<sk_chunk[i].numofTri;k++)
		//{
		//	//if(i==2)
		//	{
		//	glVertex3f(sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.x,sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.y,sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.z);
		//	glVertex3f(sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.x+1*sk_chunk[i].triangle[k].Normalize.x,sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.y+1*sk_chunk[i].triangle[k].Normalize.y,sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.z+1*sk_chunk[i].triangle[k].Normalize.z);
		//	}
		//}
		//glEnd();

		//for(int k=0;k<sk_chunk[i].numofTri;k++)
		//{
		//	//if(i==1)
		//	{
		//	glVertex3f(sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.x,sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.y,sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.z);
		//	glVertex3f(sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.x+1*sk_chunk[i].triangle[k].Normalizehull.x,sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.y+1*sk_chunk[i].triangle[k].Normalizehull.y,sk_chunk[i].vertex[sk_chunk[i].triangle[k].vert[0]].Coordinates.z+1*sk_chunk[i].triangle[k].Normalizehull.z);
		//	}
		//}
		//glEnd();
		/*
		glPointSize(10);//设置点的大小为10
		glBegin(GL_POINTS);
		if(i==1||i==2)
		{
			glColor3ub(255,0,0);
			glVertex3f(sk_chunk[i].vertex[2].Convexhull.x,sk_chunk[i].vertex[2].Convexhull.y,sk_chunk[i].vertex[2].Convexhull.z);
			glColor3ub(0,255,0);
			glVertex3f(sk_chunk[i].vertex[12].Convexhull.x,sk_chunk[i].vertex[12].Convexhull.y,sk_chunk[i].vertex[12].Convexhull.z);
		}
		if(i==7||i==9)//对于大臂部分
		{
			glColor3ub(255,0,0);
			glVertex3f(sk_chunk[i].vertex[0+21*3].Convexhull.x,sk_chunk[i].vertex[0+21*3].Convexhull.y,sk_chunk[i].vertex[0+21*3].Convexhull.z);
			glColor3ub(0,255,0);
			glVertex3f(sk_chunk[i].vertex[10+21*3].Convexhull.x,sk_chunk[i].vertex[10+21*3].Convexhull.y,sk_chunk[i].vertex[10+21*3].Convexhull.z);
		}
		if(i==8||i==0)//对于小臂部分
		{
			glColor3ub(255,0,0);
			glVertex3f(sk_chunk[i].vertex[0+21*10].Convexhull.x,sk_chunk[i].vertex[0+21*10].Convexhull.y,sk_chunk[i].vertex[0+21*10].Convexhull.z);
			glColor3ub(0,255,0);
			glVertex3f(sk_chunk[i].vertex[10+21*10].Convexhull.x,sk_chunk[i].vertex[10+21*10].Convexhull.y,sk_chunk[i].vertex[10+21*10].Convexhull.z);
		}
		if(i>2&&i<7)//对于腿部
		{
			glColor3ub(255,0,0);
			glVertex3f(sk_chunk[i].vertex[0].Convexhull.x,sk_chunk[i].vertex[0].Convexhull.y,sk_chunk[i].vertex[0].Convexhull.z);
			glColor3ub(0,255,0);
			glVertex3f(sk_chunk[i].vertex[10].Convexhull.x,sk_chunk[i].vertex[10].Convexhull.y,sk_chunk[i].vertex[10].Convexhull.z);
		}
		glEnd();
		*/
		/*
		glBegin(GL_LINES);
		for(int i=0;i<10;i++)
		{
		glColor3ub(255,0,0);
		//int i=5;
		if(i==1||i==2)
			temp=(sk_chunk[i].vertex[10*21+2].Coordinates+sk_chunk[i].vertex[10*21+12].Coordinates)/2.0;
		else
			temp=(sk_chunk[i].vertex[10*21].Coordinates+sk_chunk[i].vertex[10*21+10].Coordinates)/2.0;
		temp2=temp+sk_chunk[i].v*10.0;
		glVertex3d(temp.x,temp.y,temp.z);
		glVertex3d(temp2.x,temp2.y,temp2.z);
		}
		glEnd();
		*/
		//glBegin(GL_LINE_STRIP);
		////for(int k=0;k<10;k++)
		//glColor3ub(255,0,0);
		//for(int j=0;j<8;j++)
		//{
		//	glVertex3f(sk_chunk[i].AnglePoint[j].x,sk_chunk[i].AnglePoint[j].y,sk_chunk[i].AnglePoint[j].z);
		//}
		//glEnd();
	}

	




	//		glPointSize(8);
	//	glBegin(GL_POINTS);
	//	glColor3f(1.0,0.0,0.0); //设置颜色为绿色
	//	for(int k=0;k<thisphysic.physics[0].particlenumbers;k++)
	//	{
	//		//if(i==2)
	//		//for(int m=0;m<6;m++)
	//		{
	//			glVertex3f(thisphysic.physics[0].pParticles[k].vprojection.x,thisphysic.physics[0].pParticles[k].vprojection.y,thisphysic.physics[0].pParticles[k].vprojection.z);
	//			
	//		}
	//	}
	//	glEnd();
		//glBegin(GL_LINES);
		//double t;
		//for(j=0;j<thisphysic.physics[0].particlenumbers;j++)
		//{

		//	particle=&(thisphysic.physics[0].pParticles[j]);
		//	if(particle->vTempPC.x!=0&&particle->vTempPC.y!=0&&particle->vTempPC.z!=0)
		//	{
		//	glColor3f(0.0,1.0,0.0); //设置颜色为绿色
		//	glPointSize(6);		
		//	glVertex3f(particle->vPosition.x,particle->vPosition.y,particle->vPosition.z);
		//	glPointSize(6);	
		//	glColor3f(1.0,0.0,0.0); //设置颜色为红色
		//	glVertex3f(particle->vTempPC.x,particle->vTempPC.y,particle->vTempPC.z);
		//	}
		//}
		//glEnd();

	//glBegin(GL_LINES);
	//glVertex3f(165, 29, -2.6);
	//glVertex3f(169, 144, -2.6);
	//glEnd();

	return TRUE;
}
// 载入模型文件
bool C3DSModel::LoadModelFromFile(char* sfilename,int key)
{
	double time;
	ofstream outStream;
	LARGE_INTEGER BegainTime ;   
    LARGE_INTEGER EndTime ;   
    LARGE_INTEGER Frequency ; 

//静态序号
	static int ntextID=0;
	char stmp[512];
	//把文件名连接到媒体路径，成为完整路径
	strcpy(m_sFileName,sfilename);
	strcpy(stmp,g_sMediaPath);
	strcat(stmp,sfilename);
	
	//在这使用加载类
	CLoad3DS cLoad3ds;
	//载入文件

	//QueryPerformanceFrequency(&Frequency);   
    //QueryPerformanceCounter(&BegainTime) ;

	if(cLoad3ds.Import3DS(&m_3DModel, stmp,key)==false)	//读取衣服模型的3ds文件，将衣服的质点，三角面片信息存入m_3DModel中，添加边信息，创建邻接表和为简化保留下的质点进行简化标记
	{
		//失败
		m_bModelOK=FALSE;
		return FALSE;
	}

	//成功
	for(int i = 0; i < m_3DModel.numOfMaterials; i++)
	{
		// Check to see if there is a file name to load in this material
		if(strlen(m_3DModel.vctMaterials[i].strFile) > 0)
		{
			strcpy(stmp,g_sMediaPath);
			strcat(stmp,m_3DModel.vctMaterials[i].strFile);
			//此处改动，设置材料号
			CreateTexture(g_Texture,stmp , ntextID);	
			m_3DModel.vctMaterials[i].texureId = g_Texture[ntextID];
			ntextID++;

		}
		else
			m_3DModel.vctMaterials[i].texureId = 0;
	}
	//为人体的10各部位分配内存
	sk_chunk=new Human::Skeletonchunk[10];
	memset(sk_chunk, 0, sizeof(Human::Skeletonchunk) * 10);
	Human h;
	h.Init(false);	//设置模型的性别 true 女性 false 男性
	//生成人体网格模型
	h.Calculation();//构造人体burbs曲面，并获取人体的u、v方向的点存于数据结构tempchunk[10]中

	//float newPosition[3] = { 30.0f, 40.0f, 0.0f };
	//h.SetArmToNewPosition(1, 1, newPosition);

	h.FillSKMesh(sk_chunk);//根据tempchunk[10]中的点信心，生成人体网格，以及OBB和圆柱映射表等


	m_bModelOK=TRUE;
	CMSModel m_MSModel;
	
	CVector3 kneePoint;

	if (key == 0) {	//上衣
		//衣服和人体的对齐，修改的是邻接表
		AlignmentClothesAndHuman();
	}
	else if (key == 1) {	//裤子
		kneePoint = AlignmentTrousesAndHuman();
		float newKneePosition[3] = { kneePoint.x / SCALESIZE, kneePoint.y / SCALESIZE, kneePoint.z / SCALESIZE };
		h.SetLegToNewPosition(-1, 1, newKneePosition);
	}
	
	
	//根据m_3DModel的信息来初始化m_MSModel中的thisphysic中的部分信息
	m_MSModel.Initialize(&thisphysic,m_3DModel,sk_chunk,key);//根据m_3DModel的信息，存于thisphysic中，并设置质点质量，弹簧弹力等基本信息

	

	//胳膊移动来配合衣服
	CVector3 ElbowPoint = SleeveAlign();
	float newElbowPosition[3] = { ElbowPoint.x / SCALESIZE, ElbowPoint.y / SCALESIZE, ElbowPoint.z / SCALESIZE };
	h.SetArmToNewPosition(-1, 1, newElbowPosition);
	h.FillSKMesh(sk_chunk);

	CollisionCheckForAlign(&thisphysic, sk_chunk);
    
	//根据m_3DModel的信息来创建m_MSModel中的thisphysic中的弹簧

	//修改的地方
//	m_MSModel.PreExam(&thisphysic,sk_chunk,key);//完成衣服与人体的对齐
	m_MSModel.CreateSpring(&thisphysic,m_3DModel);//根据m_3DModel中构造的邻接表信息，为thisphysic中的衣服构造质点弹簧模型
	
	

	return TRUE;
}
