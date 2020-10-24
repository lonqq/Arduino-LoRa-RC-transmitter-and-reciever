
void setDefaultModelName();
void setDefaultModelBasicParams();
void setDefaultModelMixerParams();
void setDefaultModelMixerParams(uint8_t _mixNo);

//------------------- SYSTEM PARAMS ----------------------------------------------------------------

enum {
  SOUND_OFF = 0, 
  SOUND_ALARMS, 
  SOUND_NOKEY, 
  SOUND_ALL,
  SOUND_LAST = SOUND_ALL
};

enum {
  BACKLIGHT_OFF = 0, 
  BACKLIGHT_5S, 
  BACKLIGHT_15S, 
  BACKLIGHT_60S, 
  BACKLIGHT_ON,
  BACKLIGHT_LAST = BACKLIGHT_ON
};

struct sysParams {
  uint8_t transmitterID;
  
  uint8_t activeModel = 1; //The current model
  
  bool rfOutputEnabled = false;
  uint8_t soundMode = SOUND_ALL; 
  uint8_t backlightMode = BACKLIGHT_15S;
  uint8_t PWM_Mode_Ch3 = 1; //1 is servo pwm, 0 is ordinary pwm

  int rollMax  = 1023, rollMin  = 0, rollCenterVal = 512;
  int yawMax   = 1023, yawMin   = 0, yawCenterVal = 512;
  int pitchMax = 1023, pitchMin = 0, pitchCenterVal = 512;
  int thrtlMax = 1023, thrtlMin = 0;
  uint8_t deadZonePerc = 5; //in percentage of stick range
} Sys;

//------------------- MODEL PARAMS -----------------------------------------------------------------

#define NUM_PRP_CHANNLES 8  //Number of proportional channels ## Leave this

#define NUM_MIXSLOTS 10     //More slots results into less models and more ram usage 

struct modelParams {
  //------- first entity is the modelName ----
  char modelName[7]; //6chars + Null
  
  //------- basic params ---------
  
  bool Reverse[NUM_PRP_CHANNLES];
  uint8_t EndpointL[NUM_PRP_CHANNLES];   //left endpoint, 0 to 100
  uint8_t EndpointR[NUM_PRP_CHANNLES];   //right endpoint, 0 to 100
  uint8_t Subtrim[NUM_PRP_CHANNLES];     //0 to 100, centered at 50
  uint8_t Failsafe[NUM_PRP_CHANNLES];    //0 to 201. 0 is off, 1 is -100, 101 is 0, 201 is 100
  uint8_t CutValue[NUM_PRP_CHANNLES];    //0 to 201. 0 is off, 1 is -100, 101 is 0, 201 is 100
  
  // Ail, Ele, Rud.
  uint8_t RateNormal[3];   //0 to 100, 100 is 1:1 ie normal
  uint8_t RateSport[3];    //0 to 100, 100 is 1:1 ie normal
  uint8_t ExpoNormal[3];   //0 to 200, 100 is no expo
  uint8_t ExpoSport[3];    //0 to 200, 100 is no expo
  bool DualRateEnabled[3]; 
  
  uint8_t ThrottlePts[5];  //for interpolation of throttle. Range 0 to 200, center is 100
  
  uint8_t Trim[4];         //for Ail, Ele, Thr, Rud inputs. Are percentages. Values 75 to 125, centered at 100.
  
  uint8_t Curve1Src;       //User defined curve
  uint8_t Curve1Pts[5];    //User defined interpolation points. Range 0 to 200, center is 100
  
  uint8_t throttleTimerType;      
  uint8_t throttleTimerInitMins; 
  uint8_t throttleTimerThreshold; // in percentage of throttle stick input: 0 to 100
  
  //------- mixer params ---------
  
  uint8_t MixIn1[NUM_MIXSLOTS];       //index in mix sources array
  uint8_t MixIn1Offset[NUM_MIXSLOTS]; //0 to 200, centered at 100
  uint8_t MixIn1Weight[NUM_MIXSLOTS]; //0 to 200, centered at 100
  uint8_t MixIn1Diff[NUM_MIXSLOTS];   //0 to 200, centered at 100
  
