
// The Transmission Mode Variable defines what actions will occur  
#define SDOWN     0    // Shutdown.         Standard shutdown of controller board.   
#define NEUTRAL   1    // Neutral          
#define FORWARD   2    // Forward Drive            
#define REVERSE   3    // Reverse Drive
#define CALIBRATE 4    // Calibrate Mode


// The Fault Mode Variable limits or inhibits that action
#define PESTOP   0    // Perm. Emergency Stop.Kill Eng using Relay:No Restart Allowed
#define ESTOP    1    // Emergency Stop.   Kill Engine using Relay
#define ENEUT    2    // Emergency Neutral Disable Transmission using solenoids
#define DTHROT   3    // Disable Throttle Motor.
#define LIMPHOME 4    // Limp Home Mode
#define HEALTHY  5    // Healthy           No limits or inhibition to operation



