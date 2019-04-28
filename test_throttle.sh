touch data
CPUCLK=(1 10 100 1000 10000)
CPUSLEEP=(1 2 3 4 5 6 7 8 9)
for cpusleep in ${CPUSLEEP[@]}; do
  for clk in ${CPUCLK[@]}; do
    DISPFREQ=`echo 16666.6 / $cpusleep | bc -l | awk -F. '{print $1}'`
    echo "$clk $cpusleep $DISPFREQ"
    sed -i 's:define CPU_CLKS x:define CPU_CLKS '$clk':g' cpu.cpp
    sed -i 's:define CPU_SLEEP CPU_CLKS\*x:define CPU_SLEEP CPU_CLKS\*'$cpusleep':g' cpu.cpp
    sed -i 's:define LCDUPDATECLK x:define LCDUPDATECLK '$DISPFREQ':g' lcd.cpp
    make
    ./main -l asm-bin/joyp.gb &
    PID=`echo $!`
    echo emulator PID is $PID
    sleep 30s
    CPUAVG=`ps -p $PID -o %cpu | tail -1`
    kill -9 $PID
    echo $clk $cpusleep $CPUAVG >> data
    sed -i 's:define CPU_CLKS '$clk':define CPU_CLKS x:g' cpu.cpp
    sed -i 's:define CPU_SLEEP CPU_CLKS\*'$cpusleep':define CPU_SLEEP CPU_CLKS\*x:g' cpu.cpp
    sed -i 's:define LCDUPDATECLK '$DISPFREQ':define LCDUPDATECLK x:g' lcd.cpp
  done
done