  uint8_t MixIn2[NUM_MIXSLOTS];       //index in mix sources array
  uint8_t MixIn2Offset[NUM_MIXSLOTS]; //0 to 200, centered at 100
  uint8_t MixIn2Weight[NUM_MIXSLOTS]; //0 to 200, centered at 100
  uint8_t MixIn2Diff[NUM_MIXSLOTS];   //0 to 200, centered at 100
  
  uint8_t MixOperator[NUM_MIXSLOTS];  
  uint8_t MixSwitch[NUM_MIXSLOTS];    
  uint8_t MixOut[NUM_MIXSLOTS];       //index in mix sources array
  
} Model; 

enum {
  AILRTE = 0, 
  ELERTE, 
  RUDRTE
};

enum {
  TIMERCOUNTUP = 0, 
  TIMERCOUNTDOWN
};

enum {
  OPERATOR_ADD = 0,
  OPERATOR_MULTIPLY,
  OPERATOR_REPLACE,
  NUM_MIXOPERATORS //should be last
};

enum { 
  //Mixer sources. These are indexes in mix sources array
  //Same order as the name order in the UI
  IDX_ROLL = 0, IDX_PITCH, IDX_THRTL_RAW, IDX_YAW, IDX_KNOB, 
  IDX_SWA, IDX_SWB, IDX_SWC, IDX_SWD,
  IDX_CRV1,
  IDX_AIL, IDX_ELE, IDX_THRTL_CURV, IDX_RUD,
  IDX_NONE, 
  IDX_CH1, IDX_CH2, IDX_CH3, IDX_CH4, IDX_CH5, IDX_CH6, IDX_CH7, IDX_CH8,
  IDX_VRT1, IDX_VRT2,
 
  NUM_MIXSOURCES //should be last
};

enum { //possible values in MixSwitch array
  SW_NONE = 0,
  SWA_UP, SWA_DOWN,
  SWB_UP, SWB_DOWN,
  SWC_UP, SWC_MID, SWC_DOWN, SWC_NOT_UP, SWC_NOT_MID, SWC_NOT_DOWN,
  SWD_UP, SWD_DOWN,
  
  NUM_MIXSWITCHERS //should be last
};

void setDefaultModelName()
{
  uint8_t len = sizeof(Model.modelName)/sizeof(Model.modelName[0]);
  for(uint8_t i = 0; i < len - 1; i++) 
    Model.modelName[i] = ' ';
  Model.modelName[len - 1] = '\0';
}

void setDefaultModelBasicParams()
{
  //reverse, subtrim, endpoints, cut, failsafe
  for(uint8_t i = 0; i < NUM_PRP_CHANNLES; i++)
  {
    Model.Reverse[i]   = false;
    Model.EndpointL[i] = 100;
    Model.EndpointR[i] = 100;
    Model.Subtrim[i]   = 50;
    if(i == 2) //specify cut and failsafe on Channel 3
    {
      Model.CutValue[i]  = 1; 
      Model.Failsafe[i]  = 1;
    }
    else
    {
      Model.CutValue[i]  = 0; 
      Model.Failsafe[i]  = 0;
    }
  }
  
  //trim
  Model.Trim[0] = 100;
  Model.Trim[1] = 100;
  Model.Trim[2] = 100;
  Model.Trim[3] = 100;
  
  //rate and expo, dualrate enabled
  for(uint8_t i = 0; i < 3; i++)
  {
    Model.RateNormal[i] = 100;
    Model.RateSport[i]  = 100;
    Model.ExpoNormal[i] = 100;
    Model.ExpoSport[i]  = 100;
    Model.DualRateEnabled[i] = false;
  }
  
  //throttle curve
  Model.ThrottlePts[0] = 0;
  Model.ThrottlePts[1] = 50;
  Model.ThrottlePts[2] = 100;
  Model.ThrottlePts[3] = 150;
  Model.ThrottlePts[4] = 200;
  
  //user defined curves
  Model.Curve1Src = IDX_KNOB;
  Model.Curve1Pts[0] = 0;
  Model.Curve1Pts[1] = 50;
  Model.Curve1Pts[2] = 100;
  Model.Curve1Pts[3] = 150;
  Model.Curve1Pts[4] = 200;
  
  //throttle timer
  Model.throttleTimerType = TIMERCOUNTUP;
  Model.throttleTimerInitMins = 5; 
  Model.throttleTimerThreshold = 25; 
}

