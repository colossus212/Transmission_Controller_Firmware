
// #include SPI_Int.H   // variables and function prototypes
    
#pragma TRAP_PROC SAVE_REGS
void SPI_ISR(void)    // Called by SPI Interrupt     
// This interrupt is called when SPSR.SPIF is high and
//  SPCR2.SPIFIE  'SPI Finished Interrupt Enable' is also set
{     
  
  if (SPSR.SPIF)
  {    
    SPI_LastCmd = SPI_Command; // Remember last command sent
    SPI_Command = 0;           // Reset SPI_Command so everyone knows it's ok to transmit
    SPSR.SPIF = 0;               // Clear Flag 
  }
  
  
}






unsigned char SPIGetByte(unsigned short EEAdd, unsigned char *EEVal)
// This function retrieves value at EEAdd in Atmel AT25040 Serial EEPROM 
// and places it into EEVal and then returns 0 to indicate a successful read.
{ // START SPI_GetByte 

static unsigned char ReadCommand;  // Read Command to send to Serial EEPROM
static unsigned char EEAddChar;    // LSB of Serial EEPROM Address to Read

  if (!SPI_CommandPending)
  { // START if no communications already in progress
       
    EEAddChar = (EEAdd & 0xFF);    // LSB of Address
    if (EEAdd<=255)
      ReadCommand = EEPROM_READ;
    else
      ReadCommand = EEPROM_READ | 8;  
         
         
    // Read from EEPROM
    TR0 = ReadCommand;
    TR1 = EEAddChar;
      
    SPI_CR0 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR1 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR2 = SPI_CR_LAST;   // Allow Chip Select high after this byte      
  
    SPCR2.ENDQP  = 2;        // Ending Queue Pointer  
      
    SPI_LastCmd=0;           // Reset to zero to avoid confusion
    SPI_Command=SPI_GETBYTE; // Remember what I'm doing
  
    SPI_CommandPending = 1;  // Set Flag so we know we're busy
    SPCR1.SPE = 1;           // Start Transmission of command   
  } // END if no communications already in progress 
  
  
  
  if (SPI_LastCmd==SPI_GETBYTE)  
    SPI_CommandPending = 0;  // REset Flag so we know we're NOT busy
    
  if (!SPI_CommandPending)
    *EEVal = RR2;  // Read Byte from SPI Buffer
    
  return(SPI_CommandPending); // Tell calling process whether good read or not      
} // END SPI_GetByte


unsigned char SPISetByte(unsigned short EEAdd, unsigned char EEVal)
// This function sets value at EEAdd in Atmel AT25040 Serial EEPROM to EEVal
// and then returns 0 to indicate a successful write.
{ // START SPI_SetByte 

static unsigned char WriteCommand;  // Write Command to send to Serial EEPROM
static unsigned char EEAddChar;    // LSB of Serial EEPROM Address to Write


  if (!SPI_CommandPending)
  { // START if no communications already in progress
       
    EEAddChar = (EEAdd & 0xFF);    // LSB of Address
    if (EEAdd<=255)
      WriteCommand = EEPROM_WRITE;
    else
      WriteCommand = EEPROM_WRITE | 8;  
      
      
    // Write to EEPROM 
    TR0 = WriteCommand;
    TR1 = EEAddChar;
    TR2 = EEVal;
  
    SPI_CR0 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR1 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR2 = SPI_CR_LAST;   // Allow Chip Select high after this byte      
  
    SPCR2.ENDQP  = 2;        // Ending Queue Pointer 
    
      
    SPI_LastCmd=0;           // Reset to zero to avoid confusion
    SPI_Command=SPI_SETBYTE; // Remember what I'm doing
    SPI_CommandPending = 1;  // Set Flag so we know we're busy
  
    SPCR1.SPE = 1;           // Start Transmission of command 
  
  } // END if no communications already in progress
  
  
  if (SPI_LastCmd==SPI_SETBYTE)
  {
    SPI_CommandPending = 0;  // successfuly wrote to EEPROM

  }
 
    
  return(SPI_CommandPending); // Tell calling process whether good write or not          
} // END SPI_SetByte 


