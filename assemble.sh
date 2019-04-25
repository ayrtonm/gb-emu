for i in `ls asm/*.asm`;
  do python assembler.py $i ${i%.asm}.gb;
done
