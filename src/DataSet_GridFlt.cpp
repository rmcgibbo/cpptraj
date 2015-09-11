#include "DataSet_GridFlt.h"

DataSet_GridFlt::DataSet_GridFlt(DataSet_GridFlt const& rhs) : DataSet_3D(rhs), grid_(rhs.grid_) {}

void DataSet_GridFlt::WriteBuffer(CpptrajFile& outfile, SizeArray const& pIn) const {
  size_t x = pIn[0];
  size_t y = pIn[1];
  size_t z = pIn[2];
  if ( x >= grid_.NX() || y >= grid_.NY() || z >= grid_.NZ() )
    outfile.Printf(format_.fmt(), 0.0);
  else
    outfile.Printf(format_.fmt(), grid_.element(x,y,z));
}