unsigned char SPIRStatus(unsigned char *Status)
// This function reads the status register of the  Atmel AT25040 Serial EEPROM
// and then returns 0 to indicate a successful read
{ // START SPI_ReadStatus 


  if (!SPI_CommandPending)
  { // START if no communications already in progress 
  
    TR0 = EEPROM_RDSR;    
    SPI_CR0 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR1 = SPI_CR_LAST;   // Allow Chip Select high after this byte        
  
    SPCR2.ENDQP  = 1;        // Ending Queue Pointer 
       
    SPI_LastCmd=0;           // Reset to zero to avoid confusion
    SPI_Command=SPI_RSTATUS; // Remember what I'm doing
    SPI_CommandPending = 1;  // Set Flag so we know we're busy
  
    SPCR1.SPE = 1;           // Start Transmission of command 
  
  } // END if no communications already in progress
  
  
  if (SPI_LastCmd==SPI_RSTATUS)
    SPI_CommandPending = 0;  // successfuly wrote to EEPROM
  
    
  if (!SPI_CommandPending)
    *Status = RR1;  // Read Byte from SPI Buffer
    
  return(SPI_CommandPending); // Tell calling process whether good read or not          
} // END SPI_ReadStatus 


unsigned char SPIWStatus(unsigned char Status)
// This function writes the status register of the  Atmel AT25040 Serial EEPROM
// and then returns 0 to indicate a successful write
{ // START SPI_WriteStatus 

 
  if (!SPI_CommandPending)
  { // START if no communications already in progress    
   
    TR0 = EEPROM_WRSR;
    TR1 = Status;            // load for transmit
    
    SPI_CR0 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR1 = SPI_CR_LAST;   // Allow Chip Select high after this byte        
  
    SPCR2.ENDQP  = 1;        // Ending Queue Pointer
       
    SPI_LastCmd=0;           // Reset to zero to avoid confusion
    SPI_Command=SPI_WSTATUS; // Remember what I'm doing
    SPI_CommandPending = 1;  // Set Flag so we know we're busy
  
    SPCR1.SPE = 1;           // Start Transmission of command 
  
  } // END if no communications already in progress
  
  
  if (SPI_LastCmd==SPI_WSTATUS)
    SPI_CommandPending = 0;  // successfuly wrote to EEPROM
 
    
  return(SPI_CommandPending); // Tell calling process whether good write or not          
} // END SPI_WriteStatus 

unsigned char WEnable(void)
// This function issues a WriteEnable Instruction to the Atmel AT25040 Serial EEPROM
// and then returns 0 to indicate a successful write
{ // START SPI_WriteEnable

  
  if (!SPI_CommandPending)
  { // START if no communications already in progress    
   
    TR0 = EEPROM_WREN;
   
    
    SPI_CR0 = SPI_CR_LAST;   // Allow Chip Select high after this byte      
  
    SPCR2.ENDQP  = 0;        // Ending Queue Pointer
       
    SPI_LastCmd=0;           // Reset to zero to avoid confusion
    SPI_Command=SPI_WENABLE; // Remember what I'm doing
    SPI_CommandPending = 1;  // Set Flag so we know we're busy
  
    SPCR1.SPE = 1;           // Start Transmission of command 
  
  } // END if no communications already in progress
  
  
  if (SPI_LastCmd==SPI_WENABLE)
    SPI_CommandPending = 0;  // successfuly wrote to EEPROM
  
  
  return(SPI_CommandPending); // Tell calling process whether good write or not          
} // END SPI_WriteEnable 


unsigned char SPIWDisable(void)
// This function issues WriteDisable Instruction to the Atmel AT25040 Serial EEPROM
// and then returns 0 to indicate a successful write
{ // START SPI_WriteDisable

 
  if (!SPI_CommandPending)
  { // START if no communications already in progress    
   
    TR0 = EEPROM_WRDI;    
   
    SPI_CR0 = SPI_CR_LAST;   // Allow Chip Select high after this byte        
  
    SPCR2.ENDQP  = 0;        // Ending Queue Pointer
       
    SPI_LastCmd=0;           // Reset to zero to avoid confusion
    SPI_Command=SPI_WDISABLE; // Remember what I'm doing
    SPI_CommandPending = 1;  // Set Flag so we know we're busy
  
    SPCR1.SPE = 1;           // Start Transmission of command 
  
  } // END if no communications already in progress
  
  
  if (SPI_LastCmd==SPI_WDISABLE)
    SPI_CommandPending = 0;  // successfuly wrote to EEPROM
  
    
  return(SPI_CommandPending); // Tell calling process whether good write or not          
} // END SPI_WriteDisable 



