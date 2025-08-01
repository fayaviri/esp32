const aedes = require('aedes')();
const net = require('net');

const PORT = 1883;

const server = net.createServer(aedes.handle);

server.listen(PORT, () => {
  console.log(`🚀 Servidor MQTT escuchando en el puerto ${PORT}`);
});

/*
te han depositado
has recibido
boliviano
bolivianos
centavo
centavos
con
y
*/