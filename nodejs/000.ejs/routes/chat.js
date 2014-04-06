exports.index = function(req, res){
    console.log('chat started on ' + req.headers.host);
    res.render('chat', { title: 'simple chat', host: req.headers.host });
};
