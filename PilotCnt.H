
#define BIGPILOT 1000       // used when a large pilot press needs to be assigned
#define PILOTTYPES 6        // Number of methods for calculating Pilot Pressure  




// There are 6 different methods to calculate what the pilot pressure should be at
// any given time.  Some of the methods simply use a look-up table referenced to 
// a specific system variable, and some of them use a more complicated algorithm.
   
// The controller continuously calculates values of pilot pressure, using each
// method.  It then evaluates which of the five is the lowest, and sets the 
// pilot pressure to that value.
   
// The following structure allocates memory to store each of these potential
// pilot pressures.
  typedef struct {
   unsigned short Basic;     // Base Pilot Curve
   unsigned short Accel;     // Pilot called for by Accelerator Throttle-Pot
   unsigned short Power;     // Pilot called for to implement Power Adaption
   unsigned short Brake;     // Pilot called for by Brake Pressure Sensor                
   unsigned short Temp;      // Pilot called for by Loop Temperature Sensor
   unsigned short OverSpeed; // Pilot called for to implement OverSpeed Adaption
                 } PilotNameType;  
                 

                 
// This union provides two methods of referring to the alternative pilot pressure
// options.  They may be referenced by name, or by using an array.  The array
// method is handy for sorting algorithms, to determine which value is smallest. 
  typedef union      {          
       PilotNameType   Name;      // refer to values by name
       unsigned short  Value[PILOTTYPES];  // or by array   
                     } PilotType;   // fractured PilotNameType
                     


                     
// Power Adapt Gains
static unsigned short paKp;    // Power Adapt PID routine Proportional Gain
static unsigned short paKi;    // Power Adapt PID routine Integral Gain
                     
static PilotType Pilot;        // used to hold all Pilot Pressure Solutions
static unsigned short PilotSP; // pilot pressure controller is ramping towards 
static unsigned short PilotOut;   // Amount of Pilot Pressure needed at this instant
static unsigned char  PilotDuty;  // Duty Cycle including Dither offset  

static unsigned char  IgnoreRPM;  // used to determine whether pilot should be limited based on RPM


static unsigned short RPMAnticipateVariable;
static unsigned short AccelRefReduction;  // Amount to reduce Accel Ref Pilot during Throttle-Up
static unsigned short PctTUCorrection;    // Percent of full correction to use on AccelRef for TU Compensation
static unsigned short PctAccelRef;  // Pct of Accel Reference to use

static unsigned short TUCorrectedPilot;   // Pilot Pressure necessary to maintain travel speed at 2200 RPM
/*
static signed short   TUCorrectedPilot;   // Pilot Pressure necessary to maintain travel speed at FULLGOVSPD RPM

static float m_sc1;   // slope of curve fit, used to calculate speed compensation
static float b_sc1;   // intercept of curve fit, used to calculate speed compensation
static float m_sc2;   // slope of curve fit, used to calculate speed compensation
static float b_sc2;   // intercept of curve fit, used to calculate speed compensation

static float MPH_SP; // Speed we are trying to maintain, during throtle-up
*/ 

static unsigned short PctBasePilot;    // Pct of Base Pilot to use.  Used if Low Charge Pressure


// Variables used in PIDPowerAdapt
static signed short paCV;  // Control Variable for PIDPowerAdapt
static signed short paCVi; // Integral part of Control Variable for PIDPowerAdapt
static signed short paCVp; // Proportional part of Control Variable for PIDPowerAdapt

//static signed long paCV;  // Control Variable for PIDPowerAdapt
//static signed long paCVi; // Integral part of Control Variable for PIDPowerAdapt
//static signed long paCVp; // Proportional part of Control Variable for PIDPowerAdapt

static unsigned short paRPM_SP;  // RPM now attempting to control engine to

static signed   short RPMError;      // Difference between RPM and RPM Setpoint
static unsigned short fRPM_SP;       // RPM we would expect engine to be at, low pass filtered to simulate actual engine response in time
static unsigned short fRPM_SP_Inc;   // Increment/Decrement for fRPM_SP
static unsigned short fRPM_SPFinal;  // basically, just RPMSetpoint[PedalIndex] = no load RPM

static unsigned short RPM_SPIndex; // Speed of engine expected divided by 10  




static unsigned char  PilotCounter;   // used in PickPilot function counter   
static unsigned short LowestPilot;     // Variable used to pick lowest of all pilot solutions
static unsigned short ModifiedPilot;   // Variable used modify pilot based on temperature compensation

static unsigned short TempCompMultiplier; // multiplier between 67 and 124 to temp compensate pilot
static unsigned short TempCompPilot;    // New Pilot Pressure, compensated to temperature. 

static signed short   RPMCompPilot;    // New Pilot Pressure, compensated to RPM variance from RPMSetpoint

//static unsigned char DitherUp;  // Keeps track of whether we're dithering up or down

static unsigned short ChargePressMultiplier;  // Inhibits Pilot during low charge 
  
static unsigned short TemperaturePilot;  // Pilot Pressure because of Truck Temperature
static unsigned char  OTempMode1;  // Used in Hysteresis Control of Pilot based on Temperature
static unsigned char  OTempMode2;  // Used in Hysteresis Control of Pilot based on Temperature

static unsigned short BrakeSPFinal;  // used to ramp brake pressure
static unsigned short BrakeSP;  // used to ramp brake pressure
static unsigned short Brake_SP_Inc;

static unsigned short OverSpeedPilotSP;  // used to ramp overspeed pilot pressure
static unsigned short OverSpeedPilot_SP_Inc;  // used to ramp overspeed pilot pressure



// Variables used to control transient reponse of truck
//#define FilterSize 20 // size of arrays used to low pass filter system energy variables
//static unsigned short RPMFilter[FilterSize];   // used to low pass filter RPM for transient control
//static unsigned short PilotFilter[FilterSize]; // used to low pass filter Pilot for transient control
//static unsigned char  FilterCounter1; // used to index OldRPM & OldPilot when adding data
//static unsigned char  FilterCounter2; // used to index OldRPM & OldPilot when averaging data
//static unsigned long  RPMFiltered;   // low pass filtered value of RPM
//static unsigned long  PilotFiltered; // low pass filtered value of Pilot


