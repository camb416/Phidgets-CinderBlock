/*
Adapted from example available here: https://www.phidgets.com/?view=code_samples&lang=C
*/

#pragma once

#define __stdcall
#define __cdecl
#define __attribute__ ((dllimport))
#define __declspec(dllimport)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"

#include <phidget22.h>
#include <memory>

#pragma clang diagnostic pop

namespace po
{
	namespace phidget
	{
		class VoltageRatioInput;
		typedef std::shared_ptr<VoltageRatioInput> VoltageRatioInputRef;

		class VoltageRatioInputDelegate;
		typedef std::weak_ptr<VoltageRatioInputDelegate> VoltageRatioInputDelegateRef;


		class VoltageRatioInputDelegate
		{
			public:
				virtual void voltageRatioValueChanged( double ratio ) = 0;
		};

		class VoltageRatioInput
			: public std::enable_shared_from_this<VoltageRatioInput>
		{
			public:
				static VoltageRatioInputRef create();
				static VoltageRatioInputRef create( int serialNum, int channelNum, int dataInterval, double changeTrigger );

				void setDelegate( VoltageRatioInputDelegateRef delegate ) { mDelegate = delegate; };

				double getSensorVal();
				~VoltageRatioInput();

			protected:
				VoltageRatioInput();
				void setup( int serialNum = -1, int channelNum = 0, int dataInterval = 100, double changeTrigger = 0.05 );

			private:
				PhidgetVoltageRatioInputHandle mHandle = NULL;

				//	called during setup
				static int createVoltageRatioInput( PhidgetVoltageRatioInputHandle* pvrih );
				int setSerialNumber( PhidgetHandle ph, int deviceSerialNumber = -1 );
				int setChannel( PhidgetHandle ph, int channel = 0 );

				uint32_t mDataInterval;
				double mChangeTrigger;

				int setAttachDetachError_Handlers( PhidgetHandle ch );
				int setVoltageRatioHandler( PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr );
				int openPhidgetChannelWithTimeout( PhidgetHandle ch, int timeout = 5000 );
				int closePhidgetChannel( PhidgetHandle ch );

				//	event handlers
				static void CCONV onAttachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onDetachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onErrorHandler( PhidgetHandle phid, void* ctx, Phidget_ErrorEventCode errorCode, const char* errorString );
				static void CCONV onVoltageRatioChangeHandler( PhidgetVoltageRatioInputHandle pvrih, void* ctx, double ratio );

				//	error display
				static void displayError( PhidgetReturnCode code );

				//  communication
				VoltageRatioInputDelegateRef mDelegate;
				void testCallbackFunction( double test );
		};
	}
}
