#include "CoreFunctions.h"

#include "BoundingBox.h"
#include "ParticleMsg.h"
#include "Reader.h"
#include "ThreadStateHolder.h"
#include "Subtree.h"
#include "Partition.h"
#include "Configuration.h"

#include "ProxyPackSeparate.h"

#include "paratreet.decl.h"
#include "Paratreet.h"
#include "NewMain.h"
#include "NewMainMessages.h"


void NewMain::start(StartMessage* m)
{
    //auto& main = CsvAccess(main_);
    auto& main = main_;
    main_->subset_argc = m->argc;
    main_->subset_argv = (char**)malloc(sizeof(char*)*main_->subset_argc);
    char** to_parse = (char**)malloc(sizeof(char*)*main_->subset_argc);
    int it = 0;
    int itforl = 0;
    for(int i =0; i<main_->subset_argc; ++i)
    {
        int lengthofstring = 0;
        while(m->argv[itforl]!=' ')
        {
            ++lengthofstring;
            ++itforl;
        }
        main_->subset_argv[i] = (char*)malloc(sizeof(char)*(lengthofstring+1));
        to_parse[i] = (char*)malloc(sizeof(char)*(lengthofstring+1));
        int j = 0;
        while(m->argv[it]!=' ')
        {
            main_->subset_argv[i][j] = m->argv[it];
            to_parse[i][j] = m->argv[it];
            ++it;
            ++j;

        }
        main_->subset_argv[i][j] = '\0';
        to_parse[i][j] = '\0';
        ++itforl;
        ++it;
    }

    main->setDefaults();
    (main->configuration()).parse(m->argc, to_parse);
    CkArgMsg* mm = new CkArgMsg(); 
    mm->argc = m->argc;
    mm->argv = to_parse;

    if(m->d != 0)
    {
        main->configuration().linking_length /= 2;
        CkPrintf("LINKING LENGTH: %f\n",main->configuration().linking_length);
        main->configuration().output_file.append("."+to_string(group_index));
    }
    main->main(mm);

    

    if(m->d == 0)
    {
        
        pm = 0;
        n_particles = 0;
        CkCallback runCB(CkIndex_NewMain::run(), thisProxy);
        main->initializeDriver(runCB, m->passed_particles,m->n_particles);
        
    }
    else
    {
        pm = (Particle*)malloc(sizeof(Particle)*m->n_particles);
        //pm = m->passed_particles;
        n_particles = m->n_particles;
        for(int i=0; i<n_particles; ++i)
        {
            pm[i]=m->passed_particles[i];
        }
        
    }
    delete(m);
}

void NewMain::run() {
    main_->run();
}





void NewMain::setConfiguration(std::shared_ptr<paratreet::Configuration>&& cfg) {
    main_->setConfiguration(std::move(cfg));
}

void NewMain::getConfiguration(CkCallback cb)
{


    cb.send(new configurationMsg<paratreet::DefaultConfiguration>(*((paratreet::DefaultConfiguration*)(&(main_->configuration())))));
}

void NewMain::getDepth(CkCallback cb)
{


    cb.send(new depthMsg(depth));
}

void NewMain::getTimestep(BoundingBox box, Real real, CkCallback cb)
{
    Real time_step = main_->getTimestep(box, real);
    //this->contribute(sizeof(Real), &time_step, CkReduction::nop, cb);
    cb.send(new timestepMsg(time_step));
}


