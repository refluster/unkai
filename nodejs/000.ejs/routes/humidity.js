exports.index = function(req, res){
    console.log('getting humidity from ' + req.headers.host);
    res.render('humidity', { title: 'humidity and temperature', host: req.headers.host });
};

exports.init_socket = function(io, client){
    // for humidity
    client.on('humidity get_value', function(data) {
        console.log("get humidity");
        client.emit('humidity ret_value', { humidity: 30, temperature: 23});
    });
};
