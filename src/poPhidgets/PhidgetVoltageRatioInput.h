/*
Adapted from example available here: https://www.phidgets.com/?view=code_samples&lang=C
*/

#pragma once

#include <memory>
#include <phidget22.h>

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
            virtual void voltageRatioValueChanged(double ratio) = 0;
        };
        
		class VoltageRatioInput
		{
			public:
            typedef void(VoltageRatioInput::*myFunc)();
            
				static VoltageRatioInputRef create();
				static VoltageRatioInputRef create( int serialNum, int channelNum );
            
				~VoltageRatioInput();
                void setDelegate(VoltageRatioInputDelegateRef delegate){ mDelegate = delegate; };
            
				double getSensorVal();

			protected:
				VoltageRatioInput();
				void setup( int serialNum = -1, int channelNum = 0 );

			private:
				PhidgetVoltageRatioInputHandle mHandle = NULL;
                double (VoltageRatioInput::*testFuncPointer)(double) = NULL;
                myFunc testFunc = NULL;

				//	called during setup
				static int createVoltageRatioInput( PhidgetVoltageRatioInputHandle* pvrih );
				int setSerialNumber( PhidgetHandle ph, int deviceSerialNumber = -1 );
				int setChannel( PhidgetHandle ph, int channel = 0 );
				int setAttachDetachError_Handlers( PhidgetHandle ch );
				int setVoltageRatioHandler( PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr );
                int setVoltageRatioHandler(PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr, VoltageRatioInputDelegateRef delegate);
                int setVoltageRatioHandler(PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr, double(VoltageRatioInput::*)(double));
                int setVoltageRatioHandler(PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr, std::function<void(double)>func2);
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
                void testCallbackFunction(double test);
                void superSimpleTestFunc();
		};
	}
}
