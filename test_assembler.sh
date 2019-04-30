for i in `ls asm-src`;
  do python assembler.py asm-src/$i asm-bin/${i%.asm}.gb;
done
#./main -l asm-bin/joyp.gb &
