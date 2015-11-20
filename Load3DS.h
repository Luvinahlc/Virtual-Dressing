// Load3DS.h: interface for the CLoad3DS class.
//     ԭʼ�������ԣ�GameTutorials
//     �ıࣺ
//     �й�3D������������
//     http://www.c3dn.net
//     mengmakies@live.cn
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOAD3DS_H__69E7E7AC_7A07_4479_9687_AC19CED0E3CF__INCLUDED_)
#define AFX_LOAD3DS_H__69E7E7AC_7A07_4479_9687_AC19CED0E3CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3dmath.h"
#include"D:\examples\src\shared\math3d.h"
//����STL
#include <vector>
using namespace std;

//>------ Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

//>------ Main Chunks
#define OBJECTINFO    0x3D3D				// This gives the version of the mesh and is found right before the material and object information
#define VERSION       0x0002				// This gives the version of the .3ds file
#define EDITKEYFRAME  0xB000				// This is the header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF				// This stored the texture info
#define OBJECT		  0x4000				// This stores the faces, vertices, etc...

//>------ sub defines of MATERIAL
#define MATNAME       0xA000				// This holds the material name
#define MATDIFFUSE    0xA020				// This holds the color of the object/material
#define MATMAP        0xA200				// This is a header for a new material
#define MATMAPFILE    0xA300				// This holds the file name of the texture

#define OBJECT_MESH   0x4100				// This lets us know that we are reading a new object

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110			// The objects vertices
#define OBJECT_FACES		0x4120			// The objects faces
#define OBJECT_MATERIAL		0x4130			// This is found if the object has a material, either texture map or color
#define OBJECT_UV			0x4140			// The UV texture coordinates


// This class handles all of the loading code
class CLoad3DS
{
//���⼸���ṹ�������������棬ʹ��װ������
public:
	// Here is our structure for our 3DS indicies (since .3DS stores 4 unsigned shorts)
	struct tIndices {							

		unsigned short a, b, c, bVisible;		// This will hold point1, 2, and 3 index's into the vertex array plus a visible flag
	};

	// This holds the chunk info
	struct tChunk
	{
		unsigned short int ID;					// The chunk's ID		
		unsigned int length;					// The length of the chunk
		unsigned int bytesRead;					// The amount of bytes read within that chunk
	};

	// This is our face structure.  This is is used for indexing into the vertex	
	// and texture coordinate arrays.  From this information we know which vertices
	// from our vertex array go to which face, along with the correct texture coordinates.
	struct tEdge{                    //��ӵı߽ṹ
		int flag;                    //��Ǹñߵ����ͣ�1��ʾ���ߣ�2��ʾ��ߣ�3��ʾб��,4��ʾ����Ϊ��ıߣ���������»���֣�
		int edgenumber;              //�ñߵ����
		int vertIndex[2];            //�ñߵ���������������
		int coordIndex[2];           //�ñߵ������������������������
		int AdjFaceCounts;           //�ñ����������εĸ���
		vector<int> AdjFacesIndex;   //��¼�ñ����������ε�������
	};

	struct tFace
	{
		int vertIndex[3];			// indicies for the verts that make up this triangle
		float ybound[2];            //��¼ÿ����������y�����ϵ����½�
		int coordIndex[3];			// indicies for the tex coords to texture this face
		int edgeIndex[3];           //����������ߵ�������
		CVector3 Normalize;       //�����εĵ�λ������
		//CVector3 Anothervetex[4]; //�ֱ��¼�������ε������е����������

		M3DVector4d A[4];         //���Ա�ʾ�������η�������d���ɵ�һ������
		M3DVector4d Q[4];         //��������Ƭ��������
		//M3DVector4d InvertofQ[4]; //Q������3*3����������
		float d;                  //d��ʾ������������ƽ���ƽ�淽�̳���
		float area;               //�����ε����
		int implicit;             //�����ε��������
		//bool haveInOfA;
		
	};

