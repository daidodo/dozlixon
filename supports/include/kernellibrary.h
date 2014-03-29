#ifndef	_DOLIXON_KERNELLIBRARY_H_
#define	_DOLIXON_KERNELLIBRARY_H_

#include "proto.h"

struct Process;

struct KernelLibrary
{
	static __U32 video_current_start_addr;
	//extern "C"
//	static void		(* disp_str)(const char * pszInfo);
	static void		disp_str(const char * pszInfo);
	static void		(* disp_color_str)(const char * info, int color);
	static void		(* out_byte)(__Port port,__U8 value);
	static __U8		(* in_byte)(__Port port);
	static void		(* enable_irq)(int irq);
	static void		(* disable_irq)(int irq);
	static void *	(* memcpy)(void* p_dst, void* p_src, int size);
	static void		(* memset)(void* p_dst, char ch, int size);
	static void		(* enable_int)();
	static void		(* disable_int)();
	//C++
	static void	disp_int(int value);
	static void delay(int time);
	static void disp_str_DONE(__Bool done = true);
	static void scroll_down();
	//system calls' implementation
	static int sys_get_ticks();
	static int sys_write(const char * buf, int len, Process * p_proc);
};

struct SystemCall
{
	static const __PF_IntHandler sys_call;
	//system calls' interface
	static int (* get_ticks)();
	static void (* write)(const char * buf, int len);
};

#endif
