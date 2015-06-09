exports.init = function(drv, server) {
	var io = require('socket.io').listen(server);
	
	io.sockets.on('connection', function(client) {
		console.log("connection");
		
		// client disconnected
		client.on('disconnect', function(){
			console.log("disconnect");
		});
		
		// for humidity
		client.on('sensor/get', function(data) {
			drv.getSensor(function(val) {
				client.emit('sensor/response', val);
				console.log('sensor');
			});
		});
	});
};
