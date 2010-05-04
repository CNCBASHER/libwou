#ifndef __wb_regs_h__
#define __wb_regs_h__

/**
 *******************************************************************************
 * @registers for GPIO (General Purpose IO)
 *******************************************************************************
 * GPIO_BASE                0x0000
 * GPIO_MASK                0x000F
 *******************************************************************************
 * REG_NAME                 ADDR_OFFSET   ACCESS  DESCRIPTION
 * GPIO_SYSTEM    [ 1: 0]   0x0000        W       System Registers
 *    SOFT_RST    [    0]   0x0000        W       issue a RESET pulse for logic modules
 *    RECONFIG    [    1]   0x0000        W       make the FPGA in re-configuration mode, 
 *                                                let CPLD control the USB ports.
 * RESERVED       [ 7: 2]   0x0000
 * GPIO_LEDS      [ 7: 0]   0x0001        W       drive the 7i43 LEDS
 * GPIO_LEDS_SEL  [ 1: 0]   0x0002        W       LED source selection
 * GPIO_OUT       [ 7: 0]   0x0003        W       drive the 7i37 out ports
 * GPIO_MASK_IN0  [ 7: 0]   0x0004        W       mask for input bits [7:0]
 * GPIO_MASK_IN1  [ 7: 0]   0x0005        W       mask for input bits [15:8]
 *                                                inport = mask & bits_i
 *******************************************************************************
 *******************************************************************************
 * @registers for JCMD (Joint Command Processor)
 *******************************************************************************
 * JCMD_BASE            0x0020
 * JCMD_MASK            0x000F
 *******************************************************************************
 * REG_NAME             ADDR_OFFSET   ACCESS  DESCRIPTION
 * POS_W       [12: 0]  0x0000.FIFO   W       Relative Angle Distance (0 ~ 8191)
 * DIR_W       [   13]  0x0000.FIFO   W       Direction, (positive(1), negative(0))
 *          Write addr[1] will push {DIR_W,POS_W} into JCMD_FIFO. 
 *          The WB_WRITE got stalled if JCMD_FIFO is full. 
 * DIR_POL     [ 3: 0]  0x0001        W       Direction Polarity for
 *                                            mechanical movement
 *                                            馬達裝配方向相反時，同樣順時針旋
 *                                            轉，卻會造成機構往不同方向運動。
 *                                            用此參數以補償 inverse Kinematics
 *                                            的結果
 * RESERVED    [ 7: 0]  0x0002
 * RESERVED    [ 7: 0]  0x0003
 * 
 * //OBSOLETE: TBASE [ 3: 0] 0x0004 W         time base of pulse generator
 * //OBSOLETE:                                0: timebase is 2^15 ticks 
 * //OBSOLETE:                                   1.31ms = 2^15 * 1/25000
 * //OBSOLETE:                                1: timebase is 2^14 ticks
 * //OBSOLETE:                                   0.66ms = 2^14 * 1/25000
 * RESERVED    [ 7: 0]  0x0004
 *
 * JCMD_CTRL   [ 7: 0]  0x0005
 *    BPRU_EN  [    0]  0x0005        W       BasePeriod WOU Registers Update
 *                                            (1)enable (0)disable
 *                                            periodically and automatically transmit WOU registers to HOST
 *    SSIF_EN  [    1]  0x0005        W       Servo/Stepper Interface Enable
 *    RST      [    2]  0x0005        W       Reset JCMD
 *******************************************************************************
 * RESERVED             0x0030 ~ 0x007F
 *******************************************************************************
 * @REGISTERS FOR SSIF (Servo/Stepper InterFace)
 *******************************************************************************
 * SSIF_BASE            0X0080
 * SSIF_MASK            0X007F  (0X80 ~ 0XFF)
 * BP: Base Period register updating
 *******************************************************************************
 * REG_NAME             ADDR_OFFSET   ACCESS  DESCRIPTION
 * SSIF_SIF_CMD         0X0000        R       (0X00 ~ 0X0F) JNT_0 ~ JNT_3, SIF-COMMAND FROM JCMD FIFO
 * SSIF_PULSE_POS       0X0010        R(BP)   (0X10 ~ 0X1F) JNT_0 ~ JNT_3, PULSE-Position to Driver
 * SSIF_ENC_POS         0X0020        R(BP)   (0X20 ~ 0X2F) JNT_0 ~ JNT_3, ENCODER-POSITION FROM SERVO DRIVER
 * SSIF_SWITCHES        0X0030        R(BP)   (0X30 ~ 0X31) 16 INPUT SWITCHES FOR HOME, CCWL, AND CWL
 * RESERVED             0x0032~0x3F
 * SSIF_HOME_POS        0x0040        R       (0x40 ~ 0x4F) JNT_0 ~ JNT_3, home-position from servo driver
 * RESERVED             0x0050~0x007B
 * SSIF_MAX_PWM         0x007C~0x007F W       (0x7C ~ 0x7F) JNT_0 ~ JNT_3, 8-bits, Max PWM Ratio (Stepper Current Limit)
 *******************************************************************************
 * for 華谷：
 * JNT_0 ~ JNT_2: current limit: 2.12A/phase (DST56EX43A)
 *                set SSIF_MAX_PWM as 180
 * JNT_3:         current limit: 3.0A/phase (DST86EM82A)
 *                set SSIF_MAX_PWM as 255
 *******************************************************************************
 **/