unsigned char SPIGetEvent(signed short ENum,EventLogType *EventRecord)
// This function retrieves value at EEAdd in Atmel AT25040 Serial EEPROM 
// and places it into EEVal and then returns 0 to indicate a successful read.

// This function retrieves EventRecord number ENUm from serial EEPROM at 
// address EEAdd.
// It returns 1 until it successfully accomplishes this, and then
// it returns 0, which alerts calling process that it has finished.   
{ // START ReadEvent 

static fracInt EventHour,EventSec; 

static unsigned char ReadCommand;  // Read Command to send to Serial EEPROM
static unsigned short EEAdd;
static unsigned char EEAddChar;    // LSB of Serial EEPROM Address to Read

static EventLogType ThisRecord;    
 
  
  // We need to start on the right address, so we are always on "page boundaries"
  // in the eeprom, so that we can read and write "pages"
  // It is a little mysterious why 11,19,27,35,27,35,43,51, etc are considered
  // page boundaries.  You would expect 8,16,32, etc.  However, this is what 
  // works.
  if ( (ENum>=0) && (ENum<EVENT_BUFF_SIZE) ) // Event Records are 8 bytes long, starting at address 11
    EEAdd = (ENum*8)+11; 
  else  
    EEAdd = 3;     

  if (!SPI_CommandPending)
  { // START if no communications already in progress
       
    EEAddChar = (EEAdd & 0xFF);    // LSB of Address
    if (EEAdd<=255)
      ReadCommand = EEPROM_READ;
    else
      ReadCommand = EEPROM_READ | 8;  
         
         
    // Read from EEPROM
    TR0 = ReadCommand;
    TR1 = EEAddChar;  // Tell address of event byte 0   
    
      
    SPI_CR0 = SPI_CR_CONT;   // Keep Chip Select low for next byte  send rd command
    SPI_CR1 = SPI_CR_CONT;   // Keep Chip Select low for next byte  send address
    SPI_CR2 = SPI_CR_CONT;   // Keep Chip Select low for next byte  get byte 0 
    SPI_CR3 = SPI_CR_CONT;   // Keep Chip Select low for next byte  get byte 1
    SPI_CR4 = SPI_CR_CONT;   // Keep Chip Select low for next byte  get byte 2 
    SPI_CR5 = SPI_CR_CONT;   // Keep Chip Select low for next byte  get byte 3     
    SPI_CR6 = SPI_CR_LAST;   // Keep Chip Select low for next byte  get byte 4

//    SPI_CR7 = SPI_CR_CONT;   // Keep Chip Select low for next byte  get byte 5     
//    SPI_CR8 = SPI_CR_CONT;   // Keep Chip Select low for next byte  get byte 6        
//    SPI_CR9 = SPI_CR_LAST;   // Allow Chip Select high after this byte  get byte 7    
  
    SPCR2.ENDQP  = 6;        // Ending Queue Pointer  
      
    SPI_LastCmd=0;           // Reset to zero to avoid confusion
    SPI_Command=SPI_GETEVENT; // Remember what I'm doing
  
    SPI_CommandPending = 1;  // Set Flag so we know we're busy
    SPCR1.SPE = 1;           // Start Transmission of command   
  } // END if no communications already in progress 
  
  
  
  if (SPI_LastCmd==SPI_GETEVENT)  
    SPI_CommandPending = 0;  // REset Flag so we know we're NOT busy
    
  if (!SPI_CommandPending)
  {

    EventHour.byte.high = RR2;  // Byte 0 of Event Record
    EventHour.byte.low  = RR3;  // Byte 1 of Event Record
    ThisRecord.Hours = EventHour.whole; // Put hours into Event Record
    
    EventSec.byte.high  = RR4;  // Byte 2 of Event Record
    EventSec.byte.low   = RR5;  // Byte 3 of Event Record
    ThisRecord.Seconds = EventSec.whole;  // Put seconds into Event Record   
    
    ThisRecord.Event = RR6;  // Byte 4 of Event Record    
    
    *EventRecord = ThisRecord; 
  }
    
  return(SPI_CommandPending); // Tell calling process whether good read or not      
} // END SPIGetEvent


