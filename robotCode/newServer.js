const os = require('os');
const networkInterfaces = os.networkInterfaces();
const hostname = networkInterfaces['wlan0'][0]['address']
const port = 8080

const express = require('express');
const app = express();

app.get("/page/:id",function(request, response){
	/*
    var id = request.params.id;
    // do something with id
    // send a response to user based on id
    var obj = { id : id, Content : "content " +id };

    response.writeHead(200, {"Content-Type": "application/json"});
    response.write(JSON.stringify(obj));
    */
	console.log("hi");
});
