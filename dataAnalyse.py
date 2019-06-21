#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
"""
========================================
Last updated Sat. 20th June 2019
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

# Loads datafile
parameters = np.loadtxt("dataPy.txt")

WLS_sizes = parameters[:,0]
PMT_hits = parameters[:,1]
PhotonsGenerated = parameters[:,2]

efficiency = PMT_hits / PhotonsGenerated
area = WLS_sizes * WLS_sizes

PMTArea = np.pi * 12.7 * 12.7

factorArea = area / PMTArea

gain = 1 + factorArea * efficiency

plt.plot(area,efficiency)
plt.grid(linestyle='dotted')
plt.title("Efficiency of gain as a function of area factors")
plt.xlabel('Area in factors of a 10" PMT')
plt.ylabel("Efficiency")
plt.show()

""" Storage



alpha = [None] * len(efficiency)
for i in range(len(efficiency)):
	alpha[i] = 1 + sum(efficiency[0:i])

beta = alpha * area







"""
