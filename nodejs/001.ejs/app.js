var express = require('express');
var http = require('http');
var path = require('path');

var app = express();
var server;

function stop() {
	// terminate background processes
	//   T.B.D.

	// close http server
	server.close();
}

function isMobile(ua) {
	if ((ua.indexOf('iPhone') > 0 && ua.indexOf('iPad') == -1) ||
		ua.indexOf('iPod') > 0 ||
		ua.indexOf('Android') > 0) {
		return true;
	}
	return false;
}

function start() {
	// server settings
	app.set('port', process.env.PORT || 3000);
	app.set('view engine', 'ejs');
	app.use(express.static(path.join(__dirname, 'public')));

	// start server
	server = http.createServer(app);
	server.listen(app.get('port'), function() {
		console.log('Express server listening on port ' + app.get('port'));
	});

	// driver
	driver = require('./lib/drv');

	// conditioner
	conditioner = require('./lib/conditioner');
	conditioner.init(driver);
	conditioner.start();

	// socket io
	socket = require('./lib/socket');
	socket.init(driver, server, conditioner);

	// pages
	app.get("/status", function(req, res) {
		var mobile = false;
		
		if (isMobile(JSON.stringify(req.headers['user-agent']))) {
			mobile = true;
		}
		res.render('status', {host: req.headers.host, is_mobile: mobile});
	});

	app.get("/", function(req, res) {
		var mobile = false;
		
		if (isMobile(JSON.stringify(req.headers['user-agent']))) {
			mobile = true;
		}
		res.render('index', {host: req.headers.host, is_mobile: mobile});
	});
}

start();
