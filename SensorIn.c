
void CheckSensors(void)  // look for problems with sensor data
{ 
  // This function checks for unscaled, raw A/D values which are not consistant
  // with what is expected from the sensors.  This indicates a sensor failure
  // of some kind.     
  
  unsigned short scratchMin;  // Used in checking some sensors
  unsigned short scratchMax;  // Used in checking some sensors


  
  // *** Check Health of Rack Position Transducer ***  
  scratchMin = MaxRack;
  scratchMax = MinRack;
  
  // let them drift a little off of calibration
  if (scratchMin>10)
    scratchMin -= 10; // Let drift 10 counts below calibration
  else
    scratchMin = 0;   // or to zero, if calibration point is close to zero

  if (scratchMax<245)
    scratchMax += 10; // Let drift 10 counts above calibration
  else
    scratchMax = 255;   // or to 255, if calibration point is close to 255  
  
  if (   (UnScaled[RACKPOS_INPUT] < scratchMin)  ||
         (UnScaled[RACKPOS_INPUT] > scratchMax) )
      {
        SensorFail[RACKPOS_INPUT] = 1;
      }
    else
      SensorFail[RACKPOS_INPUT] = 0;


       
  // *** Check Health of Hoist Lever Transducer ***
  // Hoist Lever Position -- Used for Throttle-Up Systems
  scratchMin = MaxHoist;
  scratchMax = MinHoist;
  
  // let them drift a little off of calibration
  if (scratchMin>10)
    scratchMin -= 10; // Let drift 10 counts below calibration
  else
    scratchMin = 0;   // or to zero, if calibration point is close to zero

  if (scratchMax<235)
    scratchMax += 20; // Let drift 20 counts above calibration
  else
    scratchMax = 255;   // or to 255, if calibration point is close to 255  
  
  if (   (UnScaled[HOISTPOS_INPUT] < scratchMin)   )//  ||
    //   (UnScaled[HOISTPOS_INPUT] > scratchMax) ) // won't hurt to be off a bit here, elliminate nuisance alarms
      {
        SensorFail[HOISTPOS_INPUT] = 1;
      }
    else
      SensorFail[HOISTPOS_INPUT] = 0;
   


   
  // *** Check Health of Throttle Pedal Transducer ***
  scratchMin = MaxPedal;
  scratchMax = MinPedal;
  
  // let them drift a little off of calibration
  if (scratchMin>20)
    scratchMin -= 20; // Let drift 10 counts below calibration
  else
    scratchMin = 0;   // or to zero, if calibration point is close to zero

  if (scratchMax<235)
    scratchMax += 20; // Let drift 10 counts above calibration
  else
    scratchMax = 255;   // or to 255, if calibration point is close to 255  
  
  if (   (UnScaled[PEDAL_INPUT] < scratchMin)  ||
         (UnScaled[PEDAL_INPUT] > scratchMax) )
      {
        SensorFail[PEDAL_INPUT] = 1;
      }
    else
      SensorFail[PEDAL_INPUT] = 0;   
  
  // *** Check Health of Charge Pressure Transducer ***
  if ((UnScaled[CHGPRESS_INPUT] <= 15) ||            // 26 is 0.5 V
      (UnScaled[CHGPRESS_INPUT] >= 241))             // 230 is 4.5V
    SensorFail[CHGPRESS_INPUT] = 1;
  else
    SensorFail[CHGPRESS_INPUT] = 0;



  // *** Check Health of Brake Pressure Transducer ***
  if ((UnScaled[BRKPRESS_INPUT] <= 15) ||            // 26 is 0.5 V
      (UnScaled[BRKPRESS_INPUT] >= 241))             // 230 is 4.5V
    SensorFail[BRKPRESS_INPUT] = 1;
  else
    SensorFail[BRKPRESS_INPUT] = 0;



  // *** Check Health of Temperature Transducer ***
  if (UnScaled[OILTEMP_INPUT] >= 220)               // 220 is 264 degF 
    SensorFail[OILTEMP_INPUT] = 1;
  else
    SensorFail[OILTEMP_INPUT] = 0;
        

}



