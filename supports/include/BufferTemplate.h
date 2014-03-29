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
	int			count_;			/* ��������ǰ��С */
	__Type		buf_[SIZE];		/* ������ */
	__Type *	p_head_;		/* ָ�򻺳�������һ������λ�� */
	__Type *	p_tail_;		/* ָ������Ӧ�����λ�� */
};

#endif
