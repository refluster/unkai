exports.index = function(req, res){
    console.log('chat started');
    res.render('chat', { title: 'Express' });
};
