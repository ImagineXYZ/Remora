import Gnuplot
import numpy as np
from numpy import genfromtxt

g = Gnuplot.Gnuplot(persist=0)

#f(x)=kx+b
kI1=0.0016113
bI1=0.0402832
kI2=0.0001611
bI2=-0.0072510
kVL=0.0033999

iI1=0
iI2=1
iVL=2
iTimestamp=3

I2_margin=0.015
I2_base = 0.015
data_raw = genfromtxt ('../Data/MRAW0000.TXT', delimiter=',',skip_header=2)

I1_raw = data_raw[:,iI1]
I2_raw = data_raw[:,iI2]
VL_raw = data_raw[:,iVL]
Timestamp_raw= data_raw[:,iTimestamp]

I1=I1_raw*kI1-bI1
I2=I2_raw*kI2-bI2
VL=VL_raw*kVL
Timestamp_s = Timestamp_raw/1000000

#print I1
#print I2
IL = np.where((I1>(I2+I2_margin)) & (I2 > I2_base), I1,I2)
PL = VL*IL
EL = np.trapz(PL,Timestamp_s)

print EL

#print IL
dI1 = Gnuplot.Data (Timestamp_s,I1,inline=1, title="I1[A]", with_='linespoints pt 7 ps 0.5 ')
dI2 = Gnuplot.Data (Timestamp_s,I2,inline=1, title="I2[A]", with_='linespoints pt 7 ps 0.5 ')
dVL = Gnuplot.Data (Timestamp_s,VL,inline=1, title="VL[V]", with_='linespoints pt 7 ps 0.5 ')
dIL = Gnuplot.Data (Timestamp_s,IL,inline=1, title="IL[A]", with_='linespoints pt 7 ps 0.5 ')
dPL = Gnuplot.Data (Timestamp_s,PL,inline=1, title="PL[A]", with_='linespoints pt 7 ps 0.5 ')
#dEL = Gnuplot.Data (Timestamp_s,EL,inline=1, title="EL[A]", with_='linespoints pt 7 ps 0.5 ')

#g.plot(dI1, dI2, dVL, dIL)
g.plot (dIL, dVL, dPL)


#g.hardcopy (filename='/tmp/rainfall-intensity.png', terminal='png') # write last plot to another terminal
while(True):
	pass
	
