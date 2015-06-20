exports.init = function(driver, server, conditioner) {
	const SLEEP_CONDITIONER_INTERVAL = 10000;
	var io = require('socket.io').listen(server);
	var timer;

	io.sockets.on('connection', function(client) {
		console.log("connection");
		
		// client disconnected
		client.on('disconnect', function(){
			console.log("disconnect");
		});
		
		// sensor
		client.on('sensor/get', function(data) {
			driver.getSensor(function(val) {
				client.emit('sensor/response', val);
				console.log('sensor');
			});
		});
		
		// led
		client.on('led/set', function(data) {
			driver.setLed(data.brightness);
			console.log("led/set " + data.brightness);
			conditioner.stop();
			driver.setMistgen(100);
			clearInterval(timer);
			timer = setTimeout(function() {
				conditioner.start();
			}, SLEEP_CONDITIONER_INTERVAL);
		});
		
		// rtc
		client.on('rtc/set', function(data) {
			driver.setRtc(data.date);
			console.log("rtc/set " + data.date);
		});
	});
};
