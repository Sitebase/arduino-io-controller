var mqtt = require('mqtt')
  , host = 'localhost' // or localhost
  //, client = mqtt.connect();
  , client = mqtt.connect({ port: 1883, host: host, keepalive: 10000});

client.subscribe('presence');
client.subscribe('state');
client.publish('presence', 'bin hier');
client.on('message', function (topic, message) {
  console.log('received', topic, message.toString());
});

setTimeout(function() {
    //client.publish('')
}, 1000);
