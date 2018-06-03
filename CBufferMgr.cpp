// CBufferMgr.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include "CBufferMgr.h"


int main()
{
	std::cout << "hello world!" << std::endl;

	CBufferMgr*pBufMgr = CBufferMgr::instance();
	pBufMgr->init(4, 100);

	for (int i = 0; i < 100; ++i)
	{
		int * pInt = (int *)(pBufMgr->getBuffer());
		if (pInt)
		{
			*pInt = i;
			pBufMgr->s_freeBuffer(pInt);
			pInt = NULL;
		}
	}

	for (int j = 0; j < 100; ++j)
	{
		int * pInt = (int *)(pBufMgr->getBuffer());

		if (pInt)
		{
			std::cout << j << " : " << *pInt << std::endl;
		}
	}

	std::getchar();

    return 0;
}
