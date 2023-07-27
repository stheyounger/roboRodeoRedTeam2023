const http = require('http')
const { exec } = require("child_process");
const fs = require('fs');
const os = require('os');

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
		console.log("we gottem")
		const range = req.headers.range
  		const videoPath = 'video.mp4';
    		const videoSize = fs.statSync(videoPath).size
    		const chunkSize = 1 * 1e6;
    		const start = Number(range.replace(/\D/g, ""))
    		const end = Math.min(start + chunkSize, videoSize - 1)
    		const contentLength = end - start + 1;
    		const headers = {
        		"Content-Range": `bytes ${start}-${end}/${videoSize}`,
        		"Accept-Ranges": "bytes",
        		"Content-Length": contentLength,
        		"Content-Type": "video/mp4"
    		}
    		res.writeHead(206, headers)
		fs.createReadStream(videoPath, {start, end}).pipe(res)
	}
}

const server = http.createServer((req, res) => {
	const {url} = req;

	const paths = {
	   "/":root,
	   "/video":video
	}

	paths[url](req, res)
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
