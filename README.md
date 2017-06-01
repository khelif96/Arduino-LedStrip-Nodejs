# Arduino-LedStrip-Nodejs
This Code allows you to control a Radioshack Tri-Color led strip through a web-app using a arduino and a computer like the raspberry pi that acts as a server.

## To Download
git clone https://github.com/khelif96/Arduino-LedStrip-Nodejs.git
This will create a folder called Arduino-LedStrip-Nodejs

## Set up
Connect the led strip to the Arduino the black cable goes to ground, red cable goes to vin, green cable goes to pin A0
The led strip requires an external power source so connect a 12v dc power supply to the arduino
Connect the arduino to the computer through the usb port

## To Install
Open the code in the folder called Arduino_code in the arduino ide and upload that to the arduino

In the folder named Arduino-LedStrip-Nodejs open the file named server.js in a text editor and make any modifications to the file such as desired port to run the sever on and the Serial port the arduino is connected to.

Run npm install

## To Run
Run nodejs server.js or npm start in the command line to start the server
In your browser navigate to your devices ipaddress:port and you should see the controls for the led strip
