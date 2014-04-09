var exec = require('child_process').exec;

exports.index = function(req, res){
    console.log('led-ctrl @ ' + req.headers.host);
    res.render('led-ctrl', { title: 'LED control', host: req.headers.host });
};

exports.init_socket = function(io, client){
    client.on('led-ctrl switch', function(data) {
		console.log("led switch " + data.value);
		
		// get humidity ,celsius, fahrenheit
		var cmd = "../../c/000.gpio-set/000.gpio-set 4 " + data.value;
		exec(cmd, {timeout: 1000}, function(error, stdout, stderr) {
			var humidity, celsius, fahrenheit;
			console.log('stdout: '+(stdout||'none'));
			console.log('stderr: '+(stderr||'none'));
			if (error !== null) {
				console.log('exec error: '+error);
			} else {
			    console.log('exec ok');
            }
		});
	});
};
