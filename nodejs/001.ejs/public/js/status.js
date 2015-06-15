// send message
function send_request() {
	socket.emit('sensor/get');
}

var Status = function(pageTransition) {
	this.thisIsStatus;
	this.pageTransition = pageTransition;
	
	this.$toIndex = $(".page-to-index");
	this.$page = $('#page-status');
	
	this.$toIndex.bind("touchend", function(e) {
		this.pageTransition("#page-index");
	}.bind(this));

	this.update_time = 0;
	socket.on('connect', function(msg) {
		console.log('connect');
	});
	
	// receive message
	socket.on('sensor/response', function(data) {
		this.update_time ++;
		console.log('sensor/response');
		$("#illuminance").html(data.illuminance);
		$("#moisture").html(data.moisture);
	});
};
Status.prototype.show = function() {
	this.$page.css('display', 'block');
	this.timer = setInterval(send_request, 1000);
};
Status.prototype.hidden = function() {
	this.$page.css('display', 'none');
	clearInterval(this.timer);
};
