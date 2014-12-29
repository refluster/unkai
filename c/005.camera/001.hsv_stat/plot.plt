set size 3,3
set terminal png font "arial,8" size 800,600
unset border
set grid ytics lw 0.1 lt -1
set nokey
set output "tmp.png"

set multiplot

set origin 0,0
set size 0.33,0.33
plot '~/save2.work/test000_env_led.dat' u 1:2 w l

set origin 0.33,0
set size 0.33,0.33
plot '~/save2.work/test000_env_led.dat' u 1:3 w l

set origin 0.66,0
set size 0.33,0.33
plot '~/save2.work/test000_env_led.dat' u 1:4 w l

set origin 0,0.33
set size 0.33,0.33
plot '~/save2.work/test000_env.dat' u 1:2 w l

set origin 0.33,0.33
set size 0.33,0.33
plot '~/save2.work/test000_env.dat' u 1:3 w l

set origin 0.66,0.33
set size 0.33,0.33
plot '~/save2.work/test000_env.dat' u 1:4 w l

set origin 0,0.66
set size 0.33,0.33
plot '~/save2.work/test000_led.dat' u 1:2 w l

set origin 0.33,0.66
set size 0.33,0.33
plot '~/save2.work/test000_led.dat' u 1:3 w l

set origin 0.66,0.66
set size 0.33,0.33
plot '~/save2.work/test000_led.dat' u 1:4 w l

# 'nocrop font "arial,12" fontscale 1.0 size 640,480 '

