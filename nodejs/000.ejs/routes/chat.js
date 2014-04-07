exports.index = function(req, res){
    console.log('chat started on ' + req.headers.host);
    res.render('chat', { title: 'simple chat', host: req.headers.host });
};

exports.init_socket = function(io, client){
    // chat message received
    client.on('chat send_client_to_server', function(data) {
        // broadcast to all clients
        console.log("message");
        io.sockets.emit('chat send_server_to_clients', { value: data.value });
    });
}
