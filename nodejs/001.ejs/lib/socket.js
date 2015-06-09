exports.init = function(drv, server) {
	var io = require('socket.io').listen(server);
	
	io.sockets.on('connection', function(client) {
		console.log("connection");
		
		// client disconnected
		client.on('disconnect', function(){
			console.log("disconnect");
		});
		
		// for humidity
		client.on('humidity get_value', function(data) {
			drv.getHumidity(function(val) {
				client.emit('humidity ret_value', { humidity: val.humidity,
													celsius: val.celsius,
													fahrenheit: val.fahrenheit});
			});
			console.log('hum get');
		});
	});
};
