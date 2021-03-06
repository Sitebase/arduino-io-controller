# Home IO Controller

This project is a prototype that should eventually replace the PLC that I'm now using for doing all my home automation.

The only thing this component should do is controlling IO, meaning dispatching an event on the network when an input is received, and setting the states of outputs.
This functionality can then be made available over a API that is available on the network.

By keeping the controller so simple it's making it possible to write all the logic, of what switch should control what light, in a language/server/OS you prefer.

## Reason

* For the moment my house is running on a Beckhoff CX9000 PLC. Apart from the fact that it's running Window CE you are locked into a horrible old IDE that only runs "well" on
a Window XP system.
* I like to build my own stuff :)

## Goals

* First goal: create a fixed example that uses an input and toggles an output, this simple implementation is just a test to see how reliable this runs 24/7
* Listen for events on inputs, for this first prototype I will be using polling because the Arduino UNO I'm using has only a few interrupts
* Distribute events over the network using MQTT
* Set output states using MQTT
* Detect different types of events: click, double click, hold, ...

## Resources

* [Button click types detect](http://forum.arduino.cc/index.php?topic=14479.0)

## Raspberry Pi

Configure to send all syslog messages to main server. To do this edit `/etc/rsyslog.conf`

    *.* 192.168.1.117:514

 Restart service after edit:

    sudo service rsyslog restart
