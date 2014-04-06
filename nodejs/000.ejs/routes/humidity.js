exports.index = function(req, res){
    console.log('getting humidity from ' + req.headers.host);
    res.render('humidity', { title: 'humidity and temperature', host: req.headers.host });
};
