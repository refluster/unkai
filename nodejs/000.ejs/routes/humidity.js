var exec = require('child_process').exec;

exports.index = function(req, res){
    console.log('getting humidity from ' + req.headers.host);
    res.render('humidity', { title: 'humidity and temperature', host: req.headers.host });
};

exports.init_socket = function(io, client){
    // for humidity
    client.on('humidity get_value', function(data) {
		console.log("get humidity");
		
		// get humidity ,celsius, fahrenheit
		var cmd = "../../c/002.humidity-sensor/002.humidity -1";
		exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
			var humidity, celsius, fahrenheit;
			console.log('stdout: '+(stdout||'none'));
			console.log('stderr: '+(stderr||'none'));
			if (error !== null) {
				console.log('exec error: '+error);
			} else {
				var re = /humidity:(\d+\.\d+) celsius:(\d+\.\d+) fahrenheit:(\d+\.\d+)/;
				var re_match = stdout.match(re);
				humidity = re_match[1];
				celsius = re_match[2];
				fahrenheit = re_match[3];
			}
			client.emit('humidity ret_value', { humidity: humidity,
												celsius: celsius,
												fahrenheit: fahrenheit});
		});
	});
};
