// 3DSModel.h: interface for the C3DSModel class.
//    
//     �й�3D������������
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


//3ds ģ����
class C3DSModel
{
public:
	//�ļ���
	char m_sFileName[100];
	//��־
	bool m_bModelOK;

	C3DSModel(void);
	virtual ~C3DSModel(void);
	// ����ģ���ļ�
	bool LoadModelFromFile(char* sfilename,int key);//key ���ﴫ�����·����ͣ������»��ǿ��ӣ�0��ʾ���£�1��ʾ���ӣ�Ϊ���������?��Ϊ���ºͿ��ӵ����ű����Լ�ƽ��������岻һ��	
	// ����ģ��
	bool Drawcloth(int key);
	bool Drawbody();

	bool Drawcloth_V3();	//���·�ģ�ͣ�Version 3

	void AlignmentClothesAndHuman();
	CVector3 SleeveAlign();

	CVector3 AlignmentTrousesAndHuman();

	void CollisionCheckForAlign(all_Physics *thisphysic, Human::Skeletonchunk *sk_chunk);

public:
	//ģ������
	CLoad3DS::t3DModel m_3DModel;		// 3dmaxģ�Ͷ�ȡ��Ķ��󣬸��ඨ����"Load3DS.h"�ļ���
	all_Physics thisphysic;   // �·�ģ���ع���Ķ����繹��������ʵ㵯��ģ�ͣ��� m_3DModel��Ӧ��������"Mass-SpringModel.h"��
	Human::Skeletonchunk *sk_chunk;//����������󣬸ö���ĺ����У����������������ɣ�OBB��Χ�У�Բ��ӳ���Ĺ����Լ������������ܺ����������ڡ�human.h���ļ���


};

#endif // !defined(AFX_3DSMODEL_H__98FC8CCA_A44F_4C45_9C99_CAC11614A519__INCLUDED_)
