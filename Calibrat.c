void ProcessNoLoadData(void)

{
  unsigned short i;
  unsigned char  BigPedal;  // Largest Pedal value recorded
  unsigned char  LilPedal;  // Smallest Pedal value recorded (excluding 0)
  unsigned char  BigRack;   // Largest Rack value recorded
  unsigned char  LilRack;   // Smallest Rack value recorded (excluding 0)
  unsigned short ThisIndex; // Percent Pedal of this reading   
  unsigned short LoRPM;     // Lowest RPM seen
  unsigned short HiRPM;     // Highest RPM seen
 
  // 1st, let's find min and max values for each pot
  
  BigPedal = BigRack = 0;   // Initialize to small value before search
  LilPedal = LilRack = 255; // Initialize to big value before search
 
 
  for (i=0;i<=340;i++)  
  {
    if (BigPedal < PedalPotCal[i])
    {
      BigPedal = PedalPotCal[i];
      LoRPM = i*10;
    }
     
    if (BigRack < RackPotCal[i])
      BigRack = RackPotCal[i];
     
    if ((LilPedal > PedalPotCal[i]) && (PedalPotCal[i]>0))
    {
      LilPedal = PedalPotCal[i];
      HiRPM = i * 10;
    }
     
    if ((LilRack > RackPotCal[i]) && (RackPotCal[i]>0))
      LilRack = RackPotCal[i];
  }
 
 
  // Next, prepare the no load array
  for (i=0;i<=100;i++)
    NoLoadCalArray[i] = 0;
   
  // Now, place the data into the array
  for (i=0;i<=340;i++)
  {
   
    // determine where this point lies between smallest and largest pedal values
    if (PedalPotCal[i]>BigPedal)
      ThisIndex = 0;      // this should never happen
    else
    {
      if (PedalPotCal[i]>=LilPedal)
        ThisIndex = (100 * (BigPedal - PedalPotCal[i]))/(BigPedal-LilPedal); 
      else
        ThisIndex = 101;  // RPM values over around 2200 will go into 101, which is ignored
    }
   
    // Now, place it in the array
    NoLoadCalArray[ThisIndex] = i*10; // i is actually RPM/10 in this context
  }
 
 
  // OK, we would be done now, except there's a chance we missed a couple of points by
  // accelerating the truck too quickly.  So we need to fill any holes in our data
  
  // by definition of the PedalCalIndex, we know we'll never have a hole
  // at index = 0 (which is BigPedal), and index = 100 (which is LilPedal)
  // so we can take our hole search from 1 to 99, which help us not waste
  // a lot of time error checking that we aren't at 0 when we subtract an 
  // index number, or aren't at 100 when we add an index number
  for (i=1;i<=99;i++)
  {
    if  (NoLoadCalArray[i] == 0)  // This is a data hole
    {
      if (NoLoadCalArray[i+1] != 0)  // only one hole, not two in a row      
        NoLoadCalArray[i] = (NoLoadCalArray[i-1]+NoLoadCalArray[i+1])/2; // average them      
      else  // two holes in a row.  Fix the 1st one.  Next will get fixed when we get to it
        if (NoLoadCalArray[i+2] != 0)  // two holes in a row, but not three        
          NoLoadCalArray[i] = (NoLoadCalArray[i+2]-NoLoadCalArray[i-1])/3 + NoLoadCalArray[i-1];        
        else // more than two holes in a row.  Forget interpolation, just duplicate
          NoLoadCalArray[i] = NoLoadCalArray[i-1];
    }
  }
  
  
  
  // OK, go ahead and write data to flash
  
  // 1st, rember what's in flash now
  FieldDataRead();  // Remember what's in flash now
  
  // 2nd, set the Cal Parameters that we've just learned
  
  // Confirm that data did change before going through with this  
  if ( ((BigPedal-LilPedal)>10) && ((HiRPM-LoRPM)>300) )
  {
    for (i=0;i<=100;i++)
      CalRPMSetpoint[i] = NoLoadCalArray[i]; 
    
    CalMinPedal = BigPedal;
    CalMaxPedal = LilPedal; 
    CalLowIdle  = LoRPM;
    CalHighIdle = HiRPM;
  
  
    // Some trucks won't have throttle-up, so won't have a rack pot
    if ((BigRack-LilRack)>10)  // Make sure it actually changed
    {
      CalMinRack  = BigRack;
      CalMaxRack  = LilRack;
    }
    else
    {
      CalMinRack  = BigRack;  // don't want to have rack errors on non-throttle-up trucks
      CalMaxRack  = 0;
    }
    
    
     // Some trucks won't have throttle-up, or the user didn't cal the throttle-pot
     // so only change if it did change 
    if ((BigHoist-LilHoist)>10)  // Make sure it actually changed
    {
      CalMinHoist  = BigHoist;
      CalMaxHoist  = LilHoist;
    }
    else
    {
      CalMinHoist  = BigHoist;  // don't want to have hoist errors on non-throttle-up trucks
      CalMaxHoist  = 0;
    }

  
    // 3rd, write the data back
    FieldDataWrite(); // Write back field calibration parameters

  }
  
  
  
  ThrottleCalFlag = 0;  // Tell System That we are done calibrating

  // Reset Errors, as they probably are in error because of old bad calibration
  TempEventCode = 0;
  EventCode = 0;  
}



