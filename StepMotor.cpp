#include "Arduino.h"
#include "StepMotor.h"

StepMotor::StepMotor(int _driver)
{


  driver = _driver;
  enableRun = true;
  switch (driver)
  {
    case DM542_05:

      gearRatio = 1;
      resolution = 2;
      stepAngle = 1.8;
      motorSpeed = 30;
      pulse_delay = calculatorPulseDelay();

      pin_pulse = 7;
      pin_dir = 6;
      break;

    case MD5_HD14:

      gearRatio = 10;
      resolution = 1;
      stepAngle = 0.72;
      motorSpeed = 30;
      pulse_delay = calculatorPulseDelay();

      pin_cw = 7;
      pin_ccw = 6;
      pin_hold_off = 5;
      pin_div_sel = 4;

      break;

    default:
      break;
  }
}

double StepMotor::calculatorPulseDelay()
{
  double _step = stepAngle / resolution / gearRatio;

  double freq = (360 * motorSpeed) / (60 * _step);
  double _pulse_delay = 1000000 / freq; // us

  //double nPulsePerRev = 360/_step;
  //double nStepInAMinutie = nPulsePerRev*motorSpeed;
  //double t = 60000000/nStepInAMinutie;

#ifdef DEBUG
  Serial.println("Calculator -> driver = " + String(driver));
  Serial.println("Calculator -> speed(rpm) = " + String(motorSpeed));
  Serial.println("Calculator -> stepAngle = " + String(stepAngle));
  Serial.println("Calculator -> _step = " + String(_step));
  Serial.println("Calculator -> freq = " + String(freq));
  Serial.println("Calculator -> pulse_delay (uS)= " + String(pulse_delay));
  //Serial.println("Calculator -> t (us) = " + String(t));
#endif

  return _pulse_delay;
}
StepMotor::~StepMotor()
{

}

void StepMotor::initVar()
{

  switch (driver)
  {
    case DM542_05:
      pinMode(pin_pulse, OUTPUT);
      pinMode(pin_dir, OUTPUT);
      digitalWrite(pin_pulse, LOW);
      digitalWrite(pin_dir, LOW);
      break;

    case MD5_HD14:
      pinMode(pin_cw, OUTPUT);
      pinMode(pin_ccw, OUTPUT);
      pinMode(pin_hold_off, OUTPUT);
      pinMode(pin_div_sel, OUTPUT);

      digitalWrite(pin_cw, LOW);
      digitalWrite(pin_ccw, LOW);
      digitalWrite(pin_hold_off, LOW);
      digitalWrite(pin_div_sel, LOW);
      break;

    default:
      break;
  }
}
void StepMotor::setDriver(int driver_type)
{
  driver = driver_type;
}

void StepMotor::setGear(int _gearRatio)
{
  gearRatio = _gearRatio;
  pulse_delay = calculatorPulseDelay();
}

void StepMotor::setResolution(double microStep)
{
  resolution = microStep;
  pulse_delay = calculatorPulseDelay();
}

// Max=390; rpm res=2; pulse_delay 384.62 pulse
void StepMotor::setMotorSpeed(double _speed)
{
  motorSpeed = _speed;
  pulse_delay = calculatorPulseDelay();
}

void StepMotor::setStepAngle(double _stepAngle)
{
  stepAngle = _stepAngle;
  pulse_delay = calculatorPulseDelay();
}
void StepMotor::setPin(int pPulse, int pDir, int pHoldOff, int pDivSel)
{
  pin_cw = pPulse;
  pin_ccw = pDir;
  pin_hold_off = pHoldOff;
  pin_div_sel = pDir;

  pin_pulse = pPulse;
  pin_dir = pDir;
}

