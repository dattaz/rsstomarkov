# coding: utf8
from __future__ import unicode_literals
import sys
import feedparser
import html2text
rss = sys.argv[1]
folder = sys.argv[2]


feeds = feedparser.parse(rss)

#print feeds
for i in range(0, len(feeds)-1):
    print feeds.entries[i]['title']
    #print feeds.entries[i]['description']
    file = open( folder + str(i) + ".txt", 'w')
    file.write(html2text.html2text(feeds.entries[i]['description']).encode('utf8'))
    file.close()
