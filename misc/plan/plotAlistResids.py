#!/usr/bin/env python

from __future__ import absolute_import
from __future__ import print_function
import datetime
import numpy
from collections import defaultdict
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import sys

alist = open(sys.argv[1])
pfile = sys.argv[2]
refSt = sys.argv[3]
print('using alist:', sys.argv[1])
print('saving png: ', sys.argv[2])
print('ref station:', sys.argv[3])

#refSt = "L"
#datematch = '2017-04-11'
datematch = ''

dates = {}
times = {}
#rates = {}
rates = defaultdict(list)
delays = defaultdict(list)
for line in alist:
    line = line.strip()
    if line.startswith("*"):
        continue
    field = line.split()
    try:
        if len(field < 46):
            print('bogus line',line)
            continue
    except:
        print('really bogus',line)
    st1 = field[14][0]
    st2 = field[14][1]
    q = int(field[15][0])
    snr = float(field[20])

    # discard autocorrelations
    if st1 == st2:
        continue
    # discard non-detections
    if q == 0 or snr <= 5.0 :
        continue
    
    # only consider values to the reference station
    if st1 != refSt and st2 != refSt:
        continue

    
    doy,time = field[11].split("-")
    hour = time[0:2]
    minute = time[2:4]
    second = time[4:6]
    delay = field[24]
    rate = field[27]
    
    date = datetime.datetime.strptime(field[10]+"-"+field[11], "%Y-%j-%H%M%S")
    if len(datematch)>0 and str(date)[0:10] != datematch: continue
    
    #time = datetime.datetime.strptime(field[11], "%j-%H%M%S")
    print(date, time, rate, delay)
    
    print(date, st1, st2, q, snr, field[17], field[28], field[29], field[30], field[31], rate)
    
    if st1 in rates.keys():
        rates[st1] = numpy.append(rates[st1], float(rate))
        delays[st1] = numpy.append(delays[st1], float(delay))
        dates[st1].append(date)
        times[st1].append(time)
    else:
        rates[st1]= numpy.array(float(rate))
        delays[st1] = numpy.array(float(delay))
        dates[st1] = [date]
        times[st1] = [time]
    if st2 in rates.keys():
        rates[st2] = numpy.append(rates[st2], float(rate))
        delays[st2] = numpy.append(delays[st2], float(delay))
        dates[st2].append(date)
        times[st2].append(time)
    else:
        rates[st2]= numpy.array(float(rate))
        delays[st2] = numpy.array(float(delay))
        dates[st2] = [date]
        times[st2] = [time]


print("Reference station: ", refSt)
print("{0:2s} {1:3s}  {2:4s}   {3:8s}".format("st", "num", "mean", "std dev."))

plt.ioff()
fig = plt.figure(figsize=(12,8))
ax1 = fig.add_subplot(211)
ax2 = fig.add_subplot(212)
fig.subplots_adjust(right=0.65)

ax1.set_ylabel('delay [us]')
ax2.set_ylabel('rate [us/s]')

delayStat = "Delays statistics\n{0:2s} {1:4s} {2:7s} {3:4s}\n".format("st", "#pts", "mean", "std. dev")
rateStat = "Rate statistics\n{0:2s} {1:4s} {2:7s} {3:4s}\n".format("st", "#pts", "mean", "std. dev")

for st in rates.keys():
    if st == refSt:
        continue
    plt.gca().xaxis.set_major_formatter( mdates.DateFormatter('%d-%m-%Y %H:%M:%S'))
    delayStat += "{0:2s} {1:002d}   {2:+2.4f} {3:=4f}".format(st, len(delays[st]), numpy.mean(delays[st]), numpy.std(delays[st]))+"\n"
    rateStat  += "{0:2s} {1:002d}   {2:+2.4f} {3:=4f}".format(st, len(rates[st]), numpy.mean(rates[st]), numpy.std(rates[st]))+"\n"
    print("{0:2s} {1:002d} {2:+2.4f} {3:=4f}".format(st, len(rates[st]), numpy.mean(rates[st]), numpy.std(rates[st])))
    ax1.plot(dates[st], delays[st], marker="o", label=st)
    ax2.plot(dates[st], rates[st], marker="o", label=st)
#   ax3.plot (times[st], delays[st], marker="o", label=st)
#   ax4.plot (times[st], rates[st], marker="o", label=st)
#plt.plot (dates["J"], rates["J"])
ax1.set_title("delay")
ax2.set_title("rate")

ax1.legend(loc='center left', bbox_to_anchor=(1, 0.5), fancybox=True)
ax2.legend(loc='center left', bbox_to_anchor=(1, 0.5), fancybox=True)

fig.text(0.75, 0.68, delayStat, family='courier', bbox={'facecolor':'white', 'alpha':0.5, 'pad':10})
fig.text(0.75, 0.29, rateStat,  family='courier', bbox={'facecolor':'white', 'alpha':0.5, 'pad':10})

plt.gcf().autofmt_xdate()
#plt.show()
fig.savefig(pfile)

#
# eof
#
