// init tlc5940 driver daemon
var spawn = require('child_process').spawn;
var tlc5940_drv = spawn("../../c/003.tlc5940/003.tlc5940", ["-p", "1000", "-t", "500", "-n", "4"]);

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
	console.log('tlc5940 @ ' + req.headers.host);
	res.render('tlc5940', { title: 'tlc5940 control', host: req.headers.host, is_mobile: is_mobile });
};

exports.init_socket = function(io, client){
	client.on('tlc5940 update', function(data) {
		var cmd = "1"; // update cmd
		for (var i = 0; i < 4; i++) {
			cmd += " " + data.brightness[i];
		}
		cmd += "\n";
		tlc5940_drv.stdin.write(cmd);
		console.log("tlc5940 update " + data.brightness);
	});

	tlc5940_drv.stdout.on('data', function(data) {
		console.log('stdout: ' + data);
	})
};
