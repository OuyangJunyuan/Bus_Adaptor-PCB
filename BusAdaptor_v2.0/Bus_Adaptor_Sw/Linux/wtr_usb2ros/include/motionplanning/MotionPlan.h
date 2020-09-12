#pragma once
#include <vector>
#include "Eigen/Dense"
using namespace Eigen;
using namespace std;

enum PlanningType
{
	LFPB_CartesianSpace = 0,
	LFPB_JointSpace,
	B_Spline_c2
}; 
class Pose //单位°
{
public:
	Pose(double x, double y, double z, double yaw, double pitch, double roll);
	~Pose() {};
	double x;				// x坐标，单位mm
	double y;				// y坐标，单位mm
	double z;				// z坐标，单位mm
	double yaw;				// yaw坐标，单位度
	double pitch;			// pitch坐标，单位度
	double roll;			// roll坐标，单位度
	
	static void PoseSets(vector<Pose>& vp, double pose[], int n);
};


class LFPB
{
private:
	double bPos, ePos;
	double dir;
public:
	double x, v, a1, a2;
	double dt_phase1, dt_phase2, dt_phase3, t_total;
	LFPB(double bPos, double ePos, double vel, double acc, double dec);
	~LFPB() {};
	double Mapping(double now);
};
class LFPB_Cartesian :public LFPB
{
private:
	Eigen::Vector3d bPos, ePos;
	Eigen::Vector3d dir;
public:
	LFPB yawLFPB;//yaw角
	LFPB_Cartesian(Eigen::Vector3d& _bPos, Eigen::Vector3d& _ePos, double vel, double acc, double dec, double bYaw, double eYaw, double avel, double aacc, double adec);
	~LFPB_Cartesian() {};
	Vector4d Mapping(double now);
};
/*p表示次数，基函数满足p-k阶连续性*/
class BSpline
{
private:
	int p, m, n;//次数，节点索引下标最大值，控制点索引下标最大值
	vector<double> knot;
	vector<Vector4d> ControlPoint;
	double* DR, * DL,*B;
	void gen_knot(void);
	inline void gen_BasisFunc_now(double u, int i);//当前时刻:映射到[0,1]的节点上
public:
	~BSpline();
	BSpline(vector<Pose> &cp,int degree);
	Vector4d Mapping(double now);
};

class MotionPlan
{
public:
	vector<Pose> target;
	double JointAngleBegin[4],JointAngleEnd[4];					//单位度

	double TotalTime=5;									//为了防止忘记设置就直接调用B样条曲线而设置裕量5
	double SampleTime=0.001;							//采样点位(单位S)

	double Vel,Acc,Dec,Ja,Jd;						//速度，加速加速度，减速加速度 单位m								
	double AVel,AAcc,ADec,AJa,AJd;					//角....					   单位°	

public:
	void SetTotalTime(double maxtime) { TotalTime = maxtime; }; //B样条曲线规划模式下必须设置总时间。
	void SetSampleTime(double sampleTime) { SampleTime = sampleTime; };		//设置采样时间
	void SetProfile(double vel, double acc, double dec, double avel, double aacc, double adec)//设置运动参数，速度、加速度和减速度
	{
		Vel = vel; Acc = acc;	Dec = dec; AVel = avel; ADec = adec; AAcc = aacc;
	};
	void SetPlanPointSet(vector<Pose>& Poses);		//输入起始点位和结束点位的笛卡尔坐标
	//TODO:梯形速度规划根据 目标点集是否大于2来设置是否进行多线段规划
	//TODO:可以选择是否重写输出文本，还是续写输出文本。
    void GetPlan(PlanningType type,vector<int> &output);
};
 void zyz2rot(Pose& startPos, double* mStartMatrixData);
