
// Compiler Options -Ml -cb       for Large  Memory Model


//#define P6TRUCK      // P6 Truck has TI Charge Pressure Xducer
#define PILOTTRUCK     // Pilot Trucks have Rack Pot comming in on what was spare Input


#include "HC16Z1.h"    // HC16Z1 Registers defined as far pointers
#include "TC1Wire.h"   // 16R0 Registers and I/O Board Mapping

#include "TC1Defs.h"   // Programmer Defined Parameters 
#include "TC1Param.h"  // Dealer & Factory Defined Parameters

#include "Strcturs.h"  // Structures used in Data Manipulation

#include "FlshWrte.H"  // Variable declaration for FlshWrte.C

#include "Errors.h"    // Include Errors.h   Error Definitions 
#include "SPI_Int.h"   // Variable declarations for SPI_Int.C

#include "PilotCnt.H"  // Pilot Control Variables
#include "ThrtlCnt.H"  // Throttle Motor Control Variables
#include "SensorIn.H"  // Analog Input & Scaling, Digital Input & Conditioning Vars
#include "Timers.H"    // Timer related variables

#include "SCI_Int.H"   // Variable declaration for SCI_Int.C

#include "Calibrat.H"  // Variable declaration for Calibrat.C



static unsigned char  StartUpMode;  // temp variable used in Mode determination
static unsigned char  CalMode;      // temp variable used in Mode determination
static unsigned char  ReverseMode;  // temp variable used in Mode determination    
static unsigned char  ForwardMode;  // temp variable used in Mode determination   
static unsigned char  NeutralMode;  // temp variable used in Mode determination
static unsigned char  ShutDownMode; // temp variable used in Mode determination  
static unsigned char  DisThrotMode; // temp variable used in Mode determination
static unsigned char  LimpHomeMode; // temp variable used in Mode determination  
static unsigned char  ENeutMode;    // temp variable used in Mode determination 
static unsigned char  EStopMode;    // temp variable used in Mode determination
static unsigned char  PEStopMode;   // temp variable used in Mode determination   

static unsigned char  TransMode;    // mode of transmission(Forward,Reverse,Neutral)
static unsigned char  FaultMode;    // fault condition of transmission.

static unsigned char  OverSpeedLatch; // Flag to remember overspeed occurred
static unsigned char  InRetardMode;   // Flag to remember retard mode is engaged

static unsigned char  WriteEventCounter;  // Used to write to EEPROM all Events
static unsigned char  ReadEventCounter;   // Used to read all Event from EEPROM
static unsigned char  SPIWorking;     // Flag to track progress of SPI routines

static unsigned char  PowerDownOK;    // Flag to allow Board to be shut off

static unsigned char  testChar1;
static unsigned char  testChar2;
static unsigned char  testChar3;
static unsigned short testInt;


#include "PilotCnt.C"  // Pilot Control Routines  
#include "ThrtlCnt.c"  // Throttle Motor Control Routines

#include "FlshWrte.c"  // Flash EPROM Write Routines
#include "Errors.c"    // Event/Error Code Handling
#include "SensorIn.c"  // Analog Input & Scaling, Digital Input & Conditioning
#include "Timers.c"    // Timers and HourMeter
#include "SPI_Int.c"   // SPI  Routines   
#include "Init.c"      // Initialization Code
#include "SCI_Int.c"   // SCI Routines

#include "Calibrat.c"  // Parameter Calibration Routines


void ProcessEvents(void)
// Various flags and inputs are polled in this routine, so that
// when certain conditions occur, steps can be taken.
// Basically, this is a place for miscellaneous event handling
// to occur, that doesn't really fit into any other category.
{
  // Initiate a Throttle-Cal Mode, if operator signals it.
  // which he does by setting the parkbrake on a running truck
  // and then holding down the brake for five seconds.
  // and then releasing it.    
  if (ThrottleCalOK)
  {
    ThrottleCalOK = 0;  // reset ThrottleCalOK    
    
    if ((RPM>DEADTHRES) && (TransMode == NEUTRAL))  // and if other conditions are met...
      StartThrottleCal();  // initialize arrays and set ThrottleCalFlag  
  }
  
  
  // Gather Throttle System Calibration Data
  if (ThrottleCalFlag)
    GatherThrottleData();  // Gather data for Throttle System Calibration   
    
 
  
}



