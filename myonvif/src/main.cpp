#include <stdio.h>
#include <stdlib.h>

#include "OnvifSoap.h"
#include "EchoServer.h"


#define SOAP_SOCK_TIMEOUT    (10)                                               // socket超时时间（单位秒）

#define USERNAME    "admin"
#define PASSWORD    "hidoo123"

void ptzCrtlProcesser(EchoServer* s ,OnvifSoap* soap, const boost::property_tree::ptree& pt){
    std::stringstream ss;
    boost::property_tree::write_json(ss, pt);
    std::cout<<boost::posix_time::microsec_clock::local_time()<<"---"<<ss.str()<<std::endl;

    std::string ip = pt.get<std::string>("ip");
    std::string userName = pt.get<std::string>("userName");
    std::string pwd = pt.get<std::string>("pwd");

    int controlType = pt.get<int>("controlType");
    auto t0 = boost::posix_time::microsec_clock::local_time();
    soap->continuousMove(ip.c_str(),userName.c_str(),pwd.c_str(),controlType);
    auto t1 = boost::posix_time::microsec_clock::local_time();
    std::cout<<"take times:"<<t1-t0<<std::endl;

    s->sendResponse(pt);
}
int main()
{
    OnvifSoap soap(SOAP_SOCK_TIMEOUT);

    boost::asio::io_context io_context;
    EchoServer s(io_context, 10250);
    s.registerProcesser("ptz-crtl",std::bind(ptzCrtlProcesser,&s,&soap,std::placeholders::_1));

    io_context.run();

    return 0;
}
