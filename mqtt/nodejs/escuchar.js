const mqtt = require('mqtt');
// const client = mqtt.connect('mqtt://localhost:1883');
const client = mqtt.connect('mqtt://192.168.100.191:1883');

const topic = 'noti/numero';

client.on('connect', () => {
  console.log(`👂 Escuchando el topic: ${topic}`);
  client.subscribe(topic);
});

client.on('message', (topic, message) => {
  console.log(`📥 Recibido: ${message.toString()}`);
});
