const http = require('http')
const { exec } = require("child_process");
const fs = require('fs');
const os = require('os');

const WebSocket = require('ws');

const networkInterfaces = os.networkInterfaces();
const hostname = networkInterfaces['wlan0'][0]['address']
const port = 8080



const root = (req, res)=>{
	const { headers, method, url } = req;
	if (req.method == 'POST') {
    		req.on('data', function(data) {
			res.writeHead(200, {'Content-Type': 'text/plain'})
			res.end("yup")

			console.log("Server recived: " + data);
			handlePost(data)
		})
  	} else if (req.method == "GET") {
		res.writeHead(200, {'Content-Type': 'text/html'})
  		fs.createReadStream('webControl.html').pipe(res)
	}
}

const video = (req, res) => {
	const { headers, method, url } = req;
	if (req.method == "GET") {
		console.log("gettingVideo")
    		const path = 'video.mp4'
  		const stat = fs.statSync(path)
  		const fileSize = stat.size
  		const head = {
    			'Content-Length': fileSize,
    			'Content-Type': 'video/mp4',
  		}
  		res.writeHead(200, head)
		fs.createReadStream(path).pipe(res)
	}
}

const wsSite = (req, res) => {
	const { headers, method, url } = req;
	res.writeHead(200, {'Content-Type': 'text/html'})
  	fs.createReadStream('webControl.html').pipe(res)
}

const server = http.createServer((req, res) => {
	const {url} = req;

	const paths = {
		"/":root,
		"/video":video/*,
		"/ws": wsSite*/
	}

	paths[url](req, res)
})

server.listen(port, hostname, () => {
 	console.log(`Server running at http://`+hostname+`:`+port+`/`)
})


const socket = new WebSocket.Server({ port: 8081 });
socket.on('connection', (ws) => {
	ws.on('message', (messageAsString) => {
		//const message = JSON.parse(messageAsString);

		//const outbound = JSON.stringify(message);
		console.log("recived message: " + messageAsString);
        	ws.send("Wassap it the server");
    	});
	ws.on("close", () => {
		console.log("socket closed");
    	});

	console.log("socket connected");
});


function handlePost(data) {
	if (isJson(data)) {
		parsedData = JSON.parse(data);

		console.log("data.keyCode " + parsedData.keyCode);

		sendDataToMotorController(convertKeyPressToAction(parsedData.keyCode))
	}
}

function convertKeyPressToAction(keyPressed) {
	switch (keyPressed) {
		case "KeyQ":
			return { portNumber : "0", position : "1" };
			break;
		case "KeyA":
			return { portNumber : "0", position : "0" };
			break;
		case "KeyS": 
			return { portNumber : "1", position : "0" };
			break;
		case "KeyW": 
			return { portNumber : "1", position : "1" };
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
