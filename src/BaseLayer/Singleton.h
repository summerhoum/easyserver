#ifndef __SINGLETON_H_
#define __SINGLETON_H_


template <typename T>
class CSingleton
{
public:
	static T& Inst()
	{
		static T s_instance;
		return s_instance;
	}

protected:
	CSingleton() { }
	~CSingleton() { }

private:
	CSingleton(const CSingleton&);
	CSingleton& operator =(const CSingleton&);
};  

#endif