#include "start16.h"

#define SYSPCR    0xFA21
#define SYNCR     0xFA04
#define SIMMCR    0xFA00
#define CSPAR0    0xFA44
#define CSPAR1    0xFA46
#define CSBARBT   0xFA48
#define CSORBT    0xFA4A


struct _tagStartup _startupData;

/**** This variable is initialized by the linker and allocated in ROM. */
                                     
#ifdef __cplusplus
  extern "C"  void _CALLYX(void); /* runtime routine for far function pointer calls */
#endif

/**********************************************************************/

#pragma NO_FRAME

static void Init (void)
  /**** Clears the memory area where data is allocated and copies initialization
        data from ROM to RAM.
          NOTE: XK, YK are undefined, EK points to '_startupData', and the stack
        and frame pointer already are set up (including their page registers).
   */
{ 
  asm {
    ZeroOut:
             LDAB  _startupData.pZeroOut      ; Get pointer to first descriptor
             LDY   _startupData.pZeroOut:1
             TBYK
             LDE   _startupData.nofZeroOuts   ; Get 'nofZeroOuts'     
             BEQ   CopyDown
    NextZeroOut:
             LDAB  0,Y                        ; Get start address of block to clear
             LDX   1,Y
             TBXK
             LDD   3,Y                        ; Get byte count
             AIY   #5                         ; Set ptr to next descriptor             
             BITB  #1
             BEQ   TestForZero
             CLR   0,X                        ; ODD: clear one byte
             AIX   #1
             SUBD  #1
    TestForZero:
             TSTD
             BEQ   TestNextBlock             
    NextWord:
             CLRW  0,X                        ; Clear word
             AIX   #2                         ; Inc pointer
             SUBD  #2                         ; Decrement byte count
             BNE   NextWord
    TestNextBlock:
             SUBE  #1                         ; Decrement 'nofZeroOuts'
             BNE   NextZeroOut
    
    /***** Now copy initialization data... */
    
    CopyDown:
             LDAB  _startupData.toCopyDownBeg   ; Get adress of first descriptor
             LDY   _startupData.toCopyDownBeg:1
             TBYK
    NextBlock:
             LDE   0,Y                          ; Load counter
             BEQ   FuncInits                    ; Zero terminates!
             LDAB  3,Y                          ; Get destination address
             LDX   4,Y
             TBXK
             AIY   #6                           ; Data is following immediately!
  Copy:
             SUBE  #2                           ; Decrement byte counter
             BMI   CopyByte                     ; If < 0 --> 1 byte left
             LDD   0,Y
             STD   0,X
             AIX   #2                           ; Increment addresses
             AIY   #2
             TSTE
             BNE   Copy                         ; Not finished yet
             BRA   NextBlock                    ; Copy next block
  CopyByte:
             LDAB  0,Y                          ; Copy last byte
             STAB  0,X
             AIY   #2                           ; Set Y to next counter
             BRA   NextBlock                    ; Copy next block

  FuncInits:
#ifdef __cplusplus
  #ifdef __ELF_OBJECT_FILE_FORMAT__
    #error "ELF/DWARF not supported yet!"
  #elif defined(__HIWARE_OBJECT_FILE_FORMAT__)
             LDX   _startupData.mInits:1
             LDAB  _startupData.mInits
             TBXK
     ; from now on, X is treated as as an index pointer to the init function table
      nextInit:
             LDY   0,X                      ; load page of constructor
             BNE loadAddr                   ; if not zero, it is not the last one
             TSTW  2,X                      ; test if it is the last entry
             BEQ   done                     ; no entry any more
      loadAddr:
             PSHM  K,X                      ; save actual address
             LDX   2,X                      ; load function address
             JSR   _CALLYX                  ; call initialization function
             PULM  K,X                      ; restore actual address
             AIX  #4                        ; next entry
             BRA   nextInit
done:     
  #else
    #error "unknown object file format!"
  #endif /* object file format */
#endif /* __cplusplus */
  } /* end asm */ 
} /* end Init */

/**********************************************************************/
                               
#pragma NO_FRAME 
  
#ifdef __cplusplus
extern "C" 
#endif
void _Startup (void)
  /**** Initializes the stack, calls 'Init' and then the main function.
        NOTE: Interrupts should be OFF during startup!! */
{
  for (;;) {     
    asm {
            
            ORP  #0x00E0            ;Disable Interrupts
            
            // Init Chip Selects, clock, and watchdog 
            LDX #0
            LDAB #0xF
            TBEK
            
            
            // Init SYPCR          ;SYPCR watch dog disable, BM enable 64clks 
            LDAB #0x04
            STAB SYSPCR
           
            // Init Onboard RAM 
            LDD  #0x8000                  
            STD  0xFB00              ;Disable internal SRAM array 
         
            LDD  #0x0005                  
            STD  0xFB04              ;internal RAM at 50000h to 503FEh
           
            LDD  #0x0000                  
            STD  0xFB06              ;internal RAM at 50000h to 503FEh
             
            LDD  #0x0000                  
            STD  0xFB00              ;Enable internal SRAM array     
            
          
            // Init SYNCR              
            LDD  #0x9801             ;SYNCR w=1,x=0 -> 13.107 MHz              
            STD SYNCR
          
            // Init SIMMCR           
            LDD  #0x40CF             ;MCR FRZSW enable, FRZBM enable, MM at FFFF...                            
            STD SIMMCR             
            // Init CSPAR0 
            LDD  #0x00F3             ;CSPAR0 -  Define CSBOOT -> CS5                         
            STD CSPAR0
            // Init CSPAR1                                         
            LDD  #0x0000             ;CS6->CS10 are outputs, not chip selects
            STD CSPAR1
            // Init CSBARBT
            LDD  #0x0005             ;CSBOOT cs000000,256k                       
            STD CSBARBT
            // Init CSORBT 
            LDD  #0x7830             ;CS0RBT - both,r/w,as,0wait     
            //LDD #0x7B70              ;CSORBT -13wait
            STD CSORBT
            
            
            LDD  #0x0403                  
            STD  0xFA50              ;CS1=WRRAM cs40000,64k 
             
            LDD  #0x3830                  
            STD  0xFA52              ;CS1 = lower,r/w,0wait
             
            LDD  #0x0403                  
            STD  0xFA54              ;CS2= WRRAM cs40000,64k 
             
            LDD  #0x5830                  
            STD  0xFA56              ;CS2 = upper,r/w,0wait
            
                   
            LDAB #0
            TBEK
            // End Init Chip Select 
            
            
            
            LDZ   #0                           ; terminate dynamic link (for debugger)
            LDAB  @_startupData:PAGE           ; load page of _startupData
            TBEK                               ; into EK
            TST   _startupData.flags         
            BNE   Initialize
            LDAB  _startupData.stackOffset   ; init SP
            LDS   _startupData.stackOffset:1
            TBSK 
            TBZK
      Initialize:                 
            JSR Init
      CallMain:                             
            LDY   _startupData.main            ; load address of main function
            LDX   _startupData.main:2
            LDAB  _startupData.dataPage        ; data page
            TBXK
            TBYK
            TBEK
                   
            BSR  DoCall                        ; call main function indirectly!

    } /* end asm */
  } /* end loop forever */
  asm{                         
    DoCall:  
            AIX  #2                             ; correct address
            PSHM X,Y                            ; push PK:PC
            RTS                                 ; return is call !
      
  } /* end asm */                         
} /* end _Startup */

/**********************************************************************/
/* end startup.c */

