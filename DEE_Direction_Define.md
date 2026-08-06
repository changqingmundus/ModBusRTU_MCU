
/*encoder.h*/
#define DEE_Encoder_MagicKey 0            //DEE存放編碼器出廠狀態數據
#define DEE_Encoder_MultiTurnBitSize 1    //DEE存放編碼器多圈位數配置
#define DEE_Encoder_SingleTurnBitSize 2   //DEE存放編碼器單圈位數配置
#define DEE_Encoder_CRCBitSize 3          //DEE存放編碼器CRC位數配置

#define DEE_Speed_Update_Period 4         //DEE存放編碼器速度采集周期
#define DEE_POSITION_OFFSET_L   5         //DEE存放編碼器全部值低16位
#define DEE_POSITION_OFFSET_H   6         //DEE存放編碼器全部值高16位
#define DEE_Direction 7                   //DEE存放編碼器旋轉方向
#define DEE_MultiTurn_Origin_Mode 8       //DEE存放編碼器多圈原點數據



/*MB_User_Config.h*/
#define DEE_MODBUS_MagicKey 9
#define DEE_SLAVE_ID       10
#define DEE_BAUDRATE_INDEX 11
#define DEE_PARITY         12



/*Protocol_Config.h*/
#define DEE_Encoder_Protocol  13  //通信模式存儲地址
#define DEE_Update_Time       14  //上傳時間存儲地址