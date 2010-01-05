;(function($) {

var cat = [];

$.getJSON('hichannel.json', function(data, stat) {
    if (stat == 'success') {
        $('fieldset legend').html(data.title);
        $.each(data.category, function(i, category) {
            cat[i] = category;
            $('#category').append('<option value="' + i + '">' + category.title + '</option>');
        });
        $.each(data.category[0].channel, function(i, channel) {
            $('#channel').append('<option value="' + i + '">' + channel.title + '</option>');
        });
        $('#category').bind('change', function() {
            $('#channel').empty();
            $.each(cat[this.value].channel, function(i, channel) {
                $('#channel').append('<option value="' + i + '">' + channel.title + '</option>');
            });
        });
        $('#play').bind('click', function() {
            var url = cat[$('#category').val()].channel[$('#channel').val()].url;
            var title = cat[$('#category').val()].channel[$('#channel').val()].title;
            $('#player').empty();
            $('#player').append('<object codebase="http://www.apple.com/qtactivex/qtplugin.cab"' +
                'classid="clsid:6BF52A52-394A-11d3-B153-00C04F79FAA6"' +
                'type="application/x-oleobject">' +
                '<param name="url" value="' + url + '"' +
                '<embed src="' + url + '"' +
                'type="application/x-mplayer2"' +
                'pluginspage="http://www.microsoft.com/Windows/MediaPlayer/"></embed>' +
                '</object>' +
                '<div>' + title + '</div>');
        });
    }
});

})(jQuery);