void GatherThrottleData(void)
// The No-Load Curve tells the system what RPM to expect for any given pedal position 
// or any given rack position.  (Because both of these are scaled in percents, either
// one can be used to index the No-Load Curve)
// The No-Load Curve is used in several places in the code.  It is used to 
// determine what RPM the truck should be at, and therefore how much droop
// there is, during power adapt.  It is used to determine what RPM the 
// throttle-up system is attempting to achieve by indexing using the 
// rack potentiometer, while throttling up.  Finally, it is used to determine
// the no-load RPM, expected at the current pedal position, with said data then
// being used to reference the AccelRef curve, which is indexed on No-Load RPM.
// Needless to say, it is quite important that the No-Load Curve be accurately
// calibrated.  Truck to truck variations in pot mounting and throttle-cable 
// tensioning, result in the necessity for each truck to be calibrated individually.

// This routine will only be called if in "ThrottleCal Mode"
// It will be called repeatedly, as part of the main loop, while in this mode so that
// serial operations and such can continue to be called.  Therefore, this routine doesn't
// act "all at once", but instead, acts as a data capture routine a little bit at a time.
{


  // Read Hoist, in case they are doing a calibration
  if (Hoist<LilHoist)
    LilHoist = Hoist;
    
  if (Hoist>BigHoist)
    BigHoist = Hoist;
    
    
  //  Average RPM's 
  RPMAverager = 0;     // zero out averaging holding register
  for (RPMAverageCounter2=0;RPMAverageCounter2<RPMCOUNTS-1;RPMAverageCounter2++)
    RPMAverager += (unsigned long)(OldRPM[RPMAverageCounter2]);
  RPMAvg = RPMAverager/RPMCOUNTS;  // Calculate average RPM
  

  PedalPotCal[RPMAvg/10] = UnScaled[PEDAL_INPUT];   // Raw value from A/D of Accel Pedal Pot  
  RackPotCal[RPMAvg/10] =  UnScaled[RACKPOS_INPUT]; // Raw value from A/D of Rack Pot
  
  
  // A 10 second timer begins timing after the RPM has exceeded (FULLGOVSPD-100) RPM 
  // Once this timer times out, we proceed to Processing the data
  if (Timer[THRTLCAL_TIMER].EN && Timer[THRTLCAL_TIMER].DN)
     ProcessNoLoadData();  // Process the data and write to flash  
 
  // If ThrottleCalFlag is 0, then this routine won't be called again, which means we're aborted
  // We want to abort, if the cal takes too long, as that indicates a problem or accidental calibration.
  if (Timer[CALABORT_TIMER].DN)
    ThrottleCalFlag = 0;         // Tell System that we've aborted the calibration    

}


void StartThrottleCal(void)
// clear contents of calibration arrays in preparation for cal procedure
// and sets the ThrottleCalFlag, which kicks off the rest of cal procedure
{
  unsigned short i;
  
  ThrottleCalFlag = 1;  // Tell System That we have begun calibrating
  
  BigHoist = 0;     // Init to small value, in case they are doing a hoist cal
  LilHoist = 255;       // Init to big value, in case they are doing a hoist cal
  
  // 1st, initialize the data arrays to known values
  for (i=0;i<=340;i++)
  {
    PedalPotCal[i] = 0;  
    RackPotCal[i] = 0;
  }
  
}

