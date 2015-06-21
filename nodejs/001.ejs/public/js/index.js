var Index = function(pageTransition) {
	this.pageTransition = pageTransition;

	this.$page = $("#page-index");
	this.$doc = $("#index-doc");
	this.$link = $("#index-link");
	this.$toSky = $("#page-to-sky");
	this.$toStatus = $("#page-to-status");
	this.$reset = $("#reset");

	socket.on('system/reset/complete', function(data) {
		this.$reset.css('background-color', '');
	}.bind(this));

	this.$toSky.click(function(e) {
		this.pageTransition("#page-sky");
	}.bind(this));
	this.$toStatus.click(function(e) {
		this.pageTransition("#page-status");
	}.bind(this));

	this.$toSky.bind('touchstart', function(e) {
		this.pageTransition("#page-sky");
	}.bind(this));
	this.$toStatus.bind('touchstart', function(e) {
		this.pageTransition("#page-status");
	}.bind(this));
	this.$reset.bind('touchstart', function(e) {
		socket.emit('system/reset');
		this.$reset.css('background-color', 'red');
	}.bind(this));
};
Index.prototype.show = function() {
	this.resize();
	this.$page.css('display', 'block');

	this.$doc.hideTranslation(0, 80);
	this.$toSky.hideTranslation(0, 80);
	this.$toStatus.hideTranslation(0, 80);

	setTimeout(function() {
		this.$doc.appear(.4);
	}.bind(this), 100);
	setTimeout(function() {
		this.$toSky.appear(.4);
	}.bind(this), 300);
	setTimeout(function() {
		this.$toStatus.appear(.4);
	}.bind(this), 500);
};
Index.prototype.hidden = function() {
	this.$page.css('display', 'none');
};
Index.prototype.resize = function() {
	this.w = this.$page.width();
	this.h = this.$page.height();

	var shortSide = (this.w > this.h? this.h: this.w);
	var buttonRadius = Math.floor(shortSide * 0.18);
	var buttonSideMarigin = Math.floor(this.w/2 - shortSide*0.18);
	this.$doc
		.css('top', (this.h * 0.15) + 'px')
		.css('font-size', (this.h * 0.04) + 'px')
		.css('text-align', 'center');
	this.$link
		.css('font-size', this.h * 0.04 + 'px')
		.css('top', this.h * 0.47 + 'px');
	this.$toSky
		.css('background-image', 'url(/img/icon/sky02.png)')
		.css('background-size', buttonRadius + 'px')
		.css('border-radius', buttonRadius + 'px')
		.css('box-shadow', '0px 8px 16px rgba(0,0,0,0.4)')
		.css('line-height', buttonRadius + 'px')
		.css('height', buttonRadius + 'px')
		.css('width', buttonRadius + 'px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('color', '#fff')
		.css('top', '0px')
		.css('left', (buttonSideMarigin - buttonRadius/2) + 'px');
	this.$toStatus
		.css('background-image', 'url(/img/icon/status02.png)')
		.css('background-size', buttonRadius + 'px')
		.css('border-radius', buttonRadius + 'px')
		.css('box-shadow', '0px 8px 16px rgba(0,0,0,0.4)')
		.css('line-height', buttonRadius + 'px')
		.css('height', buttonRadius + 'px')
		.css('width', buttonRadius + 'px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('color', '#fff')
		.css('top', '0px')
		.css('right', (buttonSideMarigin - buttonRadius/2) + 'px');
	this.$reset
		.css('border-radius', (buttonRadius/2) + 'px')
		.css('box-shadow', '0px 4px 8px rgba(0,0,0,0.4)')
		.css('line-height', (buttonRadius/2) + 'px')
		.css('height', (buttonRadius/2) + 'px')
		.css('width', (buttonRadius/2) + 'px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('top', (this.h/2 + shortSide*0.4) + 'px')
		.css('right', (this.w/2 - buttonRadius/4) + 'px');
};