unsigned char SPISetEvent(signed short ENum,EventLogType EventRecord)
// This function stores EventRecord into serial EEPROM at address EEAdd
// It returns 1 until it successfully accomplishes this, and than
// it returns 0, which alerts calling process that it has finished.   
{

  static fracInt EventHour,EventSec; 
  
  static unsigned char  WriteCommand; // Write Command to send to Serial EEPROM
  static unsigned short EEAdd;
  static unsigned char  EEAddChar;    // LSB of Serial EEPROM Address to Write

  
  EventHour.whole = EventRecord.Hours;  // Put into a structure to split into chars
  EventSec.whole = EventRecord.Seconds; // Put into a structure to split into chars
  
  
  
  // We need to start on the right address, so we are always on "page boundaries"
  // in the eeprom, so that we can read and write "pages"
  // It is a little mysterious why 11,19,27,35,27,35,43,51, etc are considered
  // page boundaries.  You would expect 8,16,32, etc.  However, this is what 
  // works.
  if ( (ENum>=0) && (ENum<EVENT_BUFF_SIZE)) // Event Records are 8 bytes long, starting at address 11
    EEAdd = (ENum*8)+11; 
  else  
    EEAdd = 3;      

  if (!SPI_CommandPending)
  { // START if no communications already in progress
       
    EEAddChar = (EEAdd & 0xFF);    // LSB of Address
    if (EEAdd<=255)
      WriteCommand = EEPROM_WRITE;
    else
      WriteCommand = EEPROM_WRITE | 8;  
      
      
    // Write to EEPROM 
    TR0 = WriteCommand;
    TR1 = EEAddChar;
    TR2 = EventHour.byte.high;  // Byte 0 of Event Record
    TR3 = EventHour.byte.low;   // Byte 1 of Event Record
    TR4 = EventSec.byte.high;   // Byte 2 of Event Record                           
    TR5 = EventSec.byte.low;    // Byte 3 of Event Record
    TR6 = EventRecord.Event;    // Byte 4 of Event Record
    TR7 = 0;  // Not Used Yet   // Byte 5 of Event Record
    TR8 = 0;  // Not Used Yet   // Byte 6 of Event Record
    TR9 = 0;  // Not Used Yet   // Byte 7 of Event Record
  
    SPI_CR0 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR1 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR2 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR3 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR4 = SPI_CR_CONT;   // Keep Chip Select low for next byte
    SPI_CR5 = SPI_CR_CONT;   // Keep Chip Select low for next byte        
    SPI_CR6 = SPI_CR_LAST;   // Keep Chip Select low for next byte    

//    SPI_CR7 = SPI_CR_CONT;   // Keep Chip Select low for next byte    
//    SPI_CR8 = SPI_CR_CONT;   // Keep Chip Select low for next byte    
//    SPI_CR9 = SPI_CR_LAST;   // Allow Chip Select high after this byte      
  
    SPCR2.ENDQP  = 6;        // Ending Queue Pointer                                      
      
    SPI_LastCmd=0;            // Reset to zero to avoid confusion
    SPI_Command=SPI_SETEVENT; // Remember what I'm doing
    SPI_CommandPending = 1;   // Set Flag so we know we're busy
  
    SPCR1.SPE = 1;           // Start Transmission of command 
  
  } // END if no communications already in progress
  
  
  if (SPI_LastCmd==SPI_SETEVENT)
  {
    SPI_CommandPending = 0;  // successfuly wrote to EEPROM

  }
 
    
  return(SPI_CommandPending); // Tell calling process whether good write or not            
}               



