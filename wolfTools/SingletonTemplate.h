/********************************************************************
            Copyright (c) 2013, �������� �з�����
                   All rights reserved
         
    �������ڣ�  2009��12��2�� 17ʱ04��
    �ļ����ƣ�  SingletonTemplate.h
    ˵    ����  ����ģʽ�ķ���ʵ�֡�����Ԫ�ṩ�������͵����ࣺ

				�ֱ�ʵ�������������ϵĵ����� ���У�

				1. ��CisSingleInstance<T>��T��is a�Ĺ�ϵ��Ӧ�ô�ģʽ��˵��T���ǿɼ̳еģ�����T�������������������
				   1.1 Ĭ�Ϲ��캯��������������public��protected�ķ��ʼ���
				   1.2 ��������������virtual��

				2. ��ChaSingleInstance<T>��T��has a�Ĺ�ϵ����������ϵ��Ӧ�ô�ģʽ��˵��T��ChaSingleInstance<T>��
				   ��һ����̬��Ա�������ͣ�����T�������������������
				   2.1 Ĭ�Ϲ��캯��������������public�ķ��ʼ���

				���飺1. ���ȿ���ʹ��ChaSingleInstance<T>
					  2. typedef ��ʹ������߿ɶ��ԣ�
					  �磺typedef ChaSingleInstance<T> TSingleInstance;
    
    ��ǰ�汾��  1.00
    ��    �ߣ�  Conserlin
    ��    ����  ����  

*********************************************************************/
#pragma once

template <typename T>
class CiaSingleton : public T
{
public:
	typedef T SingletonType;

public:
	// ��ȡ����
	static CiaSingleton* singleton();
	// �ͷŵ���
	static void release();

private:
	// ����ʵ��ָ��
	static CiaSingleton* pSingleInstance;

private:
	// ���಻�ɼ̳У�����ʵ����
	CiaSingleton(){}
	~CiaSingleton(){}
};

template <typename T>
class ChaSingleton
{
public:
	typedef T SingletonType;

public:
	typedef T SingleInstanceType;

	// ��ȡ����
	static SingleInstanceType* singleton();
	// �ͷŵ���
	static void release();

private:
	// ����ʵ��ָ��
	static SingleInstanceType* pSingleInstance;

private:
	// ���಻�ɼ̳У�����ʵ����
	ChaSingleton() {}
	~ChaSingleton() {}	
};

// ��CiaSingleInstance�Ķ���

template <typename T>
CiaSingleton<T>* CiaSingleton<T>::pSingleInstance = nullptr;

template <typename T>
CiaSingleton<T>* CiaSingleton<T>::singleton() {
	if (nullptr == pSingleInstance) {
		pSingleInstance = new CiaSingleton<T>();
	}

	return pSingleInstance;
}

template <typename T>
void CiaSingleton<T>::release() {
	delete pSingleInstance;
	pSingleInstance = nullptr;
}


// ��ChaSingleInstance�Ķ���
template <typename T>
class ChaSingletonPtr { //ʵ���˳�ʱ�Զ��ͷ�
private:
    typename T::SingleInstanceType *m_ptr;

public:
    ChaSingletonPtr() : m_ptr(nullptr) {}
    ~ChaSingletonPtr() {/*T::release();*/}
    void setPtr(typename T::SingleInstanceType* ptr) {m_ptr = ptr;}
};

template <typename T>
typename ChaSingleton<T>::SingleInstanceType* ChaSingleton<T>::pSingleInstance = nullptr;

template <typename T>
typename ChaSingleton<T>::SingleInstanceType* ChaSingleton<T>::singleton() {
	if (nullptr == pSingleInstance) {
		pSingleInstance = new SingleInstanceType();
        static ChaSingletonPtr<ChaSingleton<T> > ptr;
        ptr.setPtr(pSingleInstance);
	}

	return pSingleInstance;
}

template <typename T>
void ChaSingleton<T>::release() {
	delete pSingleInstance;
	pSingleInstance = nullptr;
}