void SetMode (void)       // determine mode of controller operation
// Set Mode of Controller.  This function determines Mode.
// It is important this function is set up in a manner that
// erratic behavior doesn't result if it's interupted before
// it completely finishes evaluting
{ 
  
  // There are 6 Transmission Modes Possible.  These Modes determine
  // major operation characteristics of the truck.  
  
 
  if (!PedalSwitch)   // StartUpMode is set to 1 on start.It stays 1 till PedalSwitch
    StartUpMode = 0;  // is read as off which proves PedalSwitch is not welded shut.
  
  CalMode     = ThrottleCalFlag || HoistCalFlag;
  

  // P3 has a messed up forward/reverse switch, causing it to generate spurious signals on the 
  // unused input.  (flashing ReverseSwitch, if going forward and vice versa.
  // so the following is added just till P3 is fixed.
//  ReverseMode = (ReverseSwitch && !ForwardSwitch && !ParkSwitch && !CalMode && !StartUpMode);      
//  ForwardMode = (ForwardSwitch && !ReverseSwitch && !ParkSwitch && !CalMode && !StartUpMode); 
  
  ReverseMode = ((ReverseSwitch || ForwardSwitch) && !ParkSwitch && !CalMode && !StartUpMode);      
  ForwardMode = ((ForwardSwitch || ReverseSwitch) && !ParkSwitch && !CalMode && !StartUpMode);
  
  
  NeutralMode = !CalMode && (ParkSwitch || (!ReverseSwitch && !ForwardSwitch));   
  
  ShutDownMode = !KeySwitch;// && Timer[KEYOFF_TIMER].DN; 
  
 
  // There are 5 Fault Modes Possible.  These modes act to limit or inhibit
  // the operations requested by the Transmission Modes.  These modes
  // are mutually exclusive to each other, but will coexist with the
  // transmission modes.

  
  
  // ***** Errors 40 to 49 cause LIMP HOME MODE *****
  // ***** These conditions do cause the fault light to light                 *****   
  LimpHomeMode = ((EventCode>=40) && (EventCode<=49));    
  
  // ***** Events 50 to 59 cause DISABLE THROTTLE MODE ***** 
  DisThrotMode = ((EventCode>=50) && (EventCode<=59));              
 
  // ***** Events 60 to 79 cause EMERGENCY NEUTRAL MODE *****
  ENeutMode  = ((EventCode>=60) && (EventCode<=79));           
  
  // ***** Events 80 to 89 cause EMERGENCY SHUTDOWN MODE *****
  EStopMode = ((EventCode>=80) && (EventCode<=89));
  
  // ***** Events 90 to 99 cause Emergency Shutdown Mode w/ NO RESTART ALLOWED *****
  PEStopMode = ((EventCode>=90) && (EventCode<=99));
  
  
  // Set Transmission Mode variable.
  if (ShutDownMode)
    TransMode = SDOWN;
  else        
    if (NeutralMode)
      TransMode = NEUTRAL;
    else
      if (ForwardMode)
        TransMode = FORWARD;
      else
        if (ReverseMode)
          TransMode = REVERSE;
        else
          if (CalMode)
            TransMode = CALIBRATE;
          else
            TransMode = NEUTRAL;  // This will exectute if in StartUpMode
                    
                    
// Set Fault Mode variable.
  if (PEStopMode)
    FaultMode = PESTOP;  // Permanent Emergency Stop (No Restart Allowed)
  else
    if (EStopMode)
      FaultMode = ESTOP; // Emergency Stop (Restart Allowed)
    else
      if (ENeutMode)
        FaultMode = ENEUT; // Emergency Neutral
      else
        if (DisThrotMode)
          FaultMode = DTHROT;  // Disable Throttle Motor
        else     
          if (LimpHomeMode)
            FaultMode = LIMPHOME;  // Limp Home Mode
          else       
            if (EventCode<20)    // No Fault Mode present
              FaultMode = HEALTHY;
            else
              FaultMode = HEALTHY;
}    
    

void OutputControl(void)