unsigned char ReadEvent(signed short ENum,EventLogType *EventRecord) 
// This function confirms that EEPROM is ready to talk, and than 
// calls SPIGetEvent to read the event records from EEPROM


// Working = ReadEvent(12,EventRec);
// if (!Working)  MyEvent = EventRec;  // only valid if ReadEvent returns 0
{ 
  static unsigned char Working;           // Used to mark progess of SPI operations 
  static EventLogType  SPIEvent;
  
  if ((SPI_Operation==0) || (SPI_Operation==SPI_READEVENT_OP))
  {  // START  No other operations are accessing SPI port
 
  
    SPI_Operation = SPI_READEVENT_OP;  // Set SPI_Operation flag to reserve SPI Port
    
    if (ReadValueStep>2)
      ReadValueStep = 0;
  
    if (ReadValueStep==0) // This just started
    {  // START   Keep issuing Read Status Commands until EEPROM says it's Ready   
  
     
      EEPROMStatus.whole = 0xFF;
      Working = SPIRStatus(&EEPROMStatus.whole);  // Read Status of EEPROM    
      if (!Working) // Successful read
      {      
        if (!EEPROMStatus.bit.BSY) // EEPROM is ready
          ReadValueStep = 1;   // Remember where in sequence we are     
      }
    } // END   Keep issuing Read Status Commands until EEPROM says it's Ready 
  
  
    if (ReadValueStep==1) // EEPROM is READY
    { 
      Working = SPIGetEvent(ENum,&SPIEvent);
      if (!Working)  // successfully issued instruction
      {
          ReadValueStep = 99;   // Sequence FINISHED.  Use 99 to flag this          
          *EventRecord = SPIEvent;
          
      }
    }  
    
    
    if (ReadValueStep==99)
      SPI_Operation = 0;  // REset SPI_OperationPending flag to free SPI Port
      
  }  // END    No other operations are accessing SPI port  

  return(SPI_Operation);
}
  

unsigned char StoreEvent(signed short ENum,EventLogType EventRecord)
// This function confirms that EEPROM is ready to talk, and is not write protected 
// than calls SPISetEvent to store the event record to EEPROM

// It returns 1 until it successfully accomplishes this, and than
// it returns 0, which alerts calling process that it has finished.

// This function uses the global variable StoreValueStep to keep track
// of what it has done already.

// This function is a little tricky to understand....
// The serial EEPROM can only be accessed at a certain speed.  We must wait
// till it has responded to one command, before another command may be issued
// to it.  We do not wish to hold up other functions while waiting for the 
// serial eeprom to get around to telling us it's ready.

// This function executes hundreds of times probably before it finishes all of 
// the tasks it undertakes to accomplish.  It keeps track of what it has already
// accomplished using the variable StoreValueStep.  This variable is set to 99
// after the function finally completes it's mission.  At the same time,
// the function returns 0, where it had been returning 1 previously.
// The next time the function executes, it resets StoreValueStep and starts the 
// procedure all over again.

// It is important that StoreValueStep take on a value of 0 or 99 to start with
// or successful writes to the EEPROM may not occur.



