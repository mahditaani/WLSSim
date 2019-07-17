#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
"""
========================================
Last updated Sat. 1st July 2019
by Andrias Hansen
Title:
    dataAnalyse.py
Discription:
    Analyses the data from the ray tracing simulation of photons on a wavelength shifting plate
========================================
"""

# Importing Libraries
import numpy as np
import matplotlib.pyplot as plt

QE      = 0.59   # Quantum efficiency
QY      = 1.00   # Quantum yield
T       = 0.78   # Transmission: plastic -> air
U       = 0.80   # Uniformity
L       = 0.54  # Loss from the top and bottom plate
WLS0    = 1.00   # Unknown scaling factor

eff = QE * QY * T * U * L * WLS0

# Loads datafile
parameters = np.loadtxt("data.txt")

WLSx_sizes = parameters[:,0]
WLSy_sizes = parameters[:,1]
ratio = parameters[:,2]
PMT_hits = parameters[:,3]
PhotonsGenerated = parameters[:,4]
Attenuation = parameters[:,5]

Bounces0 = parameters[:,6]
Bounces1 = parameters[:,7] + parameters[:,8]

PMTArea = np.pi * 14.0 * 14.0

efficiency = PMT_hits / PhotonsGenerated
area = (WLSx_sizes * WLSy_sizes) - PMTArea

factorArea = area / PMTArea

gain = 1 + factorArea * efficiency * eff

index = np.arange(len(ratio))

p1 = plt.bar(index, (Bounces0/PhotonsGenerated) * eff, width=0.6, label="= 0 reflections")
p2 = plt.bar(index, (Bounces1/PhotonsGenerated) * eff, width=0.6, label="$\geq$ 1 reflections")

plt.xticks(index, ('1.00', '', '0.93', '', '0.87', '', '0.80', '', '0.73', '', '0.67', '', '0.60', '', '0.53'))

plt.grid(linestyle='dotted')
plt.title('Efficiency as a function of ratio - w/ a fixed $2500 \mathrm{ cm^2}$ area')
plt.xlabel('Ratio of length / width')
plt.ylabel("Efficiency")

plt.legend()

plt.ylim(0, 0.040)

plt.show()
