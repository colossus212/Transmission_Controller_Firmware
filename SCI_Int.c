
// comprising commands of only the digits 1,2,5,6,8,B,C,& F insures each command 
// is "Gray Coded".  By only using commands made up of these digits, we insure that
// there will be no command that is not at least TWO bit changes from another 
// command.  Noise in the serial line would need to corrupt two bits rather
// than just one, before the wrong command was executed.  


#define SI_LF        10      // Line Feed
#define SI_CR        13      // Carriage Return


// Commands which use the BYTE_SHORT format
// (A one byte command is responded to with a short int number.  No Parameters)
// These all use the generic GetValue() function in the DLL 
#define SI_GETSN          0x11    // Get Controller Serial Number  
#define SI_GET_D_INS      0x12    // Get All External Digital Input Bits
#define SI_GETPILOT       0x15    // Get Pilot Pressure Setting

#define SI_GET_D_COM      0x16    // Get All External Digital Output Command Bits
#define SI_GET_D_MAN      0x18    // Get All External Digital Output Manual Bits
#define SI_GET_D_MODE     0x1B    // Get All External Digital Output Mode Bits
#define SI_GET_D_SENSE    0x1C    // Get All External Digital Output Sensed Bits (confirmation bits)

#define SI_GET_A_COM      0x21    // Get All External Analog Output Command Bytes
#define SI_GET_A_MAN      0x22    // Get All External Analog Output Manual Bytes
#define SI_GET_A_MODE     0x25    // Get All External Analog Output Mode Bytes

#define SI_GET_HOURS      0x51    // Get Hour Meter Reading
#define SI_GET_SECS       0x52    // Get Second Meter Reading

#define SI_GET_PA_PROP_GAIN 0x53  // Get Power-Adapt Proportional Gain
#define SI_GET_PA_INT_GAIN  0x54  // Get Power-Adapt Integral Gain
#define SI_GET_RPMSP_RAMP   0x55  // Get RPM Setpoint Ramp Rate

#define SI_GET_TU_PROP_GAIN 0x56  // Get Power-Adapt Proportional Gain
#define SI_GET_TU_INT_GAIN  0x57  // Get Power-Adapt Integral Gain

#define SI_GET_CUR_EVENT    0x5A  // Get Current Event


// Commands which use the BYTE_SHORT_SHORT format
// (A one byte command and a one short parameter is responded to 
// by echoing the one short parameter back.)
// These all use the generic SetValue() function in the DLL
#define SI_SET_D_MODE     0x61    // Set All External Digital Output Mode Bits 
#define SI_SET_D_MAN      0x62    // Set All External Digital Output Manual Bits

#define SI_SET_A_MODE     0x85    // Set All External Analog Output Mode Bytes 
#define SI_SET_A_MAN      0x86    // Set All External Analog Output Manual Bytes

#define SI_SET_HOURS      0xB1    // Set Hour Meter Reading
#define SI_SET_SECS       0xB2    // Set Second Meter Reading

#define SI_SET_PA_PROP_GAIN 0xB3  // Set Power-Adapt Proportional Gain
#define SI_SET_PA_INT_GAIN  0xB4  // Set Power-Adapt Integral Gain
#define SI_SET_RPMSP_RAMP   0xB5  // Set RPM Setpoint Ramp Rate
#define SI_STORE_PA_GAINS   0xB6  // Store Power-Adapt Gains into Flash

#define SI_SET_TU_PROP_GAIN 0xB7  // Set Power-Adapt Proportional Gain
#define SI_SET_TU_INT_GAIN  0xB8  // Set Power-Adapt Integral Gain
#define SI_STORE_TU_GAINS   0xB9  // Store Power-Adapt Gains into Flash




// Commands which use special formats of command, parameter, and/or response.
#define SI_GETINFO           'i'     // Get Info
#define SI_GETSCALED         0xC1    // Get Scaled Analog Input Value
#define SI_GETUNSCALED       0xC2    // Get UnScaled Analog Input Value
#define SI_BATCH_GETSCALED   0xC5    // Get All Scaled Analog Input Values
#define SI_BATCH_GETUNSCALED 0xC6    // Get All UnScaled Analog Input Values

#define SI_GET_MANINFO       0xC8    // Get Manufacture Information
#define SI_SET_MANINFO       0xCB    // Set Manufacture Information

#define SI_MANUAL_ENABLE     0xF1    // Enable Manual control of board outputs 


#define SI_GETEVENTS         0xF8    // Get Events in Event Buffer 
#define SI_CLEAREVENTS       0xFB    // Clear all Events in Event Buffer



#define MANUAL_OK_PASSWORD1  123    // This number must be sent from monitor to enable manual op
#define MANUAL_OK_PASSWORD2  214    // This number ALSO must be sent from monitor to enable manual op
#define CLEAREVENTS_OK_PWD   187    // This number must be sent from monitor to allow events to be cleared





