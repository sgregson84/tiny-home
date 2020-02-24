const http = require('http');
const fs = require('fs');
const hostname = '192.168.1.199';
const port = 3000;
const path = require('path'),  
      filePath = path.join(__dirname, 'index.html');


const server = http.createServer((req, res) => {
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

server.listen(port, hostname, () => {
  console.log(`Tiny Hub Server running at http://${hostname}:${port}/`);
});