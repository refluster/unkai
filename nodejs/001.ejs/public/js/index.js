var Index = function(pageTransition) {
	this.pageTransition = pageTransition;
	$("#page-to-space").bind("touchend", function(e) {
		this.pageTransition("#page-space");
	}.bind(this));
};
Index.prototype.show = function() {
	$('#page-index').css('display', 'block');
};
Index.prototype.hidden = function() {
	$('#page-index').css('display', 'none');
};
