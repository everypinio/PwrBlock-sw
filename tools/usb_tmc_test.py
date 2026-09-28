#!/usr/bin/env python3

import pyvisa
import time
import sys
from pathlib import Path


FIRMWARE_VERSION = (Path(__file__).resolve().parents[1] / "VERSION").read_text().strip()


def test_idn():
    idn = inst.query("*idn?")
    print(f"ID: {idn}")
    assert idn == f"EVERYPINIO,POWERBLOCK,0,{FIRMWARE_VERSION}\r\n"
    assert inst.is_4882_compliant


def test_out():
    state = 1
    print(f"SET {state}")
    inst.write(f"output:state {state}")
    out_state = inst.query(f"output:state?")
    print(f"GET {out_state}")
    assert out_state == "1\r\n", "Fail to set output state INT"
    state = "OFF"
    inst.write(f"output:state OFF")
    print(f"SET {state}")
    out_state = inst.query(f"output:state?")
    print(f"GET {out_state}")
    assert out_state == "0\r\n", "Fail to set output state STR"


rm = pyvisa.ResourceManager()
reslist = rm.list_resources("USB?::?*::INSTR")
print(reslist)

if len(reslist) == 0:
    sys.exit()

inst = rm.open_resource(reslist[0])
inst.timeout = 3000

inst.clear()

print("+ IDN")
test_idn()

print("+ OUT")
test_out()

inst.close()
print("Test complete")
