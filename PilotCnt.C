
void PickPilot(void)
// called by PilotControl to choose minimum value of all possible pilot solutions
{
  
  // *** Pick the Lowest Pilot Solution from all possible solutions ***
  LowestPilot = BIGPILOT;  // start with a large number
  for (PilotCounter=0;PilotCounter<PILOTTYPES;PilotCounter++)  
  {
    if (Pilot.Value[PilotCounter]<LowestPilot)
      LowestPilot = Pilot.Value[PilotCounter];  
  }

  // *** Make sure we picked one ***
  if (LowestPilot != BIGPILOT)   
    ModifiedPilot = LowestPilot;  // Set to minimum pilot solution
  else
    ModifiedPilot = 0;    // something's wrong, this should never execute
 
  
    
    
  // *** Check for Limp-Home Mode ***  
  if (FaultMode == LIMPHOME)  // if Limp Home Mode
    if (ModifiedPilot>LimpHomePilot)
      ModifiedPilot = LimpHomePilot;  


  // *** Compensate for Oil Temperature ***  
  TempCompMultiplier = (LoopTemp*100)/556 + 73;  
  TempCompPilot =  ModifiedPilot*TempCompMultiplier/100;

  
  // *** Un-Compensate if in Power Adapt Mode ***  
  if (Pilot.Name.Power<TempCompPilot)
    TempCompPilot = Pilot.Name.Power; 


  // *** Compensate for Engine Speed ***
  // On the SOMAT, putting the pilot solenoid in manual and ramping RPM
  // we can see that pilot pressure responds to changes in RPM
  // according to the equation delta Pilot = (delta RPM)/145
  // The delta RPM will be the current RPM - the RPM we should be at the current pedal position
  // The pilot/duty cycle table is calibrated at RPMSetpoint, so altering the pilot here
  // in the opposite direction from the way RPM is altering it, should cancel the RPM effect,
  // giving us the pilot we would have had, if RPM didn't have an effect on pilot.
  RPMCompPilot = TempCompPilot - (RPM - RPMSetpoint[PedalIndex])/145;
  if (RPMCompPilot<0)
    RPMCompPilot = 0;
 

  // *** Set Output Pilot Pressure ***    
  PilotOut = RPMCompPilot; 

  
  // *** Limit Output Pilot Pressure to not mis-index PWM table ***    
  if (PilotOut>600)
    PilotOut = 600;



  // *** Set PWM Duty Cycle *** 
  PilotDuty = PilotPWM[PilotOut]; // Duty Cycle necessary to generate pressure
  
  
}


void GetTemperaturePilot(void)
{
// This function "clips" pilot pressure according to how much the truck transmission 
// temperature is over the maximum operating temperature.

 if (LoopTemp>=OverTemperature1)
    OTempMode1 = 1;
    
  if (OTempMode1 && (LoopTemp<=(OverTemperature1-2)))
    OTempMode1 = 0;
    
    
    
  if (LoopTemp>=OverTemperature2)
    OTempMode2 = 1;
    
  if (OTempMode2 && (LoopTemp<=(OverTemperature2-2)))
    OTempMode2 = 0;
    
    
  
  
    
  if (!OTempMode1)
    TemperaturePilot = TempPilot0;  // Full Pilot  
  else        
    if (!OTempMode2)          
      TemperaturePilot = OverTempPilot1; // 1st Pilot Reduction    
    else     
      TemperaturePilot = OverTempPilot2; // 2nd Pilot Reduction
        
      
     
    

  Pilot.Name.Temp = TemperaturePilot;   
  
}


