var exec = require('child_process').exec;
var date_util = require('date-utils');
var client = null;

function camera_capture(callback) {
    var dt = new Date();
    var date = dt.toFormat("YYYY-MM-DD_HH24:MI:SS");

	// get humidity ,celsius, fahrenheit
	var cmd = '../../c/005.camera/005.camera -r 320x240 -o log/' + date + '.jpg';
	exec(cmd, {timeout: 3000}, function(error, stdout, stderr) {
		console.log('stdout: '+(stdout||'none'));
		console.log('stderr: '+(stderr||'none'));
		if (client) {
			client.emit('camera update', {value: 'img/' + date + '.jpg'});
		}
		callback(date + '.jpg');
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
	res.render('camera', { title: 'Camera', host: req.headers.host, is_mobile: is_mobile });
};

exports.init_socket = function(io, _client){
	client = _client;
};

exports.get = camera_capture;

exports.path = '/camera';

