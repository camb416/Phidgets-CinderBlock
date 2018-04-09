#include "PhidgetVoltageRatioInput.h"
#include "cinder/Log.h"
#include "cinder/app/App.h"

namespace po
{
	namespace phidget
	{
		//
		//	Create without parameters uses default values in setup
		//
		VoltageRatioInputRef VoltageRatioInput::create()
		{
			VoltageRatioInputRef ref( new VoltageRatioInput() );
			ref->setup( -1, 0 , 100, 0.05);
			return ref;
		}

		//
        //	Create and specify the following:
        //  Serial number: set to -1 to read from device
        //  Channel number
        //  Data interval: The frequency, in milliseconds, the device checks the value
        //  Change trigger: amount of change that will trigger an event; set to 0 to trigger event every interval
		//
		VoltageRatioInputRef VoltageRatioInput::create( int serialNum, int channelNum, int dataInterval, double changeTrigger )
		{
			VoltageRatioInputRef ref( new VoltageRatioInput() );
			ref->setup( serialNum, channelNum, dataInterval, changeTrigger );
			return ref;
		}

		VoltageRatioInput::VoltageRatioInput()
            : mDataInterval(100)
            , mChangeTrigger(0.05)
		{}

		//
		//	close channel on destroy
		//
		VoltageRatioInput::~VoltageRatioInput()
		{
			if( mHandle != NULL ) {
				CI_LOG_V( "Closing voltage ratio input handle" );
				closePhidgetChannel( ( PhidgetHandle )mHandle );
				PhidgetVoltageRatioInput_delete( &mHandle ); // this sets handle value to NULL

			}
			else {
				CI_LOG_V( "Handle was null; methods in destructor skipped" );
			}
		}

		//
		//	Public method;
		//	Returns last reported voltage ratio value from channel
		double VoltageRatioInput::getSensorVal()
		{
			if( mHandle == NULL ) {
				CI_LOG_W( "VoltageRatioInput handle does not exist" );
				return 0.0;
			}

			PhidgetReturnCode prc;
			double voltageRatio;
			prc = PhidgetVoltageRatioInput_getVoltageRatio( mHandle, &voltageRatio );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Creating VoltageRatioInput:" );
				displayError( prc );
				return 1;
			}

			return voltageRatio;
		}

		void VoltageRatioInput::setup( int serialNum, int channelNum, int dataInterval, double changeTrigger )
		{
            mDataInterval = dataInterval;
            mChangeTrigger = changeTrigger;
            
			if( createVoltageRatioInput( &mHandle ) ) {
				return;
			}

			if( setSerialNumber( PhidgetHandle( mHandle ), serialNum ) ) {
				return;
			}

			if( setChannel( PhidgetHandle( mHandle ), channelNum ) ) {
				return;
			}

			if( setAttachDetachError_Handlers( PhidgetHandle( mHandle ) ) ) {
				return;
			}

            if( setVoltageRatioHandler( mHandle, onVoltageRatioChangeHandler))  {
                return;
            }

			if( openPhidgetChannelWithTimeout( PhidgetHandle( mHandle ), 5000 ) ) {
				return;
			}

		}

