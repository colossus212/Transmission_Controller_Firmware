// Page numbers referenced are from the Motorola Semiconductor Technical Data
// MC68HC16Z1 Technical Summary Document # MC68HC16Z1TS/D
// Registers are defined here, in the order they are listed in the 
// Technical Summary, not in numeric order.


// It's important to use the -Cb compiler option to use the following bitfields   

// ***************  System Integration Module Registers *************
// *****       System Integration Module begins on Page 40       ****



// pg 44
#define SIMMCR  (*((SIMMCR_type*)(0xFFA00))) // SIM Configuration Register
typedef struct  {
   unsigned int IARB  :4,   // Interupt Arbitration Identification Number
                SP1   :2,   // Not Used      
                MM    :1,   // Module Mapping 1=>Modules at FFF000-FFFFFF
                SUPV  :1,   // 1=>Supervised Mode 
                SHEN  :2,   // Show Cycle Enable 00=>Show Cycles Disabled
                SP2   :1,   // Not Used    
                SLVEN :1,   // Factory Test Mode Enabled
                SP3   :1,   // Not Used                            
                FRZBM :1,   // Freeze Bus Monitor Eable             
                FRZSW :1,   // Freeze Software Enable
                EXOFF :1;   // 1=>CLKOUT pin becomes high-impedance (no CLKOUT sig)
                    } SIMMCR_type; 



// pg 45
#define SYPCR  (*((SYPCR_type*)(0xFFA21))) // System Protection Control Register
typedef struct  {
   unsigned int BMT   :2,   // Bus Monitor Timing
                BME   :1,   // Bus Monitor External Enable
                HME   :1,   // Halt Monitor Enable
                SWT   :2,   // Software Watchdog Timing
                SWP   :1,   // Software Watchdog Prescale
                SWE   :1;   // Software Watchdog Enable                     
                    } SYPCR_type; 
                    
                    
                    
                     
              
// pg 47
#define SWSR (*((unsigned char*)(0xFFA27))) // Software Service Register
                    

// pg 50
#define SYNCR  (*((SYNCR_type*)(0xFFA04))) // Clock Synthesizer Control Register
typedef struct  {
   unsigned int STEXT :1,   // Stop Mode External Clock
                STSIM :1,   // Stop Mode System Integration Clock
                RSTEN :1,   // Reset Enable
                SLOCK :1,   // Synthesizer Lock Flag
                SLIMP :1,   // Limp Mode Flag
                SP1   :2,   // Not Used
                EDIV  :1,   // E Clock Divide Rate
                Y     :6,   // Frequency Control Counter
                X     :1,   // Frequency Control Bit (Prescale)
                W     :1;   // Frequency Control (VCO)
                    } SYNCR_type; 
                    
// pg 51
#define PICR  (*((PICR_type*)(0xFFA22))) // Periodic Interrupt Control Register
typedef struct  {
   unsigned int PIV   :8,   // Periodic Interrupt Vector
                PIRQL :3,   // Periodic Interrupt Request Level
                SP1   :5;   // Not Used                                     
                    } PICR_type;  
                    
                    
// pg 52
#define PITR  (*((PITR_type*)(0xFFA24))) // Periodic Interrupt Timer Register
typedef struct  {
   unsigned int PITM  :8,   // Periodic Interrupt Timing Modulus Field
                PTP   :1,   // Periodic Timer Prescaler Control
                SP1   :7;   // Not Used                                     
                    } PITR_type;      
                    
                    

// pg 59
#define CSPAR0  (*((CSPAR0_type*)(0xFFA44))) // Chip Select Pin Assignment Reg 0
typedef struct  {
   unsigned int CSBOOT:2,   // CSBOOT Pin Assignment
                CS0   :2,   // CS0 (BR) Pin Assignment 
                CS1   :2,   // CS1 (BG) Pin Assignment 
                CS2   :2,   // CS2 (BGACK) Pin Assignment 
                CS3   :2,   // CS3 (FC0) Pin Assignment 
                CS4   :2,   // CS4 (FC1) Pin Assignment 
                CS5   :2,   // CS5 (FC2) Pin Assignment 
                SP1   :2;   // Not Used                                       
                    } CSPAR0_type;          
                    
// pg 59
#define CSPAR1  (*((CSPAR1_type*)(0xFFA46))) // Chip Select Pin Assignment Reg 1
typedef struct  {
   unsigned int CS6   :2,   // CS6  (ADDR19) Pin Assignment 
                CS7   :2,   // CS7  (ADDR20) Pin Assignment 
                CS8   :2,   // CS8  (ADDR21) Pin Assignment 
                CS9   :2,   // CS9  (ADDR22) Pin Assignment 
                CS10  :2,   // CS10 (ADDR23) Pin Assignment                 
                SP1   :6;   // Not Used                                       
                    } CSPAR1_type;


