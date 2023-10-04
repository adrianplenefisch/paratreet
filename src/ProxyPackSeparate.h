#ifndef PARATREET_PROXYPACKSEPARATE_H_
#define PARATREET_PROXYPACKSEPARATE_H_

#include "paratreet.decl.h"
#include "unionFindLib.decl.h"

template <typename Data>
struct ProxyPack {
  CProxy_Driver<Data> driver;
  CProxy_Subtree<Data> subtree;
  CProxy_Partition<Data> partition;
  CProxy_CacheManager<Data> cache;
  CProxy_UnionFindLib libProxy;
  CProxy_ThreadStateHolder thread_state_holder;
  
  ProxyPack(CProxy_Driver<Data> d, CProxy_Subtree<Data> s, CProxy_Partition<Data> p, CProxy_CacheManager<Data> c, CProxy_UnionFindLib u, CProxy_ThreadStateHolder t)
    : driver(d), subtree(s), partition(p), cache(c), libProxy(u), thread_state_holder(t) {}
  ProxyPack() {}
  void pup(PUP::er& p) {
    p | driver;
    p | subtree;
    p | partition;
    p | cache;
    p | libProxy;
    p | thread_state_holder;
  }
};

#endif
