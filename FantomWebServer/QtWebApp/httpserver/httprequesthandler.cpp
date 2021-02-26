/*
  Copyright (c) 2021, Moscow Center for Diagnostics & Telemedicine

  This is a modified version of the QtWebApp software.
  The original license terms (GNU LGPLv3) are effective. See copyright.txt.
*/
#include "pre.h"
/**
  @file
  @author Stefan Frings
*/

#include "httprequesthandler.h"

using namespace stefanfrings;

HttpRequestHandler::HttpRequestHandler(QObject* parent)
    : QObject(parent)
{}

HttpRequestHandler::~HttpRequestHandler()
{}

void HttpRequestHandler::service(HttpRequest& request, HttpResponse& response)
{
    qCritical("HttpRequestHandler: you need to override the service() function");
    qDebug("HttpRequestHandler: request=%s %s %s",request.getMethod().data(),request.getPath().data(),request.getVersion().data());
    response.setStatus(501,"not implemented");
    response.write("501 not implemented",true);
}
