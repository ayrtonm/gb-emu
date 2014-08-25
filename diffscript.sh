#!/bin/bash
FILE=~/gb/roms/opus5.gb
./main -p $FILE > text1
./../mednafen/src/mednafen $FILE > text2
N1=$(wc -l text1 | awk '{print $1;}')
N2=$(wc -l text2 | awk '{print $1;}')
tail -n $(($N1-5)) text1 > new
mv new text1
tail -n $(($N2-47)) text2 > new
mv new text2
if [ "$(($N1-5))" -gt "$(($N2-47))" ]
	then head -n $(($N2-47)) text1 > new;mv new text1
	else head -n $(($N1-5)) text2 > new;mv new text2
fi
diff text1 text2 > difftext