void GetAccelRef(void)
{
// This routine looks up Accel Reference Pilot Pressure, which is indexed by 
// No-Load RPM, which in turn, is indexed by Throttle Pedal Position
// However, after a Pilot Pressure is determined, it may be modified (reduced)
// depending upon whether the engine RPM is higher than the No-Load RPM for 
// the current pedal position.

// It is reduced for two reasons.  If the engine is being back-driven from the
// transmission, we reduce pilot pressure so that there is less of a mechanical
// coupling between the engine and the transmission.  This makes a smoother
// power reversal.

// Or, if we are engaged in a Throttle-Up operation, we may choose to reduce pilot
// pressure so that the increase engine RPM doesn't cause the truck to increase
// it's ground speed.

  // ** Look up Accelerator Pedal Pilot Reference Curve **
  
  unsigned short CalcRPM;  // Don't let RPM change while we're calculating

  CalcRPM = RPM;  // Lock in at this RPM value, for the entire calculation

  RPM_SPIndex = RPMSetpoint[PedalIndex]/10;


  if ( !( ThrottleUpActive || Timer[THRTLUPOFF_TIMER].TT) ) // if not (throttling up, or recently throttling up)
  { // Begin Non-Throttle-Up Accel Ref Routine
  
    // If we are overspeeding (the engine RPM is greater than it should be
    // for this pedal position, than we assume that we are driving the engine
    // from the transmission, as the result of going down a hill, or a power reversal.
    // We decouple the transmission from the engine by reducing pilot pressure
    // so that during power reversals, we have a more smooth deceleration.
    
    
    if ((CalcRPM > (RPMSetpoint[PedalIndex]+100) ) && (RPMSetpoint[PedalIndex]<2000) )// if overspeeding    
    {    
      if ( (CalcRPM - (RPMSetpoint[PedalIndex]+100) ) <= 600)  // don't break your math!
        PctAccelRef = 100-((CalcRPM - (RPMSetpoint[PedalIndex]+100) )/6);    
      else
        PctAccelRef = 0;
    }
    else
      PctAccelRef = 100;
      
    Pilot.Name.Accel  = (PctAccelRef*AccelRefPilot[RPMSetpoint[PedalIndex]/10])/100;

  } // End Non-Throttle-Up Accel Ref Routine
  else  // Throttle-Up Active, so use different scheme
  { // Begin Throttle-Up Accel Ref Routine
    // If we are throttling-up, we want to minimize change in travel speed
    // caused by the increase in engine speed.  This is accomplished by 
    // reducing pilot pressure based on engine overspeed.           


    // We want to anticipate where the engine RPM will be a quarter of a second or so 
    // in the future, if engine speed is increasing.  Otherwise, we "get behind", and
    // the truck speeds up, then slows down, when the control finally catches up.
    // Therefore, instead of looking at RPM, we look at Throttle-Up setpoint.  Or
    // something that anticipates where Throttle-Up setpoint will be in the future.      

    if (Throttle_SP<Throttle_SPFinal)  // Engine speed is INCREASING toward Throttle_SPFinal
    {
      if (Throttle_SPFinal>HighIdle)
        PctTUCorrection = 100;
      else
      {
        if (Throttle_SPFinal>RPMSetpoint[PedalIndex])
          PctTUCorrection = 100 - (100 * (HighIdle - Throttle_SPFinal))/(HighIdle-RPMSetpoint[PedalIndex]); 
        else
          PctTUCorrection = 0;
      }             
    }
    else // Engine speed is DECREASING toward Throttle_SPFinal
    {      
      if (Throttle_SPFinal>RPMSetpoint[PedalIndex])   
        RPMAnticipateVariable = Throttle_SPFinal;
      else
        RPMAnticipateVariable = RPMSetpoint[PedalIndex];       
      
      if (RPMAnticipateVariable>HighIdle)
        PctTUCorrection = 100;
      else
      {
        if (RPMAnticipateVariable>RPMSetpoint[PedalIndex])
          PctTUCorrection = 100 - (100 * (HighIdle - RPMAnticipateVariable))/(HighIdle-RPMSetpoint[PedalIndex]); 
        else
          PctTUCorrection = 0;
      }         
    }            
    
   
    // Look up what pilot pressure would be to maintain this travel speed if
    // engine goes to HighIdle RPM.  This is looked up from a table that was
    // determined empirically.
    TUCorrectedPilot = CompensatedPilot[AccelRefPilot[RPM_SPIndex]];         
   
    
    // Don't allow pilot to go too low, as we want operator to always feel that accel pedal
    // does control truck speed.  Also want to be able to "crowd" a load.
    if (TUCorrectedPilot<Threshold[RPM_SPIndex])
      TUCorrectedPilot = Threshold[RPM_SPIndex];              
    
      
      
    // Determine how much to subtract from current AccelRef, based on how far
    // from RPMSetpoint[PedalIndex] we are right now
    AccelRefReduction = (AccelRefPilot[RPM_SPIndex]-TUCorrectedPilot)*PctTUCorrection/100;
       
    if ( (AccelRefReduction>=0) ) //&& !TUFuncWasTilt) 
      Pilot.Name.Accel  = AccelRefPilot[RPM_SPIndex]-AccelRefReduction;
    else
      Pilot.Name.Accel  = AccelRefPilot[RPM_SPIndex];     
        
   
  }  // End Throttle-Up Accel Ref Routine
}


