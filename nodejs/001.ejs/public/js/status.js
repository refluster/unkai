// send message
function send_request() {
	socket.emit('sensor/get');
}

var Status = function(pageTransition) {
	this.pageTransition = pageTransition;

	this.$illuminanceIcon = $("#status-illuminance-icon");
	this.$illuminanceValue = $("#status-illuminance-val");
	this.$moistureIcon = $("#status-moisture-icon");
	this.$moistureValue = $("#status-moisture-val");
	this.$toIndex = $(".page-to-index");
	this.$page = $('#page-status');
	
	this.$toIndex.bind("touchend", function(e) {
		this.pageTransition("#page-index");
	}.bind(this));

	this.update_time = 0;

	// receive message
	socket.on('sensor/response', function(data) {
		console.log('sensor/response');
		this.update_time ++;
		this.$illuminanceValue.html(data.illuminance);
		this.$moistureValue.html(data.moisture);
	}.bind(this));
};
Status.prototype.show = function() {
	this.resize();
	this.$page.css('display', 'block');
	this.timer = setInterval(send_request, 1000);
};
Status.prototype.hidden = function() {
	this.$page.css('display', 'none');
	clearInterval(this.timer);
};
Status.prototype.resize = function() {
	this.w = this.$page.width();
	this.h = this.$page.height();

	var shortSide = (this.w > this.h? this.h: this.w);
	var iconSize = Math.floor(shortSide * 0.15);
	var valueWidth = shortSide * 0.18 * 2;
	var illuminancePosH = Math.floor(this.w/2 - shortSide * 0.18);
	var moisturePosH = Math.floor(this.w/2 + shortSide * 0.18);
	var iconPosV = Math.floor(this.h/2 - shortSide * 0.1);
	var valuePosV = Math.floor(this.h/2 + shortSide * 0.15);
	var fontSize = Math.floor(shortSide * 0.08);

	this.$illuminanceIcon
		.css('position', 'absolute')
		.css('font-family', "sans-serif")
		.css('font-size', fontSize + "px")
		.css('background-image', 'url(/img/icon/illuminance.png)')
		.css('background-size', iconSize + 'px')
		.css('height', iconSize + 'px')
		.css('width', iconSize + 'px')
		.css('line-height', iconSize + 'px')
		.css('top', (iconPosV - iconSize/2) + 'px')
		.css('left', (illuminancePosH - iconSize/2) + 'px');
	this.$moistureIcon
		.css('position', 'absolute')
		.css('font-family', "sans-serif")
		.css('font-size', fontSize + "px")
		.css('background-image', 'url(/img/icon/moisture.png)')
		.css('background-size', iconSize + 'px')
		.css('height', iconSize + 'px')
		.css('width', iconSize + 'px')
		.css('line-height', iconSize + 'px')
		.css('top', (iconPosV - iconSize/2) + 'px')
		.css('left', (moisturePosH - iconSize/2) + 'px');
	this.$illuminanceValue
		.css('position', 'absolute')
		.css('font-family', "sans-serif")
		.css('font-size', fontSize + "px")
		.css('line-height', iconSize + 'px')
		.css('height', iconSize + 'px')
		.css('width', valueWidth + 'px')
		.css('text-align', "center")
		.css('top', (valuePosV - iconSize/2) + 'px')
		.css('left', (illuminancePosH - valueWidth/2) + 'px');
	this.$moistureValue
		.css('position', 'absolute')
		.css('font-family', "sans-serif")
		.css('font-size', fontSize + "px")
		.css('line-height', iconSize + 'px')
		.css('height', iconSize + 'px')
		.css('width', valueWidth + 'px')
		.css('text-align', "center")
		.css('top', (valuePosV - iconSize/2) + 'px')
		.css('left', (moisturePosH - valueWidth/2) + 'px');
};