// This routine determines whether an output is in auto or manual, and
// if in auto, sets the command bit according to the control logic,
// otherwise, the command bit is set to the manual bit, which is 
// set via the serial link
{  



  // Control Pilot Solenoid PWM Duty Cycle
  if ((FaultMode != ENEUT) && (FaultMode != ESTOP) && (FaultMode != PESTOP))
    ExtAnlgOut.Auto.o.PILOTSOL = PilotDuty;
  else
    ExtAnlgOut.Auto.o.PILOTSOL = 0;
    

  // Control Throttle Motor PWM Duty Cycle
  if ((FaultMode != DTHROT) && (FaultMode != ESTOP) && (FaultMode != PESTOP) 
      && (TransMode != CALIBRATE))
    ExtAnlgOut.Auto.o.THRTLMTR = (unsigned char)(ThrottleOnTime);
  else
    ExtAnlgOut.Auto.o.THRTLMTR = 0;

  
 
  // Engine Cutoff Relay slaved to ESTOP Mode & PESTOP Mode
  // Engine Cutoff Relay must be ON to allow engine to run  
  ExtDigOut.Auto.o.ENGRLY = !((FaultMode == ESTOP) || (FaultMode == PESTOP)); 
  
  
  
  // Pilot Enable.  On new boards, this goes through watchdog circuit
  ExtDigOut.Auto.o.PILOTEN  = ((FaultMode != ENEUT) && (FaultMode != ESTOP) && (FaultMode != PESTOP));

  
  
  // ******* START Retard Solenoid ******
  // Retard Solenoid Logic.  Remember that it fails OPEN, it is ENERGIZED to CLOSE  

  if (RPM>OVERSPEED_POINT1)
    InRetardMode = 1;   // Place into Retard Mode on Overspeed (valve will OPEN)
    
  if ( (InRetardMode) && (RPM<OVERSPEED_POINT3))  
    InRetardMode = 0;   // Exit Retard Mode with Hysteresis (valve will CLOSE)

   
  ExtDigOut.Auto.o.RETARD = (  ((ChargePress>50) || (RPM>DEADTHRES)) && (!InRetardMode));    
  // ******* END Retard Solenoid *****
  

   // ****** Start Enable Signal ****** 
   // Wiring Harness is somehow "LATCHING" on power to board if the 
   // StartEnable output is high.  So I'm fixing this in software by
   // making sure that StartEnable is OFF when trying to power board off.
  
   ExtDigOut.Auto.o.STARTEN = (!PowerDownOK && (           // turn off start enable if in Power down
                                (TransMode == NEUTRAL) ||   // otherwise, enable if in neutral
                                ParkSwitch ||               // or the park brake is switched
                                (BrakePress>300)            // or brake is pressed
                                               )
                              );
    
   // ******  START Fault Lights ******  
   
   if (TransMode != CALIBRATE)
   {
     ExtDigOut.Auto.o.OVRTEMP = ((LoopTemp>=TEMPHI_POINT) && ESecTick); 
     ExtDigOut.Auto.o.OVRSPD = ((OverSpeedLatch) && ESecTick);
     ExtDigOut.Auto.o.FAULT = FaultLight;
   }
   else // in CalMode, I want to flash all the lights in sequence
   {
     ExtDigOut.Auto.o.OVRTEMP = (TMSecs>=0) && (TMSecs<16); 
     ExtDigOut.Auto.o.OVRSPD  = (TMSecs>=16) && (TMSecs<33);
     ExtDigOut.Auto.o.FAULT   = (TMSecs>=33) && (TMSecs<51); 
   }
   // ******  END Fault Lights ******  
   
  
  
  
  // 1st, only let manual operation proceed if appropriate safeguards are met
  if ( (ManualOK1 == MANUAL_OK_PASSWORD1) && (ManualOK2 == MANUAL_OK_PASSWORD2))
  { 
    
    // ******** Analog Outputs *********
    
    // *** Pilot Solenoid PWM ***
    if (ExtAnlgOut.Mode.o.PILOTSOL)   // Check if output is in Auto or Manual
    {
      ExtAnlgOut.Command.o.PILOTSOL =  ExtAnlgOut.Manual.o.PILOTSOL;
      
      // Enable PWM driver for Pilot Solenoid always, if in Manual
      PILOT_PWM_DIS = 0;  
    }
    else
    {
      ExtAnlgOut.Command.o.PILOTSOL =  ExtAnlgOut.Auto.o.PILOTSOL;       
      ExtAnlgOut.Manual.o.PILOTSOL  = ExtAnlgOut.Auto.o.PILOTSOL;  // for bumpless transfer to manual
      
      // Disable PWM driver for Pilot Solenoid if not in forward or reverse mode, and in Auto
      PILOT_PWM_DIS = !((TransMode == FORWARD) || (TransMode == REVERSE));  
    }
    
    
    // *** Throttle Motor PWM ***
    if (ExtAnlgOut.Mode.o.THRTLMTR)   // Check if output is in Auto or Manual
      ExtAnlgOut.Command.o.THRTLMTR =  ExtAnlgOut.Manual.o.THRTLMTR;
    else
    {
      ExtAnlgOut.Command.o.THRTLMTR =  ExtAnlgOut.Auto.o.THRTLMTR; 
      ExtAnlgOut.Manual.o.THRTLMTR  = ExtAnlgOut.Auto.o.THRTLMTR;  // for bumpless transfer to manual
    }
  
  
  
    // ******** Digital Outputs *********
    
    // *** Fault Light ***
    if (ExtDigOut.Mode.o.FAULT)   // Check if output is in Auto or Manual
      ExtDigOut.Command.o.FAULT =  ExtDigOut.Manual.o.FAULT;
    else
    {
      ExtDigOut.Command.o.FAULT =  ExtDigOut.Auto.o.FAULT; 
      ExtDigOut.Manual.o.FAULT  = ExtDigOut.Auto.o.FAULT;  // for bumpless transfer to manual
    }
      
    
    // *** Overspeed Light ***
    if (ExtDigOut.Mode.o.OVRSPD)  // Check if output is in Auto or Manual
      ExtDigOut.Command.o.OVRSPD =  ExtDigOut.Manual.o.OVRSPD;
    else
    {
      ExtDigOut.Command.o.OVRSPD =  ExtDigOut.Auto.o.OVRSPD;  
      ExtDigOut.Manual.o.OVRSPD  = ExtDigOut.Auto.o.OVRSPD;  // for bumpless transfer to manual
    }
      
      
    // *** Over Temp Lamp ***
    if (ExtDigOut.Mode.o.OVRTEMP)  // Check if output is in Auto or Manual
      ExtDigOut.Command.o.OVRTEMP =  ExtDigOut.Manual.o.OVRTEMP;
    else
    {
      ExtDigOut.Command.o.OVRTEMP =  ExtDigOut.Auto.o.OVRTEMP;
      ExtDigOut.Manual.o.OVRTEMP  = ExtDigOut.Auto.o.OVRTEMP;  // for bumpless transfer to manual
    }
      
      
    // *** Start Enable ***
    if (ExtDigOut.Mode.o.STARTEN)  // Check if output is in Auto or Manual
      ExtDigOut.Command.o.STARTEN =  ExtDigOut.Manual.o.STARTEN;
    else
    {
      ExtDigOut.Command.o.STARTEN =  ExtDigOut.Auto.o.STARTEN; 
      ExtDigOut.Manual.o.STARTEN  = ExtDigOut.Auto.o.STARTEN;  // for bumpless transfer to manual
    }
      
    // *** Engine Cutoff Relay ***
    if (ExtDigOut.Mode.o.ENGRLY)  // Check if output is in Auto or Manual
      ExtDigOut.Command.o.ENGRLY =  ExtDigOut.Manual.o.ENGRLY;
    else
    {
      ExtDigOut.Command.o.ENGRLY =  ExtDigOut.Auto.o.ENGRLY;  
      ExtDigOut.Manual.o.ENGRLY  = ExtDigOut.Auto.o.ENGRLY;  // for bumpless transfer to manual
    }
      
      
    // *** Pilot Enable Solenoid ***
    if (ExtDigOut.Mode.o.PILOTEN)  // Check if output is in Auto or Manual
      ExtDigOut.Command.o.PILOTEN =  ExtDigOut.Manual.o.PILOTEN;
    else
    {
      ExtDigOut.Command.o.PILOTEN =  ExtDigOut.Auto.o.PILOTEN;  
      ExtDigOut.Manual.o.PILOTEN  = ExtDigOut.Auto.o.PILOTEN;  // for bumpless transfer to manual
    }
      
    
    // *** Retard Solenoid ***
    if (ExtDigOut.Mode.o.RETARD)  // Check if output is in Auto or Manual
      ExtDigOut.Command.o.RETARD =  ExtDigOut.Manual.o.RETARD;
    else
    {
      ExtDigOut.Command.o.RETARD =  ExtDigOut.Auto.o.RETARD;  
      ExtDigOut.Manual.o.RETARD  = ExtDigOut.Auto.o.RETARD;  // for bumpless transfer to manual
    }
      
    
    // *** Governor Motor pseudo PWM output ***
    if (ExtDigOut.Mode.o.GOVPWM)  // Check if output is in Auto or Manual
      ExtDigOut.Command.o.GOVPWM =  ExtDigOut.Manual.o.GOVPWM;
    else
    {
      ExtDigOut.Command.o.GOVPWM =  ExtDigOut.Auto.o.GOVPWM;  
      ExtDigOut.Manual.o.GOVPWM  = ExtDigOut.Auto.o.GOVPWM;  // for bumpless transfer to manual
    }
      
      
    // *** Aux pseudo PWM output ***
    if (ExtDigOut.Mode.o.AUXPWM)  // Check if output is in Auto or Manual
      ExtDigOut.Command.o.AUXPWM =  ExtDigOut.Manual.o.AUXPWM;
    else
    {
      ExtDigOut.Command.o.AUXPWM =  ExtDigOut.Auto.o.AUXPWM;  
      ExtDigOut.Manual.o.AUXPWM  = ExtDigOut.Auto.o.AUXPWM;  // for bumpless transfer to manual
    }
    
    
    
  }
  else
  {
    // Not OK to even attempt manual operation, so use Auto controls
    
    // ******** Analog Outputs *********
    ExtAnlgOut.Command.whole = ExtAnlgOut.Auto.whole;
    ExtAnlgOut.Manual.whole = ExtAnlgOut.Auto.whole;  // for bumpless transfer to manual   
    
    // Disable PWM driver for Pilot Solenoid if not in forward or reverse mode, and in Auto
    PILOT_PWM_DIS = !((TransMode == FORWARD) || (TransMode == REVERSE));  
    
    // ******** Digital Outputs *********
    ExtDigOut.Command.whole = ExtDigOut.Auto.whole;
    ExtDigOut.Manual.whole = ExtDigOut.Auto.whole;  // for bumpless transfer to manual    
  }
}



