// init mistgen driver daemon
var spawn = require('child_process').spawn;
var mistgen_drv = spawn("../../c/006.mistgen/006.mistgen");
var mistgen_percent = 0;

function mistgen_set_val(value) {
	var cmd = "1 " + value + "\n";
	mistgen_drv.stdin.write(cmd);
	mistgen_percent = value;
}

function mistgen_get_val(callback) {
	callback(mistgen_percent);
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
	res.render('mistgen', { title: 'mist control', host: req.headers.host, is_mobile: is_mobile});
};

exports.init_socket = function(io, client){
	client.on('mistgen update', function(data) {
		mistgen_set_val(data.value);
	});

	mistgen_drv.stdout.on('data', function(data) {
		console.log('mistgen stdout: ' + data);
	})
};

exports.set = mistgen_set_val;
exports.get = mistgen_get_val;
