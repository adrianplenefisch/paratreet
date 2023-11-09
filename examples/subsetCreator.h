#ifndef PARATREET_SUBSETCREATOR_H
#define PARATREET_SUBSETCREATOR_H

#include "subsetVisitor.h"
#include "FoF.decl.h"


class subsetCreator /*: public CBase_subsetCreator*/
{

public:

    std::vector<std::vector<Particle>> toSend;
    std::vector<int> group_n_lookup;

    subsetCreator()
    {
        
    }

    void createSubsets(CProxy_Partition<CentroidData> partitionProxy,int n_particles)
    {
        
        Particle* particles;
        CkReductionMsg* mymsg;
        partitionProxy.copyParticlesCb(0,CkCallbackResumeThread((void*&)mymsg));
        particles = ((Particle*)mymsg->getData());
        for(int i = 0; i < n_particles; ++i)
        {
            if(particles[i].group_number==0)
            {
                continue;
            }
            auto it = std::find(group_n_lookup.begin(), group_n_lookup.end(), particles[i].group_number);
            if (it == group_n_lookup.end())
            {
                group_n_lookup.push_back(particles[i].group_number);
                std::vector<Particle> p;
                p.push_back(particles[i]);
                toSend.push_back(p);
            } 
            else
            {
                auto group_n_index = std::distance(group_n_lookup.begin(), it);
                toSend[group_n_index].push_back(particles[i]);
            }
        }
    }

    static bool compare_size(std::vector<Particle> v1, std::vector<Particle> v2)
    {
        return v1.size()<v2.size();
    }

    void runSubsets(CkArgMsg* m)
    {
        std::sort(toSend.begin(),toSend.end(),compare_size);
        for(int i = 0; i<5;++i)
        {
            StartMessage* mm = new StartMessage(m->argc, m->argv,true, toSend[i]);
            CProxy_NewMain firstmain = CProxy_NewMain::ckNew(mm,1);
            
        }
    }

};

#endif
