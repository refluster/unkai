const TEST_DRIVER = 1;

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
var tlc5940_drv;
if (! TEST_DRIVER) {
	tlc5940_drv = spawn("../../c/003.tlc5940/003.tlc5940", ["-n", String(num_led)]);
}
var tlc5940_brightness = [];

function tlc5940_set(brightness) {
	// update cmd
	if (TEST_DRIVER) {
		console.log('tlc5940_set: ' + brightness);
		return;
	}
	var cmd = "1 " + brightness.join(" ") + "\n";
	tlc5940_drv.stdin.write(cmd);
	tlc5940_brightness = brightness;
}

function tlc5940_get_val(callback) {
	callback(tlc5940_brightness);
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
}

exports.setLed = function(brightness) {
	tlc5940_set(brightness);
}

exports.getLed = function(callback) {
	tlc5940_get(callback);
}