// pg 60
#define CSBARBT  (*((CSBAR_type*)(0xFFA48))) // Chip-Sel Base Addr Reg Boot ROM
#define CSBAR0   (*((CSBAR_type*)(0xFFA4C))) // Chip-Sel  0 Base Addr Reg
#define CSBAR1   (*((CSBAR_type*)(0xFFA50))) // Chip-Sel  1 Base Addr Reg
#define CSBAR2   (*((CSBAR_type*)(0xFFA54))) // Chip-Sel  2 Base Addr Reg
#define CSBAR3   (*((CSBAR_type*)(0xFFA58))) // Chip-Sel  3 Base Addr Reg
#define CSBAR4   (*((CSBAR_type*)(0xFFA5C))) // Chip-Sel  4 Base Addr Reg
#define CSBAR5   (*((CSBAR_type*)(0xFFA60))) // Chip-Sel  5 Base Addr Reg
#define CSBAR6   (*((CSBAR_type*)(0xFFA64))) // Chip-Sel  6 Base Addr Reg
#define CSBAR7   (*((CSBAR_type*)(0xFFA68))) // Chip-Sel  7 Base Addr Reg
#define CSBAR8   (*((CSBAR_type*)(0xFFA6C))) // Chip-Sel  8 Base Addr Reg
#define CSBAR9   (*((CSBAR_type*)(0xFFA70))) // Chip-Sel  9 Base Addr Reg
#define CSBAR10  (*((CSBAR_type*)(0xFFA74))) // Chip-Sel 10 Base Addr Reg
typedef struct  {
   unsigned int BLKSZ :3,   // Block Size 4=>128K
                CSBAR :9,   // Base Address
                UPADDR:4;   // 0 or 15: ADDR20->ADDR23 must be same as ADDR19
                    } CSBAR_type; 
                    
                    
// pg 61
#define CSORBT  (*((CSOR_type*)(0xFFA4A))) // Chip-Select Boot ROM Option Register
#define CSOR0   (*((CSOR_type*)(0xFFA4E))) // Chip-Select  0 Option Register
#define CSOR1   (*((CSOR_type*)(0xFFA52))) // Chip-Select  1 Option Register
#define CSOR2   (*((CSOR_type*)(0xFFA56))) // Chip-Select  2 Option Register
#define CSOR3   (*((CSOR_type*)(0xFFA5A))) // Chip-Select  3 Option Register
#define CSOR4   (*((CSOR_type*)(0xFFA5E))) // Chip-Select  4 Option Register
#define CSOR5   (*((CSOR_type*)(0xFFA62))) // Chip-Select  5 Option Register
#define CSOR6   (*((CSOR_type*)(0xFFA66))) // Chip-Select  6 Option Register
#define CSOR7   (*((CSOR_type*)(0xFFA6A))) // Chip-Select  7 Option Register
#define CSOR8   (*((CSOR_type*)(0xFFA6E))) // Chip-Select  8 Option Register
#define CSOR9   (*((CSOR_type*)(0xFFA72))) // Chip-Select  9 Option Register 
#define CSOR10  (*((CSOR_type*)(0xFFA76))) // Chip-Select 10 Option Register 
typedef struct  {
   unsigned int AVEC  :1,   // Autovector Enable
                IPL   :3,   // Interrupt Priority Level
                SPACE :2,   // Address Space 2=>Supervisor space 0=>CPU Space
                DSACK :4,   // Data Strobe Acknowledge
                STRB  :1,   // 0=>Address Strobe 1=>Data Strobe
                RW    :2,   // 3=>Both Read and Write
                BYTE  :2,   // 3=>Both Bytes
                MODE  :1;   // 0=>Asynchronous Mode
                    } CSOR_type;  
                    
// pg 64
#define PORTC  (*((PORTC_type*)(0xFFA41))) // Port C Data Register
typedef struct  {
   unsigned int PC0   :1,   // Port C Pin 0
                PC1   :1,   // Port C Pin 1
                PC2   :1,   // Port C Pin 2
                PC3   :1,   // Port C Pin 3
                PC4   :1,   // Port C Pin 4
                PC5   :1,   // Port C Pin 5
                PC6   :1,   // Port C Pin 6
                SP1   :1;   // Not used
                    } PORTC_type;       
                    
// pg 64
#define PORTE  (*((PORTE_type*)(0xFFA11))) // Port E Data Register
typedef struct  {
   unsigned int PE0   :1,   // Port E Pin 0
                PE1   :1,   // Port E Pin 1
                PE2   :1,   // Port E Pin 2
                PE3   :1,   // Port E Pin 3
                PE4   :1,   // Port E Pin 4
                PE5   :1,   // Port E Pin 5
                PE6   :1,   // Port E Pin 6
                PE7   :1;   // Port E Pin 7
                    } PORTE_type;  

// pg 64
#define DDRE (*((DDRE_type*)(0xFFA15))) // Port E Data Direction Register
typedef struct  {
   unsigned int DDE0  :1,   // 0=>Input 1=>Output Port E Pin 0 
                DDE1  :1,   // 0=>Input 1=>Output Port E Pin 1
                DDE2  :1,   // 0=>Input 1=>Output Port E Pin 2
                DDE3  :1,   // 0=>Input 1=>Output Port E Pin 3
                DDE4  :1,   // 0=>Input 1=>Output Port E Pin 4
                DDE5  :1,   // 0=>Input 1=>Output Port E Pin 5
                DDE6  :1,   // 0=>Input 1=>Output Port E Pin 6
                DDE7  :1;   // 0=>Input 1=>Output Port E Pin 7
                    } DDRE_type;  
                    
