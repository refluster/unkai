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
    console.log('chat started on ' + req.headers.host);
    res.render('chat', { title: 'simple chat', host: req.headers.host, is_mobile: is_mobile});
};

exports.init_socket = function(io, client){
    // chat message received
    client.on('chat send_client_to_server', function(data) {
        // broadcast to all clients
        console.log("message");
        io.sockets.emit('chat send_server_to_clients', { value: data.value });
    });
}
