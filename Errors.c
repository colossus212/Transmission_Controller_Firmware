void AddEvent(unsigned char EventCodeParam)
{ 
   
  TempEventCode = EventCodeParam;  // Remember Event for Truck Control
  
  // We want to log events that are NEW.  We don't want to waste our buffer
  // re-logging the same errors over and over again.  However, we DO care
  // if an error condition is persistent, as that may have warranty considerations.
  
  // Therefore, we search back in the Event Buffer over the previous hour, looking 
  // to see whether we've already logged the event this hour.  If we have logged 
  // it within the current hour, the Already_Logged flag is set, and the event is 
  // ignored.
  Event_Logged = 0;     // Initialize flag to not logged
  Events_Searched = 0;
  EventSearchHours = Hours; // Initialize Time to search as current hour reading
  EventSearchCounter = EventNumber; // Set search to current pos in event buffer

  while ((EventSearchHours==Hours)  && 
    (Events_Searched<EVENT_BUFF_SIZE))   // Go back in time searching for current event.
  {
    
    if (EventSearchCounter>0)      
      EventSearchCounter--;                 // Go BACK in the event buffer
    else
      EventSearchCounter = EVENT_BUFF_SIZE-1; // wrapping around if necessary           
      
    EventSearchHours = EventBuff[EventSearchCounter].Hours;
    
    Events_Searched++;  // Keep track of how many events we've looked at
    
    if (EventBuff[EventSearchCounter].Event==EventCodeParam)
      Event_Logged = 1;    
  }
  
  
  if (!Event_Logged) // Go ahead and log the event if not already logged this hour
  {  
    EventBuff[EventNumber].Event   = EventCodeParam;
    EventBuff[EventNumber].Hours   = Hours;
    EventBuff[EventNumber].Seconds = Seconds;  
    
    // Set up EventNumber for next event
    if (EventNumber > (EVENT_BUFF_SIZE-1) )
      EventNumber = 0;
    else
      EventNumber++;
  }
}




