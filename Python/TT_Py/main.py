from array import *
from SerialFunctions import *

lpm_com = 'COM5'                                                                    # defines lpm COM
tt_com = 'COM10'                                                                    # defines tt COM
patl = 0.8                                                                          # test tool path length

xmega = init_tt(tt_com)                                                             # returns tt serial object

xmega.write(5)
v1 = read_float(xmega)
v2 = read_float(xmega)
v3 = read_float(xmega)
v4 = read_float(xmega)
v5 = read_float(xmega)
v6 = read_float(xmega)
v7 = read_float(xmega)

lpm = init_lpm(lpm_com)                                                             # returns lpm serial object
set_lpm(lpm, patl)                                                                  # writes settings to lpm


