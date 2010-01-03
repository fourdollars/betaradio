#! /usr/bin/env python
# -*- coding: utf-8 -*-

import re
import urllib2
import yaml

prog = re.compile('.*(mms://[^&"]*).*')

def fetchMMS(id):
    url = 'http://hichannel.hinet.net/player/radio/index.jsp?radio_id=%d' % id
    opener = urllib2.build_opener()
    opener.addheaders = [('User-agent', 'Windows Media Player')]
    page = opener.open(url)
    while 1:
        line = page.readline()
        if line:
            result = prog.match(line)
            if result:
                return result.group(1)
        else:
            break

def main():
    file = open('hichannel.yaml')
    data = yaml.load(file)
    file.close()
    if 'content' in data:
        if 'title' in data: print 'title: ' + data['title'].encode('utf-8')
        if 'url' in data: print 'url: ' + data['url']
        print 'content:'
        for item in data['content']:
            print '- category: ' + item['category'].encode('utf-8')
            print '  channel:'
            for channel in item['channel']:
                print "  - title: %s" % channel['title'].encode('utf-8')
                print "    id: %d" % channel['id']
                if 'url' in channel:
                    print "    url: %s" % channel['url']
                else:
                    print "    url: %s" % fetchMMS(channel['id'])

if __name__ == '__main__':
    main()
