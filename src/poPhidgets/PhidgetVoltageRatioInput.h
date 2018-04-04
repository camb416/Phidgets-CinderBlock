/*
Adapted from example available here: https://www.phidgets.com/?view=code_samples&lang=C
*/

#pragma once

#include <memory>
#include <phidget22.h>

extern "C" {
#include "PhidgetHelperFunctions.h"
}

namespace po
{
	namespace phidget
	{
		class VoltageRatioInput;
		typedef std::shared_ptr<VoltageRatioInput> VoltageRatioInputRef;

		class VoltageRatioInput
		{
			public:
				static VoltageRatioInputRef create();
				static VoltageRatioInputRef create( int serialNum, int channelNum );
				~VoltageRatioInput();

				double getSensorVal();

			protected:
				VoltageRatioInput();
				void setup( int serialNum = -1, int channelNum = 0 );

			private:
				PhidgetVoltageRatioInputHandle mHandle;

				//	called during setup
				static int createVoltageRatioInput( PhidgetVoltageRatioInputHandle* pvrih );
				int setSerialNumber( PhidgetHandle ph, int deviceSerialNumber = -1 );
				int setChannel( PhidgetHandle ph, int channel = 0 );
				int setAttachDetachError_Handlers( PhidgetHandle ch );
				static int setVoltageRatioHandler( PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr );
				int openPhidgetChannelWithTimeout( PhidgetHandle ch, int timeout = 5000 );
				int closePhidgetChannel( PhidgetHandle ch );

				//	event handlers
				static void CCONV onAttachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onDetachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onErrorHandler( PhidgetHandle phid, void* ctx, Phidget_ErrorEventCode errorCode, const char* errorString );
				static void CCONV onVoltageRatioChangeHandler( PhidgetVoltageRatioInputHandle pvrih, void* ctx, double ratio );
		};
	}
}