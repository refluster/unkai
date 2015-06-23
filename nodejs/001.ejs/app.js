var App = function() {
	this.express = require('express');
	this.http = require('http');
	this.path = require('path');
	
	this.app = this.express();
	this.server;
};

App.prototype.stop = function() {
	// terminate background processes
	//   T.B.D.

    // close http server
    this.server.close();
}

App.prototype.isMobile = function(ua) {
	if ((ua.indexOf('iPhone') > 0 && ua.indexOf('iPad') == -1) ||
		ua.indexOf('iPod') > 0 ||
		ua.indexOf('Android') > 0) {
		return true;
	}
	return false;
}

App.prototype.start = function() {
	// server settings
	this.app.set('port', process.env.PORT || 3000);
	this.app.set('view engine', 'ejs');
	this.app.use(this.express.static(this.path.join(__dirname, 'public')));

	// start server
	this.server = this.http.createServer(this.app);
	this.server.listen(this.app.get('port'), function() {
		console.log('Express server listening on port ' + this.app.get('port'));
	}.bind(this));

	// driver
	this.driver = require('./lib/drv');
	this.driver.start();

	// conditioner
	this.conditioner = require('./lib/conditioner');
	this.conditioner.init(this.driver);
	this.conditioner.start();

	// logger
	this.logger = require('./lib/logger');
	this.logger.init(this.driver);
	this.logger.start();

	// socket io
	this.socket = require('./lib/socket');
	this.socket.init(this.driver, this.server, this.conditioner);

	// pages
	this.app.get("/status", function(req, res) {
		var mobile = false;
		
		if (this.isMobile(JSON.stringify(req.headers['user-agent']))) {
			mobile = true;
		}
		res.render('status', {host: req.headers.host, is_mobile: mobile});
	}.bind(this));

	this.app.get("/", function(req, res) {
		var mobile = false;
		
		if (this.isMobile(JSON.stringify(req.headers['user-agent']))) {
			mobile = true;
		}
		res.render('index', {host: req.headers.host, is_mobile: mobile});
	}.bind(this));
}

app = new App();
app.start();
