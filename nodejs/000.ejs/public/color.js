var msg;
var colorData = {r: 0, g: 0, b: 0, val: 1};
var baseDistance;

function toColor(colorData) {
	var r, g, b;

	r = colorData.r*colorData.val;
	g = colorData.g*colorData.val;
	b = colorData.b*colorData.val;

	brightness_update_from_hue([parseInt(r*4095/255), parseInt(g*4095/255), parseInt(b*4095/255)]);

	msg.innerHTML = parseInt(r) + ' ' + parseInt(g) + ' ' + parseInt(b);

	return 'rgb(' +
		parseInt(r) + ',' +
		parseInt(g) + ',' +
		parseInt(b) + ')';
}

// ½é´ü²½
function init_color() {
	var canvas = document.getElementById("canvas");
	var context = canvas.getContext("2d");
	var color = document.getElementById("color");
	
	msg = document.getElementById("msg");
	
	var image = new Image();
	
	canvas.addEventListener("touchstart", touchStartHandler, false);
	canvas.addEventListener("touchmove", touchMoveHandler, false);
	canvas.addEventListener("touchend", touchEndHandler, false);

	document.addEventListener("mousedown", mouseDownHandler, false);

	image.src = "color.jpg";
	image.onload = function() {
		context.drawImage(image, 0, 0, canvas.width, canvas.height);
	}
}

function changeHueSaturation(x, y) {
	var canvas = document.getElementById("canvas");
	var context = canvas.getContext("2d");
	var imagedata = context.getImageData(x, y, 1, 1);
	var data = Array.prototype.slice.apply(imagedata.data);
	var color = document.getElementById("color");
	
	colorData.r = data[0];
	colorData.g = data[1];
	colorData.b = data[2];
	color.style.backgroundColor = toColor(colorData);
	//document.bgColor = toColor(colorData);
}

function changeValue(inc) {
	colorData.val += inc;
	if (colorData.val > 1.0) {
		colorData.val = 1;
	} else if (colorData.val < 0.0) {
		colorData.val = 0;
	}
	color.style.backgroundColor = toColor(colorData);
}

function touchStartHandler(event) {
	if (event.touches.length == 1) {
		var rect = event.target.getBoundingClientRect();
		var x = event.touches[0].clientX - rect.left;
		var y = event.touches[0].clientY - rect.top;
		changeHueSaturation(x, y);
	} else {
		baseDistance = Math.sqrt((event.touches[0].pageX - event.touches[1].pageX)*
								 (event.touches[0].pageX - event.touches[1].pageX) + 
								 (event.touches[0].pageY - event.touches[1].pageY)*
								 (event.touches[0].pageY - event.touches[1].pageY));
	}

	event.preventDefault();
}

function touchMoveHandler(event) {
	var canvas = document.getElementById("canvas");

	if (event.touches.length == 1) {
		var rect = event.target.getBoundingClientRect();
		var x = event.touches[0].clientX - rect.left;
		var y = event.touches[0].clientY - rect.top;
		changeHueSaturation(x, y);
	} else {
		var newDistance = Math.sqrt((event.touches[0].pageX - event.touches[1].pageX)*
									(event.touches[0].pageX - event.touches[1].pageX) + 
									(event.touches[0].pageY - event.touches[1].pageY)*
									(event.touches[0].pageY - event.touches[1].pageY));
		var coe = 0.02;
		changeValue((newDistance - baseDistance)*coe);
		baseDistance = newDistance;
	}

	event.preventDefault();
}

function touchEndHandler(event) {
	event.preventDefault();
}

function mouseDownHandler(event) {
    var rect = event.target.getBoundingClientRect();
    var x = event.clientX - rect.left;
    var y = event.clientY - rect.top;

	changeHueSaturation(x, y);
}
