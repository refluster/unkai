var exec = require('child_process').exec;

exports.index = function(req, res){
	// get user-agent
	var ua = JSON.stringify(req.headers['user-agent']);
	var is_mobile = false;

	if ((ua.indexOf('iPhone') > 0 && ua.indexOf('iPad') == -1) ||
		ua.indexOf('iPod') > 0 ||
		ua.indexOf('Android') > 0) {
		is_mobile = true;
	}
	res.render('led-ctrl', { title: 'LED control', host: req.headers.host, is_mobile: is_mobile });
};

exports.init_socket = function(io, client){
	client.on('led-ctrl switch', function(data) {
		// get humidity ,celsius, fahrenheit
		var cmd = "../../c/000.gpio-set/000.gpio-set 4 " + data.value;
		exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
			var humidity, celsius, fahrenheit;
			console.log('stdout: '+(stdout||'none'));
			console.log('stderr: '+(stderr||'none'));
		});
	});
};

exports.path = '/led-ctrl';
