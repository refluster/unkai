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

	var iconRadius = (this.w > this.h? this.h: this.w) * 0.18;
	var illuminancePosV = this.h * 0.5;
	var moisturePosV = this.h * 0.7;
	var iconPosH = this.w/2 - this.h * 0.2;
	var valuePosH = this.w/2 + this.h * 0.1;
	var fontSize = (this.w > this.h? this.h: this.w) * 0.05;

	this.$illuminanceIcon
		.css('position', 'absolute')
		.css('font-size', fontSize + "px")
		.css('-webkit-border-radius', iconRadius + 'px')
		.css('-moz-border-radius', iconRadius + 'px')
		.css('border-radius', iconRadius + 'px')
		.css('background-color', '#999')
		.css('height', iconRadius + 'px')
		.css('width', iconRadius + 'px')
		.css('line-height', iconRadius + 'px')
		.css('top', (illuminancePosV - iconRadius) + 'px')
		.css('left', (iconPosH) + 'px');
	this.$moistureIcon
		.css('position', 'absolute')
		.css('font-size', fontSize + "px")
		.css('-webkit-border-radius', iconRadius + 'px')
		.css('-moz-border-radius', iconRadius + 'px')
		.css('border-radius', iconRadius + 'px')
		.css('background-color', '#999')
		.css('height', iconRadius + 'px')
		.css('width', iconRadius + 'px')
		.css('line-height', iconRadius + 'px')
		.css('top', (moisturePosV - iconRadius) + 'px')
		.css('left', (iconPosH) + 'px');
	this.$illuminanceValue
		.css('position', 'absolute')
		.css('font-size', fontSize + "px")
		.css('line-height', iconRadius + 'px')
		.css('height', iconRadius + 'px')
		.css('top', (illuminancePosV - iconRadius) + 'px')
		.css('left', valuePosH + 'px');
	this.$moistureValue
		.css('position', 'absolute')
		.css('font-size', fontSize + "px")
		.css('line-height', iconRadius + 'px')
		.css('height', iconRadius + 'px')
		.css('top', (moisturePosV - iconRadius) + 'px')
		.css('left', valuePosH + 'px');
};
