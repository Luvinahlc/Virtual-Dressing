// Load3DS.cpp: implementation of the CLoad3DS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Load3DS.h"
#include "Mass-SpringModel.h"
#include <fstream>
// 缓冲区
static int gBuffer[50000] = {0};					
// 用来跳过不需要的数据（此程序不需要）
static int temEdges=0;         //记录临时边数的静态变量

//////////////////////////////////////////////////////////////////////
/////	This constructor initializes the tChunk data
//////////////////////////////////////////////////////////////////////

CLoad3DS::CLoad3DS()
{
	m_FilePointer = NULL;
}

CLoad3DS::~CLoad3DS()
{
	m_FilePointer = NULL;
}

///////////////////////////////// IMPORT 3DS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	打开3ds文件
/////
///////////////////////////////// IMPORT 3DS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CLoad3DS::Import3DS(CLoad3DS::t3DModel *pModel, char *strFileName,int key)
{
	char strMessage[255] = {0};
	tChunk currentChunk = {0};
	int i=0;

	// 打开文件
	m_FilePointer = fopen(strFileName, "rb");

	// 确保文件打开
	if(!m_FilePointer) 
	{
		sprintf(strMessage, "找不到文件: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// 文件打开后，读取第一个文件块

	ReadChunk(&currentChunk);

	// 文件标志判断
	if (currentChunk.ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// 使用 ProcessNextChunk() 来递归读取内容

	// 加载对象
	ProcessNextChunk(pModel, &currentChunk,key);
	
	//设置对象的边信息
	SetObjEdge(pModel);

	// 计算法线
	ComputeNormals(pModel);

    //创建邻接表信息
	CreateAdjList(pModel);

	// 清除
	CleanUp();

	return true;
}

///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	清空（关闭文件）
/////
///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::CleanUp()
{
	if (m_FilePointer) {
		fclose(m_FilePointer);					// Close the current file pointer
		m_FilePointer = NULL;
	}
}


///////////////////////////////// PROCESS NEXT CHUNK\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	函数读取3ds的各分块，并且进行深层的递归
/////
///////////////////////////////// PROCESS NEXT CHUNK\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextChunk(CLoad3DS::t3DModel *pModel, tChunk *pPreviousChunk,int key)
{
	t3DObject newObject = {0};					// This is used to add to our object list
	tMaterialInfo newTexture = {0};				// This is used to add to our material list

	tChunk currentChunk = {0};					// The current chunk to load
	tChunk tempChunk = {0};						// A temp chunk for holding data		

	// Below we check our chunk ID each time we read a new chunk.  Then, if
	// we want to extract the information from that chunk, we do so.
	// If we don't want a chunk, we just read past it.  

	// Continue to read the sub chunks until we have reached the length.
	// After we read ANYTHING we add the bytes read to the chunk and then check
	// check against the length.
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Read next Chunk
		ReadChunk(&currentChunk);

		// Check the chunk ID
		switch (currentChunk.ID)
		{
		case VERSION:							// This holds the version of the file
			
			// If the file was made in 3D Studio Max, this chunk has an int that 
			// holds the file version.  Since there might be new additions to the 3DS file
			// format in 4.0, we give a warning to that problem.
			// However, if the file wasn't made by 3D Studio Max, we don't 100% what the
			// version length will be so we'll simply ignore the value

			// Read the file version and add the bytes read to our bytesRead variable
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);

			// If the file version is over 3, give a warning that there could be a problem
			if ((currentChunk.length - currentChunk.bytesRead == 4) && (gBuffer[0] > 0x03)) {
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			}
			break;

		case OBJECTINFO:						// This holds the version of the mesh
			{	
			// This chunk holds the version of the mesh.  It is also the head of the MATERIAL
			// and OBJECT chunks.  From here on we start reading in the material and object info.

			// Read the next chunk
			ReadChunk(&tempChunk);

			// Get the version of the mesh
			tempChunk.bytesRead += fread(gBuffer, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

			// Increase the bytesRead by the bytes read from the last chunk
			currentChunk.bytesRead += tempChunk.bytesRead;

			// Go to the next chunk, which is the object has a texture, it should be MATERIAL, then OBJECT.
			ProcessNextChunk(pModel, &currentChunk,key);
			break;
		}
		case MATERIAL:							// This holds the material information

			// This chunk is the header for the material info chunks

			// Increase the number of materials
			pModel->numOfMaterials++;

			// Add a empty texture structure to our texture list.
			// If you are unfamiliar with STL's "vector" class, all push_back()
			// does is add a new node onto the list.  I used the vector class
			// so I didn't need to write my own link list functions.  
			pModel->vctMaterials.push_back(newTexture);

			// Proceed to the material loading function
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case OBJECT:							// This holds the name of the object being read
				
			// This chunk is the header for the object info chunks.  It also
			// holds the name of the object.

			// Increase the object count
			pModel->numOfObjects++;
		
			// Add a new tObject node to our list of objects (like a link list)
			pModel->vctObjects.push_back(newObject);
			
			// Initialize the object and all it's data members
			memset(&(pModel->vctObjects[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

			// Get the name of the object and store it, then add the read bytes to our byte counter.
			currentChunk.bytesRead += GetString(pModel->vctObjects[pModel->numOfObjects - 1].strName);
			
			// Now proceed to read in the rest of the object information
			ProcessNextObjectChunk(pModel, &(pModel->vctObjects[pModel->numOfObjects - 1]), &currentChunk,key);
			break;

		case EDITKEYFRAME:

			// Because I wanted to make this a SIMPLE tutorial as possible, I did not include
			// the key frame information.  This chunk is the header for all the animation info.
			// In a later tutorial this will be the subject and explained thoroughly.

			//ProcessNextKeyFrameChunk(pModel, currentChunk);

			// Read past this chunk and add the bytes read to the byte counter
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		default: 
			
			// If we didn't care about a chunk, then we get here.  We still need
			// to read past the unknown or ignored chunk and add the bytes read to the byte counter.
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}


///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	处理一个对象块
/////
///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextObjectChunk(CLoad3DS::t3DModel *pModel, CLoad3DS::t3DObject *pObject, CLoad3DS::tChunk *pPreviousChunk,int key)
{
	// The current chunk to work with
	tChunk currentChunk = {0};
	vector<tMatREF*> vctMatIDS;
	
	// Continue to read these chunks until we read the end of this sub chunk
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Read the next chunk
		ReadChunk(&currentChunk);

		// Check which chunk we just read
		switch (currentChunk.ID)
		{
		case OBJECT_MESH:					// This lets us know that we are reading a new object
		
			// We found a new object, so let's read in it's info using recursion
			ProcessNextObjectChunk(pModel, pObject, &currentChunk,key);
			break;

		case OBJECT_VERTICES:				// This is the objects vertices
			ReadVertices(pObject, &currentChunk,key);
			break;

		case OBJECT_FACES:					// This is the objects face information
			ReadVertexIndices(pObject, &currentChunk);
			break;

		case OBJECT_MATERIAL:				// This holds the material name that the object has
			
			// This chunk holds the name of the material that the object has assigned to it.
			// This could either be just a color or a texture map.  This chunk also holds
			// the faces that the texture is assigned to (In the case that there is multiple
			// textures assigned to one object, or it just has a texture on a part of the object.
			// Since most of my game objects just have the texture around the whole object, and 
			// they aren't multitextured, I just want the material name.

			// We now will read the name of the material assigned to this object
			ReadObjectMaterial(pModel, pObject, &currentChunk,&vctMatIDS);			
			break;

		case OBJECT_UV:						// This holds the UV texture coordinates for the object

			// This chunk holds all of the UV coordinates for our object.  Let's read them in.
			ReadUVCoordinates(pObject, &currentChunk);
			break;

		case 0x4111: //TRI_VERTEXOPTIONS:
			// Read past the ignored or unknown chunks
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		default:  
			// Read past the ignored or unknown chunks
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += currentChunk.bytesRead;

	}
	if(pPreviousChunk->ID!=OBJECT_MESH) return;
	//必须是OBJECT_MESH
	int size=vctMatIDS.size();
	if(size)
	{
		pObject->numOfMaterials=size;
		pObject->pMaterialREFS=new tMatREF[size];
		for(int i=0;i<size;i++)
		{
			pObject->pMaterialREFS[i]=*(vctMatIDS[i]);
		}
		vctMatIDS.clear();
	}
}

///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	处理材料信息，名字，贴图等
/////
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextMaterialChunk(CLoad3DS::t3DModel *pModel, CLoad3DS::tChunk *pPreviousChunk)
{
	// The current chunk to work with
	tChunk currentChunk = {0};

	// Continue to read these chunks until we read the end of this sub chunk
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Read the next chunk
		ReadChunk(&currentChunk);

		// Check which chunk we just read in
		switch (currentChunk.ID)
		{
		case MATNAME:							// This chunk holds the name of the material
			
			// Here we read in the material name
			currentChunk.bytesRead += fread(pModel->vctMaterials[pModel->numOfMaterials - 1].strName, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		case MATDIFFUSE:						// This holds the R G B color of our object
			ReadColorChunk(&(pModel->vctMaterials[pModel->numOfMaterials - 1]), &currentChunk);
			break;
		
		case MATMAP:							// This is the header for the texture info
			
			// Proceed to read in the material information
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case MATMAPFILE:						// This stores the file name of the material

			// Here we read in the material's file name
			currentChunk.bytesRead += fread(pModel->vctMaterials[pModel->numOfMaterials - 1].strFile, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		
		default:  

			// Read past the ignored or unknown chunks
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}

///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	读取一个块的ID 和长度（字节）
/////
///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadChunk(CLoad3DS::tChunk *pChunk)
{
	// This reads the chunk ID which is 2 bytes.
	// The chunk ID is like OBJECT or MATERIAL.  It tells what data is
	// able to be read in within the chunks section.  
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

	// Then, we read the length of the chunk which is 4 bytes.
	// This is how we know how much to read in, or read past.
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	读取字符串
/////
///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;

	// Read 1 byte of data which is the first letter of the string
	fread(pBuffer, 1, 1, m_FilePointer);

	// Loop until we get NULL
	while (*(pBuffer + index++) != 0) {

		// Read in a character at a time until we hit NULL.
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	// Return the string length, which is how many bytes we read in (including the NULL)
	return strlen(pBuffer) + 1;
}


///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	读取颜色数据
/////
///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadColorChunk(CLoad3DS::tMaterialInfo *pMaterial, CLoad3DS::tChunk *pChunk)
{
	tChunk tempChunk = {0};

	// Read the color chunk info
	ReadChunk(&tempChunk);

	// Read in the R G B color (3 bytes - 0 through 255)
	tempChunk.bytesRead += fread(pMaterial->color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

	// Add the bytes read to our chunk
	pChunk->bytesRead += tempChunk.bytesRead;
}


///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	读取端点索引（构成三角面的点序号）
/////
///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadVertexIndices(CLoad3DS::t3DObject *pObject, CLoad3DS::tChunk *pPreviousChunk)
{
	unsigned short index = 0;					// This is used to read in the current face index
	bool b=false;
	CVector3 zero;
	zero.Set(0,0,0);
	// In order to read in the vertex indices for the object, we need to first
	// read in the number of them, then read them in.  Remember,
	// we only want 3 of the 4 values read in for each face.  The fourth is
	// a visibility flag for 3D Studio Max that doesn't mean anything to us.

	// Read in the number of faces that are in this object (int)
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	// Alloc enough memory for the faces and initialize the structure
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);
	// Go through all of the faces in this object
	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		// Next, we read in the A then B then C index for the face, but ignore the 4th value.
		// The fourth value is a visibility flag for 3D Studio Max, we don't care about this.
		pObject->pFaces[i].ybound[0]=1000;
		pObject->pFaces[i].ybound[1]=-1000;
		int Num;
		for(int j = 0; j < 4; j++)
		{
			// Read the first vertice index for the current face 
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if(j < 3)
			{
				int k;
				// Store the index in our face structure.
				for(k=0;k<pObject->numOfVerts;k++)
					if(pObject->pVerts[k]-pObject->pVerts[index]==zero){
						b=true;
						break;
					}
				if(b==true)
					pObject->pFaces[i].vertIndex[j] = k;
				else
					pObject->pFaces[i].vertIndex[j] = index;
				Num=pObject->pFaces[i].vertIndex[j];
				if(pObject->pVerts[Num].y>pObject->pFaces[i].ybound[1])pObject->pFaces[i].ybound[1]=pObject->pVerts[Num].y;
				if(pObject->pVerts[Num].y<pObject->pFaces[i].ybound[0])pObject->pFaces[i].ybound[0]=pObject->pVerts[Num].y;
			}
		}
	}
}


///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	读取纹理坐标
/////
///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// In order to read in the UV indices for the object, we need to first
	// read in the amount there are, then read them in.

	// Read in the number of UV coordinates there are (int)
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

	// Allocate memory to hold the UV coordinates
	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];

	// Read in the texture coodinates (an array 2 float)
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	读取端点
/////
///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadVertices(CLoad3DS::t3DObject *pObject, CLoad3DS::tChunk *pPreviousChunk,int key)
{
	// Like most chunks, before we read in the actual vertices, we need
	// to find out how many there are to read in.  Once we have that number
	// we then fread() them into our vertice array.

	// Read in the number of vertices (int)
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	// Allocate the memory for the verts and initialize the structure
	pObject->pVerts = new CVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);

	// Read in the array of vertices (an array of 3 floats)
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

	// Now we should have all of the vertices read in.  Because 3D Studio Max
	// Models with the Z-Axis pointing up (strange and ugly I know!), we need
	// to flip the y values with the z values in our vertices.  That way it
	// will be normal, with Y pointing up.  If you prefer to work with Z pointing
	// up, then just delete this next loop.  Also, because we swap the Y and Z
	// we need to negate the Z to make it come out correctly.

	// Go through all of the vertices that we just read and swap the Y and Z values
	if(key==0){
		for(int i = 0; i < pObject->numOfVerts; i++)
		{
			// Store off the Y value
			float fTempY = pObject->pVerts[i].y*0.75;//20

			// Set the Y value to the Z value
			pObject->pVerts[i].y = pObject->pVerts[i].z*0.72;//18  19.4 -14.9

			// Set the Z value to the Y value, 
			// but negative Z because 3D Studio max does the opposite.
			pObject->pVerts[i].z = -fTempY;//+0.1

			pObject->pVerts[i].x = pObject->pVerts[i].x*0.71;
		
				//if(pObject->pVerts[i].x>1.37||pObject->pVerts[i].x<-1.34)
		}
	}
	if(key==1){
		for(int i = 0; i < pObject->numOfVerts; i++)
		{
			// Store off the Y value
			float fTempY = pObject->pVerts[i].y*20;//20

			// Set the Y value to the Z value
			pObject->pVerts[i].y = pObject->pVerts[i].z*16;//如果是裙子为12

			// Set the Z value to the Y value, 
			// but negative Z because 3D Studio max does the opposite.
			pObject->pVerts[i].z = -fTempY;//裙子无需*

			pObject->pVerts[i].x=pObject->pVerts[i].x*14;
		
				//if(pObject->pVerts[i].x>1.37||pObject->pVerts[i].x<-1.34)
		}
	}
}


///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	读取材料名称，设置材料的ID （改动比较大）
/////
///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadObjectMaterial(CLoad3DS::t3DModel *pModel,CLoad3DS::t3DObject *pObject, CLoad3DS::tChunk *pPreviousChunk,vector<tMatREF*> *pvmatids)
{
	char strMaterial[255] = {0};			// This is used to hold the objects material name
	bool bmaterror=true;

	tMatREF *pMatref;
	pMatref=new tMatREF;

	// *What is a material?*  - A material is either the color or the texture map of the object.
	// It can also hold other information like the brightness, shine, etc... Stuff we don't
	// really care about.  We just want the color, or the texture map file name really.

	// Here we read the material name that is assigned to the current object.
	// strMaterial should now have a string of the material name, like "Material #2" etc..
	pPreviousChunk->bytesRead += GetString(strMaterial);

	// Now that we have a material name, we need to go through all of the materials
	// and check the name against each material.  When we find a material in our material
	// list that matches this name we just read in, then we assign the materialID
	// of the object to that material index.  You will notice that we passed in the
	// model to this function.  This is because we need the number of textures.
	// Yes though, we could have just passed in the model and not the object too.

	// Go through all of the textures
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		// If the material we just read in matches the current texture name
		if(strcmp(strMaterial, pModel->vctMaterials[i].strName) == 0)
		{
			// Set the material ID to the current index 'i' and stop checking
			pObject->materialID = i;
			pMatref->nMaterialID=i;
			// Now that we found the material, check if it's a texture map.
			// If the strFile has a string length of 1 and over it's a texture
			if(strlen(pModel->vctMaterials[i].strFile) > 0) {
				// Set the object's flag to say it has a texture map to bind.
				pObject->bHasTexture = true;
				pMatref->bHasTexture=true;
			}	
			bmaterror=false;
			break;
		}
		else
		{
			// Set the ID to -1 to show there is no material for this object
			pObject->materialID = -1;
			pMatref->nMaterialID=-1;
			bmaterror=true;
		}
	}
//--------------------------------------------------------------
	//下面语句，读入了该材料相关的面索引号
	pPreviousChunk->bytesRead += fread(gBuffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
	if(!bmaterror)
	{
		pMatref->nFaceNum=gBuffer[0]&0x0000FFFF;
		pMatref->pFaceIndexs=new USHORT[pMatref->nFaceNum];
		memcpy(pMatref->pFaceIndexs,2+(BYTE*)gBuffer,pMatref->nFaceNum*sizeof(USHORT));
		//保存材料号
		pvmatids->push_back(pMatref);
	}
//--------------------------------------------------------------
}			

///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	计算各端点法线
/////
///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ComputeNormals(CLoad3DS::t3DModel *pModel)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];
	float cos,sin;
	// If there are no objects, we can skip this part
	if(pModel->numOfObjects <= 0)
		return;

	// What are vertex normals?  And how are they different from other normals?
	// Well, if you find the normal to a triangle, you are finding a "Face Normal".
	// If you give OpenGL a face normal for lighting, it will make your object look
	// really flat and not very round.  If we find the normal for each vertex, it makes
	// the smooth lighting look.  This also covers up blocky looking objects and they appear
	// to have more polygons than they do.    Basically, what you do is first
	// calculate the face normals, then you take the average of all the normals around each
	// vertex.  It's just averaging.  That way you get a better approximation for that vertex.

	// Go through each of the objects to calculate their normals
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		// Get the current object
		t3DObject *pObject = &(pModel->vctObjects[index]);

		// Here we allocate all the memory we need to calculate the normals
		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];

		// Go though all of the faces of this object
		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			// To cut down LARGE code, we extract the 3 points of this face
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			// Now let's calculate the face normals (Get 2 vectors and find the cross product of those 2)

			vVector1 = Vector(vPoly[0], vPoly[2]);		// Get the vector of the polygon (we just need 2 sides for the normal)
			vVector2 = Vector(vPoly[2], vPoly[1]);		// Get a second vector of the polygon

			vNormal  = Cross(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
			pTempNormals[i] = vNormal;					// Save the un-normalized normal for the vertex normals
			vNormal  = Normalize(vNormal);				// Normalize the cross product to give us the polygons normal
			
			pNormals[i] = vNormal;						// Assign the normal to the list of normals
			
			//借此为三角面片添加其他信息，如法向量、平面方程常量等等
			//计算面片法向量
			pObject->pFaces[i].Normalize=vNormal;
			//计算面积
			cos=Dot(vVector1,vVector2)/(Magnitude(vVector1)*Magnitude(vVector2));
			sin=sqrt(1-cos*cos);
			pObject->pFaces[i].area=Magnitude(vVector1)*Magnitude(vVector2)*sin;
			//计算d
			pObject->pFaces[i].d=Dot(vPoly[0],vNormal)*(-1);

			pObject->pFaces[i].implicit=1;
			//计算三角形另外四个点的坐标值
			//pObject->pFaces[i].Anothervetex[0].Set((vPoly[0].x+vPoly[1].x)/2,(vPoly[0].y+vPoly[1].y)/2,(vPoly[0].z+vPoly[1].z)/2);
			//pObject->pFaces[i].Anothervetex[1].Set((vPoly[0].x+vPoly[2].x)/2,(vPoly[0].y+vPoly[2].y)/2,(vPoly[0].z+vPoly[2].z)/2);
			//pObject->pFaces[i].Anothervetex[2].Set((vPoly[1].x+vPoly[2].x)/2,(vPoly[1].y+vPoly[2].y)/2,(vPoly[1].z+vPoly[2].z)/2);
			//pObject->pFaces[i].Anothervetex[3].Set((vPoly[0].x+pObject->pFaces[i].Anothervetex[2].x*2)/3,(vPoly[0].y+pObject->pFaces[i].Anothervetex[2].y*2)/3,(vPoly[0].z+pObject->pFaces[i].Anothervetex[2].z*2)/3);
			//计算矩阵A
			pObject->pFaces[i].A[0][0]=vNormal.x*vNormal.x;
			pObject->pFaces[i].A[0][1]=vNormal.x*vNormal.y;
			pObject->pFaces[i].A[0][2]=vNormal.x*vNormal.z;
			pObject->pFaces[i].A[1][0]=pObject->pFaces[i].A[0][1];
			pObject->pFaces[i].A[1][1]=vNormal.y*vNormal.y;
			pObject->pFaces[i].A[1][2]=vNormal.y*vNormal.z;
			pObject->pFaces[i].A[2][0]=pObject->pFaces[i].A[0][2];
			pObject->pFaces[i].A[2][1]=pObject->pFaces[i].A[1][2];
			pObject->pFaces[i].A[2][2]=vNormal.z*vNormal.z;
			pObject->pFaces[i].A[0][3]=pObject->pFaces[i].A[3][0]=vNormal.x*pObject->pFaces[i].d;
			pObject->pFaces[i].A[1][3]=pObject->pFaces[i].A[3][1]=vNormal.y*pObject->pFaces[i].d;
			pObject->pFaces[i].A[2][3]=pObject->pFaces[i].A[3][2]=vNormal.z*pObject->pFaces[i].d;
			pObject->pFaces[i].A[3][3]=pObject->pFaces[i].d*pObject->pFaces[i].d;
		}

		//////////////// Now Get The Vertex Normals /////////////////

		CVector3 vSum (0.0, 0.0, 0.0);
		CVector3 vZero = vSum;
		int shared=0;

		for (int i = 0; i < pObject->numOfVerts; i++)			// Go through all of the vertices
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	// Go through all of the triangles
			{												// Check if the vertex is shared by another face
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum =vSum+pTempNormals[j];// AddVector(vSum, pTempNormals[j]);// Add the un-normalized normal of the shared face
					shared++;								// Increase the number of shared triangles
				}
			}      
			
			// Get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
			pObject->pNormals[i] = vSum/float(-shared);// .DivideVectorByScaler(vSum, float(-shared));

			// Normalize the normal for the final vertex normal
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);	

			vSum = vZero;									// Reset the sum
			shared = 0;										// Reset the shared
		}
	
		// Free our memory and start over on the next object
		delete [] pTempNormals;
		delete [] pNormals;
	}
}
void CLoad3DS::SetObjEdge(CLoad3DS::t3DModel *pModel){
	CLoad3DS::t3DObject *pObject;
	int newIndex=0;
	//ofstream f("F:\\Myprojects\\质点-弹簧模型\\3ds读取\\test1.txt");
	for(int j=0;j<pModel->numOfObjects;j++){
		pObject=&pModel->vctObjects[j];
		pObject->numOfEdges=0;
		pObject->pEdges = new tEdge [40000];
		memset(pObject->pEdges, 0, sizeof(tEdge) * (40000));
		//第0个三角面片的边信息
		pObject->pEdges[0].flag=1;
		pObject->pEdges[0].vertIndex[0]=pObject->pFaces[0].vertIndex[0];
		pObject->pEdges[0].vertIndex[1]=pObject->pFaces[0].vertIndex[1];
		pObject->pEdges[0].AdjFaceCounts=1;
		pObject->pEdges[0].AdjFacesIndex.push_back(newIndex);
		pObject->pEdges[0].AdjFacesIndex[0]=0;
		pObject->pEdges[1].flag=2;
		pObject->pEdges[1].vertIndex[0]=pObject->pFaces[0].vertIndex[1];
		pObject->pEdges[1].vertIndex[1]=pObject->pFaces[0].vertIndex[2];
		pObject->pEdges[1].AdjFaceCounts=1;
		pObject->pEdges[1].AdjFacesIndex.push_back(newIndex);
		pObject->pEdges[1].AdjFacesIndex[0]=0;
		pObject->pEdges[2].flag=3;
		pObject->pEdges[2].vertIndex[0]=pObject->pFaces[0].vertIndex[2];
		pObject->pEdges[2].vertIndex[1]=pObject->pFaces[0].vertIndex[0];
		pObject->pEdges[2].AdjFaceCounts=1;
		pObject->pEdges[2].AdjFacesIndex.push_back(newIndex);
		pObject->pEdges[2].AdjFacesIndex[0]=0;
		pObject->numOfEdges=3;
		temEdges=3;
		int i,k;
		for(i = 1; i < pObject->numOfFaces; i++)//循环访问三角面片，填充对象的边信息
	   {
		    int m[3]={0,0,0};
			bool mark[3]={false,false,false};
			/*if(temEdges<100)
				k=0;
			else k=temEdges-100;*/
			for(k=0;k<temEdges;k++){//向已存的前100个边信息中查询，看有没有当前三角面片的三个边的存储信息，若有，进行标记，以便不重复存储。
				if((pObject->pEdges[k].vertIndex[0]==pObject->pFaces[i].vertIndex[0])&&(pObject->pEdges[k].vertIndex[1]==pObject->pFaces[i].vertIndex[1])
					||(pObject->pEdges[k].vertIndex[0]==pObject->pFaces[i].vertIndex[1])&&(pObject->pEdges[k].vertIndex[1]==pObject->pFaces[i].vertIndex[0])){
					mark[0]=true;
					m[0]=k;
					pObject->pEdges[k].AdjFaceCounts++;
					pObject->pEdges[k].AdjFacesIndex.push_back(newIndex);
					pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]=i;
		           // memset(&(pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]), i, sizeof(int));
					//f<<" 边序号："<<k<<"  邻面数："<<pObject->pEdges[k].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]<<endl;
				}
				if((pObject->pEdges[k].vertIndex[0]==pObject->pFaces[i].vertIndex[1])&&(pObject->pEdges[k].vertIndex[1]==pObject->pFaces[i].vertIndex[2])
					||(pObject->pEdges[k].vertIndex[0]==pObject->pFaces[i].vertIndex[2])&&(pObject->pEdges[k].vertIndex[1]==pObject->pFaces[i].vertIndex[1])){
					mark[1]=true;
					m[1]=k;
					pObject->pEdges[k].AdjFaceCounts++;//后来我们知道每个边的邻接面最多只有两个
					pObject->pEdges[k].AdjFacesIndex.push_back(newIndex);
					pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]=i;
		           // memset(&(pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]), i, sizeof(int));
					//f<<" 边序号："<<k<<"  邻面数："<<pObject->pEdges[k].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]<<endl;
				}
				if((pObject->pEdges[k].vertIndex[0]==pObject->pFaces[i].vertIndex[2])&&(pObject->pEdges[k].vertIndex[1]==pObject->pFaces[i].vertIndex[0])
					||(pObject->pEdges[k].vertIndex[0]==pObject->pFaces[i].vertIndex[0])&&(pObject->pEdges[k].vertIndex[1]==pObject->pFaces[i].vertIndex[2])){
					mark[2]=true;
					m[2]=k;
					pObject->pEdges[k].AdjFaceCounts++;
					pObject->pEdges[k].AdjFacesIndex.push_back(newIndex);
					pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]=i;
		           // memset(&(pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]), i, sizeof(int));
					//f<<" 边序号："<<k<<"  邻面数："<<pObject->pEdges[k].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]<<endl;
				}
			}
			if(mark[0]==false){//当前三角形的第一条边之前未被存储
			   pObject->pEdges[temEdges].flag=1;
			   pObject->pEdges[temEdges].vertIndex[0]=pObject->pFaces[i].vertIndex[0];
			   pObject->pEdges[temEdges].vertIndex[1]=pObject->pFaces[i].vertIndex[1];
			   pObject->pEdges[temEdges].AdjFaceCounts=1;
			   pObject->pEdges[temEdges].AdjFacesIndex.push_back(newIndex);
			   pObject->pEdges[temEdges].AdjFacesIndex[0]=i;
		      // memset(&(pObject->pEdges[temEdges].AdjFacesIndex[0]), i, sizeof(int));
			   pObject->pEdges[temEdges].edgenumber=temEdges;
			   pObject->pFaces[i].edgeIndex[0]=temEdges; 
			  // f<<" 边序号："<<temEdges<<"  邻面数："<<pObject->pEdges[temEdges].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[temEdges].AdjFacesIndex[0]<<endl;
			   temEdges++;
			}else  pObject->pFaces[i].edgeIndex[0]=m[0];//当前三角形的第一条边之前已被存储
			
			if(mark[1]==false){//当前三角形的第二条边之前未被存储
			   pObject->pEdges[temEdges].flag=2;
			   pObject->pEdges[temEdges].vertIndex[0]=pObject->pFaces[i].vertIndex[1];
			   pObject->pEdges[temEdges].vertIndex[1]=pObject->pFaces[i].vertIndex[2];
			   pObject->pEdges[temEdges].AdjFaceCounts=1;
			   pObject->pEdges[temEdges].AdjFacesIndex.push_back(newIndex);
			   pObject->pEdges[temEdges].AdjFacesIndex[0]=i;
		      // memset(&(pObject->pEdges[temEdges].AdjFacesIndex[0]), i, sizeof(int));
			   pObject->pEdges[temEdges].edgenumber=temEdges;
			   pObject->pFaces[i].edgeIndex[1]=temEdges;
			  // f<<" 边序号："<<temEdges<<"  邻面数："<<pObject->pEdges[temEdges].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[temEdges].AdjFacesIndex[0]<<endl;
			   temEdges++;
			}else pObject->pFaces[i].edgeIndex[1]=m[1];//当前三角形的第二条边之前已被存储

			if(mark[2]==false){//当前三角形的第三条边之前未被存储
			   pObject->pEdges[temEdges].flag=3;
			   pObject->pEdges[temEdges].vertIndex[0]=pObject->pFaces[i].vertIndex[2];
			   pObject->pEdges[temEdges].vertIndex[1]=pObject->pFaces[i].vertIndex[0];
			   pObject->pEdges[temEdges].AdjFaceCounts=1;
			   pObject->pEdges[temEdges].AdjFacesIndex.push_back(newIndex);
			   pObject->pEdges[temEdges].AdjFacesIndex[0]=i;
		      // memset(&(pObject->pEdges[temEdges].AdjFacesIndex[0]), i, sizeof(int));
			   pObject->pEdges[temEdges].edgenumber=temEdges;
			   pObject->pFaces[i].edgeIndex[2]=temEdges;
			  // f<<" 边序号："<<temEdges<<"  邻面数："<<pObject->pEdges[temEdges].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[temEdges].AdjFacesIndex[0]<<endl;
			   temEdges++;
			}else pObject->pFaces[i].edgeIndex[2]=m[2];//当前三角形的第三条边之前已被存储
		}
		
		pObject->numOfEdges=temEdges;
	}
	//f.close();
}
void CLoad3DS::CreateAdjList(CLoad3DS::t3DModel *pModel){
	    /*CLoad3DS::vertInfo newvertInfo = {0};//用于添加到vertInfo链表中的临时变量
		CLoad3DS::edgeInfo newedgeInfo = {0};//用于添加到edgeInfo链表中的临时变量
		CLoad3DS::faceInfo newfaceInfo = {0};//用于添加到faceInfo链表中的临时变量*/
		CLoad3DS::MeshInfo newMeshInfo = {0};//用于添加到MeshInfo链表中的临时变量
		CLoad3DS::t3DObject *pObject;
		CLoad3DS::MeshInfo *pMeshInfo;
		/*ofstream f("F:\\Myprojects\\质点-弹簧模型\\3ds读取\\pObjvert13+3218.txt");
		f<<"存前点13的坐标：<"<<pModel->vctObjects[0].pVerts[13].x<<","<<pModel->vctObjects[0].pVerts[13].y<<","<<pModel->vctObjects[0].pVerts[13].z<<">"<<endl;
		f<<"存前点3218的坐标：<"<<pModel->vctObjects[0].pVerts[3218].x<<","<<pModel->vctObjects[0].pVerts[3218].y<<","<<pModel->vctObjects[0].pVerts[3218].z<<">"<<endl;
		*/
		//ofstream f1("F:\\Myprojects\\质点-弹簧模型\\3ds读取\\testvert2.txt");
		int newIndex=0;
		pModel->numOfMeshInfos=pModel->numOfObjects;
		for(int i=0;i<pModel->numOfMeshInfos;i++){
			pModel->vctMeshInfos.push_back(newMeshInfo);
			memset(&(pModel->vctMeshInfos[i]), 0, sizeof(MeshInfo));
			pObject=&pModel->vctObjects[i];
			pMeshInfo=&pModel->vctMeshInfos[i];
			pMeshInfo->vertInfcounts=pObject->numOfVerts;
			pMeshInfo->edgeInfcounts=pObject->numOfEdges;
			pMeshInfo->faceInfcounts=pObject->numOfFaces;

			//为pMeshInfo中三个邻接表信息分配空间
			pMeshInfo->vertInfos = new vertInfo [pMeshInfo->vertInfcounts];
		    memset(pMeshInfo->vertInfos, 0, sizeof(vertInfo) * (pMeshInfo->vertInfcounts));
			pMeshInfo->edgeInfos = new edgeInfo [pMeshInfo->edgeInfcounts];
		    memset(pMeshInfo->edgeInfos, 0, sizeof(edgeInfo) * (pMeshInfo->edgeInfcounts));
			pMeshInfo->faceInfos = new faceInfo [pMeshInfo->faceInfcounts];
		    memset(pMeshInfo->faceInfos, 0, sizeof(faceInfo) * (pMeshInfo->faceInfcounts));

			//以下分别对pObject中的pEdges数组、pFaces数组进行循环访问，以填充pMeshInfo中的三个邻接表信息
			int j,p,q,r;

			for(j=0;j<pObject->numOfEdges;j++){//循环访问pObject中的pEdges数组
				tEdge *edge=&(pObject->pEdges[j]);
				//为pMeshInfo中的vertInfos数组添加相关信息
				p=edge->vertIndex[0];//记录边的两个端点的索引
				q=edge->vertIndex[1];
				//为vertInfo[p]添加坐标信息，【此处可添加条件信息以提高速度】
				pMeshInfo->vertInfos[p].Coordinates.x=pObject->pVerts[p].x;
				pMeshInfo->vertInfos[p].Coordinates.y=pObject->pVerts[p].y;
				pMeshInfo->vertInfos[p].Coordinates.z=pObject->pVerts[p].z;
				//为vertInfo[p]添加点邻接表信息
				pMeshInfo->vertInfos[p].AdjVertCounts++;
				pMeshInfo->vertInfos[p].AdjPointsIndex.push_back(newIndex);
				pMeshInfo->vertInfos[p].AdjPointsIndex[pMeshInfo->vertInfos[p].AdjVertCounts-1]=q;
				//为vertInfo[p]添加边邻接表信息
				pMeshInfo->vertInfos[p].AdjEdgeCounts++;
				pMeshInfo->vertInfos[p].AdjEdgesIndex.push_back(newIndex);
				pMeshInfo->vertInfos[p].AdjEdgesIndex[pMeshInfo->vertInfos[p].AdjEdgeCounts-1]=j;
				//为vertInfo[q]添加坐标信息
				pMeshInfo->vertInfos[q].Coordinates.x=pObject->pVerts[q].x;
				pMeshInfo->vertInfos[q].Coordinates.y=pObject->pVerts[q].y;
				pMeshInfo->vertInfos[q].Coordinates.z=pObject->pVerts[q].z;
				//为vertInfo[q]添加点邻接表信息
			    pMeshInfo->vertInfos[q].AdjVertCounts++;
				pMeshInfo->vertInfos[q].AdjPointsIndex.push_back(newIndex);
				pMeshInfo->vertInfos[q].AdjPointsIndex[pMeshInfo->vertInfos[q].AdjVertCounts-1]=p;
				//为vertInfo[q]添加边邻接表信息
				pMeshInfo->vertInfos[q].AdjEdgeCounts++;
				pMeshInfo->vertInfos[q].AdjEdgesIndex.push_back(newIndex);
				pMeshInfo->vertInfos[q].AdjEdgesIndex[pMeshInfo->vertInfos[q].AdjEdgeCounts-1]=j;

				//为pMeshInfo中的edgeInfos数组添加相关信息
				pMeshInfo->edgeInfos[j].polesinfo=edge;

				//为pMeshInfo中的faceInfos数组添加相关信息
				for(p=0;p<edge->AdjFaceCounts;p++){
					q=edge->AdjFacesIndex[p];
					int k=0;
					while(k<edge->AdjFaceCounts){
						if(k!=p){
							pMeshInfo->faceInfos[q].AdjFaceCounts++;
							pMeshInfo->faceInfos[q].AdjFacesIndex.push_back(newIndex);
							pMeshInfo->faceInfos[q].AdjFacesIndex[pMeshInfo->faceInfos[q].AdjFaceCounts-1]=edge->AdjFacesIndex[k];	
						}
						k++;
					}
				}
			}

			for(j=0;j<pObject->numOfFaces;j++){//循环访问pObject中的pFaces数组
				tFace *face=&(pObject->pFaces[j]);
				//为pMeshInfo中的vertInfos数组添加相关信息
				p=face->vertIndex[0];
				q=face->vertIndex[1];
				r=face->vertIndex[2];
				//为vertInfo[p]添加面邻接表信息
				pMeshInfo->vertInfos[p].AdjFaceCounts++;
				pMeshInfo->vertInfos[p].AdjFacesIndex.push_back(newIndex);
				pMeshInfo->vertInfos[p].AdjFacesIndex[pMeshInfo->vertInfos[p].AdjFaceCounts-1]=j;
				//为vertInfo[q]添加面邻接表信息
				pMeshInfo->vertInfos[q].AdjFaceCounts++;
				pMeshInfo->vertInfos[q].AdjFacesIndex.push_back(newIndex);
				pMeshInfo->vertInfos[q].AdjFacesIndex[pMeshInfo->vertInfos[q].AdjFaceCounts-1]=j;
				//为vertInfo[r]添加面邻接表信息
				pMeshInfo->vertInfos[r].AdjFaceCounts++;
				pMeshInfo->vertInfos[r].AdjFacesIndex.push_back(newIndex);
				pMeshInfo->vertInfos[r].AdjFacesIndex[pMeshInfo->vertInfos[r].AdjFaceCounts-1]=j;

				//为pMeshInfo中的faceInfos数组添加相关信息
				pMeshInfo->faceInfos[j].triangleinfo=new tFace;
				memset(pMeshInfo->faceInfos[j].triangleinfo, 0, sizeof(tFace));
				pMeshInfo->faceInfos[j].triangleinfo->area=face->area;
				pMeshInfo->faceInfos[j].triangleinfo->d=face->d;
				pMeshInfo->faceInfos[j].triangleinfo->implicit=1;
				pMeshInfo->faceInfos[j].triangleinfo->Normalize=face->Normalize;
				pMeshInfo->faceInfos[j].triangleinfo->ybound[0]=face->ybound[0];
				pMeshInfo->faceInfos[j].triangleinfo->ybound[1]=face->ybound[1];
				for(int k=0;k<4;k++){
					for(int t=0;t<4;t++)
						pMeshInfo->faceInfos[j].triangleinfo->A[k][t]=face->A[k][t];
					//pMeshInfo->faceInfos[j].triangleinfo->Anothervetex[k]=face->Anothervetex[k];
					if(k<3){
						pMeshInfo->faceInfos[j].triangleinfo->coordIndex[k]=face->coordIndex[k];
						pMeshInfo->faceInfos[j].triangleinfo->edgeIndex[k]=face->edgeIndex[k];
						pMeshInfo->faceInfos[j].triangleinfo->vertIndex[k]=face->vertIndex[k];
					}
				}

			}

			for(j=0;j<pMeshInfo->edgeInfcounts;j++){//循环访问pMeshInfo中的edgeInfos数组，为其中的每条边添加邻接边信息
				p=pMeshInfo->edgeInfos[j].polesinfo->vertIndex[0];
				q=pMeshInfo->edgeInfos[j].polesinfo->vertIndex[1];
				int k;
				for(k=0;k<pMeshInfo->vertInfos[p].AdjEdgeCounts;k++){
					if(pMeshInfo->vertInfos[p].AdjEdgesIndex[k]!=j){
						pMeshInfo->edgeInfos[j].AdjEdgeCounts++;
						pMeshInfo->edgeInfos[j].AdjEdgesIndex.push_back(newIndex);
						pMeshInfo->edgeInfos[j].AdjEdgesIndex[pMeshInfo->edgeInfos[j].AdjEdgeCounts-1]=pMeshInfo->vertInfos[p].AdjEdgesIndex[k];
					}
				}
				for(k=0;k<pMeshInfo->vertInfos[q].AdjEdgeCounts;k++){
					if(pMeshInfo->vertInfos[q].AdjEdgesIndex[k]!=j){
						pMeshInfo->edgeInfos[j].AdjEdgeCounts++;
						pMeshInfo->edgeInfos[j].AdjEdgesIndex.push_back(newIndex);
						pMeshInfo->edgeInfos[j].AdjEdgesIndex[pMeshInfo->edgeInfos[j].AdjEdgeCounts-1]=pMeshInfo->vertInfos[q].AdjEdgesIndex[k];
					}
				}
				
				
				/*if(pMeshInfo->edgeInfos[j].polesinfo->flag==3){
					f<<"边号："<<j<<"    该边邻面个数："<<pMeshInfo->edgeInfos[j].polesinfo->AdjFaceCounts<<"  邻面索引号： ";
					for(int l=0;l<pMeshInfo->edgeInfos[j].polesinfo->AdjFaceCounts;l++){
						f<<pMeshInfo->edgeInfos[j].polesinfo->AdjFacesIndex[l]<<"  ";
					}
					f<<endl;
				}*/
				
			}
		//	f.close();

			/*for(j=0;j<pMeshInfo->vertInfcounts;j++){
				f1<<"点号："<<j<<"    该点邻边个数："<<pMeshInfo->vertInfos[j].AdjEdgeCounts<<"  邻边索引号： ";
				  for(int l=0;l<pMeshInfo->vertInfos[j].AdjEdgeCounts;l++){
					  if(pMeshInfo->edgeInfos[pMeshInfo->vertInfos[j].AdjEdgesIndex[l]].polesinfo->flag==1)
					      /f1<<pMeshInfo->vertInfos[j].AdjEdgesIndex[l]<<"  ";
				}
				  f1<<endl;
			}
			f1.close();*/
			//由于有多余的点序号（点位置相同，而序号不同导致的多余），在这里重置实际点个数
			CVector3 zero;
			zero.Set(0,0,0);
			for(j=0;j<pMeshInfo->vertInfcounts;j++)
				if(pMeshInfo->vertInfos[j].Coordinates==zero)
					break;
			pMeshInfo->vertInfcounts=j;
			CreateBrifTri(pModel,i);
		}	
}

