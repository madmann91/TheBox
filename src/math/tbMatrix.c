#include "tbMatrix.h"

void tbIdentity(tbMatrix m)
{
	m[0]=(tbScalar)1.0;
	m[1]=(tbScalar)0.0;
	m[2]=(tbScalar)0.0;
	m[3]=(tbScalar)1.0;
}

void tbMultiply(const tbMatrix m1,const tbMatrix m2,tbMatrix m3)
{
	m3[0]=m1[0]*m2[0]+m1[1]*m2[2];
	m3[1]=m1[0]*m2[1]+m1[1]*m2[3];
	m3[2]=m1[2]*m2[0]+m1[3]*m2[2];
	m3[3]=m1[2]*m2[1]+m1[3]*m2[3];
}

void tbTranspose(const tbMatrix m1,tbMatrix m2)
{
	m2[0]=m1[0];
	m2[1]=m1[2];
	m2[2]=m1[1];
	m2[3]=m1[3];
}

void tbInverse(const tbMatrix m1,tbMatrix m2)
{
	tbScalar div=(tbScalar)1.0/(m1[0]*m1[3]-m1[1]*m1[2]);
	m2[0]=m1[3]*div;
	m2[1]=-m1[1]*div;
	m2[2]=-m1[2]*div;
	m2[3]=m1[0]*div;
}

tbScalar tbDeterminant(const tbMatrix m)
{
	return m[0]*m[3]-m[1]*m[2];
}
