// mirrorController.h
/**
 * MirrorController is a stepper motor controller class designed to control stepper motors that are combined with mirrors
 * in order to make a simple 'laser show'. 
 * This class creates a sawtooth oscillation and allows you to control the speed, amplitude offset and phase of the oscillation. 
 * Each parameter can be controlled in real time (using external sensors) which allow for a smooth transition and creative movements.
 * 
 * 
 */

#ifndef __MIRROR_CONTROLLER__
#define __MIRROR_CONTROLLER__

#include "Arduino.h"

class MirrorController 
{
    public:

    /**
     * MirrorController : constructor allowing to define the stepper driver pins
     * @param pinDIR pin number of the DIR output 
     * @param pinPU pin number of the PU or STEP output
     * @param pinMF pin number of the MF or ENABLE output (you can also not connect the MF pin)
     */
    MirrorController(const int pinDIR, const int pinPU, const int pinMF);

    /**
     * Init : initialize the pins and library
     * Needs to be called after the constructor in the setup() function
    */
    void init();

    /**
     * setSpeed : set the rotation speed of the axle
     * @param speed in Rotation per Minute (RPM), integers only
    */
    void setSpeed(int speed);

    /**
     * setOffset : set the relative offset of the oscillation. 
     * Offset 0 corresponds to the position of the axle when the system was turned on. 
     * @param offsetDegrees offset angle in degrees, double type
    */
    void setOffset(double offsetDegrees);

    /**
     * setAmplitude : set the amplitude of the oscillation. 
     * @param amplitudeDegrees amplitude angle in degrees, double type
    */
    void setAmplitude(double amplitudeDegrees);

    /**
     * setPhase : set the phase of the oscillation. The phase corresponds to at which point the oscillation starts
     * @param phaseDegrees integer value between 0 and 360 degrees. 
    */
    void setPhase(int phaseDegrees);

    /**
     * setStepsPerRevolution : set the number of steps the stepper motor needs to make for an entire revolution 
     * @param stepsPerRevolution number of steps per revolution (default value is 800)
    */
    void setStepsPerRevolution(int stepsPerRevolution);

    /**
     * getSpeed : get the current speed setting 
     * @return integer value corresponding to the set speed in RPM
    */
    int     getSpeed();

    /**
     * getAmplitude : get the current amplitude setting 
     * @return double value corresponding to the set amplitude in degrees
    */
    double  getAmplitude();

    /**
     * getOffset : get the current offset setting 
     * @return double value corresponding to the set offset in degrees
    */
    double  getOffset();

    /**
     * getPhase : get the current phase setting 
     * @return integer value corresponding to the set phase in degrees
    */
    int     getPhase();

    /**
     * runMovement : this function needs to be called in the loop as many times as possible 
     * This is where the mirror control algorithm works. To work properly, the loop() needs to be as fast as possible. 
     * You should avoid time consuming functions inside the loop() or call this function inside a while() loop. 
    */
    void runMovement();

    /**
     * stopMovement : this function needs to be called in the loop to stop the movement.
     * It will bring the mirrors back to their offset position and make sure you don't accumulate undesired offsets.  
    */
    void stopMovement();


    /**
     * runStepsBlocking : blocking function just to test the stepper motor. This function just rotates the axle by a desired amount of steps.
     * @param numberOfSteps number of steps the mirror needs to rotate
     * @param direction boolean to determine the direction of rotation : true means clockwise, false means counter clockwise
    */
    void runStepsBlocking(int numberOfSteps, bool direction);


    private:

    bool        _makeOneStep();
    void        _setDirection(bool direction);
    void        _initPhase();

    int         _currentStep;
    bool        _direction;
    int         _pinDir;
    int         _pinPU;
    int         _pinMF;

    int         _stepsPerRevolution;
    double      _offset;
    int         _offsetSteps;
    double      _amplitude;
    int         _amplitudeSteps;
    int         _phase;
    int         _phaseSteps;
    int         _speedRpm; 

    uint32_t    _stepInterval;
    uint32_t    _phaseTime;
    uint32_t    _lastStep;

};

#endif
