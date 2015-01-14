var exec = require('child_process').exec;

function njl7502_get_val(callback) {
	var illuminance;
	var cmd = "../../c/007.njl7502/007.njl7502";
	exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
		console.log('stdout: '+(stdout||'none'));
		console.log('stderr: '+(stderr||'none'));
		if (error !== null) {
			console.log('exec error: '+error);
		} else {
			var re = /illuminance:(\d+\.\d+)/;
			var re_match = stdout.match(re);
			if (re_match) {
				illuminance = re_match[1];
			}
		}
		callback(illuminance);
	});
}

exports.index = function(req, res){
	// get user-agent
	var ua = JSON.stringify(req.headers['user-agent']);
	var is_mobile = false;
	console.log('UA ' + ua);
	if ((ua.indexOf('iPhone') > 0 && ua.indexOf('iPad') == -1) ||
		ua.indexOf('iPod') > 0 ||
		ua.indexOf('Android') > 0) {
		is_mobile = true;
	}
	console.log('UA ' + ua);
	console.log('getting illuminance from ' + req.headers.host);
	res.render('njl7502', { title: 'illuminance', host: req.headers.host, is_mobile: is_mobile});
};

exports.init_socket = function(io, client){
	client.on('illuminance get_value', function(data) {
		var illuminance;
		console.log("get illuminance");
		njl7502_get_val(function(illuminance) {
			client.emit('illuminance ret_value', {illuminance: illuminance});
		});
	});
};

exports.get = njl7502_get_val;
exports.set = function(val){};
