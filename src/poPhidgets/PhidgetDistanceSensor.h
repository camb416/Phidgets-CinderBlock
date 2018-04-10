#pragma once
#include "poPhidgets/PhidgetBaseInput.h"

namespace po
{
	namespace phidget
	{
		class DistanceSensor;
		typedef std::shared_ptr<DistanceSensor> DistanceSensorRef;

		class DistanceSensor
			: public po::phidget::BaseInput
		{
			public:
				static DistanceSensorRef create();
				void setProperties( int serialNum, int channelNum );


				virtual PhidgetHandle getHandle() override { return PhidgetHandle( mHandle ); };
				~DistanceSensor();

			protected:
			private:
				PhidgetDistanceSensorHandle mHandle;
				int createSpecificInput() override;

				int setAttachDetachErrorHandlers( PhidgetHandle ch ) override;
				void setChangeHandlers( PhidgetHandle ch ) override;

				//	event handlers
				static void CCONV onAttachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onDetachHandler( PhidgetHandle ph, void* ctx );
				static void CCONV onErrorHandler( PhidgetHandle phid, void* ctx, Phidget_ErrorEventCode errorCode, const char* errorString );
				static void CCONV onDistanceChangeHandler( PhidgetDistanceSensorHandle ch, void* ctx, int distance );
				static void CCONV onSonarReflectionsUpdateHandler( PhidgetDistanceSensorHandle ch, void* ctx, const uint32_t distances[8], const uint32_t amplitudes[8], uint32_t count );

		};
	}
}