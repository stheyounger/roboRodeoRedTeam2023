const http = require('http')

const hostname = '192.168.0.158'
const port = 8080

const fs = require('fs');

const server = http.createServer((req, res) => {
	const { headers, method, url } = req;
	if (req.method == 'POST') {
		console.log('data: ' + url.toString());
//    		var body = ''
//    		request.on('data', function(data) {
//      			body += data
//      			console.log('Partial body: ' + body)
//    		})
//    		request.on('end', function() {
//      			console.log('Body: ' + body)
//      			response.writeHead(200, {'Content-Type': 'text/html'})
//      			response.end('post received')
//    		})
  	}
	res.writeHead(200, {'Content-Type': 'text/html'})
  	fs.createReadStream('frontEnd/index.html').pipe(res)
})

server.listen(port, hostname, () => {
 	console.log(`Server running at http://`+hostname+`:`+port+`/`)
})

//Request types:
//GET, POST, PUT, PATCH, and DELETE
