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

	this.$doc
		.css('top', '30px')
		.css('text-align', 'center');
	this.$link
		.css('top', '300px')
		.css('top', '300px')
	this.$toSky
		.css('-webkit-border-radius', '100px')
		.css('-moz-border-radius', '100px')
		.css('border-radius', '100px')
		.css('background-color', '#bbb')
		.css('line-height', '100px')
		.css('height', '100px')
		.css('width', '100px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('top', '0px')
		.css('left', '100px');
	this.$toStatus
		.css('-webkit-border-radius', '100px')
		.css('-moz-border-radius', '100px')
		.css('border-radius', '100px')
		.css('background-color', '#bbb')
		.css('line-height', '100px')
		.css('height', '100px')
		.css('width', '100px')
		.css('text-align', 'center')
		.css('position', 'absolute')
		.css('top', '0px')
		.css('left', '400px');
};
Index.prototype.show = function() {
	this.$page.css('display', 'block');
};
Index.prototype.hidden = function() {
	this.$page.css('display', 'none');
};
