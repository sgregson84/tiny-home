var express = require('express');
var devices = require('../data/device-data');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { 
    title: 'Tiny Home Hub',
    version: '0.0.1', 
    devices: devices
  });
});

module.exports = router;
