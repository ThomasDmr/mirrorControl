// mirrorController.h

#ifndef __MIRROR_CONTROLLER__
#define __MIRROR_CONTROLLER__

#include "Arduino.h"

class MirrorController 
{
    public:

    MirrorController(const int pinDIR, const int pinPU, const int pinMF);

    void init();

    void setSpeed(int speed);
    void setOffset(double offsetDegrees);
    void setAmplitude(double amplitudeDegrees);
    void setStepsPerRevolution(int stepsPerRevolution);
    void setPhase(int phaseDegrees);

    int     getSpeed();
    double  getAmplitude();
    double  getOffset();
    int     getPhase();

    void runMovement();
    void stopMovement();

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
