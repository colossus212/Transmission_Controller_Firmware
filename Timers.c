
void InitTimers(void)   // Sets up Timers before interupts are enabled
//  Timer List is kept in TC1Wire.h 
{
   // Make sure Timer Presets are set up before enabling interrupts
  

  Timer[LITTLETICK_TIMER].PRESET = 4;  // 1/25 Second Timer
  Timer[LITTLETICK_TIMER].EN = 1;      // Enable  Timer
  Timer[LITTLETICK_TIMER].R = 1;       // Reset Timer  

  Timer[ETHSEC_TIMER].PRESET = 12;     // 1/8 Second Timer
  Timer[ETHSEC_TIMER].EN = 1;          // Enable  Timer
  Timer[ETHSEC_TIMER].R = 1;           // Reset Timer  
  
  Timer[KEYOFF_TIMER].PRESET = 200;    // 2 Second Timer  
  Timer[KEYOFF_TIMER].R = 1;           // Reset Timer  
  Timer[KEYOFF_TIMER].DN = 0;          // Initialize Done Bit, just in case
  Timer[KEYOFF_TIMER].ACCUM = 0;       // Initialize Accum Register, just in case
  
  
  Timer[KEYON_TIMER].PRESET = 200;    // 2 Second Timer  
  Timer[KEYON_TIMER].R = 1;           // Reset Timer  
  Timer[KEYON_TIMER].DN = 0;          // Initialize Done Bit, just in case
  Timer[KEYON_TIMER].ACCUM = 0;       // Initialize Accum Register, just in case




  Timer[OVERTEMP_TIMER].PRESET = 3000;  // Set OverTemp timer for 30 seconds
  
  Timer[OVERSPEED_TIMER].PRESET = 100;  // Set OverSpeed timer for 1 seconds
  Timer[OVERSPEED_TIMER].R = 1;           // Reset Timer  
  Timer[OVERSPEED_TIMER].DN = 0;          // Initialize Done Bit, just in case
  Timer[OVERSPEED_TIMER].ACCUM = 0;       // Initialize Accum Register, just in case


  Timer[CHARGELO_TIMER].PRESET = 200;    // Set ChargeLow timer for 2 seconds
  Timer[CHARGELOLO_TIMER].PRESET = 200;  // Set ChargeLowLow timer for 2 seconds  
    

  Timer[FLASH_TIMER].PRESET  = 12;  // Time light flashes on
  Timer[FSPACE_TIMER].PRESET = 25;  // Time between light flashes within a digit
  Timer[DSPACE_TIMER].PRESET = 75;  // Time between light flashes between two digits
  Timer[RSPACE_TIMER].PRESET = 150;  // Time between two error codes
  
 
  Timer[BRAKEON_TIMER].PRESET = 2000;   // Times for 20 seconds after brake is pressed
  Timer[BRAKEON_TIMER].R  = 1;         // Reset timer 
  
  Timer[THRTLCAL_TIMER].PRESET = 1000;   // Throttle Calibration End 10 Second Timer  
  Timer[THRTLCAL_TIMER].R = 1;           // Reset Timer  
  
  Timer[CALABORT_TIMER].PRESET = 12000;   // Abort any Calibration after 2 minutes

  Timer[RETARDBAD_TIMER].PRESET = 200;   // Retard Valve must be in wrong postion for 2 seconds before error
  Timer[RETARDBAD_TIMER].R = 1;          // Reset Timer 
  Timer[RETARDBAD_TIMER].DN = 0;         // Initialize Done Bit, just in case
  Timer[RETARDBAD_TIMER].ACCUM = 0;      // Initialize Accum Register, just in case


  Timer[RACKBAD_TIMER].PRESET = 100;   // Rack Pot must be in wrong postion for 1 second before error
  Timer[RACKBAD_TIMER].R = 1;          // Reset Timer 

  Timer[HOISTBAD_TIMER].PRESET = 100;   // Hoist Lever Pot must be in wrong postion for 1 second before error
  Timer[HOISTBAD_TIMER].R = 1;          // Reset Timer 

  Timer[TEMPBAD_TIMER].PRESET = 100;   // Temperature Sensor must be in wrong postion for 1 second before error
  Timer[TEMPBAD_TIMER].R = 1;          // Reset Timer 

  Timer[BRAKEBAD_TIMER].PRESET = 4000;  // Brake Sensor must be in wrong postion for 40 seconds before error
  Timer[BRAKEBAD_TIMER].R = 1;          // Reset Timer 

  Timer[CHARGEBAD_TIMER].PRESET = 100;   // Charge Sensor must be in wrong postion for 1 second before error
  Timer[CHARGEBAD_TIMER].R = 1;          // Reset Timer 

  Timer[PEDALBAD_TIMER].PRESET = 100;   // Pedal Pot must be in wrong postion for 1 second before error
  Timer[PEDALBAD_TIMER].R = 1;          // Reset Timer 
  

  Timer[FWDREVBAD_TIMER].PRESET = 25;   // Forward and Reverse Switch must both be high for 0.25 seconds before error
  Timer[FWDREVBAD_TIMER].R = 1;         // Reset Timer 


  // Used to sense how long since forward or rev switch was on
  Timer[FWDSWITCHOFF_TIMER].PRESET = 100; // Times for small time after fwd switch is clicked off
  Timer[REVSWITCHOFF_TIMER].PRESET = 100; // Times for small time after rev switch is clicked off
  
  
  // Used to sense how long since thortle-up system was active
  Timer[THRTLUPOFF_TIMER].PRESET = 280;  // for 2.8 seconds after a throttle-up, disable overspeed
  Timer[THRTLUPOFF_TIMER].R = 1;         // reset timer

  
  // Used to sense how long since thortle-up system was active
  Timer[RPMBAD_TIMER].PRESET = 100;  // for 1 seconds after RPM<400 and charge>50, time
  Timer[RPMBAD_TIMER].R = 1;         // reset timer

  // Used to sense how long since thortle-up system was active
  Timer[FUSEBAD_TIMER].PRESET = 9000;  // for 90 seconds after Power<9V time timer
  Timer[FUSEBAD_TIMER].R = 1;          // reset timer

  
 
}  