#define INBUFFLEN  50    // Serial Input Buffer Length
#define OUTBUFFLEN 600   //  Serial Output Buffer Length


static unsigned char InputBuffer[INBUFFLEN];   // Input Buffer Array
static unsigned char OutputBuffer[OUTBUFFLEN]; // Output Buffer Array

static unsigned char InBuffPtr;  // Pointer to end of input buffer
static unsigned char OutBuffPtr; // Pointer to end of output buffer

// #include SCI_Int.H   // variables and function prototypes
    
#pragma TRAP_PROC SAVE_REGS
void SCI_ISR(void)    // Called by SCI Interrupt   
{ 

  unsigned char i;     // counter used to cycle through buffers.    

  if (SCSR.TDRE)      // Transmit Data Register Empty
  {
  
    // always send character in buffer position 0
    // if there is a valid char there to send.
   
    if (OutBuffPtr>0)
    {
      SCDR = OutputBuffer[0];  // Send next character        
      
      // Shift rest of buffer down, to make room for more data to be added
      // to the end of the buffer
      for (i=1;i<OutBuffPtr;i++)
        OutputBuffer[i-1] = OutputBuffer[i];
        
      OutBuffPtr--;            // Decrement Pointer
    } 
    else
      SCCR1.TIE    = 0;  // Disable TDRE interrupt, if nothing else to transmit
      
  }
  
  if (SCSR.RDRF)  // Receive Data Register Full Flag
  {     
    // Add Pending character to input buffer.
    InputBuffer[InBuffPtr] = SCDR;
    
    // Set up buffer for next character
    if (InBuffPtr<(INBUFFLEN-1))
      InBuffPtr++;       
  } 
}


unsigned char ReadChar(unsigned char *rchar)
// Reads a character from the input buffer
// returns 0 if successful
// returns 1 if input buffer is empty
{
  unsigned char i;  // counter
  unsigned char BufferEmpty; 
  
  
  SCCR1.RIE   = 0;  // Disable RDRF Interrupt.  This will cause some
  // spurious interrupts, but that's handled.

  BufferEmpty = (InBuffPtr==0);
  
  if (!BufferEmpty)
  {
    *rchar = InputBuffer[0];  // Read next character
    
    // Shift rest of buffer down, to make room for more data to be added
    // to the end of the buffer
    for (i=1;i<InBuffPtr;i++)
      InputBuffer[i-1] = InputBuffer[i];
    
    InBuffPtr--;  // Decrement Pointer
  }
  
   SCCR1.RIE   = 1;  // ReEnable Receive Interrupt

  return(BufferEmpty);    
}



unsigned char WriteChar(unsigned char wchar)
// Add a character to the Output Buffer.
// returns 0 if successful
// returns 1 if buffer is full
{
   
   unsigned char BufferFull;

   SCCR1.TIE    = 0;  // Disable TDRE interrupt, when altering output buffer
   // (this will generate spurious interrupts, but that's better than 
   //  bad communications)       

   BufferFull = (OutBuffPtr>=(OUTBUFFLEN-1));  // Don't overrun buffer
   if (!BufferFull)
   {       
     OutputBuffer[OutBuffPtr] = wchar;  // add character to output buffer
     OutBuffPtr++;  // set up output buffer for next character              
   }  
   
   SCCR1.TIE    = 1;  // Enable TDRE interrupt, after altering output buffer       
   
   return(BufferFull);
}


unsigned char WriteString(char *wstring,unsigned char wlen)
// Adds characters in wstring to output buffer
// returns 0 of successful
// returns 1 if buffer overfilled
{
  unsigned char i;  // counter
  unsigned char err;
  
  err = 0;
  for (i=0;i<wlen;i++)
    err += WriteChar(wstring[i]);
    
  return(err);
}



// Because the other end of this is a DLL that programmers may not have access to the source code for,
// I will make the serial routine only have two functions.  Read Memory and Write Memory.  That way
// programmers can map certain memory locations in their code to correspond to anything they want,
// rather than relying on there being functions present to read A/D channel 3, or other things equally
// arcane.  

// Or perhaps ReadRamLoc, WriteRamLoc, ReadFlashLoc, WriteFlashLoc, ReadFlashSeg, WriteFlashSeg




