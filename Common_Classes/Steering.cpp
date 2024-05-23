#include "Steering.h"
#include "math.h"

Steering::Steering() // @suppress("Class members should be properly initialized")
{

}
Steering::~Steering() // @suppress("Class members should be properly initialized")
{

}
void Steering::init(System *sys, uint32_t baseX, uint32_t baseY, uint32_t sampleSeqX, uint32_t sampleSeqY, uint32_t analogInX, uint32_t analogInY)
{
    // Initialize the ADC objects to read the joystick values
    xValue.init(sys, baseX, sampleSeqX, analogInX);
    yValue.init(sys, baseY, sampleSeqY, analogInY);
}
void Steering::calcValue()
{
    /*
     * The joystick values given by the UART Communication are between -100 and 100.
     * Since the joystick is returns small values even when its centered, these values are
     * grounded to zero.
     * Furthermore, the x axis is inverted in comparison to the intuition.
     */

    //TODO: Put your code here -> A6.1
}
float Steering::getLeftSpeed()
{
    //TODO: Put your code here -> A6.1
}
float Steering::getRightSpeed()

{
    //TODO: Put your code here -> A6.1
}
float Steering::getXValue()
{
    //TODO: Put your code here -> A6.1
}
float Steering::getYValue()
{
    //TODO: Put your code here -> A6.1
}
