#include "stdafx.h"
#include "Mass-SpringModel.h"

int ClothThickness = 0.05;	//布料的厚度


//逼近性检测 点面之间
void ApproximationSelfCollisionDetection(Particle *particle, CLoad3DS::t3DModel *pModel) {
	// If there are no objects, we can skip this part
	if (pModel->numOfObjects <= 0)
		return;
	CVector3 vFace[3];
	for (int index = 0; index < pModel->numOfObjects; index++) {
		CLoad3DS::t3DObject *pObject = &(pModel->vctObjects[index]);

		for (int i = 0; i < pObject->numOfFaces; i++) {
			vFace[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vFace[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vFace[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			if (Dot(particle->vPosition, pObject->pFaces[i].Normalize) < ClothThickness) {
				float w1, w2;
				CVector3 v13, v23, v03;
				v13 = Vector(vFace[0], vFace[2]);
				v23 = Vector(vFace[1], vFace[2]);
				v03 = Vector(particle->vPosition, vFace[2]);

				float tempa = Dot(v13, v13);
				float tempb = Dot(v13, v23);
				float tempc = Dot(v23, v23);
				float tempx = Dot(v03, v13);
				float tempy = Dot(v03, v23);

				w1 = (tempx*tempc - tempy*tempb) / (tempa*tempc - tempb*tempb);
				w2 = (tempy*tempa - tempx*tempb) / (tempa*tempc - tempb*tempb);

				if (0 <= w1 && w1 <= 1 && 0 <= w2 && w2 <= 1 && w1 + w2 <= 1) {	//投影落在三角形内
					CVector3 Projection;
					Projection.x = w1*vFace[0].x + w2*vFace[1].x + (1 - w1 - w2)*vFace[2].x;
					Projection.y = w1*vFace[0].y + w2*vFace[1].y + (1 - w1 - w2)*vFace[2].y;
					Projection.z = w1*vFace[0].z + w2*vFace[1].z + (1 - w1 - w2)*vFace[2].z;
					if (Distance(particle->vPosition, Projection) < ClothThickness) {
						particle->isSelfCollision = true;	//判断为自碰撞
					}
				}
				//不在三角形内则放到边边检测中
			}
		}
	}
}

//逼近性检测 边边之间
void ApproximationSelfCollisionDetection(CLoad3DS::t3DModel *pModel){
	if (pModel->numOfObjects <= 0)
		return;
	for (int index = 0; index < pModel->numOfObjects; index++) {
		CLoad3DS::t3DObject *pObject = &(pModel->vctObjects[index]);

		for (int i = 0; i < pObject->numOfEdges-1; i++) {
			CVector3 vEdge1[2], vEdge2[2];
			vEdge1[0] = pObject->pVerts[pObject->pEdges[i].vertIndex[0]];
			vEdge1[1] = pObject->pVerts[pObject->pEdges[i].vertIndex[1]];
			for (int j = i + 1; j < pObject->numOfEdges; j++) {
				vEdge2[0] = pObject->pVerts[pObject->pEdges[j].vertIndex[0]];
				vEdge2[1] = pObject->pVerts[pObject->pEdges[j].vertIndex[1]];
				CVector3 edge1, edge2;
				edge1 = Vector(vEdge1[0],vEdge1[1]);
				edge2 = Vector(vEdge2[0],vEdge2[1]);
				if (Dot(edge1, edge2) == 0) {	//两条边平行或在同一条直线上
					//particle->isSelfCollision = true;	//判断为自碰撞
				}
			}
		}
	}
}


//相交检测
void IntersectionSelfCollisionDetection(all_Physics *thisphysic) {

}

void SelfCollisionDetdction(all_Physics *thisphysic) {
	Particle *particle;

	for (int i = 0; i < thisphysic->objectnumbers; i++) {
		for (int j = 0; j < thisphysic->physics[i].particlenumbers; j++) {
			//利用三角形表面曲率简化计算，当曲率小于某个阈值时，不可能发生碰撞

			particle = &(thisphysic->physics[i].pParticles[j]);

		}
	}
}