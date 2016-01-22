var socket = io.connect();

// var speedBar = document.getElementById('speedBar');
function changeSpeed(){
	document.getElementById('speed_value').innerHTML = document.getElementById('speedBar').value;
	send('speed:'+ document.getElementById('speedBar').value);

}

// document.getElementById("solid").addEventListener("click", send('solid'));
function send(input){

    // var colorInput = document.getElementById('colorChoice');
		var inputValue = input.split(":");
		var temp = "";
		if(inputValue[0] == "color"){
		if(inputValue[1]=="red"){
			temp = "#D9534F";
		}else if (inputValue[1]=="green") {
			temp = "#5CB85C";
		}else if(inputValue[1] == "yellow"){
			temp = "#F0AD4E";
		}else if(inputValue[1] == "blue"){
			temp = "#337AB7";
		}else if(inputValue[1] == "off"){
			temp = "#000000";
		}
		document.getElementById('title').style.color=temp;

	}
		console.log("Sending : " + input)
    socket.emit('input',{value:input});
}