void FlashEvent(void)
{    
 FlashedTens = (TD>=EventCode_TensDigit);
 DigitWaited = Timer[DSPACE_TIMER].DN;
 FlashedOnes = (OD>=EventCode_OnesDigit);
 EventWaited = Timer[RSPACE_TIMER].DN;
 
 Timer[FLASH_TIMER].EN = FaultLight;
 Timer[FLASH_TIMER].R = !FaultLight;
 
 Timer[FSPACE_TIMER].EN = !FaultLight;
 Timer[FSPACE_TIMER].R =  FaultLight; 
 
 
 if (!FlashedTens)  // haven't flashed the tens digit yet
 {
   Timer[DSPACE_TIMER].EN = 0; // Disable Digit Wait Timer
   Timer[DSPACE_TIMER].R = 1;  // Reset Digit Wait Timer   
     
   if (!FaultLight && Timer[FSPACE_TIMER].DN)
     FaultLight = 1;
     
   if (FaultLight && Timer[FLASH_TIMER].DN)
   {
     FaultLight = 0;
     TD++;
   }
     
 }
 else
   if (!DigitWaited)  // haven't waited for time between digits yet
   {
     Timer[DSPACE_TIMER].EN = 1;  // Enable Digit Wait Timer
     Timer[DSPACE_TIMER].R = 0;   // Quit Resetting Digit Wait Timer
   }
   else
     if (!FlashedOnes)  // haven't flashed the ones digit yet
     {
       Timer[RSPACE_TIMER].EN = 0;  // Disable Repetition Wait Timer
       Timer[RSPACE_TIMER].R = 1;   // Reset Repetition Wait Timer
       
       if (!FaultLight && Timer[FSPACE_TIMER].DN)
       FaultLight = 1;
     
       if (FaultLight && Timer[FLASH_TIMER].DN)
       {
         FaultLight = 0;
         OD++;
       }
     }
     else
       if (!EventWaited) // haven't waited for time between Repetitions yet
       {
         Timer[RSPACE_TIMER].EN = 1;  // Enable Repetition Wait Timer
         Timer[RSPACE_TIMER].R = 0;   // Quit Reseting Repetition Wait Timer
       }
       
       else  // everything is done, reset all flags
       {
          FlashedTens = 0;
          DigitWaited = 0;
          FlashedOnes = 0;
          EventWaited = 0;
          TD = 0;
          OD = 0;
          FaultLight = 0;
       } 
}
 


