#include "motionplanning/MotionPlan.h"
#include "math.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace Eigen;
#define PI 3.14159
Pose::Pose(double x, double y, double z, double yaw, double pitch, double roll) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
}
void Pose::PoseSets(vector<Pose>& vp, double pose[], int n)
{
	for (int i = 0; i < n; i+=6)
	{
		Pose tp(pose[i], pose[i + 1], pose[i + 2], pose[i + 3], pose[i + 4], pose[i + 5]);
		vp.push_back(tp);
	}
}
void zyz2rot(Pose &startPos, double* mStartMatrixData)
{
	double startAngle[3];
	startAngle[0] = startPos.yaw * PI / 180;
	startAngle[1] = startPos.pitch * PI / 180;
	startAngle[2] = startPos.roll * PI / 180;

	mStartMatrixData[0] = cos(startAngle[0]) * cos(startAngle[1]) * cos(startAngle[2]) - sin(startAngle[0]) * sin(startAngle[2]);
	mStartMatrixData[1] = -cos(startAngle[0]) * cos(startAngle[1]) * sin(startAngle[2]) - sin(startAngle[0]) * cos(startAngle[2]);
	mStartMatrixData[2] = cos(startAngle[0]) * sin(startAngle[1]);
	mStartMatrixData[3] = startPos.x / 1000;

	mStartMatrixData[4] = sin(startAngle[0]) * cos(startAngle[1]) * cos(startAngle[2]) + cos(startAngle[0]) * sin(startAngle[2]);
	mStartMatrixData[5] = -sin(startAngle[0]) * cos(startAngle[1]) * sin(startAngle[2]) + cos(startAngle[0]) * cos(startAngle[2]);
	mStartMatrixData[6] = sin(startAngle[0]) * sin(startAngle[1]);
	mStartMatrixData[7] = startPos.y / 1000;

	mStartMatrixData[8] = -sin(startAngle[1]) * cos(startAngle[2]);
	mStartMatrixData[9] = sin(startAngle[1]) * sin(startAngle[2]);
	mStartMatrixData[10] = cos(startAngle[1]);
	mStartMatrixData[11] = startPos.z / 1000;

	mStartMatrixData[12] = 0;
	mStartMatrixData[13] = 0;
	mStartMatrixData[14] = 0;
	mStartMatrixData[15] = 1;
}

#pragma region LFPB

/********************************************************************
ABSTRACT:	??LFPB
INPUTS:		??????????(???)???????(???)???????(???)??????
***********************************************************************/
LFPB::LFPB(double _bPos, double _ePos, double vel, double acc, double dec)//????????????????????
{
	bPos = _bPos;
	ePos = _ePos;

	dir = (bPos - ePos) < 0 ? 1 : -1;
	x = dir * (ePos - bPos);
	v = vel;
	a1 = dir == 1 ? acc : dec;
	a2 = dir == 1 ? dec : acc;

	dt_phase1 = v / a1, dt_phase3 = v / a2, dt_phase2 = 0;
	double dx_phase1 = 0.5 * a1 * dt_phase1 * dt_phase1, dx_phase3 = 0.5 * a2 * dt_phase3 * dt_phase3, dx_phase2 = 0;
	if (dx_phase1 + dx_phase3 <= x)//?????
	{
		dx_phase2 = x - dx_phase1 - dx_phase3;
		dt_phase2 = dx_phase2 / v;
	}
	else
	{
		dt_phase1 = sqrt(2 * x / (a1 + a1 * a1 / a2));
		dt_phase3 = a1 * dt_phase1 / a2;
	}
	t_total = dt_phase1 + dt_phase2 + dt_phase3;
}

/*?????????????*/
double LFPB::Mapping(double now)
{
	double output;

	if (now <= dt_phase1)
		output = bPos + dir * (0.5 * a1 * now * now);
	else if (now <= dt_phase1 + dt_phase2)
		output = bPos + dir * (0.5 * a1 * dt_phase1 * dt_phase1 + v * (now - dt_phase1));
	else if (now <= dt_phase1 + dt_phase2 + dt_phase3)
		output = ePos - dir * (0.5 * a2 * pow(dt_phase1 + dt_phase2 + dt_phase3 - now, 2));
	else
		output = ePos;
	return output;
}
/*3d??+yaw???????
todolist?????3d????LFPB_Cartesian?3d??1?LFPB_Cartesian???????????*/
LFPB_Cartesian::LFPB_Cartesian(Eigen::Vector3d& _bPos, Eigen::Vector3d &_ePos, double vel, double acc, double dec, double bYaw, double eYaw, double avel, double aacc, double adec)
	:LFPB(0, (_ePos - _bPos).norm(), vel, acc, dec), yawLFPB(bYaw, eYaw, avel, aacc, adec)
{
	bPos = _bPos;
	ePos = _ePos;
	dir = (_ePos - _bPos) / (_ePos - _bPos).norm();
}
Vector4d LFPB_Cartesian::Mapping(double now)
{
	Vector4d output;
	double distance;
	distance = LFPB::Mapping(now);
	output << bPos + distance * dir, yawLFPB.Mapping(now / t_total * yawLFPB.t_total);
	return output;
}
#pragma endregion 

