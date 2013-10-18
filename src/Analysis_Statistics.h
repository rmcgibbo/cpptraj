#ifndef INC_ANALYSIS_STATISTICS_H
#define INC_ANALYSIS_STATISTICS_H
#include "Analysis.h"
#include "Array1D.h"
class Analysis_Statistics : public Analysis {
  public:
    // The following 2 are also used in Analysis_Crankshaft
    static const char* torsion_ss[];
    static const double torsion_offset[];

    Analysis_Statistics();

    static DispatchObject* Alloc() { return (DispatchObject*)new Analysis_Statistics(); }
    static void Help();

    Analysis::RetType Setup(ArgList&,DataSetList*,TopologyList*,DataFileList*,int);
    Analysis::RetType Analyze();
  private:
    Array1D datasets_;
    std::string filename_;
    CpptrajFile outfile_;
    double shift_;
    int debug_;

    static const char* pucker_ss[];
    void PuckerAnalysis( DataSet_1D const&, int );
    void TorsionAnalysis( DataSet_1D const&, int );
    static const char* distance_ss[];
    void DistanceAnalysis( DataSet_1D const&, int );
};
#endif
