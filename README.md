# Home IO Controller

This project is a prototype that should eventually replace the PLC that I'm now using for doing all my home automation.

The only thing this component should do is controlling IO, meaning dispatching an event on the network when an input is received, and setting the states of outputs.
This functionality can then be made available over a API that is available on the network.

By keeping the controller so simple it's making it possible to write all the logic, of what switch should control what light, in a language/server/OS your prefer.

## Reason

* For the moment my house is running on a Beckhoff CX9000 PLC. Apart from the fact that it's running Window CE you are locked into a horrible old IDE that only runs well on
a Window XP system.
* I like to build my own stuff :)

## Develop

    node server.js | bunyan
    ./build
    /usr/local/Cellar/mosquitto/1.3.5/sbin/mosquitto -p 1883 -v
    node client.js

## Messages

* val (`digital read`): Values of the digital inputs configured as inputs
* c (`channel`): Each IO controller has it's own channel so that you know from what controller you receive the input. While sending a command this can also be used to target a specific IO controller.
* val[0-5] (`analog read`): analog readed values
* cmd (`command`): execute a command. e.g. dr, ar, ds

a `dr` command is both triggered by an input change or by sending the `dr` command. The latter case can be used to retrieve the current state of the digital input pins.

|  Topic  |                Message                 |                 Description                  |
| ------- | -------------------------------------- | -------------------------------------------- |
| state   | {"c":0,"cmd":"dr","val":8}             | State of the digital inputs                  |
| state   | {"c":0,"cmd":"ar","val0":200,"val0":2} | Values of the analoge inputs                 |
| command | {"c":0,"cmd":"dr"}                     | Command to request digital input values      |
| command | {"c":0,"cmd":"ar"}                     | Command to request analoge input values      |
| command | {"c":0,"cmd":"ds","v":1,"p":13}        | Command to set the digital output 13 to high |

## Goals

* Listen for events on inputs, for this first prototype I will be using polling because the Arduino UNO I'm using has only a few interrupts
* Distribute events over the network using MQTT
* Set output states using MQTT
* Detect different types of events: click, double click, hold, ...

## Resources

* [Button click types detect](http://forum.arduino.cc/index.php?topic=14479.0)

## Todo

* Publish from node client is not received by the arduino, while mosquitto does receive it and sends it off to the arduinoClient
