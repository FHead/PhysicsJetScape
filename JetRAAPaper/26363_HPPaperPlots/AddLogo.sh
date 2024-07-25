#!/bin/bash

cat LatexTemplate.tex | sed "s/__FILE__/$1/" | sed "s/__X__/$2/" | sed "s/__Y__/$3/" \
   | sed "s/__SCALE__/$4/" > TempLatex.tex
xelatex TempLatex.tex
mv TempLatex.pdf $5
rm TempLatex.aux TempLatex.log TempLatex.tex
