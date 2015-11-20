#include "stdafx.h"
#include "Mass-SpringModel.h"
#include <fstream>
#include <string>
using namespace std;
ofstream f;
//Particle *tempparticle[5];//存储质点信息的临时数组

int extern q[10];//“定义在human.cpp”中
int extern w[10];
int extern e[10];
int extern r[10];


int extern h[10];//“定义在human.cpp”中
int extern v[10];
int Xcount=0;
int extern Randint(int min, int max) {//生成在[min,max]中的随机数
	int	number;
    number = (((abs(rand())%(max-min+1))+min));    
    
    if(number>max) {
    	number = max;
    }
    
    if(number<min) {
    	number = min;
    }	
		
	return number;
}

CMSModel::CMSModel(){
	WindVector.x=0;
	WindVector.y=0;
	WindVector.z=0;
}

CMSModel::~CMSModel(){
		
}
////确定衣服上的每个质点所属人体的部位，进行标记
void  CMSModel::surePP(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk){
	Particle *particle;
	CVector3 v;
	int i,j;
	for(i=0;i<thisphysic->objectnumbers;i++)
	{
		for(j=0;j<thisphysic->physics[i].particlenumbers;j++)
		{
			particle=&(thisphysic->physics[i].pParticles[j]);
			particle->Cylindex=-1;
			//y值比臀部大，且x值比肩部小
			//if(particle->vPosition.y>=sk_chunk[1].vertex[20].Coordinates.y)
				//particle->bLocked==1;
			
			if (particle->vPosition.y >= sk_chunk[2].vertex[40].Coordinates.y&&particle->vPosition.y<sk_chunk[1].vertex[12].Coordinates.y&&particle->vPosition.x>sk_chunk[9].vertex[0].Coordinates.x - 0.5&&particle->vPosition.x<sk_chunk[7].vertex[0].Coordinates.x + 0.5)
				particle->belongtoPart = 1;		//表示属于人体的第1部分(身体)	
			else{
				//y值比身体小，比大腿大，则为臀部
				if(particle->vPosition.y<sk_chunk[1].vertex[21*20+2].Convexhull.y&&particle->vPosition.y>sk_chunk[3].vertex[0].Convexhull.y&&particle->vPosition.x>sk_chunk[9].vertex[0].Coordinates.x-0.7&&particle->vPosition.x<sk_chunk[7].vertex[0].Coordinates.x+0.7)
					particle->belongtoPart=2;
				else{
					if(particle->vPosition.x<sk_chunk[9].vertex[0].Convexhull.x&&particle->vPosition.y>sk_chunk[3].vertex[21*10].Convexhull.y)
					{//左大臂和左小臂，分别是9和0
						v=Vector(particle->vPosition,sk_chunk[0].vertex[0].Convexhull);
						if (Dot(v, sk_chunk[0].v) > 0) {
							//这里我做了修改——黄璐宸2015.06.08
							if (particle->vPosition.y > sk_chunk[0].vertex[0].Convexhull.y)
								particle->belongtoPart = 9;
							else
								particle->belongtoPart = 1;
						}
						else particle->belongtoPart=0; 
					}
					else
					{
						if(particle->vPosition.x>sk_chunk[7].vertex[0].Convexhull.x&&particle->vPosition.y>sk_chunk[3].vertex[21*10].Convexhull.y)
						{//右大臂和右小臂，分别是7和8
							v=Vector(particle->vPosition,sk_chunk[9].vertex[0].Convexhull);
							if(Dot(v,sk_chunk[9].v)>0)particle->belongtoPart=7; 
							else particle->belongtoPart=9;
						}
						//else
						//{
						//	if(particle->vPosition.x>=(sk_chunk[1].vertex[2].Coordinates.x+sk_chunk[1].vertex[12].Coordinates.x)/2){
						//		v=Vector(particle->vPosition,sk_chunk[5].vertex[20*(21)].Convexhull);
						//		if(Dot(v,sk_chunk[5].v)>=0)particle->belongtoPart=3; //第3和5部分,右大腿和右小腿
						//		else particle->belongtoPart=5;
						//	}
							//else
								//{
							//	v=Vector(particle->vPosition,sk_chunk[6].vertex[20*21].Convexhull);
							//	if(Dot(v,sk_chunk[6].v)>=0)particle->belongtoPart=4; //第4和6部分
							//	else particle->belongtoPart=6;
							//}
						//}
					}
				}
			}	
			if(particle->vPosition.y<=sk_chunk[3].vertex[0].Coordinates.y&&particle->belongtoPart!=8&&particle->belongtoPart!=0)
			{
				//为腿部
				if(particle->vPosition.x>(sk_chunk[1].vertex[2].Coordinates.x+sk_chunk[1].vertex[12].Coordinates.x)/2)
				//if(particle->vPosition.x>sk_chunk[3].vertex[0].)
				{
					v=Vector(particle->vPosition,sk_chunk[5].vertex[20*(21)].Coordinates);
					if(Dot(v,sk_chunk[5].v)>=0)
						particle->belongtoPart=3; //第3和5部分,右大腿和右小腿
					else 
						particle->belongtoPart=5;
				}
		/*		if(particle->vPosition.x<(sk_chunk[2].vertex[2].Coordinates.x+sk_chunk[2].vertex[12].Coordinates.x)/2&&particle->vPosition.y>sk_chunk[6].vertex[0].Coordinates.y)
					particle->belongtoPart=4;*/
				if(particle->vPosition.x<(sk_chunk[2].vertex[2].Coordinates.x+sk_chunk[2].vertex[12].Coordinates.x)/2)
				{
					v=Vector(particle->vPosition,sk_chunk[6].vertex[20*21+10].Coordinates);
					if(Dot(v,sk_chunk[6].v)>=0)
						particle->belongtoPart=4; //第4和6部分
					else 
						particle->belongtoPart=6;
				}
			}
		}
	}
}
//为模型中的每个对象的所有质点添加物理信息
void CMSModel::Initialize(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,int key){
	int i,maxyindex;
	CLoad3DS::vertInfo *vert;
	double maxpointy=-10000;
	//ofstream f("F:\\Myprojects\\质点-弹簧模型\\3ds读取\\简化点个数.txt");所有的文件输入输出操作均为测试所需！！！
	int count=0;
	thisphysic->objectnumbers=m_3DModel.numOfObjects;
	thisphysic->physics=new one_Physic[thisphysic->objectnumbers];
    memset(thisphysic->physics, 0, sizeof(one_Physic) * (thisphysic->objectnumbers));

	for(i=0;i<thisphysic->objectnumbers;i++)
	{//循环搜索每个对象，为每个对象的所有质点添加物理信息
		vert=m_3DModel.vctMeshInfos[i].vertInfos;
		thisphysic->physics[i].particlenumbers=m_3DModel.vctMeshInfos[i].vertInfcounts;//为thisphysic中的每个对象获得质点个数
		thisphysic->physics[i].pOriginnumbers=thisphysic->physics[i].particlenumbers;
		thisphysic->physics[i].pParticles=new Particle[thisphysic->physics[i].particlenumbers+1000];
		memset(thisphysic->physics[i].pParticles,0,sizeof(Particle)*thisphysic->physics[i].particlenumbers);//分配内存空间
		thisphysic->physics[i].mass=PHISYCSMASS;
		for(int j=0;j<thisphysic->physics[i].particlenumbers;j++){   //循环扫描每个对象的质点，为其添加物理信息
			
			if(vert[j].Coordinates.y>maxpointy){
				maxpointy=vert[j].Coordinates.y;
				maxyindex=j;
			}
			thisphysic->physics[i].pParticles[j].fMass=(thisphysic->physics[i].mass/(3*m_3DModel.vctObjects[i].numOfFaces))*vert[j].AdjFaceCounts;//thisphysic->physics[i].mass/thisphysic->physics[i].particlenumbers;//
			thisphysic->physics[i].pParticles[j].fInvMass=(1/thisphysic->physics[i].pParticles[j].fMass);

			thisphysic->physics[i].pParticles[j].vForces.Set(0,0,0);
			thisphysic->physics[i].pParticles[j].vVelocity.Set(0,0,0);
			thisphysic->physics[i].pParticles[j].vAcceleration.Set(0,0,0);
			thisphysic->physics[i].pParticles[j].vTempPC.Set(0,0,0);
			thisphysic->physics[i].pParticles[j].vPosition.x=vert[j].Coordinates.x;
			thisphysic->physics[i].pParticles[j].vPosition.y=vert[j].Coordinates.y;
			thisphysic->physics[i].pParticles[j].vPosition.z=vert[j].Coordinates.z;

			thisphysic->physics[i].pParticles[j].vprePosition.x=vert[j].Coordinates.x;
			thisphysic->physics[i].pParticles[j].vprePosition.y=vert[j].Coordinates.y;
			thisphysic->physics[i].pParticles[j].vprePosition.z=vert[j].Coordinates.z;

			//f<<"点序号："<<j<<"  y坐标值:"<<vert[j].Coordinates.y<<"   x坐标："<<vert[j].Coordinates.x<<"   z坐标："<<vert[j].Coordinates.z<<endl;
			if(m_3DModel.vctMeshInfos[i].brivertInfos[j].AdjVertCounts!=-1){
				thisphysic->physics[i].pParticles[j].isbrief=true;
				////f<<"点序号："<<j<<"  y坐标值:"<<vert[j].Coordinates.y<<"   x坐标："<<vert[j].Coordinates.x<<"   z坐标："<<vert[j].Coordinates.z<<endl;
			}
			else
				thisphysic->physics[i].pParticles[j].isbrief=false;

			thisphysic->physics[i].pParticles[j].iscollision=false;

			thisphysic->physics[i].pParticles[j].isSelfCollision = false;

			thisphysic->physics[i].pParticles[j].isrotation=false;
		}
		//f.close();
	}
	
	surePP(thisphysic, sk_chunk);


	////上衣和裤子的对齐——薛原做的对齐
	//double dx,dy,dz;
	//if(key==0)
	//{//上衣
	//	for(i=0;i<thisphysic->objectnumbers;i++)
	//	{
	//		
	//		dx=(sk_chunk[1].vertex[2].Coordinates.x+sk_chunk[1].vertex[12].Coordinates.x)/2-thisphysic->physics[i].pParticles[maxyindex].vPosition.x;
	//		dy=(sk_chunk[1].vertex[11].Coordinates.y)-thisphysic->physics[i].pParticles[maxyindex].vPosition.y;
	//		dz=(sk_chunk[1].vertex[7].Coordinates.z+sk_chunk[1].vertex[7].Coordinates.z)/2-0.15-thisphysic->physics[i].pParticles[maxyindex].vPosition.z;
	//		for(int j=0;j<thisphysic->physics[i].particlenumbers;j++){
	//			//修改的地方
	//			thisphysic->physics[i].pParticles[j].vPosition.x+=dx-1.5;
	//			thisphysic->physics[i].pParticles[j].vPosition.y+=dy-0.4;
	//			thisphysic->physics[i].pParticles[j].vPosition.z+=dz-0.2;//衬衫-0.23

	//			//thisphysic->physics[i].pParticles[j].vPosition.y+=dy+0.2;//衣裙1、2---+0.4//衬衫+0.5
	//			//thisphysic->physics[i].pParticles[j].vPosition.z+=dz+0.26;//衣裙1、2--------（+0.45消除衣袋变形需要）衣裙2--------（-0.2看后背需要）
	//			thisphysic->physics[i].pParticles[j].vprePosition=thisphysic->physics[i].pParticles[j].vPosition;
	//	
	//			if(thisphysic->physics[i].pParticles[j].vPosition.y>=11.23)//
	//				thisphysic->physics[i].pParticles[j].bLocked=1;
	//			else
	//				thisphysic->physics[i].pParticles[j].bLocked=0;
	//		}
	//	}
	//}
	//if(key==1)
	//{//裤子
	//	for(i=0;i<1;i++)
	//	
	//	{
	//		dy=sk_chunk[2].vertex[0].Coordinates.y-thisphysic->physics[i].pParticles[maxyindex].vPosition.y;
	//		for(int j=0;j<thisphysic->physics[i].particlenumbers;j++){
	//			/*thisphysic->physics[i].pParticles[j].vPosition.x-=0.2;*/
	//			thisphysic->physics[i].pParticles[j].vPosition.y+=dy;
	//			thisphysic->physics[i].pParticles[j].vPosition.z+=0.2;//裙子+0.2
	//			thisphysic->physics[i].pParticles[j].vprePosition=thisphysic->physics[i].pParticles[j].vPosition;
	//			//if(thisphysic->physics[i].pParticles[j].vPosition.y>sk_chunk[2].vertex[21].Coordinates.y)
	//				//thisphysic->physics[i].pParticles[j].bLocked=true;
	//		}
	//	}
	//}
	
}

//构建弹簧
void  CMSModel::CreateSpring(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel){
	CLoad3DS::edgeInfo *edge;
	CLoad3DS::vertInfo *vert;
	Particle*particle;
	CLoad3DS::faceInfo face1,face2;//用于存储一条斜边的两个邻接面的临时变量
	CLoad3DS::edgeInfo v_Adjedge1[5],v_Adjedge2[6];//用于存储一个顶点的邻接竖边和邻接横边的两个临时数组，经过测试这两类数据的大小不会超过5个。
	int v_v1[5],v_v2[6];//用于分别记录一个顶点的两个邻接边数组v_Adjedge1[5]和v_Adjedge2[5]中的边的另一个（与该顶点相对应的）端点索引
	CVector3 L;
	Spring newspring;
	int tempnumbers;           //用于记录弹簧个数的临时变量
	int i,j,k,t,brinb;
	for(i=0;i<m_3DModel.numOfMeshInfos;i++){
		vert=m_3DModel.vctMeshInfos[i].vertInfos;
		edge=m_3DModel.vctMeshInfos[i].edgeInfos;
		thisphysic->physics[i].springnumbers=0;
		tempnumbers=0;
		particle=thisphysic->physics[i].pParticles;

		//为网格对象添加结构弹簧和剪切弹簧
		for(j=0;j<m_3DModel.vctMeshInfos[i].edgeInfcounts;j++){
			thisphysic->physics[i].springnumbers++;
			tempnumbers=thisphysic->physics[i].springnumbers;
			thisphysic->physics[i].pSprings.push_back(newspring);//增加一个弹簧
			memset(&(thisphysic->physics[i].pSprings[tempnumbers - 1]), 0, sizeof(Spring));//为增加的弹簧分配空间
			//为该弹簧添加信息
			thisphysic->physics[i].pSprings[tempnumbers - 1].index1=edge[j].polesinfo->vertIndex[0];
			thisphysic->physics[i].pSprings[tempnumbers - 1].index2=edge[j].polesinfo->vertIndex[1];

			L=particle[edge[j].polesinfo->vertIndex[0]].vPosition-particle[edge[j].polesinfo->vertIndex[1]].vPosition;
			if(edge[j].polesinfo->flag!=3){	//该边不为斜边，则添加结构弹簧
				if(edge[j].polesinfo->flag==4)
					thisphysic->physics[i].pSprings[tempnumbers - 1].type=4;
				else
					thisphysic->physics[i].pSprings[tempnumbers - 1].type=STRUCTURAL_SPRING;
				thisphysic->physics[i].pSprings[tempnumbers - 1].Kd=STRUCTGDAMPINGCONSTANT;
				thisphysic->physics[i].pSprings[tempnumbers - 1].Ks=STRUCTTENSIONCONSTANT;
				thisphysic->physics[i].pSprings[tempnumbers - 1].restLen=SPRINGORIGINLENGTH*Magnitude(L);
				thisphysic->physics[i].pSprings[tempnumbers - 1].maxLen=(1+SPRINGMAXELONGATION)*Magnitude(L);
				thisphysic->physics[i].pSprings[tempnumbers - 1].curLen=Magnitude(L);
			}
			
			else
			{          //该边是斜边，则添加剪切弹簧
			    thisphysic->physics[i].pSprings[tempnumbers - 1].type=SHEAR_SPRING;
				thisphysic->physics[i].pSprings[tempnumbers - 1].Kd=SHEARGDAMPINGCONSTANT;
				thisphysic->physics[i].pSprings[tempnumbers - 1].Ks=SHEARTENSIONCONSTANT;
				thisphysic->physics[i].pSprings[tempnumbers - 1].restLen=Magnitude(L);
				thisphysic->physics[i].pSprings[tempnumbers - 1].maxLen=(1+SPRINGMAXELONGATION)*Magnitude(L);
				thisphysic->physics[i].pSprings[tempnumbers - 1].curLen=Magnitude(L);
				if((edge[j].polesinfo->AdjFaceCounts)==2){//添加另一个剪切弹簧
					thisphysic->physics[i].springnumbers++;
					tempnumbers++;
					thisphysic->physics[i].pSprings.push_back(newspring);
					memset(&(thisphysic->physics[i].pSprings[tempnumbers - 1]), 0, sizeof(Spring));
					//获取该边的两个邻面
					face1=m_3DModel.vctMeshInfos[i].faceInfos[edge[j].polesinfo->AdjFacesIndex[0]];
					face2=m_3DModel.vctMeshInfos[i].faceInfos[edge[j].polesinfo->AdjFacesIndex[1]];
					//将这两个面的第二个顶点索引号分别赋给新添加的剪切弹簧（的两个端点）
					thisphysic->physics[i].pSprings[tempnumbers - 1].index1=face1.triangleinfo->vertIndex[1];
					thisphysic->physics[i].pSprings[tempnumbers - 1].index2=face2.triangleinfo->vertIndex[1];

					//L=vert[face1.triangleinfo->vertIndex[1]].Coordinates-vert[face2.triangleinfo->vertIndex[1]].Coordinates;
					L=particle[face1.triangleinfo->vertIndex[1]].vPosition-particle[face2.triangleinfo->vertIndex[1]].vPosition;
                    thisphysic->physics[i].pSprings[tempnumbers - 1].restLen=Magnitude(L);
					thisphysic->physics[i].pSprings[tempnumbers - 1].maxLen=(1+SPRINGMAXELONGATION)*Magnitude(L);
					thisphysic->physics[i].pSprings[tempnumbers - 1].curLen=Magnitude(L);

					thisphysic->physics[i].pSprings[tempnumbers - 1].type=SHEAR_SPRING;
					thisphysic->physics[i].pSprings[tempnumbers - 1].Kd=SHEARGDAMPINGCONSTANT;
					thisphysic->physics[i].pSprings[tempnumbers - 1].Ks=SHEARTENSIONCONSTANT;
				}
			}
			
		}
	}
}

