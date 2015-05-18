#ifndef __IDA_SDK_H
#define __IDA_SDK_H
/*IDA Define */

#undef	itoa
#define itoa		_itoa
#undef	strdup
#define strdup		_strdup
#undef	strcpy
#define strcpy		qstrncpy
#undef  strtrim
#define strtrim		trim
#undef  sprintf
#define sprintf		qsnprintf
#undef	printf
#define printf		msg
/**/
#undef  fopen
#define fopen		qfopen
#undef  fclose
#define fclose		qfclose
#undef  fseek
#define fseek		qfseek
#undef  fgetc
#define fgetc		qfgetc
#undef  fgets
#define fgets		qfgets
#undef  fputs
#define fputs		qfputs
#undef  fprintf
#define fprintf		qfprintf
#undef  fscanf
#define fscanf		qfscanf
#undef	fputc
#define fputc		qfputc
/**/
/*¶¨Òå__DEBUG__º¯Êý*/
#ifdef	_DEBUG
	#define Debug_Run(val) val	
#else
	#define Debug_Run(val) 
#endif
//
#ifdef _DEBUG
	#define	 _MSG	msg
#else 
	#define	 _MSG
#endif 


#endif