void SetOut(void)
// All Analog and Digital Outputs are set in this function.  This function
// turns on fault lights, clips pilot pressure, controls retard solenoid,etc.,
// all based on TransMode and FaultMode conditions.
{   
   
  // Control Pilot Solenoid PWM Duty Cycle
  PWMA = ExtAnlgOut.Command.o.PILOTSOL;
//  PWMA = testChar1;
  
  // Control Throttle Motor PWM Duty Cycle
  PWMB = ExtAnlgOut.Command.o.THRTLMTR;
//  PWMB = testChar1;
 


  MYPORTGP.ENGRLY = ExtDigOut.Command.o.ENGRLY;
  

  MYPORTC.PILOTEN = ExtDigOut.Command.o.PILOTEN && LittleTick;   // must kickdog on R1 Board

  
  
  MYPORTGP.RETARD = ExtDigOut.Command.o.RETARD;
  

  MYPORTGP.OVRTEMP = ExtDigOut.Command.o.OVRTEMP;    
  MYPORTC.OVRSPD = !ExtDigOut.Command.o.OVRSPD;     
  MYPORTC.FAULT = !ExtDigOut.Command.o.FAULT;      

  
  MYPORTQS.STARTEN = !ExtDigOut.Command.o.STARTEN; 
  
  
  // Currently unused outputs
  
  MYPORTGP.GOVPWM = ExtDigOut.Command.o.GOVPWM;  // OC4 Governor Motor pseudo PWM output  
  MYPORTGP.AUXPWM = ExtDigOut.Command.o.AUXPWM;  // OC5 Aux pseudo PWM output
   
  

  MYPORTQS.CPUON = !PowerDownOK;  // Turn off board, if PowerDown is approved
  
  // if shutting transmission down
  if ((TransMode == SDOWN) && !PowerDownOK)
  {
    // Turn off all external controls 1st thing
    MYPORTC.PILOTEN  = 0;  // Turn off Pilot Enable   
    PWMA = 0;  // Set Duty Cycle to 0
    
    
    // Write HourMeter Reading, and EventNumber into EEPROM
    HourMeterRecord.Hours = Hours;      // remember hours 
    HourMeterRecord.Seconds = Seconds;  // remember seconds
    HourMeterRecord.Event = EventNumber;// remember Event Number;

    
    SPI_Operation = 0; // Must Reset this to insure successful EEPROM write
    SPI_CommandPending = 0;  // Must Reset this too...
    SPIWorking = 1;
    while (SPIWorking)    
      SPIWorking = StoreEvent(-1,HourMeterRecord);
                                                       
    // Write entire event buffer into serial EEPROM
    for (WriteEventCounter = 0;WriteEventCounter<EVENT_BUFF_SIZE-1;WriteEventCounter++) 
    {
      SPIWorking = 1;
      while (SPIWorking)      
        SPIWorking = StoreEvent(WriteEventCounter,EventBuff[WriteEventCounter]);      
    } 


    if (FaultMode != PESTOP)  // Don't power off board if Permanent ESTOP
      PowerDownOK = 1;  // Allow board to be powered down
  }  
}    
   


