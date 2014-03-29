#ifndef	_DOLIXON_TYPE_H_
#define	_DOLIXON_TYPE_H_


typedef	unsigned int	__U32;
typedef	unsigned short	__U16;
typedef	unsigned char	__U8;
typedef	bool			__Bool;

typedef __U32	__Port;

typedef	void (*__PF_IntHandler)();
typedef void (*__PF_Task)();
typedef void (*__PF_IRQHandler)(int irq);
//可变参数
typedef char * va_list;

#endif
