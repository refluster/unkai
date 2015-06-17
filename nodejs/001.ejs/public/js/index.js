var Index = function(pageTransition) {
	this.pageTransition = pageTransition;

	this.$page = $("#page-index");
	this.$doc = $("#index-doc");
	this.$link = $("#index-link");
	this.$toSky = $("#page-to-sky");
	this.$toStatus = $("#page-to-status");

	this.$toSky.bind("touchend", function(e) {
		this.pageTransition("#page-sky");
	}.bind(this));
	this.$toStatus.bind("touchend", function(e) {
		this.pageTransition("#page-status");
	}.bind(this));
};
Index.prototype.show = function() {
	this.resize();
	this.$page.css('display', 'block');
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
		.css('line-height', buttonRadius + 'px')
		.css('height', buttonRadius + 'px')
		.css('width', buttonRadius + 'px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('top', '0px')
		.css('left', (buttonSideMarigin - buttonRadius/2) + 'px');
	this.$toStatus
		.css('background-image', 'url(/img/icon/status02.png)')
		.css('background-size', buttonRadius + 'px')
		.css('line-height', buttonRadius + 'px')
		.css('height', buttonRadius + 'px')
		.css('width', buttonRadius + 'px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('top', '0px')
		.css('right', (buttonSideMarigin - buttonRadius/2) + 'px');
};