void SetError(void)    // determine latest error condition
{   
  TempEventCode    = 0;  // Set Temp Event to 0 and see if anything contradicts this
  
  // ***** Events less than 20 are logged, but do not cause a mode change.   *****
  // ***** These conditions do not light the fault light                     *****

    
 
  // ***** Errors 20 to 29  are logged, but do not DIRECTLY cause a mode change  *****
  // ***** These conditions DO light the fault light                             *****
  // ***** ... BUT ONLY as long as the condition persists                        *****

  if (LoopTemp<TEMPLO_POINT)
    AddEvent(TEMPLO_ERR);          //  Loop Temperature Lo

  if (LoopTemp>TEMPHI_POINT)
    AddEvent(TEMPHI_ERR);          //  Loop Temperature Hi

  if (Timer[FUSEBAD_TIMER].DN)
    AddEvent(FUSEBLOWN_ERR);       //  10A Fuse Blown
    

  // ***** Errors 30 to 39 are logged, but do not DIRECTLY cause a mode change  *****
  // ***** These conditions DO light the fault light                            *****
  // ***** ... which will remain lit until truck power is cycled                *****
 
  if (Timer[CHARGELO_TIMER].DN)
    AddEvent(CHARGELO_ERR);        //  Charge Pressure Low for 2 seconds 
 
  
    
  // ***** Errors 40 to 49 cause LIMP HOME MODE                  *****
  // ***** These conditions DO light the fault light             *****
  // ***** ... which will remain lit until truck power is cycled *****

  if (Timer[CHARGEBAD_TIMER].DN)
    AddEvent(CHARGESF_ERR);       // Charge Pressure Sensor Failure
 
  if (Timer[PEDALBAD_TIMER].DN)       
    AddEvent(PEDALSF_ERR);        // Throttle Pedal Sensor Failure      
    
  if (Timer[TEMPBAD_TIMER].DN)      
    AddEvent(TEMPSF_ERR);         // Oil Temperature Sensor Failure
  
  if (Timer[BRAKEBAD_TIMER].DN)       
    AddEvent(BRAKESF_ERR);        // Brake Pressure Sensor Failure
  
  if (Timer[RETARDBAD_TIMER].DN)   
    AddEvent(RETARD_ERR);          // Retard Valve in wrong state for 1 second 
  
  if (Timer[RPMBAD_TIMER].DN)   
    AddEvent(RPMSF_ERR);          // RPM Sensor Failure
  

  
  // ***** Events 50 to 59 cause DISABLE THROTTLE MODE           ***** 
  // ***** These conditions DO light the fault light             *****
  // ***** ... which will remain lit until truck power is cycled *****

  if (Timer[HOISTBAD_TIMER].DN)       
    AddEvent(HOISTSF_ERR);        //  Hoist Lever Pot Sensor Failure   

  if (Timer[RACKBAD_TIMER].DN)       
    AddEvent(RACKSF_ERR);         //  Rack Pot Sensor Failure   
 
    
  // ***** Errors 60 to 79 cause EMERGENCY NEUTRAL MODE          *****
  // ***** These conditions DO light the fault light             *****
  // ***** ... which will remain lit until truck power is cycled *****
    
  if (Scaled[SOLCUR_INPUT]>MAXSOLCRNT)         
    AddEvent(SOLCURHI_ERR);       //  Solenoid Current Hi

   // Kept shutting P10 down            
//  if (Scaled[THRCUR_INPUT]>MAXTHRCRNT)       
//    AddEvent(THRMCURHI_ERR);       //  Throttle Motor PWM Current Hi
  
    // Pedal Up Switch causes this to trip on pre-pilot wiring harnesses
//  if (Timer[FWDREVBAD_TIMER].DN) // both switches are active
//    AddEvent(FSWRSWSF_ERR);       //  Forward Sw/Reverse Sw Sensor Failure 
    
  
//  if ((Scaled[THRCUR_INPUT]>3500) && !ThrottleUpActive)
//    AddEvent(THRTLFETS_ERR);     //  Current in Motor when throttle-up not active
    
 
  if (Timer[OVERSPEED_TIMER].DN) // Engine has been overspeed for 1 second
  {
    OverSpeedLatch = 1;
    AddEvent(RPMHIHI_ERR);             // RPM Past Mechanical Tolerance
  }
  



  // ***** Errors 80 to 89 cause EMERGENCY SHUTDOWN MODE         ***** 
  // ***** These conditions DO light the fault light             *****
  // ***** ... which will remain lit until truck power is cycled *****  

  if (Timer[OVERTEMP_TIMER].DN)  // Loop has been OverTemp for 30 seconds
    AddEvent(TEMPHIHI_ERR);            // Loop Temperature HiHi





// ***** Events 90 to 99 cause Emergency Shutdown Mode w/ NO RESTART ALLOWED *****                   
  if (Timer[CHARGELOLO_TIMER].DN)  // LoLo charge press for 2 seconds
    AddEvent(CHARGELOLO_ERR);          // Charge Pressure LoLo

          
    
    
  // At this point, all the events have been checked, and it is OK to transfer
  // the temporary event variable into the standard event variable.  The use of
  // a temporary event variable is to assure no strange behavior due to interupts.
  
  // EventCodes that are greater than 20 may only be cleared by the controller
  // losing it's memory in a PowerDown.  This latches errors, forcing the operator
  // to restart the truck, which is important from a operator and system safety view
  
  if ( (EventCode<30)  // if a no current error or current error is  non-critical
      || (EventCode<TempEventCode)) // OR the new error is even worse than old one
            EventCode = TempEventCode;  // transfer new Event Code into variable
 
 
  // Split EventCode into it's two digits for FlashMode
  EventCode_TensDigit = EventCode/10;
  EventCode_OnesDigit = EventCode-(EventCode_TensDigit*10);           
}




#pragma TRAP_PROC SAVE_REGS
void SPURRIOUS_ISR(void)    // Pointed to by Spurrious Interrupt Vector
{
  // This will be called sometimes during heavy communications
//  AddEvent(SPUR_ERR);       //  Spurious Interrupt Occurred
}


#pragma TRAP_PROC SAVE_REGS
void DIV0_ISR(void)    // Pointed to by Division by 0 Interrupt Vector
{
  AddEvent(DIV0_ERR);      //  Division by Zero Occurred
}

#pragma TRAP_PROC SAVE_REGS
void BUSERR_ISR(void)    // Pointed to by Bus Error Interrupt Vector
{    
   AddEvent(BUSERR_ERR);   //  Bus Error Occurred   
}  

#pragma TRAP_PROC SAVE_REGS
void ILLEGAL_ISR(void)    // Pointed to by Illegal Instruction Interrupt Vector
{
  AddEvent(ILLEGAL_ERR);   //  Illegal Instruction
}  

#pragma TRAP_PROC SAVE_REGS
void MISC_ISR(void)    // Pointed to by non-assigned Interrupt Vectors   
{
  AddEvent(MISCINT_ERR);   //  A non-defined interrupt occurred
}
