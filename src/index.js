var mqtt    = require('mqtt');
var express = require('express');
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