void CMSModel::SetWindVector(double x, double y, double z){
	WindVector.x=x;
	WindVector.y=y;
	WindVector.z=z;
}

void CMSModel::CalcForces(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,PiMotion*pi_m){
	int i,j,k,randnumber;
	double L,G;
	CVector3 damvector,dist,v,vwind(WINDFACTOR,0,0);
	Particle *particle;
	Spring *spring;

	//处理运动
	for (k = 0; k<10; k++){
		if (pi_m[k].ismotion){
			//处理人体与衣物的动画
			if (pi_m[k].isfirstSet)
				surePP(thisphysic, sk_chunk);
			Animation(thisphysic, m_3DModel, sk_chunk, k, pi_m);
		}
	}



	for(k=0;k<thisphysic->objectnumbers;k++){
		for(i=0;i<thisphysic->physics[k].particlenumbers;i++){
			particle=&(thisphysic->physics[k].pParticles[i]);
			if(particle->bLocked==0)
			{
				//计算每个质点的重力
				G=(double)GRAVITY * (particle->fMass);
				particle->vForces.Set(0.0,G,0.0); 
				/*particle->vForces=particle->vForces+vwind;*/
				//计算空气的阻力，与物体的速度有关
				damvector=particle->vVelocity * (-1.0);
				Normalize(damvector);
				L=Magnitude(particle->vVelocity)*Magnitude(particle->vVelocity);
				L=L*(double)DRAGCOEFFICIENT ;
				particle->vForces=particle->vForces + damvector * L;
				
			}
		}
		int p1,p2;
		CVector3 f1,f2;
		for(i=0;i<thisphysic->physics[k].springnumbers;i++){
			spring=&(thisphysic->physics[k].pSprings[i]);
			
			p1=spring->index1;
			p2=spring->index2;

		    dist=thisphysic->physics[k].pParticles[p1].vPosition-thisphysic->physics[k].pParticles[p2].vPosition;
		
			if(Magnitude(dist)!=0){
				v=thisphysic->physics[k].pParticles[p1].vVelocity - thisphysic->physics[k].pParticles[p2].vVelocity;
			    L=spring->restLen;
				f1= (dist/(Magnitude(dist)))*(-(spring->Ks * (Magnitude(dist)-L) + spring->Kd*(Dot(dist,v)/Magnitude(dist))));
				f2=f1*(-1);	
				if(thisphysic->physics[k].pParticles[p1].bLocked!=1)
				{
					thisphysic->physics[k].pParticles[p1].vForces=thisphysic->physics[k].pParticles[p1].vForces + f1;
				}
				if(thisphysic->physics[k].pParticles[p2].bLocked!=1)
				{
					thisphysic->physics[k].pParticles[p2].vForces=thisphysic->physics[k].pParticles[p2].vForces + f2;
				}
			}
		}
    }
}
void CMSModel::Verlet(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk,double dt){
	//重新计算动画的人体部位
	static int step=0;
	CVector3 a,b,dist,tempPosition,xchan,vcmpo;//
	int i,j;
	int p1,p2;
	double r,distlengh;
	Particle *particle;
	Spring *spring;
	b.Set(1,1,1);
	//surePP(thisphysic,sk_chunk);//每次计算得新位置都划分一次
	for(i=0;i<thisphysic->objectnumbers;i++){//利用Verlet积分算法计算出每个质点的新位置
		for(j=0;j<thisphysic->physics[i].particlenumbers;j++){
			particle=&(thisphysic->physics[i].pParticles[j]);
			if(particle->bLocked==0)
			{
				a=particle->vForces * particle->fInvMass;
				particle->vAcceleration=a;
				tempPosition=particle->vPosition;
				particle->vPosition = particle->vPosition*2 - particle->vprePosition + a*dt*dt;//+ (b*dt*dt*dt*dt);
				particle->vprePosition=tempPosition;
				particle->vVelocity = (particle->vPosition-particle->vprePosition)/(2*dt);// + (b*dt*dt);
				
				

				
				distlengh=Distance(particle->vprePosition,particle->vPosition);//防止不稳定所采取的措施，指点前后距离大于5时，则修改为前一步的位置，设为固定

				if((distlengh>5)){
			
					particle->vPosition=particle->vprePosition;		

					particle->vVelocity.Set(0,0,0);
					//particle->bLocked=1;
				}

				//SingleCheck(particle,sk_chunk);
			}
		}
	}
	
	//弹簧位置的校正
	
	int count;
	for(count=0;count<3;count++)
	{
		for(i=0;i<thisphysic->objectnumbers;i++){
			for(j=0;j<thisphysic->physics[i].springnumbers;j++){
				spring=&(thisphysic->physics[i].pSprings[j]);
				p1=spring->index1;
				p2=spring->index2;
				dist=thisphysic->physics[i].pParticles[p1].vPosition - thisphysic->physics[i].pParticles[p2].vPosition;
				if(spring->type!=3)//对于非弯曲弹簧
				{
					if(Magnitude(dist)!=0){
					spring->curLen=Magnitude(dist);
					if(Magnitude(dist)>spring->maxLen)//当弹簧大于最大长度时
					{
						b=Normalize(dist)*(spring->curLen-spring->maxLen);
						r=thisphysic->physics[i].pParticles[p1].fInvMass/(thisphysic->physics[i].pParticles[p1].fInvMass + thisphysic->physics[i].pParticles[p2].fInvMass);
						if(thisphysic->physics[i].pParticles[p1].bLocked==0)
						{
							thisphysic->physics[i].pParticles[p1].vPosition = thisphysic->physics[i].pParticles[p1].vPosition - b*r;
							//thisphysic->physics[i].pParticles[p1].vVelocity.Set(0,0,0);
						}
						r=thisphysic->physics[i].pParticles[p2].fInvMass/(thisphysic->physics[i].pParticles[p1].fInvMass + thisphysic->physics[i].pParticles[p2].fInvMass);
						if(thisphysic->physics[i].pParticles[p2].bLocked==0)
						{
							thisphysic->physics[i].pParticles[p2].vPosition = thisphysic->physics[i].pParticles[p2].vPosition + b*r; 
							//thisphysic->physics[i].pParticles[p2].vVelocity.Set(0,0,0);
						}
					}
					}
				}
			}
		}		
	}
	surePP(thisphysic,sk_chunk);//每次计算得新位置都划分一次
	//glColor3ub(0,255,0);
	//glPointSize(10);
	//glBegin(GL_POINTS);
	//for(int i=0;i<thisphysic->objectnumbers;i++)
	//{
	//for(int m=0;m<thisphysic->physics[i].particlenumbers;m++)
	//{
	//	if(thisphysic->physics[i].pParticles[m].belongtoPart==4)
	//	{
	//		glColor3ub(255,0,0);
	//		glVertex3f(thisphysic->physics[i].pParticles[m].vPosition.x,thisphysic->physics[i].pParticles[m].vPosition.y,thisphysic->physics[i].pParticles[m].vPosition.z);
	//	}
	//	//if(thisphysic->physics[i].pParticles[m].belongtoPart==4)
	//	//{
	//	//	glColor3ub(255,255,0);
	//	//	glVertex3f(thisphysic->physics[i].pParticles[m].vPosition.x,thisphysic->physics[i].pParticles[m].vPosition.y,thisphysic->physics[i].pParticles[m].vPosition.z);
	//	//}
	//}
	//}
	//glEnd();
	CollisionCheck(thisphysic,sk_chunk);
	step++;
}
void CMSModel::SingleCheck(Particle *particle,Human::Skeletonchunk *sk_chunk)
{
	int vert,parti;
	int a=0;
	bool isinternal;
	CVector3 nNormal,vector,v;
	int indexz,indexQ,index,bound;
	double z,y;
	parti=particle->belongtoPart;
	isinternal=true;
		particle->iscollision=true;
	//与人体网格进行碰撞
	particle->plocal_xyz.x=sk_chunk[parti].InvertofM[0]*particle->vPosition.x+sk_chunk[parti].InvertofM[4]*particle->vPosition.y+sk_chunk[parti].InvertofM[8]*particle->vPosition.z+sk_chunk[parti].InvertofM[12];
	particle->plocal_xyz.y=sk_chunk[parti].InvertofM[1]*particle->vPosition.x+sk_chunk[parti].InvertofM[5]*particle->vPosition.y+sk_chunk[parti].InvertofM[9]*particle->vPosition.z+sk_chunk[parti].InvertofM[13];
	particle->plocal_xyz.z=sk_chunk[parti].InvertofM[2]*particle->vPosition.x+sk_chunk[parti].InvertofM[6]*particle->vPosition.y+sk_chunk[parti].InvertofM[10]*particle->vPosition.z+sk_chunk[parti].InvertofM[14];
	particle->plocal_rQz.x=sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y+particle->plocal_xyz.z*particle->plocal_xyz.z);
	z=particle->plocal_xyz.z;
	y=particle->plocal_xyz.y;
	if(z>=0&&y>0)particle->plocal_rQz.y=atan(z/y);
	if(z>0&&y==0)particle->plocal_rQz.y=PI*0.5;
	if(z<0&&y==0)particle->plocal_rQz.y=PI*1.5;
	if(y<0)particle->plocal_rQz.y=PI+atan(z/y);
	if(z<0&&y>0)particle->plocal_rQz.y=2*PI+atan(z/y);
	particle->plocal_rQz.z=particle->plocal_xyz.x;
	indexz=(sk_chunk[parti].z_min_max[1]-particle->plocal_rQz.z)/sk_chunk[parti].dz;
	if(indexz==21)indexz=21-1;
	bound=21;
	indexQ=particle->plocal_rQz.y/sk_chunk[parti].dQ;
	if(indexQ==bound)indexQ=0;
	index=indexz*bound+indexQ;
	//if(index<21*bound&&index>=0)
	{
		
		float mindist=10000.0;
		float tempdist;
		int triindex;
		for(int m=0;m<sk_chunk[parti].numofTri;m++)
		{
			CVector3 trian;
			trian=sk_chunk[parti].vertex[sk_chunk[parti].triangle[m].vert[0]].Convexhull;
			tempdist=Magnitude(particle->vPosition-trian);
			if(tempdist<mindist)
			{
				mindist=tempdist;
				triindex=m;
			}
		}
		vert=sk_chunk[parti].triangle[triindex].vert[0];
		vector=particle->vPosition-sk_chunk[parti].vertex[vert].Convexhull;
		nNormal=sk_chunk[parti].triangle[triindex].Normalizehull;
		if(Dot(vector,nNormal)>0)
		{
			isinternal=false;

		}
								//if(isinternal)
		{
			particle->iscollision=true;			
									//if(isinside(tempPC,sk_chunk,k,triindex))
			{
			particle->vPosition=sk_chunk[parti].vertex[sk_chunk[parti].triangle[triindex].vert[0]].Convexhull;
			particle->vprePosition=particle->vPosition;
			particle->iscollision=false;
			particle->bLocked=1;
			}
									//else particle->iscollision=false;

		}
	}
	//else particle->iscollision=false;
}
/*
void CMSModel::SingleCheck(Particle *particle,Human::Skeletonchunk *sk_chunk)
{
	int vert,parti;
	int a=0;
	bool isinternal;
	CVector3 nNormal,vector,v,tempPC;
	int indexz,indexQ,index,bound;
	double z,y;
	parti=particle->belongtoPart;
	isinternal=true;
	for(int k=0;k<6;k++)//先与OBB包围盒进行碰撞检测
	{
		vert=sk_chunk[parti].Obbface[k].vert[0];
		vector=particle->vPosition-sk_chunk[parti].AnglePoint[vert];
		nNormal=sk_chunk[parti].Obbface[k].Normalize;
		if(Dot(vector,nNormal)>0)
		{
			isinternal=false;
			break;
		}
	}
	if(isinternal)
		particle->iscollision=true;//在OBB内，记为碰撞
	//进一步与人体网格进行碰撞
	particle->plocal_xyz.x=sk_chunk[parti].InvertofM[0]*particle->vPosition.x+sk_chunk[parti].InvertofM[4]*particle->vPosition.y+sk_chunk[parti].InvertofM[8]*particle->vPosition.z+sk_chunk[parti].InvertofM[12];
	particle->plocal_xyz.y=sk_chunk[parti].InvertofM[1]*particle->vPosition.x+sk_chunk[parti].InvertofM[5]*particle->vPosition.y+sk_chunk[parti].InvertofM[9]*particle->vPosition.z+sk_chunk[parti].InvertofM[13];
	particle->plocal_xyz.z=sk_chunk[parti].InvertofM[2]*particle->vPosition.x+sk_chunk[parti].InvertofM[6]*particle->vPosition.y+sk_chunk[parti].InvertofM[10]*particle->vPosition.z+sk_chunk[parti].InvertofM[14];
	particle->plocal_rQz.x=sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y+particle->plocal_xyz.z*particle->plocal_xyz.z);
	z=particle->plocal_xyz.z;
	y=particle->plocal_xyz.y;
	if(z>=0&&y>0)particle->plocal_rQz.y=atan(z/y);
	if(z>0&&y==0)particle->plocal_rQz.y=PI*0.5;
	if(z<0&&y==0)particle->plocal_rQz.y=PI*1.5;
	if(y<0)particle->plocal_rQz.y=PI+atan(z/y);
	if(z<0&&y>0)particle->plocal_rQz.y=2*PI+atan(z/y);
	particle->plocal_rQz.z=particle->plocal_xyz.x;
	indexz=(sk_chunk[parti].z_min_max[1]-particle->plocal_rQz.z)/sk_chunk[parti].dz;
	if(indexz==21)indexz=21-1;
	bound=21;
	indexQ=particle->plocal_rQz.y/sk_chunk[parti].dQ;
	if(indexQ==bound)indexQ=0;
	index=indexz*bound+indexQ;
	if(index<21*bound&&index>=0)
	{
		isinternal=true;
		for(int k=0;k<sk_chunk[parti].Cylindricalmap[index].numofTri;k++)
		{
			a=sk_chunk[parti].Cylindricalmap[index].Triindex[k];
			vert=sk_chunk[parti].triangle[a].vert[0];
			vector=particle->vPosition-sk_chunk[parti].vertex[vert].Convexhull;
			nNormal=sk_chunk[parti].triangle[a].Normalizehull;
			if(Dot(vector,nNormal)>0)
			{
				isinternal=false;
				break;
			}
			if(isinternal)
			{
				CVector3 p;
				if(parti==1||parti==2)
				{
					p.Set(0,particle->plocal_xyz.y,0);
					particle->vprojection.x=sk_chunk[parti].Mobb[12];
					particle->vprojection.y=p.y+sk_chunk[parti].Mobb[13];
					particle->vprojection.z=sk_chunk[parti].Mobb[14];
				}
				else
				{
					p.Set(particle->plocal_xyz.x,0,0);
					particle->vprojection.x=sk_chunk[parti].Mobb[0] *p.x+sk_chunk[parti].Mobb[12];
					particle->vprojection.y=sk_chunk[parti].Mobb[1] *p.x+sk_chunk[parti].Mobb[13];
					particle->vprojection.z=sk_chunk[parti].Mobb[2] *p.x+sk_chunk[parti].Mobb[14];
				}
				particle->Cylindex=index;
				nNormal=sk_chunk[parti].triangle[a].Normalizehull;
				double t;
				//if(parti==1)
				t=(particle->vPosition.x*nNormal.x+particle->vPosition.y*nNormal.y+particle->vPosition.z*nNormal.z+sk_chunk[parti].triangle[a].dhull)/(vector.x*nNormal.x+vector.y*nNormal.y+vector.z*nNormal.z);
				tempPC.x=particle->vPosition.x-t*vector.x;
				tempPC.y=particle->vPosition.y-t*vector.y;
				tempPC.z=particle->vPosition.z-t*vector.z;
				if(isinside(tempPC,sk_chunk,parti,a)){
					particle->vPosition=tempPC+nNormal*0.05;//线衫1： *0.3
					particle->vprePosition=particle->vPosition;
					particle->iscollision=false;
					particle->bLocked=1;
					break;
				}
			}
			else particle->iscollision=false;
		}
	}
	else particle->iscollision=false;
}
*/

