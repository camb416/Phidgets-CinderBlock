#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#define CI_MIN_LOG_LEVEL 4
#include "cinder/Log.h"

#include "poPhidgets/PhidgetVoltageRatioInput.h"

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
};

void BasicVoltageRatioApp::setup()
{
	//    auto consoleLogger = log::makeLogger<cinder::log::LoggerConsole>();
	mInput = po::phidget::VoltageRatioInput::create( -1, 5, 10, 0.01 );
}

void BasicVoltageRatioApp::mouseDown( MouseEvent event )
{
	double sensorVal = mInput->getSensorVal();
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
