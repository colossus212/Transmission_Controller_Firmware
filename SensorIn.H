
#define RPMCOUNTS 180   // number of pulses from tach to use in average RPM calcs



  typedef struct  {
   unsigned int KEY_SWITCH :1,   // Key Switch
                FWDSW      :1,   // Forward Switch
                REVSW      :1,   // Reverse Switch               
                PEDALSW    :1,   // Throttle Pedal Switch                
                TILTSW     :1,   // Lever Switches                
                RPM        :1,   // Speed Sensor                
                WSLEFTA    :1,   // Left Wheel Speed Channel A  
                WSLEFTB    :1,   // Left Wheel Speed Channel B 
                WSRIGHTA   :1,   // Right Wheel Speed Channel A 
                WSRIGHTB   :1,   // Right Wheel Speed Channel B                
                PARKSW     :1,   // Park Brake Switch                
                HOISTSW    :1,   // Hoist Pressure Switch                          
                SPAREIN    :1,   // Spare Digital Input 1
                UNUSED1    :1,   // Not used in structure
                UNUSED2    :1,   // Not used in structure              
                NEVERUSE   :1;   // Must not use this to prevent legit 65000, which indicates comm error
                    } DigInputs_type;

  typedef union      {   
       unsigned int   whole; // this allows programmer access to entire integer
       byte_type      byte;  // or each byte of it (byte_type is defined in strcturs.h)
       DigInputs_type i;     // or each bit of it, as each input is a bit    
           } fracDigInputs;  // fractured DigInput type

  static fracDigInputs ExternalInput; // Place inputs in a word, for transmission to user interface                    
                    

  typedef struct  {
   unsigned int FAULT      :1,   // Fault Light 
                OVRSPD     :1,   // Overspeed Light
                OVRTEMP    :1,   // Over Temp Lamp                            
                STARTEN    :1,   // Start Enable 
                ENGRLY     :1,   // Engine Cutoff Relay
                PILOTEN    :1,   // Pilot Enable Solenoid   
                RETARD     :1,   // Retard Solenoid
                GOVPWM     :1,   // Governor Motor pseudo PWM output
                AUXPWM     :1,   // Aux pseudo PWM output
                THRTFET    :1,   // Throttle Motor PWM FET
                UNUSED1    :1,   // Not used in structure
                UNUSED2    :1,   // Not used in structure   
                UNUSED3    :1,   // Not used in structure              
                UNUSED4    :1,   // Not used in structure   
                UNUSED5    :1,   // Not used in structure   
                NEVERUSE   :1;   // Must not use this to prevent legit 65000, which indicates comm error
                    } DigOutputs_type;                     
  
  
   typedef union      {   
       unsigned int    whole; // this allows programmer access to entire integer
       byte_type       byte;  // or each byte of it (byte_type is defined in strcturs.h)
       DigOutputs_type o;     // or each bit of it, as each input is a bit    
           } fracDigOutputs; // fractured DigInput type            
                    
  typedef struct {
    fracDigOutputs Command;  // State CPU is attempting to put output
    fracDigOutputs Sensed;   // Actual state of output
    fracDigOutputs Auto;     // State requested by control program
    fracDigOutputs Manual;   // State requested by external monitor
    fracDigOutputs Mode;     // 0=>Auto, 1=> Manual
                 }  ExternalDigOutput_type;  
  
                               
                    
  static ExternalDigOutput_type ExtDigOut; // Place outputs in a word, for transmission to/from user interface                    
                     
                    

                 
  
  typedef struct {
   unsigned int PILOTSOL   :8,  // Pilot Solenoid
                THRTLMTR   :8;  // Throttle Motor
                 } AnalogOut_type;
                 
                 
  typedef union      {   
       unsigned int    whole;   // this allows programmer access to entire integer
       AnalogOut_type      o;   // or each section of it       
           } fracAnaOutputs;    // fractured AnalogOut_type 
           
  typedef struct {
    fracAnaOutputs Command;  // State CPU is attempting to put output
    fracAnaOutputs Sensed;   // Actual state of output
    fracAnaOutputs Auto;     // State requested by control program
    fracAnaOutputs Manual;   // State requested by external monitor
    fracAnaOutputs Mode;     // 0=>Auto, 1=> Manual
                 }  ExternalAnaOutput_type;
                 
  static ExternalAnaOutput_type ExtAnlgOut;  // Place outputs in a word, for transmission to/from user interface
                 
                       



static unsigned char  Raw[8];         // read direct from A/D Channels 
static unsigned char  UnScaled[ANALOG_CHANS];    // unscaled sensor values
static unsigned short Scaled[ANALOG_CHANS];      // scaled sensor values
static unsigned char  SensorFail[ANALOG_CHANS];  // sensor error flags


