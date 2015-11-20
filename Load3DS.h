// Load3DS.h: interface for the CLoad3DS class.
//     原始代码来自：GameTutorials
//     改编：
//     中国3D技术开发社区
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
//向量STL
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
//把这几个结构体声明到类里面，使封装更彻底
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
	struct tEdge{                    //添加的边结构
		int flag;                    //标记该边的类型，1表示竖边，2表示横边，3表示斜边,4表示长度为零的边（特殊情况下会出现）
		int edgenumber;              //该边的序号
		int vertIndex[2];            //该边的两个顶点索引号
		int coordIndex[2];           //该边的两个顶点的纹理坐标索引号
		int AdjFaceCounts;           //该边所属三角形的个数
		vector<int> AdjFacesIndex;   //记录该边所属三角形的索引号
	};

	struct tFace
	{
		int vertIndex[3];			// indicies for the verts that make up this triangle
		float ybound[2];            //记录每个三角形在y方向上的上下界
		int coordIndex[3];			// indicies for the tex coords to texture this face
		int edgeIndex[3];           //该面的三个边的索引号
		CVector3 Normalize;       //三角形的单位法向量
		//CVector3 Anothervetex[4]; //分别记录该三角形的三边中点和重心坐标

		M3DVector4d A[4];         //用以表示该三角形法向量和d构成的一个矩阵
		M3DVector4d Q[4];         //该三角面片的误差矩阵
		//M3DVector4d InvertofQ[4]; //Q的右上3*3矩阵的逆矩阵
		float d;                  //d表示该三角面所在平面的平面方程常数
		float area;               //三角形的面积
		int implicit;             //三角形的消隐标记
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
	//材料引用类
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
		int  numOfEdges;			// 该模型的边数
		int  numOfFaces;			// The number of faces in the model
		int  numTexVertex;			// The number of texture coordinates
		int  numOfMaterials;		// the 材料数量
		int  materialID;			// The texture ID to use, which is the index into our texture array
		bool bHasTexture;			// This is TRUE if there is a texture map for this object
		char strName[255];			// The name of the object
		CVector3  *pVerts;			// The object's vertices
		CVector3  *pNormals;		// The object's normals
		CVector2  *pTexVerts;		// The texture's UV coordinates
		tEdge *pEdges;
		tFace *pFaces;				// The faces information of the object
		tMatREF	      *pMaterialREFS;// 材料IDS
		
	};

public://邻接表的相关数据结构声明
	struct vertInfo{                   //邻接表的点存储结构信息
		CVector3  Coordinates;         //该点的坐标
		int AdjVertCounts;             //邻接点的个数
		int AdjEdgeCounts;             //邻接边的个数
		int AdjFaceCounts;             //邻接面的个数
		vector<int> AdjPointsIndex;    //该点的邻接点的索引号
		vector<int> AdjEdgesIndex;     //该点的邻接边的索引号
		vector<int> AdjFacesIndex;     //该点的邻接面的索引号
    };

	struct edgeInfo{                   //邻接表的边存储结构信息
		tEdge *polesinfo;              //该边的端点 以及邻接面信息
		int AdjEdgeCounts;             //邻接边的个数
		vector<int> AdjEdgesIndex;     //该边的邻接边的索引号
	};

	struct faceInfo{                   //邻接表的面存储结构信息
		tFace *triangleinfo;           //该面的三角信息
        int AdjFaceCounts;             //邻接面的个数
		vector<int> AdjFacesIndex;     //该面的邻接面的索引号


	};

	struct  MeshInfo{               //三维网格的整体信息
		int vertInfcounts;          //邻接表的点存储结构信息的个数
		int edgeInfcounts;          //邻接表的边存储结构信息的个数
		int faceInfcounts;          //邻接表的面存储结构信息的个数
		vertInfo *vertInfos;        //邻接表的所有点信息
		edgeInfo *edgeInfos;        //邻接表的所有边信息
		faceInfo *faceInfos;        //邻接表的所有面信息
		faceInfo *brifaceInfos;     //邻接表的所有简化三角面片信息,用于简化碰撞检测
		vertInfo *brivertInfos;     //邻接表的所有简化点信息
		edgeInfo *briedgeInfos;     //邻接表的所有简化边信息（简化三角面片上的）
		int briedgeNum;            //简化边的个数

	};
	// This holds our model information.  This should also turn into a robust class.
	// We use STL's (Standard Template Library) vector class to ease our link list burdens. :)
	struct t3DModel 
	{
		int numOfObjects;					// The number of objects in the model
		int numOfMaterials;					// The number of materials for the model
		int numOfMeshInfos;
		//改名了	
		vector<tMaterialInfo> vctMaterials;	// The list of material information (Textures and colors)
		vector<t3DObject> vctObjects;		// The object list for our model
		vector<MeshInfo> vctMeshInfos;      // 用于存储所有对象的邻接表
	};
public:
	//构造
	CLoad3DS();								
	//析构
	~CLoad3DS();
	// 载入3ds文件
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
	void ComputeNormals(t3DModel *pModel);//该函数同时实现了为三角面片添加其他信息的功能

	//设置对象中的边信息
	void SetObjEdge(t3DModel *pModel);

	//创建简化边信息
	void CreateBriEdge(t3DModel *pModel,int i);

	//构建模型的所有对象的邻接表
	void CreateAdjList(t3DModel *pModel);

	//构建简化三角形信息
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
	
	int min_eg(M3DVector3d eg[],int length){//获得eg中的最小值的索引号
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