void OverSpeedAdapt(void)   
{     
    if (!InRetardMode)
      Pilot.Name.OverSpeed = BasePilot[RPMIndex];
    else
      Pilot.Name.OverSpeed = RetardBasePilot[RPMIndex];


    // When the retard valve opens up, there will be a surge in pilot supply, which will
    // suddenly give us the pilot we thought we were getting all along.  This would cause a jolt
    // unless we limit the pilot we ask for to what the pilot can supply, and then
    // slowly ramp up what we ask for, to merge back into the regular control scheme.

    // OverspeedPilotSP is constantly striving to ramp towards 540 at 250 psi/sec.  It spends
    // most of it's life equalling 540.  However, the instant the retard valve is closed, after
    // it has been open during a retard event, OverspeedPilotSP is forced down to 400, where it
    // must ramp it's way back up to 540 slowly.  This ramping smoothes out the truck dynamics
    // when the retard valve closes.

    if (InRetardMode)          // Force ramp variable to low value, so it can start to ramp up from there
      OverSpeedPilotSP = 400;  // at 250 psi/sec the instant we finally exit Retard Mode
  
    if (Pilot.Name.OverSpeed>OverSpeedPilotSP)
      Pilot.Name.OverSpeed=OverSpeedPilotSP;
      
  
} // End OverSpeedAdapt function


void PIDPowerAdapt(void)   
{     
   
   // This routine is used to attempt to control engine speed to 200 RPM less than the
   // no load engine speed for the current pedal position.
   // Full Governed Speed of 2200 - 200 is full torque at around 2000 when floored

    
   // fRPM_SP is a floating value, that is ramping towards RPMSetpoint[PedalIndex] at some rate
   // this ramping is done in the OC3_ISR routine.
   RPMError = RPM - (fRPM_SP-200); // Calc Error to use for PID           

   paCVp = paKp*RPMError;  // Calc Proportional Term

   // When using +=, doesn't allow the RPMError/2.  (worked with just RPMError, but with RPMError/2, 
   // the Math breaks for some reason so must use long format below.)

   paCVi = paCVi + paKi*(RPMError/2); // Calc Integral Term      
 
   paCV = (paCVp/50 + paCVi/50);    // was paCVi/50
    
   // Power Adapt needs to be kept close to AccelRef, but not too close.  
   // I added the +50 on Oct 18,1999 to keep Power Adapt from interfering with AccelRef, 
   // causing noise in pilot, when pilot should be reading just AccelRef, 
   // and throttle pedal is dead steady.  (should be noiseless then)
   if ((paCV>(AccelRefPilot[RPM_SPIndex]+50)) && (AccelRefPilot[RPM_SPIndex]<=600))  
    {
      paCV =   AccelRefPilot[RPM_SPIndex]+50;  // limit operating range of PID
      paCVi = (50*paCV - paCVp);  // reset integral within limit
//      paCVi = (500*paCV - 10*paCVp);  // reset integral within limit
    }     
                 
    if (paCV<40)   
    {
      paCV = 40;  // 40 limit operating range of PID
      paCVi = (50*paCV - paCVp);  // reset integral within limit
//      paCVi = (500*paCV - 10*paCVp);  // reset integral within limit
    } 

    
    Pilot.Name.Power = paCV;

   
} // End PowerAdapt function