		/**
		* Creates a new instance of a VoltageRatioInput channel.
		*
		* @param pvrih Pointer to the PhidgetVoltageRatioInputHandle channel to create
		* @return 0 if the operation succeeds, 1 if it fails
		*/
		int VoltageRatioInput::createVoltageRatioInput( PhidgetVoltageRatioInputHandle* pvrih )
		{
			PhidgetReturnCode prc;
			CI_LOG_V( "Creating VoltageRatioInput Channel..." );
			prc = PhidgetVoltageRatioInput_create( pvrih );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Creating VoltageRatioInput:" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		//
		//	Set serial number to
		int VoltageRatioInput::setSerialNumber( PhidgetHandle ph, int deviceSerialNumber )
		{
			CI_LOG_V( "Setting Phidget serial number to " << deviceSerialNumber );
			PhidgetReturnCode prc;
			prc = Phidget_setDeviceSerialNumber( ph, deviceSerialNumber );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Setting DeviceSerialNumber" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		int VoltageRatioInput::setChannel( PhidgetHandle ph, int channel )
		{
			CI_LOG_V( "Setting channel to " << channel );
			PhidgetReturnCode prc;
			prc = Phidget_setChannel( ph, channel );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Setting Channel: \n\t" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		/**
		* Sets the event handlers for Phidget Attach, Phidget Detach, Phidget Error events
		*
		* @param ph The Phidget channel to add event handlers to
		* @return 0 if the operation succeeds, 1 if it fails
		*/
		int VoltageRatioInput::setAttachDetachError_Handlers( PhidgetHandle ch )
		{
			PhidgetReturnCode prc;
			CI_LOG_V( "Setting OnAttachHandler..." );
			prc = Phidget_setOnAttachHandler( ch, onAttachHandler, this );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Set Attach Handler" );
				displayError( prc );
				return 1;
			}

			CI_LOG_V( "Setting OnDetachHandler..." );
			prc = Phidget_setOnDetachHandler( ch, onDetachHandler, NULL );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Set Detach Handler" );
				displayError( prc );
				return 1;
			}

			CI_LOG_V( "Setting OnErrorHandler..." );
			prc = Phidget_setOnErrorHandler( ch, onErrorHandler, NULL );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Set Error Handler" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		/**
		* Sets the event handler for VoltageRatioInput's VoltageRatioChange event
		*
		* @param pvrih The PhidgetVoltageRatioInputHandle channel to add the event to
		* @param fptr The callback function to be called when a VoltageRatioChange event is fired
		* @return 0 if the operation succeeds, 1 if it fails
		*/
		int VoltageRatioInput::setVoltageRatioHandler( PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr )
		{
			PhidgetReturnCode prc;
			CI_LOG_V( "Setting voltage ratio handler" );
			prc = PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler( pvrih, fptr, this );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Setting VoltageRatioChangeHandler: \n\t" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

        int VoltageRatioInput::openPhidgetChannelWithTimeout( PhidgetHandle ch, int timeout )
		{
			PhidgetReturnCode prc;

			CI_LOG_V( "Opening Channel..." );
			prc = Phidget_openWaitForAttachment( ch, timeout );

			if( prc != EPHIDGET_OK ) {
				CI_LOG_E( "Runtime Error -> Opening Phidget Channel" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		int VoltageRatioInput::closePhidgetChannel( PhidgetHandle ch )
		{
			PhidgetReturnCode prc;

			CI_LOG_V( "Closing Channel..." );
			prc = Phidget_close( ch );

			if( prc != EPHIDGET_OK ) {
				CI_LOG_E( "Runtime Error -> Closing Phidget Channel" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		/**
		* Configures the device's DataInterval and ChangeTrigger.
		* Displays info about the attached phidget channel.
		* Fired when a Phidget channel with onAttachHandler registered attaches
		*
		* @param ph The Phidget channel that fired the attach event
		* @param *ctx Context pointer
		*/
		void CCONV VoltageRatioInput::onAttachHandler( PhidgetHandle ph, void* ctx )
		{
            VoltageRatioInput* voltageRatioInstance = (VoltageRatioInput*)ctx;

			CI_LOG_V( "onAttachHandler" );
			PhidgetReturnCode prc;
			int32_t serialNumber;
			PhidgetHandle hub;
			int32_t hubPort;
			int32_t channel;

            /*
             *    Set the DataInterval inside of the attach handler to initialize the device with this value.
             *    DataInterval defines the minimum time between VoltageRatioChange events.
             *    DataInterval can be set to any value from MinDataInterval to MaxDataInterval.
             */
            
			//	Find max and min data intervals

			uint32_t minDataInterval;
			uint32_t maxDataInterval;

			prc = PhidgetVoltageRatioInput_getMinDataInterval( ( PhidgetVoltageRatioInputHandle )ph, &minDataInterval );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Getting min data interval" );
				displayError( prc );
				return;
			}

			prc = PhidgetVoltageRatioInput_getMaxDataInterval( ( PhidgetVoltageRatioInputHandle )ph, &maxDataInterval );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Getting max data interval" );
				displayError( prc );
				return;
			}

			CI_LOG_D( "Max and min data intervals (in milliseconds)\t Max: " << maxDataInterval << ", min: " << minDataInterval );
            
            if (voltageRatioInstance->mDataInterval > maxDataInterval || voltageRatioInstance->mDataInterval < minDataInterval) {
                CI_LOG_W("Setting data interval to value outside max and min limits.");
            }

            //  Set the actual data interval
			prc = PhidgetVoltageRatioInput_setDataInterval( ( PhidgetVoltageRatioInputHandle )ph, voltageRatioInstance->mDataInterval );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Set DataInterval" );
				displayError( prc );
				return;
			}

			/*
			*	Set the VoltageRatioChangeTrigger inside of the attach handler to initialize the device with this value.
			*	VoltageRatioChangeTrigger will affect the frequency of VoltageRatioChange events, by limiting them to only occur when
			*	the ratio changes by at least the value set.
			*/
            
            //  find min and max change trigger
            
            double minChangeTrigger;
            double maxChangeTrigger;
            
            prc = PhidgetVoltageRatioInput_getMinVoltageRatioChangeTrigger( ( PhidgetVoltageRatioInputHandle )ph, &minChangeTrigger );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting min change trigger" );
                displayError( prc );
                return;
            }
            
            prc = PhidgetVoltageRatioInput_getMaxVoltageRatioChangeTrigger( ( PhidgetVoltageRatioInputHandle )ph, &maxChangeTrigger );
            
            if( EPHIDGET_OK != prc ) {
                CI_LOG_E( "Runtime Error -> Getting max change trigger" );
                displayError( prc );
                return;
            }
            
            CI_LOG_D( "Max and min voltage ratio change triggers\t Max: " << maxChangeTrigger << ", min: " << minChangeTrigger );
            CI_LOG_V("Setting change trigger to " << voltageRatioInstance->mChangeTrigger);
            if (voltageRatioInstance->mChangeTrigger > maxChangeTrigger || voltageRatioInstance->mChangeTrigger < minChangeTrigger) {
                CI_LOG_W("Setting change trigger to value outside the max and min limits.");
            }
			prc = PhidgetVoltageRatioInput_setVoltageRatioChangeTrigger( ( PhidgetVoltageRatioInputHandle )ph, voltageRatioInstance->mChangeTrigger );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Set VoltageRatioChangeTrigger: \n\t" );
				displayError( prc );
				return;
			}

			prc = Phidget_getDeviceSerialNumber( ph, &serialNumber );
			CI_LOG_V( "Getting serial number: " << serialNumber );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Get DeviceSerialNumber: \n\t" );
				displayError( prc );
				return;
			}

			prc = Phidget_getChannel( ph, &channel );
			CI_LOG_V( "Getting channel: " << channel );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Get Channel: \n\t" );
				displayError( prc );
				return;
			}

			//Check if this is a VINT device
			prc = Phidget_getHub( ph, &hub );
			CI_LOG_V( "Checking hub: " << hub );

			if( EPHIDGET_WRONGDEVICE != prc ) {
				prc = Phidget_getHubPort( ph, &hubPort );

				if( EPHIDGET_OK != prc ) {
					CI_LOG_E( "Runtime Error -> Get HubPort: \n\t" );
					displayError( prc );
					return;
				}

				CI_LOG_V( "Attach Event: Serial Number: " << serialNumber << "\n\t-> Hub Port: " << hubPort << "\n\t-> Channel: " << channel );
			}
			else {
				CI_LOG_V( "Attach Event:Serial Number: " << serialNumber << "\n\t-> Channel: " << channel );
			}
		}

		/**
		* Displays info about the detached phidget channel.
		* Fired when a Phidget channel with onDetachHandler registered detaches
		*
		* @param ph The Phidget channel that fired the detach event
		* @param *ctx Context pointer
		*/

		inline void CCONV VoltageRatioInput::onDetachHandler( PhidgetHandle ph, void* ctx )
		{
			CI_LOG_V( "onDetachHandler" );
			PhidgetReturnCode prc;
			PhidgetHandle hub;
			int serialNumber;
			int hubPort;
			int channel;

			prc = Phidget_getDeviceSerialNumber( ph, &serialNumber );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Get DeviceSerialNumber: \n\t" );
				displayError( prc );
				return;
			}

			prc = Phidget_getChannel( ph, &channel );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Get Channel: \n\t" );
				displayError( prc );
				return;
			}

			//Check if this is a VINT device
			prc = Phidget_getHub( ph, &hub );

			if( EPHIDGET_WRONGDEVICE != prc ) {
				prc = Phidget_getHubPort( ph, &hubPort );

				if( EPHIDGET_OK != prc ) {
					CI_LOG_E( "Runtime Error -> Get HubPort: \n\t" );
					displayError( prc );
					return;
				}

				CI_LOG_V( "Detach Event:-> Serial Number: " << serialNumber << "\n\t-> Hub Port: " << hubPort << "\n\t-> Channel " << channel );
			}
			else {
				CI_LOG_V( "Detach Event:-> Serial Number: " << serialNumber << "\n\t-> Channel " << channel );
			}
		}

