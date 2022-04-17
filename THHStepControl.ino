/*
  Serial Event example


*/


#include "DefSystem.h"
#include "StringSplitter.h"
#include "StepMotor.h"
#include <SimpleKalmanFilter.h>


StringSplitter *splitter;
StepMotor *stepMotor;
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);
// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

int mode_process = eMain_Do_nothing;
int old_mode_process = eMain_Do_nothing;


int motor_direction = CW;
double motor_microstep = 2;
double motor_num_pulse = 1;
double motor_num_degree = 1;
double motor_num_revolution = 1;
double motor_rpm = 30;
double adc_speed = 0;

double RevTest = 1;
double nPulseRun = 0;
bool runForever = false;


void setup() {
  // initialize serial:
  Serial.begin(9600);

  inputString.reserve(200);   // reserve 200 bytes for the inputString:
  pinMode(LED_BUILTIN, OUTPUT);

  stepMotor = new StepMotor(DM542_05);
  splitter = new StringSplitter();

  Timer1.initialize(390);         // initialize timer1 with T=120us
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}

void loop() {
  adcSpeed();
  processCommand();
  processMotor();

}

//int i  = 0;
void callback()
{



  if (runForever)
    return;

#ifndef DEBUG
  Serial.println("runForever = " + String(runForever));

#endif

  if (nPulseRun >= 0)
  {
    nPulseRun --;
    if (nPulseRun == 0)
    {
      stepMotor->stopMotor();
    }
  }
  //  else
  //  {
  //    stepMotor->stopMotor();
  //  }
}
void processMotor()
{
  switch (mode_process)
  {
    case eMain_Do_nothing:
      break;

    case eMain_Change_direction:
      delay(1000);
      nPulseRun = stepMotor->runByRevolution(RevTest, motor_direction);
      mode_process = eMain_Do_nothing;
      break;

    case eMain_Run_pulse:
      nPulseRun = stepMotor->runByPulse(motor_num_pulse, motor_direction);
      mode_process = eMain_Do_nothing;
      break;

    case eMain_Run_degree:
      nPulseRun = stepMotor->runByDegree(motor_num_degree, motor_direction);
      mode_process = eMain_Do_nothing;
      break;

    case eMain_Run_revolution:
      if (motor_num_revolution < 0)
      {
        runForever = true;
      }
      else
      {
        runForever = false;
      }
      nPulseRun = stepMotor->runByRevolution(motor_num_revolution, motor_direction);
      mode_process = eMain_Do_nothing;
      break;


    case eMain_Change_speed:
      stepMotor->setMotorSpeed(motor_rpm);
      nPulseRun = stepMotor->runByRevolution(RevTest, motor_direction);
      mode_process = eMain_Do_nothing;
      break;


    case eMain_Change_resolution:
      stepMotor->setResolution(motor_microstep);
      nPulseRun = stepMotor->runByRevolution(RevTest, motor_direction);
      mode_process = eMain_Do_nothing;
      break;

  }
}

bool enbaleADCSpeed = true;

void adcSpeed()
{
  if (!enbaleADCSpeed)
    return;

  // read a reference value from A0 and map it from 0 to 100
  float real_value = analogRead(A0) / 1024.0 * 100.0;
  // calculate the estimated value with Kalman Filter
  adc_speed = simpleKalmanFilter.updateEstimate(real_value) / 100 * stepMotor->getMaxSpeed();


#ifndef DEBUG
  Serial.println("adc_speed = " + String(adc_speed));

#endif


  if (motor_rpm >= adc_speed - offset && motor_rpm <= adc_speed + offset)
  {
    //do nothing
    //keep old speed

#ifndef DEBUG
    Serial.println("adc_speed = " + String(adc_speed));
    Serial.println("eMain_Change_speed_adc, keep = " + String(motor_rpm));
#endif
  }
  else
  {
    //update speed
    motor_rpm = adc_speed;
    stepMotor->setMotorSpeed(motor_rpm);

    Serial.println("mode_process = " + String(mode_process));

    runForever = true;

    nPulseRun = stepMotor->runByPulse(RevTest, motor_direction);


  }

}

void processCommand()
{
  // print the string when a newline arrives:
  if (!stringComplete)
    return;


#ifdef DEBUG
  Serial.println(inputString);
#endif

  // mode_param1_param2_..._paramn\n
  // split data
  splitter->SplitString(inputString, '_', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
  if (itemCount == 0)
  {
    clear_buffer();
    return;
  }

#ifdef DEBUG
  Serial.println("Item count: " + String(itemCount));
  for (int i = 0; i < itemCount; i++) {
    String item = splitter->getItemAtIndex(i);
    Serial.println("Item @ index " + String(i) + ": " + String(item));
  }
#endif

  //consider mode by 1st param
  String cmd_item = splitter->getItemAtIndex(0);

  //chieu quay
  if (cmd_item.equalsIgnoreCase(ROTATION))
  {
    mode_process = eMain_Change_direction;
    int dir = splitter->getItemAtIndex(1).toInt();
    motor_direction = dir;
#ifdef DEBUG
    Serial.println("motor_direction = " +  String(motor_direction));
#endif

  }//end chieu quay
  
  //che do quay
  if (cmd_item.equalsIgnoreCase(RUN))
  {
    //quay n xung
    if (splitter->getItemAtIndex(1).equalsIgnoreCase(RUN_PULSE))
    {
      mode_process = eMain_Run_pulse;
      motor_num_pulse = splitter->getItemAtIndex(2).toDouble();
#ifdef DEBUG
      Serial.println("eMain_Run_pulse = " + String(motor_num_pulse));
#endif
    }

    //quay n do
    else if (splitter->getItemAtIndex(1).equalsIgnoreCase(RUN_DEGREE))
    {
      mode_process = eMain_Run_degree;
      motor_num_degree = splitter->getItemAtIndex(2).toDouble();
#ifdef DEBUG
      Serial.println("eMain_Run_degree = " + String(motor_num_degree));
#endif
    }

    //quay n vong
    if (splitter->getItemAtIndex(1).equalsIgnoreCase(RUN_REVOLUTION))
    {
      mode_process = eMain_Run_revolution;
      motor_num_revolution = splitter->getItemAtIndex(2).toDouble();
#ifdef DEBUG
      Serial.println("eMain_Run_revolution = " + String(motor_num_revolution));
#endif
    }



  }//end---che do quay

  //toc do quay
  if (cmd_item.equalsIgnoreCase(SPEED))
  {
    mode_process = eMain_Change_speed;
    motor_rpm = splitter->getItemAtIndex(1).toDouble();

#ifdef DEBUG
    Serial.println("eMain_Change_speed = " + String(motor_rpm));
#endif
  }//end - toc do quay

  //thay doi do phan giai
  if (cmd_item.equalsIgnoreCase(RESOLUTION))
  {
    mode_process = eMain_Change_resolution;
    motor_microstep = splitter->getItemAtIndex(1).toDouble();

#ifdef DEBUG
    Serial.println("eMain_Change_resolution = " + String(motor_microstep) + " - microstep");
#endif
  }//end do phan giai

  //cac truong hop khac
//  else
//  {
//    mode_process = eMain_Do_nothing;
//#ifdef DEBUG
//    Serial.println("mode_process = eMain_Do_nothing");
//#endif
//  }

  clear_buffer();

}

void clear_buffer()
{

  // clear the string:
  inputString = "";
  stringComplete = false;
  //delete splitter;
  //splitter = NULL;

}
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  //stepMotor->enableRun = false;
  while (Serial.available()) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    //
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {

      stringComplete = true;
      mode_process = eMain_Do_nothing;
    }
  }
}
