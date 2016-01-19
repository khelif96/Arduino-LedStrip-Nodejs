var socket = io.connect();

$('#ex1').slider({
	formatter: function(value) {
		return 'Current value: ' + value;
	}
});

// Without JQuery
var slider = new Slider('#ex1', {
	formatter: function(value) {
		return 'Current value: ' + value;
	}
});

// document.getElementById("solid").addEventListener("click", send('solid'));
function send(input){
    // var colorInput = document.getElementById('colorChoice');
    socket.emit('input',{value:input});
}