void StepMotor::runByPulse(double nPulse, int dir)
{
  if (nPulse <= 0 || dir < 0)
    return;


  switch (driver)
  {
    case DM542_05:
      enableMotor(true);
      changeMotorDirection(dir);
      for (double i = 0; (i < nPulse) && (enableRun == true); i++)
      {
        pulseMotor(pulse_delay);
      }
      break;

    case MD5_HD14:
#ifdef DEBUG
      Serial.println("runByPulse -> driver = " + String(driver));
      Serial.println("runByPulse -> dir = " + String(dir));
      Serial.println("runByPulse -> nPulse = " + String(nPulse));
      Serial.println("runByPulse -> pulse_delay = " + String(pulse_delay));
#endif
      enableRun = true;
      if (dir == CW)
      {
        digitalWrite(pin_ccw, LOW);
        delayMicroseconds(10);
        for (double i = 0; i < nPulse; i++)
        {
          pulseMotor(pin_cw, pulse_delay);
        }
      }
      else
      {
        digitalWrite(pin_cw, LOW);
        delayMicroseconds(10);
        for (double i = 0; i < nPulse; i++)
        {
          pulseMotor(pin_ccw, pulse_delay);
        }
      }
      break;
    default:
      break;
  }
}
void StepMotor::runByDegree(double nDegree, int dir)
{

  double _step = stepAngle / resolution / gearRatio;
  double nPulse = nDegree / _step;
  pulse_delay = calculatorPulseDelay();

#ifdef DEBUG
  Serial.println("runByDegree -> nPulse = " + String(nPulse));
  Serial.println("runByDegree -> pulse_delay = " + String(pulse_delay));
#endif
  runByPulse(nPulse, dir);
}

void StepMotor::runByRevolution(double nRev, int dir)
{
  enableRun = true;
  double _step = stepAngle / resolution / gearRatio;
  double nPulse = 360 * nRev / _step;
  double freq = (360 * motorSpeed) / (60 * _step);
  pulse_delay = 1000000 / freq; // us
#ifdef DEBUG
  Serial.println("runByRevolution -> _step = " + String(_step));
  Serial.println("runByRevolution -> stepAngle = " + String(stepAngle));
  Serial.println("runByRevolution -> resolution = " + String(resolution));
  Serial.println("runByRevolution -> gearRatio = " + String(gearRatio));
  Serial.println("runByRevolution -> nPulse = " + String(nPulse));
  Serial.println("runByRevolution -> pulse_delay " + String(pulse_delay) + " pulse");
#endif

  if (nRev > 0)
  {
    runByPulse(nPulse, dir);
  }
  else if (nRev < 0)
  {

    while (enableRun)
    {
      runByPulse(10, dir);
    }
  }
}

/*Motor Autonic*/
void StepMotor::pulseMotor(int pin, double time_delay)
{
  //#ifdef DEBUG
  //  Serial.println("runByPulse -> pin = " + String(pin) + ", delay = " + String(time_delay));
  //#endif
  digitalWrite(pin, HIGH);
  delayMicroseconds(time_delay/2);
  digitalWrite(pin, LOW);
  delayMicroseconds(time_delay/2);
}

/*Motor Leadshine*/
void StepMotor::pulseMotor(double time_delay)
{
  digitalWrite(pin_pulse, HIGH);
  delayMicroseconds(time_delay/2);
  digitalWrite(pin_pulse, LOW);
  delayMicroseconds(time_delay/2);
}

void StepMotor::changeMotorDirection(int dir)
{
  enableMotor(true);
  switch (dir)
  {
    case CW:
      digitalWrite(pin_dir, HIGH);
      break;

    case CCW:
      digitalWrite(pin_dir, LOW);
      break;
    default:
#ifdef DEBUG
      Serial.println("Not support dir = " + String(dir));
#endif
      break;
  }
  delayMicroseconds(10);
}

void StepMotor::enableMotor(bool en)
{
  if (en)
  {
    digitalWrite(PIN_ENABLE, HIGH);
  }
  else
  {
    digitalWrite(PIN_ENABLE, LOW);
  }
  delayMicroseconds(10);
}
