
// compile this code into FLASH_ERASE_ROM area, it will be transferred to RAM to be run
#pragma CODE_SEG FLASH_ERASE_ROM_SPACE
void RAM_EraseFlashSector(void)
// This routine will be run from RAM
// So it needs to be forced into the linking in the ENTRIES section of the .PRM
// file.  Otherwise, the linker will outsmart you, and there won't be anything 
// at this address. (Since the linker sees that RAM_EraseFlashSector is never called directly)
{
  unsigned short *SA;   // sector we are erasing   
  unsigned char i;      // used to fool compiler
  // OK, the compiler tried to outsmart me by seeing that I was writing two different values consecutively
  // to the same address, and tried to ignore the 1st value.  Of course, I know better than the compiler
  // why this needs done.  (to put the eeprom into erase mode), so I put in the i assignment to
  // force compiler into doing what we want.


  SA = (unsigned short *far)(EraseSector);      // sector we are erasing in flash   
 
  FLASHCOMMAND1 = 0xAAAA;  // 1st Bus Cycle to initiate Sector Erase
  i = 1;
  FLASHCOMMAND2 = 0x5555;  // 2nd Bus Cycle to initiate Sector Erase
  i = 2;
  FLASHCOMMAND3 = 0x8080;  // 3rd Bus Cycle to initiate Sector Erase
  i = 3;
  FLASHCOMMAND4 = 0xAAAA;  // 4rd Bus Cycle to initiate Sector Erase
  i = 4;
  FLASHCOMMAND5 = 0x5555;  // 5th Bus Cycle to initiate Sector Erase
  i = 5;
  *SA = 0x3030;            // 6th Bus Cycle to initiate Sector Erase     

  while (*SA != 0xFFFF);   // loop till done erasing 
}


#pragma CODE_SEG DEFAULT_ROM // Put this in regular ROM area
void EraseFlashSector(unsigned long Address)
// This is the hook that is used to get to the actual
// Erase Sector Code, which is run from RAM
{   
  EraseSector = Address;  // define address of sector we will be writing/erasing     
 
  asm {     ORP  #0x00E0                   ;// disable interrupts while working with Flash         
            JSR  FLASH_ERASE_RAM_LOC       ;// jump to Erase Sector Routine in Ram 
            ANDP #0xFF1F                   ;// OK to enable interrupts again
      }   

}



