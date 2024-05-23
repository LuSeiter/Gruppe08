#ifndef STEERING_H_
#define STEERING_H_

#include "System.h"
#include "ADC.h"

class Steering
{
public:
    Steering();
    virtual ~Steering();
    void init(System *sys, uint32_t baseX, uint32_t baseY, uint32_t sampleSeqX, uint32_t sampleSeqY, uint32_t analogInX, uint32_t analogInY);
    void calcValue();
    void calcValue2();
    float getRightSpeed();
    float getLeftSpeed();
    float getXValue();
    float getYValue();
private:
    System *steeringSys;
    ADC xValue, yValue;
    float rightSpeed, leftSpeed;
    bool dir = true;
    float hori,vert;
    uint16_t directioncounter = 0;
};



#endif /* STEERING_H_ */
