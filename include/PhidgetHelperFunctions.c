#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phidget22.h"
#include "PhidgetHelperFunctions.h"


int ProcessYesNo_Input(int* val, int def) {
	char strvar[100];

	if (fgets(strvar, 100, stdin) == NULL)
		return 1;

	if (strvar[0] == '\n') {
		if (def == -1)
			return 1;
		*val = def;
		return 0;
	}

	if (strvar[0] == 'N' || strvar[0] == 'n') {
		*val = 0;
		return 0;
	}

	if (strvar[0] == 'Y' || strvar[0] == 'y') {
		*val = 1;
		return 0;
	}

	return 1;
}

void DisplayError(PhidgetReturnCode returnCode) {
	PhidgetReturnCode prc;
	const char* error;

	prc = Phidget_getErrorDescription(returnCode, &error);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Getting ErrorDescription: \n\t");
		DisplayError(prc);
		return;
	}

	fprintf(stderr, "Desc: %s\n", error);
}

int SetSerialNumber(PhidgetHandle ph) {
	int deviceSerialNumber = 0;
	PhidgetReturnCode prc;
	char strvar[100];

	printf("\nFor all questions, enter the value, or press ENTER to select the [Default]\n");

	printf("\n--------------------------------------\n");
	printf("\n  | Some Phidgets have a unique serial number, printed on a white label on the device.\n"
	  "  | For Phidgets and other devices plugged into a VINT Port, use the serial number of the VINT Hub.\n"
	  "  | Specify the serial number to ensure you are only opening channels from that specific device.\n"
	  "  | Otherwise, use -1 to open a channel on any device.\n");
	while (1) {
		printf("\nWhat is the Serial Number? [-1] ");
		if (fgets(strvar, 100, stdin) == NULL)
			continue;
		if (strvar[0] == '\n') {
			deviceSerialNumber = PHIDGET_SERIALNUMBER_ANY;
			break;
		}
		deviceSerialNumber = atoi(strvar);
		if (deviceSerialNumber >= -1 && deviceSerialNumber != 0)
			break;
	}

	prc = Phidget_setDeviceSerialNumber(ph, deviceSerialNumber);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Setting DeviceSerialNumber: \n\t");
		DisplayError(prc);
		return 1;
	}
	return 0;
}

int SetIsHubPortDevice(PhidgetHandle ph) {
	int isHubPortDevice = 0;
	PhidgetReturnCode prc;

	while (1) {
		printf("\nIs this a \"HubPortDevice\"? [y/n] ");
		if (!ProcessYesNo_Input(&isHubPortDevice, -1))
			break;
	}

	prc = Phidget_setIsHubPortDevice(ph, isHubPortDevice);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Setting isHubPortDevice: \n\t");
		DisplayError(prc);
		return 1;
	}
	return 0;
}