void InitSCI(void)  // Init SCI System   
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
  QILR.ILSCI  = 3;   // Interrupt Level of SCI              
  
  // QIVR is also set in InitSPI, so better make sure they match.
  // It is done twice, in case this code is modified and the SPI or
  // SCI code it deleted.
  // pg 85     // QSM Interrupt Vector Register 
  QIVR = 0xA0; // QSM Interrupts Base Vector  
  
  // SCI Control Register 0
  //  Baud = System Clock/(32*SCBR)  System Clock =  13.107 MHz
  // SCBR = 13107000/(32*BAUDRATE)
  SCCR0.SCBR  = 43;  // Baud Rate = 9600              
  
                    
  // pg 97    // SCI Control Register 1
  SCCR1.SBK   = 0;  // 0=> Normal Operation 1=>Send Break
  SCCR1.RWU   = 0;  // 0 => All received data recognized
  SCCR1.RE    = 1;  // 1=>Receiver Enable
  SCCR1.TE    = 1;  // 2=>Transmitter Enable
  SCCR1.ILIE  = 0;  // 1=> Interrupt occurs when Idle Line Detect IDLE is set
  SCCR1.RIE   = 1;  // 1=> Interrupt occurs when Receive Data Register Full RDRF is set
  SCCR1.TCIE  = 0;  // 1=> Interrupt occurs when Transmit Complete TC is set
  SCCR1.TIE   = 1;  // 1=> Interrupt occurs when Transmit Data Reg Empty TDRE is set
  SCCR1.WAKE  = 0;  // 0=> Receiver Wake by idle line detect.  
  SCCR1.M     = 0;  // 0 => 8 data bits (must use this because I mapped SCDR to a char
  SCCR1.PE    = 0;  // 0 => No Parity 
  SCCR1.PT    = 0;  // 0 => Even Parity if Parity is Enabled
  SCCR1.ILT   = 1;  // Idle-Line Detect Type
  SCCR1.WOMS  = 0;  // Wired-OR Mode for SCI Pins
  SCCR1.LOOPS = 0;  // Liip Mode
             
                    
// pg 99  // SCI Status Register
// SCSR.PF     // Parity Flag Error
// SCSR.FE     // Framing Error
// SCSR.NF     // Noise Error Flag
// SCSR.OR     // Overrun Error Flag
// SCSR.IDLE   // Idle-Line Detected Flag
// SCSR.RAF    // Receiver Active Flag
// SCSR.RDRF   // Receive Data Register Full Flag
// SCSR.TC     // Transmit Complete Flag
// SCSR.TDRE   // Transmit Data Register Empty Flag
               
                    
// pg 101  Note, I am ignoring the 9th bit, so can't use 9-bit serial protocols
// SCDR // SCI Data Register



}


