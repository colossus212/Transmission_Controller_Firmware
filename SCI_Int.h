
#define SCI_GETBYTE  1 // SCI Get Byte Command
#define SCI_SETBYTE  2 // SCI Set Byte Command
#define SCI_RSTATUS  3 // SCI Read Status Register Command
#define SCI_WSTATUS  4 // SCI Write Status Register Command
#define SCI_WENABLE  5 // SCI Write Enable Command
#define SCI_WDISABLE 6 // SCI Write Disable Command

#define SCI_GETEVENT 7 // SCI Get Event Command
#define SCI_SETEVENT 8 // SCI Set Event Command

#define SCI_READVAL_OP    1 // SCI Read Value Operation
#define SCI_STOREVAL_OP   2 // SCI Store Value Operation
#define SCI_READEVENT_OP  3 // SCI Read Event Operation
#define SCI_STOREEVENT_OP 4 // SCI Store Event Operation





// Variables used by SCI Communications Link 
static unsigned char testchar;

static unsigned char ManInfoByteCounter; // used when parsing stream of manufacture information

static unsigned char SCICommand;  // Command from SCI port to controller

static unsigned char SCI_InParam;    // Read parameter character from input buffer
static unsigned char SCI_InParam2;   // Read 2nd parameter character from input buffer
static unsigned char SCI_InParamRead; // Remember that we've already read SCI_InParam



static fracInt SCI_Out_Int; // Used to parse an integer to be sent out SCI port
static fracInt SCI_In_Int;  // Used to parse an integer just received from SCI port


static unsigned char SCIWriteIntOK;   // Flag to signal OK to send SCI_Out_Int


static unsigned char SCI_Out_Err; // Flag errors in transmitting from SCI port
static unsigned char SCI_In_Err;  // Flag errors in receiving from SCI port


static unsigned char EventSendCounter;  // Counts through Events while sending to serial port

static unsigned char ManualOK1;   // Monitoring program must send a specific value here to enable Manual
static unsigned char ManualOK2;   // Monitoring program must send a specific value here to enable Manual

