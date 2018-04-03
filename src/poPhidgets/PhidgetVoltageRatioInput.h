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
				~VoltageRatioInput();

			protected:
				VoltageRatioInput();
				void setup();

			private:
				PhidgetVoltageRatioInputHandle mHandle;

				static int createVoltageRatioInput( PhidgetVoltageRatioInputHandle* pvrih );
				int setSerialNumber( PhidgetHandle ph, int deviceSerialNumber = -1 );
				int setChannel( PhidgetHandle ph, int channel = 0 );
				int setAttachDetachError_Handlers( PhidgetHandle ch );
				//int setVoltageRatioChangeHandler( PhidgetVoltageRatioInputHandle pvrih );

				static void CCONV onAttachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onDetachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onErrorHandler( PhidgetHandle phid, void* ctx, Phidget_ErrorEventCode errorCode, const char* errorString );
				static void CCONV onVoltageRatioChangeHandler( PhidgetVoltageRatioInputHandle pvrih, void* ctx, double ratio );
				static int setVoltageRatioHandler( PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr );
		};
	}
}