void ReadAnalog(void)
// Read all Analog Data from A/D Converter
{  

  unsigned short scratchVal;  // Used as intermediate value in scaling calcs
  
  // Transfer results from A/D into an array of values
  Raw[0] = RSLT0;   // Read Raw A/D Value Channel 0
  Raw[1] = RSLT1;   // Read Raw A/D Value Channel 1
  Raw[2] = RSLT2;   // Read Raw A/D Value Channel 2
  Raw[3] = RSLT3;   // Read Raw A/D Value Channel 3
  Raw[4] = RSLT4;   // Read Raw A/D Value Channel 4
  Raw[5] = RSLT5;   // Read Raw A/D Value Channel 5
  Raw[6] = RSLT6;   // Read Raw A/D Value Channel 6
  Raw[7] = RSLT7;   // Read Raw A/D Value Channel 7      


  
  // There are more Analog Inputs than there are Analog Channels on the HC16
  // so a mux is used so more than one analog signal comes in on one of the inputs.
  // The following code stuffs the appropriate analog value into it's variable.
  UnScaled[SPARE_INPUT]    = Raw[SPARE_MUX_INDEX];
  UnScaled[OILTEMP_INPUT]  = Raw[OILTEMP_INDEX];
  UnScaled[BRKPRESS_INPUT] = Raw[BRKPRESS_INDEX];
  UnScaled[CHGPRESS_INPUT] = Raw[CHGPRESS_INDEX];
  UnScaled[PEDAL_INPUT]    = Raw[PEDAL_INDEX];
  UnScaled[THRCUR_INPUT]   = Raw[THRCUR_INDEX];
  UnScaled[SOLCUR_INPUT]   = Raw[SOLCUR_INDEX];
  if (MuxCtr>1)  // Allow Mux time to settle, before reading from it
  {
    if (MuxVal == HOISTPOS_MUX_INDEX)
      UnScaled[HOISTPOS_INPUT] = Raw[MUX_INDEX]; 
    
    if (MuxVal == RACKPOS_INDEX)   
      UnScaled[RACKPOS_INPUT]  = Raw[MUX_INDEX];      
      
    if (MuxVal == PREC_MUX_INDEX)
      UnScaled[PREC_INPUT]     = Raw[MUX_INDEX];
      
    if (MuxVal == PWR_MUX_INDEX)
      UnScaled[PWR_INPUT]      = Raw[MUX_INDEX];  
  }     
      

#ifndef PILOTTRUCK  // if not a pilot truck then
  UnScaled[RACKPOS_INPUT]  = Raw[SPARE_MUX_INDEX];  // remap rack pot to old rack position input
#endif

  // RPM can be thought of as an analog input, even though it is digital pulses,
  // because it is a time varying signal.  Therefore, this is done in this routine.    
  UnScaled[RPM_INPUT]   = RPM;  // Treat this as an analog.
  Scaled[RPM_INPUT] = RPM;      // Store in array for easy access 
 

  // Pilot Pressure we are trying to produce is not an input at all.  However, it is convenient to 
  // treat it as one for the communications link
  UnScaled[PILOT_INPUT]   = PilotOut;  // Treat this as an analog input.
  Scaled[PILOT_INPUT] = PilotOut;      // Store in array for easy access 

  // RPM the throttle-up is trying to produce is not an input at all.  However, it is convenient to 
  // treat it as one for the communications link
  UnScaled[RPMSP_INPUT]   = Throttle_SP;  // Treat this as an analog input.
  Scaled[RPMSP_INPUT] = Throttle_SP;      // Store in array for easy access 


  
  
  // Control Curves are indexed to RPM/10 not to RPM
  RPMIndex = RPM/10;  

  
  // Diesel Injection Pump Position
  Rack = UnScaled[RACKPOS_INPUT];
 
  if (Rack>MinRack)
    RackIndex = 0;
  else
  {
    if (Rack>MaxRack)
      RackIndex = (100 * (MinRack - Rack))/(MinRack-MaxRack); 
    else
      RackIndex = 100;
  } 
  
  Scaled[RACKPOS_INPUT] = RackIndex; // Store in array for easy access   
  
    
//  RackRPML = (((HighIdle-LowIdle)/10)*RackIndex)/10+LowIdle;  
//  RackRPM = (unsigned short) (RackRPML);

   // The No-Load RPM curve defines what RPM we expect from the truck at various
   // Throttle Pedal percentages.  This is only the case, of course, if throttle-up
   // is not active.  Actually, the RackIndex is more precisely a valid index for
   // the No-Load RPM curve, because it is valid always.  Whether Throttle-Up is
   // active or not.  ....  Therefore, we can index the
   // No-Load RPM curve using the RackIndex, rather than the PedalIndex, to get
   // what RPM we expect, at this rack position.
   RackRPM  = RPMSetpoint[RackIndex];  // Look up No-Load RPM for this rack position
  
  
  
    
  // Read Hydraulic Loop Temperature, scale it, 
  // and limit it, to keep it from mis-indexing arrays
  LoopTemp = 12*UnScaled[OILTEMP_INPUT]/10;     // scaled Loop Temperature [degF] 0-250      
  if (LoopTemp>250)
    LoopTemp = 250;    // Don't let a bad sensor mis-index the arrays
    
  // Don't let LoopTemp go below 12 degF, or we'll break our math in temperature 
  // compensation routine by generating too small a number to fit into a signed short
  if (LoopTemp<12)
    LoopTemp = 12;
    
  Scaled[OILTEMP_INPUT] = LoopTemp;  // Store in array for easy access
    
     
  // Read the Brake Pressure, scale it, 
  // and limit it, to keep it from mis-indexing arrays  
 
  //  0.5V = 0 psi  = 25 counts
  //  4.5V = 750psi = 230 counts
  //  so P = ((counts-25)/(230-25))*750
  //  or P = 750*(counts - 25)/205
  //  or 75*(counts-25)/21
  
  // the equations assume that the value will be at least 1/2V (25 Counts)
  // so don't disappoint them, or incorrect values will be calculated
  scratchVal = UnScaled[BRKPRESS_INPUT];
  if (scratchVal<25)
    scratchVal = 25;
  BrakePress = (75*(scratchVal-25))/21;  // scaled Brake Pressure [psi]    

  // Brake Pressure transducer goes to 750 psi, but we only care about 1st 500 psi
  // so limit it here. 
  if (BrakePress>500)
    BrakePress = 500;  // Don't let a bad sensor mis-index the arrays
    
  Scaled[BRKPRESS_INPUT] = BrakePress; // Store in array for easy access
    
  
  // Read the Charge Pressure, scale it, 
  // and limit it, to keep it from mis-indexing arrays
  //  ChargePress = (50*(UnScaled[CHGPRESS_INPUT]-25))/21; // scaled Charge Pressure [psi]
  
  // the equations assume that the value will be at least 0.5V (25 Counts)
  // so don't disappoint them, or incorrect values will be calculated
  scratchVal = UnScaled[CHGPRESS_INPUT];
  if (scratchVal<25)
    scratchVal = 25;

//  ChargePress = ((255*(scratchVal-25))/205);  
#ifdef P6TRUCK  // uses TI 0-1000 PSI transducer
  ChargePress = (100*(scratchVal-25))/21;  // scaled Charge Pressure [psi]    
#else           // all other trucks use standard MSI 0-750 transducer
  ChargePress = (75*(scratchVal-25))/21;  // scaled Charge Pressure [psi]    
#endif
   
  if (ChargePress>250)
    ChargePress = 250; // Don't let a bad sensor mis-index arrays  
    
  Scaled[CHGPRESS_INPUT] = ChargePress; // Store in array for easy access
    
    
  
  // Read the Pedal Input, and than calculate PedalIndex.
  // Pedal Index is limited between 0 and 100 to keep from mis-indexing arrays
  Pedal = UnScaled[PEDAL_INPUT];       // Raw value from A/D of Accel Pedal Pot    
  // Control Curves are indexed to percent pedal stroke, not to the actual A/D
  // remember that Pedal released is higher voltage than Pedal Down, it's backwards 
  
  // To keep from making cable stretch cause you to cut-out, don't send PedalIndex
  // to zero if overrange, but only hold it at 100%
  if (Pedal>MinPedal)
    PedalIndex = 0;
  else
  {
    if (Pedal>MaxPedal)
      PedalIndex = (100 * (MinPedal - Pedal))/(MinPedal-MaxPedal); 
    else
      PedalIndex = 100;
  } 
  
  Scaled[PEDAL_INPUT] = PedalIndex; // Store in array for easy access   
  
  // The RPM No Load Curve tells us where what RPM this truck should be at, for every
  // Pedal Position.  This RPM divided by 10 is used to index the AccelRef Curve.
//  RPM_SPIndex = RPMSetpoint[PedalIndex]/10;
  
  
 
  
  
  // Read the Throttle-Motor PWM input, and scale it
  // it's not used to index arrays, so limiting is not required
  
  // Using a curve fit formula, and picking a fit that is fast to calculate, even
  // if it's not exact, I have the following formula, which is close enough for 
  // Error checking, since we're not using it for control     
  //y = -0.0953x2 + 41.008x
  // so say instead, y = 41*X- X^2/10
  
  // scaled Aux PWM [mA]
  ThrCurrent = 41*UnScaled[THRCUR_INPUT] - UnScaled[THRCUR_INPUT]*UnScaled[THRCUR_INPUT]/10;    
  Scaled[THRCUR_INPUT] = ThrCurrent; // Store in array for easy access   
  
  
  // Read the Solenoid PWM input, and scale it
  // it's not used to index arrays, so limiting is not required
  // (1.000 Amps would read 1000)
  
  // Using a curve fit formula, and picking a fit that is fast to calculate, even
  // if it's not exact, I have the following formula, which is close enough for 
  // Error checking, since we're not using it for control
  // y = -0.2906x2 + 42.691x + 298.36
  // so say instead, y = 298 + 43*X - X^2*3/10
  
  // scaled Solenoid Current [mA]
  SolCurrent = 298 + 43*UnScaled[SOLCUR_INPUT] - 3*UnScaled[SOLCUR_INPUT]*UnScaled[SOLCUR_INPUT]/10;
  Scaled[SOLCUR_INPUT] = SolCurrent;   // Store in array for easy access
  
  
  // Read the Hoist Input, and then calculate HoistIndex.
  // Hoist Index is limited between 0 and 100 to keep from mis-indexing arrays
  Hoist        = UnScaled[HOISTPOS_INPUT];     
  // Control Curves are indexed to percent lift lever stroke, not to the actual A/D
  // remember that Lever released is higher voltage than Lever Down, it's backwards 
    
  // To keep from making linkage misadjustment cause you to cut-out, don't send HoistIndex
  // to zero if overrange, but only hold it at 100%
  if (Hoist>MinHoist)
    HoistIndex = 0;
  else
  {
    if (Hoist>MaxHoist)
      HoistIndex = (100 * (MinHoist - Hoist))/(MinHoist-MaxHoist); 
    else
      HoistIndex = 100;
  }
  
  Scaled[HOISTPOS_INPUT] = HoistIndex; // Store in array for easy access

  
  
  // scaled Precision Voltage (5.000V would read as 5000)
  PrecVoltage  = 39*UnScaled[PREC_INPUT]; 
  Scaled[PREC_INPUT] = PrecVoltage;    // Store in array for easy access
  
  // scaled Spare Voltage   
  SpareVoltage = UnScaled[SPARE_INPUT];
  Scaled[SPARE_INPUT] = SpareVoltage;  // Store in array for easy access
  
  // scaled 12F10 Voltage  (12.000V would read as 12000)  
  PwrVoltage   = 59*UnScaled[PWR_INPUT];
  Scaled[PWR_INPUT] = PwrVoltage;      // Store in array for easy access


  
  // We want to scroll through the Analog Mux, but we want to do it slowly
  // so we can guarantee that we have had sufficient time to read the previous
  // signal before changing the mux.
  if (MuxCtr<3)
    MuxCtr++;
  else
  {
    MuxCtr = 0;
    if (MuxVal<MUX_CHANS-1)
      MuxVal++;
    else
      MuxVal = 0;
  }    
  
  
  // Trigger the A/D to take another reading
  ADCTL1.CHAN = 0;   // Channel Selection Field  
  ADCTL1.S8CM = 1;   // 1=>Eight Conversion Sequence
  ADCTL1.MULT = 1;   // 1=>Convert Block of Channels
  ADCTL1.SCAN = 0;   // 1=>Continuous Conversion  


  MUX_CHANNEL = MuxVal;  // Send Mux Channel Select to hardware   
}


