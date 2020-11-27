#include "mirrorController.h"

// Initialize the mirror controller class : pinDIR, pinPU, pinMF
MirrorController mirrorX(3, 4, 5);
MirrorController mirrorY(8, 9, 10);

void setup()
{
  Serial.begin(115200);
  //while(!Serial){}

  pinMode(12, INPUT_PULLUP); //pin used to start/stop the movement
  
  mirrorX.setStepsPerRevolution(1600);
  mirrorX.setSpeed(40);
  mirrorX.setAmplitude(10);
  mirrorX.setOffset(0);
  mirrorX.setPhase(0);
  //mirrorX.setWaveFormSinus();
  mirrorX.init();

  mirrorY.setStepsPerRevolution(1600);
  mirrorY.setSpeed(50);
  //mirrorY.setAmplitude(10);
  //mirrorY.setOffset(0);
  //mirrorX.setPhase(0);
  mirrorY.init();

  Serial.println("Amplitude " + String(mirrorX.getAmplitude()));
  Serial.println("Speed " + String(mirrorX.getSpeed()));
  Serial.println("Offset " + String(mirrorX.getOffset()));

}

double valueA1 = 0;
double valueA2 = 0;
uint32_t timer = 0;

int incr = 1;
bool dir = 1;
void loop()
{
  valueA1 = analogRead(A1);
  double cote = (valueA1 / 1023) * 40;

  Serial.println(cote);
  
  squareDraw(cote);

  if(incr < 10 && dir==1)
  {
    mirrorX.runStepsBlocking(10, 1);
    incr++;
  }
  else if(incr == 10 && dir ==1)
  {
    mirrorX.runStepsBlocking(10, 1);
    dir = 0;
  }
  else if(incr > 0 && dir == 0)
  {
    mirrorX.runStepsBlocking(10, 0);
    incr--;
  }
  else if(incr == 0 && dir ==0)
  {
    mirrorX.runStepsBlocking(10, 0);
    dir = 1;
  }
  /*
  if(millis() - timer > 100)
  {
    timer = millis();
    valueA1 = analogRead(A1);
    valueA2 = analogRead(A2);
    double amp = (valueA1 / 1023 - 0.5) * 20;
    double phi = valueA2 / 1023 * 360;

    //Serial.println(phi);
    mirrorY.setOffset(amp);
    mirrorX.setPhase(phi);
  }
  

  if(digitalRead(12))
  {
    mirrorX.runMovement();
    mirrorY.runMovement();
  }
  else
  {
    mirrorX.stopMovement();
    mirrorY.stopMovement();
  }
  */
  
}

void squareDraw(int cote)
{
  mirrorX.runStepsBlocking(cote, 1);
  mirrorY.runStepsBlocking(cote, 1);
  mirrorX.runStepsBlocking(cote, 0);
  mirrorY.runStepsBlocking(cote, 0);
}


void squareDraw2(int cote)
{
  for(int i=0; i<cote; i++)
  {
      mirrorX.runStepsBlocking(1, 1);
      mirrorY.runStepsBlocking(1, 1);
  }
  for(int i=0; i<cote; i++)
  {
      mirrorX.runStepsBlocking(1, 0);
      mirrorY.runStepsBlocking(1, 1);
  }
  for(int i=0; i<cote; i++)
  {
      mirrorX.runStepsBlocking(1, 0);
      mirrorY.runStepsBlocking(1, 0);
  }
  for(int i=0; i<cote; i++)
  {
      mirrorX.runStepsBlocking(1, 0);
      mirrorY.runStepsBlocking(1, 1);
  }
}
