/***********************************************************************
 * Copyright � 2003 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.     *
 **********************************************************************/

#if !defined(endpointimpl_h)
#define endpointimpl_h 1

#include <string>
#include "mrnet/h/MRNet.h"

namespace MRN
{

class NetworkNode;
class EndPointImpl: public EndPoint{
 private:
    unsigned int id;
    std::string hostname;
    unsigned short port;

 public:
    EndPointImpl(int _id, const char * _hostname, unsigned short _port);
    virtual ~EndPointImpl();
    virtual const char * get_HostName()const;
    virtual unsigned short get_Port()const;
    virtual unsigned int get_Id()const;
    virtual bool compare(const char * _hostname, unsigned short _port)const;
};

inline const char * EndPointImpl::get_HostName() const
{
    return hostname.c_str();
}

inline unsigned short EndPointImpl::get_Port() const
{
    return port;
}

inline unsigned int EndPointImpl::get_Id() const
{
    return id;
}

inline bool EndPointImpl::compare(const char * _hostnamestr,
                                  unsigned short _port) const
{
    if( ( port == _port ) &&
        ( hostname == _hostnamestr ) ){
        return true;
    }
    else{
        return false;
    }
}

} // namespace MRN

#endif /* endpoint_h 1 */
