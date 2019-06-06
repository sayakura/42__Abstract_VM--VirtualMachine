
#ifndef KSTACK_HPP
# define KSTACK_HPP
# include <iostream>
# include <stack>
# include <deque>

template<typename T>
class KStack: public std::stack<T> {
public:
    typedef typename std::deque<T>::iterator iterator;
	KStack(void) {}
	KStack(KStack const & m) {*this = m;}
	~KStack(void) {}
	iterator begin(void) {return std::begin(this->c);}
	iterator end(void) {return std::end(this->c);}
};

#endif