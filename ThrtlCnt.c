
void TM_PID(void)  
// This must be called at a fixed rate, therefore, needs to be called by TOF_ISR.
// The output of this PID is the on-time for the Throttle-Motor PWM Routine to
// control the RPM using the throttle-motor

// This routine executes every 0.02 seconds
{  // Start of Throttle Motor RPM Control function 
                                
  // Don't Control RPM during ,,,,
  if (RPM>DEADTHRES)
  {  

/*    
    // following was added 9/13/99 to simulate lift lever switch 
    if (Throttle_SPFinal<=810)
      Throttle_SP = Throttle_SPFinal;  // simulate lift lever switch killing throttle-up quickly
*/
  
    // Don't let tCVi "wind-up" when throttle-up not active
    if (Throttle_SP<810)
    {
      tCV = 0;
      tCVi = 0;
      tFF  = 0;
      ThrottleUpActive = 0;  // Tell rest of program we're not throttling up      
    }
    else
    {     
      ThrottleUpActive = 1;  // Tell rest of program that throttle-up is operating

      tFF = tKFF; 
    
      // Calculate Current Throttle Error
      ThrottleError = (signed short)(Throttle_SP - RackRPM); // Calc Error to use for PID 
               
      //Divide ThrottleError by 10 to make it harder to overflow variables
      ThrottleError /= 10;

/*          
      // limit ThrottleError to keep from overflowing variables
      if (ThrottleError>100)
        ThrottleError = 100;
      
      if (ThrottleError<-100)
        ThrottleError = -100;
      
*/    
    
      tCVp  = (float)(tKp*ThrottleError);  // Calc Proportional Term
   
      tCVi += (float)(tKi*ThrottleError/10);  // Calc Integral Term          
      
//      tCVp  = (float)(testChar1*ThrottleError);  // Calc Proportional Term
   
//      tCVi += (float)(testChar2*ThrottleError/10);  // Calc Integral Term          

    
  
/*    
      if (tCVp>TMPWMCounts)
        tCVp = TMPWMCounts;
      
      if (tCVp<-TMPWMCounts)
        tCVp = -TMPWMCounts;
      
      if (tCVi>TMPWMCounts)
        tCVi = TMPWMCounts;
      
      if (tCVi<-TMPWMCounts)
        tCVi = -TMPWMCounts;
      
*/     
    
      tCV = (signed short)(tCVp + tCVi +tFF);  // Calc solution to PID equation    
   
    
       
      // Limit PID Output to within PWM Range
      if (tCV>TMPWMCounts)
      {
        tCV = TMPWMCounts;  // limit operating range of PID
        tCVi = tCV - (tCVp + tFF);  // reset integral within limit
      } 
  

      if (tCV<0) 
      {
        tCV = 0; // limit operating range of PID
        tCVi = tCV - (tCVp + tFF);  // reset integral within limit
      } 
      
    }
    
    ThrottleOnTime = (unsigned short)(tCV);    
  }
  else
  {
    ThrottleOnTime = 0;  
    ThrottleUpActive = 0;  // Tell Rest of program we're NOT throttling up
  }
} // End of Throttle Motor RPM Control function








void TU_Ramp(void)
// Ramp throttle setpoint toward final setpoint
{

  Throttle_SPFinal = TUSetpoint[HoistIndex];  // RPM Setpoint for Throttle-Up
  
      
  if ((TiltSwitch) && (Throttle_SPFinal<TiltSP)) // Use TiltSP as RPM SP, if TiltSwitch
  {
    Throttle_SPFinal = TiltSP;                   // ..is active and TiltSP is greater
    TUFuncWasTilt = 1;  // set flag that remembers if in tilt mode or not
  }     
    
  // This routine is called every 0.02 seconds, so if increment rate is 1, then
  // the ramp rate is 50 RPM per second.
  
 
  
  // Ramp Throttle_SP for Throttle-Up PID  
  if (Throttle_SP<Throttle_SPFinal)
  {
    Throttle_SP = Throttle_SP + Throttle_SPInc;
    
    // Don't ramp to current pedal pos RPM
    if (((TUSetpoint[HoistIndex]>810) || TiltSwitch) && (Throttle_SP<RPMSetpoint[PedalIndex]))
      Throttle_SP = RPMSetpoint[PedalIndex];
  }
  else
  {
    Throttle_SP = Throttle_SP - Throttle_SPInc;
  }
    
    
  // Ramp Throttle_SPx4 four times as fast for Pilot compensation during throttle-up  
  if (Throttle_SPx7<Throttle_SPFinal)  
    Throttle_SPx7 += 4*Throttle_SPInc;      
  else
    Throttle_SPx7 -= 4*Throttle_SPInc;    

}