// compile this code into FLASH_WRITE_ROM area, it will be transferred to RAM to be run  
#pragma CODE_SEG FLASH_WRITE_ROM_SPACE          
void RAM_WriteFlashByte(void)
// This routine will be run from RAM
// So it needs to be forced into the linking in the ENTRIES section of the .PRM
// file.  Otherwise, the linker will outsmart you, and there won't be anything 
// at this address. (Since the linker sees that RAM_WriteFlashByte is never called directly)
{   
// Program a byte into Flash at a specific address
// must have erased sector byte is in 1st

  unsigned short *PA;   // address of start of area we are programming
  unsigned short PD;    // Data we are programming into this address
  unsigned short ProgCounter; // Used to count time till give up on programming
  
//  FlashStatus_type *EStatus; // Read status of flash EPROM
//  FlashStatus_type ToggleCheck1; // Used to check for toggling DQ6
//  FlashStatus_type ToggleCheck2; // Used to check for toggling DQ6 

 
  PA = (unsigned short *far)(FlashWriteAddress);      // address we are programming in flash  
  PD = FlashWriteValue;                               // data we are programming into flash
//  EStatus = (unsigned short *far)(FlashWriteAddress); // address we are programming in flash  
  
  FLASHCOMMAND1 = 0xAAAA;  // 1st Bus Cycle to initiate Flash Byte Write
  FLASHCOMMAND2 = 0x5555;  // 2nd Bus Cycle to initiate Flash Byte Write
  FLASHCOMMAND3 = 0xA0A0;  // 3rd Bus Cycle to initiate Flash Byte Write   
  *PA = PD;                // 4th Bus Cycle is the address and byte we're programming  

  
  FlashPrograming = 1;   // Tell program we are programming
  ProgCounter = 0;         // Intialize counter to zero counts
  while (ProgCounter<50000)       // loop till done programming, or till time to give up
  {
    if (*PA == PD)
      ProgCounter = 65000;
    else
      ProgCounter++;
  }
  
  if (*PA != PD)  // we failed to program it
  {  // send reset command
     FLASHCOMMAND1 = 0xAAAA;  // 1st Bus Cycle to initiate Flash Reset
     FLASHCOMMAND2 = 0x5555;  // 2nd Bus Cycle to initiate Flash Reset
     FLASHCOMMAND3 = 0xF0F0;  // 3rd Bus Cycle to initiate Flash Reset
    
    // Don't set successful byte, 
    // but do reset programming byte so we can get out of here  
    FlashPrograming   = 0;   // We're done
    FlashOpErr = 1;          // but we weren't successful             
  }
  else
  {
    // everything is great
    FlashPrograming   = 0;   // We're done
    FlashOpErr = 0;          // and we were successful!       
  }
  
  
/*

  FlashPrograming   = 1;   // initialize
  FlashOpSuccessful = 0;   //  initialize
  while (FlashPrograming)
  {
    ToggleCheck1 = TESTSTATUS;  // read it once
    ToggleCheck2 = TESTSTATUS;  // read it again

    
    if (! ( (ToggleCheck1.Low_DQ6 == ToggleCheck2.Low_DQ6) &&
            (ToggleCheck1.Hi_DQ6 == ToggleCheck2.Hi_DQ6)  
       )  )
    { // Start 1st toggle check  
    
      // it's toggling
      if (!((TESTSTATUS.Low_DQ5) || (TESTSTATUS.Hi_DQ5)) )
      { // Start DQ5 is Low
        FlashPrograming = 1;  // Keep going, still programming
      }
      else
      { // DQ5 is high
        ToggleCheck1 = TESTSTATUS;  // read it once
        ToggleCheck2 = TESTSTATUS;  // read it again      

        if (! ( (ToggleCheck1.Low_DQ6 == ToggleCheck2.Low_DQ6) &&
                (ToggleCheck1.Hi_DQ6 == ToggleCheck2.Hi_DQ6)
           )  )
        { // Start 2nd toggle check  
          // it's still toggling, and DQ5 is high, so we have a problem
          // Write the reset command to the Flash
            FLASHCOMMAND1 = 0xAAAA;  // 1st Bus Cycle to initiate Flash Reset
            FLASHCOMMAND2 = 0x5555;  // 2nd Bus Cycle to initiate Flash Reset
            FLASHCOMMAND3 = 0xF0F0;  // 3rd Bus Cycle to initiate Flash Reset
            
            // and reset it once more, for good measure
            FLASHCOMMAND1 = 0xAAAA;  // 1st Bus Cycle to initiate Flash Reset
            FLASHCOMMAND2 = 0x5555;  // 2nd Bus Cycle to initiate Flash Reset
            FLASHCOMMAND3 = 0xF0F0;  // 3rd Bus Cycle to initiate Flash Reset
            
            // and reset it once again
            FLASHCOMMAND1 = 0xAAAA;  // 1st Bus Cycle to initiate Flash Reset
            FLASHCOMMAND2 = 0x5555;  // 2nd Bus Cycle to initiate Flash Reset
            FLASHCOMMAND3 = 0xF0F0;  // 3rd Bus Cycle to initiate Flash Reset
            
            // Don't set successful byte, 
            // but do reset programming byte so we can get out of here  
            FlashPrograming   = 0;   // We're done
            FlashOpSuccessful = 0;   // but we weren't successful         
        }
        else
        {  // it's not toggling, so everything is great
           FlashPrograming   = 0;   // We're done
           FlashOpSuccessful = 1;   // and we were successful!       
        }  // End 2nd toggle check   
      } // End DQ5 is High
    } 
    else
    {  // not toggling
      // it's not toggling, so everything is great
      FlashPrograming   = 0;   // We're done
      FlashOpSuccessful = 1;   // and we were successful!   
    }  // End 1st toggle check
  }  // End While
  
*/  
}  // End Function


#pragma CODE_SEG DEFAULT_ROM // Put this in regular ROM area
unsigned char WriteFlashByte(unsigned long Address, unsigned short value)
// This is the hook that is used to get to the actual
// Write Flash Byte Code, which is run from RAM
// Returns 1 if an error occured in programming, or 0 of everything went well
{
  FlashWriteAddress = Address;  // define address of byte we are writing to flash
  FlashWriteValue = value;       // define value we are writing to flash 
  
  asm {     ORP  #0x00E0                   ;// disable interrupts while working with Flash         
            JSR  FLASH_WRITE_RAM_LOC       ;//jump to Write Flash Routine in Ram 
            ANDP #0xFF1F                   ;// OK to enable interrupts again
      } 
      
  return FlashOpErr; // tell calling function if we were successful or not
}




#pragma CODE_SEG DEFAULT_ROM // Put this in regular ROM area
void TransferFlashCode(void)
// This routine transfers the Flash Write routines to RAM,
// where they will be run from.

{
  unsigned long  counter;
  unsigned short *dest; 
  unsigned short *source;  

  counter = 0;  // initialize counter  
  while (counter<=FLASH_CODE_LENGTH)
  {        
    source      = (unsigned short far*)(FLASH_CODE_ROM_LOC+counter);  // next address in Flash Routine ROM area
    dest        = (unsigned short far*)(FLASH_CODE_RAM_LOC+counter);  // next address in Flash Routine RAM area
    
    *dest = *source;  // copy from source to destination
    
    counter += 2;       // keep track of how much has been transferred
  }
}


