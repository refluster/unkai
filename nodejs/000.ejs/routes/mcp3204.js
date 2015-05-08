var exec = require('child_process').exec;

function mcp3204_get_val(callback) {
	var illuminance, moisture;
	var cmd = "../../c/007.mcp3204/007.mcp3204";
	exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
		console.log('stdout: '+(stdout||'none'));
		console.log('stderr: '+(stderr||'none'));
		if (error !== null) {
			console.log('exec error: '+error);
		} else {
			var re = /illuminance: (\d+\.\d+) lux[\s\S]*moisture: (\d+\.\d+) %/;
			var re_match = stdout.match(re);
			if (re_match) {
				illuminance = re_match[1];
				moisture = re_match[2];
			}
		}
		callback(illuminance, moisture);
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
	res.render('mcp3204', { title: 'mcp3204', host: req.headers.host, is_mobile: is_mobile});
};

exports.init_socket = function(io, client){
	client.on('mcp3204 get_value', function(data) {
		mcp3204_get_val(function(illuminance, moisture) {
			client.emit('mcp3204 ret_value', {illuminance: illuminance, moisture: moisture});
		});
	});
};

exports.get = mcp3204_get_val;
exports.set = function(val){};

exports.path = '/mcp3204';
