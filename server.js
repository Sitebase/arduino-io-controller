/**
 * this server is included for testing reason and will
 * act as the mqtt message broker
 *
 * make sure to install `mosca` first before trying to run this script
 * npm install mosca
 * node server.js
 */
var mosca = require('mosca');

//var server = new mosca.Server({ port: 1883, logger: {level: 'debug'} });
var server = new mosca.Server({ port: 1883, keepalive: 5000000 });

server.on('clientConnected', function(client) {
    console.log('client connected', client.id);
});

server.on('published', function(packet, client) {
    console.log('published', packet.payload.toString());
});

server.on('ready', function() {
    console.log('mqtt server is running');
});