void CheckSCI(void)
// This routine checks the serial input buffer for new commands 
{

  static unsigned char InCommand;  // Read command character from input buffer     
  static unsigned char i;          // Counter to count through multiple items
  static fracInt       fracTempValue;  // Used to reassemble/break up integers to/from bytes
  
  
  SCI_Out_Err = 0;  // Zero out any existing transmission errors
  SCI_In_Err = 0;   // Zero out any existing receive errors
  
  if (SCICommand==0)  
  { // begin no serial command is pending
    if (!ReadChar(&InCommand))  // if a character is present to be read
    { // begin read a new character
      
      
      switch (InCommand)              
      {
        // ************ BYTE_SHORT Format Commands ************
        // Commands which use the BYTE_SHORT format
        // (A one byte command is responded to with a short int number.  No Parameters)
        // These all use the generic GetValue() function in the DLL  
        
        
         case SI_GET_D_INS :
           SCICommand = SI_GET_D_INS;  // set pending serial command to SI_GET_D_INS 
         break;     
         
         case SI_GETPILOT :
           SCICommand = SI_GETPILOT;  // set pending serial command to SI_GETPILOT 
         break;     
         
         case SI_GETSN :
           SCICommand = SI_GETSN;  // set pending serial command to SI_GETSN 
         break;     
         
         case SI_GET_D_COM :
           SCICommand = SI_GET_D_COM;  // set pending serial command to SI_GET_D_COM 
         break;     
         
         case SI_GET_D_MAN :
           SCICommand = SI_GET_D_MAN;  // set pending serial command to SI_GET_D_MAN 
         break;   
         
         case SI_GET_D_MODE :
           SCICommand = SI_GET_D_MODE;  // set pending serial command to SI_GET_D_MODE 
         break;     
         
         case SI_GET_D_SENSE :
           SCICommand = SI_GET_D_SENSE;  // set pending serial command to SI_GET_D_SENSE 
         break; 
         
         
         case SI_GET_A_COM :
           SCICommand = SI_GET_A_COM;  // set pending serial command to SI_GET_A_COM 
         break;     
         
         case SI_GET_A_MAN :
           SCICommand = SI_GET_A_MAN;  // set pending serial command to SI_GET_A_MAN 
         break;   
         
         case SI_GET_A_MODE :
           SCICommand = SI_GET_A_MODE;  // set pending serial command to SI_GET_A_MODE 
         break;           
        
        
         case SI_GET_HOURS :
           SCICommand = SI_GET_HOURS;  // set pending serial command to SI_GET_HOURS 
         break;   
         
         case SI_GET_SECS :
           SCICommand = SI_GET_SECS;  // set pending serial command to SI_GET_SECS 
         break;   

         case SI_GET_PA_PROP_GAIN :
           SCICommand = SI_GET_PA_PROP_GAIN;  // set pending serial command to SI_GET_PA_PROP_GAIN 
         break;   
         
         case SI_GET_PA_INT_GAIN :
           SCICommand = SI_GET_PA_INT_GAIN;  // set pending serial command to SI_GET_PA_INT_GAIN 
         break; 

         case SI_GET_RPMSP_RAMP :
           SCICommand = SI_GET_RPMSP_RAMP;  // set pending serial command to SI_GET_RPMSP_RAMP 
         break;            

         case SI_GET_TU_PROP_GAIN :
           SCICommand = SI_GET_TU_PROP_GAIN;  // set pending serial command to SI_GET_TU_PROP_GAIN 
         break;   
         
         case SI_GET_TU_INT_GAIN :
           SCICommand = SI_GET_TU_INT_GAIN;  // set pending serial command to SI_GET_TU_INT_GAIN 
         break;   

         case SI_GET_CUR_EVENT :
           SCICommand = SI_GET_CUR_EVENT;  // set pending serial command to SI_GET_CUR_EVENT 
         break;              
  
         
         
         // ************ BYTE_SHORT_SHORT Format Commands ************
         // (A one byte command and a one short parameter is responded to 
         // by echoing the one short parameter back.)
         // These all use the generic SetValue() function in the DLL 
         
         case SI_SET_D_MODE :
           SCICommand = SI_SET_D_MODE;  // set pending serial command to SI_SET_D_MODE 
         break;                  
         
         case SI_SET_D_MAN :
           SCICommand = SI_SET_D_MAN;  // set pending serial command to SI_SET_D_MAN 
         break;
         
         
         case SI_SET_A_MODE :
           SCICommand = SI_SET_A_MODE;  // set pending serial command to SI_SET_A_MODE 
         break;                  
         
         case SI_SET_A_MAN :
           SCICommand = SI_SET_A_MAN;  // set pending serial command to SI_SET_A_MAN 
         break;
         
         case SI_SET_HOURS :
           SCICommand = SI_SET_HOURS;  // set pending serial command to SI_SET_HOURS 
         break; 
         
         case SI_SET_SECS :
           SCICommand = SI_SET_SECS;  // set pending serial command to SI_SET_SECS 
         break;

         case SI_SET_PA_PROP_GAIN :
           SCICommand = SI_SET_PA_PROP_GAIN;  // set pending serial command to SI_SET_PA_PROP_GAIN 
         break; 
         
         case SI_SET_PA_INT_GAIN :
           SCICommand = SI_SET_PA_INT_GAIN;  // set pending serial command to SI_SET_PA_INT_GAIN 
         break;

         case SI_SET_RPMSP_RAMP :
           SCICommand = SI_SET_RPMSP_RAMP;  // set pending serial command to SI_SET_RPMSP_RAMP 
         break;

         

         case  SI_STORE_PA_GAINS :
           SCICommand =  SI_STORE_PA_GAINS;  // set pending serial command to  SI_STORE_PA_GAINS 
         break;

         case SI_SET_TU_PROP_GAIN :
           SCICommand = SI_SET_TU_PROP_GAIN;  // set pending serial command to SI_SET_TU_PROP_GAIN 
         break; 
         
         case SI_SET_TU_INT_GAIN :
           SCICommand = SI_SET_TU_INT_GAIN;  // set pending serial command to SI_SET_TU_INT_GAIN 
         break;

         case  SI_STORE_TU_GAINS :
           SCICommand =  SI_STORE_TU_GAINS;  // set pending serial command to  SI_STORE_TU_GAINS 
         break;
            
         
    
         
         // ************ Special Format Commands ************
         // These are command which don't limit themselves to any one standard protocol
         // and to must each be handled as a special case down below.
         case SI_GETSCALED :
           SCICommand = SI_GETSCALED;    // set pending serial command to SI_GETSCALED 
         break;        
          
         case SI_GETUNSCALED :
           SCICommand = SI_GETUNSCALED; // set pending serial command to SI_GETUNSCALED 
         break;
          
         case SI_BATCH_GETSCALED :
           SCICommand = SI_BATCH_GETSCALED; // set pending serial command to SI_BATCH_GETSCALED 
         break;
          
         case SI_BATCH_GETUNSCALED :
           SCICommand = SI_BATCH_GETUNSCALED; // set pending serial command to SI_BATCH_GETUNSCALED 
         break;               
          
         case SI_GETINFO :
           SCICommand = SI_GETINFO;  // set pending serial command to SI_GETINFO 
         break; 
          
         case SI_GET_MANINFO :
           SCICommand = SI_GET_MANINFO;  // set pending serial command to SI_GET_MANINFO 
         break;  

         case SI_SET_MANINFO :
           SCICommand = SI_SET_MANINFO;  // set pending serial command to SI_SET_MANINFO 
           ManInfoByteCounter = 0;  // reset byte counter
         break; 
         
         case SI_GETEVENTS :
           SCICommand = SI_GETEVENTS;  // set pending serial command to SI_GETEVENTS 
         break;  
         
         case SI_MANUAL_ENABLE :
           SCICommand = SI_MANUAL_ENABLE;  // set pending serial command to SI_MANUAL_ENABLE 
         break; 
         
         case SI_CLEAREVENTS :
           SCICommand = SI_CLEAREVENTS;  // set pending serial command to SI_CLEAREVENTS 
         break;  
        
                        
         default :
           SCICommand = 0;  // reset pending serial command to nothing          
      }        
    } // end read a new character
  } // end no serial command is pending
  

  switch (SCICommand)
  {
     // ************ BYTE_SHORT Format Commands ************
     // Commands which use the BYTE_SHORT format
     // (A one byte command is responded to with a short int number.  No Parameters)
     // These all use the generic GetValue() function in the DLL  
        
     case SI_GET_D_INS :    
       SCI_Out_Int.whole = ExternalInput.whole;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;    
    
     case SI_GETPILOT :    
       SCI_Out_Int.whole = PilotOut;    // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;
        
     case SI_GETSN :    
       SCI_Out_Int.whole = SN; // Ready value for transmission
       SCIWriteIntOK = 1;                // Set flag to send to output buffer
     break; 
         
         
     case SI_GET_D_COM :  // Get All External Digital Output Command Bits  
       SCI_Out_Int.whole = ExtDigOut.Command.whole;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;    
         
     case SI_GET_D_MAN :  // Get All External Digital Output Manual Bits
       SCI_Out_Int.whole = ExtDigOut.Manual.whole;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break; 
         
     case SI_GET_D_MODE :  // Get All External Digital Output Mode Bits
       if ( (ManualOK1 == MANUAL_OK_PASSWORD1) && (ManualOK2 == MANUAL_OK_PASSWORD2))
         SCI_Out_Int.whole = ExtDigOut.Mode.whole;  // Ready value for transmission
       else
          SCI_Out_Int.whole = 0;   // if no passwords, doesn't matter what the mode bits are, it's in auto
    
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break; 
         
     case SI_GET_D_SENSE :  // Get All External Digital Output Sensed Bits (confirmation bits)  
       SCI_Out_Int.whole = ExtDigOut.Sensed.whole;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;
     
     
     
     case SI_GET_A_COM :  // Get All External Analog Output Command Bytes
       SCI_Out_Int.whole = ExtAnlgOut.Command.whole;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;    
         
     case SI_GET_A_MAN :  // Get All External Analog Output Manual Bytes
       SCI_Out_Int.whole = ExtAnlgOut.Manual.whole;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break; 
         
     case SI_GET_A_MODE :  // Get All External Analog Output Mode Bytes
       if ( (ManualOK1 == MANUAL_OK_PASSWORD1) && (ManualOK2 == MANUAL_OK_PASSWORD2))
         SCI_Out_Int.whole = ExtAnlgOut.Mode.whole;  // Ready value for transmission
       else
         SCI_Out_Int.whole = 0;   // if no passwords, doesn't matter what the mode bits are, it's in auto

       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break; 
                     
     
     
     
     case SI_GET_HOURS :  // Get Hour Meter Reading
       SCI_Out_Int.whole = Hours;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;
     
     case SI_GET_SECS :  // Get Second Meter Reading
       SCI_Out_Int.whole = Seconds;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;

     
     case SI_GET_PA_PROP_GAIN :  // Get Power Adapt Proportional Gain
       SCI_Out_Int.whole = paKp;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;
     
     case SI_GET_PA_INT_GAIN :  // Get Power Adapt Integral Gain        
       SCI_Out_Int.whole = paKi;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break; 

     case SI_GET_RPMSP_RAMP :  // Get RPM Setpoint Ramp Rate   
       SCI_Out_Int.whole = fRPM_SP_Inc;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;          

     case SI_GET_TU_PROP_GAIN :  // Get Throttle Up Proportional Gain
       SCI_Out_Int.whole = tKp;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;
     
     case SI_GET_TU_INT_GAIN :   // Get Throttle Up Integral Gain
       SCI_Out_Int.whole = tKi;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;  

     case SI_GET_CUR_EVENT :   // Get Current Event
       SCI_Out_Int.whole = EventCode;  // Ready value for transmission
       SCIWriteIntOK = 1;                 // Set flag to send to output buffer
     break;       

    
    
    // ************ BYTE_SHORT_SHORT Format Commands ************
    // (A one byte command and a one short parameter is responded to 
    // by echoing the one short parameter back.)
    // These all use the generic SetValue() function in the DLL
    
    case SI_SET_D_MODE :       
    // Set Auto/Manual Mode Bits for External Digital Outputs
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {       
          if ( (ManualOK1 == MANUAL_OK_PASSWORD1) && (ManualOK2 == MANUAL_OK_PASSWORD2))
          {
            ExtDigOut.Mode.byte.high = SCI_InParam;  // Set upper byte of Mode Word
            ExtDigOut.Mode.byte.low  = SCI_InParam2; // Set lower byte of Mode Word 
          }
          else
          {
            ExtDigOut.Mode.byte.high = 0;  // If no passwords, force everything to Auto
            ExtDigOut.Mode.byte.low  = 0;  // If no passwords, force everything to Auto
          }
          
          SCI_Out_Int.whole = ExtDigOut.Mode.whole; // Echo back to monitoring program      
          SCIWriteIntOK = 1;     // Set flag to send to output buffer          
        }
      }              
    break; 
    
    
    case SI_SET_D_MAN :       
    // Set Manual Command Bits for External Digital Outputs
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          ExtDigOut.Manual.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          ExtDigOut.Manual.byte.low  = SCI_InParam2; // Set lower byte of Manual Word          
          
          SCI_Out_Int.whole = ExtDigOut.Manual.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer          
        }
      }              
    break;
    
    
    
    case SI_SET_A_MODE :       
    // Set Auto/Manual Mode Bytes for External Analog Outputs
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {        
          if ( (ManualOK1 == MANUAL_OK_PASSWORD1) && (ManualOK2 == MANUAL_OK_PASSWORD2))
          {
            ExtAnlgOut.Mode.o.THRTLMTR = SCI_InParam;  // Set upper byte of Mode Word
            ExtAnlgOut.Mode.o.PILOTSOL  = SCI_InParam2; // Set lower byte of Mode Word          
          }
          else
          {
            ExtAnlgOut.Mode.o.THRTLMTR = 0;  // If no passwords, force everything to auto
            ExtAnlgOut.Mode.o.PILOTSOL  = 0; // If no passwords, force everything to auto    
          }

          
          SCI_Out_Int.whole = ExtAnlgOut.Mode.whole; // Echo back to monitoring program      
          SCIWriteIntOK = 1;     // Set flag to send to output buffer          
        }
      }              
    break; 
    
    
    case SI_SET_A_MAN :       
    // Set Manual Command Bytes for External Analog Outputs
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {             
          ExtAnlgOut.Manual.o.THRTLMTR = SCI_InParam;  // Set upper byte of Manual Word
          ExtAnlgOut.Manual.o.PILOTSOL  = SCI_InParam2; // Set lower byte of Manual Word                  
          
          SCI_Out_Int.whole = ExtAnlgOut.Manual.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer          
        }
      }              
    break;  
    
    
    case SI_SET_HOURS :       
    // Set Hour Meter Hours
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word          
          
          AddEvent(LOG_HOURSET);   // Log that the Hour Meter has been modified externally     
          
          Hours = fracTempValue.whole;  // Set Hour part of Hour Meter
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer 
          
                  
        }
      }              
    break; 
    
    
    case SI_SET_SECS :       
    // Set Hour Meter Seconds
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word
          
          Seconds = fracTempValue.whole;  // Set Hour part of Hour Meter
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer           
             
        }
      }              
    break; 


    case SI_SET_PA_PROP_GAIN :       
    // Set Power Adapt Proportional Gain
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word  
      
          paKp = fracTempValue.whole;  // Set Power Adapt Proportional Gain
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer           
             
        }
      }              
    break; 

    case SI_SET_PA_INT_GAIN :       
    // Set Power Adapt Integral Gain
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word  
 
          paKi = fracTempValue.whole;  // Set Power Adapt Proportional Gain
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer           
             
        }
      }              
    break; 

    case SI_SET_RPMSP_RAMP :       
    // Set RPM Setpoint Ramp Rate   
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word  
 
          fRPM_SP_Inc = fracTempValue.whole;  // Set RPM Setpoint Ramp Rate 
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer           
             
        }
      }              
    break;  

    case SI_STORE_PA_GAINS :       
    // Store Power Adapt Gains into Flash
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word         
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer

          if (fracTempValue.whole == 1)  // user wants to store gains into flash
          {  
            // 1st, remember what's in flash now
            FieldDataRead();  // Remember what's in flash now
  
            // 2nd, set the Cal Parameters that we want to change                
            CalpaKpConst = paKp;
            CalpaKiConst = paKi;   
            CalfilteredRPM_SP_IncConst = fRPM_SP_Inc;
  
            // 3rd, write the data back to flash
            FieldDataWrite(); // Write back field calibration parameters
          }             
        }
      }              
    break;    

    case SI_SET_TU_PROP_GAIN :       
    // Set Throttle Up Proportional Gain
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word
  
          tKp = fracTempValue.whole;     // Set Throttle-Up Proportional Gain
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer           
             
        }
      }              
    break; 

    case SI_SET_TU_INT_GAIN :       
    // Set Throttle-Up Integral Gain
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word
  
          tKi = fracTempValue.whole;     // Set Throttle-Up Proportional Gain
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer           
             
        }
      }              
    break; 

    case SI_STORE_TU_GAINS :       
    // Store Throttle-Up Gains into Flash
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          fracTempValue.byte.high = SCI_InParam;  // Set upper byte of Manual Word
          fracTempValue.byte.low  = SCI_InParam2; // Set lower byte of Manual Word         
          
          SCI_Out_Int.whole = fracTempValue.whole; // Echo back to monitoring program         
          SCIWriteIntOK = 1;     // Set flag to send to output buffer

          if (fracTempValue.whole == 1)  // user wants to store gains into flash
          {  
            // 1st, remember what's in flash now
            FieldDataRead();  // Remember what's in flash now
  
            // 2nd, set the Cal Parameters that we want to change    
            CaltKpConst = tKp; 
            CaltKiConst = tKi; 
  
            // 3rd, write the data back to flash
            FieldDataWrite(); // Write back field calibration parameters
          }             
        }
      }              
    break;    
    
    
     
    // ************ Special Format Commands ************
    // These are command which don't limit themselves to any one standard protocol
    // and to must each be handled as a special case down below.
    
    
    case SI_MANUAL_ENABLE :        
      // Read Manual Enable Passwords, to confirm that manual operation is allowed
      if (!SCI_InParamRead)
        SCI_InParamRead = !ReadChar(&SCI_InParam);  // Read InParam, but only once
        
      if (SCI_InParamRead) // if already read 1st parameter, try to read 2nd one
      {
        if (!ReadChar(&SCI_InParam2))  // and if another character is present to be read
        {          
          ManualOK1 = SCI_InParam;  // Set ManualOK1 to password.  We'll check it later
          ManualOK2 = SCI_InParam2; // Set ManualOK2 to password.  We'll check it later
          
          SCI_Out_Int.whole = 0; // Tell Monitoring program that command was accepted        
          SCIWriteIntOK = 1;     // Set flag to send to output buffer          
        }
      }              
    break;  
    
    
    case SI_CLEAREVENTS :        
      // Read ClearEvents Password, to confirm that clearing event buffer is allowed
      if (!ReadChar(&SCI_InParam))  // if a character is present to be read     
      {                  
        if (SCI_InParam==CLEAREVENTS_OK_PWD)
        {
         
          for (i=0;i<EVENT_BUFF_SIZE-1;i++)
          {
            EventBuff[i].Event   = 0;
            EventBuff[i].Hours   = 0;
            EventBuff[i].Seconds = 0;  
          } 
          
          AddEvent(LOG_EVENTSCLEAR);   // Log that the Event Log has been cleared   
         
        }          
   
        
        SCI_Out_Int.whole = 0; // Tell Monitoring program that command was accepted        
        SCIWriteIntOK = 1;     // Set flag to send to output buffer          
      }               
    break;      
   
      
    
    case SI_GETSCALED :
      if (!ReadChar(&SCI_InParam))  // if a character is present to be read
      {
        if (SCI_InParam<ANALOG_CHANS)
          SCI_Out_Int.whole = Scaled[SCI_InParam];   // Ready value for transmission     
        else
          SCI_Out_Int.whole = 0;                 // Ready value for transmission   
          
        SCIWriteIntOK = 1;                 // Set flag to send to output buffer
      }
      break;
      
   
      
    case SI_GETUNSCALED :
      if (!ReadChar(&SCI_InParam))  // if a character is present to be read
      {
        if (SCI_InParam<ANALOG_CHANS)
          SCI_Out_Int.whole = UnScaled[SCI_InParam];   // Ready value for transmission     
        else
          SCI_Out_Int.whole = 0;                 // Ready value for transmission   
          
        SCIWriteIntOK = 1;                 // Set flag to send to output buffer
      }
    break;
      
      
    case SI_BATCH_GETSCALED :        
      // Write all Scaled Analog Inputs into serial output buffer
      if (!ReadChar(&SCI_InParam))  // if a character is present to be read
        {
          if (SCI_InParam<=ANALOG_CHANS)
          {
            for (i = 0;i<SCI_InParam;i++) 
            {     
              SCI_Out_Int.whole = Scaled[i];
              SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
              SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer                
            }  
          }
         
          SCICommand = 0;  // Zero out command pending flag   
        }      
    break;   
    
    
    case SI_BATCH_GETUNSCALED :        
      // Write all UnScaled Analog Inputs into serial output buffer
      if (!ReadChar(&SCI_InParam))  // if a character is present to be read
        {
          if (SCI_InParam<=ANALOG_CHANS)
          {
            for (i = 0;i<SCI_InParam;i++) 
            {     
              SCI_Out_Int.whole = UnScaled[i];
              SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
              SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer                
            }  
          }
         
          SCICommand = 0;  // Zero out command pending flag   
        }      
    break;  
    
    
    
      
    case SI_GETINFO :         
      SCI_Out_Err = WriteString(" Transmission Controller Rev 1 ",36);  
      SCI_Out_Err = WriteChar(SI_CR); // Send Carriage Return
      SCI_Out_Err = WriteChar(SI_LF); // Send Line Feed
      
      SCICommand = 0;  // Zero out command pending flag  
    break;            

    case SI_GET_MANINFO :        
      // Write all of manufacure information into serial output buffer           
      
      SCI_Out_Int.whole = VERSIONH;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = VERSIONL;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = MODELH;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = MODELL;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = CNFIGH;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = CNFIGL;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = VN;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = SN;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = MONTH;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = DAY;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   

      SCI_Out_Int.whole = YEAR;
      SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
      SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   
       
      
      SCICommand = 0;  // Zero out command pending flag  
    break;
   
    case SI_SET_MANINFO :        
      // Set Manufacture Information
      if (!ReadChar(&SCI_InParam))  // if a character is present to be read     
      {  
        
        switch (ManInfoByteCounter)
        {

          case 0 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 1 :         
            fracTempValue.byte.low = SCI_InParam;
            ManVERSIONH = fracTempValue.whole;
          break;        
  
          case 2 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 3 :         
            fracTempValue.byte.low = SCI_InParam;
            ManVERSIONL = fracTempValue.whole;
          break;
  
          case 4 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 5 :         
            fracTempValue.byte.low = SCI_InParam;
            ManMODELH = fracTempValue.whole;
          break;
  
          case 6 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 7 :         
            fracTempValue.byte.low = SCI_InParam;
            ManMODELL = fracTempValue.whole;
          break;
  
          case 8 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 9 :         
            fracTempValue.byte.low = SCI_InParam;
            ManCNFIGH = fracTempValue.whole;
          break;
  
          case 10 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 11 :         
            fracTempValue.byte.low = SCI_InParam;
            ManCNFIGL = fracTempValue.whole;
          break;
  
          case 12 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 13 :         
            fracTempValue.byte.low = SCI_InParam;
            ManVN = fracTempValue.whole;
          break;
  
          case 14 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 15 :         
            fracTempValue.byte.low = SCI_InParam;
            ManSN = fracTempValue.whole;
          break;
  
          case 16 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 17 :         
            fracTempValue.byte.low = SCI_InParam;
            ManMONTH = fracTempValue.whole;
          break;
  
          case 18 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 19 :         
            fracTempValue.byte.low = SCI_InParam;
            ManDAY = fracTempValue.whole;
          break;
  
          case 20 :         
            fracTempValue.byte.high = SCI_InParam;
          break;
  
          case 21 :         
            fracTempValue.byte.low = SCI_InParam;
            ManYEAR = fracTempValue.whole;
          break;

          default :
            SCI_Out_Int.whole = 1; // Tell Monitoring program that something bad happened
            SCIWriteIntOK = 1;     // Set flag to send to output buffer           
          break;
        } 

        ManInfoByteCounter++;   // increment byte counter
        
        if (ManInfoByteCounter==22)
        {
          SCI_Out_Int.whole = 0; // Tell Monitoring program that command was accepted        
          SCIWriteIntOK = 1;     // Set flag to send to output buffer          

          // 1st, remember what's in flash now
          // Don't need to do this, because I'm replacing all of it
  
          // 2nd, set the Cal Parameters that we want to change    
          // Just did this, above
  
          // 3rd, write the data back to flash
          ManufactureDataWrite(); // Write back manufacture data into flash
        }
      }               
    break;      
      
    case SI_GETEVENTS :        
       // Write entire event buffer into serial output buffer
       
      for (EventSendCounter = 0;EventSendCounter<EVENT_BUFF_SIZE-1;EventSendCounter++) 
      {     
        SCI_Out_Int.whole = EventBuff[EventSendCounter].Hours;
        SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
        SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   
      
        SCI_Out_Int.whole = EventBuff[EventSendCounter].Seconds;
        SCI_Out_Err += WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
        SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   
      
        SCI_Out_Err += WriteChar(EventBuff[EventSendCounter].Event);  // place Event byte into output buffer
      }   
      
      SCICommand = 0;  // Zero out command pending flag  
      break;   
      
      
     
      
    default :
      SCIWriteIntOK = 0;   // Zero out SCIWriteIntOK flag
      SCI_InParamRead = 0; // Zero out SCI_InParamRead flag
      SCIWriteIntOK = 0; // Zero out SCIWriteIntOK flag
     
  }
  
  
  if (SCIWriteIntOK)  // if OK to add SCI output integer to output buffer
  {
    SCI_Out_Err = WriteChar(SCI_Out_Int.byte.high); // place high byte into output buffer
    SCI_Out_Err += WriteChar(SCI_Out_Int.byte.low);  // place low byte into output buffer   
      
    SCI_InParamRead = 0; // Zero out SCI_InParamRead flag
    SCIWriteIntOK = 0; // Zero out SCIWriteIntOK flag
    SCICommand = 0;    // Zero out command pending flag  
  }
  
  if (SCI_Out_Err)  // if errors in placing data into output buffer
    AddEvent(TRANSMIT_ERR); // record error in log  
    
  if (SCI_In_Err)  // if errors in receiving data into input buffer
    AddEvent(RECEIVE_ERR); // record error in log   
     
}     
