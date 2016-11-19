var io
var timer;

exports.init = function(driver, server) {
	io = require('socket.io').listen(server);

	io.sockets.on('connection', function(client) {
		console.log("client connected");
		
		// client disconnected
		client.on('disconnect', function(){
			console.log("client disconnected");
		});
		
		// led
		client.on('led/set', function(data) {
//			driver.setLed(data.brightness);
			console.log("led/set " + data.brightness);
		});
		
		// led max brightness
		client.on('led/setMaxBrightness', function(data) {
			driver.setLedMaxBrightness(data.brightness);
			console.log("led/setMaxBrightness " + data.brightness);
		});

		// led pattern
		client.on('led/setPattern', function(data) {
			driver.setLedPattern(data.pattern);
			console.log("led/setPattern " + data.pattern);
		});

		// system
		client.on('system/reset', function() {
			console.log("system/reset");
			driver.stop();
			driver.start();
			client.emit('system/reset/complete');
		});
	});
};
