var exec = require('child_process').exec;

function humidity_get_val(callback) {
	var humidity, celsius, fahrenheit;

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
		callback([humidity, celsius, fahrenheit]);
	});
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
	res.render('humidity', { title: 'humidity and temperature',
							 host: req.headers.host, is_mobile: is_mobile});
};

exports.init_socket = function(io, client){
	// for humidity
	client.on('humidity get_value', function(data) {
		humidity_get_val(function(value) {
			client.emit('humidity ret_value', { humidity: value[0],
												celsius: value[1],
												fahrenheit: value[2]});
		});
	});
};

exports.get = humidity_get_val;
exports.set = function(val){};
