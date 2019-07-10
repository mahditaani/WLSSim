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

QE      = 0.8   # Quantum efficiency
QY      = 1.0   # Quantum yield
T       = 0.4   # Transmission: plastic -> air
U       = 1.0   # Uniformity
L       = 0.77  # Loss from the top and bottom plate
WLS0    = 1.0   # Unknown scaling factor

eff = QE * QY * T * U * L * WLS0

# Loads datafile
parameters = np.loadtxt("data.txt")

WLSx_sizes = parameters[:,0]
WLSy_sizes = parameters[:,1]
ratio = parameters[:,2]
PMT_hits = parameters[:,3]
PhotonsGenerated = parameters[:,4]
Attenuation = parameters[:,5]

PMTArea = np.pi * 14.0 * 14.0

efficiency = PMT_hits / PhotonsGenerated
area = (WLSx_sizes * WLSy_sizes) - PMTArea

factorArea = area / PMTArea

gain = 1 + factorArea * efficiency * eff

plt.plot(ratio, efficiency)

plt.grid(linestyle='dotted')
plt.title('Efficiency as a function ratio with a fixed area')
plt.xlabel('Ratio')
plt.ylabel("Efficiency")

plt.show()