int SetVINTProperties(PhidgetHandle ph) {
	int canBeHubPortDevice = 0;
	Phidget_ChannelClass pcc;
	PhidgetReturnCode prc;
	char strvar[100];
	int hubPort = -1;
	int isVINT = 0;

	printf("\n--------------------------------------\n");

	while (1) {
		printf("\nDo you want to specify the hub port that your device is plugged into?\n"
			"Choose No if your device is not plugged into a VINT Hub. (y/n) ");
		if (!ProcessYesNo_Input(&isVINT, -1))
			break;
	}

	//Don't ask about the HubPort and the HubPortDevice if it's not a VINT device
	if (!isVINT)
		return 0;

	printf("\n--------------------------------------\n");
	printf("\n  | VINT Hubs have numbered ports that can be uniquely addressed.\n"
		"  | The HubPort# is identified by the number above the port it is plugged into.\n"
		"  | Specify the hub port to ensure you are only opening channels from that specific port.\n"
		"  | Otherwise, use -1 to open a channel on any port.\n");
	while (1) {
		printf("\nWhat HubPort is the device plugged into? [-1] ");
		if (fgets(strvar, 100, stdin) == NULL)
			continue;
		if (strvar[0] == '\n') {
			hubPort = PHIDGET_HUBPORT_ANY;
			break;
		}
		hubPort = atoi(strvar);
		if (hubPort >= -1 && hubPort <= 5)
			break;
	}

	prc = Phidget_setHubPort(ph, hubPort);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Setting HubPort: \n\t");
		DisplayError(prc);
		return 1;
	}

	prc = Phidget_getChannelClass(ph, &pcc);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Getting ChannelClass: \n\t");
		DisplayError(prc);
		return 1;
	}

	switch (pcc) {
	case PHIDCHCLASS_VOLTAGEINPUT:
		printf("\n--------------------------------------\n");
		printf("\n  | A VoltageInput HubPortDevice uses the VINT Hub's internal channel to measure the voltage on the white wire.\n"
		  "  | If the device you are trying to interface returns an analog voltage between 0V-5V, open it as a HubPortDevice.\n");
		canBeHubPortDevice = 1;
		break;
	case PHIDCHCLASS_VOLTAGERATIOINPUT:
		printf("\n--------------------------------------\n");
		printf("\n  | A VoltageRatioInput HubPortDevice uses the VINT Hub's internal channel to measure the voltage ratio on the white wire.\n"
		  "  | If the device you are trying to interface returns an ratiometric voltage between 0V-5V, open it as a HubPortDevice.\n");
		canBeHubPortDevice = 1;
		break;
	case PHIDCHCLASS_DIGITALINPUT:
		printf("\n--------------------------------------\n");
		printf("\n  | A DigitalInput HubPortDevice uses the VINT Hub's internal channel to detect digital changes on the white wire.\n"
		  "  | If the device you are trying to interface returns a 5V or 3.3V digital signal, open it as a HubPortDevice.\n");
		canBeHubPortDevice = 1;
		break;
	case PHIDCHCLASS_DIGITALOUTPUT:
		printf("\n--------------------------------------\n");
		printf("\n  | A DigitalOutput HubPortDevice uses the VINT Hub's internal channel to output a 3.3V digital signal on the white wire.\n"
		  "  | If the device you are trying to interface accepts a 3.3V digital signal, open it as a HubPortDevice.\n");
		canBeHubPortDevice = 1;
		break;
	default:
		break;
	}

	if (canBeHubPortDevice)
		SetIsHubPortDevice(ph);

	return 0;
}

int SetChannel(PhidgetHandle ph) {
	int isHubPortDevice = 0;
	PhidgetReturnCode prc;
	char strvar[100];
	int Channel = 0;

	prc = Phidget_getIsHubPortDevice(ph, &isHubPortDevice);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Getting isHubPortDevice: \n\t");
		DisplayError(prc);
		return 1;
	}

	// Hub port devices only have a single channel, so don't ask for the channel
	if (isHubPortDevice)
		return 0;

	printf("\n--------------------------------------\n");
	printf("\n  | Devices with multiple inputs or outputs of the same type will map them to channels.\n"
	  "  | The API tab for the device on www.phidgets.com shows the channel breakdown.\n"
	  "  | For example, a device with 4 DigitalInputs would use channels [0 - 3]\n"
	  "  | A device with 1 VoltageInput would use channel 0\n");
	while (1) {
		printf("\nWhat channel# of the device do you want to open? [0] ");
		if (fgets(strvar, 100, stdin) == NULL)
			continue;
		if (strvar[0] == '\n') {
			Channel = 0;
			break;
		}
		Channel = atoi(strvar);
		if (Channel >= 0)
			break;
	}

	prc = Phidget_setChannel(ph, Channel);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Setting Channel: \n\t");
		DisplayError(prc);
		return 1;
	}

	return 0;
}

int EnableServerDiscovery() {
	PhidgetReturnCode prc;

	printf("Enabling Server Discovery...\n");
	prc = PhidgetNet_enableServerDiscovery(PHIDGETSERVER_DEVICEREMOTE);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Enable Server Discovery: \n\t");
		DisplayError(prc);
		return 1;
	}
	return 0;
}

