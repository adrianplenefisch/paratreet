#ifndef NEW_MAIN_MESSAGES_
#define NEW_MAIN_MESSAGES_
#include "paratreet.decl.h"

class depthMsg : public CMessage_depthMsg {
public:
	int depth;
	depthMsg(int d) :depth(d) {}
};



class timestepMsg : public CMessage_timestepMsg {
public:
	Real time_step;
	timestepMsg(Real ts) :time_step(ts) {}
};

template<typename configuration_type>
class configurationMsg :public CMessage_configurationMsg<configuration_type> {
public:
    configuration_type conf;
    configurationMsg(configuration_type c) : conf(c) {}
    
};




#endif
