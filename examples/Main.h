#ifndef EXAMPLE_MAIN_H
#define EXAMPLE_MAIN_H

#include "CentroidData.h"
#include "Main.decl.h"
#include "NewMain.h"

class ExMain: public paratreet::Main<CentroidData> {

  public: 
  ExMain(CProxy_NewMain nm) : Main(nm) {}

  virtual Real getTimestep(BoundingBox&, Real) override;
  virtual void preTraversalFn(ProxyPack<CentroidData>) override;
  virtual void traversalFn(BoundingBox, ProxyPack<CentroidData>, int) override;
  virtual void postIterationFn(BoundingBox, ProxyPack<CentroidData>, int) override;
  virtual void setDefaults(void) override;
  virtual void main(CkArgMsg*) override;
  virtual void run(void) override;
};

#endif
