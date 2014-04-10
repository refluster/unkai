
/*
 * GET home page.
 */

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
    res.render('index', { title: 'Express', host: req.headers.host, is_mobile: is_mobile});
};
