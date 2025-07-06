/**===========================================================================
 * @file		CPU.h
 * @brief		CPU Header File
 * @author		mohamed elbatawi
 * @see
 * @Date		2025.04.23
 * @Version		2.0 
 *
==============================================================================*/

#ifndef	_CPU_H_
#define	_CPU_H_


/***************************************************************************************************
 *	[Include File]
 **************************************************************************************************/
#include <stdint.h>
#include <cmsis_iar.h> 
#if defined(_CPU_TMPM374FW_)
#include <intrinsics.h>
#include <stdlib.h>
#elif defined(_CPU_TMPM370FY_)
#include <intrinsics.h>
#include <stdlib.h>
#include "../core/tmpm370/inc/tmpm370_wdt.h"
#endif


/***************************************************************************************************
 *	[Define]
 **************************************************************************************************/
//#define FALSE			0
//#define TRUE			1

#define LOW				0
#define HI				1

#define OFF				0
#define ON				1

#define NG				0
#define OK				1


/***************************************************************************************************
 *	[Enumeration & TypeDeclarations]
 **************************************************************************************************/

typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef signed int			Q15;
typedef signed long			Q30;

typedef unsigned char       BOOLEAN;
typedef unsigned char       U08;
typedef signed char         S08;
typedef unsigned short      U16;
typedef signed short        S16;
typedef unsigned long       U32;
typedef signed long         S32;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_CPU_TMPM374FW_)
#define INTERRUPT		void	
#define PUBLIC 
#define DEF_GLOBAL		extern
#define DEF_READONLY	const extern

// Entering/exiting critical area (interrupt prohibited area) (=ũ��ƼĮ ����(���ͷ�Ʈ ��������) ����/����) ------------------
typedef unsigned int TCpuSR;
TCpuSR  OS_CPU_SR_Save(void);
void    OS_CPU_SR_Restore(TCpuSR cpuSR);

#define HAL_DISABLE_INTERRUPT()		__disable_interrupt()
#define HAL_ENABLE_INTERRUPT()		__enable_interrupt()
#define CPU_ENTER_CRITICAL()		{cpuSR = OS_CPU_SR_Save();}
#define CPU_EXIT_CRITICAL()			{OS_CPU_SR_Restore(cpuSR);}

#define CPU_ENTER_INTERRUPT()		
#define CPU_EXIT_INTERRUPT()

#define HAL_REFRESH_WATCHDOG()		{WDCR = 0x4E; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(_CPU_TMPM370FY_)
#define INTERRUPT		void	
#define PUBLIC 
#define DEF_GLOBAL		extern
#define DEF_READONLY	const extern

// Entering/exiting critical area (interrupt prohibited area) (=ũ��ƼĮ ����(���ͷ�Ʈ ��������) ����/����) ------------------
typedef unsigned int TCpuSR;
TCpuSR  OS_CPU_SR_Save(void);
void    OS_CPU_SR_Restore(TCpuSR cpuSR);

#define HAL_DISABLE_INTERRUPT()		__disable_irq()
#define HAL_ENABLE_INTERRUPT()		__enable_irq()
#define CPU_ENTER_CRITICAL()		{cpuSR = OS_CPU_SR_Save();}
#define CPU_EXIT_CRITICAL()			{OS_CPU_SR_Restore(cpuSR);}

#define CPU_ENTER_INTERRUPT()		
#define CPU_EXIT_INTERRUPT()

#define HAL_REFRESH_WATCHDOG()		{WDT_WriteClearCode(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
#define INTERRUPT		void	
#define PUBLIC 
#define DEF_GLOBAL		extern
#define DEF_READONLY	const extern

// Entering/exiting critical area (interrupt prohibited area) (=ũ��ƼĮ ����(���ͷ�Ʈ ��������) ����/����) ------------------
typedef unsigned int TCpuSR;
TCpuSR  OS_CPU_SR_Save(void);
void    OS_CPU_SR_Restore(TCpuSR cpuSR);


#define HAL_DISABLE_INTERRUPT()		__disable_irq()
#define HAL_ENABLE_INTERRUPT()		__enable_irq()
#define CPU_ENTER_CRITICAL()		{cpuSR = OS_CPU_SR_Save();}
#define CPU_EXIT_CRITICAL()			{OS_CPU_SR_Restore(cpuSR);}

#define CPU_ENTER_INTERRUPT()		
#define CPU_EXIT_INTERRUPT()

/*mabrouk edit wd cr*/
#define HAL_REFRESH_WATCHDOG()		{WDT_WriteClearCode(); }
//#define HAL_REFRESH_WATCHDOG()		{WDCR = 0x4E; }

/***************************************************************************************************
 *	[Constant & Variable Declarations]
 **************************************************************************************************/

/***************************************************************************************************
 *	[Function Declaration]
 **************************************************************************************************/

#endif	//_CPU_H_
/***************************************************************************************************
 *	[End Of Header File]
 **************************************************************************************************/
