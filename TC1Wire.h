
// This defines which HC16 Analog Channels are attached to which Analog Signals
// Board changes which change the order of these signals need to be accompanied
// by changes to these definitions

#define SPARE_MUX_INDEX 0  // refers to Spare Analog related value in arrays
#define OILTEMP_INDEX   1  // refers to Oil Temperature related value in arrays
#define BRKPRESS_INDEX  2  // refers to Brake Pressure related value in arrays  
#define CHGPRESS_INDEX  3  // refers to Charge Pressure related value in arrays  
#define PEDAL_INDEX     4  // refers to Throttle Pedal related value in arrays  
#define THRCUR_INDEX    5  // refers to Throttle Motor PWM Current in arrays
#define SOLCUR_INDEX    6  // refers to Solenoid Current related value in arrays  
#define MUX_INDEX       7  // refers to signals routed through Analog Mux


// There is an Analog Mux which feeds A/D Channel MUX_INDEX.  If the mux is not 
// stuffed, than the signal at Mux0 feeds directly into Channel 7, otherwise the 
// value of the Mux determines which signal feeds Channel MUX_INDEX.
// Changes to signals into the analog mux should be accompanied by changes to
// the following definitions.
#define HOISTPOS_MUX_INDEX 0  // refers to Hoist Position related value in arrays
#define RACKPOS_INDEX      1  // refers to Position of Diesel Rack in arrays
#define PREC_MUX_INDEX     2  // refers to Precision Voltage reference
#define PWR_MUX_INDEX      3  // refers to 12F10 Voltage 
#define MUX_CHANS          4  // number of Mux Channels being used


// *** Analog Data Array Indices ***       
#define RACKPOS_INPUT  0  // refers to Position of Diesel Rack in arrays
#define OILTEMP_INPUT  1  // refers to Oil Temperature related value in arrays
#define BRKPRESS_INPUT 2  // refers to Brake Pressure related value in arrays  
#define CHGPRESS_INPUT 3  // refers to Charge Pressure related value in arrays  
#define PEDAL_INPUT    4  // refers to Throttle Pedal related value in arrays  
#define THRCUR_INPUT   5  // refers to Throttle Motor PWM Current in arrays
#define SOLCUR_INPUT   6  // refers to Solenoid Current related value in arrays     
#define HOISTPOS_INPUT 7  // refers to Hoist Position related value in arrays
#define SPARE_INPUT    8  // refers to Spare Analog  related value in arrays
#define PREC_INPUT     9  // refers to Precision Voltage reference
#define PWR_INPUT      10 // refers to 12F10 Voltage  
#define RPM_INPUT      11 // refers to RPM related values in arrays
#define PILOT_INPUT    12 // refers to Pilot Pressure controller is trying to produce(Not really an input, but convenient to treat as one)
#define RPMSP_INPUT    13 // refers to RPM controller is trying to set engine to     (Not really an input, but convenient to treat as one)
#define ANALOG_CHANS   14 // number of Analog Channels present




// To add a new timer, it is only necessary to assign the timer in the following
// list, name the index, increment NUM_TIMERS, and use the timer in the code  
// The actual Timer Code resides in Timers.C       
#define LITTLETICK_TIMER    0 // Provides 1/25 second Time Tick
#define ETHSEC_TIMER        1 // Provides 1/8 second Time Tick  
#define KEYOFF_TIMER        2 // Don't do power-down until key has been off for a while 
#define KEYON_TIMER         3 // Don't turn on CPU_ON until the key has been ON for a while too!
#define OVERTEMP_TIMER      4 // 30 sec timer before overtemp shutdown occurs
#define OVERSPEED_TIMER     5 // 1 sec timer before overspeed shutdown occurs         
#define CHARGELO_TIMER      6 // 2 sec timer before charge pressure limp home occurs
#define CHARGELOLO_TIMER    7 // 2 sec timer before charge pressure shutdown
#define FLASH_TIMER         8 // times length fault light is on when flashing event
#define FSPACE_TIMER        9 // time between flashes within a digit
#define DSPACE_TIMER       10 // time between two digits
#define RSPACE_TIMER       11 // time between repetition of flash code
#define BRAKEON_TIMER      12 // times when brake is pressed.    
#define THRTLCAL_TIMER     13 // time till automatic cal procedure end   
#define CALABORT_TIMER     14 // time till any calibration procedure aborts 
#define RETARDBAD_TIMER    15 // retard valve state doesn't match commanded position
#define RACKBAD_TIMER      16 // times when rack pot is out of calibrated range
#define HOISTBAD_TIMER     17 // times when hoist lever pot is out of calibrated range
#define TEMPBAD_TIMER      18 // times when temp sensor is out of calibrated range
#define BRAKEBAD_TIMER     19 // times when brake sensor is out of calibrated range
#define CHARGEBAD_TIMER    20 // times when charge sensor is out of calibrated range
#define PEDALBAD_TIMER     21 // times when pedal pot is out of calibrated range
#define FWDREVBAD_TIMER    22 // Forward/Rev Switches both showing high
#define FWDSWITCHOFF_TIMER 23 // times after Fwd Switch is clicked off
#define REVSWITCHOFF_TIMER 24 // times after Fwd Switch is clicked off
#define RPMBAD_TIMER       25 // times if no RPM signal but does have charge>50
#define FUSEBAD_TIMER      26 // times if input power is less than 9 Volts     
#define THRTLUPOFF_TIMER   27 // times after throttle-up system is inactive
#define NUM_TIMERS         28 // Number of timers to allocate
  


                     

