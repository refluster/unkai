// init tlc5940 driver daemon
const num_led = 15; // # of led to control
var spawn = require('child_process').spawn;
var tlc5940_drv = spawn("../../c/003.tlc5940/003.tlc5940",
						["-n", String(num_led)]);
var tlc5940_brightness = [];

function tlc5940_set_val(brightness) {
	// update cmd
	var cmd = "1 " + brightness.join(" ") + "\n";
	tlc5940_drv.stdin.write(cmd);
	tlc5940_brightness = brightness;
}

function tlc5940_get_val(callback) {
	callback(tlc5940_brightness);
}

exports.index = function(req, res){
	// get user-agent
	var ua = JSON.stringify(req.headers['user-agent']);
	var is_mobile = false;

	if ((ua.indexOf('iPhone') > 0 && ua.indexOf('iPad') == -1) ||
		ua.indexOf('iPod') > 0 ||
		ua.indexOf('Android') > 0) {
		is_mobile = true;
	}
	res.render('tlc5940', { title: 'tlc5940 control', host: req.headers.host, is_mobile: is_mobile,
							num_led: num_led, init_val: tlc5940_brightness});
};

exports.init_socket = function(io, client){
	client.on('tlc5940 update', function(data) {
		tlc5940_set_val(data.brightness);
		console.log("tlc5940 update " + data.brightness);
	});

	tlc5940_drv.stdout.on('data', function(data) {
		console.log('stdout: ' + data);
	})
};

exports.set = tlc5940_set_val;
exports.get = tlc5940_get_val;

exports.path = '/tlc5940';

exports.terminate = function() {
	tlc5940_drv.stdin.write("0\n");
};

function init() {
	var b = [];
	for (var i = 0; i < num_led; i++) {
		b.push(0);
	}
	tlc5940_set_val(b);
}

init();
