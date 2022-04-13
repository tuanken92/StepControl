#ifndef DEFSYSTEM_H
#define DEFSYSTEM_H

/*atk*/
#define DEBUG true

#define ROTATION "rotation"
#define DIR_CW "ok"
#define DIR_CCW "ng"


#define RUN "run"
#define RUN_PULSE "pulse"
#define RUN_DEGREE "degree"
#define RUN_REVOLUTION "rev"

#define SPEED "speed"
#define SPEED_ADC "speed-adc"


#define RESOLUTION "resolution"

#define MODE_ONE_PULSE true
#define MODE_TWO_PULSE false


enum EDriver
{
  NOT_DEFINE,
  MD5_HD14, //Autonis
  DM542_05 //Leadshine
};


enum EDirection
{
  CW, //quay thuan
  CCW //quay nghich
};
enum EMainProcess
{
  eMain_Do_nothing,             //ORG State
  
  eMain_Change_direction,       //Change direction cw. ccw

  
  eMain_Change_modeRun,       //Change direction cw. ccw
  eMain_Run_pulse,           //Run by pulse -> then stop
  eMain_Run_degree,          //Run by degree -> then stop
  eMain_Run_revolution,      //Run by revolution -> then stop
  eMain_Run_forever,         //Run forever
  eMain_Run_hold,             //Run mode hold - like stop
  
  eMain_Change_speed,        //Change speed
  eMain_Change_speed_adc,    //Change speed by adc value
  
  eMain_Change_resolution   //Change resolution pulse/rev
      
};




/*atk*/
//__________________________________________________________________________
// X0 ~ X15
const int PIN_PULSE = 7;
const int PIN_DIRECTION = 6;
const int PIN_ENABLE = 5;
const int PIN_HOLD = 4;
const int PIN_DIVISION_SELECT = 3;


//__________________________________________________________________________                    
// OUTPUT DEFINE  
//__________________________________________________________________________
// Y0 ~ Y7
const int OUT_PULSE = 7;
const int OUT_DIRECTION = 6;
const int OUT_HOLD = 5;
const int OUT_DIVISION_SELECT = 4;



#endif // DEFSYSTEM_H
