
void InitPerifs(void)
// Initialize Peripherals
{     
       
       
  //  A/D System Configuration           
  ADCTL0.PRS   = 15;   // Prescaler Rate Selection                          
  ADCTL0.STS   = 0;    // Sample Time Select                
  ADCTL0.RES10 = 0;    // 0=>8-bit conversion                

 
  ADCTL1.CHAN = 0;   // Channel Selection Field  
  ADCTL1.S8CM = 1;   // 1=>Eight Conversion Sequence
  ADCTL1.MULT = 1;   // 1=>Convert Block of Channels
  ADCTL1.SCAN = 0;   // 1=>Continuous Conversion
  
  ADCMCR.SUPV   = 0;    // 0 => unrestricted access                
  ADCMCR.FRZ    = 2;    // On freeze, finish conversion, than freeze                 
  ADCMCR.STOP   = 0;    // 0=>Normal Operation 

  MuxVal = 0;      // Initialize Mux to Channel 0
  MuxCtr = 0;      // Initialize MuxCnt to 0
  
  
  // PWM  System Configuration
  // pg 110
  GPTMCR.IARB  = 1;    // Interrupt Arbitration Identification
  GPTMCR.INCP  = 0;    // DON'T Increment Prescaler on STOPP
  GPTMCR.STOPP = 0;    // Normal Prescaller Operation
  GPTMCR.FRZ0  = 1;    // On FREEZE, Freeze state of GP Timer
  GPTMCR.STOP  = 0;    // Clocks not shutdown
                   

  // Set GPT's at vector #80h, which is at 100h
  // so Timer Overflow is vector 89h, since it's source is 9
  // and Output Compare 4 is vector 87h, since it's source is 7
 
  GPTICR.IVBA  = 8;  // 4 Interrupt Vector Base Address MS Nibble 
  GPTICR.IPL   = 1;  // GPT Interrupts get first priority          
  GPTICR.IPA   = 0;  // GPT Source number 9 (Timer overflow gets highest priority)                      

  // pg 115
  PWMCR.F1B = 0;      // 0=>Force PWMB low, 1=> Force PWMB high
  PWMCR.F1A = 0;      // 0=>Force PWMA low, 1=> Force PWMA high
  PWMCR.SFB = 0;      // PWMB Slow/Fast Select 0=>Khz region
  PWMCR.SFA = 0;      // PWMA Slow/Fast Select 0=>Khz region
  PWMCR.PPR = 2;      // PWM at 8.19 KHz
  PWMCR.PPROUT = 0;   // 0=>Normal PWM output on PWMA
  PWMCR.FPWMB  = 0;   // 1=>Force PWMB output to F1B state
  PWMCR.FPWMA  = 0;   // 1=>Force PWMA output to F1A state                                

  PWMA = 0;         // 0% PWM A Duty Cycle  
  PWMB = 0;         // 0% PWM B Duty Cycle    
  
  // Port GP Data Direction Register
  MYDDRGP.RPM      = 0; // IC1 RPM      
  MYDDRGP.WSLEFTA  = 0; // IC2 Left Wheel Speed Channel A    
  MYDDRGP.WSRIGHTA = 0; // IC3 Right Wheel Speed Channel A    
  MYDDRGP.ENGRLY   = 1; // OC1 Engine Cutoff Relay Mosfet        
  MYDDRGP.OVRTEMP  = 1; // OC2 Over Temp Lamp?                                  
  MYDDRGP.RETARD   = 1; // OC3 Retard Solenoid Mosfet
  MYDDRGP.GOVPWM   = 1; // OC4 Governor Motor pseudo PWM output  
  MYDDRGP.AUXPWM   = 1; // OC5 Aux pseudo PWM output  
  
  
  // Set up Port F data direction as all Inputs 
  DDRF.DDF0 = 0;   // 0=>Input 1=>Output Port F Pin 0 
  DDRF.DDF1 = 0;   // 0=>Input 1=>Output Port F Pin 1
  DDRF.DDF2 = 0;   // 0=>Input 1=>Output Port F Pin 2
  DDRF.DDF3 = 0;   // 0=>Input 1=>Output Port F Pin 3
  DDRF.DDF4 = 0;   // 0=>Input 1=>Output Port F Pin 4
  DDRF.DDF5 = 0;   // 0=>Input 1=>Output Port F Pin 5
  DDRF.DDF6 = 0;   // 0=>Input 1=>Output Port F Pin 6
  DDRF.DDF7 = 0;   // 0=>Input 1=>Output Port F Pin 7 
  
  // Three bits of Port QS are used as digital I/O   
  // pg 86
  PQSPAR.PCS1 = 0;  // define bit as an I/O bit   
  PQSPAR.PCS2 = 0;  // define bit as an I/O bit 
  PQSPAR.PCS3 = 0;  // define bit as an I/O bit 
                    
  // pg 87
  DDRQS.PCS1 = 1;   // Start Enable is an Output
  DDRQS.PCS2 = 0;   // Park Switch is an Input    
  DDRQS.PCS3 = 1;   // CPU_ON is an Output
  
  // pg 110 Set up GPT Interrupts  
  
  // A 16.78MHz system clock with a divide by 4 and a 16bit overflow means that
  // our overflow ISR will execute every 15.625ms (64 hz)
  
  // A 13.107MHz system clock with a divide by 4 and a 16bit overflow means that
  // our overflow ISR will execute every 20.0ms (50 hz)  It is to create a well
  // defined, unit of time measurement that adds up in an integer manner to create
  // seconds, that the speed of the processor is set at 13.107MHz rather than
  // leaving it at 16.78MHz.
 
  TMSK12.CPR = 0;    // TCNT tick is every 4 System Clock ticks
  TMSK12.TOI = 1;    // Timer Overflow Interrupt Enable 
  
  
  TCTL12.EDGE1 = 1;  // Input Capture 1, Capture on Rising Edge Only
  TMSK12.ICI1 = 1;   // Input Capture 1 (RPM) Interrupt Enable
  
  
  TCTL12.OML3 = 0;   // Output Compare 3 Disconnected from Output Pins
  TMSK12.OCI3 = 1;   // Output Compare 3 (Ramp) Interrupt Enable  
}  



