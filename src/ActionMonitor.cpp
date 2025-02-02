#include<ActionMonitor.h>
ActionMonitor::ActionMonitor(uint8_t outPin, uint8_t inMain, uint8_t inTrigger)
{
    outputPin = outPin;
    mainInput = inMain;
    triggerInput = inTrigger;

    // Configure the output pin.
    pinMode(outputPin, OUTPUT);
    digitalWrite(outputPin, HIGH);
    pinMode(mainInput, INPUT_PULLUP);
    pinMode(triggerInput, INPUT_PULLUP);
}
void ActionMonitor::monitor() {
    bool mainValue = digitalRead(mainInput);
    bool triggerValue = digitalRead(triggerInput);

    digitalWrite(outputPin, (mainValue  && !triggerValue) ? HIGH : LOW);
  }