// pg 65
#define PEPAR (*((PEPAR_type*)(0xFFA17))) // Port E Pin Assignment Register
typedef struct  {
   unsigned int PEPA0  :1,   // 0=>I/O 1=>Bus Control Pin 0 
                PEPA1  :1,   // 0=>I/O 1=>Bus Control Pin 1
                PEPA2  :1,   // 0=>I/O 1=>Bus Control Pin 2
                PEPA3  :1,   // 0=>I/O 1=>Bus Control Pin 3
                PEPA4  :1,   // 0=>I/O 1=>Bus Control Pin 4
                PEPA5  :1,   // 0=>I/O 1=>Bus Control Pin 5
                PEPA6  :1,   // 0=>I/O 1=>Bus Control Pin 6
                PEPA7  :1;   // 0=>I/O 1=>Bus Control Pin 7
                    } PEPAR_type;            
                    
                    
// pg 65
#define PORTF  (*((PORTF_type*)(0xFFA19))) // Port F Data Register
typedef struct  {
   unsigned int PF0   :1,   // Port F Pin 0
                PF1   :1,   // Port F Pin 1
                PF2   :1,   // Port F Pin 2
                PF3   :1,   // Port F Pin 3
                PF4   :1,   // Port F Pin 4
                PF5   :1,   // Port F Pin 5
                PF6   :1,   // Port F Pin 6
                PF7   :1;   // Port F Pin 7
                    } PORTF_type;  


// pg 65
#define DDRF (*((DDRF_type*)(0xFFA1D))) // Port F Data Direction Register
typedef struct  {
   unsigned int DDF0  :1,   // 0=>Input 1=>Output Port F Pin 0 
                DDF1  :1,   // 0=>Input 1=>Output Port F Pin 1
                DDF2  :1,   // 0=>Input 1=>Output Port F Pin 2
                DDF3  :1,   // 0=>Input 1=>Output Port F Pin 3
                DDF4  :1,   // 0=>Input 1=>Output Port F Pin 4
                DDF5  :1,   // 0=>Input 1=>Output Port F Pin 5
                DDF6  :1,   // 0=>Input 1=>Output Port F Pin 6
                DDF7  :1;   // 0=>Input 1=>Output Port F Pin 7
                    } DDRF_type;  
                    

// pg 66
#define PFPAR (*((PFPAR_type*)(0xFFA1F))) // Port F Pin Assignment Register
typedef struct  {
   unsigned int PFPA0  :1,   // 0=>I/O  1=>MODCLK
                PFPA1  :1,   // 0=>I/O  1=>IRQ1
                PFPA2  :1,   // 0=>I/O  1=>IRQ2
                PFPA3  :1,   // 0=>I/O  1=>IRQ3
                PFPA4  :1,   // 0=>I/O  1=>IRQ4
                PFPA5  :1,   // 0=>I/O  1=>IRQ5
                PFPA6  :1,   // 0=>I/O  1=>IRQ6
                PFPA7  :1;   // 0=>I/O  1=>IRQ7
                    } PFPAR_type; 


// pg 66
#define RSR (*((RSR_type*)(0xFFA07))) // Reset Status Register
typedef struct  {
   unsigned int TST    :1,   // Test Submodule Reset
                SYS    :1,   // System Reset
                LOC    :1,   // Loss of Clock Reset
                SP1    :1,   // Not Used
                HLT    :1,   // Halt Monitor Reset
                SW     :1,   // Software Watchdog Reset
                POW    :1,   // Power-Up Reset
                EXT    :1;   // External Reset
                    } RSR_type; 

                    
                                                                  
                                            
// *************************  ADC Registers *************************
// **     Analog to Digital Converter Module begins on Page 73     **
                    

// pg 76
#define ADCMCR  (*((ADCMCR_type*)(0xFF700))) // ADC Module Configuration Register
typedef struct  {
   unsigned int SPARE1  :7,   // Not Used                        
                SUPV    :1,   // 0 => unrestricted access                
                SPARE2  :5,   // Not Used                   
                FRZ     :2,   // 0=>End of Conversion                   
                STOP    :1;   // 0=>Normal Operation                
                    } ADCMCR_type; 
                    
                    
// pg 76
#define ADTEST (*((unsigned int*)(0xFF702))) // ADC Test Register

// pg 76
#define PORTADA (*((unsigned char*)(0xFF707))) // Port ADA Data Register

 
// pg 77
#define ADCTL0  (*((ADCTL0_type*)(0xFF70A))) // A/D Control Register 0
typedef struct  {
   unsigned int PRS    :5,   // Prescaler Rate Selection                        
                STS    :2,   // Sample Time Select              
                RES10  :1,   // 0=>8-bit conversion                
                SPARE  :8;   // Not Used                            
                    } ADCTL0_type; 
                    
// pg 78
#define ADCTL1  (*((ADCTL1_type*)(0xFF70C))) // A/D Control Register 1
typedef struct  {
   unsigned int CHAN   :4,   // Channel Selection Field 
                S8CM   :1,   // 1=>Eight Conversion Sequence
                MULT   :1,   // 1=>Convert Block of Channels
                SCAN   :1,   // 1=>Continuous Conversion      
                SPARE  :9;   // Not Used                            
                    } ADCTL1_type; 
                    

