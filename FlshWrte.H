// Variables to hold copies of parameters defined in FIELD_CAL_SPACE before erasing sector 
unsigned short CalRPMSetpoint[102]; // No Load Curve
unsigned short CalLowIdle;    // remember where low idle is 
unsigned short CalHighIdle;   // remember where high idle is 
unsigned short CalMinPedal;   // value of A/D when throttle pedal is fully up
unsigned short CalMaxPedal;   // value of A/D when throttle pedal is floored
unsigned short CalMinHoist;   // value of A/D when Hoist Lever is released
unsigned short CalMaxHoist;   // value of A/D when Hoist Lever is fully back
unsigned short CalMinRack;    // value of A/D when Rack is at Idle
unsigned short CalMaxRack;    // value of A/D when Rack is at HighIdle  

unsigned short CalfilteredRPM_SP_IncConst; // remember filteredRPM_SP ramp rate

unsigned short CalpaKpConst;  // remember proportional power-adapt gain
unsigned short CalpaKiConst;  // remember integal power-adapt gain

unsigned short CaltKpConst;          // remember proportional throttle-up gain  
unsigned short CaltKiConst;          // remember integral throttle-up gain    
unsigned short CaltKFFConst;         // remember feed forward throttle-up gain     
unsigned short CalTiltSPConst;       // remember throttle-up tilt setpoint
unsigned short CalThrottle_SPIncConst; // remember throttle-up setpoint ramp rate


unsigned short ManVERSIONH;
unsigned short ManVERSIONL;
unsigned short ManMODELH;
unsigned short ManMODELL;
unsigned short ManCNFIGH;
unsigned short ManCNFIGL;
unsigned short ManVN;
unsigned short ManSN;
unsigned short ManMONTH;
unsigned short ManDAY;
unsigned short ManYEAR;


#define TESTSTATUS (*((FlashStatus_type*)(0x2100A))) // Read Status of EPROM during programming
typedef struct  {
   unsigned int Low_Unused0   :1,   // Unused
                Low_Unused1   :1,   // Unused
                Low_Unused2   :1,   // Unused
                Low_DQ3       :1,   // DQ3
                Low_Unused4   :1,   // Unused
                Low_DQ5       :1,   // DQ5
                Low_DQ6       :1,   // DQ6
                Low_DQ7       :1,   // DQ7
                Hi_Unused0    :1,   // Unused
                Hi_Unused1    :1,   // Unused
                Hi_Unused2    :1,   // Unused
                Hi_DQ3        :1,   // DQ3
                Hi_Unused4    :1,   // Unused
                Hi_DQ5        :1,   // DQ5
                Hi_DQ6        :1,   // DQ6
                Hi_DQ7        :1;   // DQ7
                    } FlashStatus_type;  
                    
                    
// The following constants are used in JSR instructions within the code which
// will be trasferred to and run from RAM.  These constants must match
// those used in the .PRM file to keep everything synced up
#define FLASH_CODE_RAM_LOC  0x41000   // Address of 1st Flash routine in RAM
#define FLASH_CODE_ROM_LOC  0xF000   // Address of 1st Flash routine in ROM
#define FLASH_CODE_LENGTH   0x5FF     // Length of all Flash routines in RAM


#define FLASH_ERASE_RAM_LOC 0x41000   // Matches FLASH_ERASE_RAM in .PRM file
#define FLASH_ERASE_ROM_LOC 0xF000    // Matches FLASH_ERASE_ROM in .PRM file

#define FLASH_WRITE_RAM_LOC 0x41300   // Matches FLASH_WRITE_RAM in .PRM file  
#define FLASH_WRITE_ROM_LOC 0xF300    // Matches FLASH_WRITE_ROM in .PRM file

static unsigned long  EraseSector;       // Passes which sector to erase to EraseSector
static unsigned long  FlashWriteAddress; // Passes which address to write to in Flash
static unsigned short FlashWriteValue;   // Passes value to write to Flash   

static unsigned char FlashPrograming;    // flag to check if EPROM is still programming
static unsigned char FlashOpErr;         // flag to check if EPROM operation was successful  

// The 1st command register in the AMD Am29F010 Flash EPROM is at 0x5555
// The 2nd command register in the AMD Am29F010 Flash EPROM is at 0x2AAA
// however, we have mapped our A1 address line to A0 on the EPROM, our
// A2 address line to A1 on the EPROM, and so on, so to generate the proper
// patterns for the EPROM, we must shift the 0x5555 one bit to the left to 0xAAAA
// and similarly shift the 0x2AAA one bit to the left to 0x5554.  This will
// fool the EPROM into thinking we're giving it the proper addresses

#define FLASHCOMMAND1 (*((unsigned short*)(0xAAAA))) // 1st Command Register of Flash
#define FLASHCOMMAND2 (*((unsigned short*)(0x5554))) // 2nd Command Register of Flash
#define FLASHCOMMAND3 (*((unsigned short*)(0xAAAA))) // 3rd Command Register of Flash
#define FLASHCOMMAND4 (*((unsigned short*)(0xAAAA))) // 4th Command Register of Flash
#define FLASHCOMMAND5 (*((unsigned short*)(0x5554))) // 5th Command Register of Flash
#define FLASHCOMMAND6 (*((unsigned short*)(0xAAAA))) // 6th Command Register of Flash
                    
