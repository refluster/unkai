const UPDATE_INTERVAL = 3000; // msec order
var timer;
var driver;
var processing = false;

function updateCondition() {
	const MIST_GEN_LOWER_MOISTURE = 220;
	const MIN_MOISTURE = 0;

	driver.getSensor(function(data) {
		if (data.moisture < MIST_GEN_LOWER_MOISTURE) {
			var ratio = (MIST_GEN_LOWER_MOISTURE - data.moisture) /
				(MIST_GEN_LOWER_MOISTURE - MIN_MOISTURE);
			driver.setMistgen(Math.floor(ratio * 100));
		} else {
			driver.setMistgen(0);
		}
	});
}

exports.init = function(_driver) {
	driver = _driver;
};

exports.start = function() {
	if (processing) {
		clearTimeout(timer);
	}
	timer = setInterval(updateCondition, UPDATE_INTERVAL);
	processing = true;
}

exports.stop = function() {
	if (processing) {
		clearInterval(timer);
	}
	processing = false;
}
