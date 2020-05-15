#ifndef RequestMapper_h__
#define RequestMapper_h__

/*!
 * \file RequestMapper.h
 * \date 2017/10/04 16:05
 *
 * \author kulberg
 *
 * \brief
 *
 * TODO: long description
 *
 * \note
*/

#include "QtWebApp/httpserver/httpconnectionhandler.h"
#include "QtWebApp/httpserver/httplistener.h"

using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler
{
	Q_OBJECT
		Q_DISABLE_COPY(RequestMapper)

//		int port;
public:

    /**
      Constructor.
      @param parent Parent object
    */
    RequestMapper(QObject* parent=0);

    /**
      Destructor.
    */
    ~RequestMapper();

    /**
      Dispatch incoming HTTP requests to different controllers depending on the URL.
      @param request The received HTTP request
      @param response Must be used to return the response
    */
    void service(HttpRequest& request, HttpResponse& response);

	void LoadFantom();

private:
	std::mutex m_RequestMapperMutex;
	

	bool isLoaded;

};

#endif // RequestMapper_h__
