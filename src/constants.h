#define WRT 2
#define REC 5
#define AD7 6
#define AD4 7
#define AD3 8
#define AD2 9
#define AD1 10
#define AD0 11
#define STO 12
#define LED 13
#define IO1 14
#define IO2 15
#define IO3 16
#define IO4 17
#define AD5 18
#define AD6 19

#define MAX_ADDR    255
#define ADD_PIN_CNT 8
#define IO_PIN_CNT  4

/* Defines other constants used in the program */
#define DLY       1           //EEPROM write and store delay time
#define MAXCHARS  16          //for character string input
#define IF        45000000    //radio IF in Hz
#define VLO       136000000   //radio VHF bottom band limit in Hz
#define VHI       174000000   //radio VHF top band limit in Hz
#define VSTEP     5000        //radio VHF channel step
#define ULO       403000000   //radio UHF bottom band limit in Hz
#define UHI       512000000   //radio UHF top band limit in Hz
#define USTEP     12500       //radio UHF channel step
#define MAXCHAN   7           //8 channels in the mode
#define TOT       1           //time-out timer (m:ss): 0=0:30, 1=1:00, 2=1:30, 3=2:00, etc.