{ 
  static unsigned char Working;           // Used to mark progess of SPI operations
  
  if ((SPI_Operation==0) || (SPI_Operation==SPI_STOREEVENT_OP))
  {  // START  No other operations are accessing SPI port
  
    SPI_Operation = SPI_STOREEVENT_OP;  // Set SPI_Operation flag to reserve SPI Port
  

    if (StoreValueStep>5)
      StoreValueStep = 0;
      
    if (StoreValueStep==0) // This just started
    {  // START   Keep issuing Read Status Commands until EEPROM says it's Ready   
  

      EEPROMStatus.whole = 0xFF;  // Init to all 1's so don't fall through if's
      Working = SPIRStatus(&EEPROMStatus.whole);  // Read Status of EEPROM    
      if (!Working) // Successful read
      {

        if (!EEPROMStatus.bit.BSY) // EEPROM is ready
          StoreValueStep = 1;   // Remember where in sequence we are     
      }
    } // END   Keep issuing Read Status Commands until EEPROM says it's Ready 
      
   
      
   
      
    if (StoreValueStep==1) // Have already issued Read Status command
    {  

      Working = WEnable();  // Issue WriteEnable
      if (!Working)  // successfully issed instruction
        StoreValueStep = 2;   // Remember where in sequence we are   
      
    }
    
    
    if (StoreValueStep==2) // This just started
    {  // START   Keep issuing Read Status Commands until EEPROM says it's Ready   
  
     
      EEPROMStatus.whole = 0xFF;
      Working = SPIRStatus(&EEPROMStatus.whole);  // Read Status of EEPROM    
      if (!Working) // Successful read
      {        
        if (!EEPROMStatus.bit.BSY) // EEPROM is ready
          StoreValueStep = 3;   // Remember where in sequence we are     
      }
    } // END   Keep issuing Read Status Commands until EEPROM says it's Ready 
    
    

    
    if (StoreValueStep==3) // EEPROM is write enabled 
    {  
      if (EEPROMStatus.bit.BP != 0)  // If ANY of EEPROM is Block Write Protected...
      {    
         EEPROMStatus.bit.BP = 0;   // Clear the Block Protect Bits
         EEPROMStatus.bit.WEN = 1;  // Make sure we don't reset WEN bit
         
         Working = SPIWStatus(EEPROMStatus.whole);  // Write Status Register on EEPROM
         if (!Working)  // successfully issed instruction
          StoreValueStep = 4;   // Remember where in sequence we are
      }
      else
        StoreValueStep = 5;   // Jump to Set Event, EEPROM is good to go
    }
    
    if (StoreValueStep==4) // This just started
    {  // START   Keep issuing Read Status Commands until EEPROM says it's Ready   
  
      
      EEPROMStatus.whole = 0xFF;
      Working = SPIRStatus(&EEPROMStatus.whole);  // Read Status of EEPROM    
      if (!Working) // Successful read
      {        
        if (!EEPROMStatus.bit.BSY) // EEPROM is ready
          StoreValueStep = 5;   // Remember where in sequence we are     
      }
    } // END   Keep issuing Read Status Commands until EEPROM says it's Ready 
    
    
    
    if (StoreValueStep==5) // EEPROM is NOT Block Write Protected
    {
      Working = SPISetEvent(ENum,EventRecord);
      if (!Working)  // successfully issed instruction
          StoreValueStep = 99;   // Flag that we are finished   
    }   
    
    if (StoreValueStep==99)
      SPI_Operation = 0;  // REset SPI_Operation flag to free SPI Port
      
      
  }  // END    No other operations are accessing SPI port       
    
  return(SPI_Operation); 
} 




unsigned char ReadValue(unsigned short EEAdd,unsigned char *EEVal)
// This function retrieves value at EEAdd in EEPROM and places it into EEVAL
// It returns 1 until it successfully accomplishes this, and than
// it returns 0, which alerts calling process that it has finished.

// Working = ReadValue(212,EEVal);
// if (!Working)  MyVal = EEVal;  // only valid if ReadValue returns 0
{
 
  unsigned char Working;           // Used to mark progess of SPI operations 
  unsigned char SPIVal;
  
  if ((SPI_Operation==0) || (SPI_Operation==SPI_READVAL_OP))
  {  // START  No other operations are accessing SPI port
 
  
    SPI_Operation = SPI_READVAL_OP;  // Set SPI_Operation flag to reserve SPI Port
    
    if (ReadValueStep>2)
      ReadValueStep = 0;
  
    if (ReadValueStep==0) // This just started
    {  // START   Keep issuing Read Status Commands until EEPROM says it's Ready   
  

      EEPROMStatus.whole = 0xFF;
      Working = SPIRStatus(&EEPROMStatus.whole);  // Read Status of EEPROM    
      if (!Working) // Successful read
      {          
      
        if (!EEPROMStatus.bit.BSY) // EEPROM is ready
          ReadValueStep = 1;   // Remember where in sequence we are     
      }
    } // END   Keep issuing Read Status Commands until EEPROM says it's Ready 
  
  
    if (ReadValueStep==1) // EEPROM is READY
    { 
      Working = SPIGetByte(EEAdd,&SPIVal);
      if (!Working)  // successfully issued instruction
      {
          ReadValueStep = 99;   // Sequence FINISHED.  Use 99 to flag this
          *EEVal = SPIVal;
      }
    }  
    
    
    if (ReadValueStep==99)
      SPI_Operation = 0;  // REset SPI_OperationPending flag to free SPI Port
      
  }  // END    No other operations are accessing SPI port  

  return(SPI_Operation);
}
  


