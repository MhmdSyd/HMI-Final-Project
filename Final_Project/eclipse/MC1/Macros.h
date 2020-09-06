/********************************************************************************
 *
 * Module: Important - Macros
 *
 * File Name: Macros.h
 *
 * Description: File include all rename data types libraries
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

#ifndef MACROS_H_
#define MACROS_H_

/* Clear known bit in register */
#define CLEAR_BIT(PORT,BITS) (PORT &=~ (1<<BITS))

/* Set known bit in register */
#define SET_BIT(PORT,BITS) (PORT |= (1<<BITS))

/* Toggle known bit in register */
#define TOGGLE_BIT(PORT,BITS) (PORT ^= (1<<BITS))

/* Check that bit is clear in register */
#define BIT_IS_SET(PIN,BITS) (PIN & (1<<BITS))

/* Check that bit is set in register */
#define BIT_IS_CLEAR(PIN,BITS) (!(PIN & (1<<BITS)))

/* Rotate register  data to lift like ( 00011011 ) to ( 11011000 )*/
#define ROL(REG,num) ( REG= (REG<<num) | (REG>>(8-num)) )

/* Rotate register  data to right like ( 11101010 ) to ( 01011101 ) NUM =3 */
#define ROR(REG,NUM) (  REG = (REG<<(8-NUM)) | (REG>>(NUM)) )

/* Create union REG to access REGA by bit bit */
#define PORTA_REG (*(volatile REG * const) 0x3B)
#define DDRA_REG   (*(volatile REG * const) 0x3A)
#define PINA_REG    (*(volatile const REG * const) 0x39)

/* Create union REG to access REGB by bit bit */
#define PORTB_REG (*(volatile REG * const) 0x38)
#define DDRB_REG   (*(volatile REG * const) 0x37)
#define PINB_REG    (*(volatile const REG * const) 0x36)

/* Create union REG to access REGC by bit bit */
#define PORTC_REG (*(volatile REG * const) 0x35)
#define DDRC_REG   (*(volatile REG * const) 0x34)
#define PINC_REG    (*(volatile const REG * const) 0x33)

/* Create union REG to access REGD by bit bit */
#define PORTD_REG (*(volatile REG * const) 0x32)
#define DDRD_REG   (*(volatile REG * const) 0x31)
#define PIND_REG    (*(volatile const REG * const) 0x30)

#endif /* MACROS_H_ */
