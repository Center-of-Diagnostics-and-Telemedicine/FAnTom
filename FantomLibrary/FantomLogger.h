#ifndef FantomLogger_h__
#define FantomLogger_h__

/*!
	\file
	\date 2020/04/29 23:32
	\author kulberg

	\brief  
*/

XRAD_BEGIN



class fantom_logger
{
	static	size_t	counter;
	const	size_t	m_id;
	bool	m_finished_ok;
	const string	m_function_name;
public:

	fantom_logger(const string &in_function_name);

	void	finish()
	{
		m_finished_ok = true;
	}

	~fantom_logger();
protected:
private:
};


#define	START_LOG fantom_logger	function_logger(__func__)
#define	END_LOG function_logger.finish()


XRAD_END

#endif // FantomLogger_h__
