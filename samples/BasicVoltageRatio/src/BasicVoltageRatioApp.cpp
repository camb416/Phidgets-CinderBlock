#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"


#include "poPhidgets/PhidgetVoltageRatioInput.h"
#include "poPhidgets/PhidgetDistanceSensor.h"

#include "cinder/Log.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class BasicVoltageRatioApp
	: public App
	, public po::phidget::VoltageRatioInputDelegate
{
	public:
		void setup() override;
		void mouseDown( MouseEvent event ) override;
		void update() override;
		void draw() override;
		void keyDown( KeyEvent event ) override;
		void voltageRatioValueChanged( double ratio ) override;

		po::phidget::VoltageRatioInputRef mInput;
		po::phidget::DistanceSensorRef mDistanceInput;

};

void BasicVoltageRatioApp::setup()
{
    mInput = po::phidget::VoltageRatioInput::create();
    mInput->setProperties( -1, 5, 10, 0.01 );
    mInput->createAndOpenChannel();

//    mDistanceInput = po::phidget::DistanceSensor::create();
//    mDistanceInput->setProperties( -1, 5 );
//    mDistanceInput->createAndOpenChannel();
}

void BasicVoltageRatioApp::mouseDown( MouseEvent event )
{
	double voltageRatio = mInput->getVoltageRatio();
	CI_LOG_V( "Calling getVoltageRatio on VoltageRatioInput: " << voltageRatio );
    
    double sensorVal = mInput->getSensorValue();
    CI_LOG_V( "Calling getSensorValue on VoltageRatioInput: " << sensorVal );
}

void BasicVoltageRatioApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'q' || event.getChar() == 'Q' ) {
		quit();
	}
}

void BasicVoltageRatioApp::voltageRatioValueChanged( double ratio )
{
	CI_LOG_V( ratio );
}

void BasicVoltageRatioApp::update()
{
}

void BasicVoltageRatioApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );

}

CINDER_APP( BasicVoltageRatioApp, RendererGl )
