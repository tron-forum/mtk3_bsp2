/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (ARMv7-M core)
 */

#ifndef _MTKBSP_TK_SYSLIB_DEPEND_CORE_H_
#define _MTKBSP_TK_SYSLIB_DEPEND_CORE_H_

#include <tk/errno.h>
#include <sys/sysdef.h>

/*----------------------------------------------------------------------*/
/*
 * CPU interrupt control for ARMv7-M.
 *	As armv7-m architecture does not support disable interrupt in
 *	xpsr register, we have to raise the exception priority to 
 *	that the interrupt group have. Write the BASEPRI to implement 
 *	disint.
 */

IMPORT void set_basepri(UW intsts);	/* Set Base Priority register */
IMPORT UW get_basepri(void);		/* Get Base Priority register */
IMPORT UW disint(void);			/* Disable interrupt */


#define DI(intsts)		( (intsts) = disint() )
#define EI(intsts)		( set_basepri(intsts) )
#define isDI(intsts)		( (intsts) != 0 )

#define INTLEVEL_DI		(0)
#define INTLEVEL_EI		(255)

/*
 * Interrupt priority grouping
 *
 *	PRIGROUP in the AIRCR register determines the split of group
 *	priority from subpriority. PRIGROUP is initialized to 3
 *	(pri:subpri = 4:4)) in the boot sequence.
 */
#define INTPRI_GROUP(pri, subpri)	(((pri) << (8-INTPRI_BITWIDTH)) | (subpri))


/* ------------------------------------------------------------------------ */
/*
 * Convert to interrupt definition number
 *
 * For backward compatibility.
 * 	INTVEC has been obsoleted since micro T-Kernel 2.0.
 */
#define DINTNO(intvec)	(intvec)

/* ------------------------------------------------------------------------ */
/*
 * I/O port access
 *	for memory mapped I/O
 */
Inline void out_d( UW port, UD data )
{
	*(_UD*)port = data;
}
Inline void out_w( UW port, UW data )
{
	*(_UW*)port = data;
}
Inline void out_h( UW port, UH data )
{
	*(_UH*)port = data;
}
Inline void out_b( UW port, UB data )
{
	*(_UB*)port = data;
}

Inline UD in_d( UW port )
{
	return *(_UD*)port;
}
Inline UW in_w( UW port )
{
	return *(_UW*)port;
}
Inline UH in_h( UW port )
{
	return *(_UH*)port;
}
Inline UB in_b( UW port )
{
	return *(_UB*)port;
}

#endif /* _MTKBSP_TK_SYSLIB_DEPEND_CORE_H_ */
