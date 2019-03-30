from cobs import cobs
from serial.tools.list_ports import comports
from enum import IntEnum
from tkinter import filedialog
from datetime import datetime
from time import sleep

import struct
import serial

import tkinter as tk

MSG_BYTE_LOC = 0
MSG_READ  = 0
MSG_WRITE = 1
MSG_PRINT = 2
MSG_ACK   = 3
MSG_DELIM = b'\x00'

ORIG_BYTE_CNT = 410

BAUD_RATE = 115200
TIMEOUT = 0.5 # seconds

def get_serial_port():
    ports = comports()
    print('Select a COM port:')
    for i, port in enumerate(ports, 1):
        print(f'\t[{i}]\t\t"{port[0]}"')
    i = input(f'Port [{1}-{len(ports)}, 0=exit]: ')

    if i == '0':
        from sys import exit
        exit()
    i = int(i)

    ser = serial.Serial(ports[i - 1][0], BAUD_RATE)
    sleep(0.2)
    ack_msg = read_cobs_msg(ser)
    if ack_msg[0] != MSG_ACK:
        from sys import exit
        print(f'Did not receive reboot ACK ({MSG_ACK}). Got:\n{ack_msg[0]}\nQuitting.')
        exit()

    print('Received reboot ACK.')
    return ser


def read_cobs_msg(ser, debug=False):
    raw = ser.read_until(MSG_DELIM, 512)
    xs = bytearray(raw)
    xs.pop() # Remove delim.
    dec = bytearray(cobs.decode(xs))

    if debug:
        print(f'DEBUG READ: [RAW] - {raw} | [DECODED] - {dec}')
    return dec


def write_msg(ser, msg_type, data=None, debug=False):
    enc_msg = ''
    if data:
        data_mut = bytearray(data)
        data_mut.insert(0, msg_type)
        enc_msg = bytearray(cobs.encode(data_mut))
    else:
        msg_t_bytes = bytearray([msg_type])
        enc_msg = bytearray(cobs.encode(msg_t_bytes))

    enc_msg.append(0)

    if debug: print(f'Writing encoded message:\n{enc_msg}')
    return ser.write(enc_msg)


def read_rom(ser):
    file_path = 'ROM_Dump.pxd'
    with open(file_path, 'wb') as f:
        write_msg(ser, MSG_READ)
        sleep(.1)

        dump = read_cobs_msg(ser)
        msg_id = dump.pop(0)

        if dump and msg_id == MSG_PRINT:
            # Pad the file with trailing 0 bytes to match original file size.
            rx_size = len(dump)
            pad = ORIG_BYTE_CNT - rx_size
            dump.extend(bytes(pad))
            b = f.write(dump)
            print(f'Wrote {b} bytes to log file. (Padded with {pad} 0x00 bytes.)')
        

def write_rom(ser):
    root = tk.Tk()
    root.withdraw()

    #file_path = filedialog.askopenfilename()
    file_path = '/Users/zach/Downloads/YCBNode.pxd'
    xs = bytearray(open(file_path, 'rb').read())

    b = write_msg(ser, MSG_WRITE, xs)
    print(f'Sent {b} bytes')

    sleep(0.25)
    ack_msg = read_cobs_msg(ser, debug=False)
    if ack_msg[0] == MSG_ACK:
        b = struct.unpack_from('<H', ack_msg, 1)[0]
        print(f'Received write confirmation for {b} bytes.')


if __name__ == '__main__':
    port = get_serial_port()
    sleep(.2)

    option = int(input('Enter [0] to read chip contents, or [1] to write file to chip: '))
    if option == MSG_READ: read_rom(port)
    elif option == MSG_WRITE: write_rom(port)
