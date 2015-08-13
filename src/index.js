var mqtt    = require('mqtt');
var express = require('express');
var Gpio = require('onoff').Gpio;
var config = require('../package.json');

/**********************
 * MQTT
 *********************/

var client  = mqtt.connect('mqtt://192.168.1.117');

client.on('connect', function () {
  client.subscribe('testing');
  client.publish('testing', 'Hello mqtt');
});

client.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString());
});

function getMQTTClient()
{
    return client.connected ? client : null;
}

/**********************
 * GPIO
 *********************/

var button1 = new Gpio(23, 'in', 'falling');
var button2 = new Gpio(24, 'in', 'falling');
var led1 = new Gpio(27, 'out');
var led2 = new Gpio(22, 'out');
var state1 = 0;
var state2 = 0;

button1.watch(function(err, value) {
    var mqttClient = getMQTTClient();
    console.log('Button 1 pressed', value);
    mqttClient.publish('io/button1', value);

    state1 = !state1;
    console.log('Set state of led1 to', state1);
    led1.writeSync(state1 ? 1 : 0);
});

button2.watch(function(err, value) {
    var mqttClient = getMQTTClient();
    console.log('Button 2 pressed', value);
    mqttClient.publish('io/button2', value);

    state2 = !state2;
    console.log('Set state of led2 to', state2);
    led2.writeSync(state2 ? 1 : 0);
});

/**********************
 * API
 *********************/

var app = express();

app.get('/', function (req, res) {
    res.json({ name: config.name, version: config.version });
});

app.get('/publish/:topic/:value', function (req, res) {
    var mqttClient = getMQTTClient();
    mqttClient.publish(req.params.topic, req.params.value);
    res.json({ success: true });
});

var server = app.listen(3333, function () {
  var host = server.address().address;
  var port = server.address().port;

  console.log(config.name + ' listening at http://%s:%s', host, port);
});

// gracefully exit
process.on( "SIGINT", function() {
    console.log('CLOSING [SIGINT]');
    client.end();
    process.exit();
} );