	// This holds the information for a material.  It may be a texture map of a color.
	// Some of these are not used, but I left them because you will want to eventually
	// read in the UV tile ratio and the UV tile offset for some models.
	struct tMaterialInfo
	{
		char  strName[255];			// The texture name
		char  strFile[255];			// The texture file name (If this is set it's a texture map)
		unsigned char  color[3];				// The color of the object (R, G, B)
		int   texureId;				// the texture ID
		float uTile;				// u tiling of texture  (Currently not used)
		float vTile;				// v tiling of texture	(Currently not used)
		float uOffset;			    // u offset of texture	(Currently not used)
		float vOffset;				// v offset of texture	(Currently not used)
	} ;
	//����������
	class tMatREF
	{
	public:
		int nMaterialID;
		unsigned short *pFaceIndexs;
		int nFaceNum;
		bool bHasTexture;
	public:
		tMatREF()
		{
			nMaterialID=-1;
			nFaceNum=0;
			pFaceIndexs=NULL;
			bHasTexture=false;
		}
	};

	// This holds all the information for our model/scene. 
	// You should eventually turn into a robust class that 
	// has loading/drawing/querying functions like:
	// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
	struct t3DObject 
	{
		int  numOfVerts;			// The number of verts in the model
		int  numOfEdges;			// ��ģ�͵ı���
		int  numOfFaces;			// The number of faces in the model
		int  numTexVertex;			// The number of texture coordinates
		int  numOfMaterials;		// the ��������
		int  materialID;			// The texture ID to use, which is the index into our texture array
		bool bHasTexture;			// This is TRUE if there is a texture map for this object
		char strName[255];			// The name of the object
		CVector3  *pVerts;			// The object's vertices
		CVector3  *pNormals;		// The object's normals
		CVector2  *pTexVerts;		// The texture's UV coordinates
		tEdge *pEdges;
		tFace *pFaces;				// The faces information of the object
		tMatREF	      *pMaterialREFS;// ����IDS
		
	};

public://�ڽӱ��������ݽṹ����
	struct vertInfo{                   //�ڽӱ�ĵ�洢�ṹ��Ϣ
		CVector3  Coordinates;         //�õ������
		int AdjVertCounts;             //�ڽӵ�ĸ���
		int AdjEdgeCounts;             //�ڽӱߵĸ���
		int AdjFaceCounts;             //�ڽ���ĸ���
		vector<int> AdjPointsIndex;    //�õ���ڽӵ��������
		vector<int> AdjEdgesIndex;     //�õ���ڽӱߵ�������
		vector<int> AdjFacesIndex;     //�õ���ڽ����������
    };

	struct edgeInfo{                   //�ڽӱ�ıߴ洢�ṹ��Ϣ
		tEdge *polesinfo;              //�ñߵĶ˵� �Լ��ڽ�����Ϣ
		int AdjEdgeCounts;             //�ڽӱߵĸ���
		vector<int> AdjEdgesIndex;     //�ñߵ��ڽӱߵ�������
	};

	struct faceInfo{                   //�ڽӱ����洢�ṹ��Ϣ
		tFace *triangleinfo;           //�����������Ϣ
        int AdjFaceCounts;             //�ڽ���ĸ���
		vector<int> AdjFacesIndex;     //������ڽ����������


	};