// pg 81
#define ADSTAT  (*((ADSTAT_type*)(0xFF70E))) // A/D Status Register
typedef struct ADSTAT_type {
   unsigned int CCF    :8,   // Conversion Complete Fields                      
                CCTR   :3,   // Conversion Counter Field              
                SPARE  :4,   // Not Used           
                SCF    :1;   // Sequence Complete Flag                           
                    }; 
                    
// pg 81
#define RSLT0 (*((unsigned short*)(0xFF710))) // A/D Result Register 0
#define RSLT1 (*((unsigned short*)(0xFF712))) // A/D Result Register 1
#define RSLT2 (*((unsigned short*)(0xFF714))) // A/D Result Register 2
#define RSLT3 (*((unsigned short*)(0xFF716))) // A/D Result Register 3
#define RSLT4 (*((unsigned short*)(0xFF718))) // A/D Result Register 4
#define RSLT5 (*((unsigned short*)(0xFF71A))) // A/D Result Register 5
#define RSLT6 (*((unsigned short*)(0xFF71C))) // A/D Result Register 6
#define RSLT7 (*((unsigned short*)(0xFF71E))) // A/D Result Register 7




// *****************  Queued Serial Module Registers ****************
// *****            Queued Serial Module begins on Page 82       ****

                                
// pg 84
#define QSMCR  (*((QSMCR_type*)(0xFFC00))) // QSM Configuration Register
typedef struct  {
   unsigned int IARB  :4,   // Interupt Arbitration Identification Number
                SP1   :3,   // Not Used                               
                SUPV  :1,   // 1=>Supervised Mode 
                SP2   :5,   // Not Used                                
                FRZ0  :1,   // Reserved                
                FRZ1  :1,   // 0=>Ignore Freeze Command (Background mode)
                STOP  :1;   // 0=>Normal Operation                
                    } QSMCR_type; 
                    
                    
// pg 85
#define QTEST (*((unsigned short*)(0xFFC02))) // QSM Test Register



// pg 85
#define QILR  (*((QILR_type*)(0xFFC04))) // QSM Interrupt Levels Register
typedef struct  {
   unsigned int ILSCI  :3,   // Interrupt Level of SCI                          
                ILQSPI :3,   // Interrupt Level of SPI
                SP2    :2;   // Not Used         
                    } QILR_type;  
                      
                      
// pg 85
#define QIVR (*((unsigned char*)(0xFFC05))) // QSM Interrupt Vector Register 


// pg 86
#define PORTQS  (*((PORTQS_type*)(0xFFC15))) // PORT QS Data Register
typedef struct  {
   unsigned int MISO  :1,   // Master In Slave Out                         
                MOSI  :1,   // Master Out Slave In
                SCK   :1,   // Serial Clock
                PCS0  :1,   // PCS0/Slave Select
                PCS1  :1,   // PCS1/Data4
                PCS2  :1,   // PCS2/Data5
                PCS3  :1,   // PCS3/Data6
                TXD   :1;   // TXD/Data7 
                    } PORTQS_type;  

// pg 86
#define PQSPAR  (*((PQSPAR_type*)(0xFFC16))) // PORT QS Pin Assignment Register
typedef struct  {
   unsigned int MISO  :1,   // Master In Slave Out                         
                MOSI  :1,   // Master Out Slave In
                SP2   :1,   // Not Used
                PCS0  :1,   // PCS0/Slave Select
                PCS1  :1,   // PCS1/Data4
                PCS2  :1,   // PCS2/Data5
                PCS3  :1,   // PCS3/Data6
                SP3   :1;   // Not Used  
                    } PQSPAR_type; 
                    
                    
// pg 87
#define DDRQS  (*((DDRQS_type*)(0xFFC17))) // DDRQS Data Register
typedef struct  {
   unsigned int MISO  :1,   // Master In Slave Out                         
                MOSI  :1,   // Master Out Slave In
                SCK   :1,   // Serial Clock
                PCS0  :1,   // PCS0/Slave Select
                PCS1  :1,   // PCS1/Data4
                PCS2  :1,   // PCS2/Data5
                PCS3  :1,   // PCS3/Data6
                TXD   :1;   // TXD/Data7 
                    } DDRQS_type;                                           
                                

// ***** Begin QSPI Registers pg 89 *****

// pg 90                    
#define SPCR0  (*((SPCR0_type*)(0xFFC18))) // QSPI Control Register 0
typedef struct  {
   unsigned int SPBR  :8,   // Serial Clock Baud Rate
                CPHA  :1,   // Clock Phase                       
                CPOL  :1,   // Clock Polarity
                BITS  :4,   // Bits/Transfer (8-15) or 0=16
                WOMQ  :1,   // Wired-OR Mode for QSPI Pins
                MSTR  :1;   // 1=>Master  Master/Slave Mode Select            
                    } SPCR0_type;                     
                    
                    
// pg 91
#define SPCR1  (*((SPCR1_type*)(0xFFC1A))) // QSPI Control Register 1
typedef struct  {
   unsigned int DTL   :8,   // Length of Delay after Transfer
                DSCKL :7,   // Delay before SCK
                SPE   :1;   // QSPI Enable 1=>Pins allocated by PQSPAR cntld by SPI
                    } SPCR1_type;         
                    
                    