void CheckTimers(void)   //KeySwitch
// It's important to ignore cases when DN is ON but R is also ON 
// because there could be a period of 20ms after R is ON before DN is 
// turned off where this routine would execute every pass and cause chatter.
{ 



 if (Timer[LITTLETICK_TIMER].DN && !Timer[LITTLETICK_TIMER].R) 
  {       
    if (LittleTick)
      LittleTick = 0;  // Provide 1/25 Second Tick for watchdog, etc
    else
      LittleTick = 1;
            
    Timer[LITTLETICK_TIMER].R = 1;     // Reset This Timer     
  }
  
  
  
  if (Timer[ETHSEC_TIMER].DN && !Timer[ETHSEC_TIMER].R) 
  {       
    if (ESecTick)
      ESecTick = 0;  // Provide 1/8 Second Tick for lights, etc
    else
      ESecTick = 1;
            
    Timer[ETHSEC_TIMER].R = 1;     // Reset This Timer     
  }
  

  
  // It's important not to go into shutdown mode unless key has been off
  // for a while.  This keeps from cycling power if there is Switch Bounce
  Timer[KEYOFF_TIMER].EN = !KeySwitch;  // Enable  Timer if Key is OFF
  Timer[KEYOFF_TIMER].R = KeySwitch;    // Reset Timer if Key is ON  
  
  // It's also important not to latch regulator on unless key has been on
  // for a while.  This keeps from cycling power 
  Timer[KEYON_TIMER].EN = KeySwitch;  // Enable  Timer if Key is ON
  Timer[KEYON_TIMER].R = !KeySwitch;  // Reset  Timer if Key goes off
  
  
/*  PowerDownOK = !Timer[KEYON_TIMER].DN;
    
  if ( !LatchCPUON && Timer[KEYON_TIMER].DN) 
    MYPORTQS.CPUON = 1;  // Turn Regulator ON after Key has been on for two seconds       
  LatchCPUON = Timer[KEYON_TIMER].DN;  // Once you latch it on, it's on until shutdown sequence
*/
  
  // OverSpeed Timer times when RPM is too past mechanical operating parameters
  // It is used to determine if an overspeed condition is warant shutdown, or 
  // if it is merely a glitch or very short duration event
  Timer[OVERSPEED_TIMER].R = (RPM<OVERSPEED_POINT2);   // Reset when Speed is OK
  Timer[OVERSPEED_TIMER].EN = (RPM>=OVERSPEED_POINT2); // Enable when Speed is HIGH
 

  // OverTemp Timer times when Temp is past mechanical operating parameters
  // It is used to determine if Temp condition warants shutdown, or 
  // if it is merely a glitch or very short duration event
  Timer[OVERTEMP_TIMER].R = (LoopTemp<TEMPHIHI_POINT);   // Reset when LoopTemp is OK
  Timer[OVERTEMP_TIMER].EN = (LoopTemp>=TEMPHIHI_POINT); // Enable when LoopTemp is HIGH

 
  // Charge-Low Timer times when Charge Pressure is lower than desired
  // pressure for current RPM.  If it ever times out, truck will be program down
  // the pilot pressure based on how low charge pressure is
  Timer[CHARGELO_TIMER].EN = ((ChargePress<ChargeLimp[RPMIndex]) && (RPM>DEADTHRES));;
  Timer[CHARGELO_TIMER].R = !Timer[CHARGELO_TIMER].EN;
  
  
  // Charge-LowLow Timer times when Charge Pressure is lower than minimum acceptable
  // pressure for current RPM.  If it ever times out, truck will be shut off, with
  // no restart allowed.
  Timer[CHARGELOLO_TIMER].EN = ((ChargePress<ChargeShutoff[RPMIndex]) && (RPM>DEADTHRES));
  Timer[CHARGELOLO_TIMER].R = !Timer[CHARGELOLO_TIMER].EN;
  
  
   
 
   
  // To initiate a throttle calibration procedure, the brake must be pressed continuously
  // for five seconds, and than released.  This makes sure you don't go into cal mode
  // accidentally if the brake transducer goes bad and sticks high.
  
  // 1st, evaluate ThrottleCalOK
  // if brake has been on for a long time, and is now released, signal that Throttle Calibration is OK
  // this flag will be reset elsewhere
  if (!ThrottleCalOK)
    ThrottleCalOK = ( (Timer[BRAKEON_TIMER].DN) && (BrakePress<300)  && (CalParkSwitchCount >= 5) && ParkSwitch);
  // now it's ok to update timer, after reading data for ThrottleCalOK
  Timer[BRAKEON_TIMER].EN = (BrakePress>300);  // Enable timer when brake is pressed 
  Timer[BRAKEON_TIMER].R  = (BrakePress<300);  // Reset timer when brake is released
  
  // if they're pressing the brake, and they activate the park brake, count it
  if (Timer[BRAKEON_TIMER].TT && ParkSwitch && !OldParkSwitch) 
    CalParkSwitchCount++;  // used to get into cal mode  

  if (Timer[BRAKEON_TIMER].R) // if brake timer not timing
    CalParkSwitchCount = 0;   // reset this counter for next calibration

  


  
  
  // Once a throttle system calibration has begun, and the engine has gone over 
  // (FULLGOVSPD-100) RPM, then we start this timer.  When this timer times out, we automatically
  // end the calibration.
  // The scenario is that the operator puts it into cal mode,
  // then he slowly ramps the engine up 
  // and when he's done ramping, we kick him out of cal mode and write the data
  Timer[THRTLCAL_TIMER].EN = (RPM>(FULLGOVSPD-100) );  
  Timer[THRTLCAL_TIMER].R = !CalMode;      // Reset Timer  
  
  
  
  // Just in case the operator puts the truck in cal mode accidentally, we want to
  // make sure it kicks back out of cal mode, after a while.  This is accomplished
  // using an abort timer.  This timer will cause the cal to abort if it times out.
  Timer[CALABORT_TIMER].EN = CalMode;  // Enable if in Calibration Mode
  Timer[CALABORT_TIMER].R = !CalMode;  // Reset if not

  // Retard Valve must be in wrong postion for 1 second before error
  Timer[RETARDBAD_TIMER].EN = (ExtDigOut.Command.o.RETARD!=ExtDigOut.Sensed.o.RETARD);   
  Timer[RETARDBAD_TIMER].R = !Timer[RETARDBAD_TIMER].EN; // Reset Timer if valve is correct

  // Check Analog Sensors for failure, and enable timers if they are in failed mode
  Timer[RACKBAD_TIMER].EN = SensorFail[RACKPOS_INPUT];   
  Timer[RACKBAD_TIMER].R = !SensorFail[RACKPOS_INPUT]; // Reset Timer if Sensor OK

  Timer[HOISTBAD_TIMER].EN = SensorFail[HOISTPOS_INPUT];   
  Timer[HOISTBAD_TIMER].R = !SensorFail[HOISTPOS_INPUT]; // Reset Timer if Sensor OK

  Timer[TEMPBAD_TIMER].EN = SensorFail[OILTEMP_INPUT];   
  Timer[TEMPBAD_TIMER].R = !SensorFail[OILTEMP_INPUT]; // Reset Timer if Sensor OK

  Timer[BRAKEBAD_TIMER].EN = SensorFail[BRKPRESS_INPUT];   
  Timer[BRAKEBAD_TIMER].R = !SensorFail[BRKPRESS_INPUT]; // Reset Timer if Sensor OK

  Timer[CHARGEBAD_TIMER].EN = SensorFail[CHGPRESS_INPUT];   
  Timer[CHARGEBAD_TIMER].R = !SensorFail[CHGPRESS_INPUT]; // Reset Timer if Sensor OK

  Timer[PEDALBAD_TIMER].EN = SensorFail[PEDAL_INPUT];   
  Timer[PEDALBAD_TIMER].R = !SensorFail[PEDAL_INPUT]; // Reset Timer if Sensor OK
  
  
  
  
  // Check Digital Sensors for failure, and enable timers if they are in failed mode
  Timer[FWDREVBAD_TIMER].EN = (ForwardSwitch && ReverseSwitch);   
  Timer[FWDREVBAD_TIMER].R = !(ForwardSwitch && ReverseSwitch); // Reset Timer if Sensor OK
  

  // Used to sense how long since forward or rev switch was on
  Timer[FWDSWITCHOFF_TIMER].EN = !ForwardSwitch;
  Timer[FWDSWITCHOFF_TIMER].R = ForwardSwitch;

  Timer[REVSWITCHOFF_TIMER].EN = !ReverseSwitch;
  Timer[REVSWITCHOFF_TIMER].R = ReverseSwitch;

  
  // It's important to not abandon throttle-up speed compensation immediantly
  // after a throttle-up, as the RPM may still be raised a bit, and we don't
  // want to crash pilot, thinking it's an overspeed situation
  Timer[THRTLUPOFF_TIMER].EN = !ThrottleUpActive;
  Timer[THRTLUPOFF_TIMER].R = ThrottleUpActive;

  if (Timer[THRTLUPOFF_TIMER].DN)
    TUFuncWasTilt = 0;  // reset this flag for next time

  // The only way to sense whether the tach is bad is to look at charge pressure
  // if no RPM, but charge is still high, must be a bad tach sensor
  Timer[RPMBAD_TIMER].EN = ((ChargePress>50) && (RPM<DEADTHRES));
  Timer[RPMBAD_TIMER].R = !Timer[RPMBAD_TIMER].EN;

  // We want to alert the operator if the fuse is blown.  We sense the input power
  // voltage to the board to do this.  However, we must allow the voltage to droop
  // low during engine start, and NOT give the error
  Timer[FUSEBAD_TIMER].EN = (PwrVoltage < 9000);
  Timer[FUSEBAD_TIMER].R = (PwrVoltage > 9000);
 
  

  if (EventCode != 0) // If there is an Error
    FlashEvent();  // Access FlashEvent State Machine to flash fault light
}





