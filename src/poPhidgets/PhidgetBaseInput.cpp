#include "PhidgetBaseInput.h"
#include "cinder/Log.h"

namespace po
{
	namespace phidget
	{
		//
		//	Once input's properties are set, can set up handlers and open channel
		//
		void BaseInput::createAndOpenChannel( int timeout )
		{
			createSpecificInput();

			if( setSerialNumber( getHandle(), mSerialNumber ) ) {
				return;
			}

			if( setChannel( getHandle(), mChannel ) ) {
				return;
			}

			if( setAttachDetachErrorHandlers( getHandle() ) ) {
				return;
			}

			setChangeHandlers( getHandle() );


			if( openPhidgetChannelWithTimeout( getHandle(), timeout ) ) {
				return;
			}

		}


		int BaseInput::setSerialNumber( PhidgetHandle ph, int deviceSerialNumber )
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


		int BaseInput::setChannel( PhidgetHandle ph, int channel )
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


		int BaseInput::openPhidgetChannelWithTimeout( PhidgetHandle ch, int timeout )
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

		int BaseInput::closePhidgetChannel( PhidgetHandle ch )
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

		void BaseInput::displayError( PhidgetReturnCode code )
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
	}
}