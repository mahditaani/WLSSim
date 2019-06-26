#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
"""
========================================
Last updated Sat. 26th June 2019
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
parametersL20 = np.loadtxt("dataL20.txt")

parametersL40 = np.loadtxt("dataL40.txt")
parametersL80 = np.loadtxt("dataL80.txt")
parametersL160 = np.loadtxt("dataL160.txt")
parametersL320 = np.loadtxt("dataL320.txt")

WLS_sizesL20 = parametersL20[:,0]
PMT_hitsL20 = parametersL20[:,1]
PhotonsGeneratedL20 = parametersL20[:,2]

PMT_hitsL40 = parametersL40[:,1]
PhotonsGeneratedL40 = parametersL40[:,2]
PMT_hitsL80 = parametersL80[:,1]
PhotonsGeneratedL80 = parametersL80[:,2]
PMT_hitsL160 = parametersL160[:,1]
PhotonsGeneratedL160 = parametersL160[:,2]
PMT_hitsL320 = parametersL320[:,1]
PhotonsGeneratedL320 = parametersL320[:,2]

PMTArea = np.pi * 12.7 * 12.7

efficiencyL20 = PMT_hitsL20 / PhotonsGeneratedL20
areaL20 = (WLS_sizesL20 * WLS_sizesL20) - PMTArea

efficiencyL40 = PMT_hitsL40 / PhotonsGeneratedL40
efficiencyL80 = PMT_hitsL80 / PhotonsGeneratedL80
efficiencyL160 = PMT_hitsL160 / PhotonsGeneratedL160
efficiencyL320 = PMT_hitsL320 / PhotonsGeneratedL320

factorArea = areaL20 / PMTArea

gainL20 = 1 + factorArea * efficiencyL20

gainL40 = 1 + factorArea * efficiencyL40
gainL80 = 1 + factorArea * efficiencyL80
gainL160 = 1 + factorArea * efficiencyL160
gainL320 = 1 + factorArea * efficiencyL320

plt.plot(WLS_sizesL20,gainL20, label = "l = 20 cm")

plt.plot(WLS_sizesL20,gainL40, label = "l = 40 cm")
plt.plot(WLS_sizesL20,gainL80, label = "l = 80 cm")
plt.plot(WLS_sizesL20,gainL160, label = "l = 160 cm")
plt.plot(WLS_sizesL20,gainL320, label = "l = 320 cm")

plt.grid(linestyle='dotted')
plt.title('Gain as a function of square WLS length')
plt.xlabel('Square WSL plate length [cm]')
plt.ylabel("Gain")

plt.legend()
plt.show()

