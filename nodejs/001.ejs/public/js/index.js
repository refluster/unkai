var Index = function(pageTransition) {
	this.pageTransition = pageTransition;

	this.$page = $("#page-index");
	this.$doc = $("#index-doc");
	this.$link = $("#index-link");
	this.$toSky = $("#page-to-sky");
	this.$toStatus = $("#page-to-status");

	this.w = this.$page.width();
	this.h = this.$page.height();

	this.$toSky.bind("touchend", function(e) {
		this.pageTransition("#page-sky");
	}.bind(this));
	this.$toStatus.bind("touchend", function(e) {
		this.pageTransition("#page-status");
	}.bind(this));

	var buttonRadius = (this.w > this.h? this.h: this.w) * 0.18;
	this.$doc
		.css('top', (this.h * 0.15) + 'px')
		.css('font-size', (this.h * 0.04) + 'px')
		.css('text-align', 'center');
	this.$link
		.css('top', this.h * 0.47 + 'px')
	this.$toSky
		.css('-webkit-border-radius', buttonRadius + 'px')
		.css('-moz-border-radius', buttonRadius + 'px')
		.css('border-radius', buttonRadius + 'px')
		.css('background-color', '#bbb')
		.css('line-height', buttonRadius + 'px')
		.css('height', buttonRadius + 'px')
		.css('width', buttonRadius + 'px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('top', '0px')
		.css('left', (this.w/2 - this.h*0.08 - buttonRadius) + 'px');
	this.$toStatus
		.css('-webkit-border-radius', buttonRadius + 'px')
		.css('-moz-border-radius', buttonRadius + 'px')
		.css('border-radius', buttonRadius + 'px')
		.css('background-color', '#bbb')
		.css('line-height', buttonRadius + 'px')
		.css('height', buttonRadius + 'px')
		.css('width', buttonRadius + 'px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('top', '0px')
		.css('right', (this.w/2 - this.h*0.08 - buttonRadius) + 'px');
};
Index.prototype.show = function() {
	this.$page.css('display', 'block');
};
Index.prototype.hidden = function() {
	this.$page.css('display', 'none');
};