void setDefaultModelMixerParams(uint8_t _mixNo)
{
  Model.MixIn1[_mixNo]        = IDX_NONE; 
  Model.MixIn1Offset[_mixNo]  = 100;
  Model.MixIn1Weight[_mixNo]  = 100;
  Model.MixIn1Diff[_mixNo]    = 100;
  Model.MixIn2[_mixNo]        = IDX_NONE;
  Model.MixIn2Offset[_mixNo]  = 100;
  Model.MixIn2Weight[_mixNo]  = 100;
  Model.MixIn2Diff[_mixNo]    = 100;
  Model.MixOperator[_mixNo]   = 0; 
  Model.MixSwitch[_mixNo]     = SW_NONE;
  Model.MixOut[_mixNo]        = IDX_NONE;
}

void setDefaultModelMixerParams()
{
  for(uint8_t i = 0; i < NUM_MIXSLOTS; i++)
    setDefaultModelMixerParams(i);
}

//---------------------- MISC ----------------------------------------------------------------------

//---Output channels---
int ChOut[NUM_PRP_CHANNLES]; //Proportional Channels. Centered at 0, range is -500 to 500.
uint8_t DigChA = 0, DigChB = 0; /*Digital channels. ChA is momentary, ChB is toggle.*/

int8_t mixerChOutGraphVals[NUM_PRP_CHANNLES];  //for graphing raw mixer output for channels. range -100 to 100

int curve1SrcVal = 0;

//---Sticks---
int rollIn, pitchIn, throttleIn, yawIn, knobIn; //Scaled stick values, range -500 to 500
bool isCalibratingSticks = false;

bool skipThrottleCheck = false;

//---Switches---
//2 position
bool SwAEngaged = false;
bool SwBEngaged = false;
bool SwDEngaged = false;
//3 position
enum {SWUPPERPOS = 0, SWLOWERPOS = 1, SWMIDPOS = 2};
uint8_t SwCState = SWUPPERPOS; 

//---buttons---
uint8_t buttonCode = 0; 
enum {
  SELECT_KEY = 1, 
  UP_KEY, 
  DOWN_KEY,
};

//Button events
#define LONGPRESSTIME 350
uint8_t pressedButton = 0; //triggered once when the button goes down
uint8_t clickedButton = 0; //triggered when the button is released before heldButton event
uint8_t heldButton = 0;    //triggered when button is held down long enough

unsigned long buttonStartTime = 0;
unsigned long buttonReleaseTime = 0;

//---Battery---
int battVoltsNow; //millivolts
enum{BATTLOW = 0, BATTHEALTY};
uint8_t battState = BATTHEALTY;

enum{  
  AUDIO_NONE = 0, 
  AUDIO_BATTERYWARN, AUDIO_THROTTLEWARN, AUDIO_TIMERELAPSED,
  AUDIO_SWITCHMOVED, AUDIO_TRIMSELECTED,
  AUDIO_KEYTONE      
};
uint8_t audioToPlay = AUDIO_NONE;

//--- EEPROM
//Address 0 is used to store eeprom init flag
const uint8_t eeSysDataStartAddress = 1; 
uint8_t eeModelDataStartAddress; //recalculated in setup()
uint8_t numOfModels;  //recalculated in setup()

//-----------
bool bindActivated = false;

//---etc-----
uint8_t returnedByte; //got from slave mcu

bool showPktsPerSec = false;

const uint8_t fixedLoopTime = 25; /*in milliseconds. This should be made greater than the time it 
takes for the radio module to transmit the entire packet or else the window is missed resulting in
much less throughput*/

uint32_t thisLoopNum = 0; //main loop counter. For timing certain things
