set size 3,3
set terminal png font "arial,8" size 800,600
unset border
set grid ytics lw 0.1 lt -1
set nokey
set output "tmp.png"
set xtics scale 0

set multiplot

##############################

set label 1 'env + led' at -90,0 left rotate by 90
set title 'hue'
set origin 0.04,0.66
set size 0.33,0.33
plot '~/save2.work/test000_env_led.dat' u 1:2 w l
unset label 1

set title 'saturation'
set origin 0.35,0.66
set size 0.33,0.33
plot '~/save2.work/test000_env_led.dat' u 1:3 w l

set title 'value'
set origin 0.66,0.66
set size 0.33,0.33
plot '~/save2.work/test000_env_led.dat' u 1:4 w l

unset title

##############################

set label 1 'env' at -90,0 left rotate by 90
set origin 0.04,0.33
set size 0.33,0.33
plot '~/save2.work/test000_env.dat' u 1:2 w l
unset label 1

set origin 0.35,0.33
set size 0.33,0.33
plot '~/save2.work/test000_env.dat' u 1:3 w l

set origin 0.66,0.33
set size 0.33,0.33
plot '~/save2.work/test000_env.dat' u 1:4 w l

##############################

set label 1 'led' at -90,0 left rotate by 90
set origin 0.04,0
set size 0.33,0.33
plot '~/save2.work/test000_led.dat' u 1:2 w l
unset label

set origin 0.35,0
set size 0.33,0.33
plot '~/save2.work/test000_led.dat' u 1:3 w l

set origin 0.66,0
set size 0.33,0.33
plot '~/save2.work/test000_led.dat' u 1:4 w l

