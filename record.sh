#!/bin/bash
./gb-emu -c settings.cfg -l $1 &
#pasuspender mednafen $1 &
sleep 1
#read -r w <<< $(xwininfo -name "demo" | grep Width | awk '{print $2}')
#read -r h <<< $(xwininfo -name "demo" | grep Height | awk '{print $2}')
#read -r x <<< $(xwininfo -name "demo" | grep "Absolute upper-left X" | awk '{print $4}')
#read -r y <<< $(xwininfo -name "demo" | grep "Absolute upper-left Y" | awk '{print $4}')
read -r w <<< $(xwininfo -name "Game Boy Emulator" | grep Width | awk '{print $2}')
read -r h <<< $(xwininfo -name "Game Boy Emulator" | grep Height | awk '{print $2}')
read -r x <<< $(xwininfo -name "Game Boy Emulator" | grep "Absolute upper-left X" | awk '{print $4}')
read -r y <<< $(xwininfo -name "Game Boy Emulator" | grep "Absolute upper-left Y" | awk '{print $4}')
x=$(($x+1))
y=$(($y+1))
ffmpeg -s ${w}x${h} -framerate 10 -f x11grab -i :0.0+${x},${y} $2.mp4
convert -layers optimize $2.mp4 $2.gif 