#define MYPORTC  (*((MYPORTC_type*)(0xFFA41))) // 16R0 Port C Data Register
typedef struct  {
   unsigned int MUXCH      :2, // Analog Mux Select                
                NOTIPWM2EN :1, // NOT Current Mode PWM2 Enable
                NOTIPWM1EN :1, // NOT Current Mode PWM1 Enable
                FAULT      :1, // Fault Light 
                PILOTEN    :1, // Pilot Enable Solenoid
                OVRSPD     :1, // Overspeed Light
                SP1        :1; // Not used
                    } MYPORTC_type;   
                    
#define MUX_CHANNEL       MYPORTC.MUXCH        // Map Mux channel select to hardware   
#define PILOT_PWM_DIS     MYPORTC.NOTIPWM1EN   // Map Pilot Control PWM Enable to hardware   

   





#define MYPORTF  (*((MYPORTF_type*)(0xFFA19))) // Port F Data Register
typedef struct  {
   unsigned int UNUSED   :1,   // Unused?
                FWDSW    :1,   // Forward Switch
                REVSW    :1,   // Reverse Switch
                HOISTSW  :1,   // Hoist Pressure Switch
                PEDALSW  :1,   // Pedal Switch
                WSLEFTB  :1,   // Wheel Speed Sensor, Left Channel B
                TILTSW   :1,   // Tilt Switch
                SPAREIN  :1;   // Spare Digital Input 1
                    } MYPORTF_type;  
                    

#define MYPORTQS  (*((MYPORTQS_type*)(0xFFC15))) // PORT QS Data Register
typedef struct  {
   unsigned int MISO    :1,   // Master In Slave Out                         
                MOSI    :1,   // Master Out Slave In
                SCK     :1,   // Serial Clock
                SS      :1,   // Slave Select
                STARTEN :1,   // Start Enable (Tell instrument cluster OK for truck to start)
                PARKSW  :1,   // Park Switch
                CPUON   :1,   // Controls whether regulator is on or off
                TXD     :1;   // TXD/Data7 
                    } MYPORTQS_type;       
                    

  
#define MYDDRGP  (*((MYPORTGP_type*)(0xFF906))) // Port GP Data Direction Register   
#define MYPORTGP (*((MYPORTGP_type*)(0xFF907))) // Port GP Data Register  
typedef struct  {
   unsigned int RPM      :1, // IC1 RPM      
                WSLEFTA  :1, // IC2 Left Wheel Speed Channel A    
                WSRIGHTA :1, // IC3 Right Wheel Speed Channel A   
                ENGRLY   :1, // OC1 Engine Cutoff Relay Mosfet        
                OVRTEMP  :1, // OC2 Over Temp Lamp?                                
                RETARD   :1, // OC3 Retard Solenoid Mosfet
                GOVPWM   :1, // OC4 Governor Motor pseudo PWM output  
                AUXPWM   :1; // OC5 Aux pseudo PWM output
                    } MYPORTGP_type;      
                    
                    
#define MYPACTL  (*((MYPACTL_type*)(0xFF90C))) // Pulse Accumulator Control Register
// This structure is byte reversed from page 112 of manual, but it works THIS way,
typedef struct  {
   unsigned int PACNT    :8,  // Pulse Accumulator Counter           
                PACLK    :2,  // Pulse Accumulator Clock Select
                AUXPWMOUT:1,  // 0=>OC5, make MYPORTGP.AUXPWM an OUTPUT
                THRTFET  :1,  // State of Throttle Motor pseudo PWM FET
                PEDGE    :1,  // Pulse Accumulator Edge Control
                PAMOD    :1,  // Pulse Accumulator Mode
                PAEN     :1,  // Pulse Accumulator System Enable
                WSRIGHTB :1;  // Wheel Speed Sensor, Right Channel B            
                    } MYPACTL_type;
                    
  
// This is placed at an 60001 for a reason.  The block starts at 60000.  We must read the
// 74HC541, which is an 8-bit device, in a 16 bit read, because we mapped it to D0-D7
// which seems reasonable, but actually, an 8-bit read on the 68HC16Z1 reads D8-D15,
// so an 8-bit read doesn't work on the 16R1 board.  A sixteen bit read will work, but
// we must read the lower byte, which is in the odd numbered addresses of the block
// the chip select is mapped to.  
#define MYPORTX (*((MYPORTX_type*)(0x60001)))   // Port X Digital Input Port 
typedef struct  {
   unsigned int ENGRLY   :1, // ENG_CUT_RELAY MOSFET State 
                OVRTEMP  :1, // OVER_TEMP_LAMP MOSFET State
                RETARD   :1, // RETARD_SOLENOID MOSFET State       
                FAULT    :1, // FAULT_LAMP MOSFET State    
                PILOTEN  :1, // PILOT_ENABLE MOSFET State                             
                GOVFET   :1, // GVNR_MTR_RETURN MOSFET State
                AUXFET   :1, // AUX_PWM_RETURN MOSFET State
                KEYSW    :1; // KEY_SWITCH   
                    } MYPORTX_type;                          
