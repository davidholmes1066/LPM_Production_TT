from array import *
from SerialFunctions import *

lpm_com = 'COM5'                                                                    # defines lpm COM
tt_com = 'COM10'                                                                    # defines tt COM

patl = 0.8                                                                          # test tool path length

init_tt(tt_com)                                                                     # returnes tt serial object and enables ext supply

lpm = init_lpm(lpm_com)                                                             # returns lpm serial object
set_lpm(lpm, patl)                                                                  # writes settings to lpm


