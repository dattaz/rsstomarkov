#!/bin/bash

read -p "Url of the RSS : " rss
read -p "Name of folder : " foldername
read -p "Number of sentence : " nbsentence

rm -r tmp/
mkdir tmp/
mkdir tmp/texts
python rsstofile.py $rss tmp/texts/
markov/markov -l $nbsentence tmp/texts/* > tmp/sentences
python 1sentencesbyligne.py |  awk '{if (x[$0] != "") next ; print $0 ; x[$0]=$0}' > tmp/sentenceswithoutrepetition
mkdir $foldername
cp tmp/sentenceswithoutrepetition $foldername/.
cp index.php $foldername/.
cp Parsedown.php $foldername/.
nbofligne=$(wc -l tmp/sentenceswithoutrepetition |sed 's/^\(.*\) .*/\1/')
echo $nbofligne
sed -i "s/numberofsentences/$nbofligne/" $foldername/index.php
sed -i "s/XXX/$foldername/g" $foldername/index.php