#pragma region Bspline
BSpline::BSpline(vector<Pose> &cp, int degree)
{
	for (int i = 0; i < cp.size(); i++)
	{
		Vector4d temp;
		temp << cp[i].x, cp[i].y, cp[i].z, cp[i].roll* PI / 180;
		ControlPoint.push_back(temp);
	}
	p = degree;
	n = cp.size() - 1;
	m = n + p + 1;
	DR = new double[p + 1];
	DL = new double[p + 1];
	B = new double[p + 1];
	gen_knot();
}
BSpline::~BSpline()
{
	delete[] DR;
	delete[] B;
	delete[] DL;
}
//????clamped??
void BSpline::gen_knot(void)
{
	int temp = m - 2 * p;
	for (int i = 0; i <p; i++)
		this->knot.push_back(0);
	for (int i = 0; i <= temp; i++)
		this->knot.push_back(i*1.0f / temp);
	for (int i = 0; i < p; i++)
		this->knot.push_back(1);
}
//?????????????u ? [u_i,u_i+1)

inline int whichSpan(vector<double>& U, double u, int m, int p)
{
	int high = m - p, low = p, mid = 0;
	if (u == U[m])
		mid = high;
	else
	{
		mid = (int)((high + low) / 2);
		while (u < U[mid] || u >= U[mid + 1])
		{
			if (u == U[mid + 1])
				mid = mid + 1;
			else
			{
				if (u > U[mid + 1])
					low = mid;
				else
					high = mid;
				mid = (high + low) / 2;
			}
		}
	}
	return mid;
}
//?????????????????i?
inline void BSpline::gen_BasisFunc_now(double u,int i)
{
	int j, r;
	double temp, acc;
	B[0] = 1;
	for (j = 1; j <= p; j++)
	{
		DL[j] = u - knot[i + 1 - j];
		DR[j] = knot[i + j] - u;
		acc = 0;
		for (r = 0; r <= j - 1; r++)
		{
			temp = B[r] / (DR[r + 1] + DL[j - r]);
			B[r] = acc + DR[r + 1] * temp;
			acc = DL[j - r] * temp;
		}
		B[j] = acc;
	}
}
Vector4d BSpline::Mapping(double now)
{
	Vector4d pout;
	pout << 0, 0, 0, 0;
	int i = whichSpan(knot, now, m, p);
	gen_BasisFunc_now(now,i);//??????now???
	for (int k = 0; k <= p; k++)
	{
		pout += ControlPoint[i - p + k] * B[k];
	}
	return pout;
}


#pragma endregion 

#pragma region MotionPlan


void MotionPlan::SetPlanPointSet(vector<Pose>& Poses)
{
	this->target = Poses;
}
#include <unistd.h>
void MotionPlan::GetPlan(PlanningType type,vector<int> &output)
{
    vector<int> out;
    output=out;
	double dt = this->SampleTime;
	double now = 0;
	switch (type)
	{

	case LFPB_CartesianSpace:
	{
		break;
	}
    case LFPB_JointSpace:
    {
        break;
    }
	case B_Spline_c2:
	{
		Vector4d C_output;
		vector<Pose> expand_target;//???????????????3??????????0?? ??0???
		for (int i = 0; i < 3-1; i++)
			expand_target.push_back(target.front());
		expand_target.insert(expand_target.end(), target.begin(), target.end());
		for (int i = 0; i < 3-1; i++)
			expand_target.push_back(target.back());

		BSpline bspline(expand_target,3);
		for (now; now < TotalTime; now += dt)
		{
			C_output = bspline.Mapping(now/TotalTime);
			usleep(1000);
            output.push_back(C_output[0]);
		}
		break;
	}
	default:
		break;
	}
}
#pragma endregion 

