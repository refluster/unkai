
/**
 * Module dependencies.
 */
const TEST_MODE = 0;

var express = require('express');
var http = require('http');
var path = require('path');

var app = express();

var logger;

var pages = [];
var routes;
var chat;
var humidity;
var led_ctrl;
var tlc5940;
var njl7502;
var mistgen;
var light;
var date;
var camera;
var update;

function start() {
	// start services
	routes = require('./routes');
	if (TEST_MODE == 0) {
		chat = require('./routes/chat');
		humidity = require('./routes/humidity');
		led_ctrl = require('./routes/led-ctrl');
		tlc5940 = require('./routes/tlc5940');
		njl7502 = require('./routes/njl7502');
		mistgen = require('./routes/mistgen');
		light = require('./routes/light');
		date = require('./routes/date');
		camera = require('./routes/camera');
	}
	update = require('./routes/update');
	logger = require('./routes/unkai-log');

	// add services into pages array
	pages.push(routes);
	if (TEST_MODE == 0) {
		pages.push(chat);
		pages.push(humidity);
		pages.push(led_ctrl);
		pages.push(tlc5940);
		pages.push(njl7502);
		pages.push(mistgen);
		pages.push(light);
		pages.push(date);
		pages.push(camera);
	}
	pages.push(logger);
	pages.push(update);

	// add test device for sample logger
	if (TEST_MODE == 1) {
		var tmp_dev = {};
		tmp_dev.get = function(fn) {fn(123);};
	}

	// add devices into logger
	if (logger) {
		if (TEST_MODE == 0) {
			logger.dev_add(humidity);
			logger.dev_add(tlc5940);
			logger.dev_add(njl7502);
			logger.dev_add(mistgen);
			logger.dev_add(camera);
		} else {
			logger.dev_add(tmp_dev);
		}
	}

	// server settings
	app.set('port', process.env.PORT || 3000);
	app.set('views', path.join(__dirname, 'views'));
	app.set('view engine', 'ejs');
	app.use(express.static(path.join(__dirname, 'public')));
	for (var i = 0; i < pages.length; i++) {
		app.get(pages[i].path, pages[i].index);
	}

	// start server
	var server = http.createServer(app);
	server.listen(app.get('port'), function(){
		console.log('Express server listening on port ' + app.get('port'));
	});

	// start socket.io
	var io = require('socket.io').listen(server);

	// add handler for client connections
	io.sockets.on('connection', function(client) {
		console.log("connection");
		for (var i = 0; i < pages.length; i++) {
			if (pages[i].init_socket) {
				pages[i].init_socket(io, client);
			}
		}

		// client disconnected
		client.on('disconnect', function(){
			console.log("disconnect");
		});
	});

}

start();