void TimerTick(void)    // Called by Timer Overflow Interrupt   
// This is called by an interrupt service routine.  If porting this code 
// remember to set up the processor so that this is called every 0.02 seconds.
// I wanted it every 0.01 seconds, but the HC16 couldn't generate TOF 
// interrupts that fast.  However 0.02 seconds is quite accurate enough

// It is used to process all timers which use the TimerType structure
// These timers must be declared in an array of TimerType called Timer
 
{     
  // set up tti as a global unsigned char to save reallocation time

  for (tti=0;tti<NUM_TIMERS;tti++)  // Do all timers
  { // start of loop through all timers code
    if (Timer[tti].R)  // if reset bit is high
    {
      Timer[tti].ACCUM = 0;     // reset the timer      
      Timer[tti].R = 0;         // Clear Reset State              
    }
    
    
    if (Timer[tti].EN)               // if timer is enabled
    { // start of "Timer is Enabled" section of code
    
      if (Timer[tti].ACCUM<Timer[tti].PRESET)  // if timer is still counting
      {
        Timer[tti].ACCUM += 2;   // increment timer accumulator        
        Timer[tti].TT = 1;      // set Timer Timing bit
        Timer[tti].DN = 0;      // set Timer Done bit to low    
      }
      else
      {
        Timer[tti].TT = 0;      // reset Timer Timing bit
        Timer[tti].DN = 1;      // set Timer Done bit to high
      }     
    } // end of "Timer is Enabled" section of code
    else
    { // start of "Timer is NOT Enabled" code
      Timer[tti].DN = 0;   // reset done bit if not enabled
      Timer[tti].TT = 0;   // reset timer timing if not enabled
    } // end of "Timer is NOT Enabled" code
  } // emd of loop through all timers code  
}


