const http = require('http')
const { exec } = require("child_process");
const fs = require('fs');
const os = require('os');

const WebSocket = require('ws');

const networkInterfaces = os.networkInterfaces();
const hostname = networkInterfaces['wlan0'][0]['address']
const port = 8080

const socket = new WebSocket.Server({ port: port });
socket.on('connection', (ws) => {
	ws.on('message', (messageAsString) => {
		const message = JSON.parse(messageAsString);

		const outbound = JSON.stringify(message);
		console.log("gottem");
        	ws.send(outbound);
    	});
	ws.on("close", () => {
      		clients.delete(ws);
    	});

	console.log("socket up");
});