void GetBasePilot(void)
// This routine looks up BasePilot Pressure from table, indexed to current PRM
// however, it also limits BasePilot, if charge pressure is too low
{
  // *** Power Truck down, if Charge Pressure is below Charge Limp Curve ***
  if (ChargePress<ChargeLimp[RPMIndex])  // Need to Power Down Truck
  {
    // at ChargeLimp, we want full BasePilot
    // at ChargeShutoff, we want LimpHome Pilot
    // at intermediate points, we want to linearly interpolate between these
    
    
    // Determine what percentage of pilot pressure to use
    if (ChargePress>ChargeShutoff[RPMIndex])  // Need to interpolate between basepilot and limphome
      PctBasePilot = (100*(ChargePress-ChargeShutoff[RPMIndex]))/(ChargeLimp[RPMIndex]-ChargeShutoff[RPMIndex]);
    else
      PctBasePilot = 0;

    // Determine BasePilot Pressure, based on above percentage
    if (BasePilot[RPMIndex]>LimpHomePilot)
      Pilot.Name.Basic = ((PctBasePilot*(BasePilot[RPMIndex]-LimpHomePilot)/100)+LimpHomePilot);
    else
      Pilot.Name.Basic = BasePilot[RPMIndex];    
  }
  else  // *** Charge Pressure is great, so just do a standard BasePilot Look-Up ***
  {  
    if ((!IgnoreRPM) && (RPM<2300))  
      Pilot.Name.Basic = BasePilot[RPMIndex];  // standard base pilot lookup
    else
      Pilot.Name.Basic = 540;  // Don't want RPM to limit pilot if in IgnoreRPM mode
  } 
}

void PilotControl(void)
// was called by main loop to determine Pilot Pressure
// but now called by TOF_ISR() so it happens at fixed rate for PID's
{    
    
  
  // ** Check if recent shift of direction **
  // This boolean variable is used to switch off bits of code that limit pilot pressure
  // based upon RPM value.  It's purpose is to allow accel ref to push the pilot pressure
  // up high quickly, which will act to minimize false reversing
  IgnoreRPM = ( (RPM<2300) &&                                          // not overspeeding and 
                (   (Timer[FWDSWITCHOFF_TIMER].TT && ReverseSwitch)     // just switched to reverse
                 || (Timer[REVSWITCHOFF_TIMER].TT && ForwardSwitch) ) ); // or just switched to forward


   
  // ** Look up Base Pilot Curve **
  GetBasePilot();


  // ** Look up Accel Reference Curve **
  GetAccelRef();
  
  
  // ** Look up Brake Pilot Curve ** 
  BrakeSPFinal = BrakePilot[BrakePress];
  Pilot.Name.Brake = BrakeSP;     // BrakeSP derives from BrakeSPFinal in OC3_ISR

  
  // ** Limit Pilot based on temperature, if needed  **
  GetTemperaturePilot();    
  
 
  // Calculate OverSpeed Adaption Pilot
  OverSpeedAdapt();

  // Calculate Power Adaption Pilot 
  PIDPowerAdapt();   //  PID PowerAdapt Routine
  
  
  // Pick minimum value from all pilot options and temperature compensate
  PickPilot();    
}