void CMSModel::ClothAnimation(all_Physics *thisphysic, CLoad3DS::t3DModel m_3DModel, Human::Skeletonchunk *sk_chunk, int parti, float angle, float x, float y, float z, const M3DMatrix44f matrix0, const M3DMatrix44f matrix1, const M3DMatrix44f matrix2){
	M3DVector3f objectVertex;
	M3DVector3f transformedVertex;
	M3DVector3f transformedVertex1;
	M3DVector3f transformedVertex2;
	Particle *particle;
	Spring *spring;
	//为重构的网格所添加的点申请内存
	for (int i = 0; i<thisphysic->objectnumbers; i++){
		thisphysic->physics[i].datacouple[parti] = new Datapart[400];//每个部分分配400个质点索引对信息
		memset(thisphysic->physics[i].datacouple[parti], 0, sizeof(Datapart) * (400));
	}
	for (int i = 0; i<thisphysic->objectnumbers; i++){
		for (int j = 0; j<thisphysic->physics[i].particlenumbers; j++){
			particle = &(thisphysic->physics[i].pParticles[j]);
			if (parti != 2){       //不是身体
				if (particle->belongtoPart == parti){
					particle->vPositionCopy = particle->vPosition;//记录原位置
					objectVertex[0] = particle->vPosition.x;
					objectVertex[1] = particle->vPosition.y;
					objectVertex[2] = particle->vPosition.z;
					CVector3 v = Vector(particle->vPosition, sk_chunk[parti].vertex[0].Convexhull);
					if (angle>0){
						if (parti == 4 || parti == 6)//小腿
							if (Dot(v, sk_chunk[parti].vseN)>0)particle->isrotation = true;
						if (parti == 0 || parti == 8)//小臂
							if (Dot(v, sk_chunk[parti].vseN)<0)particle->isrotation = true;
					}
					m3dTransformVector3(transformedVertex, objectVertex, matrix2);
					m3dTransformVector3(transformedVertex1, transformedVertex, matrix0);
					m3dTransformVector3(transformedVertex2, transformedVertex1, matrix1);
					particle->vPosition.x = transformedVertex2[0];
					particle->vPosition.y = transformedVertex2[1];
					particle->vPosition.z = transformedVertex2[2];
					particle->vprePosition = particle->vPosition;
					if (x != 0 && y == 0 && z == 0){//绕x轴旋转
						v = Vector(particle->vPosition, sk_chunk[parti].vertex[0].Coordinates);//要改
						if (angle>0){//逆时针转
							if (Dot(v, sk_chunk[parti].vseN)<0)particle->bLocked = 1;
							else particle->bLocked = 0;
						}
						else{
							if (Dot(v, sk_chunk[parti].vseN)>0)particle->bLocked = 1;
							else particle->bLocked = 0;
						}
					}
					if (x == 0 && y == 0 && z != 0){//绕z轴旋转
						v = Vector(particle->vPosition, sk_chunk[parti].vertex[q[parti] / 2].Coordinates);
						if (angle>0){//逆时针转
							if (Dot(v, sk_chunk[parti].vseN)>0)particle->bLocked = 1;
							else particle->bLocked = 0;
						}
						else{
							if (Dot(v, sk_chunk[parti].vseN)<0)particle->bLocked = 1;
							else particle->bLocked = 0;
						}
					}
					particle->vVelocity.Set(0, 0, 0);
				}
				if ((parti % 2) == 1){         //即parti=3,5,7,9，不会让parti=1的.即大臂或是大腿动将会带动小臂或小腿也动
					int partiadd;          //partiadd记录对应被带动的部分
					if (parti != 9) partiadd = parti + 1;
					else partiadd = 0;

					if (particle->belongtoPart == partiadd){
						particle->vPositionCopy = particle->vPosition;//记录原位置
						objectVertex[0] = particle->vPosition.x;
						objectVertex[1] = particle->vPosition.y;
						objectVertex[2] = particle->vPosition.z;
						//if(particle->vPosition.z>=sk_chunk[5].vertex[0].Convexhull.z)particle->isrotation=true;//要改
						m3dTransformVector3(transformedVertex, objectVertex, matrix2);
						m3dTransformVector3(transformedVertex1, transformedVertex, matrix0);
						m3dTransformVector3(transformedVertex2, transformedVertex1, matrix1);
						particle->vPosition.x = transformedVertex2[0];
						particle->vPosition.y = transformedVertex2[1];
						particle->vPosition.z = transformedVertex2[2];
						particle->vprePosition = particle->vPosition;
						CVector3 v;//
						if (x != 0 && y == 0 && z == 0){//绕x轴旋转
							v = Vector(particle->vPosition, sk_chunk[partiadd].vertex[0].Coordinates);
							if (angle>0){//逆时针转
								if (Dot(v, sk_chunk[partiadd].vseN)<0)particle->bLocked = 1;
								else particle->bLocked = 0;
							}
							else{
								if (Dot(v, sk_chunk[partiadd].vseN)>0)particle->bLocked = 1;
								else particle->bLocked = 0;
							}
						}
						if (x == 0 && y == 0 && z != 0){//绕z轴旋转
							v = Vector(particle->vPosition, sk_chunk[partiadd].vertex[q[parti] / 2].Coordinates);
							if (angle>0){//逆时针转
								if (Dot(v, sk_chunk[partiadd].vseN)>0)particle->bLocked = 1;
								else particle->bLocked = 0;
							}
							else{
								if (Dot(v, sk_chunk[partiadd].vseN)<0)particle->bLocked = 1;
								else particle->bLocked = 0;
							}
						}
						particle->vVelocity.Set(0, 0, 0);
					}
				}
			}
			else{
				if (particle->belongtoPart<3 || particle->belongtoPart>6){//即为0,1,2,7,8,9,非腿部
					particle->vPositionCopy = particle->vPosition;//记录原位置
					objectVertex[0] = particle->vPosition.x;
					objectVertex[1] = particle->vPosition.y;
					objectVertex[2] = particle->vPosition.z;
					//if(particle->vPosition.z>=sk_chunk[5].vertex[0].Convexhull.z)particle->isrotation=true;//要改
					m3dTransformVector3(transformedVertex, objectVertex, matrix2);
					m3dTransformVector3(transformedVertex1, transformedVertex, matrix0);
					m3dTransformVector3(transformedVertex2, transformedVertex1, matrix1);
					particle->vPosition.x = transformedVertex2[0];
					particle->vPosition.y = transformedVertex2[1];
					particle->vPosition.z = transformedVertex2[2];
					particle->vprePosition = particle->vPosition;
					CVector3 v;//
					if (x != 0 && y == 0 && z == 0){//绕x轴旋转
						v = Vector(particle->vPosition, sk_chunk[particle->belongtoPart].vertex[0].Coordinates);
						if (angle>0){//逆时针转
							if (Dot(v, sk_chunk[particle->belongtoPart].vseN)<0)particle->bLocked = 1;
							else particle->bLocked = 0;
						}
						else{
							if (Dot(v, sk_chunk[particle->belongtoPart].vseN)>0)particle->bLocked = 1;
							else particle->bLocked = 0;
						}
					}
					if (x == 0 && y == 0 && z != 0){//绕z轴旋转
						v = Vector(particle->vPosition, sk_chunk[particle->belongtoPart].vertex[q[parti] / 2].Coordinates);
						if (angle>0){//逆时针转
							if (Dot(v, sk_chunk[particle->belongtoPart].vseN)>0)particle->bLocked = 1;
							else particle->bLocked = 0;
						}
						else{
							if (Dot(v, sk_chunk[particle->belongtoPart].vseN)<0)particle->bLocked = 1;
							else particle->bLocked = 0;
						}
					}
					particle->vVelocity.Set(0, 0, 0);
				}
			}
		}
	}
}

