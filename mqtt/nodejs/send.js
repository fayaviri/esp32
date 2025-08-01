const mqtt = require('mqtt');
// const client = mqtt.connect('mqtt://localhost:1883');
const client = mqtt.connect('mqtt://192.168.100.191:1883');

const topic = 'noti/numero';
// const valor = 45.75; // Cambia el número que desees enviar
// const valor = 45.46; // Cambia el número que desees enviar
const valor = 98.70; // Cambia el número que desees enviar

client.on('connect', () => {
  console.log('✅ Conectado al broker');
  client.publish(topic, valor.toString(), () => {
    console.log(`📤 Enviado: ${valor}`);
    client.end();
  });
});
