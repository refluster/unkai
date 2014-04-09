
/**
 * Module dependencies.
 */

var express = require('express');
var http = require('http');
var path = require('path');

var app = express();

var routes = require('./routes');
var user = require('./routes/user');
var chat = require('./routes/chat');
var humidity = require('./routes/humidity');
var led_ctrl = require('./routes/led-ctrl');

// all environments
app.configure('development', function(){
    app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
});
app.set('port', process.env.PORT || 3000);
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.json());
app.use(express.urlencoded());
app.use(express.methodOverride());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
    app.use(express.errorHandler());
}

app.get('/', routes.index);
app.get('/users', user.list);
app.get('/chat', chat.index);
app.get('/humidity', humidity.index);
app.get('/led-ctrl', led_ctrl.index);

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

    // client disconnected
    client.on('disconnect', function(){
        console.log("disconnect");
    });
});
