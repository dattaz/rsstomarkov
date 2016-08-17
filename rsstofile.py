# coding: utf8
from __future__ import unicode_literals
import sys
import feedparser
rss = sys.argv[1]
folder = sys.argv[2]


feeds = feedparser.parse(rss)

#print feeds
for i in range(0, len(feeds)):
    print feeds.entries[i]['title']
    #print feeds.entries[i]['description']
    file = open( folder + str(i) + ".txt", 'w')
    file.write(feeds.entries[i]['description'].encode('utf8'))
    file.close()
