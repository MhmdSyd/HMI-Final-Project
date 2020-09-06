/********************************************************************************
 *
 * Module: Data - Types Rename
 *
 * File Name: Std_Types.h
 *
 * Description: File include all rename data types libraries
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/* Data type rename */
typedef signed char 				sint8;
typedef signed short 				sint16;
typedef signed long 				sint32;
typedef signed long long 		sint64;
typedef unsigned char 			uint8;
typedef unsigned short 			uint16;
typedef unsigned long 			uint32;
typedef unsigned long long   uint64;
typedef float 							float32;
typedef double 						float64;

/* register access bit bit or one time by data */
typedef union{
	uint8 DATA;
	struct{
		uint8 BIT0 : 1;
		uint8 BIT1 : 1;
		uint8 BIT2 : 1;
		uint8 BIT3 : 1;
		uint8 BIT4 : 1;
		uint8 BIT5 : 1;
		uint8 BIT6 : 1;
		uint8 BIT7 : 1;
	}BITS;
}REG;

/* Boolean Data Type */
typedef unsigned char bool;

/* Boolean Values */
enum{
	FALSE=0,TRUE,LOW=0,HIGHT,CLEAR=0,SET
};

#endif /* STD_TYPES_H_ */
