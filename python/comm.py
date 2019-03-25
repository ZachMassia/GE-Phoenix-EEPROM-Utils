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
MSG_ACK = 3

def read_x_bytes(ser, cnt):
    raw = ser.read(cnt)
    xs = bytearray(raw)
    xs.pop()
    return cobs.decode(xs)

def write_msg(ser, msg_type, data):
    data_mut = bytearray(data)
    data_mut.insert(0, msg_type)
    enc_msg = bytearray(cobs.encode(data_mut))
    enc_msg.append(0)
    print(f'Writing: {enc_msg}')
    ser.write(enc_msg)

def read_rom():
    file_path = 'ROM_Dump.pxd'
    with open(file_path, 'wb') as f:
        dump = ''
        with serial.Serial('COM14', 115200, timeout=5) as ser:    
            sleep(1) # Arduino reboot on port open
            print(f'Ack message: {read_x_bytes(ser, 32)}')
            write_msg(ser, MSG_READ, [1, 2, 3])
            sleep(8)
            dump = bytearray(read_x_bytes(ser, 512))
            msg_id = dump.pop(0)
        if dump and msg_id == MSG_PRINT:
            print(f'dump: {dump}')
            f.write(dump)
        

def write_rom():
    root = tk.Tk()
    root.withdraw()

    file_path = filedialog.askopenfilename()
    xs = bytearray(open(file_path, 'rb').read())

    with serial.Serial('COM14', 115200, timeout=2) as ser:
        write_msg(ser, MSG_WRITE, xs)
    
    print('Wrote:\n{}'.format(xs))


if __name__ == '__main__':
    option = int(input('Enter [0] to read chip contents, or [1] to write file to chip: '))
    if option == MSG_READ: read_rom()
    elif option == MSG_WRITE: write_rom()
