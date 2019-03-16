from cobs import cobs
from serial.tools.list_ports import comports
from enum import IntEnum
from tkinter import filedialog
from datetime import datetime
from time import sleep

import serial

import tkinter as tk

MSG_BYTE_LOC = 0
MSG_READ = 0
MSG_WRITE = 1
MSG_PRINT = 2


def read_rom():
    now = datetime.now()
    file_path = f'ROM_Dump_{now.strftime("%b-%d-%Y_%k:%M:%S")}.pxd'
    with open(file_path, 'wb') as f:
        dump = ''
        with serial.Serial() as ser:
            ser.baudrate = 115200
            ser.timeout = 2 # seconds
            ser.port = comports()[0]
            ser.open()
            sleep(2) # Arduino reboot on port open
            ser.flush()

            enc_msg = cobs.encode(bytes([MSG_PRINT]))
            ser.write(enc_msg)

            dump = cobs.decode(ser.read(512))
        f.write(dump)
        

def write_rom():
    root = tk.Tk()
    root.withdraw()

    file_path = filedialog.askopenfilename()
    xs = bytearray(open(file_path, 'rb').read())
    xs.insert(MSG_BYTE_LOC, MSG_WRITE) # write file command byte

    xs_enc = cobs.encode(bytes(xs))
    print('Wrote:\n{}'.format(xs_enc))

    with serial.Serial() as ser:
        ser.baudrate = 115200
        ser.port = comports()[0]
        ser.open()
        ser.write(xs_enc)


if __name__ == '__main__':
    option = int(input('Enter [0] to read chip contents, or [1] to write file to chip: '))
    if option == MSG_READ: read_rom()
    elif option == MSG_WRITE: write_rom()
