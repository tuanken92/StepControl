#include "Arduino.h"
#include "StepMotor.h"


StepMotor::StepMotor(int _driver)
{
  initVar(_driver);
}

double StepMotor::calculatorPulseDelay()
{
  double _step = stepAngle / resolution / gearRatio;

  double freq = (360 * motorSpeed) / (60 * _step);
  double _pulse_delay = 1000000 / freq; // us
  //Timer1.setPeriod(_pulse_delay / 2);
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

void StepMotor::initVar(int _driver)
{

  pinMode(pin_pulse, OUTPUT);
  pinMode(pin_dir, OUTPUT);
  digitalWrite(pin_pulse, HIGH);
  digitalWrite(pin_dir, LOW);

  pin_pulse = 10;
  pin_dir = 9;
  driver = _driver;

  switch (driver)
  {
    case DM542_05:
      Timer1.disablePwm(pin_dir);
      gearRatio = 1;
      resolution = 2;
      stepAngle = 1.8;
      motorSpeed = 80;
      maxSpeed = 160;
      pulse_delay = calculatorPulseDelay();
      break;
    case MD5_HD14:
      gearRatio = 10;
      resolution = 1;
      stepAngle = 0.72;
      motorSpeed = 30;
      maxSpeed = 30;
      pulse_delay = calculatorPulseDelay();
      break;

    default:
      break;
  }
}

void StepMotor::setDriver(int driver_type)
{
  initVar(driver_type);
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
  if (_speed <= 0)
  {
    return;
  }

  if (_speed >= maxSpeed)
  {
    _speed = maxSpeed;


  }
  motorSpeed = _speed;

#ifdef DEBUG
  Serial.println("Calculator -> driver = " + String(driver));
  Serial.println("Calculator -> motorSpeed(rpm) = " + String(motorSpeed));
#endif
  pulse_delay = calculatorPulseDelay();
}

void StepMotor::setStepAngle(double _stepAngle)
{
  stepAngle = _stepAngle;
  pulse_delay = calculatorPulseDelay();
}


double StepMotor::getMaxSpeed()
{
  return maxSpeed;
}

double StepMotor::runByPulse(double nPulse, int dir)
{
  //  if (nPulse <= 0 || dir < 0)
  //    return 0;


  switch (driver)
  {
    case DM542_05:

      changeMotorDirection(dir);
      Timer1.pwm(pin_pulse, duty, pulse_delay / 2);
      break;

    case MD5_HD14:
#ifdef DEBUG
      Serial.println("runByPulse -> driver = " + String(driver));
      Serial.println("runByPulse -> dir = " + String(dir));
      Serial.println("runByPulse -> nPulse = " + String(nPulse));
      Serial.println("runByPulse -> pulse_delay = " + String(pulse_delay));
#endif

      if (dir == CW)
      {
        Timer1.disablePwm(pin_dir);
        digitalWrite(pin_dir, LOW);
        delayMicroseconds(10);
        Timer1.pwm(pin_pulse, duty, pulse_delay / 2);
      }
      else
      {
        Timer1.disablePwm(pin_pulse);
        digitalWrite(pin_pulse, LOW);
        delayMicroseconds(10);
        Timer1.pwm(pin_dir, duty, pulse_delay / 2);
      }
      break;
    default:
      break;
  }

  return nPulse;
}
double StepMotor::runByDegree(double nDegree, int dir)
{

  double _step = stepAngle / resolution / gearRatio;
  double nPulse = nDegree / _step;
  pulse_delay = calculatorPulseDelay();
  //Timer1.setPeriod(pulse_delay / 2);
#ifdef DEBUG
  Serial.println("runByDegree -> nPulse = " + String(nPulse));
  Serial.println("runByDegree -> pulse_delay = " + String(pulse_delay));
#endif

  return runByPulse(nPulse, dir);
}

double StepMotor::runByRevolution(double nRev, int dir)
{
  double _step = stepAngle / resolution / gearRatio;
  double nPulse = 360 * nRev / _step;
  double freq = (360 * motorSpeed) / (60 * _step);
  pulse_delay = 1000000 / freq; // us
  //Timer1.pwm(pulse, duty, pulse_delay / 2);
#ifdef DEBUG
  Serial.println("runByRevolution -> _step = " + String(_step));
  Serial.println("runByRevolution -> stepAngle = " + String(stepAngle));
  Serial.println("runByRevolution -> resolution = " + String(resolution));
  Serial.println("runByRevolution -> gearRatio = " + String(gearRatio));
  Serial.println("runByRevolution -> nPulse = " + String(nPulse));
  Serial.println("runByRevolution -> pulse_delay " + String(pulse_delay) + " pulse");
#endif


  return runByPulse(nPulse, dir);
}



void StepMotor::changeMotorDirection(int dir)
{
  Timer1.disablePwm(pin_dir);
  
  //stopMotor();
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

#ifdef DEBUG
  Serial.println("Dicrection = pin" + String(pin_dir) + " = " + digitalRead(pin_dir));
#endif
  delayMicroseconds(1000);
}

void StepMotor::stopMotor()
{
  Timer1.disablePwm(pin_dir);
  Timer1.disablePwm(pin_pulse);
#ifdef DEBUG
  Serial.println("stopMotor = true");
#endif
}