void CMSModel::Animation(all_Physics *thisphysic, CLoad3DS::t3DModel m_3DModel, Human::Skeletonchunk *sk_chunk, int i, PiMotion*pi_m){
	M3DMatrix44f   transformationMatrix;
	M3DMatrix44f   transformationMatrix1;
	M3DMatrix44f   transformationMatrix2;
	M3DVector3f objectVertex;
	M3DVector3f transformedVertex;
	M3DVector3f transformedVertex1;
	M3DVector3f transformedVertex2;
	float angle, x, y, z;
	angle = pi_m[i].d_a->angle;
	x = pi_m[i].d_a->x;
	y = pi_m[i].d_a->y;
	z = pi_m[i].d_a->z;
	Human h;
	if (i != 2){
		int q_or_qi;
		if (i == 7 || i == 8)q_or_qi = q[i];//为了保持左右运动的对称性
		else q_or_qi = 0;
		m3dRotationMatrix44(transformationMatrix, angle, x, y, z);
		m3dRotationMatrix44(transformationMatrix1, 0, 0, 0, 1);
		transformationMatrix1[12] = sk_chunk[i].vertex[q_or_qi].Convexhull.x;
		transformationMatrix1[13] = sk_chunk[i].vertex[q_or_qi].Convexhull.y;
		transformationMatrix1[14] = sk_chunk[i].vertex[q_or_qi].Convexhull.z;
		m3dRotationMatrix44(transformationMatrix2, 0, 0, 0, 1);
		transformationMatrix2[12] = -sk_chunk[i].vertex[q_or_qi].Convexhull.x;
		transformationMatrix2[13] = -sk_chunk[i].vertex[q_or_qi].Convexhull.y;
		transformationMatrix2[14] = -sk_chunk[i].vertex[q_or_qi].Convexhull.z;
		//身体部位i旋转一定角度angle
		for (int j = 0; j<sk_chunk[i].numofVert; j++){
			objectVertex[0] = sk_chunk[i].vertex[j].Coordinates.x;
			objectVertex[1] = sk_chunk[i].vertex[j].Coordinates.y;
			objectVertex[2] = sk_chunk[i].vertex[j].Coordinates.z;
			m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
			m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
			m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
			sk_chunk[i].vertex[j].Coordinates.x = transformedVertex2[0];
			sk_chunk[i].vertex[j].Coordinates.y = transformedVertex2[1];
			sk_chunk[i].vertex[j].Coordinates.z = transformedVertex2[2];
		}
		//重新计算每个三角面片的法向量等
		for (int j = 0; j<sk_chunk[i].numofTri; j++){
			int a, b, c;
			a = sk_chunk[i].triangle[j].vert[0];
			b = sk_chunk[i].triangle[j].vert[1];
			c = sk_chunk[i].triangle[j].vert[2];
			h.TriNormalize_d(sk_chunk, i, a, b, c, j);
		}
		//重新计算凸包体
		for (int j = 0; j<sk_chunk[i].numofVert; j++){
			objectVertex[0] = sk_chunk[i].vertex[j].Convexhull.x;
			objectVertex[1] = sk_chunk[i].vertex[j].Convexhull.y;
			objectVertex[2] = sk_chunk[i].vertex[j].Convexhull.z;
			m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
			m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
			m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
			sk_chunk[i].vertex[j].Convexhull.x = transformedVertex2[0];
			sk_chunk[i].vertex[j].Convexhull.y = transformedVertex2[1];
			sk_chunk[i].vertex[j].Convexhull.z = transformedVertex2[2];
		}
		//重新计算凸包体的法向量等
		h.HullTriNormalize_d(sk_chunk, i);
		//重新计算局部坐标系
		CVector3 Vx, tempx, Vy, Vz;
		int index1, index2, jk, k;
		//计算局部坐标系的中心点
		jk = w[i] / 2;
		index1 = jk*(q[i] + 1);
		index2 = index1 + q[i];

		sk_chunk[i].Locenter = (sk_chunk[i].vertex[index1].Coordinates + sk_chunk[i].vertex[index2].Coordinates) / 2.0;
		//if(i==2)sk_chunk[i].Locenter.z+=0.5;

		sk_chunk[i].Mobb[12] = sk_chunk[i].Locenter.x;
		sk_chunk[i].Mobb[13] = sk_chunk[i].Locenter.y;
		sk_chunk[i].Mobb[14] = sk_chunk[i].Locenter.z;
		sk_chunk[i].Mobb[15] = 1;
		//构造Mobb和其逆矩阵InvertofM
		if (i != 1 && i != 2){
			Vy = Normalize(sk_chunk[i].vertex[index1].Coordinates - sk_chunk[i].vertex[index2].Coordinates);
			sk_chunk[i].Mobb[4] = Vy.x;
			sk_chunk[i].Mobb[5] = Vy.y;
			sk_chunk[i].Mobb[6] = Vy.z;
			sk_chunk[i].Mobb[7] = 0;

			index1 = w[i] * (q[i] + 1);
			tempx = Normalize(sk_chunk[i].vertex[0].Coordinates - sk_chunk[i].vertex[index1].Coordinates);
			Vz = Normalize(Cross(tempx, Vy));
			sk_chunk[i].Mobb[8] = Vz.x;
			sk_chunk[i].Mobb[9] = Vz.y;
			sk_chunk[i].Mobb[10] = Vz.z;
			sk_chunk[i].Mobb[11] = 0;

			Vx = Normalize(Cross(Vy, Vz));
			sk_chunk[i].Mobb[0] = Vx.x;
			sk_chunk[i].Mobb[1] = Vx.y;
			sk_chunk[i].Mobb[2] = Vx.z;
			sk_chunk[i].Mobb[3] = 0;
		}
		else{
			Vx = Normalize(sk_chunk[i].vertex[index1].Coordinates - sk_chunk[i].vertex[index2].Coordinates);
			sk_chunk[i].Mobb[0] = Vx.x;
			sk_chunk[i].Mobb[1] = Vx.y;
			sk_chunk[i].Mobb[2] = Vx.z;
			sk_chunk[i].Mobb[3] = 0;

			index1 = w[i] * (q[i] + 1);
			tempx = Normalize(sk_chunk[i].vertex[0].Coordinates - sk_chunk[i].vertex[index1].Coordinates);
			Vz = Normalize(Cross(Vx, tempx));
			sk_chunk[i].Mobb[8] = Vz.x;
			sk_chunk[i].Mobb[9] = Vz.y;
			sk_chunk[i].Mobb[10] = Vz.z;
			sk_chunk[i].Mobb[11] = 0;

			Vy = Normalize(Cross(Vz, Vx));
			sk_chunk[i].Mobb[4] = Vy.x;
			sk_chunk[i].Mobb[5] = Vy.y;
			sk_chunk[i].Mobb[6] = Vy.z;
			sk_chunk[i].Mobb[7] = 0;
		}
		//double test=Dot(Vx,Vy);	
		m3dInvertMatrix44(sk_chunk[i].InvertofM, sk_chunk[i].Mobb);

		//重新构造OBB包围盒，仅需将包围盒的8个顶点和6个面的法向量进行旋转即可，无需重新计算圆柱映射表
		for (int j = 0; j<8; j++){
			objectVertex[0] = sk_chunk[i].AnglePoint[j].x;
			objectVertex[1] = sk_chunk[i].AnglePoint[j].y;
			objectVertex[2] = sk_chunk[i].AnglePoint[j].z;
			m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
			m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
			m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
			sk_chunk[i].AnglePoint[j].x = transformedVertex2[0];
			sk_chunk[i].AnglePoint[j].y = transformedVertex2[1];
			sk_chunk[i].AnglePoint[j].z = transformedVertex2[2];
		}
		for (int j = 0; j<6; j++){
			int p1;
			p1 = sk_chunk[i].Obbface[j].vert[0];
			objectVertex[0] = sk_chunk[i].Obbface[j].Normalize.x;
			objectVertex[1] = sk_chunk[i].Obbface[j].Normalize.y;
			objectVertex[2] = sk_chunk[i].Obbface[j].Normalize.z;
			m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
			m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
			m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
			sk_chunk[i].Obbface[j].Normalize.x = transformedVertex2[0];
			sk_chunk[i].Obbface[j].Normalize.y = transformedVertex2[1];
			sk_chunk[i].Obbface[j].Normalize.z = transformedVertex2[2];
			sk_chunk[i].Obbface[j].d = Dot(sk_chunk[i].AnglePoint[p1], sk_chunk[i].Obbface[j].Normalize)*(-1);
			if (j == 1)sk_chunk[i].vseN = sk_chunk[i].Obbface[j].Normalize;
			if (j == 2)sk_chunk[i].vyzN = sk_chunk[i].Obbface[j].Normalize;
		}
		if (i == 3 || i == 5 || i == 7 || i == 9){
			int indexk;
			if (i != 9)indexk = i + 1;
			else indexk = 0;
			//身体部位indexk旋转一定角度angle
			for (int j = 0; j<sk_chunk[indexk].numofVert; j++){
				objectVertex[0] = sk_chunk[indexk].vertex[j].Coordinates.x;
				objectVertex[1] = sk_chunk[indexk].vertex[j].Coordinates.y;
				objectVertex[2] = sk_chunk[indexk].vertex[j].Coordinates.z;
				m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
				m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
				m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
				sk_chunk[indexk].vertex[j].Coordinates.x = transformedVertex2[0];
				sk_chunk[indexk].vertex[j].Coordinates.y = transformedVertex2[1];
				sk_chunk[indexk].vertex[j].Coordinates.z = transformedVertex2[2];
			}
			//重新计算每个三角面片的法向量等
			for (int j = 0; j<sk_chunk[indexk].numofTri; j++){
				int a, b, c;
				a = sk_chunk[indexk].triangle[j].vert[0];
				b = sk_chunk[indexk].triangle[j].vert[1];
				c = sk_chunk[indexk].triangle[j].vert[2];
				h.TriNormalize_d(sk_chunk, indexk, a, b, c, j);
			}
			//重新计算凸包体
			for (int j = 0; j<sk_chunk[indexk].numofVert; j++){
				objectVertex[0] = sk_chunk[indexk].vertex[j].Convexhull.x;
				objectVertex[1] = sk_chunk[indexk].vertex[j].Convexhull.y;
				objectVertex[2] = sk_chunk[indexk].vertex[j].Convexhull.z;
				m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
				m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
				m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
				sk_chunk[indexk].vertex[j].Convexhull.x = transformedVertex2[0];
				sk_chunk[indexk].vertex[j].Convexhull.y = transformedVertex2[1];
				sk_chunk[indexk].vertex[j].Convexhull.z = transformedVertex2[2];
			}
			//重新计算凸包体的法向量等
			h.HullTriNormalize_d(sk_chunk, indexk);
			//重新计算局部坐标系
			//计算局部坐标系的中心点
			jk = w[indexk] / 2;
			index1 = jk*(q[indexk] + 1);
			index2 = jk*(q[indexk] + 1) + q[indexk];

			sk_chunk[indexk].Locenter = (sk_chunk[indexk].vertex[index1].Coordinates + sk_chunk[indexk].vertex[index2].Coordinates) / 2.0;
			//if(i==2)sk_chunk[indexk].Locenter.z+=0.5;

			sk_chunk[indexk].Mobb[12] = sk_chunk[indexk].Locenter.x;
			sk_chunk[indexk].Mobb[13] = sk_chunk[indexk].Locenter.y;
			sk_chunk[indexk].Mobb[14] = sk_chunk[indexk].Locenter.z;
			sk_chunk[indexk].Mobb[15] = 1;
			//构造Mobb和其逆矩阵InvertofM

			Vy = Normalize(sk_chunk[indexk].vertex[index1].Coordinates - sk_chunk[indexk].vertex[index2].Coordinates);
			sk_chunk[indexk].Mobb[4] = Vy.x;
			sk_chunk[indexk].Mobb[5] = Vy.y;
			sk_chunk[indexk].Mobb[6] = Vy.z;
			sk_chunk[indexk].Mobb[7] = 0;

			index1 = w[indexk] * (q[indexk] + 1);
			tempx = Normalize(sk_chunk[indexk].vertex[0].Coordinates - sk_chunk[indexk].vertex[index1].Coordinates);
			Vz = Normalize(Cross(tempx, Vy));
			sk_chunk[indexk].Mobb[8] = Vz.x;
			sk_chunk[indexk].Mobb[9] = Vz.y;
			sk_chunk[indexk].Mobb[10] = Vz.z;
			sk_chunk[indexk].Mobb[11] = 0;

			Vx = Normalize(Cross(Vy, Vz));
			sk_chunk[indexk].Mobb[0] = Vx.x;
			sk_chunk[indexk].Mobb[1] = Vx.y;
			sk_chunk[indexk].Mobb[2] = Vx.z;
			sk_chunk[indexk].Mobb[3] = 0;
			//double test=Dot(Vx,Vy);	
			m3dInvertMatrix44(sk_chunk[indexk].InvertofM, sk_chunk[indexk].Mobb);

			//重新构造OBB包围盒，仅需将包围盒的8个顶点和6个面的法向量进行旋转即可，无需重新计算圆柱映射表
			for (int j = 0; j<8; j++){
				objectVertex[0] = sk_chunk[indexk].AnglePoint[j].x;
				objectVertex[1] = sk_chunk[indexk].AnglePoint[j].y;
				objectVertex[2] = sk_chunk[indexk].AnglePoint[j].z;
				m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
				m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
				m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
				sk_chunk[indexk].AnglePoint[j].x = transformedVertex2[0];
				sk_chunk[indexk].AnglePoint[j].y = transformedVertex2[1];
				sk_chunk[indexk].AnglePoint[j].z = transformedVertex2[2];
			}
			for (int j = 0; j<6; j++){
				int p1;
				p1 = sk_chunk[indexk].Obbface[j].vert[0];
				objectVertex[0] = sk_chunk[indexk].Obbface[j].Normalize.x;
				objectVertex[1] = sk_chunk[indexk].Obbface[j].Normalize.y;
				objectVertex[2] = sk_chunk[indexk].Obbface[j].Normalize.z;
				m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
				m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
				m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
				sk_chunk[indexk].Obbface[j].Normalize.x = transformedVertex2[0];
				sk_chunk[indexk].Obbface[j].Normalize.y = transformedVertex2[1];
				sk_chunk[indexk].Obbface[j].Normalize.z = transformedVertex2[2];
				sk_chunk[indexk].Obbface[j].d = Dot(sk_chunk[indexk].AnglePoint[p1], sk_chunk[indexk].Obbface[j].Normalize)*(-1);
				if (j == 1)sk_chunk[indexk].vseN = sk_chunk[indexk].Obbface[j].Normalize;
				if (j == 2)sk_chunk[indexk].vyzN = sk_chunk[indexk].Obbface[j].Normalize;
			}
		}
	}
	else{
		m3dRotationMatrix44(transformationMatrix, angle, x, y, z);
		m3dRotationMatrix44(transformationMatrix1, 0, 0, 0, 1);
		transformationMatrix1[12] = sk_chunk[i].vertex[q[i] * (w[i] + 1)].Convexhull.x;
		transformationMatrix1[13] = sk_chunk[i].vertex[q[i] * (w[i] + 1)].Convexhull.y;
		transformationMatrix1[14] = sk_chunk[i].vertex[q[i] * (w[i] + 1)].Convexhull.z;
		m3dRotationMatrix44(transformationMatrix2, 0, 0, 0, 1);
		transformationMatrix2[12] = -sk_chunk[i].vertex[q[i] * (w[i] + 1)].Convexhull.x;
		transformationMatrix2[13] = -sk_chunk[i].vertex[q[i] * (w[i] + 1)].Convexhull.y;
		transformationMatrix2[14] = -sk_chunk[i].vertex[q[i] * (w[i] + 1)].Convexhull.z;
		//身体部位2旋转带动其他部位的旋转	
		int coindex[6] = { 0, 1, 2, 7, 8, 9 };
		for (int l = 0; l<6; l++){
			//身体部位coindex[l]旋转一定的角度
			for (int j = 0; j<sk_chunk[coindex[l]].numofVert; j++){
				objectVertex[0] = sk_chunk[coindex[l]].vertex[j].Coordinates.x;
				objectVertex[1] = sk_chunk[coindex[l]].vertex[j].Coordinates.y;
				objectVertex[2] = sk_chunk[coindex[l]].vertex[j].Coordinates.z;
				m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
				m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
				m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
				sk_chunk[coindex[l]].vertex[j].Coordinates.x = transformedVertex2[0];
				sk_chunk[coindex[l]].vertex[j].Coordinates.y = transformedVertex2[1];
				sk_chunk[coindex[l]].vertex[j].Coordinates.z = transformedVertex2[2];
			}
			//重新计算每个三角面片的法向量等
			for (int j = 0; j<sk_chunk[coindex[l]].numofTri; j++){
				int a, b, c;
				a = sk_chunk[coindex[l]].triangle[j].vert[0];
				b = sk_chunk[coindex[l]].triangle[j].vert[1];
				c = sk_chunk[coindex[l]].triangle[j].vert[2];
				h.TriNormalize_d(sk_chunk, coindex[l], a, b, c, j);
			}
			//重新计算凸包体
			for (int j = 0; j<sk_chunk[coindex[l]].numofVert; j++){
				objectVertex[0] = sk_chunk[coindex[l]].vertex[j].Convexhull.x;
				objectVertex[1] = sk_chunk[coindex[l]].vertex[j].Convexhull.y;
				objectVertex[2] = sk_chunk[coindex[l]].vertex[j].Convexhull.z;
				m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
				m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
				m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
				sk_chunk[coindex[l]].vertex[j].Convexhull.x = transformedVertex2[0];
				sk_chunk[coindex[l]].vertex[j].Convexhull.y = transformedVertex2[1];
				sk_chunk[coindex[l]].vertex[j].Convexhull.z = transformedVertex2[2];
			}
			//重新计算凸包体的法向量等
			h.HullTriNormalize_d(sk_chunk, coindex[l]);
			//重新计算局部坐标系
			CVector3 Vx, tempx, Vy, Vz;
			int index1, index2, jk, k;
			//计算局部坐标系的中心点
			jk = w[coindex[l]] / 2;
			index1 = jk*(q[coindex[l]] + 1);
			index2 = jk*(q[coindex[l]] + 1) + q[coindex[l]];

			sk_chunk[coindex[l]].Locenter = (sk_chunk[coindex[l]].vertex[index1].Coordinates + sk_chunk[coindex[l]].vertex[index2].Coordinates) / 2.0;
			if (coindex[l] == 2)sk_chunk[coindex[l]].Locenter.z += 0.5;

			sk_chunk[coindex[l]].Mobb[12] = sk_chunk[coindex[l]].Locenter.x;
			sk_chunk[coindex[l]].Mobb[13] = sk_chunk[coindex[l]].Locenter.y;
			sk_chunk[coindex[l]].Mobb[14] = sk_chunk[coindex[l]].Locenter.z;
			sk_chunk[coindex[l]].Mobb[15] = 1;
			//构造Mobb和其逆矩阵InvertofM

			Vy = Normalize(sk_chunk[coindex[l]].vertex[index1].Coordinates - sk_chunk[coindex[l]].vertex[index2].Coordinates);
			sk_chunk[coindex[l]].Mobb[4] = Vy.x;
			sk_chunk[coindex[l]].Mobb[5] = Vy.y;
			sk_chunk[coindex[l]].Mobb[6] = Vy.z;
			sk_chunk[coindex[l]].Mobb[7] = 0;

			index1 = w[coindex[l]] * (q[coindex[l]] + 1);
			tempx = Normalize(sk_chunk[coindex[l]].vertex[0].Coordinates - sk_chunk[coindex[l]].vertex[index1].Coordinates);
			Vz = Normalize(Cross(tempx, Vy));
			sk_chunk[coindex[l]].Mobb[8] = Vz.x;
			sk_chunk[coindex[l]].Mobb[9] = Vz.y;
			sk_chunk[coindex[l]].Mobb[10] = Vz.z;
			sk_chunk[coindex[l]].Mobb[11] = 0;

			Vx = Normalize(Cross(Vy, Vz));
			sk_chunk[coindex[l]].Mobb[0] = Vx.x;
			sk_chunk[coindex[l]].Mobb[1] = Vx.y;
			sk_chunk[coindex[l]].Mobb[2] = Vx.z;
			sk_chunk[coindex[l]].Mobb[3] = 0;
			//double test=Dot(Vx,Vy);	
			m3dInvertMatrix44(sk_chunk[coindex[l]].InvertofM, sk_chunk[coindex[l]].Mobb);

			//重新构造OBB包围盒，仅需将包围盒的8个顶点和6个面的法向量进行旋转即可，无需重新计算圆柱映射表
			for (int j = 0; j<8; j++){
				objectVertex[0] = sk_chunk[coindex[l]].AnglePoint[j].x;
				objectVertex[1] = sk_chunk[coindex[l]].AnglePoint[j].y;
				objectVertex[2] = sk_chunk[coindex[l]].AnglePoint[j].z;
				m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
				m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
				m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
				sk_chunk[coindex[l]].AnglePoint[j].x = transformedVertex2[0];
				sk_chunk[coindex[l]].AnglePoint[j].y = transformedVertex2[1];
				sk_chunk[coindex[l]].AnglePoint[j].z = transformedVertex2[2];
			}
			for (int j = 0; j<6; j++){
				int p1;
				p1 = sk_chunk[coindex[l]].Obbface[j].vert[0];
				objectVertex[0] = sk_chunk[coindex[l]].Obbface[j].Normalize.x;
				objectVertex[1] = sk_chunk[coindex[l]].Obbface[j].Normalize.y;
				objectVertex[2] = sk_chunk[coindex[l]].Obbface[j].Normalize.z;
				m3dTransformVector3(transformedVertex, objectVertex, transformationMatrix2);
				m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrix);
				m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrix1);
				sk_chunk[coindex[l]].Obbface[j].Normalize.x = transformedVertex2[0];
				sk_chunk[coindex[l]].Obbface[j].Normalize.y = transformedVertex2[1];
				sk_chunk[coindex[l]].Obbface[j].Normalize.z = transformedVertex2[2];
				sk_chunk[coindex[l]].Obbface[j].d = Dot(sk_chunk[i].AnglePoint[p1], sk_chunk[i].Obbface[j].Normalize)*(-1);
				if (j == 1)sk_chunk[coindex[l]].vseN = sk_chunk[coindex[l]].Obbface[j].Normalize;
				if (j == 2)sk_chunk[coindex[l]].vyzN = sk_chunk[coindex[l]].Obbface[j].Normalize;
			}
		}
		//delete [] coindex;
	}
	ClothAnimation(thisphysic, m_3DModel, sk_chunk, i, angle, x, y, z, transformationMatrix, transformationMatrix1, transformationMatrix2);
}