/**
 * refer to stdint.h:
 * The ISO C99 standard specifies that in C++ implementations these
 * macros should only be defined if explicitly requested.
 * the following checking is for C only
 **/
#if !defined __cplusplus && !defined SWIG
#if !defined(UINT8_MAX) || !defined(UINT16_MAX) || !defined(INT32_MAX)
#error "Must include <inttypes.h> or <stdint.h> before any customized header."
#endif
#endif

// #undef ATTRIBUTE_PACKED
// 
// #if defined(__GNUC__)
// #if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
// #define ATTRIBUTE_PACKED __attribute__ ((packed))
// #endif
// #endif

// #if !defined(ATTRIBUTE_PACKED)
// #error "Unsupported C Compiler"
// #endif

// for WOU protocol (wishbone over usb)
#define WB_REG_SIZE     16384   // addressable wishbone register size (2^14)
#define WOU_HDR_SIZE    4       // header sizea of WOU_HEADER
#define MAX_DSIZE       256     // Maximum data size
#define WB_RD_CMD       0x00
#define WB_WR_CMD       0x80
#define WB_AI_MODE      0x40    // ADDRESS INCREMENT MODE
#define WB_FIFO_MODE    0x00    // FIFO MODE, const address

// GPIO register space: (8-bit GPIO for LEDs, purpose: test Wishbone protocol)
#define GPIO_BASE       0x0000
#define GPIO_MASK       0x000F
// offset to GPIO registers:
#define GPIO_SYSTEM     0x0000  // GPIO_SYSTEM.[1:0]
#define GPIO_SOFT_RST   0x01    // GPIO_SYSTEM.[0]
#define GPIO_RECONFIG   0x02    // GPIO_SYSTEM.[1]
#define GPIO_LEDS       0x0001  // GPIO_LEDS.[7:0]
#define GPIO_LEDS_SEL   0x0002  // GPIO_LEDS_SEL.[1:0] :
                                //  2'b00: select gpio_leds[7:0]
                                //  2'b01: select servo_if pulse output
                                //  2'b10: select debug_port_0
#define GPIO_OUT        0x0003  // GPIO_OUT.[7:0]
#define GPIO_MASK_IN0   0x0004  // mask for input bits [7:0], reset to 0x00
#define GPIO_MASK_IN1   0x0005  // mask for input bits [15:8], reset to 0x00
                                // inport = mask & bits_i

