
// Every timer will use this structure for storage of timer data
// Timers time in hundredths of seconds, using a sixteen bit register
// so timers can time from 0.01 seconds to 655.35 seconds

typedef struct {
   unsigned int EN    :1,  // 1 => Timer allowed to increment accumulated time
                R     :1,  // 1 => Accumulated time set to zero, if OR was 0
                DN    :1,  // 1 => Accumulated time has reached Preset                
                OR    :1,  // internally used to track state of Reset bit  
                TT    :1,  // 1 => Timer Timing:(Enabled,Accum not to Preset yet)
                Unused:3,  // unused part of timer control byte                 
                PRESET:16, // number of hundredths of seconds to time
                ACCUM :16; // accumulated number of hundredths of seconds
                 } TimerType; 

static TimerType  Timer[NUM_TIMERS];  // Array of Timers
static unsigned char tti; // counter for counting through all timers in use

// Time Related Variables.  These are also used in the Event Buffer
static unsigned short Hours;   // Current Hour Meter reading
static unsigned short Seconds; // Current Seconds Meter reading
static unsigned short TMSecs;  // Counter used to count to 1 second

static unsigned char  LatchCPUON; // Used to keep power from cycling on/off on/off


static unsigned char  LittleTick; // 25th Second Tick (on for 40ms, off for 40ms)
static unsigned char  ESecTick; // Eighth Second Tick(on for 120ms, off for 120ms)

static unsigned char  PedalOver15Pct;  // Determine if pedal is over 15% w / hyster.

static unsigned char JustShiftedForward;  // shifted forward within last 0.3 secs
static unsigned char JustShiftedReverse;  // shifted reverse within last 0.3 secs
static unsigned char RecentlyShiftedForward; // shifted forward within last 4 secs
static unsigned char RecentlyShiftedReverse; // shifted reverse within last 4 secs
  
static unsigned char PowerReversalStart; // detects rapid change of fwd/rev lever
static unsigned char PowerReversalMode;  // flags condition of power reversal

static unsigned long PowerReversalRamp;  // Ramp Rate psi/sec for power reversal

static unsigned char CalParkSwitchCount;  // Used to enter cal mode, counts number of park brake on's

static unsigned char TD;  // TensDigit of EventCode being flashed
static unsigned char OD;  // Ones Digit of EventCode being flashed
static unsigned char FlashedTens; // Flag to keep place in event flash state machine
static unsigned char DigitWaited; // Flag to keep place in event flash state machine
static unsigned char FlashedOnes; // Flag to keep place in event flash state machine
static unsigned char EventWaited; // Flag to keep place in event flash state machine
static unsigned char FaultLight; // On when Fault Light is on during flash codes


  





