const TEST_DRIVER = 0;

var spawn = require('child_process').spawn;

// tlc5940 ////////////////////////////
const num_led = 15; // # of led to control
var tlc5940_process;
var tlc5940_brightness = [];

function tlc5940_set(brightness) {
	var cmd = "1 " + brightness.join(" ") + "\n";
	
	if (TEST_DRIVER) {
		console.log('tlc5940_set: ' + brightness);
		return;
	}
	
	tlc5940_process.stdin.write(cmd);
	tlc5940_brightness = brightness;
}

function tlc5940_get(callback) {
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
exports.setLed = function(brightness) {
	tlc5940_set(brightness);
};

exports.getLed = function(callback) {
	tlc5940_get(callback);
};

exports.start = function() {
	if (! TEST_DRIVER) {
		tlc5940_process = spawn("../../c/003.tlc5940/003.tlc5940", ["-n", String(num_led)]);
	}
};

exports.stop = function() {
	if (TEST_DRIVER) {
		console.log('driver.stop');
		return;
	}
	
	tlc5940_process.stdin.write("0\n");
};

