;(function($) {

var cat = [];
var play = function() {
    var url = cat[$('#category').val()].channel[$('#channel').val()].url;
    var title = cat[$('#category').val()].channel[$('#channel').val()].title;
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
    var select = $('#hour');
    for (var i = 0; i < 24; i++) {
        if ($.browser.msie) {
            select.add(new Option(i, i), i);
        } else {
            select.append(new Option(i, i));
        }
    }
    select.val(hour);
}

var addOptionMinutes = function() {
    var minute = Math.ceil(new Date().getMinutes() / 5) * 5;
    var select = $('#minute');
    for (var i = 0; i < 60; i+=5) {
        if ($.browser.msie) {
            select.add(new Option(i, i), i);
        } else {
            select.append(new Option(i, i));
        }
    }
    if (minute == 60) {
        select.val(0);
    } else {
        select.val(minute);
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
