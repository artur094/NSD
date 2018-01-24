#!/usr/bin/gnuplot -persist

set terminal png size 1200,900 enhanced font "Helvetica" 14

# set term postscript eps enhanced color "Helvetica" 18 dl 3

# set logscale x
# set logscale y
# set logscale cb

# matlab palette colors
set palette defined ( 0 "#000090",\
                      1 "#000fff",\
                      2 "#0090ff",\
                      3 "#0fffee",\
                      4 "#90ff70",\
                      5 "#ffee00",\
                      6 "#ff7000",\
                      7 "#ee0000",\
                      8 "#7f0000")


set title "Communities size distribution"

# set xtics 7e-8,10e-8,20e-5

# set ytics 1

# set format x "10^{%L}";
# set format y "10^{%L}";
# set format cb "10^{%L}";

set xrange [0:]
set yrange [0:2]

# set ytics ("1" 1, "10" 10, "20" 20)

set key bmargin center horizontal nobox
set style line 1 pt 7 ps 1.5 # circles

set grid y
set grid x

set xlabel "Communities deteted"
# set ylabel
# set cblabel ""
set output "nr_detec_hist.png"
plot 'nr_detec_hist.dat' using 1:2 notitle with points ls 1
