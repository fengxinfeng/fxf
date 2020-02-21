#include "stdafx.h"

/*
CEarthPoint        pt1, pt2, pt3;
double            dDistance1;
double            dDistance2;

pt1.SetPos(g_xpos, g_ypos);        //    我的位置
pt2.SetPos(_wtof(ar1[2]), _wtof(ar1[3]));        //    目标位置 

dDistance1 = CEarthPoint::Distance(pt1, pt2);
*/






#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif


//    坐标系
class CEarthPoint
{
public:
	double            m_dEarthR;        //    6371.004    6378.140    6356.755
	double            m_dRealR;
	double            m_dAngE;        //    纬度-东
	double            m_dAngN;        //    经度-北
	double            m_x;
	double            m_y;
	double            m_z;
public:
	CEarthPoint()
	{
		m_dEarthR = 6371.004;
		m_dRealR = 0.0;
	}
	virtual ~CEarthPoint()
	{
	}
public:
	//    dAngE:    经度, 东经正数, 西经负数
	  //    dAngN:    纬度, 北纬正数, 南纬负数
	BOOL    SetPos(double dAngE, double dAngN)
	{
		double        dPlanR;
		double        dEarthR;

		m_dAngE = dAngE;
		m_dAngN = dAngN;
		Revisory(dAngN);
		dEarthR = m_dRealR;

		dAngE = m_dAngE * M_PI / 180;
		dAngN = m_dAngN * M_PI / 180;

		m_z = dEarthR * sin(dAngN);
		dPlanR = dEarthR * cos(dAngN);
		m_x = dPlanR * cos(dAngE);
		m_y = dPlanR * sin(dAngE);
		return TRUE;
	}


	BOOL    Revisory(double dAngN)
	{
		double        dMin, dMax, dDlt;

		dAngN = fabs(dAngN);
		if (dAngN > 90.0)
			return false;


		dMin = 6356.755;
		dMax = 6378.140;
		dDlt = (dMax - dMin) * dAngN / 90;
		m_dRealR = dMax - dDlt;
		return TRUE;
	}

	BOOL    SetPos(double x, double y, double z)
	{
		return FALSE;
	}


	static    double    GetArcDistance(double dDis, double dRealR)
	{
		double            dRealDis, dAng;

		dAng = asin(dDis / 2 / dRealR);
		dAng *= 2;
		dRealDis = dRealR * dAng;
		return dRealDis;
	}

	static    double    Distance(CEarthPoint &Pos1, CEarthPoint &Pos2, BOOL bArc = TRUE)
	{
		double            dDis1, dSum, dRealDis;
		double            dRealR;

		dSum = 0;
		dDis1 = Pos1.m_x - Pos2.m_x;    dSum += dDis1 * dDis1;
		dDis1 = Pos1.m_y - Pos2.m_y;    dSum += dDis1 * dDis1;
		dDis1 = Pos1.m_z - Pos2.m_z;    dSum += dDis1 * dDis1;
		dSum = sqrt(dSum);
		if (bArc == FALSE)
		{
			dRealDis = dSum;
			return dRealDis;
		}

		dRealR = (Pos1.m_dRealR + Pos2.m_dRealR) / 2;
		dRealDis = GetArcDistance(dSum, dRealR);
		return dRealDis;
	}
};


