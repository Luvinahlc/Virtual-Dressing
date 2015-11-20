// 3DSModel.h: interface for the C3DSModel class.
//    
//     中国3D技术开发社区
//     http://www.C3DN.net
//     mengmakies@live.cn
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DSMODEL_H__98FC8CCA_A44F_4C45_9C99_CAC11614A519__INCLUDED_)
#define AFX_3DSMODEL_H__98FC8CCA_A44F_4C45_9C99_CAC11614A519__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mass-SpringModel.h"


//3ds 模型类
class C3DSModel
{
public:
	//文件名
	char m_sFileName[100];
	//标志
	bool m_bModelOK;

	C3DSModel(void);
	virtual ~C3DSModel(void);
	// 载入模型文件
	bool LoadModelFromFile(char* sfilename,int key);//key 这里传递着衣服类型，是上衣还是裤子，0表示上衣，1表示裤子，为何如此区别?因为上衣和裤子的缩放比例以及平移相对人体不一致	
	// 绘制模型
	bool Drawcloth(int key);
	bool Drawbody();

	bool Drawcloth_V3();	//画衣服模型，Version 3

	void AlignmentClothesAndHuman();
	CVector3 SleeveAlign();

	CVector3 AlignmentTrousesAndHuman();

	void CollisionCheckForAlign(all_Physics *thisphysic, Human::Skeletonchunk *sk_chunk);

public:
	//模型数据
	CLoad3DS::t3DModel m_3DModel;		// 3dmax模型读取类的对象，该类定义在"Load3DS.h"文件中
	all_Physics thisphysic;   // 衣服模型重构类的对象，如构造衣物的质点弹簧模型，与 m_3DModel对应，定义在"Mass-SpringModel.h"中
	Human::Skeletonchunk *sk_chunk;//人体网格对象，该对象的函数中，完成人体网格的生成，OBB包围盒，圆柱映射表的构建以及其他基本功能函数，定义在“human.h”文件中


};

#endif // !defined(AFX_3DSMODEL_H__98FC8CCA_A44F_4C45_9C99_CAC11614A519__INCLUDED_)
