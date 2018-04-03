#pragma once
#include <memory>

#include "phidget22.h"
#include "PhidgetHelperFunctions.h"


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

			protected:
			private:

		};

	}
}