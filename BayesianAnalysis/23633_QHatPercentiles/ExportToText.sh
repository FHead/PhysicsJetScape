

for i in `ls root/ | grep root$`
do
   echo $i
   mkdir -p root/txt/${i/.root/}/

   for j in P T PM PL TM TL
   do
      for k in 0 10 20 30 40 50 60 68 70 80 90
      do
         ./ExecuteExport --Input root/$i --Output root/txt/${i/.root/}/QHat${j}${k}.txt --Curve QHat${j}${k}
         ./ExecuteExport --Input root/$i --Output root/txt/${i/.root/}/QHat${j}Prior${k}.txt --Curve QHat${j}Prior${k}
      done
   done
done


