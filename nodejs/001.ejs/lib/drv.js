const TEST_DRIVER = 1;

var exec = require('child_process').exec;

exports.getHumidity = function(callback) {
	var humidity, celsius, fahrenheit;

	if (TEST_DRIVER) {
		callback({humidity: 33, celsius: 23, fahrenheit: 56});
		return;
	}

	// get humidity ,celsius, fahrenheit
	var cmd = "../../c/002.humidity-sensor/002.humidity -1";
	exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
		console.log('stdout: '+(stdout||'none'));
		console.log('stderr: '+(stderr||'none'));
		if (error !== null) {
			console.log('exec error: '+error);
		} else {
			var re = /humidity:(\d+\.\d+) celsius:(\d+\.\d+) fahrenheit:(\d+\.\d+)/;
			var re_match = stdout.match(re);
			if (re_match) {
				humidity = re_match[1];
				celsius = re_match[2];
				fahrenheit = re_match[3];
			}
		}
		callback({humidity: humidity, celsius: celsius, fahrenheit: fahrenheit});
	});
}
