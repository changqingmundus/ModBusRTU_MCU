Modbus RTU Register:

Address     Description          Range
------------------------------------------------
0x0000      SingleTurn Value     0~65535       (FunctionCode:03H)
0x0001      MultiTurn Value      0~65535       (FunctionCode:03H)

0x0002      Slave ID             1~127         (FunctionCode:03H,06H,10H)

0x0003      Baud Rate Index      0x01~0x08     (FunctionCode:03H,06H,10H)
            0x01: 9600
            0x02: 19200
            0x03: 38400
            0x04: 57600
            0x05: 115200
            0x06: 4800
            0x07: 2400
            0x08: 1200

0x0004      Parity               0x01~0x03     (FunctionCode:03H,06H,10H)
            0x01: None
            0x02: Odd
            0x03: Even

0x0005      Encoder Zero         Write 0xFF    (FunctionCode:06H,10H)

0x0006      Count Direction       0x01~0x02    (FunctionCode:03H,06H,10H)
            0x01: CW  Increase
            0x02: CCW Increase

0x0007      Protocol              0x01~0x02    (FunctionCode:03H,06H,10H)
            0x01: Modbus RTU
            0x02: RS485 Custom

0x0008      Report Interval       1~3000 ms    (FunctionCode:03H,06H,10H)

0x0009      Encoder Value High     16-bit      (FunctionCode:03H)

0x000A      Encoder Value Low      16-bit      (FunctionCode:03H)

0x000B      Rotation Direction    0~2          (FunctionCode:03H)
            0x00:Stop
            0x01:CCW
            0x02:CW

0x000C      Speed                  RPM         (FunctionCode:03H)

0x0023      Zero Position          1~2         (FunctionCode:03H,06H,10H)
            /*after setzero*/
            0x01:Middle Position Reference 
            0x02:zero   Position Reference 

0x0024      Speed Update Period    1~200 (x10ms)    (FunctionCode:03H,06H,10H)

0x0032      Write Position Low     16-bit        (FunctionCode:10H)

0x0033      Write Position High    16-bit        (FunctionCode:10H)

0x0400      Encoder Value Low     16-bit         (FunctionCode:03H)

0x0401      Encoder Value High    16-bit         (FunctionCode:03H)