#ifndef PHIDGET_HELPER_FUNCTIONS_H
#define PHIDGET_HELPER_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <phidget22.h>

/* Determine if we are on Windows of Unix */
#if defined(_MSC_VER)
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

/**
* Reads user input and sets val to 1 if input Y or y, sets val to 0 if input N or n
* If def is not -1, and input is empty, sets val to def.
*
* @param val pointer to integer that will get set depending on the user input
* @return 0 if the operation succeeds, 1 if it fails
*/
int ProcessYesNo_Input(int* val, int def);

/**
* Displays the error string associated to the PhidgetRetrunCode from a phidget function call
*
* @param returnCode The PhidgetReturnCode to get the error string of
*/
void DisplayError(PhidgetReturnCode returnCode);

/**
* Sets the DeviceSerialNumber of the Phidget channel
*
* @param ph The Phidget channel to set
* @return 0 if the operation succeeds, 1 if it fails
*/
int SetSerialNumber(PhidgetHandle ph);

/**
* Sets the isHubPortDevice property of the Phidget channel
*
* @param ph The Phidget channel to set
* @return 0 if the operation succeeds, 1 if it fails
*/
int SetIsHubPortDevice(PhidgetHandle ph);

/**
* Sets the HubPort and isHubportDevice of the Phidget channel if it is a VINT device
*
* @param ph The Phidget channel to set
* @return 0 if the operation succeeds, 1 if it fails
*/
int SetVINTProperties(PhidgetHandle ph);

/**
* Sets the Channel value of the Phidget channel
*
* @param ph The Phidget channel to set
* @return 0 if the operation succeeds, 1 if it fails
*/
int SetChannel(PhidgetHandle ph);

/**
* Sets IsRemote for the Phidget Channel.
* If isRemote is true, ask the user for network info. Using either server discovery or manual server info
*
* @param ph The Phidget channel to set
* @return 0 if the operation succeeds, 1 if it fails
*/
int SetupNetwork(PhidgetHandle ph);

/**
* Enables server discovery for this program instance, using PHIDGETSERVER_DEVICEREMOTE as an arguement
*/
int EnableServerDiscovery();

/**
* Opens the phidget channel asyncronously. This function will not block.
*
* @param ph The Phidget channel to be opened
* @return 0 if the operation succeeds, 1 if it fails
*/
int OpenPhidgetChannel(PhidgetHandle ph);

/**
* Opens the phidget channel synchronously. This function will block for a duration of timeout_in_ms.
*
* @param ph The Phidget channel to be opened
* @param timeout_in_ms The number of milliseconds to wait for attach.
* @return 0 if the operation succeeds, 1 if it fails or times out
*/
int OpenPhidgetChannel_waitForAttach(PhidgetHandle ph, int timeout_in_ms);

/**
* Closes the phidget channel, releasing control of the object.
*
* @param ph The Phidget channel to be closed
* @return 0 if the operation succeeds, 1 if it fails
*/
int ClosePhidgetChannel(PhidgetHandle ph);

#endif
