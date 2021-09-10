test: "PINA = 0x01 -> PORTB  0x01"
setPINA 0x01
continue 5
expectedPORTB 0x01
checkResult