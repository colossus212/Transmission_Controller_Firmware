
#define EnableInterrupts   {asm ANDP #0xFF1F;}
#define DisableInterrupts  {asm ORP  #0x00E0;}  



#define TUPID_IND     0  // Throttle-Up PID
#define NUM_PID       1  // Number of PID Loops

           

                 
  // this structure is very nice for parsing ints into their low and high bytes
  typedef struct {
   unsigned int low   :8,  // low byte of int
                high  :8;  // high byte of int
                 } byte_type;       

  typedef union      {   
       unsigned int  whole; // this allows programmer access to entire integer
       byte_type     byte;  // or each byte of it       
           } fracInt;   // fractured Int type
