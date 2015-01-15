// logger daemon

var fs = require('fs');
var date_util = require('date-utils');

var camera, humidity, led_ctrl, tlc5940, njl5702;
var devices = [];

function is(type, obj) {
    var clas = Object.prototype.toString.call(obj).slice(8, -1);
    return obj !== undefined && obj !== null && clas === type;
}

function record_data() {
	var dt = new Date();
	var record = "";
	var date = dt.toFormat("YYYY/MM/DD HH24:MI:SS");

	function correct_data_from_a_device(value) {
		var dev = devices.pop();

		if (dev != undefined) {
			dev.get(correct_data_from_a_device)
			devices.push(dev);
		}

		if (value == undefined) {
			var writeData = date + record + "\n";
			fs.writeFile('writetest.txt', writeData, {flag: 'a'}, function(err) {
				console.log(err);
			});
				
			console.log("record : " + date + record);
		} else {
			record += "," + value;
		}
	}

	correct_data_from_a_device();
}

exports.init = function(_camera, _humidity, _led_ctrl, _tlc5940, _njl5702) {
	camera = _camera;
	humidity = _humidity;
	led_ctrl = led_ctrl;
	tlc5940 = _tlc5940;
	njl5702 = _njl5702;

	devices.push(camera);
	devices.push(humidity);
	devices.push(led_ctrl);
	devices.push(tlc5940);
	devices.push(njl5702);
};

exports.start = function() {
	
};

var test_dev = function() {
	this.get = function(callback) {
		callback(101);
	};
};

var test_dev1 = function() {
	this.get = function(callback) {
		callback(102);
	};
};


function test() {
	console.log("test()");

	// set device
	devices.push(new test_dev());
	devices.push(new test_dev1());
	devices.push(new test_dev1());

	// record data
	//record_data();
	
	setInterval(record_data, 1000);
}

test();
