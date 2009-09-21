#!/usr/bin/env python
import matplotlib as mpl
mpl.use('cairo')

from pylab import *
import glob
import sys
import os
import re

inch=2.54

if len(sys.argv) < 3:
	print "usage plot.py model pid"
	exit(-1)
	
format="pdf"
	
if len(sys.argv) > 3:
	format=sys.argv[3]

rc('font', size=8)
rc('figure.subplot', left=0.185, right=0.985, top=0.97, bottom=0.15)

model = sys.argv[1]
pid = sys.argv[2]

shared = csv2rec('%s-shared-%s-time.avg', delimiter='\t', names=['nthreads', 'runtime'])
shared.speedup = map(lambda r: shared.runtime[0] / r, shared.runtime)
nonshared = csv2rec('%s-nonshared-%s-time.avg', delimiter='\t', names=['nthreads', 'runtime'])
nonshared.speedup = map(lambda r: nonshared.runtime[0] / r, nonshared.runtime)

f=figure(figsize=(8/inch, 5.1/inch))

plot(range(len(shared.nthreads)), range(len(shared.nthreads)))

lines=[]

line=plot(shared.nthreads, shared.speedup)
line.set_linestyle('-')
lines.append(line)
line=plot(nonshared.nthreads, nonshared.speedup)
line.set_linestyle('--')
lines.append(line)
	
xlabel('number of threads')#, va='bottom')
ylabel('speedup')#, ha='left')
l=legend(lines, ["shared", "nonshared"], 'best')
l.draw_frame(False)
savefig('imgs/%s-%s-speedup.%s' % (model, pid, format), format=format, dpi=400)#, bbox_inches="tight")
#show()
