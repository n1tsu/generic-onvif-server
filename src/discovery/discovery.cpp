#include "discovery.h"
#include "arguments.h"


extern Context context;


const char* get_rand_endpoint(struct soap *soap_srv)
{
    static char tmp[48];      //UUID "urn:uuid:xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx"

    const char *random_UUID = soap_wsa_rand_uuid(soap_srv);

    if(!random_UUID)
        return NULL;

    snprintf(tmp, sizeof(tmp), "%s", random_UUID);

    return tmp;
}


void discovery_routine()
{
    struct soap *soap_discover = soap_new1(SOAP_IO_UDP);

    // Setup multicast socket
    in_addr_t addr                     = inet_addr("239.255.255.250");
    soap_discover->ipv4_multicast_if   = (char *)&addr;
    soap_discover->ipv6_multicast_if   = addr;
    soap_discover->ipv4_multicast_ttl  = 1;
    soap_discover->connect_flags       = SO_BROADCAST;
    soap_discover->bind_flags          = SO_REUSEADDR;

    if(!soap_valid_socket(soap_bind(soap_discover, NULL, 3702, 100)))
    {
      std::cerr << "Failed to bind multicast socket." << std::endl;
      exit(1);
    }

    // Create endpoint and save it to be used by ServiceDiscovery
    context.endpoint = get_rand_endpoint(soap_discover);
    soap_discover->user = &context;

    // Join multicast group
    struct ip_mreq mcast;
    mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    mcast.imr_interface.s_addr = INADDR_ANY;

    setsockopt(soap_discover->master, IPPROTO_IP, IP_MULTICAST_IF,
               &mcast.imr_interface.s_addr, sizeof(struct in_addr));

    if(setsockopt(soap_discover->master, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                  (char *)&mcast, sizeof(mcast)) != 0 )
    {
      std::cerr << "Failed to add multicast subscription." << std::endl;
      exit(1);
    }

    std::string xaddr = "http://" + context.xaddr + ":" + std::to_string(context.port);
    std::string scopes = context.get_scopes();
    const char *endpoint = context.endpoint.c_str();

    soap_wsdd_Hello(soap_discover,
                    SOAP_WSDD_ADHOC,                                 // or SOAP_WSDD_ADHOC for ad-hoc mode
                    "soap.udp://239.255.255.250:3702",               // "http(s):" URL, or "soap.udp:" UDP, or TCP/IP address
                    soap_wsa_rand_uuid(soap_discover),               // a unique message ID
                    NULL,
                    endpoint,
                    "tdn:NetworkVideoTransmitter",                   // Types: I'm a TS
                    scopes.c_str(),
                    NULL,                                            // MatchBy
                    xaddr.c_str(),
                    0);                                              // MDVersion

    while (true)
    {
      if (soap_wsdd_listen(soap_discover, 0))
        soap_print_fault(soap_discover, stderr);
    }
}
