exports.init = function(drv, server) {
	var io = require('socket.io').listen(server);
	
	io.sockets.on('connection', function(client) {
		console.log("connection");
		
		// client disconnected
		client.on('disconnect', function(){
			console.log("disconnect");
		});
		
		// sensor
		client.on('sensor/get', function(data) {
			drv.getSensor(function(val) {
				client.emit('sensor/response', val);
				console.log('sensor');
			});
		});
		
		// led
		client.on('led/set', function(data) {
			drv.setLed(data.brightness);
			console.log("led/set " + data.brightness);
		});
		
		// rtc
		client.on('rtc/set', function(data) {
			drv.setRtc(data.date);
			console.log("rtc/set " + data.date);
		});
	});
};
