#include "debug.h"
#include "ch32v00x_gpio.h"
#include "ch32v00x_rcc.h"

// ต้องแน่ใจว่าได้ include ไฟล์ delay.h และเรียกใช้ฟังก์ชัน Delay_Init()
extern void Delay_Ms(uint32_t n); // ประกาศว่ามีฟังก์ชันนี้อยู่

// *** เปลี่ยน: กำหนดขา GPIO สำหรับควบคุม MOSFET และ LED เป็น PC1 และ PC2 ***
#define MOSFET_PIN GPIO_Pin_1 // เปลี่ยนจาก GPIO_Pin_0 (สำหรับ PD0) เป็น GPIO_Pin_1 (สำหรับ PC1)
#define MOSFET_PORT GPIOC     // เปลี่ยนจาก GPIOD เป็น GPIOC

#define LED_PIN GPIO_Pin_2   // เปลี่ยนจาก GPIO_Pin_1 (สำหรับ PD1) เป็น GPIO_Pin_2 (สำหรับ PC2)
#define LED_PORT GPIOC     // เปลี่ยนจาก GPIOD เป็น GPIOC

int main(void)
{
    Delay_Init();
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 1. เปิด Clock สำหรับ Port ที่ใช้งาน (GPIOC)
    // *** เปลี่ยน: จาก RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); ***
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // เปิด Clock ของ GPIOC

    // 2. กำหนดค่าขา GPIO สำหรับ MOSFET Driver (PC1)
    GPIO_InitStruct.GPIO_Pin = MOSFET_PIN; // ใช้ค่าจาก Define ใหม่ (GPIO_Pin_1)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOSFET_PORT, &GPIO_InitStruct); // ใช้ค่าจาก Define ใหม่ (GPIOC)

    // 3. กำหนดค่าขา GPIO สำหรับ LED (PC2)
    // ต้อง Reset GPIO_InitStruct ก่อนใช้ซ้ำ หรือประกาศใหม่ก็ได้ ถ้าเป็น struct เดียวกันต้องระวังค่าเก่าค้าง
    // แต่ในที่นี้ กำหนดค่า Pin ใหม่ทับไปเลยได้
    GPIO_InitStruct.GPIO_Pin = LED_PIN; // ใช้ค่าจาก Define ใหม่ (GPIO_Pin_2)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStruct); // ใช้ค่าจาก Define ใหม่ (GPIOC)

    // ตั้งค่าสถานะเริ่มต้น (จะเริ่มที่ ON หรือ OFF ก่อนก็ได้)
    // สำหรับวงจร Driver NPN+P-Channel: High = ON, Low = OFF
    // ตั้งค่าเริ่มต้นที่สถานะปิดก่อนเล็กน้อย
    GPIO_WriteBit(MOSFET_PORT, MOSFET_PIN, Bit_RESET); // Set PC1 Low -> MOSFET OFF
    GPIO_WriteBit(LED_PORT, LED_PIN, Bit_RESET);     // Set PC2 Low -> LED OFF
    Delay_Ms(100); // หน่วงเล็กน้อยก่อนเริ่ม Loop หลัก

    while(1)
    {
        // --- ช่วงเปิดไฟ (5 นาที) ---
        // *** ใช้ค่า Port และ Pin จาก Define ใหม่ ***
        GPIO_WriteBit(MOSFET_PORT, MOSFET_PIN, Bit_SET); // Set PC1 High -> MOSFET ON (จ่ายไฟให้ USB)

        //คอมเม้นต์เพื่อปิดไฟแสดงสถานะตอนที่จ่ายไฟ ถ้าจะให้เปิดไฟแสดงสถานะให้เอาคอมเมนต์ออก
        //GPIO_WriteBit(LED_PORT, LED_PIN, Bit_SET);     // Set PC2 High -> LED ON (เปิดค้างแสดงสถานะจ่ายไฟ)

        // หน่วงเวลา 5 นาที (5 * 60 = 300 วินาที)ตอนจ่ายไฟ
    // **** ค่าความคลาดเคลื่อนอยู่ที่ 2 เท่า จะต้องลดรอบลงครึ่งหนึ่ง เป็นวนรอบ 150 ครั้ง ถึงจะเท่ากับ 5 นาที
        for(int i = 0; i < 150; i++)
        {
            Delay_Ms(1000); // หน่วง 1 วินาที
        }

        // --- ช่วงปิดไฟ (5 นาที) พร้อม LED กระพริบ ---
        // *** ใช้ค่า Port และ Pin จาก Define ใหม่ ***
        GPIO_WriteBit(MOSFET_PORT, MOSFET_PIN, Bit_RESET); // Set PC1 Low -> MOSFET OFF (หยุดจ่ายไฟให้ USB)
        // ในช่วงปิดไฟนี้ LED จะต้องกระพริบ

        // หน่วงเวลา 5 นาที (300 วินาที) ด้วยการกระพริบ LED (จังหวะ 1Hz: ติด 500ms, ดับ 500ms)
        // **** ค่าความคลาดเคลื่อนอยู่ที่ 2 เท่า จะต้องลดรอบลงครึ่งหนึ่ง เป็นวนรอบ 150 ครั้ง ถึงจะเท่ากับ 5 นาที
        for(int i = 0; i < 150; i++)
        {
            // *** ใช้ค่า Port และ Pin จาก Define ใหม่ ***
            GPIO_WriteBit(LED_PORT, LED_PIN, Bit_SET);   // Turn PC2 ON (LED ติด)
            Delay_Ms(500);                              // หน่วง 500 มิลลิวินาที

            // *** ใช้ค่า Port และ Pin จาก Define ใหม่ ***
            GPIO_WriteBit(LED_PORT, LED_PIN, Bit_RESET); // Turn PC2 OFF (LED ดับ)
            Delay_Ms(500);                              // หน่วงอีก 500 มิลลิวินาที
        }

        // Loop จะกลับไปที่จุดเริ่มต้นของ while(1)
        // เพื่อเริ่มช่วงเปิดไฟ 5 นาทีอีกครั้ง
    }
}
