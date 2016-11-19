const TEST_DRIVER = 0;

var spawn = require('child_process').spawn;

// tlc5940 ////////////////////////////
const num_led = 15; // # of led to control
var tlc5940_process;
var tlc5940_brightness = [];
var brightness_ratio = [];
var max_brightness = 1000;

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

exports.setLedMaxBrightness = function(b) {
	max_brightness = b;
	var brightness = brightness_ratio.map(function(r) {r * max_brightness});
	tlc5940_set(brightness);
};

exports.setLedPattern = function(b) {
	if (b == 'on') {
		for (var i = 0; i < brightness_ratio.length; i++) {
			brightness_ratio[i] = 1;
		}
	} else {
		for (var i = 0; i < brightness_ratio.length; i++) {
			brightness_ratio[i] = 0;
		}
	}
	var brightness = brightness_ratio.map(function(r) {return r * max_brightness});
	console.log(max_brightness);
	console.log(brightness_ratio);
	console.log(brightness);
	tlc5940_set(brightness);
};

exports.start = function() {
	if (! TEST_DRIVER) {
		tlc5940_process = spawn("../../c/003.tlc5940/003.tlc5940", ["-n", String(num_led)]);
	}

	brightness = [];
	for (var i = 0; i < num_led; i++) {
		brightness.push(0);
	}

	brightness_ratio = [];
	for (var i = 0; i < num_led; i++) {
		brightness_ratio.push(1);
	}

/*
	var increment = 100;
	setInterval(function() {
		for (var i = 0; i < num_led; i++) {
			brightness[i] += increment;
		}
		if ((increment > 0 && brightness[0] + increment > 4095) ||
			(increment < 0 && brightness[0] + increment < 0)) {
			increment = -increment;
		}
		
		tlc5940_set(brightness);
	}, 100);
*/

/*
	var increment = 100;
	setInterval(function() {
		var n = parseInt(Math.random() * num_led);

		for (var i = 0; i < num_led; i++) {
			brightness[i] = 0;
		}
		brightness[n] = 1000;
		
		tlc5940_set(brightness);
	}, 30);
*/
};

exports.stop = function() {
	if (TEST_DRIVER) {
		console.log('driver.stop');
		return;
	}
	
	tlc5940_process.stdin.write("0\n");
};
