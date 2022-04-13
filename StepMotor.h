#ifndef StepMotor_h
#define StepMotor_h

#include "DefSystem.h"
//#include "MTickTimeAd.h"
enum LS_DM542
{
	LMS2 = 2,
	LMS4 = 4,
	LMS8 = 8,
	LMS16 = 16,
	LMS32 = 32,
	LMS64 = 64,
	LMS128 = 128,
	LMS5 = 5,
	LMS10 = 10,
	LMS20 = 20,
	LMS25 = 25,
	LMS40 = 40,
	LMS50 = 50,
	LMS100 = 100,
	LMS125 = 125
};

enum Autonis_MD5HD14
{
	AMS1 = 1,
	AMS2 = 2,
	AMS4 = 4,
	AMS5 = 5,
	AMS8 = 8,
	AMS16 = 16,
	AMS20 = 20,
	AMS25 = 25,
	AMS40 = 40,
	AMS50 = 50,
	AMS80 = 80,
	AMS100 = 100,
	AMS125 = 125,
	AMS200 = 200,
	AMS250 = 250
};

class StepMotor
{
private:

//	MTickTimeAd *pDelay;
	int driver = NOT_DEFINE;
	int gearRatio = 1;
	int resolution = 1;
	double stepAngle = 1.8;

	double motorSpeed = 30;
	double pulse_delay = 100;

	// pin control
	int pin_cw = 7;
	int pin_ccw = 6;
	int pin_hold_off = 5;
	int pin_div_sel = 4;

	int pin_pulse = 7;
	int pin_dir = 6;

public:
	bool enableRun;
	StepMotor(int driver);
	~StepMotor();

	void initVar();
	void setDriver(int driver_type);
	void setGear(int gearRatio);
	void setResolution(double microStep);
	void setMotorSpeed(double _speed);
	void setStepAngle(double stepAngle);
	void setPin(int pPulse, int pDir, int pHoldOff, int pDivSel);
	
	void runByPulse(double nPulse, int dir);
	void runByDegree(double nDegree, int dir);
	void runByRevolution(double nRev, int dir);

	double calculatorPulseDelay();

	void pulseMotor(double pulse_delay);
	void pulseMotor(int pin, double pulse_delay);
	void changeMotorDirection(int dir);
	void enableMotor(bool en);

};

#endif
