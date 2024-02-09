#ifndef NEW_MAIN_MESSAGES_
#define NEW_MAIN_MESSAGES_
#include "paratreet.decl.h"

class depthMsg : public CMessage_depthMsg {
public:
	int depth;
	depthMsg(int d) :depth(d) {}
};

template<typename configuration_type>
class configurationMsg :public CMessage_configurationMsg<configuration_type> {
public:
    configuration_type conf;
    configurationMsg(configuration_type c) : conf(c) {}
    
};




#endif