// pg 92
#define SPCR2  (*((SPCR2_type*)(0xFFC1C))) // QSPI Control Register 2
typedef struct  {
   unsigned int NEWQP :4,   // New Queue Pointer Value
                SP1   :4,   // Not Used
                ENDQP :4,   // Ending Queue Pointer
                SP2   :1,   // Not Used
                WRTO  :1,   // Wrap To
                WREN  :1,   // Wrap Enable
                SPIFIE:1;   // SPI Finished Interrupt Enable
                    } SPCR2_type;                
                    
// pg 93
#define SPCR3  (*((SPCR3_type*)(0xFFC1E))) // QSPI Control Register 3
typedef struct  {
   unsigned int HALT  :1,   // 0=>Halt not enabled
                HMIE  :1,   // HALTA and MODF Interrupt Enable
                LOOPQ :1,   // QSPI Loop Mode
                SP2   :5;   // Not Used                              
                    } SPCR3_type; 
                                           

// pg 93
#define SPSRtest (*((unsigned char*)(0xFFC1F))) // QSPI Status Register    
#define SPSR  (*((SPSR_type*)(0xFFC1F))) // QSPI Status Register  
typedef struct {
   unsigned int CPTQP :4,   // QSPI Status Register
                SP1   :1,   // Not Used
                HALTA :1,   // Halt Acknowledge Flag
                MODF  :1,   // Mode Fault Flag
                SPIF  :1;   // QSPI Finished Flag   
                    } SPSR_type;    
                    
// pg 94
#define RR0 (*((unsigned short*)(0xFFD00))) // Receive Data RAM 0     
#define RR1 (*((unsigned short*)(0xFFD02))) // Receive Data RAM 1
#define RR2 (*((unsigned short*)(0xFFD04))) // Receive Data RAM 2     
#define RR3 (*((unsigned short*)(0xFFD06))) // Receive Data RAM 3     
#define RR4 (*((unsigned short*)(0xFFD08))) // Receive Data RAM 4     
#define RR5 (*((unsigned short*)(0xFFD0A))) // Receive Data RAM 5     
#define RR6 (*((unsigned short*)(0xFFD0C))) // Receive Data RAM 6     
#define RR7 (*((unsigned short*)(0xFFD0E))) // Receive Data RAM 7     
#define RR8 (*((unsigned short*)(0xFFD10))) // Receive Data RAM 8     
#define RR9 (*((unsigned short*)(0xFFD12))) // Receive Data RAM 9     
#define RRA (*((unsigned short*)(0xFFD14))) // Receive Data RAM A     
#define RRB (*((unsigned short*)(0xFFD16))) // Receive Data RAM B     
#define RRC (*((unsigned short*)(0xFFD18))) // Receive Data RAM C     
#define RRD (*((unsigned short*)(0xFFD1A))) // Receive Data RAM D     
#define RRE (*((unsigned short*)(0xFFD1C))) // Receive Data RAM E     
#define RRF (*((unsigned short*)(0xFFD1E))) // Receive Data RAM F     


// pg 95
#define TR0 (*((unsigned short*)(0xFFD20))) // Transmit Data RAM 0     
#define TR1 (*((unsigned short*)(0xFFD22))) // Transmit Data RAM 1
#define TR2 (*((unsigned short*)(0xFFD24))) // Transmit Data RAM 2     
#define TR3 (*((unsigned short*)(0xFFD26))) // Transmit Data RAM 3     
#define TR4 (*((unsigned short*)(0xFFD28))) // Transmit Data RAM 4     
#define TR5 (*((unsigned short*)(0xFFD2A))) // Transmit Data RAM 5     
#define TR6 (*((unsigned short*)(0xFFD2C))) // Transmit Data RAM 6     
#define TR7 (*((unsigned short*)(0xFFD2E))) // Transmit Data RAM 7     
#define TR8 (*((unsigned short*)(0xFFD30))) // Transmit Data RAM 8     
#define TR9 (*((unsigned short*)(0xFFD32))) // Transmit Data RAM 9     
#define TRA (*((unsigned short*)(0xFFD34))) // Transmit Data RAM A     
#define TRB (*((unsigned short*)(0xFFD36))) // Transmit Data RAM B     
#define TRC (*((unsigned short*)(0xFFD38))) // Transmit Data RAM C     
#define TRD (*((unsigned short*)(0xFFD3A))) // Transmit Data RAM D     
#define TRE (*((unsigned short*)(0xFFD3C))) // Transmit Data RAM E     
#define TRF (*((unsigned short*)(0xFFD3E))) // Transmit Data RAM F 

