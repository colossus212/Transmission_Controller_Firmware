
// Throttle Motor PWM Constants

  
 #define TMPWMCounts 250       // uses hardware PWM
 
 
 // Throttle Motor RPM Control PID Parameters
 unsigned short tKp;            // Throttle Motor RPM Control routine Proportional Gain
 unsigned short tKi;            // Throttle Motor RPM Control routine Integral Gain
 unsigned short tKFF;           // Throttle Motor RPM Control routine FeedForward Gain
 unsigned short TiltSP;         // Tilt & Auxilliary Function RPM Setpoint
 unsigned short Throttle_SPInc; // RPM/sec ramp rate of RPM Setpoint

 

// Variables used in TM_PID
static unsigned short Throttle_SP;      // RPM to attempt to control engine to
static unsigned short Throttle_SPFinal; // RPM to ramp Throttle_SP towards
static unsigned short Throttle_SPx7;    // RPM to control pilot to, for pilot compensation

static signed short tCV;  // Control Variable for TM_PID
static signed short tFF;  // FeedForward term
static float tCVi; // Integral part of Control Variable for TM_PID
static float tCVp; // Proportional part of Control Variable for TM_PID
static signed short ThrottleError; // Difference between RPM and Throttle_SP
static unsigned char ThrottleUpActive;  // Flag to tell program if throttling up
static unsigned char TUFuncWasTilt;  // Used to not use speed compensation during/after a tilt operation 

// Throttle Motor PWM 
static unsigned short ThrottleOnTime;  // 0->13107 On-Time of Throttle Motor PWM
static unsigned short ThrottleOffTime; // 0->13107 Off-Time of Throttle Motor PWM
static unsigned char  ThrottlePWMPin;  // Remember state of PWM Pin

static unsigned short TMPWM_Timer;     // Value of TCNT when OC4 is first called
static unsigned short TMPWM_Reg;       // Register Value to store in OC4 register



