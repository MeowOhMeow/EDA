reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 0,0 to 20,17
set label "M1" at 10,8center
set object 2 rect from 20,0 to 30,35
set label "M2" at 25,17center
set object 3 rect from 0,17 to 10,32
set label "M3" at 5,24center
set object 4 rect from 30,0 to 38,20
set label "M4" at 34,10center
set object 5 rect from 38,0 to 55,60
set label "M5" at 46,30center
set object 6 rect from 0,35 to 38,55
set label "M6" at 19,45center
set object 7 rect from 55,0 to 80,45
set label "M7" at 67,22center
set object 8 rect from 80,0 to 96,23
set label "M8" at 88,11center
set xtics 20
set ytics 16
plot [0:100][0:80]0
set terminal png size 1024,768
set output "output.png"
replot
