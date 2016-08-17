# coding: utf8
from __future__ import unicode_literals
fichier = open("tmp/sentences", "r")
ligne=fichier.read().decode('utf8').split("\n")
phrase = [ ]
for i in ligne:
    phrase = phrase + i.split(". ")
for i in phrase:
    print i.encode('utf8')


