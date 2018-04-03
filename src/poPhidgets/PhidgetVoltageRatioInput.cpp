#include "PhidgetVoltageRatioInput.h"
#include "cinder/Log.h"


namespace po
{
	namespace phidget
	{
		VoltageRatioInputRef VoltageRatioInput::create()
		{
			return VoltageRatioInputRef();
		}

		VoltageRatioInput::VoltageRatioInput()
		{}

		void VoltageRatioInput::setup()
		{

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

		VoltageRatioInput::~VoltageRatioInput()
		{
			ClosePhidgetChannel( ( PhidgetHandle )mHandle );
			PhidgetVoltageRatioInput_delete( &mHandle );
		}


	}
}
