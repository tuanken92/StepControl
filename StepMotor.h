#ifndef StepMotor_h
#define StepMotor_h

#include "DefSystem.h"
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

    int driver = NOT_DEFINE;
    int gearRatio = 1;
    int resolution = 1;
    double stepAngle = 1.8;

    double motorSpeed = 30;
    double maxSpeed = 30;
    double pulse_delay = 100;



    // pin control
    int pin_pulse = 10;
    int pin_dir = 9;

  public:
   


    StepMotor(int driver);
    ~StepMotor();

    void initVar(int driver_type);
    void setDriver(int driver_type);
    void setGear(int gearRatio);
    void setResolution(double microStep);
    void setMotorSpeed(double _speed);
    void setStepAngle(double stepAngle);

    double getMaxSpeed();
    
    double runByPulse(double nPulse, int dir);
    double runByDegree(double nDegree, int dir);
    double runByRevolution(double nRev, int dir);

    double calculatorPulseDelay();

    void changeMotorDirection(int dir);
    void stopMotor();


    

};

#endif