unsigned char StoreValue(unsigned short EEAdd,unsigned char EEVal)
// This function stores EEVal into serial EEPROM at address EEAdd
// It returns 1 until it successfully accomplishes this, and than
// it returns 0, which alerts calling process that it has finished.

// This function uses the global variable StoreValueStep to keep track
// of what it has done already.

// This function is a little tricky to understand....
// The serial EEPROM can only be accessed at a certain speed.  We must wait
// till it has responded to one command, before another command may be issued
// to it.  We do not wish to hold up other functions while waiting for the 
// serial eeprom to get around to telling us it's ready.

// This function executes hundreds of times probably before it finishes all of 
// the tasks it undertakes to accomplish.  It keeps track of what it has already
// accomplished using the variable StoreValueStep.  This variable is set to 99
// after the function finally completes it's mission.  At the same time,
// the function returns 0, where it had been returning 1 previously.
// The next time the function executes, it resets StoreValueStep and starts the 
// procedure all over again.

// It is important that StoreValueStep take on a value of 0 or 99 to start with
// or successful writes to the EEPROM may not occur.



{   

  static unsigned char Working;           // Used to mark progess of SPI operations
  
  if ((SPI_Operation==0) || (SPI_Operation==SPI_STOREVAL_OP))
  {  // START  No other operations are accessing SPI port
  
    SPI_Operation = SPI_STOREVAL_OP;  // Set SPI_Operation flag to reserve SPI Port
  
    if (StoreValueStep>5)
      StoreValueStep = 0;
  
    if (StoreValueStep==0) // This just started
    {  // START   Keep issuing Read Status Commands until EEPROM says it's Ready   
  

      EEPROMStatus.whole = 0xFF;  // Init to all 1's so don't fall through if's
      Working = SPIRStatus(&EEPROMStatus.whole);  // Read Status of EEPROM    
      if (!Working) // Successful read
      {

        if (!EEPROMStatus.bit.BSY) // EEPROM is ready
          StoreValueStep = 1;   // Remember where in sequence we are     
      }
    } // END   Keep issuing Read Status Commands until EEPROM says it's Ready 
      
   
      
    if (StoreValueStep==1) // Have already issued Read Status command
    {  

      Working = WEnable();  // Issue WriteEnable
      if (!Working)  // successfully issed instruction
        StoreValueStep = 2;   // Remember where in sequence we are   

    }
    
    
    if (StoreValueStep==2) // This just started
    {  // START   Keep issuing Read Status Commands until EEPROM says it's Ready   
  

      EEPROMStatus.whole = 0xFF;
      Working = SPIRStatus(&EEPROMStatus.whole);  // Read Status of EEPROM    
      if (!Working) // Successful read
      {

        if (!EEPROMStatus.bit.BSY) // EEPROM is ready
          StoreValueStep = 3;   // Remember where in sequence we are     
      }
    } // END   Keep issuing Read Status Commands until EEPROM says it's Ready 
    
    

    
    if (StoreValueStep==3) // EEPROM is write enabled 
    {  
      if (EEPROMStatus.bit.BP != 0)  // If ANY of EEPROM is Block Write Protected...
      {    
         EEPROMStatus.bit.BP = 0;   // Clear the Block Protect Bits
         EEPROMStatus.bit.WEN = 1;  // Make sure we don't reset WEN bit

         Working = SPIWStatus(EEPROMStatus.whole);  // Write Status Register on EEPROM
         if (!Working)  // successfully issed instruction
          StoreValueStep = 4;   // Remember where in sequence we are
      }
      else
        StoreValueStep = 5;   // Jump to Set Byte, EEPROM is good to go
    }
    
    if (StoreValueStep==4) // This just started
    {  // START   Keep issuing Read Status Commands until EEPROM says it's Ready   
  

      EEPROMStatus.whole = 0xFF;
      Working = SPIRStatus(&EEPROMStatus.whole);  // Read Status of EEPROM    
      if (!Working) // Successful read
      {   
      
        if (!EEPROMStatus.bit.BSY) // EEPROM is ready
          StoreValueStep = 5;   // Remember where in sequence we are     
      }
    } // END   Keep issuing Read Status Commands until EEPROM says it's Ready 
    
    
    
    if (StoreValueStep==5) // EEPROM is NOT Block Write Protected
    {
      Working = SPISetByte(EEAdd,EEVal);
      if (!Working)  // successfully issed instruction
          StoreValueStep = 99;    // Sequence FINISHED.  Use 99 to flag this
    }         
    
    if (StoreValueStep==99)
      SPI_Operation = 0;  // REset SPI_Operation flag to free SPI Port
      
      
  }  // END    No other operations are accessing SPI port       
    
  return(SPI_Operation); 
} 



