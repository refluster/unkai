
/**
 * Module dependencies.
 */
const WITH_DATA_RECORD = 1;

var express = require('express');
var http = require('http');
var path = require('path');

var app = express();

var logger;
if (WITH_DATA_RECORD) {
	logger = require('./unkai-log');
}

var pages = [];

var routes = require('./routes');
var chat = require('./routes/chat');
var humidity = require('./routes/humidity');
var led_ctrl = require('./routes/led-ctrl');
var tlc5940 = require('./routes/tlc5940');
var njl7502 = require('./routes/njl7502');
var mistgen = require('./routes/mistgen');
var light = require('./routes/light');
var date = require('./routes/date');
var camera = require('./routes/camera');

pages.push(routes);
pages.push(chat);
pages.push(humidity);
pages.push(led_ctrl);
pages.push(tlc5940);
pages.push(njl7502);
pages.push(mistgen);
pages.push(light);
pages.push(date);
pages.push(camera);

if (logger) {
	logger.dev_add(humidity);
	logger.dev_add(tlc5940);
	logger.dev_add(njl7502);
	logger.dev_add(mistgen);
	logger.dev_add(camera);
	logger.start();
}

app.set('port', process.env.PORT || 3000);
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));

for (var i = 0; i < pages.length; i++) {
	app.get(pages[i].path, pages[i].index);
}

var server = http.createServer(app);
server.listen(app.get('port'), function(){
	console.log('Express server listening on port ' + app.get('port'));
});

var io = require('socket.io').listen(server);

// client connected
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

