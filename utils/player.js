;(function($) {

var cat = [];

var save = function(category, channel) {
    if (window.localStorage === undefined) {
        $.cookie('category', category);
        $.cookie('channel', channel);
    } else {
        localStorage['category'] = category;
        localStorage['channel'] = channel;
    }
}

var load_category = function() {
    var category;
    if (window.localStorage === undefined) {
        category = $.cookie('category');
    } else {
        category = localStorage['category'];
    }
    if (category === undefined || category === null) {
        category = 0;
    }
    $('#category').val(category);
    return category;
}

var load_channel = function() {
    var channel;
    if (window.localStorage === undefined) {
        channel = $.cookie('channel');
    } else {
        channel = localStorage['channel'];
    }
    if (channel === undefined || channel === null) {
        channel = 0;
    }
    $('#channel').val(channel);
    return channel;
}

var play = function() {
    var category = $('#category').val();
    var channel = $('#channel').val();
    var url = cat[category].channel[channel].url;
    var title = cat[category].channel[channel].title;
    var player = $('#player');
    player.empty().append(
        '<object codebase="http://www.apple.com/qtactivex/qtplugin.cab"'
        + 'classid="clsid:6BF52A52-394A-11d3-B153-00C04F79FAA6"'
        + 'type="application/x-oleobject">'
        + '<param name="url" value="' + url + '"'
        + '<embed autostart="1" src="' + url + '"'
        + 'type="application/x-mplayer2"'
        + 'pluginspage="http://www.microsoft.com/Windows/MediaPlayer/"></embed>'
        + '</object>'
        + '<div>' + title + '</div>');
    $('#control').val('■');
    save(category, channel);
}

var stop = function() {
    $('#player').empty();
    $('#control').val('▶');
}

var insert = function(selector, item, i) {
    var select = $.browser.msie ? $(selector)[0] : $(selector);
    if ($.browser.msie) {
        select.add(new Option(item.title, i), i);
    } else {
        select.append(new Option(item.title, i));
    }
}

var setAlarm = function() {
    var alarm = $('#alarm');
    alarm.bind('click', function() {
        if (alarm[0].checked == true) {
            alarm.everyTime(1000 , function () {
                var hour = $('#hour').val();
                var minute = $('#minute').val();
                var now = new Date();
                var now_hour = now.getHours();
                var now_minute = now.getMinutes();
                if (hour == now_hour && minute == now_minute) {
                    play();
                    alarm[0].checked = false;
                    alarm.stopTime();
                }
            });
        } else {
            alarm.stopTime();
        }
    });
}

var addOptionHours = function() {
    var hour = new Date().getHours();
    var select = $.browser.msie ? $('#hour')[0] : $('#hour');
    for (var i = 0; i < 24; i++) {
        if ($.browser.msie) {
            select.add(new Option(i, i), i);
        } else {
            select.append(new Option(i, i));
        }
    }
    $('#hour').val(hour);
}

var addOptionMinutes = function() {
    var minute = Math.ceil(new Date().getMinutes() / 5) * 5;
    var select = $.browser.msie ? $('#minute')[0] : $('#minute');
    for (var i = 0; i < 60; i+=5) {
        if ($.browser.msie) {
            select.add(new Option(i, i), i);
        } else {
            select.append(new Option(i, i));
        }
    }
    if (minute == 60) {
        $('#minute').val(0);
    } else {
        $('#minute').val(minute);
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
        var category = load_category();
        $.each(data.category[category].channel, function(i, item) {
            if (item) {
                insert('#channel', item, i);
            }
        });
        load_channel();
        $('#category').bind('change', function() {
            $('#channel').empty();
            $.each(cat[this.value].channel, function(i, item) {
                if (item) {
                    insert('#channel', item, i);
                }
            });
            play();
        });
        $('#channel').bind('change', function() {
            play();
        });
        $('#control').bind('click', function() {
            if ($(this).val() == '▶') {
                play();
            } else {
                stop();
            }
        });
        addOptionHours();
        addOptionMinutes();
        setAlarm();
    }
});

})(jQuery);