	struct  MeshInfo{               //��ά�����������Ϣ
		int vertInfcounts;          //�ڽӱ�ĵ�洢�ṹ��Ϣ�ĸ���
		int edgeInfcounts;          //�ڽӱ�ıߴ洢�ṹ��Ϣ�ĸ���
		int faceInfcounts;          //�ڽӱ����洢�ṹ��Ϣ�ĸ���
		vertInfo *vertInfos;        //�ڽӱ�����е���Ϣ
		edgeInfo *edgeInfos;        //�ڽӱ�����б���Ϣ
		faceInfo *faceInfos;        //�ڽӱ����������Ϣ
		faceInfo *brifaceInfos;     //�ڽӱ�����м�������Ƭ��Ϣ,���ڼ���ײ���
		vertInfo *brivertInfos;     //�ڽӱ�����м򻯵���Ϣ
		edgeInfo *briedgeInfos;     //�ڽӱ�����м򻯱���Ϣ����������Ƭ�ϵģ�
		int briedgeNum;            //�򻯱ߵĸ���

	};
	// This holds our model information.  This should also turn into a robust class.
	// We use STL's (Standard Template Library) vector class to ease our link list burdens. :)
	struct t3DModel 
	{
		int numOfObjects;					// The number of objects in the model
		int numOfMaterials;					// The number of materials for the model
		int numOfMeshInfos;
		//������	
		vector<tMaterialInfo> vctMaterials;	// The list of material information (Textures and colors)
		vector<t3DObject> vctObjects;		// The object list for our model
		vector<MeshInfo> vctMeshInfos;      // ���ڴ洢���ж�����ڽӱ�
	};
public:
	//����
	CLoad3DS();								
	//����
	~CLoad3DS();
	// ����3ds�ļ�
	bool Import3DS(t3DModel *pModel, char *strFileName,int key);

private:
	// This reads in a string and saves it in the char array passed in
	int GetString(char *);

	// This reads the next chunk
	void ReadChunk(tChunk *);

	// This reads the next large chunk
	void ProcessNextChunk(t3DModel *pModel, tChunk *,int key);

	// This reads the object chunks
	void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *,int key);

	// This reads the material chunks
	void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);

	// This reads the RGB value for the object's color
	void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk);

	// This reads the objects vertices
	void ReadVertices(t3DObject *pObject, tChunk *,int key);

	// This reads the objects face information
	void ReadVertexIndices(t3DObject *pObject, tChunk *);

	// This reads the texture coodinates of the object
	void ReadUVCoordinates(t3DObject *pObject, tChunk *);

	// This reads in the material name assigned to the object and sets the materialID
	void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk,vector<tMatREF*> *pvmatids);
	
	// This computes the vertex normals for the object (used for lighting)
	void ComputeNormals(t3DModel *pModel);//�ú���ͬʱʵ����Ϊ������Ƭ���������Ϣ�Ĺ���

	//���ö����еı���Ϣ
	void SetObjEdge(t3DModel *pModel);

	//�����򻯱���Ϣ
	void CreateBriEdge(t3DModel *pModel,int i);

	//����ģ�͵����ж�����ڽӱ�
	void CreateAdjList(t3DModel *pModel);

	//��������������Ϣ
	void CreateBrifTri(t3DModel *pModel,int i);
	void VetexQ(t3DModel *pModel,int i,int index,M3DVector4d Q[]);
	void briTriNormalize_d(t3DModel *pModel,int i,int triindex);
	// This frees memory and closes the file
	void CleanUp();
	
	// The file pointer
	FILE *m_FilePointer;

	double m3dVectorMulMatrix44( M3DVector4d v,M3DVector4d M[]){
		double sum=0,s[4]={0,0,0,0};
		int i,j;
	
		for(i=0;i<4;i++){
			for(j=0;j<4;j++)
				s[i]+=v[j]*M[j][i];
			sum+=s[i]*v[i];
		}
		return sum;
	}
	
	int min_eg(M3DVector3d eg[],int length){//���eg�е���Сֵ��������
		int i,k=0;
		while(k<length&&eg[k][1]!=1)k++;
		for(i=k+1;i<length;i++){
			if(eg[i][1]==1&&eg[i][0]<eg[k][0])k=i;
		}
		if(k==length)return -1;
		else return k;
	}
};


#endif // !defined(AFX_LOAD3DS_H__69E7E7AC_7A07_4479_9687_AC19CED0E3CF__INCLUDED_)
