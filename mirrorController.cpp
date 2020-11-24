#include "mirrorController.h"


MirrorController::MirrorController(const int pinDIR, const int pinPU, const int pinMF)
{
    _pinDir = pinDIR;
    _pinPU = pinPU;
    _pinMF = pinMF;
    _direction = true; //clockwise
    _currentStep = 0;
    _stepsPerRevolution = 800; //default value
    _lastStep = 0;
}

void MirrorController::init()
{
    pinMode(_pinDir, OUTPUT);
    pinMode(_pinPU, OUTPUT);
    pinMode(_pinMF, OUTPUT);
    digitalWrite(_pinMF, HIGH); // activate driver
}

void MirrorController::setSpeed(int speedRPM)
{
    if(_speedRpm != speedRPM)
    {
        _stepInterval = (long) 60 * 1000000 / ((long)speedRPM * _stepsPerRevolution);
        _speedRpm = speedRPM;
    }
}

void MirrorController::setOffset(double offsetDegrees)
{
    if(_offset != offsetDegrees)
    {
        _offsetSteps = (offsetDegrees * _stepsPerRevolution) / 360; // offset in steps
        _offset = offsetDegrees;
    }
}

void MirrorController::setAmplitude(double amplitudeDegrees)
{
    if(_amplitude != amplitudeDegrees)
    {
        _amplitudeSteps = (amplitudeDegrees * _stepsPerRevolution) / 360; // amplitude in steps
        _amplitude = amplitudeDegrees;
    }
}

void MirrorController::setPhase(int phaseDegrees)
{
    if(_phase != phaseDegrees)
    {
        if(phaseDegrees < 0)
        {
            _phase = 0;
        }
        else
        {
            _phase = phaseDegrees%360;
            _phaseSteps = (long)_phase * _amplitudeSteps / 180;
            _phaseTime = _phaseSteps * _stepInterval;
        }
    }
}

int MirrorController::getSpeed()
{
    return (_stepInterval * _stepsPerRevolution) / (60.0 * 1000000.0);
}

double MirrorController::getAmplitude()
{
    return (double)_amplitudeSteps * 360 / _stepsPerRevolution;
}

double MirrorController::getOffset()
{
    return (double)_offsetSteps * 360 / _stepsPerRevolution;
}

int MirrorController::getPhase()
{
    return _phase;
}

void MirrorController::setStepsPerRevolution(int stepsPerRevolution)
{
    _stepsPerRevolution = stepsPerRevolution;
}

void MirrorController::runMovement()
{
    if(_lastStep == 0)
    {
        Serial.println(String(micros()) + "\tStart");
        _lastStep = micros();
    }

    if(_direction) //clockwise, we are going up
    {
        if(_makeOneStep())
        {
            _currentStep++;
        }

        if(_currentStep >= _offsetSteps + _amplitudeSteps / 2) // we reached maximum point
        {
            _setDirection(false); //going the other way round
        }
    }
    else //counter clockwise, we are going down
    {
        if(_makeOneStep())
        {
            _currentStep--;
        }

        if(_currentStep <= _offsetSteps - _amplitudeSteps / 2)
        {
            _setDirection(true); //going the other way round
        }
    }
}

void MirrorController::stopMovement()
{
    int incr = 0;
    if(_currentStep <= _offsetSteps)
    {
        _setDirection(true);
        incr = 1;
    }
    else
    {
        _setDirection(false); 
        incr = -1;
    }
    
    while(_currentStep != _offsetSteps)
    {
        if(_makeOneStep())
        {
            _currentStep += incr;
        }
    }

    // reinit setup
    _direction = true; //clockwise
    _currentStep = 0;
    _lastStep = 0;
}

void MirrorController::runStepsBlocking(int numberOfSteps, bool direction)
{
    _setDirection(direction);
    _currentStep = 0;
    while(_currentStep < numberOfSteps)
    {
        if(_makeOneStep())
        {
            _currentStep++;
        }
    }
}

bool    MirrorController::_makeOneStep()
{
    if(micros() - _lastStep > _stepInterval + _phaseTime)
    {
        digitalWrite(_pinPU, LOW);
        _lastStep = micros() - _phaseTime;
        delayMicroseconds(10);
        digitalWrite(_pinPU, HIGH);
        //Serial.println(String(micros()) + "\t" + String(_currentStep));
        return true;
    }
    return false;
}

void    MirrorController::_setDirection(bool direction)
{
    digitalWrite(_pinDir, direction);
    _direction = direction;
}

void MirrorController::_initPhase()
{
    if(_phase < 90 || _phase >= 270)
    {
        _direction = true;

    }
}
