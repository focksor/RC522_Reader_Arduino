#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9           // 配置针脚
#define SS_PIN          10          
MFRC522 mfrc522(SS_PIN, RST_PIN);   // 创建新的RFID实例
MFRC522::MIFARE_Key key;
void setup() {
    Serial.begin(9600); // 设置串口波特率为9600
    while (!Serial);    // 如果串口没有打开，则死循环下去不进行下面的操作
    SPI.begin();        // SPI开始
    mfrc522.PCD_Init(); // Init MFRC522 card
 
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    Serial.println();
}
 
 
void loop() {
    // 寻找新卡
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
 
    // 选择一张卡
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
 
    // 显示卡片的详细信息
    Serial.print(F("UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("TYPE: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
 
    // 检查兼容性
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("error!"));
        return;
    }
 
    // 停止 PICC
    mfrc522.PICC_HaltA();
    //停止加密PCD
    mfrc522.PCD_StopCrypto1();
}
 
/**
 * 将字节数组转储为串行的十六进制值
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : "");
        Serial.print(buffer[i], HEX);
    }
}
