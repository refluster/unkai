
/**
 * Module dependencies.
 */
const WITH_OLD_VER = 0;
const WITH_DATA_RECORD = 1;

var express = require('express');
var http = require('http');
var path = require('path');

var app = express();

var logger;
if (WITH_DATA_RECORD) {
	logger = require('./unkai-log');
}

var routes = require('./routes');
var user = require('./routes/user');

var chat = require('./routes/chat');
var humidity = require('./routes/humidity');
var led_ctrl = require('./routes/led-ctrl');
var tlc5940 = require('./routes/tlc5940');
var njl7502 = require('./routes/njl7502');
var mistgen = require('./routes/mistgen');

if (logger) {
	logger.dev_add(njl7502);
	logger.dev_add(mistgen);
	logger.start();
}

if (WITH_OLD_VER) {
	// all environments
	app.configure('development', function(){
		app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
	});
}

app.set('port', process.env.PORT || 3000);
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

if (WITH_OLD_VER) {
	app.use(express.favicon());
	app.use(express.logger('dev'));
	app.use(express.json());
	app.use(express.urlencoded());
	app.use(express.methodOverride());
	app.use(app.router);
}
app.use(express.static(path.join(__dirname, 'public')));

if (WITH_OLD_VER) {
	// development only
	if ('development' == app.get('env')) {
		app.use(express.errorHandler());
	}
}

app.get('/', routes.index);
app.get('/users', user.list);

app.get('/chat', chat.index);
app.get('/humidity', humidity.index);
app.get('/led-ctrl', led_ctrl.index);
app.get('/tlc5940', tlc5940.index);
app.get('/njl7502', njl7502.index);
app.get('/mistgen', mistgen.index);

var server = http.createServer(app);
server.listen(app.get('port'), function(){
	console.log('Express server listening on port ' + app.get('port'));
});

var io = require('socket.io').listen(server);

// client connected
io.sockets.on('connection', function(client) {
	console.log("connection");
	chat.init_socket(io, client);
	humidity.init_socket(io, client);
	led_ctrl.init_socket(io, client);
	tlc5940.init_socket(io, client);
	njl7502.init_socket(io, client);
	mistgen.init_socket(io, client);

	// client disconnected
	client.on('disconnect', function(){
		console.log("disconnect");
	});
});

