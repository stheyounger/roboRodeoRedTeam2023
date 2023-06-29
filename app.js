const http = require('http')

const hostname = '127.0.0.69'
const port = 8080

const fs = require('fs');

const server = http.createServer((req, res) => {
  res.statusCode = 200
  res.setHeader('Content-Type', 'text/html')
  fs.createReadStream('index.html').pipe(res)
})

server.listen(port, hostname, () => {
  console.log(`Server running at http://`+hostname+`:`+port+`/`)
})
