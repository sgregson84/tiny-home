const http = require('http');
const fs = require('fs');
const hostname = '192.168.1.199';
const port = 3000;
const path = require('path'),  
      filePath = path.join(__dirname, 'index.html');
const express = require('express');
const app = express();
const DEVICE_PATH  = '/devices'; 
var garageDoor = {
  id: "6f77a7cd-d21e-486c-a1a0-dd8dee905893",
  name:"Garage Door", 
  state: "CLOSED"
};


/*const server = http.createServer((req, res) => {
  res.statusCode = 200;
  res.setHeader('Content-Type', 'text/html');

  fs.readFile(filePath,function(error,data){
    if(error){
      res.writeHead(404);
      res.write('Whoops! File not found!');
    }else{
      res.write(data);
    }

  console.log('New connection');
  
  res.end();
  });
});
*/

const deviceResponse = `
    {"status" :"COMPLETE"}
`; 

app.use(express.json());

app.get('/', (req, res) => {
    //res.send (filePath); 
    fs.readFile(filePath,function(error,data){
      if(error){
        res.writeHead(404);
        res.write('Whoops! File not found!');
      }else{
        res.write(data);
      }
  
    console.log('New connection');
    }); 
});

app.get('/devices', (req, res) => { 

    res.send(garageDoor);
    
}
);

/***
 * Process the device POST on http://[host]:[port]/devices
 */
app.post('/devices', (req, res) => {
    console.log(`Got ${JSON.stringify(req.body)}`); 
    // Save for later
    garageDoor = req.body; 
    res.send(deviceResponse); 
});

app.listen(port, hostname, () => {
  console.log(`Tiny Hub Server running at http://${hostname}:${port}/`);
});
















/*const http = require('http');
const fs = require('fs');
const hostname = '127.0.0.1';
const port = 3000;

let handleRequest = (request, response) => {
  response.writeHead(200, {
      'Content-Type': 'text/html'
  });
  fs.readFile('./index.html', null, function (error, data) {
      if (error) {
          response.writeHead(404);
          respone.write('Whoops! File not found!');
      } else {
          response.write(data);
      }
      response.end();
  });
};

const server = http.createServer(handleRequest).listen(port);

/*var userCount = 0;

const server = http.createServer((req, res) => {
  res.statusCode = 200;
  userCount++;
  res.setHeader('Content-Type', 'text/plain');
  res.write('Hello World');
  console.log('New connection, '+userCount+'');
  res.write('Hello!\n');
  res.write('We have had '+userCount+' visits!\n');
  res.end();
  
});




server.listen(port, hostname, () => {
  console.log(`Tiny Hub Server running at http://${hostname}:${port}/`);
});
*/