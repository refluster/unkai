var exec = require('child_process').exec;

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
	res.render('update', { title: 'Software update', host: req.headers.host, is_mobile: is_mobile });
};

var command = [
	{cmdline: "ls ~/", complete: "ls ~/ done", fail: "ls ~/ failed"},
	{cmdline: "ls ~/work", complete: "ls ~/work done", fail: "ls ~/work failed"},
	{cmdline: "ls ~/work/unka", complete: "ls ~/work/unka done", fail: "ls ~/work/unka failed"},
	{cmdline: "ls ~/work/unkai", complete: "ls ~/work/unkai done", fail: "ls ~/work/unkai failed"},
];

function exec_commands(client, command_idx) {
	if (command.length <= command_idx) {
		return;
	}
	
	var cmd = command[command_idx];
	exec(cmd.cmdline, {timeout: 1000}, function(error, stdout, stderr) {
		if (error == null) {
			client.emit("update report", {complete: true, msg: cmd.complete});
			exec_commands(client, command_idx + 1);
		} else {
			client.emit("update report", {complete: false,
										  msg: cmd.fail,
										  stdout: stdout,
										  stderr: stderr
										 });
		}
	});
}

exports.init_socket = function(io, client){
	client.emit('update welcome');

	client.on('update start-update', function(data) {
		console.log('update start');
		exec_commands(client, 0);
	});
};

exports.path = '/update';