static unsigned char CSi;  // counter used in CheckSensors();


// Analog Sensor Data
static unsigned short Rack;         // Raw value from Rack Pot
static unsigned short RackIndex;    // Percent stroke of rack
static unsigned long  RackRPML;     // Position of Rack, scaled into RPM
static unsigned short RackRPM;
static unsigned short Pedal;       // Raw value from A/D of Accel Pedal Pot
static unsigned short  BrakePress;    // scaled Brake Pressure [psi]
static unsigned short  LoopTemp;      // scaled Loop Temperature [degF] 0-250
static unsigned short  ChargePress;   // scaled Charge Pressure [psi]


static unsigned short ThrCurrent;   // scaled Throtle Motor PWM Current [mA]
static unsigned short SolCurrent;   // scaled Solenoid Current [mA]
  
static unsigned short Hoist;        // scaled Hoist Lever Position Setpoint [pct]
static unsigned short PrecVoltage;  // scaled Precision Voltage
static unsigned short SpareVoltage; // scaled Spare Voltage   
static unsigned short PwrVoltage;   // scaled 12F10 Voltage       
  
static unsigned char  MuxVal;       // Value Mux is switched to
static unsigned short MuxCtr;       // Used to delay incrementing MuxVal


// **** Indexes, slaved off of analog sensors, but scaled for table look-up ****
static unsigned short PedalIndex;  // Percent Pedal has been stroked
static unsigned short RPMIndex;    // Speed of engine divided by 10;
static unsigned short HoistIndex;  // Percent Hoist lever has been stroked



// Variables uses in RPM Calculation

static unsigned char  RPMDirty;    // Tell RPM routine if OK to calc RPM
static unsigned short Tmr1CapOld;  // Counter Value when last pulse occured
static unsigned short Tmr1CapNew;  // Counter Value when latest pulse occured
static unsigned long  PPSPeriod;   // Difference between pulse timestamps
static unsigned long  PPS;         // Pulses per second
static unsigned short RPMCounter;  // Used to determine if engine is dead  

static unsigned long  RPML;        // Unaveraged,instantaneous RPM just calculated  
static unsigned short RPM;         // Same as above, but converted to short 


static unsigned short OldRPM[RPMCOUNTS+1]; // Used to average RPM readings
static unsigned char  RPMAverageCounter1; // Used to access OldRPM array in RPM ISR
static unsigned char  RPMAverageCounter2; // Used to access OldRPM array in RPM ISR
static unsigned long  RPMAverager;       // Add all RPMCOUNT RPMs up for averaging
static unsigned long  RPMAvg;            // Average RPM 


// ****  Switch Sensor Data  ****
static unsigned char PilotEnFETState; // Map Pilot Enable FET State Var to HW 
static unsigned char KeySwitch;     // State of KeySwitch (Ign_Sw_Volts) Input


 
// Port F Inputs
static unsigned char ForwardSwitch;   // Map Forward Switch Variable to HW
static unsigned char ReverseSwitch;   // Map Reverse Switch Variable to HW
static unsigned char HoistSwitch;     // Map Hoist Pressure Switch Var to HW
static unsigned char PedalSwitch;     // Map Pedal Switch Variable to HW
static unsigned char WSLeftB;         // Map Left Wheel Speed Channel A to HW
static unsigned char TiltSwitch;      // Map Tilt Switch Variable to HW
static unsigned char SpareSwitch;     // Map Spare Switch Variable to HW  

// Port QS Inputs
static unsigned char OldParkSwitch;       // Remember last value of ParkSwitch
static unsigned char ParkSwitch;    // Map Park Switch Variable to HW
static unsigned char EngCutFETState;// Map Engine Cutoff Relay State to HW

// Port GP Inputs   
static unsigned char RPMInput;    // State of RPM Input   
static unsigned char WSLeftA;     // Left Wheel Speed Channel A 
static unsigned char WSRightA;    // Right Wheel Speed Channel A 

// PACTL Inputs    
static unsigned char WSRightB;      // Map Right Wheel Speed Channel B to HW
static unsigned char ThrotFETState; // Map Throttle Motor FET State Var to HW


// Port X Inputs 
static unsigned char OverTempFETState; // OVER_TEMP_LAMP MOSFET State
static unsigned char RetardFETState;   // RETARD_SOLENOID MOSFET State       
static unsigned char FaultFETState;    // FAULT_LAMP MOSFET State                                        
static unsigned char GovFETState;      // GVNR_MTR_RETURN MOSFET State
static unsigned char AuxFETState;      // AUX_PWM_RETURN MOSFET State



