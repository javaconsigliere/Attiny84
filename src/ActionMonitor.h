#ifndef XLOGISTX_ACTION_MONITOR_h
#define XLOGISTX_ACTION_MONITOR_h

#include <Arduino.h>


class ActionMonitor {
protected:
  uint8_t outputPin;  // The output pin number.
  uint8_t mainInput;
  uint8_t triggerInput;

public:
  /**
   * Constructor:
   * @param outPin The output pin number.
   * @param inPins An initializer_list of input pin numbers.
   */
   ActionMonitor(uint8_t outPin, uint8_t inMain, uint8_t inTrigger);



  /**
   * Virtual function to monitor the input pins.
   * Default behavior: if any input pin reads HIGH,
   * the output pin is set HIGH; otherwise, it is set LOW.
   * This function can be overridden in a derived class for custom behavior.
   */
  virtual void monitor();


};



#endif