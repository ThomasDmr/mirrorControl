#include "mirrorController.h"


MirrorController::MirrorController(const int pinDIR, const int pinPU, const int pinMF)
{
    _pinDir = pinDIR;
    _pinPU = pinPU;
    _pinMF = pinMF;
    _direction = true; //clockwise
    _currentStep = 0;
    _stepsPerRevolution = 800; //default value
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
    _stepInterval = (long) 60 * 1000000 / ((long)speedRPM * _stepsPerRevolution);
    Serial.println(_stepInterval);
}

void MirrorController::setOffset(double offsetDegrees)
{
    _offset = (offsetDegrees * _stepsPerRevolution) / 360; // offset in steps
}

void MirrorController::setAmplitude(double amplitudeDegrees)
{
    _amplitude = (amplitudeDegrees * _stepsPerRevolution) / 360; // amplitude in steps
}

int MirrorController::getSpeed()
{
    return (_stepInterval * _stepsPerRevolution) / (60.0 * 1000000.0);
}

double MirrorController::getAmplitude()
{
    return (double)_amplitude * 360 / _stepsPerRevolution;
}

double MirrorController::getOffset()
{
    return (double)_offset * 360 / _stepsPerRevolution;
}

void MirrorController::setStepsPerRevolution(int stepsPerRevolution)
{
    _stepsPerRevolution = stepsPerRevolution;
}

void MirrorController::runMovement()
{
    if(_direction) //clockwise, we are going up
    {
        if(_makeOneStep())
        {
            _currentStep++;
        }

        if(_currentStep >= _offset + _amplitude / 2) // we reached maximum point
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

        if(_currentStep <= _offset - _amplitude / 2)
        {
            _setDirection(true); //going the other way round
        }
    }
}

void MirrorController::stopMovement()
{
    int incr = 0;
    if(_currentStep <= _offset)
    {
        _setDirection(true);
        incr = 1;
    }
    else
    {
        _setDirection(false); 
        incr = -1;
    }
    
    while(_currentStep != _offset)
    {
        if(_makeOneStep())
        {
            _currentStep += incr;
        }
    }
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
    if(micros() - _lastStep > _stepInterval)
    {
        digitalWrite(_pinPU, LOW);
        _lastStep = micros();
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
