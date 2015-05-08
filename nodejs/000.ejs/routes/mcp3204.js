var exec = require('child_process').exec;

function mcp3204_get_val(callback) {
	var illuminance;
	var cmd = "../../c/007.mcp3204/007.mcp3204";
	exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
		console.log('stdout: '+(stdout||'none'));
		console.log('stderr: '+(stderr||'none'));
		if (error !== null) {
			console.log('exec error: '+error);
		} else {
			var re = /illuminance: (\d+\.\d+) lux/;
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

	if ((ua.indexOf('iPhone') > 0 && ua.indexOf('iPad') == -1) ||
		ua.indexOf('iPod') > 0 ||
		ua.indexOf('Android') > 0) {
		is_mobile = true;
	}
	res.render('mcp3204', { title: 'illuminance', host: req.headers.host, is_mobile: is_mobile});
};

exports.init_socket = function(io, client){
	client.on('illuminance get_value', function(data) {
		mcp3204_get_val(function(illuminance) {
			client.emit('illuminance ret_value', {illuminance: illuminance});
		});
	});
};

exports.get = mcp3204_get_val;
exports.set = function(val){};

exports.path = '/mcp3204';
