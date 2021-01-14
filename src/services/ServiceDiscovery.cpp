#include "soapRemoteDiscoveryBindingService.h"
#include "soapwsddService.h"
#include "wsddapi.h"
#include "macros.h"
#include "arguments.h"


int RemoteDiscoveryBindingService::Hello(const struct wsdd__HelloType& tdn__Hello, struct wsdd__ResolveType &tdn__HelloResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int RemoteDiscoveryBindingService::Bye(const struct wsdd__ByeType& tdn__Bye, struct wsdd__ResolveType &tdn__ByeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int RemoteDiscoveryBindingService::Probe(const struct wsdd__ProbeType& tdn__Probe, struct wsdd__ProbeMatchesType &tdn__ProbeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int wsddService::Hello(struct wsdd__HelloType *hello)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return __wsdd__Hello(this->soap, hello);
}


int wsddService::Bye(struct wsdd__ByeType *bye)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return __wsdd__Bye(this->soap, bye);
}


int wsddService::Probe(struct wsdd__ProbeType *probe)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return __wsdd__Probe(this->soap, probe);
}


int wsddService::ProbeMatches(struct wsdd__ProbeMatchesType *matches)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return __wsdd__ProbeMatches(this->soap, matches);
}


int wsddService::Resolve(struct wsdd__ResolveType *resolve)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return __wsdd__Resolve(this->soap, resolve);
}


int wsddService::ResolveMatches(struct wsdd__ResolveMatchesType *matches)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return __wsdd__ResolveMatches(this->soap, matches);
}


////////////
// Events //
////////////


void wsdd_event_ProbeMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ProbeMatchesType *matches)
{
  Context *context = (Context *)soap->user;
  DEBUG_FUNCTION(context);
}



void wsdd_event_ResolveMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ResolveMatchType *match)
{
  Context *context = (Context *)soap->user;
  DEBUG_FUNCTION(context);
}



void wsdd_event_Hello(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int MetadataVersion)
{
  Context *context = (Context *)soap->user;
  DEBUG_FUNCTION(context);
}



void wsdd_event_Bye(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int *MetadataVersion)
{
  Context *context = (Context *)soap->user;
  DEBUG_FUNCTION(context);
}



soap_wsdd_mode wsdd_event_Resolve(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *EndpointReference, struct wsdd__ResolveMatchType *match)
{
  Context *context = (Context *)soap->user;
  DEBUG_FUNCTION(context);
  /*
  if (EndpointReference && (strcmp(EndpointReference, _endpoint) == 0) )
  {
    soap_wsdd_ResolveMatches(soap, NULL, soap_wsa_rand_uuid(soap), MessageID, ReplyTo, _endpoint, _type, _scope, NULL, _xaddr, _metadataVersion);
  }
  */
  return SOAP_WSDD_ADHOC;
}



soap_wsdd_mode wsdd_event_Probe(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *Types, const char *Scopes, const char *MatchBy, struct wsdd__ProbeMatchesType *matches)
{
  Context *context = (Context *)soap->user;
  DEBUG_FUNCTION(context);

  soap_wsdd_init_ProbeMatches(soap, matches);

  std::string xaddr = context->ws_context->get_xaddr();
  std::string scopes = context->ws_context->get_scopes();
  std::string endpoint = context->ws_context->endpoint;

  soap_wsdd_add_ProbeMatch(soap,
                           matches,
                           endpoint.c_str(),
                           "tdn:NetworkVideoTransmitter",
                           scopes.c_str(),
                           NULL,
                           xaddr.c_str(),
                           0);

  soap_wsdd_ProbeMatches(soap,
                         NULL,
                         soap_wsa_rand_uuid(soap),
                         MessageID,
                         ReplyTo,
                         matches);

  return SOAP_WSDD_ADHOC;
}
