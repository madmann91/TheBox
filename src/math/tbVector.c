#include "tbConfig.h"
#include "tbVector.h"

void tbZero(tbVector v)
{
	v[0]=(tbScalar)0.0;
	v[1]=(tbScalar)0.0;
}

tbScalar tbDot(const tbVector u,const tbVector v)
{
	return u[0]*v[0]+u[1]*v[1];
}

void tbPerp(const tbVector u,tbVector v)
{
	v[0]=-u[1];
	v[1]=u[0];
}

tbScalar tbLength(const tbVector v)
{
	return tbSqrt(v[0]*v[0]+v[1]*v[1]);
}

tbScalar tbSquareLength(const tbVector v)
{
	return v[0]*v[0]+v[1]*v[1];
}

void tbSum(const tbVector u,const tbVector v,tbVector w)
{
	w[0]=u[0]+v[0];
	w[1]=u[1]+v[1];
}

void tbDiff(const tbVector u,const tbVector v,tbVector w)
{
	w[0]=u[0]-v[0];
	w[1]=u[1]-v[1];
}

void tbScale(tbVector v,tbScalar s)
{
	v[0]*=s;
	v[1]*=s;
}

void tbMin(const tbVector a,const tbVector b,tbVector c)
{
	c[0]=(a[0]<b[0])?a[0]:b[0];
	c[1]=(a[1]<b[1])?a[1]:b[1];
}

void tbMax(const tbVector a,const tbVector b,tbVector c)
{
	c[0]=(a[0]>b[0])?a[0]:b[0];
	c[1]=(a[1]>b[1])?a[1]:b[1];
}