void RPMCheck(void)
// The RPM is calculated using an input capture routine every time
// a new pulse occurs.  If no pulses are occuring however, than it
// never recalculates, and the CPU is fooled into thinking the RPM
// is whatever it was before the pulses stopped comming.  This routine
// is used to zero out the RPM if it hasn't been updated recently
{

  // if the dead threshold is 400 RPM, and there are 45 pulses per revolution
  // than the minimum valid PPS would be 300, which is 3.33 ms between pulses
  // This is executed every 20 ms, so we are more than conservative by saying
  // the RPM is zero if the RPMCounter gets over 24.  This still tells the CPU
  // of an engine stop within 500 ms of it's occurance, which is plenty fast.
  if (RPMCounter>24)  
    RPM = 0;       // reset RPM
  else
    RPMCounter++;  // increment counter
} 



#pragma TRAP_PROC SAVE_REGS
void IC1_ISR(void)    // Called by Input Capture 1 Interrupt   
// This is the Input Capture 1 Interrupt Service Routine
// This routine calculates engine RPM using the input capture interrupt
{    
  Tmr1CapOld = Tmr1CapNew;      // Remember Old Counter Value
  Tmr1CapNew = TIC1;            // Get New Counter Value 
  
  if (RPMDirty)  // if this routine was interferred with
    RPMDirty = 0;  // Reset dirty flag 
  else
  { // begin OK to calculate RPM     
     
    RPMCounter = 0;  // Reset RPMCounter, so we know engine isn't stopped   
    
    if (Tmr1CapNew>Tmr1CapOld)
      PPSPeriod = Tmr1CapNew - Tmr1CapOld;        
    else
      PPSPeriod = (65535-Tmr1CapOld)+Tmr1CapNew;       

    // OK, at this point, we want to calculate RPM.
    // The easy to understand way to do this is as follows.
    // We know the timer times at 13.107MHz 

    // So, each Count is 76.3 nanosecond, but our prescaler is 4
    // so for us, each tick is 305.18 nanoseconds
    // We are counting the time between each pulse
    // so the Pulses per Second or PPS would be 3,276,750/PPSPeriod
      
    if (PPSPeriod>0)
      PPS = (unsigned long)(3276750/PPSPeriod);       // Pulses per Second
    
    RPML = (unsigned long) ( (PPS*60)/PULSESPERREV ); // Revolutions per Minute   
    RPM  = (unsigned short)( RPML );     

  
    if (RPM>3400) // Some of the code uses RPM to reference various tables
      RPM = 3400; // so limit RPM to keep control.  
      
    // Store RPM's in an array for averaging later, if needed
    OldRPM[RPMAverageCounter1] = RPM;  // Remember this RPM value    
    if (RPMAverageCounter1<RPMCOUNTS)
      RPMAverageCounter1++;
    else
      RPMAverageCounter1 = 0;
    
    


  
  } // end OK to calculate RPM 
  
  TFLG12.ICF1 = 0;    // Clear IC1 flag
    
}  



