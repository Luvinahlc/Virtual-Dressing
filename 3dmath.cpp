#include "stdafx.h"
#include "3dmath.h"

//数学函数
// This computes the magnitude of a normal.   (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// 向量化
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector;							// The variable to hold the resultant vector

	vVector.x = vPoint1.x - vPoint2.x;			// Subtract point1 and point2 x's
	vVector.y = vPoint1.y - vPoint2.y;			// Subtract point1 and point2 y's
	vVector.z = vPoint1.z - vPoint2.z;			// Subtract point1 and point2 z's

	return vVector;								// Return the resultant vector
}
// 规格化
CVector3 Normalize(CVector3 vNormal)
{
	float Magnitude;							// This holds the magitude			

	Magnitude = Mag(vNormal);					// Get the magnitude
	if(Magnitude!=0){
		vNormal.x /= (float)Magnitude;				// Divide the vector's X by the magnitude
		vNormal.y /= (float)Magnitude;				// Divide the vector's Y by the magnitude
		vNormal.z /= (float)Magnitude;				// Divide the vector's Z by the magnitude
	}

	return vNormal;								// Return the normal
}
//绝对值
float Absolute(float num)
{
	return num>=0?num:-num;
}

/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a normal (or any other vector)
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(CVector3 vNormal)
{
	// This will give us the magnitude or "Norm" as some say, of our normal.
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)  Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) );
}
// This returns the cross product between 2 vectors
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;								// The vector to hold the cross product
												// Get the X value
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
												// Get the Y value
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
												// Get the Z value
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;								// Return the cross product
}

float Dot(CVector3 vVector1, CVector3 vVector2){
	return (vVector1.x*vVector2.x + vVector1.y*vVector2.y + vVector1.z*vVector2.z);
}
float Distance(CVector3 vPoint1, CVector3 vPoint2){
	CVector3 dist12=Vector(vPoint2,vPoint1);
	return Magnitude(dist12);
}
