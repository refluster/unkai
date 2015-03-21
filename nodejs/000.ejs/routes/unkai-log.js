// logger daemon

var fs = require('fs');
var date_util = require('date-utils');
var exec = require('child_process').exec;

const RECORD_INTERVAL_MSEC = 60000;

var devices = [];
var interval_object;

function is(type, obj) {
    var clas = Object.prototype.toString.call(obj).slice(8, -1);
    return obj !== undefined && obj !== null && clas === type;
}

function record_data() {
	var dt = new Date();
	var date = dt.toFormat("YYYY/MM/DD HH24:MI:SS");
	var devnum = 0;
	var record = [];
	
	function correct_data_from_a_device(value) {
		var dev = devices[devnum];
		devnum ++;
		record.push(value);
		if (dev != undefined) {
			dev.get(correct_data_from_a_device)
		} else {
			var writeData = record.join(',') + "\n";
			fs.writeFile('log/data.txt', writeData, {flag: 'a'}, function(err) {
				console.log(err);
			});
		}
	}

	correct_data_from_a_device(date);
};

exports.dev_add = function(dev) {
	devices.push(dev);
};

/*
exports.start = function() {
	setInterval(record_data, RECORD_INTERVAL_MSEC);
}
*/

function start_logging() {
	interval_object = setInterval(record_data, RECORD_INTERVAL_MSEC);
}

function stop_logging() {
	clearInterval(interval_object);
	interval_object = null;
}

//////////////////////////////

exports.index = function(req, res){
	// get user-agent
	var ua = JSON.stringify(req.headers['user-agent']);
	var is_mobile = false;

	if ((ua.indexOf('iPhone') > 0 && ua.indexOf('iPad') == -1) ||
		ua.indexOf('iPod') > 0 ||
		ua.indexOf('Android') > 0) {
		is_mobile = true;
	}
	res.render('unkai-log', { title: 'Unkai log', host: req.headers.host, is_mobile: is_mobile });
};

exports.init_socket = function(io, client){
	client.emit('unkai-log status', {value: (interval_object == null? "start": "false")});

	client.on('unkai-log get', function(data) {
		// must be implemented to archve and download the data
		var dt = new Date();
		var file = "log.tar.gz";
		var cmd = "tar czf public/" + file + " log";
		console.log('unkai-log get');
		exec(cmd, {timeout: 30000}, function(error, stdout, stderr) {
			client.emit("unkai-log get-res", {href: file, date: dt.toFormat("YYYY/MM/DD HH24:MI:SS")});
			console.log('stdout: '+(stdout||'none'));
			console.log('stderr: '+(stderr||'none'));
		});
	});

	client.on('unkai-log start_stop', function(data) {
		console.log('start stop');
		if (data.value == 'start') {
			console.log('start int');
			start_logging();
			var cmd = "rm -f log/*";
			exec(cmd, {timeout: 30000}, function(error, stdout, stderr) {
				client.emit('unkai-log status', {value: "stop"});
				console.log('stdout: '+(stdout||'none'));
				console.log('stderr: '+(stderr||'none'));
			});
		} else if (data.value == 'stop') {
			console.log('stop  int');
			stop_logging();
			client.emit('unkai-log status', {value: "start"});
		}
	});
};

exports.path = '/unkai-log';
