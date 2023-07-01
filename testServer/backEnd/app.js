const http = require('http')

const hostname = '192.168.0.158'
const port = 8080

const fs = require('fs');

const server = http.createServer((req, res) => {
	const { headers, method, url } = req;
	if (req.method == 'POST') {
    		req.on('data', function(data) {
    			const dataString = JSON.parse(data);
			controlServo(dataString.slider, dataString.value);
		})
  	}
	res.writeHead(200, {'Content-Type': 'text/html'})
  	fs.createReadStream('../frontEnd/index.html').pipe(res)
})

server.listen(port, hostname, () => {
 	console.log(`Server running at http://`+hostname+`:`+port+`/`)
})

//Request types:
//GET, POST, PUT, PATCH, and DELETE

function sendData(dataJson) {
	const dataString = JSON.parse(dataJson);
	console.log(dataString);
	fs.writeFile('dataFile.txt', dataString, err => {
		if (err) {
			console.log("err %s\n", err);
		}
	});
}

function controlServo(servoNumberInt, positionInt) {
	const { exec } = require("child_process");
	exec("sudo ./servoController " + servoNumberInt + " " + positionInt, (error, stdout, stderr) => console.log(stdout));
}
