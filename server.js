var express = require('express');
var app = express();
server = require('http').createServer(app);
io = require('socket.io').listen(server);

// SerialPort library
var SerialPort = require("serialport").SerialPort;
// Change /dev/ttyACM0 to which ever serialPort the arduino is connected too
var serialPort = new SerialPort("/dev/ttyACM0",{
  baudrate:9600
});

// Desired Port
var port = 3000;

server.listen(port);
app.use(express.static('public'));

console.log("Server Started on port " + port)

serialPort.on("open", function(){
  console.log('Serial Port Open');
});

io.sockets.on('connection', function(socket){
  console.log("Client Connected");

  socket.on('input', function(data){
      console.log("input: " + data.value);
      serialPort.write(data.value);


    })
});
