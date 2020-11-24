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

    int     getSpeed();
    double  getAmplitude();
    double  getOffset();

    void runMovement();
    void stopMovement();

    void runStepsBlocking(int numberOfSteps, bool direction);


    private:

    bool        _makeOneStep();
    void        _setDirection(bool direction);

    int         _currentStep;
    bool        _direction;
    int         _pinDir;
    int         _pinPU;
    int         _pinMF;

    int         _stepsPerRevolution;
    int         _offset;
    int         _amplitude;

    uint32_t    _stepInterval;
    uint32_t    _lastStep;

};

#endif