// pg 95
#define CR0 (*((SPICR_type*)(0xFFD40))) // Command RAM 0     
#define CR1 (*((SPICR_type*)(0xFFD41))) // Command RAM 1
#define CR2 (*((SPICR_type*)(0xFFD42))) // Command RAM 2     
#define CR3 (*((SPICR_type*)(0xFFD43))) // Command RAM 3     
#define CR4 (*((SPICR_type*)(0xFFD44))) // Command RAM 4     
#define CR5 (*((SPICR_type*)(0xFFD45))) // Command RAM 5     
#define CR6 (*((SPICR_type*)(0xFFD46))) // Command RAM 6     
#define CR7 (*((SPICR_type*)(0xFFD47))) // Command RAM 7     
#define CR8 (*((SPICR_type*)(0xFFD48))) // Command RAM 8     
#define CR9 (*((SPICR_type*)(0xFFD49))) // Command RAM 9     
#define CRA (*((SPICR_type*)(0xFFD4A))) // Command RAM A     
#define CRB (*((SPICR_type*)(0xFFD4C))) // Command RAM B     
#define CRC (*((SPICR_type*)(0xFFD4C))) // Command RAM C     
#define CRD (*((SPICR_type*)(0xFFD4D))) // Command RAM D     
#define CRE (*((SPICR_type*)(0xFFD4E))) // Command RAM E     
#define CRF (*((SPICR_type*)(0xFFD4F))) // Command RAM F 
typedef struct {
   unsigned int PCS0  :1,   // Peripheral Chip Select 0
                PCS1  :1,   // Peripheral Chip Select 1
                PCS2  :1,   // Peripheral Chip Select 2
                PCS3  :1,   // Peripheral Chip Select 3
                DSCK  :1,   // PCS to SCK Delay
                DT    :1,   // Delay after Transfer
                BITSE :1,   // Bits per Transfer Enable
                CONT  :1;   // Continue 
                    } SPICR_type;   
                    
                    
#define SPI_CR0 (*((unsigned char*)(0xFFD40))) // Command RAM 0     
#define SPI_CR1 (*((unsigned char*)(0xFFD41))) // Command RAM 1                        
#define SPI_CR2 (*((unsigned char*)(0xFFD42))) // Command RAM 2     
#define SPI_CR3 (*((unsigned char*)(0xFFD43))) // Command RAM 3     
#define SPI_CR4 (*((unsigned char*)(0xFFD44))) // Command RAM 4     
#define SPI_CR5 (*((unsigned char*)(0xFFD45))) // Command RAM 5     
#define SPI_CR6 (*((unsigned char*)(0xFFD46))) // Command RAM 6     
#define SPI_CR7 (*((unsigned char*)(0xFFD47))) // Command RAM 7     
#define SPI_CR8 (*((unsigned char*)(0xFFD48))) // Command RAM 8     
#define SPI_CR9 (*((unsigned char*)(0xFFD49))) // Command RAM 9     
#define SPI_CRA (*((unsigned char*)(0xFFD4A))) // Command RAM A     
#define SPI_CRB (*((unsigned char*)(0xFFD4B))) // Command RAM B     
#define SPI_CRC (*((unsigned char*)(0xFFD4C))) // Command RAM C     
#define SPI_CRD (*((unsigned char*)(0xFFD4D))) // Command RAM D     
#define SPI_CRE (*((unsigned char*)(0xFFD4E))) // Command RAM E     
#define SPI_CRF (*((unsigned char*)(0xFFD4F))) // Command RAM F  
    


// ***** Begin SCI Registers pg 97 *****

// pg 97
#define SCCR0  (*((SCCR0_type*)(0xFFC08))) // SCI Control Register 0
typedef struct  {
   unsigned int SCBR  :13,  // Baud Rate                
                SP1   :3;   // Not Used      
                    } SCCR0_type;  
                    
                    
// pg 97
#define SCCR1  (*((SCCR1_type*)(0xFFC0A))) // SCI Control Register 1
typedef struct  {
   unsigned int SBK   :1,   // Send Break
                RWU   :1,   // Receiver Wakeup
                RE    :1,   // Receiver Enable
                TE    :1,   // Transmitter Enable
                ILIE  :1,   // Idle-Line Interrupt Enable
                RIE   :1,   // Receiver Interrupt Enable
                TCIE  :1,   // Transmit Complete Interrupt Enable
                TIE   :1,   // Transmit Interrupt Enable
                WAKE  :1,   // Wakeup by Address Mark
                M     :1,   // Mode Select
                PE    :1,   // Parity Enable
                PT    :1,   // Parity Type
                ILT   :1,   // Idle-Line Detect Type
                WOMS  :1,   // Wired-OR Mode for SCI Pins
                LOOPS :1,   // Liip Mode
                SP1   :1;   // Not Used                              
                    } SCCR1_type;        
                    
                    
// pg 99
#define SCSR  (*((SCSR_type*)(0xFFC0C))) // SCI Status Register
typedef struct  {
   unsigned int PF    :1,   // Parity Flag Error
                FE    :1,   // Framing Error
                NF    :1,   // Noise Error Flag
                OR    :1,   // Overrun Error Flag
                IDLE  :1,   // Idle-Line Detected Flag
                RAF   :1,   // Receiver Active Flag
                RDRF  :1,   // Receive Data Register Full Flag
                TC    :1,   // Transmit Complete Flag
                TDRE  :1,   // Transmit Data Register Empty Flag
                SP1   :7;   // Not Used                                       
                    } SCSR_type;  
                    
// pg 101  Note, I am ignoring the 9th bit, so can't use 9-bit serial protocols
#define SCDR (*((unsigned char*)(0xFFC0F))) // SCI Data Register



// *****************   Standby RAM Module Registers  ****************   
// *****             Standby RAM Module begins on Page 102       ****
                                           