//obsolete: // SIF register space:
//obsolete: #define SIF_0_BASE      0x0020
//obsolete: #define SIF_MASK        0x001F
//obsolete: // offset to SIF registers:
//obsolete: #define SIF_REL_POS_A0  0x0000  // POSITION CMD
//obsolete: #define SIF_REL_POS_A1  0x0001
//obsolete: #define SIF_REL_POS_A2  0x0002
//obsolete: #define SIF_REL_POS_R0  0x0003
//obsolete: #define SIF_REL_POS_R1  0x0004
//obsolete: #define SIF_VELO_M0     0x0005
//obsolete: #define SIF_VELO_M1     0x0006
//obsolete: #define SIF_ACCEL       0x0007
//obsolete: #define SIF_DECEL       0x0008
//obsolete: #define SIF_SVELO       0x0009  // [3:0]
//obsolete: #define SIF_SACCEL      0x0009  // [6:4]
//obsolete: #define SIF_ANCHOR_V    0x000A
//obsolete: // place STB at last WRITE entry for USB bandwidth optimization
//obsolete: #define SIF_CTRL        0x000B  // {BUF_T, DIR_T, EXE_STB}
//obsolete: #define SIF_STATUS      0x000C  // {EXE_ACK}
//obsolete: #define SIF_ABS_POS_A0  0x000D
//obsolete: #define SIF_ABS_POS_A1  0x000E
//obsolete: #define SIF_ABS_POS_A2  0x000F
//obsolete: #define SIF_ABS_POS_R0  0x0010
//obsolete: #define SIF_ABS_POS_R1  0x0011
//obsolete: // #define SIF_OUT         0x000C  // {PC, CR, RES, SON}
//obsolete: // #define SIF_IN          0x000D  // {INP, RD, ALM}
//obsolete: // definitions for SIF registers
//obsolete: #define SIF_BUF_ABORT   0x00    // BUF_T: abort [4:2] == 0
//obsolete: #define SIF_DIR_FWD     0x02    // DIR_T: forward/positive ([1] == 1)
//obsolete: #define SIF_DIR_REV     0x00    // DIR_T: reverse/negative ([1] == 0)
//obsolete: 
//obsolete: #ifndef SWIG
//obsolete: typedef struct {
//obsolete:   uint16_t    wb_base;    // WB_REG base address
//obsolete:   // bitfield: http://www.coranac.com/documents/working-with-bits-and-bitfields/
//obsolete:   union {
//obsolete:     // uint32_t    data;
//obsolete:     uint8_t       regs[0x12];
//obsolete:     struct {
//obsolete:       // for little-endian
//obsolete:       uint8_t REL_POS_A0                    : 8;  /*   0.[7:0]   */
//obsolete:       uint8_t REL_POS_A1                    : 8;  /*   1.[7:0]   */
//obsolete:       uint8_t REL_POS_A2                    : 2;  /*   2.[1:0]   */
//obsolete:       uint8_t ZERO0                         : 6;  /*   2.[7:2]   */  
//obsolete:       uint8_t REL_POS_R0                    : 8;  /*   3.[7:0]   */
//obsolete:       uint8_t REL_POS_R1                    : 8;  /*   4.[7:0]   */
//obsolete:       uint8_t VELO_M0                       : 8;  /*   5.[7:0]   */
//obsolete:       uint8_t VELO_M1                       : 8;  /*   6.[7:0]   */
//obsolete:       uint8_t ACCEL                         : 8;  /*   7.[7:0]   */
//obsolete:       uint8_t DECEL                         : 8;  /*   8.[7:0]   */
//obsolete:       uint8_t SVELO                         : 4;  /*   9.[3:0]   */
//obsolete:       uint8_t SACCEL                        : 3;  /*   9.[6:4]   */
//obsolete:       uint8_t ZERO1                         : 1;  /*   9.[7]     */  
//obsolete:       uint8_t ANCHOR_V                      : 8;  /*   A.[7:0]   */
//obsolete:       // place STB at last WRITE entry for USB bandwidth optimization
//obsolete:       uint8_t EXE_STB                       : 1;  /*   B.[0]     */
//obsolete:       uint8_t DIR_T                         : 1;  /*   B.[1]     */
//obsolete:       uint8_t BUF_T                         : 3;  /*   B.[4:2]   */
//obsolete:       uint8_t ZERO2                         : 3;  /*   B.[7:5]   */
//obsolete:       uint8_t EXE_ACK                       : 1;  /*   C.[0]     */
//obsolete:       uint8_t ZERO3                         : 7;  /*   C.[7:1]   */
//obsolete:       uint8_t ABS_POS_A0                    : 8;  /*   D.[7:0]   */
//obsolete:       uint8_t ABS_POS_A1                    : 8;  /*   E.[7:0]   */
//obsolete:       uint8_t ABS_POS_A2                    : 2;  /*   F.[1:0]   */
//obsolete:       uint8_t ZERO4                         : 6;  /*   F.[7:2]   */  
//obsolete:       uint8_t ABS_POS_R0                    : 8;  /*  10.[7:0]   */
//obsolete:       uint8_t ABS_POS_R1                    : 8;  /*  11.[7:0]   */
//obsolete:     }; // struct
//obsolete:   }; // union
//obsolete: } sif_regs_t;
//obsolete: #endif // for non-SWIG
 