#pragma TRAP_PROC SAVE_REGS
void OC3_ISR(void)    // Called by Output Compare 3 Interrupt      
// Handles Pilot Ramping
// This routine is executed every 4ms.  It is executed this often to allow 
// it to also be used to generate the 250Hz dither required for the solenoid.
{                                                                             
  // ***************** Set up next output compare interrupt *****************   
  // We know the timer times at 13.107MHz.  So, each Count is 76.3 nanosecond, 
  // but our prescaler is 4 so for us, each tick is 305.18 nanoseconds
  // 1/250Hz is 4ms which is 13,107 counts
  

  // TCNT is a 16 bit free running timer timing at 305.18 nanoseconds per tick
  // (assuming a System Clock of 13.107MHz)
  // TOC4 tells GPT Module when to request OC4 interrupt again
  if (TCNT<52428)
    TOC3 = TCNT + 13107;
  else
    TOC3 = 13107-(65535-TCNT);
    
  // ********** Power Adapt Setpoint Ramping *********  
  
  // OK, this is a clever bit.  We needed a way to inhibit Power Adapt from Power Adapting
  // when the operator stomps the pedal.  If we think about it for a minute, we see that to 
  // the computer, the case of a pedal stomp, where the engine hasn't had an opportunity to
  // respond yet, and the case of an actual engine lug down, are indistinguishable. 
  // That was a problem because if we power adapt after a pedal stomp, we won't have crisp
  // acceleration of the truck, because we'll crash pilot pressure, as we mistakenly try to
  // unload the engine in response to this huge lug down we think we see.
  
  // Many methods were tried, but the one that had greatest robustness was as follows.
  // We ramp the RPM setpoint that Power Adapt will attempt to control to at some ramp rate
  // so that in cases of a pedal stomp, the power adapt setpoint leisurely increases, at about
  // the same rate as the engine RPM.  This keeps power adapt from cutting in when it should not.
  
  // This low pass filtered RPMSetpoint is also used in the AccelRef routine, so that when the 
  // operator rips his foot from the throttle, and the engine hasn't responded yet, the computer
  // doesn't interpret it as an overspeed situation, and crash pilot down.
  
  
  // We will always be ramping toward the current no-load rpm for current pedal position
  fRPM_SPFinal = RPMSetpoint[PedalIndex];  // Look up No-Load RPM we will ramp towards
  
       
  // Below, is our ramp, where we ramp power adapt RPM setpoint toward RPM_SPFinal, which is 
  // calculated in the PowerAdapt function.  
  if (fRPM_SP<fRPM_SPFinal)  
    fRPM_SP += fRPM_SP_Inc;
  else
    fRPM_SP -= fRPM_SP_Inc;
    
    
    
  // ********** Brake Setpoint Ramping *********  
  // A quick pop on the brake was causing power-adapt to cut in
  // so we fix it by ramping pilot back up from a brake event

  //   Brake_SP_Inc = 3;  // 750 psi/sec    
  Brake_SP_Inc = 2;  // 500 psi/sec    
  
  // Ramp Brake Pressure Setpoint, but only one direction
  if (BrakeSP<BrakeSPFinal)
    BrakeSP += Brake_SP_Inc;
  else
    BrakeSP = BrakeSPFinal;  


 
  OverSpeedPilot_SP_Inc = 1;  // 250 psi/sec
  // Ramp Overspeed Pressure Setpoint, but only one direction
  if (OverSpeedPilotSP<540)
    OverSpeedPilotSP += OverSpeedPilot_SP_Inc;
  else
    OverSpeedPilotSP = 540;  
  
 
  // When this interrupt occurs, it takes long enough that we can't guarantee
  // that we will catch the next RPM interrupt.  Therefore, just give up gracefully
  RPMDirty = 1;      // Tell RPM routine that it has been interferred with 
  
  TFLG12.OCF3 = 0;   // Clear OC3 Flag  
}


