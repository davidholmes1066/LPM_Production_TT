import minimalmodbus as mm
import serial as s


def init_lpm(com):
    lpm = mm.Instrument(com, 1, mm.MODE_RTU)                                    # Set serial device

    lpm.serial.baudrate = 115200                                                # Set baud rate
    lpm.serial.bytesize = 8                                                     # Set 8 bits/ byte
    lpm.serial.parity = mm.serial.PARITY_NONE                                   # No parity bit
    lpm.serial.stopbits = 2                                                     # Double stop bit
    lpm.serial.timeout = 1                                                      # Time out after one second

    lpm.close_port_after_each_call = True                                       # Good practice
    lpm.clear_buffers_before_each_transaction = True                            # Apparently also good practice

    return lpm

def init_tt(com):
    xmega = s.Serial(com, 115200)                                               # Generate serial object xmega
    xmega.write(1)                                                              # Enable external supply
    return xmega


def set_lpm(lpm, patl):
    lpm.write_float(11150, patl)                                                # Set path 1 to patl
    lpm.write_float(11154, patl)                                                # Set path 2 to patl
    lpm.write_float(11158, patl)                                                # Set path 3 to patl
    lpm.write_float(11162, patl)                                                # Set path 4 to patl
    lpm.write_register(12560, 11)                                               # Set to 4 path

    lpm.write_register(12680, 1000)                                             # T sensor type
    lpm.write_float(11500, 985.0)                                               # Kr parameter
    lpm.write_float(11502, 0.004)                                               # Vosr parameter
    lpm.write_float(11504, -1.0)                                                # Tos
    lpm.write_register(12681, 1)                                                # P sensor type
    lpm.write_float(11506, 156.04)                                              # mV per V
    lpm.write_float(11508, 7.0)                                                 # Pfs
    lpm.write_float(11510, 0.0039)                                              # Vosp
    lpm.write_float(11512, 0.9958)                                              # Kpv
    lpm.write_float(11514, 0.9)                                                 # Kpp
    lpm.write_float(11516, -0.2)                                                # Pos
    lpm.write_float(11518, 0.0)                                                 # Puos
    lpm.write_float(11520, 1.0)                                                 # Puk
    lpm.write_float(11522, 0.0)                                                 # Tuos
    lpm.write_float(11524, 1.0)                                                 # Tuk
    lpm.write_float(11526, -20.0)                                               # Tmin
    lpm.write_float(11528, 60.0)                                                # Tmax
    lpm.write_float(11530, 0.0)                                                 # Pmin
    lpm.write_float(11532, 20.0)                                                # Pmax
