const TEST_DRIVER = 0;
const num_led = 15; // # of led to control

var spawn = require('child_process').spawn;
var tlc5940_brightness = [];

function tlc5940_set(p, brightness) {
	var cmd = "1 " + brightness.join(" ") + "\n";
	
	if (TEST_DRIVER) {
		console.log('tlc5940_set: ' + brightness);
		return;
	}
	
	p.stdin.write(cmd);
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
	tlc5940_set(this.tlc5940_process, brightness);
};

exports.getLed = function(callback) {
	tlc5940_get(callback);
};

exports.setLedMaxBrightness = function(b) {
	this.max_brightness = b;
	var brightness = this.brightness_ratio.map(function(r) {return r * this.max_brightness}.bind(this));
	tlc5940_set(this.tlc5940_process, brightness);
};

exports.setLedPattern = function(b) {
	this.conf.pattern = b;
	if (b == 'on') {
		for (var i = 0; i < this.brightness_ratio.length; i++) {
			this.brightness_ratio[i] = 1;
		}
	} else if (b == 'off') {
		for (var i = 0; i < this.brightness_ratio.length; i++) {
			this.brightness_ratio[i] = 0;
		}
	} else if (b == 'random') {
		var brightness = [];
		setInterval(function() {
			for (var i = 0; i < num_led; i++) {
				brightness[i] = 0;
			}
			var n = parseInt(Math.random() * num_led);
			brightness[n] = 1000;
			tlc5940_set(this.tlc5940_process, brightness);
		}.bind(this), 500);
	}
	var brightness = this.brightness_ratio.map(function(r) {return r * this.max_brightness}.bind(this));
	tlc5940_set(this.tlc5940_process, brightness);
};

exports.start = function() {
	this.tlc5940_process;
	this.max_brightness = 1000;
	this.conf = {};

	if (! TEST_DRIVER) {
		this.tlc5940_process = spawn("../../c/003.tlc5940/003.tlc5940", ["-n", String(num_led)]);
	}

	this.brightness_ratio = [];
	for (var i = 0; i < num_led; i++) {
		this.brightness_ratio.push(1);
	}

	brightness = [];
	for (var i = 0; i < num_led; i++) {
		brightness.push(0);
	}
	tlc5940_set(this.tlc5940_process, brightness);
};

exports.stop = function() {
	if (TEST_DRIVER) {
		console.log('driver.stop');
		return;
	}
	
	this.tlc5940_process.stdin.write("0\n");
};
