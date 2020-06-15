const char JS_SLIDER[] PROGMEM = R"=====(
function rkmd_rangeSlider(selector){var self,slider_width,slider_offset,curnt,sliderDiscrete,range,slider;self=$(selector);slider_width=self.width();slider_offset=self.offset().left;sliderDiscrete=self;sliderDiscrete.each(function(i,v){curnt=$(this);curnt.append(sliderDiscrete_tmplt());range=curnt.find('input[type="range"]');slider=curnt.find(".slider");slider_fill=slider.find(".slider-fill");slider_handle=slider.find(".slider-handle");slider_label=slider.find(".slider-label");var range_val=parseInt(range.val());slider_fill.css("width",range_val+"%");slider_handle.css("left",range_val+"%");slider_label.find("span").text(range_val);});self.on("mousedown touchstart",".slider-handle",function(e){if(e.button===2){return false;}
var parents=$(this).parents(".rkmd-slider");var slider_width=parents.width();var slider_offset=parents.offset().left;var check_range=parents.find('input[type="range"]').is(":disabled");if(check_range===true){return false;}
$(this).addClass("is-active");var moveFu=function(e){var pageX=e.pageX||e.changedTouches[0].pageX;var slider_new_width=pageX-slider_offset;if(slider_new_width<=slider_width&&!(slider_new_width<"0")){slider_move(parents,slider_new_width,slider_width,true);}};var upFu=function(e){$(this).off(handlers);parents.find(".is-active").removeClass("is-active");};var handlers={mousemove:moveFu,touchmove:moveFu,mouseup:upFu,touchend:upFu};$(document).on(handlers);});self.on("mousedown touchstart",".slider",function(e){if(e.button===2){return false;}
var parents=$(this).parents(".rkmd-slider");var slider_width=parents.width();var slider_offset=parents.offset().left;var check_range=parents.find('input[type="range"]').is(":disabled");if(check_range===true){return false;}
var slider_new_width=e.pageX-slider_offset;if(slider_new_width<=slider_width&&!(slider_new_width<"0")){slider_move(parents,slider_new_width,slider_width,true);}
var upFu=function(e){$(this).off(handlers);};var handlers={mouseup:upFu,touchend:upFu};$(document).on(handlers);});}
function sliderDiscrete_tmplt(){var tmplt='<div class="slider">'+
'<div class="slider-fill"></div>'+
'<div class="slider-handle"><div class="slider-label"><span>0</span></div></div>'+
"</div>";return tmplt;}
function slider_move(parents,newW,sliderW,send){var slider_new_val=parseInt(Math.round((newW/sliderW)*100));var slider_fill=parents.find(".slider-fill");var slider_handle=parents.find(".slider-handle");var range=parents.find('input[type="range"]');slider_fill.css("width",slider_new_val+"%");slider_handle.css({left:slider_new_val+"%",transition:"none","-webkit-transition":"none","-moz-transition":"none"});range.val(slider_new_val);if(parents.find(".slider-handle span").text()!=slider_new_val){parents.find(".slider-handle span").text(slider_new_val);var number=parents.attr("id").substring(2);if(send)websock.send("slvalue:"+slider_new_val+":"+number);}}
)=====";

const uint8_t JS_SLIDER_GZIP[869] PROGMEM = { 31,139,8,0,19,56,231,94,2,255,237,86,77,143,155,48,16,189,231,87,100,173,237,6,186,196,155,238,49,196,185,180,170,212,67,79,173,212,74,171,85,228,128,89,172,16,131,176,73,218,102,243,223,59,254,128,0,33,171,109,79,61,244,4,246,60,143,103,222,60,123,156,84,34,82,60,23,227,114,179,141,87,37,21,79,236,75,198,99,86,122,146,101,44,82,121,233,31,118,180,28,195,40,9,164,177,172,246,60,86,105,61,200,147,68,50,21,68,85,41,148,155,251,192,101,84,50,197,2,227,206,77,134,218,3,185,62,185,13,219,222,136,182,98,243,235,53,22,235,218,154,236,191,231,227,140,37,42,236,238,99,16,189,57,204,104,148,122,137,203,206,227,193,206,63,152,24,33,4,149,114,233,135,102,132,105,81,48,17,123,221,197,43,181,45,50,216,204,15,77,6,196,66,19,14,192,9,23,69,165,30,212,207,130,17,100,172,232,113,82,71,220,6,34,108,231,80,147,78,194,179,140,216,255,46,100,170,45,39,92,74,69,156,177,97,164,181,157,176,25,93,179,11,78,141,9,144,186,122,38,208,213,142,102,164,160,165,100,159,132,242,204,20,134,41,157,102,43,66,28,73,233,33,83,9,20,52,235,110,209,155,126,128,22,168,203,113,9,103,34,112,81,201,130,10,228,99,197,126,184,173,53,218,15,143,126,104,203,43,60,180,205,43,201,226,124,47,198,42,175,162,84,42,90,130,235,126,234,65,83,83,230,31,120,226,49,188,174,148,202,5,33,228,222,63,64,245,160,4,227,132,102,146,133,199,145,206,29,50,102,66,201,186,238,216,141,129,40,45,249,105,83,37,163,242,182,36,29,176,81,101,11,224,148,89,35,186,226,212,184,40,101,209,198,30,167,6,245,130,126,48,135,120,230,49,151,116,157,177,24,130,129,196,218,46,8,81,101,197,206,210,171,83,162,113,252,62,163,186,28,92,78,41,176,179,99,46,161,109,190,99,31,43,210,230,204,114,242,196,190,19,134,205,247,249,25,106,153,234,125,226,175,154,120,38,31,102,143,214,212,206,89,176,125,67,12,152,166,29,42,116,192,125,220,130,180,233,188,185,185,58,71,160,25,242,253,131,155,214,177,122,142,172,160,15,237,222,61,134,141,240,120,52,241,85,69,47,195,154,22,8,205,179,170,41,225,188,119,202,128,112,139,41,92,50,189,247,0,133,118,131,218,7,57,24,137,106,236,220,18,27,24,165,182,39,12,162,42,230,58,40,107,133,235,197,140,142,225,181,23,231,81,181,133,40,124,45,248,83,104,175,63,5,255,229,223,75,239,76,156,78,212,255,130,60,71,127,160,206,65,165,253,133,142,142,163,122,175,241,112,79,51,231,223,252,147,201,34,230,187,113,164,101,79,144,19,194,114,114,59,26,152,183,45,106,185,184,3,203,37,136,187,160,151,3,38,219,139,150,11,221,5,150,179,197,157,249,90,103,141,75,100,255,80,232,170,108,98,60,207,167,91,7,40,192,55,199,61,124,129,34,247,90,57,149,167,211,244,62,83,149,226,50,175,64,137,158,94,122,231,150,250,111,223,205,102,126,71,227,166,91,247,238,140,110,187,110,129,93,203,30,134,55,61,187,233,196,175,57,19,23,123,114,55,183,75,141,249,160,207,226,252,28,11,218,164,66,114,77,232,28,137,92,64,67,69,211,61,91,111,184,154,158,76,232,100,219,230,191,6,12,71,247,52,50,15,136,238,46,230,244,190,196,196,184,253,22,240,175,72,111,249,225,213,107,251,251,106,126,69,181,93,195,67,172,246,65,149,42,225,74,135,59,5,203,106,45,85,201,197,147,119,111,66,52,106,129,204,101,30,109,176,30,192,35,37,3,63,21,155,163,219,62,113,48,101,61,235,174,243,27,27,117,74,231,52,11,0,0 };
