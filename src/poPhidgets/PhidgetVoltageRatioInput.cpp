#include "PhidgetVoltageRatioInput.h"
#include "cinder/Log.h"
#include "cinder/app/App.h"

namespace po
{
	namespace phidget
	{
		VoltageRatioInputRef VoltageRatioInput::create()
		{
			VoltageRatioInputRef ref( new VoltageRatioInput() );
			ref->setup();
			return ref;
		}

		VoltageRatioInput::VoltageRatioInput()
		{}


		VoltageRatioInput::~VoltageRatioInput()
		{
			CI_LOG_V( "Closing voltage ratio input handle" );
			ClosePhidgetChannel( ( PhidgetHandle )mHandle );
			PhidgetVoltageRatioInput_delete( &mHandle );
		}

		void VoltageRatioInput::setup()
		{
			createVoltageRatioInput( &mHandle );
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
				CI_LOG_E( "Runtime Error -> Creating VoltageRatioInput: \n\t" );
				DisplayError( prc );
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
			PhidgetReturnCode prc;
			int32_t serialNumber;
			PhidgetHandle hub;
			int32_t hubPort;
			int32_t channel;

			/*
			*	Set the DataInterval inside of the attach handler to initialize the device with this value.
			*	DataInterval defines the minimum time between VoltageRatioChange events.
			*	DataInterval can be set to any value from MinDataInterval to MaxDataInterval.
			*/
			printf( "\tSetting DataInterval to 1000ms\n" );
			prc = PhidgetVoltageRatioInput_setDataInterval( ( PhidgetVoltageRatioInputHandle )ph, 1000 );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Set DataInterval: \n\t" );
				DisplayError( prc );
				return;
			}

			/*
			*	Set the VoltageRatioChangeTrigger inside of the attach handler to initialize the device with this value.
			*	VoltageRatioChangeTrigger will affect the frequency of VoltageRatioChange events, by limiting them to only occur when
			*	the ratio changes by at least the value set.
			*/
			printf( "\tSetting VoltageRatio ChangeTrigger to 0.0\n" );
			prc = PhidgetVoltageRatioInput_setVoltageRatioChangeTrigger( ( PhidgetVoltageRatioInputHandle )ph, 0.0 );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Set VoltageRatioChangeTrigger: \n\t" );
				DisplayError( prc );
				return;
			}

			prc = Phidget_getDeviceSerialNumber( ph, &serialNumber );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Get DeviceSerialNumber: \n\t" );
				DisplayError( prc );
				return;
			}

			prc = Phidget_getChannel( ph, &channel );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Get Channel: \n\t" );
				DisplayError( prc );
				return;
			}

			//Check if this is a VINT device
			prc = Phidget_getHub( ph, &hub );

			if( EPHIDGET_WRONGDEVICE != prc ) {
				prc = Phidget_getHubPort( ph, &hubPort );

				if( EPHIDGET_OK != prc ) {
					fprintf( stderr, "Runtime Error -> Get HubPort: \n\t" );
					DisplayError( prc );
					return;
				}

				printf( "\n[Attach Event]:\n\t-> Serial Number: %d\n\t-> Hub Port: %d\n\t-> Channel %d\n\n", serialNumber, hubPort, channel );
			}
			else {
				printf( "\nAttach Event:\n\t-> Serial Number: %d\n\t-> Channel %d\n\n", serialNumber, channel );
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
			PhidgetReturnCode prc;
			PhidgetHandle hub;
			int serialNumber;
			int hubPort;
			int channel;

			prc = Phidget_getDeviceSerialNumber( ph, &serialNumber );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Get DeviceSerialNumber: \n\t" );
				DisplayError( prc );
				return;
			}

			prc = Phidget_getChannel( ph, &channel );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Get Channel: \n\t" );
				DisplayError( prc );
				return;
			}

			//Check if this is a VINT device
			prc = Phidget_getHub( ph, &hub );

			if( EPHIDGET_WRONGDEVICE != prc ) {
				prc = Phidget_getHubPort( ph, &hubPort );

				if( EPHIDGET_OK != prc ) {
					fprintf( stderr, "Runtime Error -> Get HubPort: \n\t" );
					DisplayError( prc );
					return;
				}

				printf( "\nDetach Event:\n\t-> Serial Number: %d\n\t-> Hub Port: %d\n\t-> Channel %d\n\n", serialNumber, hubPort, channel );
			}
			else {
				printf( "\nDetach Event:\n\t-> Serial Number: %d\n\t-> Channel %d\n\n", serialNumber, channel );
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

			fprintf( stderr, "[Phidget Error Event] -> %s (%d)\n", errorString, errorCode );
		}

		/**
		* Sets the event handlers for Phidget Attach, Phidget Detach, Phidget Error events
		*
		* @param ph The Phidget channel to add event handlers to
		* @return 0 if the operation succeeds, 1 if it fails
		*/
		int VoltageRatioInput::SetAttachDetachError_Handlers( PhidgetHandle ch )
		{
			PhidgetReturnCode prc;
			printf( "\n--------------------------------------\n" );
			printf( "\nSetting OnAttachHandler...\n" );
			prc = Phidget_setOnAttachHandler( ch, onAttachHandler, NULL );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Set Attach Handler: \n\t" );
				DisplayError( prc );
				return 1;
			}

			printf( "Setting OnDetachHandler...\n" );
			prc = Phidget_setOnDetachHandler( ch, onDetachHandler, NULL );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Set Detach Handler: \n\t" );
				DisplayError( prc );
				return 1;
			}

			printf( "Setting OnErrorHandler...\n" );
			prc = Phidget_setOnErrorHandler( ch, onErrorHandler, NULL );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Set Error Handler: \n\t" );
				DisplayError( prc );
				return 1;
			}

			return 0;
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
			printf( "[VoltageRatio Event] -> Ratio: %.4f\n", ratio );
		}

		/**
		* Creates a new instance of a VoltageRatioInput channel.
		*
		* @param pvrih Pointer to the PhidgetVoltageRatioInputHandle channel to create
		* @return 0 if the operation succeeds, 1 if it fails
		*/
		int VoltageRatioInput::CreateVoltageRatioInput( PhidgetVoltageRatioInputHandle* pvrih )
		{
			PhidgetReturnCode prc;

			printf( "Creating VoltageRatioInput Channel...\n" );
			prc = PhidgetVoltageRatioInput_create( pvrih );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Creating VoltageRatioInput: \n\t" );
				DisplayError( prc );
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
		int VoltageRatioInput::SetVoltageRatioHandler( PhidgetVoltageRatioInputHandle pvrih, PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr )
		{
			PhidgetReturnCode prc;

			if( fptr ) {
				printf( "\n--------------------\n"
				        "\n  | Voltage ratio change events contain the most recent voltage ratio received from the device.\n"
				        "  | The linked VoltageRatioChange function will run as an event at every DataInterval.\n"
				        "  | These events will not occur until a change in ratio >= to the set ChangeTrigger has occured.\n"
				        "  | DataInterval and ChangeTrigger should initially be set in the device AttachHandler function.\n" );
			}

			printf( "\n%s OnVoltageRatioChangeHandler...\n", ( fptr ? "Setting" : "Clearing" ) );
			printf( "\n--------------------\n" );
			prc = PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler( pvrih, fptr, NULL );

			if( EPHIDGET_OK != prc ) {
				fprintf( stderr, "Runtime Error -> Setting VoltageRatioChangeHandler: \n\t" );
				DisplayError( prc );
				return 1;
			}

			return 0;
		}




	}
}