void InitVariables(void)
// Make sure we start in a known state
{
  PilotOut = 0; // Init Pilot Vars
  PilotDuty = 0;  // Init Pilot Vars
  
  // Initialize all Fault and Transmission mode variables to OFF
  ReverseMode = 0;  // temp variable used in Mode determination    
  ForwardMode =0 ;  // temp variable used in Mode determination   
  NeutralMode= 0;  // temp variable used in Mode determination
  ShutDownMode = 0; // temp variable used in Mode determination  
  DisThrotMode= 0; // temp variable used in Mode determination
  ENeutMode= 0;    // temp variable used in Mode determination 
  EStopMode= 0;    // temp variable used in Mode determination
  PEStopMode= 0;   // temp variable used in Mode determination   
  
  OverSpeedLatch = 0;
  InRetardMode   = 0;  
  
  
  // Ramp Rate Initialization
  fRPM_SP_Inc = filteredRPM_SP_IncConst; // ramp rate of fRPM_SP

  // Power Adapt Initialization
  paCVi = 0;  // Reset Integral term
  paKp = paKpConst;  // load proportional power-adapt gain from flash
  paKi = paKiConst;  // load integral power-adapt gain from flash   

  // Throttle Motor RPM Control PID Parameters
  tKp = tKpConst;            // Load Proportional Gain from flash
  tKi = tKiConst;            // Load Integral Gain from flash
  tKFF = tKFFConst;           // Load FeedForward Gain from flash
  TiltSP = TiltSPConst;         // Load Tilt RPM Setpoint from flash
  Throttle_SPInc = Throttle_SPIncConst; // Load RPM/sec ramp rate from flash


  RPM = 0;           // Initialize RPM to zero
  RPMIndex = 0;  
  RPMDirty = 1;      // Tell RPM routine that it has been interferred with 

  
  EventCode = 0;
  TempEventCode = 0;
  
  
  FlashedTens = 0;
  DigitWaited = 0;
  FlashedOnes = 0;
  EventWaited = 0;
  TD = 0;
  OD = 0;
  FaultLight = 0;
  
  ThrottleCalFlag = 0; // flags that a throttle system cal is active
  HoistCalFlag = 0; // flags that a hoist lever cal is active
  
  LatchCPUON = 0;  // Init to OFF on startup.  Used on Power-Up, to avoid cycling
}
  
  