// pg 102
#define RAMMCR  (*((RAMMCR_type*)(0xFFB00))) // RAM Module Configuration Register
typedef struct  {
   unsigned int RASP  :2,   // RAM Array Space Field
                SP2   :1,   // Not Used
                RLCK  :1,   // RAM Base Address Lock
                SP3   :3,   // Not used
                STOP  :1;   // Stop Control
                    } RAMMCR_type;
                    
// pg 103
#define RAMTST (*((unsigned short*)(0xFFB02))) // RAM Test Register            

// pg 103     Note ADDR20->ADDR23 must be same as ADDR19
#define RAMBAH (*((unsigned char*)(0xFFB05))) // High Byte of RAM Address

// pg 103
#define RAMBAL (*((unsigned short*)(0xFFB06))) // Two Low Bytes of RAM Address 



// **************   General Purpose Timer Module Registers  *************
// *****           General Purpose Timer begins on Page 105          ****



// pg 110
#define GPTMCR  (*((GPTMCR_type*)(0xFF900))) // GPT Module Configuration Register
typedef struct  {
   unsigned int IARB  :4,   // Interrupt Arbitration Identification                
                SP1   :3,   // Not used
                SUPV  :1,   // Supervisor Data Space
                SP2   :3,   // Not Used
                INCP  :1,   // Increment Prescaler
                STOPP :1,   // Stop Prescaler
                FRZ0  :1,   // FREEZE Response
                FRZ1  :1,   // Not Implemented 
                STOP  :1;   // Stop Clocks                                  
                    } GPTMCR_type;  
                    
// pg 110
#define GPTTST (*((unsigned short*)(0xFF902))) // GPT Test Register  

// pg 110
#define GPTICR  (*((GPTICR_type*)(0xFF904))) // GPT Interrupt Configuration Register
typedef struct  {
   unsigned int SP1   :4,   // Not used             
                IVBA  :4,   // Interrupt Vector Base Address MS Nibble
                IPL   :3,   // Interrupt Priority Level
                SP2   :1,   // Not Used                
                IPA   :4;   // Interrupt Priority Adjust
                    } GPTICR_type; 
                    
                    
// pg 111    0=>Input 1=> Output
#define DDRGP  (*((PORTGP_type*)(0xFF906))) // Port GP Data Direction Register

// pg 111
#define PORTGP (*((PORTGP_type*)(0xFF907))) // Port GP Data Register  
typedef struct  {
   unsigned int PGP0 :1,  // Port GP Bit 0         
                PGP1 :1,  // Port GP Bit 1   
                PGP2 :1,  // Port GP Bit 2    
                PGP3 :1,  // Port GP Bit 3          
                PGP4 :1,  // Port GP Bit 4                                 
                PGP5 :1,  // Port GP Bit 5   
                PGP6 :1,  // Port GP Bit 6    
                PGP7 :1;  // Port GP Bit 7    
                    } PORTGP_type;
         
                    
// pg 111
#define OC1  (*((OC1_type*)(0xFF908))) // OC1 Action Mask/Data Register
typedef struct  {
   unsigned int SP1   :3,   // Not used             
                D1    :1,   // 1=> if M1 set pin OC1 on OC1 match
                D2    :1,   // 1=> if M2 set pin OC2 on OC1 match
                D3    :1,   // 1=> if M3 set pin OC3 on OC1 match
                D4    :1,   // 1=> if M4 set pin OC4 on OC1 match
                D5    :1,   // 1=> if M5 set pin OC5 on OC1 match
                SP2   :3,   // Not Used
                M1    :1,   // 1=> pin OC1 is affected by OC1 match
                M2    :1,   // 1=> pin OC2 is affected by OC1 match
                M3    :1,   // 1=> pin OC3 is affected by OC1 match
                M4    :1,   // 1=> pin OC4 is affected by OC1 match
                M5    :1;   // 1=> pin OC5 is affected by OC1 match
                    } OC1_type;    
                    
                    
// pg 111
#define TCNT (*((unsigned short*)(0xFF90A))) // Timer Counter Register   


// pg 112
#define PACTL  (*((PACTL_type*)(0xFF90C))) // Pulse Accumulator Control Register
typedef struct  {
   unsigned int PACNT :8,   // Pulse Accumulator Counter           
                PACLK :2,   // Pulse Accumulator Clock Select
                I4O5  :1,   // 0=>OC5 Enabled 1=>IC4 Enabled
                PCLKS :1,   // PCLK Pin State
                PEDGE :1,   // Pulse Accumulator Edge Control
                PAMOD :1,   // Pulse Accumulator Mode
                PAEN  :1,   // Pulse Accumulator System Enable
                PAIS  :1;   // PAI Pin State 
                    } PACTL_type;   
                    
// pg 113
#define TIC1 (*((unsigned short*)(0xFF90E))) // Timer Input Capture Register 1
#define TIC2 (*((unsigned short*)(0xFF910))) // Timer Input Capture Register 2
#define TIC3 (*((unsigned short*)(0xFF912))) // Timer Input Capture Register 3

// pg 113
#define TOC1 (*((unsigned short*)(0xFF914))) // Timer Output Compare Register 1
#define TOC2 (*((unsigned short*)(0xFF916))) // Timer Output Compare Register 2
#define TOC3 (*((unsigned short*)(0xFF918))) // Timer Output Compare Register 3
#define TOC4 (*((unsigned short*)(0xFF91A))) // Timer Output Compare Register 4

