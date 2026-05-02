
IOT I2C Controller:: I2C slave utility library
==========================================================================



## PROJECT INFORMATION



## LICENSE
 
 
 Licensed under the Apache License Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 https://www.apache.org/licenses/LICENSE-2.0
  
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

## RELEASES

<ul> Versions
<li> 1.07.00
</ul>

## Description
This project is an I2C controller for atmel microcontrollers.
It set the controller in I2C slave mode and allows to master
to provision and control the remote system via I2C.
There is java project that uses raspberry pi that enables web api
to send and read ios on remote atmel devices vi I2C protocol.

## Supported MCPU
<ul>
<li>Attiny84</li>
<li>Attiny85</li>
<li>Atmega328</li>
</ul>

## Features

The firmware exposes a small command-oriented protocol over I2C. The master
issues a command code (optionally followed by parameters), and the slave either
performs an action or returns a value on the next read. Default I2C slave
address is `0x09` and can be changed at runtime.

### Core I2C protocol
<ul>
<li><b>Command dispatcher</b> &mdash; pluggable <code>CommandManager</code> registers handlers per command code and routes incoming I2C requests to them.</li>
<li><b>32-byte command buffer</b> &mdash; matches the Wire library payload size; commands are parsed with offset/length so multi-byte parameters (int, long, string) can be decoded.</li>
<li><b>Get / Set / Update actions</b> &mdash; each command supports action verbs (<code>A_G</code>, <code>A_S</code>, <code>A_U</code>, <code>A_D</code>, <code>A_P</code>) so a single code can both read and write.</li>
<li><b>Runtime-reconfigurable slave address</b> &mdash; the master can rewrite the device's I2C address (range 10&ndash;127); the new address takes effect after restart.</li>
</ul>

### Built-in commands
<ul>
<li><b>Version</b> &mdash; returns device model (<code>84-I2C</code>, <code>85-I2C</code>, or <code>328-I2C</code>) and firmware version string.</li>
<li><b>Ping</b> &mdash; heartbeat with auto-incrementing counter, useful for liveness checks.</li>
<li><b>I2C message counter</b> &mdash; total count of I2C transactions handled since boot.</li>
<li><b>Uptime</b> &mdash; milliseconds since boot (accuracy is impacted by I2C library timing).</li>
<li><b>CPU speed</b> &mdash; reports the configured F_CPU in Hz.</li>
<li><b>Pin I/O</b> &mdash; configure a pin as analog or digital, then read or write its value over I2C.</li>
<li><b>Analog reference</b> &mdash; switch the ADC reference between <code>DEFAULT</code>, <code>INTERNAL</code>, and <code>EXTERNAL</code>, including a hook for an on-board precision voltage source (<code>AREF_VOLTAGE = 4.092 V</code>).</li>
<li><b>Reset</b> &mdash; software-triggered hardware reset via a dedicated reset pin (ATtiny84 only; ATtiny85 has no spare pin for this).</li>
</ul>

### Per-MCU features
<ul>
<li><b>ATtiny84</b> &mdash; full command set including hardware reset on <code>PB1</code>; uses a 1&nbsp;ms loop delay to work around I2C timing quirks.</li>
<li><b>ATtiny85</b> &mdash; adds the <code>ActionMonitor</code> background task: polls a main input pin and a trigger input pin, drives an output pin high when the main input is high and the trigger is low, with a configurable post-action delay (default 500&nbsp;ms). Wired to <code>PB1</code> (out), <code>PB3</code> (main), <code>PB4</code> (trigger).</li>
<li><b>ATmega328P</b> &mdash; standard command set; intended for Arduino Nano boards using the USB bootloader rather than ISP.</li>
</ul>

### Build &amp; toolchain
<ul>
<li><b>PlatformIO project</b> &mdash; <code>platformio.ini</code> defines three environments (<code>attiny84</code>, <code>attiny85</code>, <code>nanoatmega328</code>) sharing common ISP upload settings; the Nano environment overrides the protocol to use the Arduino USB bootloader.</li>
<li><b>Single source tree, multi-target builds</b> &mdash; per-MCU code paths are guarded by <code>__AVR_ATtiny84__</code> / <code>__AVR_ATtiny85__</code> / <code>__AVR_ATmega328P__</code>, so the same firmware sources compile for all three targets.</li>
</ul>

### Companion Raspberry Pi gateway
<ul>
<li>A separate Java project runs on a Raspberry Pi acting as the I2C master and exposes a web API, letting remote clients read and write IOs on the AVR slaves over HTTP.</li>
</ul>


## KNOWN ISSUES
<ul>
<li>Uptime values can drift on the ATtiny targets &mdash; the I2C library's interrupt timing affects the millis() counter.</li>
</ul>


## IN DEVELOPMENT
