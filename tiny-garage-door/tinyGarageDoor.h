#ifndef TINY_GARAGE_DOOR

#define VERSION "0.1.0"
#define BAUD_RATE_115200 (115200)

// Axis to use
#define DOOR_TILT_AXIS Z

// Threshold for OPEN
#define OPEN_LOWER_THRESHOLD 250
//Threshold for CLOSED
#define CLOSED_UPPER_THRESHOLD 40
#define FULL_TRAVEL (OPEN_LOWER_THRESHOLD-CLOSED_UPPER_THRESHOLD)

// Orientation of the board - use -1 or 1 to flip the readings to positive 
#define BOARD_ORIENTATION -1

// States for reporting
#define OPEN_STATE "OPEN"
#define CLOSED_STATE "CLOSED"
#define CLOSING_STATE "CLOSING"
#define OPENING_STATE "OPENING"

#endif
