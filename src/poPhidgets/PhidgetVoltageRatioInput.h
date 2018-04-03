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
		};

	}
}