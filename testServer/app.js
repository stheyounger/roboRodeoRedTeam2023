const http = require('http')

const hostname = '192.168.0.158'
const port = 8080

const fs = require('fs');

const server = http.createServer((req, res) => {
  res.statusCode = 200
  res.setHeader('Content-Type', 'text/html')
  fs.createReadStream('frontEnd/index.html').pipe(res)
})

server.listen(port, hostname, () => {
  console.log(`Server running at http://`+hostname+`:`+port+`/`)
})