void CLoad3DS::VetexQ(t3DModel *pModel,int i,int index,M3DVector4d Q[]){
	int j,k,t,triindex;
	for(k=0;k<4;k++){
		for(t=0;t<4;t++){
			for(j=0;j<pModel->vctMeshInfos[i].brivertInfos[index].AdjFaceCounts;j++){
				triindex=pModel->vctMeshInfos[i].brivertInfos[index].AdjFacesIndex[j];
				if(triindex!=-1)
					Q[k][t]+=(pModel->vctMeshInfos[i].brifaceInfos[triindex].triangleinfo->area)*(pModel->vctMeshInfos[i].brifaceInfos[triindex].triangleinfo->A[k][t]);
			}
		}
	}
}
void CLoad3DS::briTriNormalize_d(t3DModel *pModel,int i,int triindex){
	CLoad3DS::tFace *pface;
	pface=(pModel->vctMeshInfos[i].brifaceInfos[triindex].triangleinfo);

	CVector3 vPoly[3],vVector1,vVector2,vNormal;
	float cos,sin;
	vPoly[0] = pModel->vctMeshInfos[i].brivertInfos[pface->vertIndex[0]].Coordinates;
	vPoly[1] = pModel->vctMeshInfos[i].brivertInfos[pface->vertIndex[1]].Coordinates;
	vPoly[2] = pModel->vctMeshInfos[i].brivertInfos[pface->vertIndex[2]].Coordinates;

	vVector1 = Vector(vPoly[0], vPoly[2]);		
	vVector2 = Vector(vPoly[2], vPoly[1]);		

	vNormal  = Cross(vVector1, vVector2);				
	vNormal  = Normalize(vNormal);				
		
	//计算面片法向量
	pface->Normalize=vNormal;
	//计算面积
	cos=Dot(vVector1,vVector2)/(Magnitude(vVector1)*Magnitude(vVector2));
	sin=sqrt(1-cos*cos);
	pface->area=Magnitude(vVector1)*Magnitude(vVector2)*sin;
	//计算d
	pface->d=Dot(vPoly[0],vNormal)*(-1);

	//计算矩阵A
	pface->A[0][0]=vNormal.x*vNormal.x;
	pface->A[0][1]=vNormal.x*vNormal.y;
	pface->A[0][2]=vNormal.x*vNormal.z;
	pface->A[1][0]=pface->A[0][1];
	pface->A[1][1]=vNormal.y*vNormal.y;
	pface->A[1][2]=vNormal.y*vNormal.z;
	pface->A[2][0]=pface->A[0][2];
	pface->A[2][1]=pface->A[1][2];
	pface->A[2][2]=vNormal.z*vNormal.z;
	pface->A[0][3]=pface->A[3][0]=vNormal.x*pface->d;
	pface->A[1][3]=pface->A[3][1]=vNormal.y*pface->d;
	pface->A[2][3]=pface->A[3][2]=vNormal.z*pface->d;
	pface->A[3][3]=pface->d*pface->d;
}
void CLoad3DS::CreateBrifTri(t3DModel *pModel,int i){
	CLoad3DS::MeshInfo *pMeshInfo;
	pMeshInfo=&pModel->vctMeshInfos[i];
	pMeshInfo->brifaceInfos=new faceInfo [pMeshInfo->faceInfcounts];
	memset(pMeshInfo->brifaceInfos, 0, sizeof(faceInfo) * (pMeshInfo->faceInfcounts));
	pMeshInfo->brivertInfos=new vertInfo [pMeshInfo->vertInfcounts];
	memset(pMeshInfo->brivertInfos, 0, sizeof(vertInfo) * (pMeshInfo->vertInfcounts));

	int j,k,t,NumTri,newindex=0;
	int mark,p,triindex,tdex, temps[50];
	bool isexist,isfirst=true;
	M3DVector4d *Vnew;
	M3DVector3d *EG;
	NumTri=pMeshInfo->faceInfcounts;
	Vnew=new M3DVector4d[NumTri];
	memset(Vnew, 0, sizeof(M3DVector4d) * (NumTri));
	tFace*triangle;
	for(j=0;j<pMeshInfo->faceInfcounts;j++){   //用原三角面片的信息初始化简约三角面片
		pMeshInfo->brifaceInfos[j].triangleinfo=new tFace;
		memset(pMeshInfo->brifaceInfos[j].triangleinfo, 0, sizeof(tFace));

		pMeshInfo->brifaceInfos[j].triangleinfo->implicit=pMeshInfo->faceInfos[j].triangleinfo->implicit;
		pMeshInfo->brifaceInfos[j].triangleinfo->d=pMeshInfo->faceInfos[j].triangleinfo->d;
		pMeshInfo->brifaceInfos[j].triangleinfo->Normalize=pMeshInfo->faceInfos[j].triangleinfo->Normalize;
		pMeshInfo->brifaceInfos[j].triangleinfo->area=pMeshInfo->faceInfos[j].triangleinfo->area;
		for(k=0;k<4;k++){
			for(t=0;t<4;t++)
				pMeshInfo->brifaceInfos[j].triangleinfo->A[k][t]=pMeshInfo->faceInfos[j].triangleinfo->A[k][t];
					//skeleton[i].briefTri[j].InvertofA[k][t]=skeleton[i].triangle[j].InvertofA[k][t];
			if(k<3){
				pMeshInfo->brifaceInfos[j].triangleinfo->vertIndex[k]=pMeshInfo->faceInfos[j].triangleinfo->vertIndex[k];
				pMeshInfo->brifaceInfos[j].triangleinfo->coordIndex[k]=pMeshInfo->faceInfos[j].triangleinfo->coordIndex[k];
				pMeshInfo->brifaceInfos[j].triangleinfo->edgeIndex[k]=pMeshInfo->brifaceInfos[j].triangleinfo->edgeIndex[k];
			}
		}

	}
	for(j=0;j<pMeshInfo->vertInfcounts;j++){
		pMeshInfo->brivertInfos[j].Coordinates=pMeshInfo->vertInfos[j].Coordinates;
		pMeshInfo->brivertInfos[j].AdjEdgeCounts=pMeshInfo->vertInfos[j].AdjEdgeCounts;
		pMeshInfo->brivertInfos[j].AdjFaceCounts=pMeshInfo->vertInfos[j].AdjFaceCounts;
		pMeshInfo->brivertInfos[j].AdjVertCounts=pMeshInfo->vertInfos[j].AdjVertCounts;
		for(k=0;k<pMeshInfo->vertInfos[j].AdjEdgeCounts;k++){
			pMeshInfo->brivertInfos[j].AdjEdgesIndex.push_back(newindex);
			pMeshInfo->brivertInfos[j].AdjEdgesIndex[k]=pMeshInfo->vertInfos[j].AdjEdgesIndex[k];
		}
		for(k=0;k<pMeshInfo->vertInfos[j].AdjFaceCounts;k++){
			pMeshInfo->brivertInfos[j].AdjFacesIndex.push_back(newindex);
			pMeshInfo->brivertInfos[j].AdjFacesIndex[k]=pMeshInfo->vertInfos[j].AdjFacesIndex[k];
		}
		for(k=0;k<pMeshInfo->vertInfos[j].AdjVertCounts;k++){
			pMeshInfo->brivertInfos[j].AdjPointsIndex.push_back(newindex);
			pMeshInfo->brivertInfos[j].AdjPointsIndex[k]=pMeshInfo->vertInfos[j].AdjPointsIndex[k];
		}
	}

	int imvertcounts=0;//指消去的点的个数
	bool isOK=false;
	while(!isOK){
		for(j=0;j<pMeshInfo->faceInfcounts;j++){
			if(((float)(pMeshInfo->vertInfcounts-imvertcounts)/(float)pMeshInfo->vertInfcounts)<=SYMLIDEGREE){//简化度
				isOK=true;
				break;
			}
			if(pMeshInfo->brifaceInfos[j].triangleinfo->implicit==1){
				triangle=pMeshInfo->brifaceInfos[j].triangleinfo;//add
				
				//开始构造briefTri
				CVector3 Vn;
		
				Vn.Set(pMeshInfo->brivertInfos[triangle->vertIndex[0]].Coordinates.x,pMeshInfo->brivertInfos[triangle->vertIndex[0]].Coordinates.y,pMeshInfo->brivertInfos[triangle->vertIndex[0]].Coordinates.z);
	
				pMeshInfo->brifaceInfos[j].triangleinfo->implicit=0; //mark->j
				tdex=0;
				isexist=false;
				for(k=0;k<3;k++){
					p=(triangle->vertIndex[k]);//pMeshInfo->brifaceInfos[mark].triangleinfo->vertIndex[k]--->triangle->vertIndex[k]
					if(p!=triangle->vertIndex[0]){//(int)EG[mark][2]
						pMeshInfo->brivertInfos[p].AdjVertCounts=-1;//这里用作标记，表示该点是被消去的点
						imvertcounts++;
						for(t=0;t<pMeshInfo->brivertInfos[p].AdjFaceCounts;t++){
							triindex=pMeshInfo->brivertInfos[p].AdjFacesIndex[t];
							if(triindex!=j&&triindex!=-1){//mark--->j
								int l=0;
								while(l<3){
									if(pMeshInfo->brifaceInfos[triindex].triangleinfo->vertIndex[l]!=p)l++;
									else
										break;
								}
								pMeshInfo->brifaceInfos[triindex].triangleinfo->vertIndex[l]=triangle->vertIndex[0];//EG[mark][2]--->triangle->vertIndex[0]
							
								for(j=0;j<tdex;j++)
									if(temps[j]==triindex)
										isexist=true;
								if(!isexist){
									temps[tdex]=triindex;
									tdex++;	
								}else isexist=false;
							}
						}	
					}else{
						for(t=0;t<pMeshInfo->brivertInfos[p].AdjFaceCounts;t++){
							triindex=pMeshInfo->brivertInfos[p].AdjFacesIndex[t];
							if(triindex==j)//mark--->j
								pMeshInfo->brivertInfos[p].AdjFacesIndex[t]=-1;//用于标记，-1表示该邻接面不存在了
						}
						//pMeshInfo->brivertInfos[p].AdjVertCounts=-1; 这里用作标记，表示该点是被保存下来的点
					}
				}

				for(k=0;k<tdex;k++){
					if((pMeshInfo->brifaceInfos[temps[k]].triangleinfo->vertIndex[0]==pMeshInfo->brifaceInfos[temps[k]].triangleinfo->vertIndex[1])||(pMeshInfo->brifaceInfos[temps[k]].triangleinfo->vertIndex[0]==pMeshInfo->brifaceInfos[temps[k]].triangleinfo->vertIndex[2])||(pMeshInfo->brifaceInfos[temps[k]].triangleinfo->vertIndex[1]==pMeshInfo->brifaceInfos[temps[k]].triangleinfo->vertIndex[2]))
					{	
						pMeshInfo->brifaceInfos[temps[k]].triangleinfo->implicit=0;
					//	EG[temps[k]][1]=0;

						for(t=0;t<pMeshInfo->brivertInfos[triangle->vertIndex[0]].AdjFaceCounts;t++){//(int)EG[mark][2]
							triindex=pMeshInfo->brivertInfos[triangle->vertIndex[0]].AdjFacesIndex[t];//(int)EG[mark][2]
							if(triindex==temps[k])
								pMeshInfo->brivertInfos[triangle->vertIndex[0]].AdjFacesIndex[t]=-1;//用于标记 -1表示该邻接面不存在了(int)EG[mark][2]
						}
					}
					else{
							pMeshInfo->brifaceInfos[temps[k]].triangleinfo->implicit=2;
							//EG[temps[k]][1]=-1;
							briTriNormalize_d(pModel,i, temps[k]);
							pMeshInfo->brivertInfos[triangle->vertIndex[0]].AdjFaceCounts++;//(int)EG[mark][2]
							pMeshInfo->brivertInfos[triangle->vertIndex[0]].AdjFacesIndex.push_back(newindex);//(int)EG[mark][2]
							pMeshInfo->brivertInfos[triangle->vertIndex[0]].AdjFacesIndex[pMeshInfo->brivertInfos[triangle->vertIndex[0]].AdjFaceCounts-1]=temps[k];
						}				
				}

			
					//isfirst=false;
			}else{
				if(j==pMeshInfo->faceInfcounts-1){
					for(k=0;k<pMeshInfo->faceInfcounts;k++){
						if(pMeshInfo->brifaceInfos[k].triangleinfo->implicit==2)
							pMeshInfo->brifaceInfos[k].triangleinfo->implicit=1;	
					}
				}
			}
		}
	}
	/*int facecounts=0;
	for(j=0;j<pMeshInfo->faceInfcounts;j++)
		if(pMeshInfo->brifaceInfos[j].triangleinfo->implicit==0)
			facecounts++;*/

	/*ofstream f("F:\\Myprojects\\质点-弹簧模型\\3ds读取\\简约信息.txt");
	f<<"简约面片："<<endl;
	f<<pMeshInfo->faceInfcounts-facecounts<<endl;
	f<<pMeshInfo->faceInfcounts<<endl;
	f<<"简约点："<<endl;
	f<<pMeshInfo->vertInfcounts-imvertcounts<<endl;
	f<<pMeshInfo->vertInfcounts;
	f.close();*/
	CreateBriEdge(pModel,i);

}
void CLoad3DS::CreateBriEdge(t3DModel *pModel,int i){

		int newIndex=0,temcounts=0;
		CLoad3DS::MeshInfo *pMeshInfo;
		pMeshInfo=&pModel->vctMeshInfos[i];
		pMeshInfo->briedgeInfos=new edgeInfo [pMeshInfo->edgeInfcounts];
		memset(pMeshInfo->briedgeInfos, 0, sizeof(edgeInfo) * (pMeshInfo->edgeInfcounts));
		pMeshInfo->briedgeNum=0;

		//第0个三角面片的边信息
		int j;
		for(j=0;j<pMeshInfo->faceInfcounts;j++){
			if(pMeshInfo->brifaceInfos[j].triangleinfo->implicit!=0)
				break;
		}
	
		pMeshInfo->briedgeInfos[0].polesinfo = new tEdge;
		memset(pMeshInfo->briedgeInfos[0].polesinfo, 0, sizeof(tEdge));
		pMeshInfo->briedgeInfos[0].polesinfo->flag=1;
		pMeshInfo->briedgeInfos[0].polesinfo->vertIndex[0]=pMeshInfo->brifaceInfos[j].triangleinfo->vertIndex[0];
		pMeshInfo->briedgeInfos[0].polesinfo->vertIndex[1]=pMeshInfo->brifaceInfos[j].triangleinfo->vertIndex[1];
		pMeshInfo->briedgeInfos[0].polesinfo->AdjFaceCounts=1;
		pMeshInfo->briedgeInfos[0].polesinfo->AdjFacesIndex.push_back(newIndex);
		pMeshInfo->briedgeInfos[0].polesinfo->AdjFacesIndex[0]=0;

		pMeshInfo->briedgeInfos[1].polesinfo = new tEdge;
		memset(pMeshInfo->briedgeInfos[1].polesinfo, 0, sizeof(tEdge));
		pMeshInfo->briedgeInfos[1].polesinfo->flag=2;
		pMeshInfo->briedgeInfos[1].polesinfo->vertIndex[0]=pMeshInfo->brifaceInfos[j].triangleinfo->vertIndex[1];
		pMeshInfo->briedgeInfos[1].polesinfo->vertIndex[1]=pMeshInfo->brifaceInfos[j].triangleinfo->vertIndex[2];
		pMeshInfo->briedgeInfos[1].polesinfo->AdjFaceCounts=1;
		pMeshInfo->briedgeInfos[1].polesinfo->AdjFacesIndex.push_back(newIndex);
		pMeshInfo->briedgeInfos[1].polesinfo->AdjFacesIndex[0]=0;

		pMeshInfo->briedgeInfos[2].polesinfo = new tEdge;
		memset(pMeshInfo->briedgeInfos[2].polesinfo, 0, sizeof(tEdge));
		pMeshInfo->briedgeInfos[2].polesinfo->flag=3;
		pMeshInfo->briedgeInfos[2].polesinfo->vertIndex[0]=pMeshInfo->brifaceInfos[j].triangleinfo->vertIndex[2];
		pMeshInfo->briedgeInfos[2].polesinfo->vertIndex[1]=pMeshInfo->brifaceInfos[j].triangleinfo->vertIndex[0];
		pMeshInfo->briedgeInfos[2].polesinfo->AdjFaceCounts=1;
		pMeshInfo->briedgeInfos[2].polesinfo->AdjFacesIndex.push_back(newIndex);
		pMeshInfo->briedgeInfos[2].polesinfo->AdjFacesIndex[0]=0;
		pMeshInfo->briedgeNum=3;
		temcounts=3;
		int r,k;
		for(r = j+1; r < pMeshInfo->faceInfcounts; r++)//循环访问三角面片，填充对象的边信息
	   {
		    int m[3]={0,0,0};
			bool mark[3]={false,false,false};
			if(pMeshInfo->brifaceInfos[r].triangleinfo->implicit!=0){
				for(k=0;k<temcounts;k++){//向已存的前100个边信息中查询，看有没有当前三角面片的三个边的存储信息，若有，进行标记，以便不重复存储。
					if((pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[0]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[0])&&(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[1]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[1])
						||(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[0]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[1])&&(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[1]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[0])){
						mark[0]=true;
						m[0]=k;
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFaceCounts++;
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFacesIndex.push_back(newIndex);
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFacesIndex[pMeshInfo->briedgeInfos[k].polesinfo->AdjFaceCounts-1]=r;
					   // memset(&(pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]), i, sizeof(int));
						//f<<" 边序号："<<k<<"  邻面数："<<pObject->pEdges[k].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]<<endl;
					}
					if((pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[0]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[1])&&(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[1]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[2])
						||(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[0]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[2])&&(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[1]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[1])){
						mark[1]=true;
						m[1]=k;
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFaceCounts++;//后来我们知道每个边的邻接面最多只有两个
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFacesIndex.push_back(newIndex);
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFacesIndex[pMeshInfo->briedgeInfos[k].polesinfo->AdjFaceCounts-1]=r;
					   // memset(&(pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]), i, sizeof(int));
						//f<<" 边序号："<<k<<"  邻面数："<<pObject->pEdges[k].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]<<endl;
					}
					if((pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[0]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[0])&&(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[1]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[2])
						||(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[0]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[2])&&(pMeshInfo->briedgeInfos[k].polesinfo->vertIndex[1]==pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[0])){
						mark[2]=true;
						m[2]=k;
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFaceCounts++;
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFacesIndex.push_back(newIndex);
						pMeshInfo->briedgeInfos[k].polesinfo->AdjFacesIndex[pMeshInfo->briedgeInfos[k].polesinfo->AdjFaceCounts-1]=r;
					   // memset(&(pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]), i, sizeof(int));
						//f<<" 边序号："<<k<<"  邻面数："<<pObject->pEdges[k].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[k].AdjFacesIndex[pObject->pEdges[k].AdjFaceCounts-1]<<endl;
					}
				}
				if(mark[0]==false){//当前三角形的第一条边之前未被存储
				   pMeshInfo->briedgeInfos[temcounts].polesinfo = new tEdge;
				   memset(pMeshInfo->briedgeInfos[temcounts].polesinfo, 0, sizeof(tEdge));
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->flag=1;
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->vertIndex[0]=pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[0];
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->vertIndex[1]=pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[1];
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFaceCounts=1;
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFacesIndex.push_back(newIndex);
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFacesIndex[0]=r;
				  // memset(&(pObject->pEdges[temEdges].AdjFacesIndex[0]), i, sizeof(int));
				  pMeshInfo->briedgeInfos[temcounts].polesinfo->edgenumber=temcounts;
				  pMeshInfo->brifaceInfos[r].triangleinfo->edgeIndex[0]=temcounts; 
				  // f<<" 边序号："<<temEdges<<"  邻面数："<<pObject->pEdges[temEdges].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[temEdges].AdjFacesIndex[0]<<endl;
				   temcounts++;
				}else  pMeshInfo->brifaceInfos[r].triangleinfo->edgeIndex[0]=m[0];//当前三角形的第一条边之前已被存储
			
				if(mark[1]==false){//当前三角形的第二条边之前未被存储
				   pMeshInfo->briedgeInfos[temcounts].polesinfo = new tEdge;
				   memset(pMeshInfo->briedgeInfos[temcounts].polesinfo, 0, sizeof(tEdge));
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->flag=2;
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->vertIndex[0]=pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[1];
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->vertIndex[1]=pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[2];
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFaceCounts=1;
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFacesIndex.push_back(newIndex);
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFacesIndex[0]=r;
				  // memset(&(pObject->pEdges[temEdges].AdjFacesIndex[0]), i, sizeof(int));
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->edgenumber=temEdges;
				  pMeshInfo->brifaceInfos[r].triangleinfo->edgeIndex[1]=temcounts;
				  // f<<" 边序号："<<temEdges<<"  邻面数："<<pObject->pEdges[temEdges].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[temEdges].AdjFacesIndex[0]<<endl;
				   temcounts++;
				}else pMeshInfo->brifaceInfos[r].triangleinfo->edgeIndex[1]=m[1];//当前三角形的第二条边之前已被存储

				if(mark[2]==false){//当前三角形的第三条边之前未被存储
				   pMeshInfo->briedgeInfos[temcounts].polesinfo = new tEdge;
				   memset(pMeshInfo->briedgeInfos[temcounts].polesinfo, 0, sizeof(tEdge));
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->flag=3;
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->vertIndex[0]=pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[2];
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->vertIndex[1]=pMeshInfo->brifaceInfos[r].triangleinfo->vertIndex[0];
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFaceCounts=1;
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFacesIndex.push_back(newIndex);
				   pMeshInfo->briedgeInfos[temcounts].polesinfo->AdjFacesIndex[0]=r;
				  // memset(&(pObject->pEdges[temEdges].AdjFacesIndex[0]), i, sizeof(int));
				    pMeshInfo->briedgeInfos[temcounts].polesinfo->edgenumber=temcounts;
				   pMeshInfo->brifaceInfos[r].triangleinfo->edgeIndex[2]=temcounts;
				  // f<<" 边序号："<<temEdges<<"  邻面数："<<pObject->pEdges[temEdges].AdjFaceCounts<<"  邻面索引号： "<<pObject->pEdges[temEdges].AdjFacesIndex[0]<<endl;
				   temcounts++;
				}else pMeshInfo->brifaceInfos[r].triangleinfo->edgeIndex[2]=m[2];//当前三角形的第三条边之前已被存储
			}
		}
		
		pMeshInfo->briedgeNum=temcounts;
}