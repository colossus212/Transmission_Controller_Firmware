static unsigned char PedalPotCal[350];     // Pedal Pot reading indexed with RPM/10
static unsigned char RackPotCal[350];      // Rack Pot reading indexed with RPM/10
static unsigned short NoLoadCalArray[102]; // No Load RPM curve we are generating

static unsigned char  ThrottleCalFlag; // flags that a throttle system cal is active
static unsigned char  HoistCalFlag;    // flags that a hoist lever cal is active
static unsigned char  ThrottleCalOK;   // flags that brake has been pressed over 5 secs and released

static unsigned char  LilHoist;     // Min Hoist Pot Cal Value
static unsigned char  BigHoist;     // Max Hoist Pot Cal Value




