



/*************************************************************************
   Tiny Garage Door
   Door tilt indicator using the Tiny BMA250 shield

 ************************************************************************/

#include <Wire.h>         // For I2C communication with sensor
#include "BMA250.h"       // For interfacing with the accel. sensor
#include "tinyGarageDoor.h"

// Accelerometer sensor variables for the sensor and its values
BMA250 accel_sensor;
int x, y, z;
double temp;
int doorAxisValue;
int lastDoorAxisValue;
int percentOfFullTravel;
int doorMotion;
String doorState;

#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif

/**
   Set up the IO and serial port
*/
void setup() {
  SerialMonitorInterface.begin(BAUD_RATE_115200);
  Wire.begin();
  SerialMonitorInterface.print(F("Tiny-Garage Version: "));
  SerialMonitorInterface.println(F(VERSION));
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  SerialMonitorInterface.print("Initializing BMA...");
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms);
}

void loop() {
  accel_sensor.read();//This function gets new data from the acccelerometer

  // Get the acceleration values from the sensor and store them into global variables
  // (Makes reading the rest of the program easier)
  x = accel_sensor.X;
  y = accel_sensor.Y;
  z = accel_sensor.Z;
  temp = ((accel_sensor.rawTemp * 0.5) + 24.0);

  // If the BMA250 is not found, nor connected correctly, these values will be produced
  // by the sensor
  if (x == -1 && y == -1 && z == -1) {
    // Print error message to Serial Monitor
    SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
  }

  else { // if we have correct sensor readings:
    showSerial();                 //Print to Serial Monitor or Plotter
  }


  // Report the accelerometer axis as the door position

  SerialMonitorInterface.print("Door: ");

  // Check the door state on the axis
  doorAxisValue = accel_sensor.DOOR_TILT_AXIS * BOARD_ORIENTATION;


  if (doorAxisValue > CLOSED_UPPER_THRESHOLD && doorAxisValue < OPEN_LOWER_THRESHOLD) {
    // In the middle
    doorMotion = doorAxisValue - lastDoorAxisValue; 
    if ( doorMotion> 0) {
      percentOfFullTravel = FULL_TRAVEL / (OPEN_LOWER_THRESHOLD - doorAxisValue);
      doorState = OPENING_STATE;
    } else {
      percentOfFullTravel =  FULL_TRAVEL / (doorAxisValue - CLOSED_UPPER_THRESHOLD);

      doorState = CLOSING_STATE;
    }
    
     SerialMonitorInterface.print(doorState);
     SerialMonitorInterface.print("(");
     SerialMonitorInterface.print(percentOfFullTravel);
     SerialMonitorInterface.println("%)");

  } else {
    if (doorAxisValue >= OPEN_LOWER_THRESHOLD) {
      // Fully open
      doorState = OPEN_STATE;
    }

    if (doorAxisValue <= CLOSED_UPPER_THRESHOLD) {
      // Fully closed
      doorState = CLOSED_STATE;

    }
    SerialMonitorInterface.println(doorState);
  }

  // Save the last
  lastDoorAxisValue = doorAxisValue;

  // Just a flash to indicate activite

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(250);
  // ***Without the delay, there would not be any sensor output***
}

// Prints the sensor values to the Serial Monitor, or Serial Plotter (found under 'Tools')
void showSerial() {
  SerialMonitorInterface.print("X = ");
  SerialMonitorInterface.print(x);

  SerialMonitorInterface.print("  Y = ");
  SerialMonitorInterface.print(y);

  SerialMonitorInterface.print("  Z = ");
  SerialMonitorInterface.print(z);

  SerialMonitorInterface.print("  Temperature(C) = ");
  SerialMonitorInterface.println(temp);
}
