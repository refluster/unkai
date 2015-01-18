// logger daemon

var fs = require('fs');
var date_util = require('date-utils');

var camera, humidity, led_ctrl, tlc5940, njl5702;
var devices = [];

function is(type, obj) {
    var clas = Object.prototype.toString.call(obj).slice(8, -1);
    return obj !== undefined && obj !== null && clas === type;
}

var record;

function record_data() {
	var dt = new Date();
	var date = dt.toFormat("YYYY/MM/DD HH24:MI:SS");
	var devnum = 0;

	function correct_data_from_a_device(value) {
		var dev = devices[devnum];
		devnum ++;
		record += ',' + value;
		if (dev != undefined) {
			dev.get(correct_data_from_a_device)
		} else {
			var writeData = record + "\n";
			fs.writeFile('writetest.txt', writeData, {flag: 'a'}, function(err) {
				console.log(err);
			});
		}
	}

	record = '';
	correct_data_from_a_device(date);
};

exports.dev_add = function(dev) {
	devices.push(dev);
};

exports.start = function() {
	setInterval(record_data, 1000);
}