		/**
		* Writes phidget error info to stderr.
		* Fired when a Phidget channel with onErrorHandler registered encounters an error in the library
		*
		* @param ph The Phidget channel that fired the error event
		* @param *ctx Context pointer
		* @param errorCode the code associated with the error of enum type Phidget_ErrorEventCode
		* @param *errorString string containing the description of the error fired
		*/
		void CCONV VoltageRatioInput::onErrorHandler( PhidgetHandle phid, void* ctx, Phidget_ErrorEventCode errorCode, const char* errorString )
		{
			CI_LOG_E( "[Phidget Error Event] -> " << errorString << ", code:" << errorCode );
		}


		/**
		* Outputs the VoltageRatioInput's most recently reported ratio.
		* Fired when a VoltageRatioInput channel with onVoltageRatioChangeHandler registered meets DataInterval and ChangeTrigger criteria
		*
		* @param pvrih The VoltageRatioInput channel that fired the VoltageRatioChange event
		* @param *ctx Context pointer
		* @param ratio The reported voltage ratio from the VoltageRatioInput channel
		*/
        void CCONV VoltageRatioInput::onVoltageRatioChangeHandler( PhidgetVoltageRatioInputHandle pvrih, void* ctx, double ratio )
		{
			CI_LOG_V( "[VoltageRatio Event] -> Ratio: " << ratio );
            VoltageRatioInput* voltageRatioInstance = (VoltageRatioInput*)ctx;
            if (!voltageRatioInstance->mDelegate.expired()) {
                voltageRatioInstance->mDelegate.lock()->voltageRatioValueChanged(ratio);
            } else {
                CI_LOG_V("delegate expired");
            }
            voltageRatioInstance->testCallbackFunction(ratio);
		}

		//
		//	Display Error messages
		//
		void VoltageRatioInput::displayError( PhidgetReturnCode code )
		{
			PhidgetReturnCode prc;
			const char* error;

			prc = Phidget_getErrorDescription( code, &error );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Getting ErrorDescription: \n\t" );
				//displayError( prc );
				return;
			}

			CI_LOG_E( "Error desc: " << error );

		}
        
        //
        //  test
        //
        void VoltageRatioInput::testCallbackFunction(double test)
        {
            CI_LOG_D("Non-static callback function value: " << test);
        }
	}
}
