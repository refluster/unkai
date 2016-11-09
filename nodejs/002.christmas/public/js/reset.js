var Reset = function(pageTransition) {
	this.pageTransition = pageTransition;

	this.$page = $("#page-reset");
	this.$status = $("#reset-statue");
	this.$toIndex = $("#reset-page-to-index");

	this.$toIndex.bind("touchend", function(e) {
		this.pageTransition("#page-index");
	}.bind(this));

	socket.on('system/reset/complete', function(data) {
		console.log('restarted');
		this.$status.html('restart complete');
	}.bind(this));
};
Reset.prototype.show = function() {
	this.resize();
	this.$status.html('restarting server...');
	this.$page.css('display', 'block');
	socket.emit('system/reset');
	console.log('system/reset emit');
};
Reset.prototype.hidden = function() {
	this.$page.css('display', 'none');
};
Reset.prototype.resize = function() {
	this.w = this.$page.width();
	this.h = this.$page.height();

	var shortSide = (this.w > this.h? this.h: this.w);
	var buttonRadius = Math.floor(shortSide * 0.18);
	var buttonSideMarigin = Math.floor(this.w/2 - shortSide*0.18);
	this.$status
		.css('top', (this.h / 2) + 'px')
		.css('font-size', (shortSide * 0.04) + 'px')
		.css('text-align', 'center');
	this.$toIndex.bind("touchend", function(e) {
		this.pageTransition("#page-index");
	}.bind(this));
};
