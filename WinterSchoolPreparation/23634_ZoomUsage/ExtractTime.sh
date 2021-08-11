#!/bin/sh

Day=$1
Input=${Day}.csv
Tag=${Day}

grep -v "JETSCAPE Summer School 2021" "$Input" \
   | cut -d ',' -f 3-4 \
   | tr ',:' ' ' \
   | awk '{if($2>0 && $2<100) print $2,$3,$4,$6,$7,$8}' \
   | tee Raw$Tag.txt \
   | TextToTree List${Tag}.root 6 "StartH:StartM:StartS:EndH:EndM:EndS"