void CMSModel::CollisionCheck(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk){
	int i,j;
	int vert,parti;
	bool isinternal;
	CVector3 nNormal,vector,v,tempPC;
	Particle*particle;
	int temp=21;
	for(i=0;i<thisphysic->objectnumbers;i++){
		for(j=0;j<thisphysic->physics[i].particlenumbers;j++){
			particle=&(thisphysic->physics[i].pParticles[j]);
			if(particle->bLocked==0)
			{
				if(particle->isbrief)
				{//首先是简化的衣服模型的碰撞检测
					parti=particle->belongtoPart;
					isinternal=true;
					for(int k=0;k<6;k++)
					{//先与OBB包围盒进行碰撞检测
						vert=sk_chunk[parti].Obbface[k].vert[0];
						vector=particle->vPosition-sk_chunk[parti].AnglePoint[vert];
						nNormal=sk_chunk[parti].Obbface[k].Normalize;
						if(Dot(vector,nNormal)>0){
							isinternal=false;
							break;
						}
					}
					if(isinternal)
					{
						particle->iscollision=true;//在OBB内，记为碰撞
					}

				}
			}
		}
	}

	int indexz,indexQ,index;
	bool needamplify[10];
	for(i=0;i<10;i++)needamplify[i]=false;
	for(i=0;i<thisphysic->objectnumbers;i++){
		for(j=0;j<thisphysic->physics[i].particlenumbers;j++){
			particle=&(thisphysic->physics[i].pParticles[j]);
			if(particle->bLocked==0)
			{
				if(particle->isbrief&&particle->iscollision)
				{//记为碰撞的点进一步与人体网格进行碰撞
					parti=particle->belongtoPart;
					particle->plocal_xyz.x=sk_chunk[parti].InvertofM[0]*particle->vPosition.x+sk_chunk[parti].InvertofM[4]*particle->vPosition.y+sk_chunk[parti].InvertofM[8]*particle->vPosition.z+sk_chunk[parti].InvertofM[12];
					particle->plocal_xyz.y=sk_chunk[parti].InvertofM[1]*particle->vPosition.x+sk_chunk[parti].InvertofM[5]*particle->vPosition.y+sk_chunk[parti].InvertofM[9]*particle->vPosition.z+sk_chunk[parti].InvertofM[13];
					particle->plocal_xyz.z=sk_chunk[parti].InvertofM[2]*particle->vPosition.x+sk_chunk[parti].InvertofM[6]*particle->vPosition.y+sk_chunk[parti].InvertofM[10]*particle->vPosition.z+sk_chunk[parti].InvertofM[14];
					particle->plocal_rQz.x=sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y+particle->plocal_xyz.z*particle->plocal_xyz.z);
					double z,y;
					z=particle->plocal_xyz.z;
					y=particle->plocal_xyz.y;
					if(z>=0&&y>0)particle->plocal_rQz.y=atan(z/y);
					if(z>0&&y==0)particle->plocal_rQz.y=PI*0.5;
					if(z<0&&y==0)particle->plocal_rQz.y=PI*1.5;
					if(y<0)particle->plocal_rQz.y=PI+atan(z/y);
					if(z<0&&y>0)particle->plocal_rQz.y=2*PI+atan(z/y);
					particle->plocal_rQz.z=particle->plocal_xyz.x;

					indexz=(sk_chunk[parti].z_min_max[1]-particle->plocal_rQz.z)/sk_chunk[parti].dz;
					if(indexz==21)indexz=21-1;

					int bound;
					bound=21;
					indexQ=particle->plocal_rQz.y/sk_chunk[parti].dQ;
					if(indexQ==bound)indexQ=0;
					index=indexz*bound+indexQ;
					if(index<21*bound&&index>=0)
					{
						isinternal=true;
						for(int k=0;k<sk_chunk[parti].Cylindricalmap[index].numofTri;k++)
						{
							int a=sk_chunk[parti].Cylindricalmap[index].Triindex[k];
							vert=sk_chunk[parti].triangle[a].vert[0];
							vector=particle->vPosition-sk_chunk[parti].vertex[vert].Convexhull;
							nNormal=sk_chunk[parti].triangle[a].Normalizehull;
							if(Dot(vector,nNormal)>0)
							{
								isinternal=false;
								break;
							}
						}
						if(isinternal){
							needamplify[parti]=true;//若某个部位中，衣服上有质点与人体碰撞了，则需进一步检测非简化点
							//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
							CVector3 p;
							if(parti==1||parti==2){
								p.Set(0,particle->plocal_xyz.y,0);
								particle->vprojection.x=sk_chunk[parti].Mobb[12];
								particle->vprojection.y=p.y+sk_chunk[parti].Mobb[13];
								particle->vprojection.z=sk_chunk[parti].Mobb[14];
							}else{
								p.Set(particle->plocal_xyz.x,0,0);
								particle->vprojection.x=sk_chunk[parti].Mobb[0] *p.x+sk_chunk[parti].Mobb[12];
								particle->vprojection.y=sk_chunk[parti].Mobb[1] *p.x+sk_chunk[parti].Mobb[13];
								particle->vprojection.z=sk_chunk[parti].Mobb[2] *p.x+sk_chunk[parti].Mobb[14];
							}
							particle->Cylindex=index;
						}
						else particle->iscollision=false;
					}
					else particle->iscollision=false;
				}

			}
		}
	}
	bool needall=false;
	i=0;
	while(i<10){
		if(needamplify[i]){
			needall=true;
			break;
		}
		i++;
	}
	if(needall)
	{
		for(i=0;i<thisphysic->objectnumbers;i++){
			for(j=0;j<thisphysic->physics[i].particlenumbers;j++){
				particle=&(thisphysic->physics[i].pParticles[j]);
				if(particle->bLocked==0)
				{
					if(!particle->isbrief){//非简化点与OBB包围盒的碰撞检测
						parti=particle->belongtoPart;
						if(needamplify[parti]){
							isinternal=true;
							for(int k=0;k<6;k++){
								vert=sk_chunk[parti].Obbface[k].vert[0];
								vector=particle->vPosition-sk_chunk[parti].AnglePoint[vert];
								nNormal=sk_chunk[parti].Obbface[k].Normalize;
								if(Dot(vector,nNormal)>0){
									isinternal=false;
									break;
								}
							}
							if(isinternal)
							{
								particle->iscollision=true;

							}
						}
					}
				}
			}
		}
		for(i=0;i<thisphysic->objectnumbers;i++)
		{
			for(j=0;j<thisphysic->physics[i].particlenumbers;j++)
			{
				particle=&(thisphysic->physics[i].pParticles[j]);
				if(particle->bLocked==0)
				{
					//if(!particle->isbrief&&particle->iscollision)
					if(particle->iscollision)
					{//非简化点与人体的碰撞检测
						for(int k=0;k<10;k++)
						{
							if((particle->belongtoPart)==k&&(needamplify[k]))
							{
								float mindist=10000.0;
								float tempdist;
								int triindex;
								for(int m=0;m<sk_chunk[k].numofTri;m++)
								{
									CVector3 trian;
									trian=sk_chunk[k].vertex[sk_chunk[k].triangle[m].vert[0]].Convexhull;
									tempdist=Magnitude(particle->vPosition-trian);
									if(tempdist<mindist)
									{
										mindist=tempdist;
										triindex=m;
									}
								}
								vert=sk_chunk[k].triangle[triindex].vert[0];
								vector=particle->vPosition-sk_chunk[k].vertex[vert].Convexhull;
								nNormal=sk_chunk[k].triangle[triindex].Normalizehull;
								if(Dot(vector,nNormal)>0)
								{
									isinternal=false;
									break;
								}
								//if(isinternal)
								{
									particle->iscollision=true;			
									//if(isinside(tempPC,sk_chunk,k,triindex))
									{
										particle->vPosition=sk_chunk[k].vertex[sk_chunk[k].triangle[triindex].vert[0]].Convexhull;
										particle->vprePosition=particle->vPosition;
										particle->iscollision=false;
										particle->bLocked=1;
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

/*
void CMSModel::CollisionCheck(all_Physics *thisphysic,Human::Skeletonchunk *sk_chunk){
	int i,j;
	int vert,parti;
	bool isinternal;
	CVector3 nNormal,vector,v,tempPC;
	Particle*particle;
	int temp=21;
	int indexz,indexQ,index;
	for(i=0;i<thisphysic->objectnumbers;i++){
		for(j=0;j<thisphysic->physics[i].particlenumbers;j++){
			particle=&(thisphysic->physics[i].pParticles[j]);
			if(particle->bLocked==0)
			{
					parti=particle->belongtoPart;
					//局部坐标
					particle->plocal_xyz.x=sk_chunk[parti].InvertofM[0]*particle->vPosition.x+sk_chunk[parti].InvertofM[4]*particle->vPosition.y+sk_chunk[parti].InvertofM[8]*particle->vPosition.z+sk_chunk[parti].InvertofM[12];
					particle->plocal_xyz.y=sk_chunk[parti].InvertofM[1]*particle->vPosition.x+sk_chunk[parti].InvertofM[5]*particle->vPosition.y+sk_chunk[parti].InvertofM[9]*particle->vPosition.z+sk_chunk[parti].InvertofM[13];
					particle->plocal_xyz.z=sk_chunk[parti].InvertofM[2]*particle->vPosition.x+sk_chunk[parti].InvertofM[6]*particle->vPosition.y+sk_chunk[parti].InvertofM[10]*particle->vPosition.z+sk_chunk[parti].InvertofM[14];
					//转化为柱坐标系
					particle->plocal_rQz.x=sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y+particle->plocal_xyz.z*particle->plocal_xyz.z);
					double z,y;
					z=particle->plocal_xyz.z;
					y=particle->plocal_xyz.y;
					if(z>=0&&y>0)particle->plocal_rQz.y=atan(z/y);
					if(z>0&&y==0)particle->plocal_rQz.y=PI*0.5;
					if(z<0&&y==0)particle->plocal_rQz.y=PI*1.5;
					if(y<0)particle->plocal_rQz.y=PI+atan(z/y);
					if(z<0&&y>0)particle->plocal_rQz.y=2*PI+atan(z/y);
					particle->plocal_rQz.z=particle->plocal_xyz.x;

					indexz=(sk_chunk[parti].z_min_max[1]-particle->plocal_rQz.z)/sk_chunk[parti].dz;
					if(indexz==21)indexz=21-1;

					int bound;
					if(parti==1)bound=21*2;
					else bound=21;

					indexQ=particle->plocal_rQz.y/sk_chunk[parti].dQ;
					if(indexQ==bound)indexQ=0;
					index=indexz*bound+indexQ;
					if(index<21*bound&&index>=0)
					{
						isinternal=true;
						for(int k=0;k<sk_chunk[parti].Cylindricalmap[index].numofTri;k++)
						{
							int a=sk_chunk[parti].Cylindricalmap[index].Triindex[k];
							vert=sk_chunk[parti].triangle[a].vert[0];
							vector=particle->vPosition-sk_chunk[parti].vertex[vert].Convexhull;
							nNormal=sk_chunk[parti].triangle[a].Normalizehull;
							if(Dot(vector,nNormal)>0){
								isinternal=false;
								break;
							}
							if(isinternal)
							{
								particle->iscollision=true;
								CVector3 p;
							    p.Set(0,particle->plocal_xyz.y,0);
								particle->vprojection.x=sk_chunk[parti].Mobb[12];
								particle->vprojection.y=p.y+sk_chunk[k].Mobb[13];
								particle->vprojection.z=sk_chunk[parti].Mobb[14];
								particle->Cylindex=index;
								nNormal=sk_chunk[parti].triangle[a].Normalizehull;
								double t=(particle->vPosition.x*nNormal.x+particle->vPosition.y*nNormal.y+particle->vPosition.z*nNormal.z+sk_chunk[parti].triangle[a].dhull)/(vector.x*nNormal.x+vector.y*nNormal.y+vector.z*nNormal.z);
								tempPC.x=particle->vPosition.x-t*vector.x;
								tempPC.y=particle->vPosition.y-t*vector.y;
								tempPC.z=particle->vPosition.z-t*vector.z;
								if(isinside(tempPC,sk_chunk,k,a)){
									particle->vPosition=tempPC+nNormal*0.05;//线衫1： *0.3
									particle->vprePosition=particle->vPosition;
									particle->iscollision=false;
									particle->bLocked=1;
									break;
								}
							}
							else
								particle->iscollision=false;
						}
					}
					else particle->iscollision=false;
			}
		}
	}
}
*/
void CMSModel::StepSimulation(all_Physics *thisphysic,CLoad3DS::t3DModel m_3DModel,Human::Skeletonchunk *sk_chunk,PiMotion*pi_m,double dt){                  //计算步长dt之后的模型

	CalcForces(thisphysic,m_3DModel,sk_chunk,pi_m);//,sk_chunk,parti,angle,x,y,z
	//修改的地方：不使用积分
    //Verlet(thisphysic,sk_chunk,dt);//Verlet 积分
}

void CMSModel::PreExam(all_Physics *thisphysic, Human::Skeletonchunk *sk_chunk, int key){
	int i, j, vert;
	int indexz, indexQ, index;
	//double mint;
	bool isinternal;
	CVector3 vector, nNormal, tempPC, v;//,Pcollision
	Particle*particle;
	M3DMatrix44f   transformationMatrixleft;     //new
	M3DMatrix44f   transformationMatrixleftl;     //new  左小臂
	M3DMatrix44f   transformationMatrixleft1;     //new
	M3DMatrix44f   transformationMatrixleft2;     //new

	M3DMatrix44f   transformationMatrixright;     //new
	M3DMatrix44f   transformationMatrixrightl;     //new  右小臂
	M3DMatrix44f   transformationMatrixright1;     //new
	M3DMatrix44f   transformationMatrixright2;     //new

	M3DMatrix44f   transformationMatrixcentorup;     //new
	M3DMatrix44f   transformationMatrixcentorup1;     //new
	M3DMatrix44f   transformationMatrixcentorup2;     //new
	M3DMatrix44f   transformationMatrixcentordown;     //new
	M3DMatrix44f   transformationMatrixcentordown1;     //new
	M3DMatrix44f   transformationMatrixcentordown2;     //new

	M3DVector3f objectVertex;				//new
	M3DVector3f transformedVertex;			//new
	M3DVector3f transformedVertex1;			//new
	M3DVector3f transformedVertex2;			//new
	M3DVector3f transformedVertext;			//new  左右小臂的旋转的临时存储
	if (key == 0){
		m3dRotationMatrix44(transformationMatrixleft, 0.38, 0.5, 0, 1);        //new
		m3dRotationMatrix44(transformationMatrixleft1, 0, 0, 0, 1);        //new
		transformationMatrixleft1[12] = sk_chunk[2].vertex[q[2]].Convexhull.x - 0.4;//-0.6  男士线衫-0.4										//new
		transformationMatrixleft1[13] = sk_chunk[2].vertex[q[2]].Convexhull.y;										//new	
		transformationMatrixleft1[14] = sk_chunk[2].vertex[q[2]].Convexhull.z;										//new
		m3dRotationMatrix44(transformationMatrixleft2, 0, 0, 0, 1);        //new
		transformationMatrixleft2[12] = -sk_chunk[2].vertex[q[2]].Convexhull.x + 0.4;//+0.6	 男士线衫+0.4									//new
		transformationMatrixleft2[13] = -sk_chunk[2].vertex[q[2]].Convexhull.y;										//new	
		transformationMatrixleft2[14] = -sk_chunk[2].vertex[q[2]].Convexhull.z;

		m3dRotationMatrix44(transformationMatrixright, -0.40, -0.5, 0, 1);        //new
		m3dRotationMatrix44(transformationMatrixrightl, 0.0, 0, 1, 0);        //new
		m3dRotationMatrix44(transformationMatrixright1, 0, 0, 0, 1);        //new
		transformationMatrixright1[12] = sk_chunk[2].vertex[0].Convexhull.x + 0.4;//+0.6	男士线衫+0.4									//new
		transformationMatrixright1[13] = sk_chunk[2].vertex[0].Convexhull.y;										//new	
		transformationMatrixright1[14] = sk_chunk[2].vertex[0].Convexhull.z;										//new
		m3dRotationMatrix44(transformationMatrixright2, 0, 0, 0, 1);        //new
		transformationMatrixright2[12] = -sk_chunk[2].vertex[0].Convexhull.x - 0.4;//-0.6	 男士线衫-0.4							//new
		transformationMatrixright2[13] = -sk_chunk[2].vertex[0].Convexhull.y;										//new	
		transformationMatrixright2[14] = -sk_chunk[2].vertex[0].Convexhull.z;
		double minz = 10000;
		int minzindex;
		for (i = 0; i<thisphysic->objectnumbers; i++){
			for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
				particle = &(thisphysic->physics[i].pParticles[j]);
				if (particle->vPosition.y>sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.y){
					if (particle->vPosition.x<sk_chunk[2].vertex[q[2]].Convexhull.x - 0.4){//-0.6  男士线衫-0.4
						objectVertex[0] = particle->vPosition.x;
						objectVertex[1] = particle->vPosition.y;
						objectVertex[2] = particle->vPosition.z;
						m3dTransformVector3(transformedVertex, objectVertex, transformationMatrixleft2);
						m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrixleft);
						m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrixleft1);
						particle->vPosition.x = transformedVertex2[0];
						particle->vPosition.y = transformedVertex2[1];
						particle->vPosition.z = transformedVertex2[2];
						//以下注释掉的代码为男士线衫1与男士人体模型2对齐时所需要的代码
						/*	v=Vector(particle->vPosition,sk_chunk[9].vertex[q[0]/2].Convexhull);
						if(Dot(v,sk_chunk[0].v)<=0){
						M3DMatrix44f lefttemp1,lefttemp2;
						M3DVector3f vtemp,vtemp1,vtemp2,vtemp3;
						m3dRotationMatrix44(transformationMatrixleftl, 1.15,-13,13,35);        //new
						m3dRotationMatrix44(lefttemp1, 0,0,0,1);
						lefttemp1[12] = sk_chunk[0].vertex[q[0]/2].Convexhull.x;// 男士线衫										//new
						lefttemp1[13] = sk_chunk[0].vertex[q[0]/2].Convexhull.y;										//new
						lefttemp1[14] = sk_chunk[0].vertex[q[0]/2].Convexhull.z;
						m3dRotationMatrix44(lefttemp2, 0,0,0,1);
						lefttemp2[12] = -sk_chunk[0].vertex[q[0]/2].Convexhull.x;// 男士线衫										//new
						lefttemp2[13] = -sk_chunk[0].vertex[q[0]/2].Convexhull.y;										//new
						lefttemp2[14] = -sk_chunk[0].vertex[q[0]/2].Convexhull.z;

						vtemp[0]=particle->vPosition.x;
						vtemp[1]=particle->vPosition.y;
						vtemp[2]=particle->vPosition.z;
						m3dTransformVector3(vtemp1,vtemp ,lefttemp2);
						m3dTransformVector3(vtemp2, vtemp1, transformationMatrixleftl);
						m3dTransformVector3( vtemp3, vtemp2, lefttemp1);
						particle->vPosition.x=vtemp3[0];
						particle->vPosition.y=vtemp3[1];
						particle->vPosition.z=vtemp3[2];
						}	*/
						particle->vprePosition = particle->vPosition;
					}
					else{
						if (particle->vPosition.x>sk_chunk[2].vertex[0].Convexhull.x + 0.4){// 男士线衫+0.4
							objectVertex[0] = particle->vPosition.x;																				//new
							objectVertex[1] = particle->vPosition.y;
							objectVertex[2] = particle->vPosition.z;
							m3dTransformVector3(transformedVertex, objectVertex, transformationMatrixright2);
							m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrixright);
							m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrixright1);
							particle->vPosition.x = transformedVertex2[0];
							particle->vPosition.y = transformedVertex2[1];
							particle->vPosition.z = transformedVertex2[2];
							//以下注释掉的代码为男士线衫1与男士人体模型2对齐时所需要的代码
							/*v=Vector(particle->vPosition,sk_chunk[8].vertex[q[8]/2].Convexhull);
							if(Dot(v,sk_chunk[7].v)<=0){
							M3DMatrix44f lefttemp1,lefttemp2;
							M3DVector3f vtemp,vtemp1,vtemp2,vtemp3;
							m3dRotationMatrix44(transformationMatrixrightl, 1.15,0,0,1);        //new
							m3dRotationMatrix44(lefttemp1, 0,0,0,1);
							lefttemp1[12] = sk_chunk[8].vertex[q[8]/2].Convexhull.x;// 男士线衫										//new
							lefttemp1[13] = sk_chunk[8].vertex[q[8]/2].Convexhull.y;										//new
							lefttemp1[14] = sk_chunk[8].vertex[q[8]/2].Convexhull.z;
							m3dRotationMatrix44(lefttemp2, 0,0,0,1);
							lefttemp2[12] = -sk_chunk[8].vertex[q[8]/2].Convexhull.x;// 男士线衫										//new
							lefttemp2[13] = -sk_chunk[8].vertex[q[8]/2].Convexhull.y;										//new
							lefttemp2[14] = -sk_chunk[8].vertex[q[8]/2].Convexhull.z;

							vtemp[0]=particle->vPosition.x;
							vtemp[1]=particle->vPosition.y;
							vtemp[2]=particle->vPosition.z;
							m3dTransformVector3(vtemp1,vtemp ,lefttemp2);
							m3dTransformVector3(vtemp2, vtemp1, transformationMatrixrightl);
							m3dTransformVector3( vtemp3, vtemp2, lefttemp1);
							particle->vPosition.x=vtemp3[0];
							particle->vPosition.y=vtemp3[1];
							particle->vPosition.z=vtemp3[2];
							}	*/
							particle->vprePosition = particle->vPosition;
						}
						else{
							if (particle->vPosition.z<sk_chunk[2].vertex[0].Convexhull.z)//女式衣服往后旋转，男士衣服往前旋转
								if (particle->vPosition.z<minz){
								minz = particle->vPosition.z;
								minzindex = j;
								}
						}
					}
				}
			}
			//*------------------------
			double dminxy = 10000, tempxy;
			int sk_dmindex;
			for (int k = (q[2] + 1)*(w[2] + 1); k<(q[2]+1)*(w[2]+1); k++){//男士---(q[2]+1)*(w[2]+1)~sk_chunk[2].numofVert;女士---0~(q[2]+1)*(w[2]+1)
				tempxy = fabs(sk_chunk[2].vertex[k].Convexhull.x - thisphysic->physics[i].pParticles[minzindex].vPosition.x) + fabs(sk_chunk[2].vertex[k].Convexhull.y - thisphysic->physics[i].pParticles[minzindex].vPosition.y);
				if (tempxy<dminxy){
					dminxy = tempxy;
					sk_dmindex = k;
				}
			}
			if (minz<sk_chunk[2].vertex[sk_dmindex].Convexhull.z){
				CVector3 v1, v2;
				int oindex;
				double cos, degree;
				oindex = (q[2] + 1)*(w[2] + 1) + (sk_dmindex - (q[2] + 1)*(w[2] + 1)) % (e[2] - 1);//男士-----(q[2]+1)*(w[2]+1)+
				v1 = sk_chunk[2].vertex[oindex].Convexhull - thisphysic->physics[i].pParticles[minzindex].vPosition;
				v2 = sk_chunk[2].vertex[oindex].Convexhull - sk_chunk[2].vertex[sk_dmindex].Convexhull;
				cos = Dot(v1, v2) / (Magnitude(v1)*Magnitude(v2));
				degree = acos(cos);

				m3dRotationMatrix44(transformationMatrixcentordown, -degree + 0.15, 1, 0, 0);
				m3dRotationMatrix44(transformationMatrixcentordown1, 0, 1, 0, 0);
				transformationMatrixcentordown1[12] = 0;//										
				transformationMatrixcentordown1[13] = sk_chunk[2].vertex[0].Convexhull.y;
				transformationMatrixcentordown1[14] = 0;
				m3dRotationMatrix44(transformationMatrixcentordown2, 0, 1, 0, 0);
				transformationMatrixcentordown2[12] = 0;//								
				transformationMatrixcentordown2[13] = -sk_chunk[2].vertex[0].Convexhull.y;
				transformationMatrixcentordown2[14] = 0;

				m3dRotationMatrix44(transformationMatrixcentorup, -0.27, 1, 0, 0);
				m3dRotationMatrix44(transformationMatrixcentorup1, 0, 1, 0, 0);
				transformationMatrixcentorup1[12] = 0;//										
				transformationMatrixcentorup1[13] = sk_chunk[2].vertex[0].Convexhull.y;
				transformationMatrixcentorup1[14] = 0;
				m3dRotationMatrix44(transformationMatrixcentorup2, 0, 1, 0, 0);
				transformationMatrixcentorup2[12] = 0;//								
				transformationMatrixcentorup2[13] = -sk_chunk[2].vertex[0].Convexhull.y;
				transformationMatrixcentorup2[14] = 0;
				for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
					particle = &(thisphysic->physics[i].pParticles[j]);//1.2
					if (particle->vPosition.y <= sk_chunk[2].vertex[q[2]].Convexhull.y){
						if (particle->vPosition.x >= sk_chunk[2].vertex[q[2]].Convexhull.x - 0.6&&particle->vPosition.x <= sk_chunk[2].vertex[0].Convexhull.x + 0.6){//
							objectVertex[0] = particle->vPosition.x;
							objectVertex[1] = particle->vPosition.y;
							objectVertex[2] = particle->vPosition.z;
							m3dTransformVector3(transformedVertex, objectVertex, transformationMatrixcentordown2);
							m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrixcentordown);
							m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrixcentordown1);
							particle->vPosition.x = transformedVertex2[0];
							particle->vPosition.y = transformedVertex2[1];
							particle->vPosition.z = transformedVertex2[2];
							particle->vprePosition = particle->vPosition;
						}
					}/*else{
					 if(particle->vPosition.x>=sk_chunk[2].vertex[q[2]].Convexhull.x-0.4&&particle->vPosition.x<=sk_chunk[2].vertex[0].Convexhull.x+0.4){//
					 objectVertex[0]=particle->vPosition.x;
					 objectVertex[1]=particle->vPosition.y;
					 objectVertex[2]=particle->vPosition.z;
					 m3dTransformVector3(transformedVertex, objectVertex, transformationMatrixcentorup2);
					 m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrixcentorup);
					 m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrixcentorup1);
					 particle->vPosition.x=transformedVertex2[0];
					 particle->vPosition.y=transformedVertex2[1];
					 particle->vPosition.z=transformedVertex2[2];
					 particle->vprePosition=particle->vPosition;
					 }
					 }*/
				}
			}//--------------*/
		}
	}
	if (key == 1){
		//m3dRotationMatrix44(transformationMatrixleft, 0.44,-3.1,0,-4.0);//z 8.1        //new
		m3dRotationMatrix44(transformationMatrixleft, -0.2, 1, 0, 0);
		m3dRotationMatrix44(transformationMatrixleft1, 0, 0, 0, 1);        //new
		transformationMatrixleft1[12] = sk_chunk[2].vertex[(w[2] + 1)*(q[2] + 1) - 1].Convexhull.x;//									//new
		transformationMatrixleft1[13] = sk_chunk[2].vertex[(w[2] + 1)*(q[2] + 1) - 1].Convexhull.y;										//new	
		transformationMatrixleft1[14] = sk_chunk[2].vertex[(w[2] + 1)*(q[2] + 1) - 1].Convexhull.z;										//new
		m3dRotationMatrix44(transformationMatrixleft2, 0, 0, 0, 1);        //new
		transformationMatrixleft2[12] = -sk_chunk[2].vertex[(w[2] + 1)*(q[2] + 1) - 1].Convexhull.x;//										//new
		transformationMatrixleft2[13] = -sk_chunk[2].vertex[(w[2] + 1)*(q[2] + 1) - 1].Convexhull.y;										//new	
		transformationMatrixleft2[14] = -sk_chunk[2].vertex[(w[2] + 1)*(q[2] + 1) - 1].Convexhull.z;

		//	m3dRotationMatrix44(transformationMatrixright, 0.38,-3.7,0,3.5);  //z 6.1      //new
		m3dRotationMatrix44(transformationMatrixright, -0.2, 1, 0, 0);
		m3dRotationMatrix44(transformationMatrixright1, 0, 0, 0, 1);        //new
		transformationMatrixright1[12] = sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.x;//									//new
		transformationMatrixright1[13] = sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.y;										//new	
		transformationMatrixright1[14] = sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.z;										//new
		m3dRotationMatrix44(transformationMatrixright2, 0, 0, 0, 1);        //new
		transformationMatrixright2[12] = -sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.x;//							//new
		transformationMatrixright2[13] = -sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.y;										//new	
		transformationMatrixright2[14] = -sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.z;
		for (i = 0; i<thisphysic->objectnumbers; i++){
			for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
				particle = &(thisphysic->physics[i].pParticles[j]);
				if (particle->vPosition.y<sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.y){
					if (particle->vPosition.x<(sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.x + sk_chunk[2].vertex[(w[2] + 1)*(q[2] + 1) - 1].Convexhull.x) / 2){//-0.6
						objectVertex[0] = particle->vPosition.x;
						objectVertex[1] = particle->vPosition.y;
						objectVertex[2] = particle->vPosition.z;
						m3dTransformVector3(transformedVertex, objectVertex, transformationMatrixleft2);
						m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrixleft);
						m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrixleft1);
						particle->vPosition.x = transformedVertex2[0];
						particle->vPosition.y = transformedVertex2[1];
						particle->vPosition.z = transformedVertex2[2];
						particle->vprePosition = particle->vPosition;
					}
					else{
						objectVertex[0] = particle->vPosition.x;																				//new
						objectVertex[1] = particle->vPosition.y;
						objectVertex[2] = particle->vPosition.z;
						m3dTransformVector3(transformedVertex, objectVertex, transformationMatrixright2);
						m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrixright);
						m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrixright1);
						particle->vPosition.x = transformedVertex2[0];
						particle->vPosition.y = transformedVertex2[1];
						particle->vPosition.z = transformedVertex2[2];
						particle->vprePosition = particle->vPosition;

					}
				}
			}
		}

		//m3dRotationMatrix44(transformationMatrixleft, 0.32,49.6,-16.43,8.95);   //z 44.8     //new
		m3dRotationMatrix44(transformationMatrixleft, 0.3, 1, 0, 0);
		m3dRotationMatrix44(transformationMatrixleft1, 0, 0, 0, 1);        //new
		transformationMatrixleft1[12] = sk_chunk[4].vertex[q[4]].Convexhull.x;//									//new
		transformationMatrixleft1[13] = sk_chunk[4].vertex[q[4]].Convexhull.y;										//new	
		transformationMatrixleft1[14] = sk_chunk[4].vertex[q[4]].Convexhull.z;										//new
		m3dRotationMatrix44(transformationMatrixleft2, 0, 0, 0, 1);        //new
		transformationMatrixleft2[12] = -sk_chunk[4].vertex[q[4]].Convexhull.x;//										//new
		transformationMatrixleft2[13] = -sk_chunk[4].vertex[q[4]].Convexhull.y;										//new	
		transformationMatrixleft2[14] = -sk_chunk[4].vertex[q[4]].Convexhull.z;

		//m3dRotationMatrix44(transformationMatrixright, 0.38,118.4,22.57,-97.6);        //new
		m3dRotationMatrix44(transformationMatrixright, 0.3, 1, 0, 0);
		m3dRotationMatrix44(transformationMatrixright1, 0, 0, 0, 1);        //new
		transformationMatrixright1[12] = sk_chunk[6].vertex[q[6]].Convexhull.x;//									//new
		transformationMatrixright1[13] = sk_chunk[6].vertex[q[6]].Convexhull.y;										//new	
		transformationMatrixright1[14] = sk_chunk[6].vertex[q[6]].Convexhull.z;										//new
		m3dRotationMatrix44(transformationMatrixright2, 0, 0, 0, 1);        //new
		transformationMatrixright2[12] = -sk_chunk[6].vertex[q[6]].Convexhull.x;//							//new
		transformationMatrixright2[13] = -sk_chunk[6].vertex[q[6]].Convexhull.y;										//new	
		transformationMatrixright2[14] = -sk_chunk[6].vertex[q[6]].Convexhull.z;
		for (i = 0; i<thisphysic->objectnumbers; i++){
			for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
				particle = &(thisphysic->physics[i].pParticles[j]);
				if (particle->vPosition.y<sk_chunk[4].vertex[0].Convexhull.y){
					if (particle->vPosition.x<(sk_chunk[2].vertex[w[2] * (q[2] + 1)].Convexhull.x + sk_chunk[2].vertex[(w[2] + 1)*(q[2] + 1) - 1].Convexhull.x) / 2){//-0.6
						objectVertex[0] = particle->vPosition.x;
						objectVertex[1] = particle->vPosition.y;
						objectVertex[2] = particle->vPosition.z;
						m3dTransformVector3(transformedVertex, objectVertex, transformationMatrixleft2);
						m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrixleft);
						m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrixleft1);
						particle->vPosition.x = transformedVertex2[0];
						particle->vPosition.y = transformedVertex2[1];
						particle->vPosition.z = transformedVertex2[2];
						particle->vprePosition = particle->vPosition;
					}
					else{
						objectVertex[0] = particle->vPosition.x;																				//new
						objectVertex[1] = particle->vPosition.y;
						objectVertex[2] = particle->vPosition.z;
						m3dTransformVector3(transformedVertex, objectVertex, transformationMatrixright2);
						m3dTransformVector3(transformedVertex1, transformedVertex, transformationMatrixright);
						m3dTransformVector3(transformedVertex2, transformedVertex1, transformationMatrixright1);
						particle->vPosition.x = transformedVertex2[0];
						particle->vPosition.y = transformedVertex2[1];
						particle->vPosition.z = transformedVertex2[2];
						particle->vprePosition = particle->vPosition;

					}
				}
			}
		}
	}
	//以上的代码为：为衣服做大致的对齐，即移位与旋转，若衣服的大小不合适，即嵌入人体中，则在以下的代码中将嵌入的部分衣服向外进行拉伸
	//*-------------
	for (i = 0; i<thisphysic->objectnumbers; i++){
		for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
			particle = &(thisphysic->physics[i].pParticles[j]);
			if (particle->bLocked == 0){
				if (particle->vPosition.y >= sk_chunk[2].vertex[w[2] * (q[2] + 1)].Coordinates.y){//检测上身碰撞情况
					if (particle->vPosition.x<sk_chunk[2].vertex[q[2]].Coordinates.x){//对左袖子的质点与左大臂进行详细检测
						v = Vector(particle->vPosition, sk_chunk[0].vertex[0].Convexhull);
						if (Dot(v, sk_chunk[0].v)>0){//将该质点的坐标转换为左大臂的局部坐标（包括xyz和rQz）
							particle->plocal_xyz.x = sk_chunk[9].InvertofM[0] * particle->vPosition.x + sk_chunk[9].InvertofM[4] * particle->vPosition.y + sk_chunk[9].InvertofM[8] * particle->vPosition.z + sk_chunk[9].InvertofM[12];
							particle->plocal_xyz.y = sk_chunk[9].InvertofM[1] * particle->vPosition.x + sk_chunk[9].InvertofM[5] * particle->vPosition.y + sk_chunk[9].InvertofM[9] * particle->vPosition.z + sk_chunk[9].InvertofM[13];
							particle->plocal_xyz.z = sk_chunk[9].InvertofM[2] * particle->vPosition.x + sk_chunk[9].InvertofM[6] * particle->vPosition.y + sk_chunk[9].InvertofM[10] * particle->vPosition.z + sk_chunk[9].InvertofM[14];
							particle->plocal_rQz.x = sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y + particle->plocal_xyz.z*particle->plocal_xyz.z);
							double z, y;
							z = particle->plocal_xyz.z;
							y = particle->plocal_xyz.y;
							if (z >= 0 && y>0)particle->plocal_rQz.y = atan(z / y);
							if (z>0 && y == 0)particle->plocal_rQz.y = PI*0.5;
							if (z<0 && y == 0)particle->plocal_rQz.y = PI*1.5;
							if (y<0)particle->plocal_rQz.y = PI + atan(z / y);
							if (z <= 0 && y>0)particle->plocal_rQz.y = 2 * PI + atan(z / y);
							particle->plocal_rQz.z = particle->plocal_xyz.x;

							indexz = (int)((sk_chunk[9].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[9].dz);
							if (indexz == w[9])indexz = w[9] - 1;
							indexQ = (int)(particle->plocal_rQz.y / sk_chunk[9].dQ);
							if (indexQ == w[9])indexQ = 0;
							index = indexz*w[9] + indexQ;
							if (index<(w[9] * w[9]) && index >= 0){
								isinternal = true;
								for (int k = 0; k<sk_chunk[9].Cylindricalmap[index].numofTri; k++){
									int a = sk_chunk[9].Cylindricalmap[index].Triindex[k];
									vert = sk_chunk[9].triangle[a].vert[0];
									vector = particle->vPosition - sk_chunk[9].vertex[vert].Convexhull;
									nNormal = sk_chunk[9].triangle[a].Normalizehull;
									if (Dot(vector, nNormal)>0){
										isinternal = false;//未碰撞
										break;
									}
								}
								if (isinternal){//碰撞
									//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
									CVector3 p;
									p.Set(particle->plocal_xyz.x, 0, 0);//p为particle的局部xyz坐标在圆柱中心轴上的投影点
									particle->vprojection.x = sk_chunk[9].Mobb[0] * p.x + sk_chunk[9].Mobb[12];
									particle->vprojection.y = sk_chunk[9].Mobb[1] * p.x + sk_chunk[9].Mobb[13];
									particle->vprojection.z = sk_chunk[9].Mobb[2] * p.x + sk_chunk[9].Mobb[14];

									vector = particle->vPosition - particle->vprojection;
									//CVector3 temptriNormal;
									//mint=10000;
									//计算射线vector与相应三角形的碰撞响应点
									for (int k = 0; k<sk_chunk[9].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[9].Cylindricalmap[index].Triindex[k];
										nNormal = sk_chunk[9].triangle[a].Normalizehull;
										double t = -(particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[9].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
										tempPC.x = particle->vPosition.x + t*vector.x;
										tempPC.y = particle->vPosition.y + t*vector.y;
										tempPC.z = particle->vPosition.z + t*vector.z;
										if (isinside(tempPC, sk_chunk, 9, a)){
											particle->vPosition = tempPC + nNormal*0.08;
											particle->vprePosition = particle->vPosition;
											if (particle->plocal_xyz.y>0)particle->bLocked = 1;
											break;
										}
									}
								}
							}
						}
						else{
							particle->plocal_xyz.x = sk_chunk[0].InvertofM[0] * particle->vPosition.x + sk_chunk[0].InvertofM[4] * particle->vPosition.y + sk_chunk[0].InvertofM[8] * particle->vPosition.z + sk_chunk[0].InvertofM[12];
							particle->plocal_xyz.y = sk_chunk[0].InvertofM[1] * particle->vPosition.x + sk_chunk[0].InvertofM[5] * particle->vPosition.y + sk_chunk[0].InvertofM[9] * particle->vPosition.z + sk_chunk[0].InvertofM[13];
							particle->plocal_xyz.z = sk_chunk[0].InvertofM[2] * particle->vPosition.x + sk_chunk[0].InvertofM[6] * particle->vPosition.y + sk_chunk[0].InvertofM[10] * particle->vPosition.z + sk_chunk[0].InvertofM[14];
							particle->plocal_rQz.x = sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y + particle->plocal_xyz.z*particle->plocal_xyz.z);
							double z, y;
							z = particle->plocal_xyz.z;
							y = particle->plocal_xyz.y;
							if (z >= 0 && y>0)particle->plocal_rQz.y = atan(z / y);
							if (z>0 && y == 0)particle->plocal_rQz.y = PI*0.5;
							if (z<0 && y == 0)particle->plocal_rQz.y = PI*1.5;
							if (y<0)particle->plocal_rQz.y = PI + atan(z / y);
							if (z <= 0 && y>0)particle->plocal_rQz.y = 2 * PI + atan(z / y);
							particle->plocal_rQz.z = particle->plocal_xyz.x;

							indexz = (int)((sk_chunk[0].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[0].dz);
							if (indexz == w[0])indexz = w[0] - 1;
							indexQ = (int)(particle->plocal_rQz.y / sk_chunk[0].dQ);
							if (indexQ == w[0])indexQ = 0;
							index = indexz*w[0] + indexQ;
							if (index<(w[0] * w[0]) && index >= 0){
								isinternal = true;
								for (int k = 0; k<sk_chunk[0].Cylindricalmap[index].numofTri; k++){
									int a = sk_chunk[0].Cylindricalmap[index].Triindex[k];
									vert = sk_chunk[0].triangle[a].vert[0];
									vector = particle->vPosition - sk_chunk[0].vertex[vert].Convexhull;
									nNormal = sk_chunk[0].triangle[a].Normalizehull;
									if (Dot(vector, nNormal)>0){
										isinternal = false;//未碰撞
										break;
									}
								}
								if (isinternal){//碰撞
									//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
									CVector3 p;
									p.Set(particle->plocal_xyz.x, 0, 0);//p为particle的局部xyz坐标在圆柱中心轴上的投影点
									particle->vprojection.x = sk_chunk[0].Mobb[0] * p.x + sk_chunk[0].Mobb[12];
									particle->vprojection.y = sk_chunk[0].Mobb[1] * p.x + sk_chunk[0].Mobb[13];
									particle->vprojection.z = sk_chunk[0].Mobb[2] * p.x + sk_chunk[0].Mobb[14];

									vector = particle->vPosition - particle->vprojection;
									//CVector3 temptriNormal;
									//mint=10000;
									//计算射线vector与相应三角形的碰撞响应点
									for (int k = 0; k<sk_chunk[0].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[0].Cylindricalmap[index].Triindex[k];
										nNormal = sk_chunk[0].triangle[a].Normalizehull;
										double t = -(particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[0].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
										tempPC.x = particle->vPosition.x + t*vector.x;
										tempPC.y = particle->vPosition.y + t*vector.y;
										tempPC.z = particle->vPosition.z + t*vector.z;
										if (isinside(tempPC, sk_chunk, 0, a)){
											if (particle->plocal_xyz.y>0){
												particle->bLocked = 1;
												particle->vPosition = tempPC - nNormal*0.3;
											}
											else particle->vPosition = tempPC + nNormal*0.15;
											particle->vprePosition = particle->vPosition;
											break;
										}
									}
								}
							}
						}
					}
					else{
						if (particle->vPosition.x <= sk_chunk[2].vertex[0].Coordinates.x){  //对衣服的身体部分的质点与身体进行详细检测 
							//将该质点的坐标转换为身体的局部坐标（包括xyz和rQz）
							particle->plocal_xyz.x = particle->vPosition.x + sk_chunk[2].InvertofM[12];
							particle->plocal_xyz.y = particle->vPosition.y + sk_chunk[2].InvertofM[13];
							particle->plocal_xyz.z = particle->vPosition.z + sk_chunk[2].InvertofM[14];
							particle->plocal_rQz.x = sqrt(particle->plocal_xyz.x*particle->plocal_xyz.x + particle->plocal_xyz.z*particle->plocal_xyz.z);
							double z, x;
							z = particle->plocal_xyz.z;
							x = particle->plocal_xyz.x;
							if (z >= 0 && x>0)particle->plocal_rQz.y = atan(z / x);
							if (z>0 && x == 0)particle->plocal_rQz.y = PI*0.5;
							if (z<0 && x == 0)particle->plocal_rQz.y = PI*1.5;
							if (x<0)particle->plocal_rQz.y = PI + atan(z / x);
							if (z<0 && x>0)particle->plocal_rQz.y = 2 * PI + atan(z / x);
							particle->plocal_rQz.z = particle->plocal_xyz.y;

							indexz = (sk_chunk[2].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[2].dz;
							if (indexz == w[2])indexz = w[2] - 1;
							indexQ = particle->plocal_rQz.y / sk_chunk[2].dQ;
							if (indexQ == w[2] * 2)indexQ = 0;
							index = indexz*w[2] * 2 + indexQ;
							if (index<(w[2] * w[2] * 2) && index >= 0){
								isinternal = true;
								for (int k = 0; k<sk_chunk[2].Cylindricalmap[index].numofTri; k++){
									int a = sk_chunk[2].Cylindricalmap[index].Triindex[k];
									vert = sk_chunk[2].triangle[a].vert[0];
									vector = particle->vPosition - sk_chunk[2].vertex[vert].Convexhull;
									nNormal = sk_chunk[2].triangle[a].Normalizehull;
									if (Dot(vector, nNormal)>0){
										isinternal = false;
										break;
									}
								}
								if (isinternal){
									//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
									CVector3 p;
									p.Set(0, particle->plocal_xyz.y, 0);
									particle->vprojection.x = sk_chunk[2].Mobb[12];
									particle->vprojection.y = p.y + sk_chunk[2].Mobb[13];
									particle->vprojection.z = sk_chunk[2].Mobb[14];

									vector = particle->vPosition - particle->vprojection;
									if (vector.x>0.5)vector.x = 0.5;//(针对衣裙的肩带所做的处理)
									if (vector.x<-0.5)vector.x = -0.5;
									//CVector3 temptriNormal;
									//mint=10000;
									//计算射线vector与相应三角形的碰撞响应点
									for (int k = 0; k<sk_chunk[2].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[2].Cylindricalmap[index].Triindex[k];
										nNormal = sk_chunk[2].triangle[a].Normalizehull;
										double t = -(particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[2].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
										tempPC.x = particle->vPosition.x + t*vector.x;
										tempPC.y = particle->vPosition.y + t*vector.y;
										tempPC.z = particle->vPosition.z + t*vector.z;
										if (isinside(tempPC, sk_chunk, 2, a)){
											if (key == 0)
												particle->vPosition = tempPC + nNormal*0.05;// 线衫1：*0.12
											else
												particle->vPosition = tempPC + nNormal*0.005;
											particle->vprePosition = particle->vPosition;
											particle->bLocked = 1;
											break;
										}
									}
								}
							}
						}
						else{                                                            //对右袖子的质点与右大臂进行详细检测
							v = Vector(particle->vPosition, sk_chunk[8].vertex[q[8] / 2].Convexhull);
							if (Dot(v, sk_chunk[8].v)>0){
								//将该质点的坐标转换为右大臂的局部坐标（包括xyz和rQz）
								particle->plocal_xyz.x = sk_chunk[7].InvertofM[0] * particle->vPosition.x + sk_chunk[7].InvertofM[4] * particle->vPosition.y + sk_chunk[7].InvertofM[8] * particle->vPosition.z + sk_chunk[7].InvertofM[12];
								particle->plocal_xyz.y = sk_chunk[7].InvertofM[1] * particle->vPosition.x + sk_chunk[7].InvertofM[5] * particle->vPosition.y + sk_chunk[7].InvertofM[9] * particle->vPosition.z + sk_chunk[7].InvertofM[13];
								particle->plocal_xyz.z = sk_chunk[7].InvertofM[2] * particle->vPosition.x + sk_chunk[7].InvertofM[6] * particle->vPosition.y + sk_chunk[7].InvertofM[10] * particle->vPosition.z + sk_chunk[7].InvertofM[14];
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

								indexz = (sk_chunk[7].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[7].dz;
								if (indexz == w[7])indexz = w[7] - 1;
								indexQ = particle->plocal_rQz.y / sk_chunk[7].dQ;
								if (indexQ == w[7])indexz = 0;
								index = indexz*w[7] + indexQ;
								if (index<w[7] * w[7] && index >= 0){
									isinternal = true;
									for (int k = 0; k<sk_chunk[7].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[7].Cylindricalmap[index].Triindex[k];
										vert = sk_chunk[7].triangle[a].vert[0];
										vector = particle->vPosition - sk_chunk[7].vertex[vert].Convexhull;
										nNormal = sk_chunk[7].triangle[a].Normalizehull;
										if (Dot(vector, nNormal)>0){
											isinternal = false;
											break;
										}
									}
									if (isinternal){
										//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
										CVector3 p;
										p.Set(particle->plocal_xyz.x, 0, 0);
										particle->vprojection.x = sk_chunk[7].Mobb[0] * p.x + sk_chunk[7].Mobb[12];
										particle->vprojection.y = sk_chunk[7].Mobb[1] * p.x + sk_chunk[7].Mobb[13];
										particle->vprojection.z = sk_chunk[7].Mobb[2] * p.x + sk_chunk[7].Mobb[14];

										vector = particle->vPosition - particle->vprojection;
										//CVector3 temptriNormal;
										//mint=10000;
										//计算射线vector与相应三角形的碰撞响应点
										for (int k = 0; k<sk_chunk[7].Cylindricalmap[index].numofTri; k++){
											int a = sk_chunk[7].Cylindricalmap[index].Triindex[k];
											nNormal = sk_chunk[7].triangle[a].Normalizehull;
											double t = (particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[7].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
											tempPC.x = particle->vPosition.x - t*vector.x;
											tempPC.y = particle->vPosition.y - t*vector.y;
											tempPC.z = particle->vPosition.z - t*vector.z;
											if (isinside(tempPC, sk_chunk, 7, a)){
												particle->vPosition = tempPC + nNormal*0.05; // 线衫1：*0.15
												particle->vprePosition = particle->vPosition;
												if (particle->plocal_xyz.y<0)particle->bLocked = 1;
												break;
											}
										}
									}
								}
							}
							else{
								particle->plocal_xyz.x = sk_chunk[8].InvertofM[0] * particle->vPosition.x + sk_chunk[8].InvertofM[4] * particle->vPosition.y + sk_chunk[8].InvertofM[8] * particle->vPosition.z + sk_chunk[8].InvertofM[12];
								particle->plocal_xyz.y = sk_chunk[8].InvertofM[1] * particle->vPosition.x + sk_chunk[8].InvertofM[5] * particle->vPosition.y + sk_chunk[8].InvertofM[9] * particle->vPosition.z + sk_chunk[8].InvertofM[13];
								particle->plocal_xyz.z = sk_chunk[8].InvertofM[2] * particle->vPosition.x + sk_chunk[8].InvertofM[6] * particle->vPosition.y + sk_chunk[8].InvertofM[10] * particle->vPosition.z + sk_chunk[8].InvertofM[14];
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

								indexz = (sk_chunk[8].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[8].dz;
								if (indexz == w[8])indexz = w[8] - 1;
								indexQ = particle->plocal_rQz.y / sk_chunk[8].dQ;
								if (indexQ == w[8])indexz = 0;
								index = indexz*w[8] + indexQ;
								if (index<w[8] * w[8] && index >= 0){
									isinternal = true;
									for (int k = 0; k<sk_chunk[8].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[8].Cylindricalmap[index].Triindex[k];
										vert = sk_chunk[8].triangle[a].vert[0];
										vector = particle->vPosition - sk_chunk[8].vertex[vert].Convexhull;
										nNormal = sk_chunk[8].triangle[a].Normalizehull;
										if (Dot(vector, nNormal)>0){
											isinternal = false;
											break;
										}
									}
									if (isinternal){
										//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
										CVector3 p;
										p.Set(particle->plocal_xyz.x, 0, 0);
										particle->vprojection.x = sk_chunk[8].Mobb[0] * p.x + sk_chunk[8].Mobb[12];
										particle->vprojection.y = sk_chunk[8].Mobb[1] * p.x + sk_chunk[8].Mobb[13];
										particle->vprojection.z = sk_chunk[8].Mobb[2] * p.x + sk_chunk[8].Mobb[14];

										vector = particle->vPosition - particle->vprojection;

										for (int k = 0; k<sk_chunk[8].Cylindricalmap[index].numofTri; k++){
											int a = sk_chunk[8].Cylindricalmap[index].Triindex[k];
											nNormal = sk_chunk[8].triangle[a].Normalizehull;
											double t = (particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[8].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
											tempPC.x = particle->vPosition.x - t*vector.x;
											tempPC.y = particle->vPosition.y - t*vector.y;
											tempPC.z = particle->vPosition.z - t*vector.z;
											if (isinside(tempPC, sk_chunk, 8, a)){
												particle->vPosition = tempPC + nNormal*0.05;
												particle->vprePosition = particle->vPosition;
												if (particle->plocal_xyz.y<0)particle->bLocked = 1;
												break;
											}
										}
									}
								}
							}
						}
					}
				}
				else{//检测腿部碰撞情况
					if (particle->vPosition.x<sk_chunk[2].vertex[q[2] / 2].Coordinates.x){//对左袖子的质点与左大臂进行详细检测
						//将该质点的坐标转换为左腿的局部坐标（包括xyz和rQz）
						if (particle->vPosition.y >= sk_chunk[3].vertex[w[3] * (q[3] + 1)].Convexhull.y){//左大腿
							particle->plocal_xyz.x = sk_chunk[3].InvertofM[0] * particle->vPosition.x + sk_chunk[3].InvertofM[4] * particle->vPosition.y + sk_chunk[3].InvertofM[8] * particle->vPosition.z + sk_chunk[3].InvertofM[12];
							particle->plocal_xyz.y = sk_chunk[3].InvertofM[1] * particle->vPosition.x + sk_chunk[3].InvertofM[5] * particle->vPosition.y + sk_chunk[3].InvertofM[9] * particle->vPosition.z + sk_chunk[3].InvertofM[13];
							particle->plocal_xyz.z = sk_chunk[3].InvertofM[2] * particle->vPosition.x + sk_chunk[3].InvertofM[6] * particle->vPosition.y + sk_chunk[3].InvertofM[10] * particle->vPosition.z + sk_chunk[3].InvertofM[14];
							particle->plocal_rQz.x = sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y + particle->plocal_xyz.z*particle->plocal_xyz.z);
							double z, y;
							z = particle->plocal_xyz.z;
							y = particle->plocal_xyz.y;
							if (z >= 0 && y>0)particle->plocal_rQz.y = atan(z / y);
							if (z>0 && y == 0)particle->plocal_rQz.y = PI*0.5;
							if (z<0 && y == 0)particle->plocal_rQz.y = PI*1.5;
							if (y<0)particle->plocal_rQz.y = PI + atan(z / y);
							if (z <= 0 && y>0)particle->plocal_rQz.y = 2 * PI + atan(z / y);
							particle->plocal_rQz.z = particle->plocal_xyz.x;

							indexz = (int)((sk_chunk[3].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[3].dz);
							if (indexz == w[3])indexz = w[3] - 1;
							indexQ = (int)(particle->plocal_rQz.y / sk_chunk[3].dQ);
							if (indexQ == w[3])indexQ = 0;
							index = indexz*w[3] + indexQ;
							if (index<(w[3] * w[3]) && index >= 0){
								isinternal = true;
								for (int k = 0; k<sk_chunk[3].Cylindricalmap[index].numofTri; k++){
									int a = sk_chunk[3].Cylindricalmap[index].Triindex[k];
									vert = sk_chunk[3].triangle[a].vert[0];
									vector = particle->vPosition - sk_chunk[3].vertex[vert].Convexhull;
									nNormal = sk_chunk[3].triangle[a].Normalizehull;
									if (Dot(vector, nNormal)>0){
										isinternal = false;//未碰撞
										break;
									}
								}
								if (isinternal){//碰撞
									//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
									CVector3 p;
									p.Set(particle->plocal_xyz.x, 0, 0);//p为particle的局部xyz坐标在圆柱中心轴上的投影点
									particle->vprojection.x = sk_chunk[3].Mobb[0] * p.x + sk_chunk[3].Mobb[12];
									particle->vprojection.y = sk_chunk[3].Mobb[1] * p.x + sk_chunk[3].Mobb[13];
									particle->vprojection.z = sk_chunk[3].Mobb[2] * p.x + sk_chunk[3].Mobb[14];

									vector = particle->vPosition - particle->vprojection;
									//CVector3 temptriNormal;
									//mint=10000;
									//计算射线vector与相应三角形的碰撞响应点
									for (int k = 0; k<sk_chunk[3].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[3].Cylindricalmap[index].Triindex[k];
										nNormal = sk_chunk[3].triangle[a].Normalizehull;
										double t = -(particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[3].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
										tempPC.x = particle->vPosition.x + t*vector.x;
										tempPC.y = particle->vPosition.y + t*vector.y;
										tempPC.z = particle->vPosition.z + t*vector.z;
										if (isinside(tempPC, sk_chunk, 3, a)){
											particle->vPosition = tempPC + nNormal*0.1;
											particle->vprePosition = particle->vPosition;
											particle->bLocked = 1;
											break;
										}
									}
								}
							}
						}
						else{//左小腿
							particle->plocal_xyz.x = sk_chunk[4].InvertofM[0] * particle->vPosition.x + sk_chunk[4].InvertofM[4] * particle->vPosition.y + sk_chunk[4].InvertofM[8] * particle->vPosition.z + sk_chunk[4].InvertofM[12];
							particle->plocal_xyz.y = sk_chunk[4].InvertofM[1] * particle->vPosition.x + sk_chunk[4].InvertofM[5] * particle->vPosition.y + sk_chunk[4].InvertofM[9] * particle->vPosition.z + sk_chunk[4].InvertofM[13];
							particle->plocal_xyz.z = sk_chunk[4].InvertofM[2] * particle->vPosition.x + sk_chunk[4].InvertofM[6] * particle->vPosition.y + sk_chunk[4].InvertofM[10] * particle->vPosition.z + sk_chunk[4].InvertofM[14];
							particle->plocal_rQz.x = sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y + particle->plocal_xyz.z*particle->plocal_xyz.z);
							double z, y;
							z = particle->plocal_xyz.z;
							y = particle->plocal_xyz.y;
							if (z >= 0 && y>0)particle->plocal_rQz.y = atan(z / y);
							if (z>0 && y == 0)particle->plocal_rQz.y = PI*0.5;
							if (z<0 && y == 0)particle->plocal_rQz.y = PI*1.5;
							if (y<0)particle->plocal_rQz.y = PI + atan(z / y);
							if (z <= 0 && y>0)particle->plocal_rQz.y = 2 * PI + atan(z / y);
							particle->plocal_rQz.z = particle->plocal_xyz.x;

							indexz = (int)((sk_chunk[4].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[4].dz);
							if (indexz == w[4])indexz = w[4] - 1;
							indexQ = (int)(particle->plocal_rQz.y / sk_chunk[4].dQ);
							if (indexQ == w[4])indexQ = 0;
							index = indexz*w[4] + indexQ;
							if (index<(w[4] * w[4]) && index >= 0){
								isinternal = true;
								for (int k = 0; k<sk_chunk[4].Cylindricalmap[index].numofTri; k++){
									int a = sk_chunk[4].Cylindricalmap[index].Triindex[k];
									vert = sk_chunk[4].triangle[a].vert[0];
									vector = particle->vPosition - sk_chunk[4].vertex[vert].Convexhull;
									nNormal = sk_chunk[4].triangle[a].Normalizehull;
									if (Dot(vector, nNormal)>0){
										isinternal = false;//未碰撞
										break;
									}
								}
								if (isinternal){//碰撞
									//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
									CVector3 p;
									p.Set(particle->plocal_xyz.x, 0, 0);//p为particle的局部xyz坐标在圆柱中心轴上的投影点
									particle->vprojection.x = sk_chunk[4].Mobb[0] * p.x + sk_chunk[4].Mobb[12];
									particle->vprojection.y = sk_chunk[4].Mobb[1] * p.x + sk_chunk[4].Mobb[13];
									particle->vprojection.z = sk_chunk[4].Mobb[2] * p.x + sk_chunk[4].Mobb[14];

									vector = particle->vPosition - particle->vprojection;
									//CVector3 temptriNormal;
									//mint=10000;
									//计算射线vector与相应三角形的碰撞响应点
									for (int k = 0; k<sk_chunk[4].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[4].Cylindricalmap[index].Triindex[k];
										nNormal = sk_chunk[4].triangle[a].Normalizehull;
										double t = -(particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[4].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
										tempPC.x = particle->vPosition.x + t*vector.x;
										tempPC.y = particle->vPosition.y + t*vector.y;
										tempPC.z = particle->vPosition.z + t*vector.z;
										if (isinside(tempPC, sk_chunk, 4, a)){
											particle->vPosition = tempPC + nNormal*0.1;
											particle->vprePosition = particle->vPosition;
											particle->bLocked = 1;
											break;
										}
									}
								}
							}
						}
					}
					else{ //右腿
						if (particle->vPosition.y >= sk_chunk[5].vertex[w[5] * (q[5] + 1)].Convexhull.y){
							particle->plocal_xyz.x = sk_chunk[5].InvertofM[0] * particle->vPosition.x + sk_chunk[5].InvertofM[4] * particle->vPosition.y + sk_chunk[5].InvertofM[8] * particle->vPosition.z + sk_chunk[5].InvertofM[12];
							particle->plocal_xyz.y = sk_chunk[5].InvertofM[1] * particle->vPosition.x + sk_chunk[5].InvertofM[5] * particle->vPosition.y + sk_chunk[5].InvertofM[9] * particle->vPosition.z + sk_chunk[5].InvertofM[13];
							particle->plocal_xyz.z = sk_chunk[5].InvertofM[2] * particle->vPosition.x + sk_chunk[5].InvertofM[6] * particle->vPosition.y + sk_chunk[5].InvertofM[10] * particle->vPosition.z + sk_chunk[5].InvertofM[14];
							particle->plocal_rQz.x = sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y + particle->plocal_xyz.z*particle->plocal_xyz.z);
							double z, y;
							z = particle->plocal_xyz.z;
							y = particle->plocal_xyz.y;
							if (z >= 0 && y>0)particle->plocal_rQz.y = atan(z / y);
							if (z>0 && y == 0)particle->plocal_rQz.y = PI*0.5;
							if (z<0 && y == 0)particle->plocal_rQz.y = PI*1.5;
							if (y<0)particle->plocal_rQz.y = PI + atan(z / y);
							if (z <= 0 && y>0)particle->plocal_rQz.y = 2 * PI + atan(z / y);
							particle->plocal_rQz.z = particle->plocal_xyz.x;

							indexz = (int)((sk_chunk[5].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[5].dz);
							if (indexz == w[5])indexz = w[5] - 1;
							indexQ = (int)(particle->plocal_rQz.y / sk_chunk[5].dQ);
							if (indexQ == w[5])indexQ = 0;
							index = indexz*w[5] + indexQ;
							if (index<(w[5] * w[5]) && index >= 0){
								isinternal = true;
								for (int k = 0; k<sk_chunk[5].Cylindricalmap[index].numofTri; k++){
									int a = sk_chunk[5].Cylindricalmap[index].Triindex[k];
									vert = sk_chunk[5].triangle[a].vert[0];
									vector = particle->vPosition - sk_chunk[5].vertex[vert].Convexhull;
									nNormal = sk_chunk[5].triangle[a].Normalizehull;
									if (Dot(vector, nNormal)>0){
										isinternal = false;//未碰撞
										break;
									}
								}
								if (isinternal){//碰撞
									//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
									CVector3 p;
									p.Set(particle->plocal_xyz.x, 0, 0);//p为particle的局部xyz坐标在圆柱中心轴上的投影点
									particle->vprojection.x = sk_chunk[5].Mobb[0] * p.x + sk_chunk[5].Mobb[12];
									particle->vprojection.y = sk_chunk[5].Mobb[1] * p.x + sk_chunk[5].Mobb[13];
									particle->vprojection.z = sk_chunk[5].Mobb[2] * p.x + sk_chunk[5].Mobb[14];

									vector = particle->vPosition - particle->vprojection;
									//CVector3 temptriNormal;
									//mint=10000;
									//计算射线vector与相应三角形的碰撞响应点
									for (int k = 0; k<sk_chunk[5].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[5].Cylindricalmap[index].Triindex[k];
										nNormal = sk_chunk[5].triangle[a].Normalizehull;
										double t = -(particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[5].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
										tempPC.x = particle->vPosition.x + t*vector.x;
										tempPC.y = particle->vPosition.y + t*vector.y;
										tempPC.z = particle->vPosition.z + t*vector.z;
										if (isinside(tempPC, sk_chunk, 5, a)){
											particle->vPosition = tempPC + nNormal*0.05;
											particle->vprePosition = particle->vPosition;
											particle->bLocked = 1;
											break;
										}
									}
								}
							}
						}
						else{
							particle->plocal_xyz.x = sk_chunk[6].InvertofM[0] * particle->vPosition.x + sk_chunk[6].InvertofM[4] * particle->vPosition.y + sk_chunk[6].InvertofM[8] * particle->vPosition.z + sk_chunk[6].InvertofM[12];
							particle->plocal_xyz.y = sk_chunk[6].InvertofM[1] * particle->vPosition.x + sk_chunk[6].InvertofM[5] * particle->vPosition.y + sk_chunk[6].InvertofM[9] * particle->vPosition.z + sk_chunk[6].InvertofM[13];
							particle->plocal_xyz.z = sk_chunk[6].InvertofM[2] * particle->vPosition.x + sk_chunk[6].InvertofM[6] * particle->vPosition.y + sk_chunk[6].InvertofM[10] * particle->vPosition.z + sk_chunk[6].InvertofM[14];
							particle->plocal_rQz.x = sqrt(particle->plocal_xyz.y*particle->plocal_xyz.y + particle->plocal_xyz.z*particle->plocal_xyz.z);
							double z, y;
							z = particle->plocal_xyz.z;
							y = particle->plocal_xyz.y;
							if (z >= 0 && y>0)particle->plocal_rQz.y = atan(z / y);
							if (z>0 && y == 0)particle->plocal_rQz.y = PI*0.5;
							if (z<0 && y == 0)particle->plocal_rQz.y = PI*1.5;
							if (y<0)particle->plocal_rQz.y = PI + atan(z / y);
							if (z <= 0 && y>0)particle->plocal_rQz.y = 2 * PI + atan(z / y);
							particle->plocal_rQz.z = particle->plocal_xyz.x;

							indexz = (int)((sk_chunk[6].z_min_max[1] - particle->plocal_rQz.z) / sk_chunk[6].dz);
							if (indexz == w[6])indexz = w[6] - 1;
							indexQ = (int)(particle->plocal_rQz.y / sk_chunk[6].dQ);
							if (indexQ == w[6])indexQ = 0;
							index = indexz*w[6] + indexQ;
							if (index<(w[6] * w[6]) && index >= 0){
								isinternal = true;
								for (int k = 0; k<sk_chunk[6].Cylindricalmap[index].numofTri; k++){
									int a = sk_chunk[6].Cylindricalmap[index].Triindex[k];
									vert = sk_chunk[6].triangle[a].vert[0];
									vector = particle->vPosition - sk_chunk[6].vertex[vert].Convexhull;
									nNormal = sk_chunk[6].triangle[a].Normalizehull;
									if (Dot(vector, nNormal)>0){
										isinternal = false;//未碰撞
										break;
									}
								}
								if (isinternal){//碰撞
									//记录该点在圆柱中心柱上的投影点，并将其转换成全局坐标系
									CVector3 p;
									p.Set(particle->plocal_xyz.x, 0, 0);//p为particle的局部xyz坐标在圆柱中心轴上的投影点
									particle->vprojection.x = sk_chunk[6].Mobb[0] * p.x + sk_chunk[6].Mobb[12];
									particle->vprojection.y = sk_chunk[6].Mobb[1] * p.x + sk_chunk[6].Mobb[13];
									particle->vprojection.z = sk_chunk[6].Mobb[2] * p.x + sk_chunk[6].Mobb[14];

									vector = particle->vPosition - particle->vprojection;
									//CVector3 temptriNormal;
									//mint=10000;
									//计算射线vector与相应三角形的碰撞响应点
									for (int k = 0; k<sk_chunk[6].Cylindricalmap[index].numofTri; k++){
										int a = sk_chunk[6].Cylindricalmap[index].Triindex[k];
										nNormal = sk_chunk[6].triangle[a].Normalizehull;
										double t = -(particle->vPosition.x*nNormal.x + particle->vPosition.y*nNormal.y + particle->vPosition.z*nNormal.z + sk_chunk[6].triangle[a].dhull) / (vector.x*nNormal.x + vector.y*nNormal.y + vector.z*nNormal.z);
										tempPC.x = particle->vPosition.x + t*vector.x;
										tempPC.y = particle->vPosition.y + t*vector.y;
										tempPC.z = particle->vPosition.z + t*vector.z;
										if (isinside(tempPC, sk_chunk, 6, a)){
											particle->vPosition = tempPC + nNormal*0.05;
											particle->vprePosition = particle->vPosition;
											particle->bLocked = 1;
											break;
										}
									}
								}
							}
						}
					}

				}
			}
		}
	}//*/

	if (key == 1){
		for (i = 0; i<thisphysic->objectnumbers; i++){
			for (j = 0; j<thisphysic->physics[i].particlenumbers; j++){
				if (thisphysic->physics[i].pParticles[j].vPosition.y >= sk_chunk[2].vertex[w[2] * (q[2] + 1)].Coordinates.y - 3)//
					thisphysic->physics[i].pParticles[j].bLocked = 1;
			}
		}

	}
}