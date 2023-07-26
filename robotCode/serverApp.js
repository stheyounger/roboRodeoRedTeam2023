const http = require('http')
const { exec } = require("child_process");
const fs = require('fs');

const hostname = '192.168.0.176'
const port = 8080

const server = http.createServer((req, res) => {
	const { headers, method, url } = req;
	if (req.method == 'POST') {
    		req.on('data', function(data) {
    			const dataString = JSON.parse(data);
			res.writeHead(200, {'Content-Type': 'text/plain'})
			res.end("yup")
			console.log(dataString)
		})
  	}else{
		res.writeHead(200, {'Content-Type': 'text/html'})
  		fs.createReadStream('webControl.html').pipe(res)
	}
})

server.listen(port, hostname, () => {
 	console.log(`Server running at http://`+hostname+`:`+port+`/`)
})


function sendData(dataJson) {
	const dataString = JSON.parse(dataJson);
	console.log(dataString);
	fs.writeFile('dataFile.txt', dataString, err => {
		if (err) {
			console.log("err %s\n", err);
		}
	});
}
