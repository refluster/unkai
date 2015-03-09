var exec = require('child_process').exec;
var server_stop;

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
	{cmdline: "make clean -C ../..", msg: "clean built objects ..."},
	{cmdline: "git pull", msg: "update program code ..."},
	{cmdline: "make -C ../..", msg: "build software ..."},
	{callback: server_stop},
	{callback: process.exit},
];

function exec_commands(client, command_idx) {
	if (command.length <= command_idx) {
		return;
	}

	var cmd = command[command_idx];

	client.emit("update process", {msg: cmd.msg});

	if (cmd.cmdline) {
		exec(cmd.cmdline, {timeout: 1200000}, function(error, stdout, stderr) {
			if (error == null) {
				client.emit("update process-result", {result: true});
				exec_commands(client, command_idx + 1);
			} else {
				client.emit("update process-result", {result: false,
													  stdout: stdout,
													  stderr: stderr
													 });
				exec_commands(client, command_idx + 1);
			}
		});
	} else if (cmd.callback) {
		cmd.callback();
		exec_commands(client, command_idx + 1);
	} else {
		exec_commands(client, command_idx + 1);
	}
}

exports.init_socket = function(io, client){
	client.on('update start-update', function(data) {
		exec_commands(client, 0);
	});
};

exports.set_close_handler = function(_server_stop) {
	server_stop = _server_stop;
}

exports.path = '/update';
