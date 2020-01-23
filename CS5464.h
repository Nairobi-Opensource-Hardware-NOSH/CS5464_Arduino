#ifndef __CS5464_SPI_H__
#define __CS5464_SPI_H__

/* Define all the register from DATASHEET for CS5464 */
/*****************************************************/

/*Register Read and Write commands contain only 5 address bits. But the internal address bus of the CS5464 is
12 bits wide. Therefore, registers are organized into “Pages”. There are 128 pages of 32 registers each. The
Page register provides the 7 high-order address bits and selects one of the 128 register pages. Not all pages
are used*/
#define PAGE_REGISTER 0x1F

/* PAGE0 Registers */
#define CONFIG 0x00
#define INST_CUR1 0x01
#define INST_VOL1 0x02
#define INST_POW1 0x03
#define INST_CUR2 0x07
#define INST_VOL2 0x08
#define INST_POW2 0x09
#define AVG_POW1 0x04
#define AVG_POW2 0x0A
#define RMS_CUR1 0x05
#define RMS_VOL1 0x06
#define RMS_CUR2 0x0B
#define RMS_VOL2 0x0C
#define INST_Q1 0x0E
#define INST_Q2 0x11
#define AVG_Q1 0x0D
#define AVG_Q2 0x10
#define PEAK_CUR1 0x12
#define PEAK_VOL1 0x13
#define PEAK_CUR2 0x16
#define PEAK_VOL2 0x17
#define APP_POW1 0x14
#define APP_POW2 0x18
#define PF1 0x15
#define PF2 0x19
#define TEMP 0x1B
#define E_PULSE 0x1D
#define S_PULSE 0x1E
#define Q_PULSE 0x1F
#define STATUS 0x0F
#define MASK 0x1A
#define CTRL 0x1C

/* PAGE1 Registers */
#define OFF_CUR1 0x00
#define OFF_VOL1 0x02
#define OFF_CUR2 0x07
#define OFF_VOL2 0x09
#define GAIN_CUR1 0x01
#define GAIN_VOL1 0x03
#define GAIN_CUR2 0x08
#define GAIN_VOL2 0x0A
#define OFF_POW1 0x04
#define OFF_POW2 0x0B
#define ACOFF_CUR1 0x05
#define ACOFF_VOL1 0x06
#define ACOFF_CUR2 0x0C
#define ACOFF_VOL2 0x0D
#define MODES 0x10
#define EPSILON 0x11
#define PULSEWIDTH 0x0E
#define PULSERATE 0x0F
#define CYCLECOUNT 0x13
#define ICHAN_LEVEL 0x12
#define E_MIN 0x18
#define WB_Q1 0x14
#define WB_Q2 0x15
#define TEMP_GAIN 0x16
#define TEMP_OFFSET 0x17
#define TIME_SETTLE 0x19
#define LOAD_MIN 0x1A
#define RMS_VF 0x1B
#define SYS_GAIN 0x1C
#define SYS_TIME 0x1D


/* PAGE2 Registers */
#define SAG_DUR_VOL1 0x00 // Voltage1 Sag duration
#define SAG_DUR_VOL2 0x08 // Voltage2 Sag duration
#define FLT_DUR_CUR1 0x04 // Current1 Fault duration
#define FLT_DUR_CUR2 0x0C // Current2 Fault duration
#define SAG_LVL_VOL1 0x01 // Voltage1 Sag level
#define SAG_LVL_VOL2 0x09 // Voltage2 Sag level
#define FLT_LVL_CUR1 0x05 // Current1 Fault level
#define FLT_LVL_CUR2 0x0D // Current2 Fault level

#endif