#include "pre.h"
#include "FantomLogger.h"
/*!
	\file
	\date 2020/04/29 23:32
	\author kulberg

	\brief 
*/

XRAD_BEGIN

size_t	fantom_logger::counter = 0;


fantom_logger::fantom_logger(const string &in_function_name) : m_id(counter++), m_function_name(in_function_name)
{
	m_finished_ok = false;
	printf("\n----------'%s' (id=%zu) started", m_function_name.c_str(), m_id);
	fflush(stdout);
}

fantom_logger::~fantom_logger()
{
	if(m_finished_ok)
	{
		printf("\n----------'%s' (id=%zu) finished. OK", m_function_name.c_str(), m_id);
	}
	else
	{
		printf("\n----------'%s' (id=%zu) finished with errors", m_function_name.c_str(), m_id);
		ForceDebugBreak();
	}
	fflush(stdout);
}


XRAD_END

