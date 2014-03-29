#ifndef _DOZLIXON_BUFFERTEMPLATE_H_
#define _DOZLIXON_BUFFERTEMPLATE_H_

template<typename __Type,int IN_SIZE>
class BufferTemplate
{
public:
	static const int SIZE = IN_SIZE;
	BufferTemplate():count_(0),p_head_(buf_),p_tail_(buf_){}
	int count() const{return count_;}
	void pushItem(__Type item){
		*(p_head_++) = item;
		if(p_head_ == buf_ + SIZE)
			p_head_ = buf_;
		if(++count_ > IN_SIZE)
			count_ = IN_SIZE;
	}
	__Type popItem(){
		if(!count_)
			return 0;
		__Type item = *(p_tail_++);
		if(p_tail_ == buf_ + SIZE)
			p_tail_ = buf_;
		--count_;
		return item;
	}
private:
	int			count_;			/* 缓冲区当前大小 */
	__Type		buf_[SIZE];		/* 缓冲区 */
	__Type *	p_head_;		/* 指向缓冲区中下一个空闲位置 */
	__Type *	p_tail_;		/* 指向任务应处理的位置 */
};

#endif
