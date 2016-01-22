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
var speed = 100;
var color = "";
var mode = "solid";
var temp;
var initialPacket
server.listen(port);
app.use(express.static('public'));

console.log("Server Started on port " + port)

serialPort.on("open", function(){
  console.log('Serial Port Open');
});

io.sockets.on('connection', function(socket){
  console.log("Client Connected");

  // socket.emit('initialPacket',{value:initialPacket});

  socket.on('input', function(data){
      console.log("input: " + data.value);
      //
      // temp = data.value.split(":");
      //
      // if(temp[0] == "color"){
      //   color = temp[1];
      // }else if(temp[0] == "speed"){
      //   speed = temp[1];
      // }else if(temp[0] == "mode"){
      //   mode = temp[1];
      // }

      serialPort.write(data.value);


    })
});
