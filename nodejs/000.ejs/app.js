
/**
 * Module dependencies.
 */

var express = require('express');
var http = require('http');
var path = require('path');

var routes = require('./routes');
var user = require('./routes/user');
var chat = require('./routes/chat');
var humidity = require('./routes/humidity');

var app = express();

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

var server = http.createServer(app);
server.listen(app.get('port'), function(){
    console.log('Express server listening on port ' + app.get('port'));
});

var socketio = require('socket.io');
// wait for connecbtions from clients
var io = socketio.listen(server);

// client connected
io.sockets.on('connection', function(socket) {
    console.log("connection");
    // chat message received
    socket.on('message', function(data) {
        // broadcast to all clients
        console.log("message");
        io.sockets.emit('message', { value: data.value });
    });
    
    // for humidity
    socket.on('get humidity', function(data) {
        console.log("get humidity");
        socket.emit('humidity', { humidity: 30, temperature: 23});
    });

    // client disconnected
    socket.on('disconnect', function(){
        console.log("disconnect");
    });
});
