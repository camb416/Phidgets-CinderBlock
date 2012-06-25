//
//  PhidgetModel.h
//  PhidgetTest
//
// Created by Cameron Browning on 6/20/12.
// Copyright (c) 2012 Cameron Browning
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// 
// 

#ifndef PhidgetTest_PhidgetModel_h
#define PhidgetTest_PhidgetModel_h

#include <iostream>
using namespace std;

class IFKitModel{
public:
    IFKitModel(int serial_in, int numSensors_in);
    ~IFKitModel();
    
    int getNumSensors();
    int getSerial();
    int setSensorVal(int whichSensor, int whatValue);
    int getSensorVal(int whichSensor);
    bool getSensorBool(int whichSensor);
    void print();

private:
    int numSensors;
    int serial;
    int * sensors;
};

// get a value as an int
int IFKitModel::getSensorVal(int whichSensor){

    if(whichSensor>-1 && whichSensor<numSensors) {
        return sensors[whichSensor];
    } else {
        return -1;
    }
    
}

// get value as a bool
bool IFKitModel::getSensorBool(int whichSensor){
    if(getSensorVal(whichSensor)>500){
        return true;
    } else {
        return false;
    }
}

// log all the values
void IFKitModel::print(){
    cout << "sensor id: " << serial << endl;
    for(int i=0;i<numSensors;i++){
        
        sensors[i]>-1 ? cout << i << ": " << sensors[i] << endl : cout << i << ": no data received (yet)." << endl;
    }
}
// get the serial number
int IFKitModel::getSerial(){
    return serial;
}

// constructor
IFKitModel::IFKitModel(int serial_in, int numSensors_in){
    numSensors = numSensors_in;
    sensors = new int[numSensors];
    serial = serial_in;
    for(int i=0;i<numSensors;i++){
        sensors[i] = -1;
    }
}
// set the value of the sensor (in the model.. not on the hardware)
int IFKitModel::setSensorVal(int whichSensor, int whatValue){
   /* 
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Current local time and date: %s", asctime (timeinfo) );
     */  
    if(whichSensor<numSensors && whichSensor>=0){
        sensors[whichSensor] = whatValue;
       // cout << "set sensor " << whichSensor << " to: " << whatValue << "." << endl;
        return 0;
    } else {
        cout << "error setting sensor model value" << endl;
        return -1;
        
    }
}

// return the number of sensors
int IFKitModel::getNumSensors(){
    return numSensors;
}


#endif
