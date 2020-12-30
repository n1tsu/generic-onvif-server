#include "wsdd.h"


extern ServiceContext service_ctx;
static struct soap *soap_discover;


const char* get_rand_endpoint(struct soap *soap_srv)
{
    static char tmp[48];      //UUID "urn:uuid:xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx"

    const char *random_UUID = soap_wsa_rand_uuid(soap_srv);

    if(!random_UUID)
        return NULL;

    snprintf(tmp, sizeof(tmp), "%s", random_UUID);

    return tmp;
}


void wsdd_routine()
{
    soap_discover = soap_new1(SOAP_IO_UDP);

    if(!soap_discover)
        daemon_error_exit("Can't get mem for discover SOAP\n");

    in_addr_t addr                     = inet_addr("239.255.255.250");
    soap_discover->ipv4_multicast_if   = (char *)&addr;
    soap_discover->ipv6_multicast_if   = addr;
    soap_discover->ipv4_multicast_ttl  = 1;
    soap_discover->connect_flags       = SO_BROADCAST;
    soap_discover->bind_flags          = SO_REUSEADDR;

    if(!soap_valid_socket(soap_bind(soap_discover, NULL, 3702, 100)))
    {
        soap_stream_fault(soap_discover, std::cerr);
        exit(EXIT_FAILURE);
    }

    service_ctx.endpoint = get_rand_endpoint(soap_discover);
    soap_discover->user = (void*)&service_ctx;

    // Join multicast group
    struct ip_mreq mcast;
    mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    if(get_addr_of_if(service_ctx.eth_ifs.back().dev_name(), AF_INET, &mcast.imr_interface) != 0)
        daemon_error_exit("Cant get addr for interface error: %m\n");

    setsockopt(soap_discover->master, IPPROTO_IP, IP_MULTICAST_IF, &mcast.imr_interface.s_addr, sizeof(struct in_addr));

    if(setsockopt(soap_discover->master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mcast, sizeof(mcast)) != 0 )
        daemon_error_exit("Cant adding multicast group error: %m\n");

    std::string xaddr = service_ctx.getXAddr(soap_discover);
    std::string scope = service_ctx.get_scopes();

    soap_wsdd_Hello(soap_discover,
                    SOAP_WSDD_ADHOC,                                 // or SOAP_WSDD_ADHOC for ad-hoc mode
                    "soap.udp://239.255.255.250:3702",               // "http(s):" URL, or "soap.udp:" UDP, or TCP/IP address
                    soap_wsa_rand_uuid(soap_discover),               // a unique message ID
                    NULL,
                    service_ctx.endpoint.c_str(),
                    "tdn:NetworkVideoTransmitter",                   // Types: I'm a TS
                    scope.c_str(),
                    NULL,                                            // MatchBy
                    xaddr.c_str(),
                    0);                                              // MDVersion

    while (true)
    {
        if (soap_wsdd_listen(soap_discover, 0))
            soap_print_fault(soap_discover, stderr);
    }
}
