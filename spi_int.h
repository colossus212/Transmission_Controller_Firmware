
#define EEPROM_WREN  6   // Set Write Enable Latch
#define EEPROM_WRDI  4   // Reset Write Enable Latch
#define EEPROM_RDSR  5   // Read Status Register
#define EEPROM_WRSR  1   // Write Status Register
#define EEPROM_READ  3   // Read Data from Memory Array
#define EEPROM_WRITE 2   // Write Data to Memory Array

#define SPI_CR_CONT 0x80   // SPI Command Register to keep CS low for next command    **** was 80
#define SPI_CR_LAST 0x00   // SPI Command Register to allow CS high after command     **** was 00

#define SPI_GETBYTE  1 // SPI Get Byte Command
#define SPI_SETBYTE  2 // SPI Set Byte Command
#define SPI_RSTATUS  3 // SPI Read Status Register Command
#define SPI_WSTATUS  4 // SPI Write Status Register Command
#define SPI_WENABLE  5 // SPI Write Enable Command
#define SPI_WDISABLE 6 // SPI Write Disable Command

#define SPI_GETEVENT 7 // SPI Get Event Command
#define SPI_SETEVENT 8 // SPI Set Event Command

#define SPI_READVAL_OP    1 // SPI Read Value Operation
#define SPI_STOREVAL_OP   2 // SPI Store Value Operation
#define SPI_READEVENT_OP  3 // SPI Read Event Operation
#define SPI_STOREEVENT_OP 4 // SPI Store Event Operation




// This struct is used to ref bits of the Atmel AT25040 Serial EEPROM Status Reg
typedef struct  {
   unsigned int BSY  :1,  // 1=>Not ready.  Write Cycle is in progress
                WEN  :1,  // 1=>Write is Enabled   
                BP   :2,  // 0=>No Prot:1=>180-1FF Prot:2=>100-1FF Prot: 3=>All Prot
                SP   :4;  // Spare, not used 
                } EEPROMStatus_type;  
                
typedef union      {   
         unsigned char       whole;  // this allows access to entire byte
         EEPROMStatus_type   bit;   // or each bit of it
           } fracEEPROMStatus;   // fractured EEPROMStatus_type                                 
                 


// Variables used by SPI Communications Link to Atmel AT25040 Serial EEPROM
static unsigned char SPI_Command;  // Command currently being sent to EEPROM
static unsigned char SPI_LastCmd;  // Previous command sent to EEPROM 
static unsigned char SPI_CommandPending;  // Flag tells if waiting for response
static unsigned char SPI_Operation;   // Flag tells if doing an SPI operation
static unsigned char StoreValueStep;  // used to remember progress in StoreValue
static unsigned char ReadValueStep;   // used to remember progress in ReadValue

static fracEEPROMStatus  EEPROMStatus;  // EEPROM Status Reg.


static unsigned char StoredEvent0;   // flag used to store Event Data to EEPROM
static unsigned char StoringEvent0;  // flag used to store Event Data to EEPROM
     
static unsigned char StoredEvent1;   // flag used to store Event Data to EEPROM
static unsigned char StoringEvent1;  // flag used to store Event Data to EEPROM
    
static unsigned char StoredEvent2;   // flag used to store Event Data to EEPROM
static unsigned char StoringEvent2;  // flag used to store Event Data to EEPROM
                                     
static unsigned char StoredEvent3;   // flag used to store Event Data to EEPROM
static unsigned char StoringEvent3;  // flag used to store Event Data to EEPROM
     
static unsigned char StoredEvent4;   // flag used to store Event Data to EEPROM
static unsigned char StoringEvent4;  // flag used to store Event Data to EEPROM

