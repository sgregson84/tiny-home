 
const express = require('express');
const app = express();
const port = 3000;
const DEVICE_PATH  = '/devices'; 
var garageDoor = {
    id: "6f77a7cd-d21e-486c-a1a0-dd8dee905893",
    name:"Garage Door", 
    state: "CLOSED"
}; 
// Landing page html 
const indexPage = `
    <http>
    <head>
        <title>TinyHouse</title>
    </head>
    <body>
    <h1>TinyHouse</h1>
    <h2>Links: </h2>
    <h3><a href="/devices">Devices</a></h3>
    </body>
    </http>
`; 
const deviceResponse = `
    {"status" :"COMPLETE"}
`; 

app.use(express.json());

app.get('/', (req, res) => {
    res.send (indexPage); 
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

app.listen(port, () => console.log(`Tiny Home listening on port ${port}!`));