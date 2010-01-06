;(function($) {

var cat = [];
var play = function() {
    var url = cat[$('#category').val()].channel[$('#channel').val()].url;
    var title = cat[$('#category').val()].channel[$('#channel').val()].title;
    var player = $('#player');
    var panel = $('#panel');
    player.empty().append(
        '<object codebase="http://www.apple.com/qtactivex/qtplugin.cab"'
        + 'classid="clsid:6BF52A52-394A-11d3-B153-00C04F79FAA6"'
        + 'type="application/x-oleobject">'
        + '<param name="url" value="' + url + '"'
        + '<embed src="' + url + '"'
        + 'type="application/x-mplayer2"'
        + 'pluginspage="http://www.microsoft.com/Windows/MediaPlayer/"></embed>'
        + '</object>'
        + '<div>' + title + '</div>');
    player.width(panel.width());
    player.height(panel.height());
}

var insert = function(selector, item, i) {
    var select = $.browser.msie ? $(selector)[0] : $(selector);
    if ($.browser.msie) {
        select.add(new Option(item.title, i), i);
    } else {
        select.append(new Option(item.title, i));
    }
}

$.getJSON('hichannel.json', function(data, stat) {
    if (stat == 'success') {
        $('fieldset legend').html('<a href="' + data.url + '">' + data.title + '</a>');
        $.each(data.category, function(i, item) {
            if (item) {
                cat[i] = item;
                insert('#category', item, i);
            }
        });
        $.each(data.category[0].channel, function(i, item) {
            if (item) {
                insert('#channel', item, i);
            }
        });
        $('#category').bind('change', function() {
            $('#channel').empty();
            $.each(cat[this.value].channel, function(i, item) {
                if (item) {
                    insert('#channel', item, i);
                }
            });
        });
        $('#control').bind('click', function() {
            console.log($(this).val());
            if ($(this).val() == '播放') {
                play();
                $(this).val('停止');
            } else {
                $('#player').empty();
                $(this).val('播放');
            }
        });
    }
});

})(jQuery);
