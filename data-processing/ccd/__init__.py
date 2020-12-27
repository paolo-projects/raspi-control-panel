import struct
import serial
from typing import List


RX_SIZE = 3694*2


def tx_data(sh: int, icg: int, integrations: int, single: bool = True) -> bytes:
    return struct.pack('>BBIIBB', 0x45, 0x52, sh, icg, 0x00 if single is True else 0x01, integrations)


def rx_parse_data(data: bytes) -> List[int]:
    results = []
    for value in struct.iter_unpack("<H", data):
        results.append(value[0])
    return results


def request_data(serial_port, sh=500, icg=100000) -> List[int]:
    with serial.Serial(serial_port, baudrate=115200, timeout=3) as ccd:
        tx_dt = tx_data(sh=sh, icg=icg, integrations=1, single=True)
        print(f"Writing {len(tx_dt)} bytes of data [{tx_dt}]...")
        wrt_count = ccd.write(tx_dt)
        if not (wrt_count and wrt_count == len(tx_dt)):
            raise ConnectionError(f"Couldn't write TX data. {wrt_count} bytes written")

        print("Receiving back...")
        recv = ccd.read(RX_SIZE)

    print(f"Read {len(recv)} bytes, parsing the result...")
    rx_data = rx_parse_data(recv)
    return rx_data