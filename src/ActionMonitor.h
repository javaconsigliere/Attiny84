#ifndef XLOGISTX_ACTION_MONITOR_h
#define XLOGISTX_ACTION_MONITOR_h

#include <Arduino.h>
#include <DataTypes.h>


class CActionMonitor: public Runnable {
protected:
  uint8_t outputPin;  // The output pin number.
  uint8_t mainInput;
  uint8_t triggerInput;
  bool currentState = false;
  unsigned long postDelay;
public:
  /**
   * Constructor:
   * @param extraDelay extra delay to add after execution
   * @param outPin The output pin number to control pump.
   * @param inMain The main pin if low monitor will return 0.
   * @param inTrigger in low and inMain is high monitor will set outPin high and return extraDelay, otherwise return 0 and outPin is low
   */
   CActionMonitor(unsigned long postDelay, uint8_t outPin, uint8_t inMain, uint8_t inTrigger);

  int run();

  /**
   * Virtual function to monitor the inMai and inTrigger.
   * Default behavior: if inMain && !intrigger is high set outPin on and return extra delay
   * otherwise outPin off and return 0
   * @delay 
   * @return true if trigger activated
   */
  bool monitor(unsigned long delay);

  void setOutPin(boolean state);
  bool getOutPin();

  bool getCurrentState();


};


extern CActionMonitor *ActionMonitor;
#endif