void FieldDataRead(void)
// This routine remembers all variables stored in Sector containing Field Cal
// parameters so that those that are not being changed can be re-written back
{
  unsigned char i;
  
  CalMinHoist = MinHoist;
  CalMaxHoist = MaxHoist;
  CalMinPedal = MinPedal;
  CalMaxPedal = MaxPedal;
  CalMinRack  = MinRack;
  CalMaxRack  = MaxRack;
  CalLowIdle  = LowIdle;
  CalHighIdle = HighIdle;
  
  CalfilteredRPM_SP_IncConst = filteredRPM_SP_IncConst;

  CalpaKpConst = paKpConst;
  CalpaKiConst = paKiConst;

  CaltKpConst = tKpConst;            
  CaltKiConst = tKiConst;            
  CaltKFFConst = tKFFConst;          
  CalTiltSPConst = TiltSPConst;         
  CalThrottle_SPIncConst = Throttle_SPIncConst; 




  for (i=0;i<=100;i++)
    CalRPMSetpoint[i] = RPMSetpoint[i];  
}

void FieldDataWrite(void)
// This routine writes all variables stored in Sector containing Field Cal
// parameters.
// Usage should be...
// FieldDataRead();
// Change what values need changed
// FieldDataWrite();
{
  unsigned char i;
  unsigned char WriteErr;
  
  
  TransferFlashCode();            // Load RAM with Flash Write Code
  EraseFlashSector( (unsigned long)(&RPMSetpoint[0]));  // erase this sector   
  
  // write the data to flash      
  WriteErr = 0;  
  for (i=0;i<=100;i++)   
    WriteErr += WriteFlashByte( (unsigned long)(&RPMSetpoint[i]),CalRPMSetpoint[i]); // write word
    
    
  WriteErr += WriteFlashByte( (unsigned long)(&MinHoist),CalMinHoist); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&MaxHoist),CalMaxHoist); // write word      
  WriteErr += WriteFlashByte( (unsigned long)(&MinPedal),CalMinPedal); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&MaxPedal),CalMaxPedal); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&MinRack),CalMinRack); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&MaxRack),CalMaxRack); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&LowIdle),CalLowIdle); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&HighIdle),CalHighIdle); // write word
 
  
  WriteErr += WriteFlashByte( (unsigned long)(&filteredRPM_SP_IncConst),CalfilteredRPM_SP_IncConst); // write word   

  WriteErr += WriteFlashByte( (unsigned long)(&paKpConst),CalpaKpConst); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&paKiConst),CalpaKiConst); // write word

  WriteErr += WriteFlashByte( (unsigned long)(&tKpConst),CaltKpConst); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&tKiConst),CaltKiConst); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&tKFFConst),CaltKFFConst); // write word
  WriteErr += WriteFlashByte( (unsigned long)(&TiltSPConst),CalTiltSPConst); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&Throttle_SPIncConst),CalThrottle_SPIncConst); // write word


  
  AddEvent(LOG_FIELDCAL);  // Log that the operator performed a field calibration 
}




void ManufactureDataRead(void)
// This routine remembers all variables stored in Sector containing Manufacture
// parameters so that those that are not being changed can be re-written back
{  
  ManVERSIONH = VERSIONH;
  ManVERSIONL = VERSIONL;
  ManMODELH = MODELH;
  ManMODELL = MODELL;
  ManCNFIGH  = CNFIGH;
  ManCNFIGL  = CNFIGL;
  ManVN  = VN;
  ManSN = SN;  
  ManMONTH = MONTH;
  ManDAY = DAY;
  ManYEAR = YEAR; 
}

void ManufactureDataWrite(void)
// This routine writes all variables stored in Sector containing Manufacture parameters.
// Usage should be...
// ManufactureDataRead();
// Change what values need changed
// ManufactureDataWrite();
{   
  unsigned char WriteErr;
  
  
  TransferFlashCode();            // Load RAM with Flash Write Code
  EraseFlashSector( (unsigned long)(&VERSIONH));  // erase this sector   
  
  // write the data to flash      
  WriteErr = 0; 
    
  WriteErr += WriteFlashByte( (unsigned long)(&VERSIONH),ManVERSIONH); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&VERSIONL),ManVERSIONL); // write word      
  WriteErr += WriteFlashByte( (unsigned long)(&MODELH),ManMODELH); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&MODELL),ManMODELL); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&CNFIGH),ManCNFIGH); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&CNFIGL),ManCNFIGL); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&VN),ManVN); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&SN),ManSN); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&MONTH),ManMONTH); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&DAY),ManDAY); // write word   
  WriteErr += WriteFlashByte( (unsigned long)(&YEAR),ManYEAR); // write word   
}
