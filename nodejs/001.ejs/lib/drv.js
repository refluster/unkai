const TEST_DRIVER = 0;

var exec = require('child_process').exec;
var spawn = require('child_process').spawn;

// mcp3204[illuminance, moisture] ////////////////////////////
function mcp3204_get(callback) {
	var illuminance, moisture;
	var cmd = "../../c/007.mcp3204/007.mcp3204";

	if (TEST_DRIVER) {
		callback({illuminance: 120, moisture: 40});
		return;
	}

	exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
		if (error !== null) {
			console.log("mcp3204 :\n" + error);
		} else {
			var re = /illuminance: (\d+\.\d+) lux[\s\S]*moisture: (\d+\.\d+) %/;
			var re_match = stdout.match(re);
			if (re_match) {
				illuminance = re_match[1];
				moisture = re_match[2];
			}
		}
		callback({illuminance: illuminance, moisture: moisture});
	});
}

// dht11 ////////////////////////////
function dht11_get(callback) {
	var humidity, celsius, fahrenheit;

	if (TEST_DRIVER) {
		callback({humidity: 33, celsius: 23, fahrenheit: 56});
		return;
	}

	// get humidity ,celsius, fahrenheit
	var cmd = "../../c/002.humidity-sensor/002.humidity -1";
	exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
		if (error !== null) {
			console.log("dht11 :\n" + error);
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

// tlc5940 ////////////////////////////
const num_led = 15; // # of led to control
var tlc5940_process;
var tlc5940_brightness = [];

function tlc5940_set(brightness) {
	var cmd = "1 " + brightness.join(" ") + "\n";
	
	if (cmd.split(' ').length != num_led + 1) {
		console.log('tlc5940_set: cmd length error');
		return;
	}

	if (TEST_DRIVER) {
		console.log('tlc5940_set: ' + cmd);
		return;
	}
	
	tlc5940_process.stdin.write(cmd);
	tlc5940_brightness = brightness;
}

function tlc5940_get(callback) {
	callback(tlc5940_brightness);
}

// rtc ////////////////////////////
function rtc_set(date) {
	var cmd = "../../c/008.date/008.date -s " + data.value;
	
	if (TEST_DRIVER) {
		console.log('rtc_set: ' + date);
		return;
	}
	
	exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
		if (error !== null) {
			console.log("rtc :\n" + error);
		}
	});
}

// mistgen ////////////////////////////
var mistgen_process;
var mistgen_percent = 0;

function mistgen_set(ratio) {
	if (TEST_DRIVER) {
		console.log('mistgen_set: ' + ratio);
		return;
	}

	var cmd = "1 " + ratio + "\n";
	mistgen_process.stdin.write(cmd);
	mistgen_percent = ratio;
}

// primitive function ////////////////////////////
function extend(target) {
	var sources = [].slice.call(arguments, 1);
	sources.forEach(function (source) {
		for (var prop in source) {
			target[prop] = source[prop];
		}
	});
	return target;
}

// exports ////////////////////////////
exports.getSensor = function(callback) {
	mcp3204_get(function(mcp3204_val) {
		dht11_get(function(dht11_val) {
			var obj = extend({}, mcp3204_val, dht11_val);
			callback(obj);
		});
	});
};

exports.setLed = function(brightness) {
	tlc5940_set(brightness);
};

exports.getLed = function(callback) {
	tlc5940_get(callback);
};

exports.setRtc = function(date) {
	rtc_set(date);
};

exports.setMistgen = function(ratio) {
	mistgen_set(ratio);
};

exports.start = function() {
	if (! TEST_DRIVER) {
		mistgen_process = spawn("../../c/006.mistgen/006.mistgen");
		tlc5940_process = spawn("../../c/003.tlc5940/003.tlc5940", ["-n", String(num_led)]);
	}
};

exports.stop = function() {
	if (TEST_DRIVER) {
		console.log('driver.stop');
		return;
	}
	
	tlc5940_process.stdin.write("0\n");
	mistgen_process.stdin.write("0\n");
};

