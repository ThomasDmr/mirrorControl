#include "mirrorController.h"

// Initialize the mirror controller class : pinDIR, pinPU, pinMF
MirrorController mirrorX(8, 9, 10);
MirrorController mirrorY(3, 4, 5);

void setup()
{
  Serial.begin(115200);

  pinMode(12, INPUT_PULLUP); //pin used to start/stop the movement
  
  mirrorX.setStepsPerRevolution(1600);
  mirrorX.setSpeed(40);
  mirrorX.setAmplitude(8.5);
  mirrorX.setOffset(0);
  mirrorX.init();

  mirrorY.setStepsPerRevolution(1600);
  mirrorY.setSpeed(40); // !! on dirait que 40 rpm = speed max
  mirrorY.setAmplitude(5);
  mirrorY.setOffset(0);
  mirrorY.init();

  Serial.println("Amplitude " + String(mirrorX.getAmplitude()));
  Serial.println("Speed " + String(mirrorX.getSpeed()));
  Serial.println("Offset " + String(mirrorX.getOffset()));

}

double valueA1 = 0;
double valueA2 = 0;
uint32_t timer = 0;

void loop()
{
  valueA1 = analogRead(A1);
  valueA2 = analogRead(A2);
  double amp = valueA1 / 1023 * 20;
  double rpm = valueA2 / 1023 * 60;
  
  if(millis() - timer > 20)
  {
    timer = millis();
    //Serial.println(amp);
    mirrorY.setSpeed(rpm);
  }
  mirrorX.setAmplitude(amp);
  

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
  
}
