#pragma once
#include <list>
#include <mutex>

class CBufferMgr
{
public:
	~CBufferMgr()
	{
		free(m_strBufferStart);
	}

	static CBufferMgr * instance()
	{
		static CBufferMgr * _instance = nullptr;
		if (nullptr == _instance)
		{
			_instance = new CBufferMgr();
		}
		return _instance;
	}

	bool init(int cellNum = 256, int num = 100)
	{
		MAX_BUFFER_NUM = num;
		BUFFER_SIZE = cellNum;
		m_strBufferStart = (char *)malloc(MAX_BUFFER_NUM * BUFFER_SIZE);
		if (NULL == m_strBufferStart)
		{
			return false;
		}

		char *strBuffer = m_strBufferStart;
		for (int i = 0; i < MAX_BUFFER_NUM; ++i)
		{
			m_idleBuffer.push_back(strBuffer);
			strBuffer += BUFFER_SIZE;
		}

		return true;
	}

	char *getBuffer()
	{
		std::lock_guard<std::mutex> lg(m_idleBufferMutex);

		char * strBuffer = NULL;

		if (m_idleBuffer.size() > 0)
		{
			strBuffer = *(m_idleBuffer.begin());
			m_idleBuffer.pop_front();
		}

		if (nullptr == strBuffer)
		{
			strBuffer = (char *)malloc(BUFFER_SIZE);
			if (nullptr == strBuffer)
			{
				return NULL;
			}
		}

		return strBuffer;

	}

	void freeBuffer(char *pBuffer)
	{
		std::lock_guard<std::mutex> lg(m_idleBufferMutex);
		if (pBuffer < m_strBufferStart || pBuffer > (m_strBufferStart + BUFFER_SIZE * MAX_BUFFER_NUM))
		{
			free(pBuffer);
		}
		else
		{
			m_idleBuffer.push_back(pBuffer);
		}
	}

	static void s_freeBuffer(void * pBuffer)
	{
		if (NULL == pBuffer)
		{
			return;
		}

		instance()->freeBuffer((char *)pBuffer);
	}

private:
	CBufferMgr()
	: m_strBufferStart(nullptr)
	, MAX_BUFFER_NUM(0)
	, BUFFER_SIZE(0)
	{
	}

public:
	//缓冲区单元数
	int MAX_BUFFER_NUM;
	
	//单元大小
	int BUFFER_SIZE;

private:
	//起始地址
	char * m_strBufferStart;

	//空闲列表
	std::list<char *> m_idleBuffer;
	std::mutex m_idleBufferMutex;
};