void ReadDigital(void)
// Read all Digital Inputs and place into variables so debugger can see them
{       
  // Port F Inputs -- Map variables to Hardware 
  ForwardSwitch   = ExternalInput.i.FWDSW   = !MYPORTF.FWDSW;     
  ReverseSwitch   = ExternalInput.i.REVSW   = !MYPORTF.REVSW;     
  HoistSwitch     = ExternalInput.i.HOISTSW = !MYPORTF.HOISTSW;   
  PedalSwitch     = ExternalInput.i.PEDALSW = !MYPORTF.PEDALSW;   
  WSLeftB         = ExternalInput.i.WSLEFTB = !MYPORTF.WSLEFTB;  
  TiltSwitch      = ExternalInput.i.TILTSW  = !MYPORTF.TILTSW;    
  SpareSwitch     = ExternalInput.i.SPAREIN = !MYPORTF.SPAREIN;
  
    
  // Port QS Inputs -- Map variables to Hardware
  OldParkSwitch  = ParkSwitch;  // Remember old Park Switch Value
  ParkSwitch     = ExternalInput.i.PARKSW = MYPORTQS.PARKSW; 

  
  // Port GP Inputs -- Map variables to Hardware   
  RPMInput  = ExternalInput.i.RPM      = !MYPORTGP.RPM;      
  WSLeftA   = ExternalInput.i.WSLEFTA  = !MYPORTGP.WSLEFTA; 
  WSRightA  = ExternalInput.i.WSRIGHTA = !MYPORTGP.WSRIGHTA; 
  

  KeySwitch  = ExternalInput.i.KEY_SWITCH = !MYPORTX.KEYSW;       
  PilotEnFETState = ExtDigOut.Sensed.o.PILOTEN = MYPORTX.PILOTEN; // PILOT_ENABLE MOSFET State       
  EngCutFETState = ExtDigOut.Sensed.o.ENGRLY = MYPORTX.ENGRLY;   // ENG_CUT_RELAY MOSFET State     
              
  OverTempFETState = ExtDigOut.Sensed.o.OVRTEMP = MYPORTX.OVRTEMP; // OVER_TEMP_LAMP MOSFET State
  RetardFETState =  ExtDigOut.Sensed.o.RETARD = MYPORTX.RETARD; // RETARD_SOLENOID MOSFET State       
  FaultFETState =  ExtDigOut.Sensed.o.FAULT = MYPORTX.FAULT; // FAULT_LAMP MOSFET State                                        
  GovFETState =  ExtDigOut.Sensed.o.GOVPWM = MYPORTX.GOVFET; // GVNR_MTR_RETURN MOSFET State
  AuxFETState = ExtDigOut.Sensed.o.AUXPWM = MYPORTX.AUXFET; // AUX_PWM_RETURN MOSFET State            
  
   
  
  // PACTL Inputs -- Map variables to Hardware    
  WSRightB = ExternalInput.i.WSRIGHTB = !MYPACTL.WSRIGHTB;  
  ThrotFETState = ExtDigOut.Sensed.o.THRTFET = MYPACTL.THRTFET; 
}     
