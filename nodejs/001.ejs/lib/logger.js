const LOG_DIR = 'public/data/';
const LOG_FILE = LOG_DIR +'sensor.tsv';
const LOGGING_INTERVAL = 60000;

var fs = require('fs');
var date_util = require('date-utils');

var timer;
var driver;

function record_data() {
	driver.getSensor(function(d) {
		var date = new Date().toFormat("YYYY/MM/DD HH24:MI:SS");
		var data = [date, d.illuminance, d.moisture, d.humidity, d.celsius].join("\t");
		fs.writeFile(LOG_FILE, data + "\n", {flag: 'a'}, function(err) {
			if (err) {
				console.log("logger: " + err);
			}
		});
	});
};

exports.init = function(_driver) {
	driver = _driver;
};

exports.start = function() {
	fs.mkdir(LOG_DIR, function(e){
		fs.exists(LOG_FILE, function(exists) {
			if (! exists) {
				var header = ['date', 'illuminance', 'moisture', 'humidity', 'celsius'].join("\t");
				fs.writeFile(LOG_FILE, header + "\n", {flag: 'w'}, function(err) {
					if (err) {
						console.log("logger: " + err);
					}
				});
			}
		});
	});

	timer = setInterval(record_data, LOGGING_INTERVAL);
};

exports.stop = function() {
	clearInterval(timer);
};


						
