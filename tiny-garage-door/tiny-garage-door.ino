/*
  TinyDuino WiFi TinyShield Example Sketch

  Just a basic tutorial showing you how to connect to WiFi with the Wifi
  TinyShield

  NOTE: There are a couple things you'll need to change for this to work!

  Written 29 May 2018
  By Laverena Wienclaw
  Modified 07 January 2019
  By Hunter Hykes

  https://TinyCircuits.com
*/

// This library is for the wifi connection
#include <WiFi101.h>
#include <Wire.h> //For I2C communication with sensor
#include "BMA250.h" //For interfacing with the accel.sensor
#include "tinyGarageDoor.h"

// Accelerometer sensor variables for the sensor and its values
BMA250 accel_sensor;
int x, y, z;
double temp;


#if defined(ARDUINO_ARCH_SAMD)
 #define SerialMonitorInterface SerialUSB
#else
 #define SerialMonitorInterface Serial
#endif

WiFiClient client;

char ssid[] = "padre";  //  your network SSID (name)
char wifiPassword[] = "fortknox";  // your network password
char server[] = "192.168.1.199";
int port = 3000;
int doorAxisValue;
int lastDoorAxisValue;
int percentOfFullTravel;
int doorMotion;
char * doorState;

void setup() {

  //WIFI Adapter setup
  SerialMonitorInterface.begin(BAUD_RATE_115200);
  WiFi.setPins(8, 2, A3, -1); // VERY IMPORTANT FOR TINYDUINO
  while(!SerialMonitorInterface);

  // Attempt to connect to Wifi network:
  SerialMonitorInterface.print("Connecting Wifi: ");
  SerialMonitorInterface.println(ssid);

  // Connect to WiFi, and loop until connection is secured
  WiFi.begin(ssid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED)
    SerialMonitorInterface.print(".");
    delay(500);

  // Print out the local IP address
  SerialMonitorInterface.println("");
  SerialMonitorInterface.println("WiFi connected");
  SerialMonitorInterface.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  SerialMonitorInterface.println(ip);

  //setup for accelerometer
  Wire.begin();

  SerialMonitorInterface.print("Initializing BMA...");
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); 
}

void loop()
{
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
    
    char data[200];
    
    
    SerialMonitorInterface.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect(server, port)) {
      SerialMonitorInterface.println("\nconnected");

      
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

  delay(500);  
        
  sprintf(data, "{\"id\":\"testid\",\"Lisa\":\"Garage Door\",\"State\":\"%s\"}",doorState);
  SerialMonitorInterface.println(data);
  
  client.println("POST /devices HTTP/1.1");
  client.println("content-type: application/json");
  client.print("content-length: ");
  client.println(strlen(data));
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();
  client.println(data);
      
  delay(2000);
      
  }
  else
    SerialMonitorInterface.println("not connected");
    
  }

  // The BMA250 can only poll new sensor values every 64ms, so this delay
  // will ensure that we can continue to read values
  delay(250);
  // ***Without the delay, there would not be any sensor output*** 

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    SerialMonitorInterface.println();
    SerialMonitorInterface.println("disconnecting from server.");
    client.stop();
  }

  
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
