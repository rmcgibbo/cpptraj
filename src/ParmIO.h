#ifndef INC_PARMIO_H
#define INC_PARMIO_H
#include "ArgList.h"
#include "Topology.h"
#include "CpptrajFile.h"
#include "BaseIOtype.h"
/// Abstract base class that all ParmIO objects inherit from
class ParmIO : public BaseIOtype {
  public:
    virtual ~ParmIO() { }
    virtual bool ID_ParmFormat(CpptrajFile&) = 0;
    virtual int processReadArgs(ArgList&) = 0; 
    virtual int ReadParm(FileName const&, Topology&) = 0;
    virtual int processWriteArgs(ArgList&) = 0;
    virtual int WriteParm(FileName const&, Topology const&) = 0;
    virtual void SetDebug(int) = 0;
    virtual bool NeedsBondSearch() const = 0;
};
#endif