void main(void) 
{  

  PowerDownOK = 1;     // Power Down is OK, till main loop begins to run
  
  InitPerifs();        // Initialize Onboard Peripherals  
  
  InitTimers();        // Initialize Timers
  InitVariables();     // Make sure we start in a known state  
  
  InitSPI();           // Initialize SPI Module 
  InitSCI();           // Initialize SCI Module  
  
/*  
  // Trigger the A/D to take another reading
  ADCTL1.CHAN = 0;   // Channel Selection Field  
  ADCTL1.S8CM = 1;   // 1=>Eight Conversion Sequence
  ADCTL1.MULT = 1;   // 1=>Convert Block of Channels
  ADCTL1.SCAN = 1;   // 1=>Continuous Conversion  
*/
  EnableInterrupts;    // Enable Interrupts    
  
  
  // Read Hour Meter and current EventNumber from serial EEPROM
  SPIWorking = 1;
  SPI_Operation = 0; // Reset this
  while (SPIWorking)
    SPIWorking = ReadEvent(-1,&HourMeterRecord); // Read from EEPROM
    
  Hours   = HourMeterRecord.Hours;
  Seconds = HourMeterRecord.Seconds;
  EventNumber = HourMeterRecord.Event;
  

  // New EEPROMs can have garbage in them, which we need to detect and 
  // get rid of.  The garbage is usually all ones (65535 or 255) so
  // just looking at the reasonableness of the numbers should fix things up
  if (Hours>43829) // Roll Hour Meter Over every 5 Years of Running
    Hours = 0;
   
  if (Seconds>3599)  // Roll Seconds over every hour
    Seconds = 0;
    
  if (EventNumber>=EVENT_BUFF_SIZE-1)
    EventNumber = 0;

    
  // Read entire event buffer from serial EEPROM    
  for (ReadEventCounter = 0;ReadEventCounter<EVENT_BUFF_SIZE-1;ReadEventCounter++)
  {
    SPIWorking = 1;
    while (SPIWorking)        
      SPIWorking = ReadEvent(ReadEventCounter,&EventBuff[ReadEventCounter]);
      
    // Check for garbage records and get rid of them .  Garbage records will be 
    // present first time eeprom is read.  It's possible they could be generated
    // other times as well, so we clean up, just in case.
    if ((EventBuff[ReadEventCounter].Seconds>3599) ||   // impossible, 3600 = 1 Hour
            (EventBuff[ReadEventCounter].Event>99) )    // impossible, events are 2 digits
    {
      EventBuff[ReadEventCounter].Hours = 0;
      EventBuff[ReadEventCounter].Seconds = 0;
      EventBuff[ReadEventCounter].Event = 0;
    }        
  }
  

  
//  StartUpMode = 1;   // start in StartUpMode.  Stay here till system checks out.
  StartUpMode = 0;   // can't use this till we get our pedal switch working.
  
  PowerDownOK = 0;  // PowerDown is NOT OK after Loop begins to run
  

  while (1)          // main loop
  {

    ReadAnalog();    // Read Analog Sensors
    ReadDigital();   // Read Digital Inputs
    
    CheckSensors();  // Check whether sensor data seems valid
    CheckTimers();   // Check if any timers have timed out
    ProcessEvents();  // Check flags and inputs for event signaling

    if (Timer[KEYON_TIMER].DN)  // wait a bit on power up, before looking for trouble
      SetError();      // Determine whether errors are present
   
    SetMode();        // Determine proper mode of transmission    
    OutputControl();  // Set Auto bits and Command bits for outputs based on Mode & Error Info
    SetOut();         // Send Command bits out to hardware
    
 
    CheckSCI();      // Check for SCI commands  
  }
}
