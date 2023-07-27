const http = require('http')
const { exec } = require("child_process");
const fs = require('fs');
const os = require('os');

const networkInterfaces = os.networkInterfaces();
const hostname = networkInterfaces['wlan0'][0]['address']
const port = 8080

const server = http.createServer((req, res) => {
	const { headers, method, url } = req;
	if (req.method == 'POST') {
    		req.on('data', function(data) {
			res.writeHead(200, {'Content-Type': 'text/plain'})
			res.end("yup")

			console.log("Server recived: " + data);
			handlePost(data)
		})
  	}else{
		res.writeHead(200, {'Content-Type': 'text/html'})
  		fs.createReadStream('webControl.html').pipe(res)
	}
})

server.listen(port, hostname, () => {
 	console.log(`Server running at http://`+hostname+`:`+port+`/`)
})

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