int SetupNetwork(PhidgetHandle ph) {
	PhidgetReturnCode prc;
	char hostname[100];
	char password[100];
	char strvar[100];
	int discovery = 0;
	int isRemote = 0;
	char *pos;
	int port;

	printf("\n--------------------------------------\n");
	printf("\n  | Devices can either be opened directly, or over the network.\n"
	  "  | In order to open over the network, the target system must be running a Phidget Server.\n");
	while (1) {
		printf("\nIs this device being opened over the network? [y/N] ");
		if (!ProcessYesNo_Input(&isRemote, 0))
			break;
	}

	prc = Phidget_setIsRemote(ph, isRemote);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> Set IsRemote: \n\t");
		DisplayError(prc);
		return 1;
	}

	// if it's not remote, don't need to ask about the network
	if (!isRemote)
		return 0;

	printf("\n--------------------------------------\n");
	printf("\n  | Server discovery enables the dynamic discovery of phidget servers that publish their identity to the network.\n"
		  "  | This allows you to open devices over the network without specifying the hostname and port of the server.\n");
	while (1) {
		printf("\nDo you want to enable server discovery? [Y/n] ");
		if (!ProcessYesNo_Input(&discovery, 1))
			break;
	}

	if (discovery)
		return EnableServerDiscovery();

	printf("\n--------------------------------------\n");
	printf("\nPlease provide the following information in order to open the device\n");

	while (1) {
		printf("\nWhat is the Hostname (or IP Address) of the server? [localhost] ");
		if (fgets(hostname, 100, stdin) == NULL)
			continue;
		if (strvar[0] == '\n') {
			snprintf(hostname, 100, "localhost");
			break;
		}
		// Remove trailing newline
		if ((pos=strchr(hostname, '\n')) != NULL) {
			*pos = '\0';
			break;
		}
	}

	printf("\n--------------------------------------\n");
	while (1) {
		printf("\nWhat port is the server on? [5661] ");
		if (fgets(strvar, 100, stdin) == NULL)
			continue;
		if (strvar[0] == '\n') {
			port = 5661;
			break;
		}
		port = atoi(strvar);
		if (port <= 65535 && port > 0)
			break;
	}
	printf("\n--------------------------------------\n");
	while (1) {
		printf("\nWhat is the password of the server? [] ");
		if (fgets(password, 100, stdin) == NULL)
			continue;
		// Remove trailing newline
		if ((pos=strchr(password, '\n')) != NULL) {
			*pos = '\0';
			break;
		}
	}
	printf("\n--------------------------------------\n");

	printf("Adding Server...\n");
	prc = PhidgetNet_addServer("Server", hostname, port, password, 0);
	if (EPHIDGET_OK != prc) {
		fprintf(stderr, "Runtime Error -> AddServer: \n\t");
		DisplayError(prc);
		return 1;
	}

	return 0;
}

int OpenPhidgetChannel(PhidgetHandle ph) {
	PhidgetReturnCode prc;

	printf("Opening Channel...\n");
	prc = Phidget_open(ph);
	if (prc != EPHIDGET_OK) {
		fprintf(stderr, "Runtime Error -> Opening Phidget Channel: \n\t");
		DisplayError(prc);
		return 1;
	}
	return 0;
}

int OpenPhidgetChannel_waitForAttach(PhidgetHandle ph, int timeout_in_ms) {
	PhidgetReturnCode prc;

	printf("Opening Channel...\n");
	prc = Phidget_openWaitForAttachment(ph, timeout_in_ms);
	if (prc != EPHIDGET_OK) {
		fprintf(stderr, "Runtime Error -> Opening Phidget Channel: \n\t");
		DisplayError(prc);
		return 1;
	}

	return 0;
}

int ClosePhidgetChannel(PhidgetHandle ph) {
	PhidgetReturnCode prc;

	printf("Closing Channel...\n");
	prc = Phidget_close(ph);
	if (prc != EPHIDGET_OK) {
		fprintf(stderr, "Runtime Error -> Closing Phidget Channel: \n\t");
		DisplayError(prc);
		return 1;
	}
	return 0;
}
