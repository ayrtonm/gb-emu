#!/bin/bash
#this is a script that runs this emulator with the -p flag and a modified version of mednafen, both programs print out the executed opcodes for the specified ROM, the script then removes the header in the text files and cuts the files to the size of the smallest one and runs diff on the text files
FILE=~/gb-emu/roms/racing.gb
./main -p $FILE > text1
./../mednafen/src/mednafen $FILE > text2
N1=$(wc -l text1 | awk '{print $1;}')
N2=$(wc -l text2 | awk '{print $1;}')
tail -n $(($N1-5)) text1 > new
mv new text1
tail -n $(($N2-48)) text2 > new
mv new text2
if [ "$(($N1-5))" -gt "$(($N2-47))" ]
	then head -n $(($N2-48)) text1 > new;mv new text1
	else head -n $(($N1-5)) text2 > new;mv new text2
fi
diff text1 text2 > dtext
