var express = require('express');
var devices = require('../data/device-data');
var router = express.Router();

/* GET users listing. */
router.get('/', function(req, res, next) {
  res.send(JSON.stringify(devices));
});

router.put('/', function(req, res, next) {
  var data = JSON.parse(req.body); 
  devices[0] = data; 
  res.send(JSON.stringify(devices));
});
module.exports = router;
