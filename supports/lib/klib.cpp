#include "kernellibrary.h"
#include "usrlibrary.h"
#include "global.h"

extern "C"{
	//klib.asm
	void disp_str_ASM_FN(const char * pszInfo);
	void disp_color_str_ASM_FN(const char * info, int color);
	void out_byte_ASM_FN(__Port port,__U8 value);
	__U8 in_byte_ASM_FN(__Port port);
	void enable_irq_ASM_FN(int irq);
	void disable_irq_ASM_FN(int irq);
	void enable_int_ASM_FN();
	void disable_int_ASM_FN();

	//string.asm
	void * memcpy_ASM_FN(void* p_dst, void* p_src, int size);
	void memset_ASM_FN(void* p_dst, char ch, int size);

	//kernel.asm
	void sys_call_ASM_FN();

	//syscall.asm
	int get_ticks_ASM_FN();
	void write_ASM_FN(const char* buf, int len);
}

//struct KernelLibrary
__U32 KernelLibrary::video_current_start_addr = 0;
//extern "C"
//void	(* KernelLibrary::disp_str)(const char * pszInfo) = disp_str_ASM_FN;
void	KernelLibrary::disp_str(const char * pszInfo)
{
	disp_str_ASM_FN(pszInfo);
	scroll_down();
}
void	(* KernelLibrary::disp_color_str)(const char * info, int color) = disp_color_str_ASM_FN;
void	(* KernelLibrary::out_byte)(__Port port,__U8 value) = out_byte_ASM_FN;
__U8	(* KernelLibrary::in_byte)(__Port port) = in_byte_ASM_FN;
void	(* KernelLibrary::enable_irq)(int irq) = enable_irq_ASM_FN;
void	(* KernelLibrary::disable_irq)(int irq) = disable_irq_ASM_FN;
void *	(* KernelLibrary::memcpy)(void* p_dst, void* p_src, int size) = memcpy_ASM_FN;
void	(* KernelLibrary::memset)(void* p_dst, char ch, int size) = memset_ASM_FN;
void	(* KernelLibrary::enable_int)() = enable_int_ASM_FN;
void	(* KernelLibrary::disable_int)() = disable_int_ASM_FN;
//C++
void KernelLibrary::disp_int(int value)
{
	char output[16];
	UserLibrary::itoa(output,value);
	disp_str(output);
}
void KernelLibrary::delay(int time)
{
	int t = SystemCall::get_ticks();
	while((SystemCall::get_ticks() - t) * 1000 / GlobalConstant::HZ < time){}
}
void KernelLibrary::disp_str_DONE(__Bool done)
{
	const int DONE_COLOR = GlobalConstant::MakeColor(GlobalConstant::GREEN,GlobalConstant::BLACK);
	const int FAILED_COLOR = GlobalConstant::MakeColor(GlobalConstant::RED,GlobalConstant::BLACK);
	disp_str("[");
	if(done)
		disp_color_str(" Done ",DONE_COLOR);
	else
		disp_color_str("Failed",FAILED_COLOR);
	disp_str("]\n");
}
//向下滚屏
void KernelLibrary::scroll_down()
{
	while(GlobalVariables::cursor_position / 2 >= video_current_start_addr + 80 * 25){
		video_current_start_addr += 80;	//不做边界检查
	}
	disable_int();
	out_byte(GlobalConstant::CRTC_ADDR_REG,
		GlobalConstant::CRTC_DATA_IDX_START_ADDR_H);
	out_byte(GlobalConstant::CRTC_DATA_REG,
		(video_current_start_addr >> 8) & 0xFF);
	out_byte(GlobalConstant::CRTC_ADDR_REG,
		GlobalConstant::CRTC_DATA_IDX_START_ADDR_L);
	out_byte(GlobalConstant::CRTC_DATA_REG,
		video_current_start_addr & 0xFF);
	enable_int();
}
//system calls' implementation
int KernelLibrary::sys_get_ticks(){
	return GlobalVariables::ticks;
}
int KernelLibrary::sys_write(const char * buf, int len, Process * p_proc)
{
	GlobalVariables::tty_table[p_proc->nr_tty]
	//	.console.out_char(buf[0]);
		.writeConsole(buf,len);
	return 0;
}

//struct SystemCall
const __PF_IntHandler SystemCall::sys_call = sys_call_ASM_FN;
//system calls' interface
int	(* SystemCall::get_ticks)() = get_ticks_ASM_FN;
void (* SystemCall::write)(const char * buf, int len) = write_ASM_FN;