void InitSPI(void)  // Init SPI System   
{
 
 
  // QSMCR is also set in InitSPI, so better make sure they match.
  // It is done twice, in case this code is modified and the SPI or
  // SCI code is deleted.
  // pg 84  // QSM Configuration Register
  QSMCR.IARB  = 2;  // Interupt Arbitration Identification Number      
  QSMCR.SUPV  = 0;  // 1=>Supervised Mode   
  QSMCR.FRZ1  = 1;  // 0=>Ignore Freeze Command (Background mode)
  QSMCR.STOP  = 0;  // 0=>Normal Operation
  
  // pg 85    // QSM Interrupt Levels Register .
  QILR.ILQSPI = 4;   // Interrupt Level of SPI 
  
  // QIVR is also set in InitSPI, so better make sure they match.
  // It is done twice, in case this code is modified and the SPI or
  // SCI code it deleted.
  // pg 85     // QSM Interrupt Vector Register 
  QIVR = 0xA0; // QSM Interrupts Base Vector  
  
  
  // pg 86  // Port QS Pin Assignment Register(Other pins in port defined elsewhere)
  PORTQS.SCK  = 0;   // Initial state of SCK is LOW
  PORTQS.MISO = 1;   // Inactive state of MISO is high       ******** was 1
  PORTQS.MOSI = 1;   // Inactive state of MOSI is high       ******** was 1
  PORTQS.PCS0 = 1;   // Inactive state of chip-seclect is HIGH 
  
                       
  // pg 87
  DDRQS.SCK  = 1;   // SCK is an OUTPUT
  DDRQS.MISO = 0;   // Master Input
  DDRQS.MOSI = 1;   // Master Output
  DDRQS.PCS0 = 1;   // Peripheral Chip Select is an OUTPUT 
  
  
  
  SPCR1.SPE   = 0;    // Disable QSPI before proceeding with initialization    
  
  // pg 86  // Port QS Pin Assignment Register(Other pins in port defined elsewhere)
  PQSPAR.MISO = 1;   // define bit as a SPI bit          
  PQSPAR.MOSI = 1;   // define bit as a SPI bit  
  PQSPAR.PCS0 = 1;   // define bit as a SPI bit 
  
  SPCR0.SPBR  = 8;   // Serial Clock Baud Rate = 410KHz    // was 16
   
  SPCR0.CPHA  = 0;   // Clock Phase                        
  SPCR0.CPOL  = 0;   // Clock Polarity                     
  SPCR0.BITS  = 8;   // Bits/Transfer (8-15) or 0=16
  SPCR0.WOMQ  = 0;   // Wired-OR Mode for QSPI Pins
  SPCR0.MSTR  = 1;   // 1=>Master  Master/Slave Mode Select 
  
  SPCR2.NEWQP  = 0;   // New Queue Pointer Value 
  SPCR2.ENDQP  = 0;   // Ending Queue Pointer
  SPCR2.WRTO   = 0;   // Wrap To
  SPCR2.WREN   = 0;   // Wrap Enable
  SPCR2.SPIFIE = 1;   // SPI Finished Interrupt Enable 
                    
  StoreValueStep = 0;
  ReadValueStep = 0;
}


