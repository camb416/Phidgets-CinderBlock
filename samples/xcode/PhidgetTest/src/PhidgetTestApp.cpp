#include "cinder/app/AppBasic.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Capture.h"
#include "cinder/Camera.h"
#include "cinder/Text.h"
#include "cinder/Rect.h"
#include "PhidgetConnector.h"

using namespace ci;
using namespace ci::app;
using namespace std;



class PhidgetTestApp : public AppBasic {
    
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void resize(ResizeEvent evt);
    vector<float> dValues;
    vector<float> values;
    vector<Rectf> rectangles;
    Vec2i * sensorAddrs;
    int numSensors;
    int margin;
    int gutter;
    
    PhidgetConnector pConnector;    // the Phidget Connector object.
    
};

void PhidgetTestApp::setup()
{
    margin = 15;
    gutter = 20;
    
    numSensors = 16;
    sensorAddrs = new Vec2i[numSensors];
    
    sensorAddrs[0] = Vec2i(148986,0);
    sensorAddrs[1] = Vec2i(148986,1);
    sensorAddrs[2] = Vec2i(148986,2);
    sensorAddrs[3] = Vec2i(148986,3);
    sensorAddrs[4] = Vec2i(148986,4);
    sensorAddrs[5] = Vec2i(148986,5);
    sensorAddrs[6] = Vec2i(148986,6);
    sensorAddrs[7] = Vec2i(148986,7);
    
    sensorAddrs[8] = Vec2i(148959,0);
    sensorAddrs[9] = Vec2i(148959,1);
    sensorAddrs[10] = Vec2i(148959,2);
    sensorAddrs[11] = Vec2i(148959,3);
    sensorAddrs[12] = Vec2i(148959,4);
    sensorAddrs[13] = Vec2i(148959,5);
    sensorAddrs[14] = Vec2i(148959,6);
    sensorAddrs[15] = Vec2i(148959,7);
    
    pConnector.useEvents(false);    // if you use events, you don't need to call the updateKits method
    pConnector.connect(148986);     // connect to as many devices as you like.
    pConnector.connect(148959);
    
    for(int i=0;i<numSensors;i++){
        int Rw = (getWindowWidth()-2*margin-gutter*(numSensors-1))/(numSensors);
        int Rx = margin+Rw*i+gutter*(i);
        int Ry = getWindowHeight()-margin;
        int Rh = 0;
        rectangles.push_back(Rectf( Vec2f( Rx,Ry ), Vec2f( Rx+Rw,Ry+Rh ) ));   
    }
    for(int i=0;i<numSensors;i++){
        dValues.push_back(10);
        values.push_back(100);
    }
}

void PhidgetTestApp::mouseDown( MouseEvent event ){
    //   pConnector.reset();
    pConnector.print();
}

void PhidgetTestApp::update(){
    // manually request the block to update the info from the hardware
    pConnector.updateKits();    
    
    for(int i=0;i<numSensors;i++){
        
        // return a true/false for the sensor (uses 500 as a cutoff
        bool thisBool = pConnector.getBool(sensorAddrs[i].x,sensorAddrs[i].y)/1000.0f*(getWindowHeight()-margin*2);         int thisInt;
        thisBool ? thisInt = (getWindowHeight()-margin*2) : thisInt = 20;
        dValues.at(i) = thisInt;        
    }
    
    for(int i=0;i<dValues.size();i++){
        values.at(i) += (dValues.at(i)-values.at(i))/16.0f;
        rectangles.at(i).y1 = rectangles.at(i).y2-values.at(i);
    }
}

void PhidgetTestApp::draw(){
    gl::clear( Color( 0.3f, 0.3f, 0.3f ) );
    ci::gl::enableAlphaBlending();
    for(int i=0;i<numSensors;i++){
        Rectf shadowRect = Rectf(rectangles.at(i).x1-10.0f,rectangles.at(i).y1-10.0f,rectangles.at(i).x2+10.0f,rectangles.at(i).y2+10.0f);
        ci::gl::color(0,0,0,0.25f);
        ci::gl::drawSolidRoundedRect(shadowRect, 15.0f);
        shadowRect = Rectf(rectangles.at(i).x1-5.0f,rectangles.at(i).y1-5.0f,rectangles.at(i).x2+5.0f,rectangles.at(i).y2+5.0f);
        ci::gl::color(255,255,255,0.25f);
        ci::gl::drawSolidRoundedRect(shadowRect, 10.0f);
        ci::gl::color(255,255,255);
        ci::gl::drawSolidRoundedRect(rectangles.at(i), 5.0f);
    }
}

void PhidgetTestApp::resize(ResizeEvent evt){
    rectangles.clear();
    for(int i=0;i<numSensors;i++){
        int Rw = (getWindowWidth()-2*margin-gutter*(numSensors-1))/(numSensors);
        int Rx = margin+Rw*i+gutter*(i);
        int Ry = getWindowHeight()-margin;
        int Rh = 0;
        rectangles.push_back(Rectf( Vec2f( Rx,Ry ), Vec2f( Rx+Rw,Ry+Rh ) ));
    }
    
}


CINDER_APP_BASIC( PhidgetTestApp, RendererGl )
