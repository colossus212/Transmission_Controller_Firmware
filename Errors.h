// ***** Events less than 20 are logged, but do not cause a mode change.   *****
// ***** These conditions do not light the fault light                     *****

#define LOG_HOURSET      1   // Hour Meter has been modified externally
#define LOG_FIELDCAL     2   // The operator performed a field calibration
#define LOG_NEWPARAM     3   // New Parameters were downloaded into controller
#define LOG_EVENTSCLEAR  4   // Event Log of controller  was cleared



// ***** Errors 20 to 29  are logged, but do not DIRECTLY cause a mode change  *****
// ***** These conditions DO light the fault light                             *****
// ***** ... BUT ONLY as long as the condition persists                        *****

#define TEMPLO_ERR      21   // Loop Temperature Lo, (possible false-reversing)
#define TEMPHI_ERR      23   // Loop Temperature Hi
#define FUSEBLOWN_ERR   25   // 10 Amp fuse is blown


// ***** Errors 30 to 39 are logged, but do not DIRECTLY cause a mode change  *****
  // ***** These conditions DO light the fault light                            *****
  // ***** ... which will remain lit until truck power is cycled                *****

#define CHARGELO_ERR    35   // Charge Pressure Lo



// ***** Errors 40 to 49 cause LIMP HOME MODE                  *****
// ***** These conditions DO light the fault light             *****
// ***** ... which will remain lit until truck power is cycled *****

#define CHARGESF_ERR    41   // Charge Pressure Sensor Failure
#define PEDALSF_ERR     42   // Throttle Pedal Sensor Failure
#define TEMPSF_ERR      43   // Oil Temperature Sensor Failure
#define BRAKESF_ERR     44   // Brake Pressure Sensor Failure
#define RETARD_ERR      45   // Retard Valve not functioning properly
#define RPMSF_ERR       46   // Tachometer Sensor Failure


// ***** Events 50 to 59 cause DISABLE THROTTLE MODE           ***** 
// ***** These conditions DO light the fault light             *****
// ***** ... which will remain lit until truck power is cycled *****

#define HOISTSF_ERR     51   // Hoist Lever Pot Sensor Failure
#define RACKSF_ERR      52   // Rack Pot Sensor Failure   


// ***** Errors 60 to 79 cause EMERGENCY NEUTRAL MODE          *****
// ***** These conditions DO light the fault light             *****
// ***** ... which will remain lit until truck power is cycled *****

#define SOLCURHI_ERR    61   // Solenoid Current Hi
#define THRMCURHI_ERR   62   // Throttle-Motor PWM Current Hi 
#define FSWRSWSF_ERR    63   // Forward Sw/Reverse Sw Sensor Failure
#define THRTLFETS_ERR   64   // Throttle Motor MosFET Failed Short    
#define RPMHIHI_ERR     65   // Engine Speed HiHi


// ***** Errors 80 to 89 cause EMERGENCY SHUTDOWN MODE         ***** 
// ***** These conditions DO light the fault light             *****
// ***** ... which will remain lit until truck power is cycled *****  

#define TEMPHIHI_ERR    82   // Loop Temperature HiHi for 30 Seconds

// ***** Events 90 to 99 cause Emergency Shutdown Mode w/ NO RESTART ALLOWED ***** 
#define CHARGELOLO_ERR  91   // Charge Pressure LoLo


// ***** Events 100 on are used for development diagnostics *****
#define SPUR_ERR       101   // Spurious Interrupt Occurred
#define DIV0_ERR       102   // Division by Zero Occurred
#define BUSERR_ERR     103   // Bus Error Occurred
#define ILLEGAL_ERR    104   // Illegal Instruction
#define MISCINT_ERR    105   // A non-defined interrupt occurred                                          
#define TRANSMIT_ERR   106   // A serial transmission error occurred
#define RECEIVE_ERR    107   // A serial receive error occurred





#define EVENT_BUFF_SIZE 60  // 512 byte in EEPROM, at 8 bytes per record = 64-2 = 62


// Event Buffer Related Variables
//.Most events are considered errors, but this is called an event log rather than
// an error log, so that it may be used for diagnostic purposes by logging non-error
// type events, should the need arise.
typedef struct {
  unsigned short Hours;     // Hour-meter reading when event occured
  unsigned short Seconds;   // Second-meter reading when event occured
  unsigned char  Event;     // Event code.  Events are usually errors
               }  EventLogType;               
               

static EventLogType   EventBuff[EVENT_BUFF_SIZE]; // Array for holding error events     
static EventLogType   HourMeterRecord;    // Keep track of hourmeter and last error #
static unsigned char  EventCode;          // Current Event Code
static unsigned char  TempEventCode;      // Temp Holding Var for Current Event Code
static unsigned short EventNumber;        // Position in the Event Buffer
static unsigned short EventSearchCounter; // Used to search event buffer
static unsigned short EventSearchHours;   // Used to limit search of event buffer
static unsigned char  Event_Logged;       // Limit duplicate entries in event buffer
static unsigned short Events_Searched;    // Number of events searched


void AddEvent(unsigned char EventCodeParam);  // function prototype

static unsigned char EventCode_TensDigit; // Tens Digit of EventCode for FlashCode
static unsigned char EventCode_OnesDigit; // Ones Digit of EventCode for FlashCode






