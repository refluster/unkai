var colorData = {r: 0, g: 0, b: 0, val: 1};
var baseDistance;

function setColor(colorData) {
	var r, g, b;
	var color = $("#color");
	var msg = document.getElementById("msg");

	r = colorData.r*colorData.val;
	g = colorData.g*colorData.val;
	b = colorData.b*colorData.val;

	brightness_update_from_hue([parseInt(r*4095/255), parseInt(g*4095/255), parseInt(b*4095/255)]);

	msg.innerHTML = parseInt(r) + ' ' + parseInt(g) + ' ' + parseInt(b);

	color.css('backgroundColor', 'rgb(' +
			  parseInt(r) + ',' +
			  parseInt(g) + ',' +
			  parseInt(b) + ')');
}

// ½é´ü²½
function init_color() {
	var canvas = $("#canvas");
	var context = canvas[0].getContext("2d");
	var image = new Image();
	
	canvas.bind("touchstart", touchStartHandler);
	canvas.bind("touchmove", touchMoveHandler);
	canvas.bind("touchend", touchEndHandler);
	canvas.mousedown(mouseDownHandler);

	image.src = "color.jpg";
	image.onload = function() {
		context.drawImage(image, 0, 0, canvas[0].width, canvas[0].height);
	}
}

function changeHueSaturation(x, y) {
	var canvas = $("#canvas");
	var context = canvas[0].getContext("2d");
	var imagedata = context.getImageData(x, y, 1, 1);
	var data = Array.prototype.slice.apply(imagedata.data);
	
	colorData.r = data[0];
	colorData.g = data[1];
	colorData.b = data[2];
	setColor(colorData);
}

function changeValue(inc) {
	colorData.val += inc;
	if (colorData.val > 1.0) {
		colorData.val = 1;
	} else if (colorData.val < 0.0) {
		colorData.val = 0;
	}
	setColor(colorData);
}

function touchStartHandler(e) {
	if (e.originalEvent.touches.length == 1) {
		var rect = e.target.getBoundingClientRect();
		var x = e.originalEvent.touches[0].clientX - rect.left;
		var y = e.originalEvent.touches[0].clientY - rect.top;
		changeHueSaturation(x, y);
	} else {
		baseDistance = Math.sqrt((e.originalEvent.touches[0].pageX - e.originalEvent.touches[1].pageX)*
								 (e.originalEvent.touches[0].pageX - e.originalEvent.touches[1].pageX) + 
								 (e.originalEvent.touches[0].pageY - e.originalEvent.touches[1].pageY)*
								 (e.originalEvent.touches[0].pageY - e.originalEvent.touches[1].pageY));
	}

	e.preventDefault();
}

function touchMoveHandler(e) {
	if (e.originalEvent.touches.length == 1) {
		var rect = e.target.getBoundingClientRect();
		var x = e.originalEvent.touches[0].clientX - rect.left;
		var y = e.originalEvent.touches[0].clientY - rect.top;
		changeHueSaturation(x, y);
	} else {
		var newDistance = Math.sqrt((e.originalEvent.touches[0].pageX - e.originalEvent.touches[1].pageX)*
									(e.originalEvent.touches[0].pageX - e.originalEvent.touches[1].pageX) + 
									(e.originalEvent.touches[0].pageY - e.originalEvent.touches[1].pageY)*
									(e.originalEvent.touches[0].pageY - e.originalEvent.touches[1].pageY));
		var coe = 0.02;
		changeValue((newDistance - baseDistance)*coe);
		baseDistance = newDistance;
	}

	e.preventDefault();
}

function touchEndHandler(e) {
	e.preventDefault();
}

function mouseDownHandler(e) {
    var rect = e.target.getBoundingClientRect();
    var x = e.clientX - rect.left;
    var y = e.clientY - rect.top;

	changeHueSaturation(x, y);
}
