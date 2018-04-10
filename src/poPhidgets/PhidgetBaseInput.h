#pragma once

#include <memory>
#include <phidget22.h>
#include "cinder/Log.h"

//
//	Abstract Phidgets input class
//

namespace po
{
	namespace phidget
	{
		class BaseInput;
		typedef std::shared_ptr<BaseInput> BaseInputRef;

		class BaseInput
			: public std::enable_shared_from_this<BaseInput>
		{
			public:
				void createAndOpenChannel( int timeout = 5000 );
				int closePhidgetChannel( PhidgetHandle ch );
				virtual PhidgetHandle getHandle() = 0;

			protected:
				//	properties of all inputs
				int mSerialNumber;
				int mChannel;

				int setSerialNumber( PhidgetHandle ph, int deviceSerialNumber = -1 );
				int setChannel( PhidgetHandle ph, int channel = 0 );
				virtual int createSpecificInput() = 0;

				virtual int setAttachDetachErrorHandlers( PhidgetHandle ph ) = 0;
				virtual void setChangeHandlers( PhidgetHandle ph ) {};
				int openPhidgetChannelWithTimeout( PhidgetHandle ch, int timeout = 5000 );

				//	error display
				static void displayError( PhidgetReturnCode code );


			private:
		};

	}
}