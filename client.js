var mqtt = require('mqtt')
  , host = 'localhost' // or localhost
  //, client = mqtt.connect();
  , client = mqtt.connect({ port: 1883, host: host, keepalive: 10000});

client.subscribe('presence');
client.subscribe('state');
client.subscribe('/state/sensor');
client.publish('presence', 'bin hier');
client.on('message', function (topic, message) {
  console.log('received', topic, message.toString());
});

setInterval(function() {
    console.log('WRITE');
    var cmd = {c: 0, cmd: 'dr'};
    client.publish('command', JSON.stringify(cmd));
}, 5000);
