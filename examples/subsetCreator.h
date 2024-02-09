#ifndef PARATREET_SUBSETCREATOR_H
#define PARATREET_SUBSETCREATOR_H

#include "subsetVisitor.h"
#include "ParticleMsg.h"
#include "FoF.decl.h"
#include "TipsyFile.h"


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
        return v1.size()>v2.size();
    }

    void runSubsets(CkArgMsg* m, std::string input_file, BoundingBox universe, CProxy_Driver<CentroidData> driver)
    {
        std::sort(toSend.begin(),toSend.end(),compare_size);
        for(int i = 0; i<1;++i)
        {
            //create new universe
            BoundingBox small_universe = *(new BoundingBox());
            small_universe.n_particles = toSend[i].size();
            small_universe.n_dark = toSend[i].size();



            //write tipsy file for specific subset
            write(small_universe, input_file+("_i")+std::to_string(i),toSend[i]);


            //create arrays for new subset
            CProxy_TreeCanopy<CentroidData> canopy = CProxy_TreeCanopy<CentroidData>::ckNew();
            canopy.doneInserting();
            CProxy_Partition<CentroidData> partitions = CProxy_Partition<CentroidData>::ckNew();
            partitions.doneInserting();
            CProxy_Subtree<CentroidData> subtrees = CProxy_Subtree<CentroidData>::ckNew();
            subtrees.doneInserting();

        

            //Create the input message for the new subset
            //char* argv;
            size_t argv_alloc_size=m->argc;
            for(int i =0; i<m->argc; ++i)
            {
                argv_alloc_size+=strlen(m->argv[i]);
            }
            //argv = (char*)malloc(sizeof(char)*argv_alloc_size);
            StartMessage* mm = new (toSend[i].size(), argv_alloc_size) StartMessage(m->argc,false,1,toSend[i].size());
            int it = 0;
            for(int i =0; i<m->argc;++i)
            {
                for(int j=0; j<strlen(m->argv[i]);++j)
                {
                    mm->argv[it] = m->argv[i][j];
                    ++it;
                }
                mm->argv[it]=' ';
                ++it;
            }
            for(int j = 0; j<toSend[i].size(); ++j)
            {
                mm->passed_particles[j] = toSend[i].data()[j];
            }
            



            //start new subset
            CProxy_NewMain firstmain = CProxy_NewMain::ckNew(mm);
            firstmain.passArrayProxies(canopy,subtrees, partitions);

            //free(argv);
            
        }
        //destroy previously used subtrees and partitions
        driver.destroyAll();
    }



    void write(BoundingBox box, std::string output_file, std::vector<Particle> particles)
    {
    Tipsy::header tipsyHeader;

    //tipsyHeader.time = time_;
    tipsyHeader.nbodies = box.n_particles;
    tipsyHeader.nsph = box.n_sph;
    tipsyHeader.ndark = box.n_dark;
    tipsyHeader.nstar = box.n_star;

    bool use_double = sizeof(Real) == 8;

    auto output_filename = output_file + ".tipsy";

    CmiFopen(output_filename.c_str(), "w");

    Tipsy::TipsyWriter w(output_filename, tipsyHeader, false, use_double, use_double);

    w.writeHeader();

    if(!w.seekParticleNum(0)) {
        CkPrintf("seeking %d particles for total %d gas %d dark %d star %d\n",
        0, box.n_particles, box.n_sph, box.n_dark, box.n_star);
        CkAbort("bad seek");
    }

    for (const auto& p : particles) {
        if (p.isGas()) {
        Tipsy::gas_particle_t<Real, Real> gp;
        gp.mass = p.mass;
        gp.pos = p.position;
        gp.vel = p.velocity; // dvFac = 1
        if(!w.putNextGasParticle_t(gp)) {
            CkError("[%d] Write gas failed, errno %d: %s\n", CkMyPe(), errno, strerror(errno));
            CkAbort("Bad Write");
        }
        }
        else if (p.isDark()) {
        Tipsy::dark_particle_t<Real, Real> dp;
        dp.mass = p.mass;
        dp.pos = p.position;
        dp.vel = p.velocity; // dvFac = 1
        if(!w.putNextDarkParticle_t(dp)) {
            CkError("[%d] Write dark failed, errno %d: %s\n", CkMyPe(), errno, strerror(errno));
            CkAbort("Bad Write");
        }
        }
        else if (p.isStar()) {
        Tipsy::star_particle_t<Real, Real> sp;
        sp.mass = p.mass;
        sp.pos = p.position;
        sp.vel = p.velocity; // dvFac = 1
        if(!w.putNextStarParticle_t(sp)) {
            CkError("[%d] Write star failed, errno %d: %s\n", CkMyPe(), errno, strerror(errno));
            CkAbort("Bad Write");
        }
        }

    }
    }



};




#endif
