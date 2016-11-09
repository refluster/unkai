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
			driver.setLed(data.brightness);
			console.log("led/set " + data.brightness);
			driver.setMistgen(100);
			clearInterval(timer);
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