// pg 113
#define TIC4OC5 (*((unsigned short*)(0xFF91C))) // Timer Inp Cap 4/Out Comp 5 Reg


// pg 113
#define TCTL12  (*((TCTL12_type*)(0xFF91E))) // Timers 1-2 Control Register
typedef struct  {
   unsigned int EDGE1 :2,   // Input Capture 1 Edge Control Bits        
                EDGE2 :2,   // Input Capture 2 Edge Control Bits   
                EDGE3 :2,   // Input Capture 3 Edge Control Bits   
                EDGE4 :2,   // Input Capture 4 Edge Control Bits   
                OML2  :2,   // Output Compare 2 Mode/Level Bits
                OML3  :2,   // Output Compare 3 Mode/Level Bits
                OML4  :2,   // Output Compare 4 Mode/Level Bits
                OML5  :2;   // Output Compare 5 Mode/Level Bits
                    } TCTL12_type;   
                    
// pg 114                    
#define TMSK12  (*((TMSK12_type*)(0xFF920))) // Timers 1-2 Interrupt Mask Register
typedef struct  {
   unsigned int CPR   :3,   // Timer Prescaler/PCLK Select Field     
                CPROUT:1,   // Compare/Capture Unit Clock Output Enable  
                PAII  :1,   // Pulse Accumulator Input Interrupt Enable
                PAOVI :1,   // Pulse Accumulator Overflow Interrupt Enable  
                SP1   :1,   // Not Used
                TOI   :1,   // Timer Overflow Interrupt Enable
                ICI1  :1,   // Input Capture 1 Interrupt Enable
                ICI2  :1,   // Input Capture 1 Interrupt Enable
                ICI3  :1,   // Input Capture 1 Interrupt Enable                  
                OCI1  :1,   // Output Compare 1 Interrupt Enable
                OCI2  :1,   // Output Compare 2 Interrupt Enable
                OCI3  :1,   // Output Compare 3 Interrupt Enable
                OCI4  :1,   // Output Compare 4 Interrupt Enable                
                I4O5IE:1;   // IC4/OC5 Interrupt Enable
                    } TMSK12_type;     
                    
                    
// pg 115
#define TFLG12  (*((TFLG12_type*)(0xFF922))) // Timers 1-2 Interrupt Flag Register
typedef struct  {
   unsigned int SP1   :3,   // Not Used 
                SP2   :1,   // Not Used 
                PAIF  :1,   // Pulse Accumulator Input Interrupt Flag
                PAOVF :1,   // Pulse Accumulator Overflow Interrupt Flag  
                SP3   :1,   // Not Used
                TOF   :1,   // Timer Overflow Interrupt Flag
                ICF1  :1,   // Input Capture 1 Interrupt Flag
                ICF2  :1,   // Input Capture 2 Interrupt Flag
                ICF3  :1,   // Input Capture 3 Interrupt Flag                
                OCF1  :1,   // Output Compare 1 Interrupt Flag
                OCF2  :1,   // Output Compare 2 Interrupt Flag
                OCF3  :1,   // Output Compare 3 Interrupt Flag
                OCF4  :1,   // Output Compare 4 Interrupt Flag
                I4O5IF:1;   // IC4/OC5 Interrupt Flag
                    } TFLG12_type;                                           


// pg 115
#define PWMCR  (*((PWMCR_type*)(0xFF924))) // PWM Control & Compare Force Register
typedef struct  {
   unsigned int F1B   :1,   // 0=>Force PWMB low, 1=> Force PWMB high
                F1A   :1,   // 0=>Force PWMA low, 1=> Force PWMA high
                SFB   :1,   // PWMB Slow/Fast Select 0=>Khz region
                SFA   :1,   // PWMA Slow/Fast Select 0=>Khz region
                PPR   :3,   // PWM Prescaler/PCLK Select
                PPROUT:1,   // 0=>Normal PWM output on PWMA
                FPWMB :1,   // 1=>Force PWMB output to F1B state
                FPWMA :1,   // 1=>Force PWMA output to F1A state
                SP1   :1,   // Not Used
                FOC1  :1,   // Force OC1 1=>causes pin action but OC flag not set 
                FOC2  :1,   // Force OC2 1=>causes pin action but OC flag not set 
                FOC3  :1,   // Force OC3 1=>causes pin action but OC flag not set 
                FOC4  :1,   // Force OC4 1=>causes pin action but OC flag not set 
                FOC5  :1;   // Force OC5 1=>causes pin action but OC flag not set
                    } PWMCR_type;  
                    
// pg 117
#define PWMA (*((unsigned char*)(0xFF926))) // PWM A Duty Cycle   

// pg 117
#define PWMB (*((unsigned char*)(0xFF927))) // PWM B Duty Cycle  

// pg 117
#define PWMCNT (*((unsigned short*)(0xFF928))) // PWM Count Register 

// pg 117
#define PWMBUFA (*((unsigned char*)(0xFF92A))) // PWM A Buffer Register     

// pg 117
#define PWMBUFB (*((unsigned char*)(0xFF92B))) // PWM B Buffer Register 

// pg 117
#define PRESCL (*((unsigned short*)(0xFF92C))) // GPT Prescaler 9-bits (0-511) 