// JCMD register space:
#define JCMD_BASE       0x0020
#define JCMD_MASK       0x000F
// offset to JCMD registers
#define JCMD_POS_W      0x0000  // 2-bytes: {DIR_W, POS_W} in FIFO mode
#define JCMD_DIR_POL    0x0001  // Direction Polarity for mechanical movement
                                // 馬達裝配方向相反時，同樣順時針旋
                                // 轉，卻會造成機構往不同方向運動。
                                // 用此參數以補償 inverse Kinematics 的結果
                                // [0]: joint_0
                                // [1]: joint_1
                                // [2]: joint_2
                                // [3]: joint_3
// #define RESERVED     0x0002  
// #define RESERVED     0x0003  
//OBSOLETE: #define JCMD_TBASE      0x0004  // [3:0]
// #define RESERVED     0x0004  
#define JCMD_CTRL       0x0005  // [2:0]: {RST, SSIF_EN, BPRU_EN}

// (0x30 ~ 0x3F) RESERVED
// (0x40 ~ 0x7F) RESERVED

// registers for SSIF (Servo/Stepper InterFace)
// BASE_PERIOD_REGS: SSIF_PULSE_POS, SSIF_ENC_POS, SSIF_SWITCHES_IN
// NB: update bp_update wou_addr[] at usb_if.v
#define SSIF_BASE       0x0080  
#define SSIF_MASK       0x007F  // (0x80 ~ 0xFF)
//      REGISTERS       OFFSET  // DESCRIPTION
#define SSIF_SIF_CMD    0x0000  // (0x00 ~ 0x0F) JNT_0 ~ JNT_3, sif-command from jcmd FIFO
#define SSIF_PULSE_POS  0x0010  // (0x10 ~ 0x1F) JNT_0 ~ JNT_3, pulse-position to driver
#define SSIF_ENC_POS    0x0020  // (0x20 ~ 0x2F) JNT_0 ~ JNT_3, encoder-position from servo driver
#define SSIF_SWITCH_IN  0x0030  // (0x30 ~ 0x31) 16 input switches for HOME, CCWL, and CWL
                                // (0x32 ~ 0x3F) RESERVED
#define SSIF_HOME_POS   0x0040  // (0x40 ~ 0x4F) JNT_0 ~ JNT_3, home-position from servo driver
#define SSIF_MAX_PWM    0x007C  // (0x7C ~ 0x7F) JNT_0 ~ JNT_3, 8-bits, Max PWM Ratio (Stepper Current Limit)

#endif // __wb_regs_h__
