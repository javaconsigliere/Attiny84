#include<ActionMonitor.h>
CActionMonitor::CActionMonitor(unsigned long postDelay, uint8_t outPin, uint8_t inMain, uint8_t inTrigger)
{
    outputPin = outPin;
    mainInput = inMain;
    triggerInput = inTrigger;
    this->postDelay = postDelay;


    // Configure the output pin.
    pinMode(outputPin, OUTPUT);
    digitalWrite(outputPin, LOW);
    
    // Configure the input pins.
    pinMode(mainInput, INPUT_PULLUP);
    pinMode(triggerInput, INPUT_PULLUP);
}


bool CActionMonitor::monitor(unsigned long toDelay)
{
    if (toDelay > 0)
        delay(toDelay);

    // figure out the current state    
    currentState = digitalRead(mainInput) && !digitalRead(triggerInput);
    

    // if current state is off but the output pin is on set to false
    if(!currentState && getOutPinState())
        setOutPinState(false);

    return getCurrentState();
}

int CActionMonitor::run()
{
    setOutPinState(getCurrentState());
    
    if (getCurrentState() && postDelay > 0)
        delay(postDelay);

    return currentState;
}

void CActionMonitor::setOutPinState(boolean state)
{
    digitalWrite(outputPin, state);
}

void CActionMonitor::setOutPin(uint8_t pin)
{
    outputPin = pin;
}
uint8_t CActionMonitor::getOutPin()
{
    return outputPin;
}

bool CActionMonitor::getOutPinState()
{
    return digitalRead(outputPin);
}

bool CActionMonitor::getCurrentState()
{
    return currentState;
}

uint8_t CActionMonitor::getInMainPin()
{
    return mainInput;
}
void CActionMonitor::setInMainPin(uint8_t pin)
{
    mainInput = pin;
}

uint8_t CActionMonitor::getInTriggerPin()
{
    return triggerInput;
}
void CActionMonitor::setInTriggerPin(uint8_t pin)
{
    triggerInput = pin;
}



CActionMonitor *ActionMonitor = NULL;
