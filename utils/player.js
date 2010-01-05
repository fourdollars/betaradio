;(function($) {

var cat = [];
var play = function() {
    var url = cat[$('#category').val()].channel[$('#channel').val()].url;
    var title = cat[$('#category').val()].channel[$('#channel').val()].title;
    $('#player').empty().append(
        '<object codebase="http://www.apple.com/qtactivex/qtplugin.cab"'
        + 'classid="clsid:6BF52A52-394A-11d3-B153-00C04F79FAA6"'
        + 'type="application/x-oleobject">'
        + '<param name="url" value="' + url + '"'
        + '<embed src="' + url + '"'
        + 'type="application/x-mplayer2"'
        + 'pluginspage="http://www.microsoft.com/Windows/MediaPlayer/"></embed>'
        + '</object>'
        + '<div>' + title + '</div>');
}

$.getJSON('hichannel.json', function(data, stat) {
    if (stat == 'success') {
        $('fieldset legend').html(data.title);
        $.each(data.category, function(i, item) {
            if (item) {
                var select = $('#category')[0];
                cat[i] = item;
                if (i == 0) {
                    select.add(new Option(item.title, i));
                } else {
                    select.add(new Option(item.title, i), $.browser.msie ? i : select.options[i]);
                }
            }
        });
        $.each(data.category[0].channel, function(i, item) {
            if (item) {
                var select = $('#channel')[0];
                if (i == 0) {
                    select.add(new Option(item.title, i));
                } else {
                    select.add(new Option(item.title, i), $.browser.msie ? i : select.options[i]);
                }
            }
        });
        $('#category').bind('change', function() {
            $('#channel').empty();
            $.each(cat[this.value].channel, function(i, item) {
                if (item) {
                    var select = $('#channel')[0];
                    if (i == 0) {
                        select.add(new Option(item.title, i));
                    } else {
                        select.add(new Option(item.title, i), $.browser.msie ? i : select.options[i]);
                    }
                }
            });
        });
        $('#control').bind('click', function() {
            play();
        });
    }
});

})(jQuery);
