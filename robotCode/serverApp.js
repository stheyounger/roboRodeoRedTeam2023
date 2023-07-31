const http = require('http')
const fs = require('fs');
const os = require('os');

const WebSocket = require('ws');

const ffmpeg = require('ffmpeg');

const networkInterfaces = os.networkInterfaces();
const hostname = networkInterfaces['wlan0'][0]['address']
const port = 8080


console.log("Server program has started");

const root = (req, res)=>{
	const { headers, method, url } = req;
	if (req.method == 'POST') {
    		req.on('data', function(data) {
			res.writeHead(200, {'Content-Type': 'text/plain'})
			res.end("yup")

			//console.log("Server recived: " + data);
			handlePost(data)
		})
  	} else if (req.method == "GET") {
		res.writeHead(200, {'Content-Type': 'text/html'})
  		fs.createReadStream('webControl.html').pipe(res)
	}
}

const favicon = (req, res) => {
	const { headers, method, url } = req;
	res.writeHead(200, {'Content-Type': 'text/html'})
  	fs.createReadStream('image.png').pipe(res)
}

const server = http.createServer((req, res) => {
	const {url} = req;

	const paths = {
		"/":root,
		"/favicon.ico":favicon
	}

	paths[url](req, res)
})

server.listen(port, hostname, () => {
 	console.log(`Server running at http://`+hostname+`:`+port+`/`)
})

//var NodeWebcam = require( "node-webcam" );

const socket = new WebSocket.Server({ port: 8081 });
socket.on('connection', async (ws) => {
	ws.on('message', (messageAsString) => {
		console.log("recived message: " + messageAsString);
        	ws.send("Wassap it the server");
		/*fs.readFile('image.png', (err, data) => {
        		if (err) throw err;
    		});   */
	});
	ws.on("close", () => {
		console.log("socket closed");
    	});

	/*while (true) {		
	NodeWebcam.capture( "my_picture", {frames: 1, delay: 0,output: "png"}, function( err, data ) {
    		if ( !err ) console.log( "Image created!" );
		fs.readFile(data, (err, data) => {
        		ws.send(data.toString('base64'));
		});
	});
	await sleep(1000);
	}*/
	console.log("socket connected");
});
function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}

function handlePost(data) {
	if (isJson(data)) {
		parsedData = JSON.parse(data);

		//console.log("data.keyCode " + parsedData.keyCode);
		
		switch (parsedData.side) {
			case "left": 
				console.log("left");
				sendDataToMotorController({ portNumber : "0", position : convertJoystickToPower(parsedData.pow) });
				sendDataToMotorController({ portNumber : "3", position : convertJoystickToPower(-parsedData.pow) });
				sendDataToMotorController({ portNumber : "4", position : convertJoystickToPower(parsedData.pow) });
				break;
			case "right":
				console.log("right");
				sendDataToMotorController({ portNumber : "1", position : convertJoystickToPower(parsedData.pow) });
				sendDataToMotorController({ portNumber : "2", position : convertJoystickToPower(-parsedData.pow) });
				sendDataToMotorController({ portNumber : "5", position : convertJoystickToPower(-parsedData.pow) });
				break;
			default:
				break;
		}
		sendDataToMotorController(convertKeyPressToActionTwo(parsedData))
		sendDataToMotorController(convertKeyPressToAction(parsedData))
	}
}
function isFloat(n){
    return Number(n) === n && n % 1 !== 0;
}

function convertJoystickToPower(value) {
	return (value + 1) / 2;
}

function convertKeyPressToActionTwo(motorMove) {
	console.log("gamepadAction " + motorMove);
	
	

	switch (motorMove.side) {
		case "backPivot":
			console.log("back pivot");
			return { portNumber: "12", position : (motorMove.pow-1)*-1};
			break;

		case "frontPivot":
			console.log("front pivot");
			return { portNumber: "14", position : ((motorMove.pow-1)*-1)};
			break;
		/*case "left": 
			console.log("left");
			return { portNumber : "2", position : convertJoystickToPower(-motorMove.pow) };
			break;
		case "right":
			console.log("right");
			return { portNumber : "3", position : convertJoystickToPower(-motorMove.pow) };
			break;*/
		default:
			return null;
	}
}
function convertKeyPressToAction(motorMove) {
	console.log("gamepadAction " + motorMove);	

	switch (motorMove.side) {
		case "backPivot":
			console.log("back pivot");
			return { portNumber: "13", position : motorMove.pow };
			break;

		case "frontPivot":
			console.log("front pivot");
			return { portNumber: "15", position : motorMove.pow };
			break;
		default:
			return null;
	}
}

function sendDataToMotorController(data) {	
	if (data != null) {
		const pipeData = "pipeData " + data.portNumber + " " + data.position
		console.log(pipeData)
	}
}

function isJson(item) { 
  let value = typeof item !== "string" ? JSON.stringify(item) : item;
  try {
    value = JSON.parse(value);
  } catch (e) {
    return false;
  }

  return typeof value === "object" && value !== null;
}
