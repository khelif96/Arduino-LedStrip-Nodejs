var socket = io.connect();

// var speedBar = document.getElementById('speedBar');
function changeSpeed(){
	send('speed:'+ document.getElementById('speedBar').value)
}

// document.getElementById("solid").addEventListener("click", send('solid'));
function send(input){
    // var colorInput = document.getElementById('colorChoice');
    socket.emit('input',{value:input});
}
