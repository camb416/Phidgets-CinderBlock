#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#define CI_MIN_LOG_LEVEL 0
#include "cinder/Log.h"

#include "poPhidgets/PhidgetVoltageRatioInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RatioAppShared;

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
        void voltageRatioValueChanged(double ratio) override;

		po::phidget::VoltageRatioInputRef mInput;
};

void BasicVoltageRatioApp::setup()
{
//    po::phidget::VoltageRatioInputDelegateRef self =  std::make_shared<po::phidget::VoltageRatioInputDelegate>(this);
//    std::weak_ptr<po::phidget::VoltageRatioInputDelegate> weakPtr = self;
//    CI_LOG_V(self);
//    CI_LOG_V(weakPtr.use_count());
//    mInput = po::phidget::VoltageRatioInput::create(weakPtr, -1, 5);
    mInput = po::phidget::VoltageRatioInput::create(-1, 5);
    
//    std::shared_ptr<App> sp = std::make_shared<App>();
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

void BasicVoltageRatioApp::voltageRatioValueChanged(double ratio)
{
    CI_LOG_V(ratio);
}

void BasicVoltageRatioApp::update()
{
}

void BasicVoltageRatioApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    int height = ci::app::getWindow()->getHeight();
    
}

CINDER_APP( BasicVoltageRatioApp, RendererGl )