void HourMeter(void)
// This routine is used to derive a time reference for errors 
// as well as an electronic hourmeter for the truck.
{
  if (RPM>DEADTHRES)  // if truck is running
  {    
    if (TMSecs<50) 
      TMSecs++;  // increment twenty-millisecond counter
    else  
      {
         TMSecs = 0;  // reset twenty-millisecond counter
         if (Seconds<3600)
           Seconds++; // increment seconds counter
         else  // increment hours (roll-over at 5 years)
           { 
             Seconds = 0;  // reset seconds counter                     
             if (Hours<43829)
               Hours++;
             else
               Hours = 0;               
           }  // end increment hours
      } // end increment seconds    
  
  } // end if truck is running  
}  // end HourMeter




#pragma TRAP_PROC SAVE_REGS
void TOF_ISR(void)    // Called by Timer Overflow Interrupt   
// This is the Timer Overflow Interrupt Service Routine
// It occurs every 0.02 seconds.
{

  TimerTick();        // Service all timers
  HourMeter();        // Update the hour meter
  RPMCheck();         // Zero RPM if no updates
  
 
  PilotControl();    //  Pick a Pilot Solution (used to do this in main()

  TM_PID();          //  Calculate PID equation to control RPM using Throttle Motor
  
  
  // This ISR contains enough instructions that it takes some time to finish.
  // Because of this, it is possible to miss some of the other Interrupt Service 
  // Routines.  For this reason, the RPMDirty bit is set, to tell the RPM ISR
  // that it can't trust the value it calculates.  Similarly, the Throttle-Motor
  // PWM routine, which implements PWM in software, is called so that the pin
  // doesn't stay in the wrong state for too long
     
  
  
  TU_Ramp();         // Ramp up Throttle-Up Setpoint
 
  RPMDirty = 1;      // Tell RPM routine that it has been interferred with 

  
  TFLG12.TOF = 0;     // Clear the timer overflow flag
} 


