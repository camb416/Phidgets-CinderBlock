#include "PhidgetDistanceSensor.h"
namespace po
{
	namespace phidget
	{
		DistanceSensorRef DistanceSensor::create()
		{
			return DistanceSensorRef( new DistanceSensor() );
		}

		void DistanceSensor::setProperties( int serialNum, int channelNum )
		{
			mSerialNumber = serialNum;
			mChannel = channelNum;
		}

		//
		//	close channel on destroy
		//
		DistanceSensor::~DistanceSensor()
		{
			if( mHandle != NULL ) {
				CI_LOG_V( "Closing distance sensor input handle" );
				closePhidgetChannel( ( PhidgetHandle )mHandle );
				PhidgetDistanceSensor_delete( &mHandle ); // this sets handle value to NULL

			}
			else {
				CI_LOG_V( "Handle was null; methods in destructor skipped" );
			}
		}

		//
		//	Called by base function;
		//	creates distance sensor input
		//
		int DistanceSensor::createSpecificInput()
		{
			PhidgetReturnCode prc;
			CI_LOG_V( "Creating DistanceSensor channel" );
			prc = PhidgetDistanceSensor_create( &mHandle );

			if( EPHIDGET_OK != prc ) {
				CI_LOG_E( "Runtime Error -> Creating DistanceSensorInput:" );
				displayError( prc );
				return 1;
			}

			return 0;
		}

		int DistanceSensor::setAttachDetachErrorHandlers( PhidgetHandle ch )
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

		void DistanceSensor::setChangeHandlers( PhidgetHandle ch )
		{
			CI_LOG_V( "setChangeHandlers" );
		}

		void CCONV DistanceSensor::onAttachHandler( PhidgetHandle ph, void* ctx )
		{
			CI_LOG_V( "onAttachHandler" );
			PhidgetReturnCode prc;
			int hubPort;
			int channel;
			int serial;

			prc = Phidget_getDeviceSerialNumber( ph, &serial );

			if( prc != EPHIDGET_OK ) {
				CI_LOG_E( "failed to get device serial number\n" );
				return;
			}

			prc = Phidget_getChannel( ph, &channel );

			if( prc != EPHIDGET_OK ) {
				CI_LOG_E( "failed to get channel number\n" );
				return;
			}

			prc = Phidget_getHubPort( ph, &hubPort );

			if( prc != EPHIDGET_OK ) {
				CI_LOG_E( "failed to get hub port\n" );
				hubPort = -1;
			}

			if( hubPort == -1 ) {
				CI_LOG_V( "channel " << channel <<  " on device " << serial << " attached\n" );
			}
			else {
				CI_LOG_V( "channel " << channel << "on device " << serial << " hub port " << hubPort << " attached\n" );
			}

		}

		void CCONV DistanceSensor::onDetachHandler( PhidgetHandle ph, void* ctx )
		{
			CI_LOG_V( "onDetachHandler" );
		}

		void CCONV DistanceSensor::onErrorHandler( PhidgetHandle phid, void* ctx, Phidget_ErrorEventCode errorCode, const char* errorString )
		{
			CI_LOG_V( "onErrorHandler" );
		}

		void CCONV DistanceSensor::onDistanceChangeHandler( PhidgetDistanceSensorHandle ch, void* ctx, int distance )
		{
			CI_LOG_V( "onDistanceChangeHandler" );
		}

		void CCONV DistanceSensor::onSonarReflectionsUpdateHandler( PhidgetDistanceSensorHandle ch, void* ctx, const uint32_t distances[8], const uint32_t amplitudes[8], uint32_t count )
		{
			CI_LOG_V( "onSonarReflectionsUpdateHandler" );
		}

	}
}
