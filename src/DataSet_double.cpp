// DataSet_double
#include "DataSet_double.h"
#include "MpiRoutines.h"

// DataSet_double::Allocate()
/** Reserve space in the Data and Frames arrays. */
int DataSet_double::Allocate( SizeArray const& sizeIn ) {
  if (!sizeIn.empty())
    Data_.reserve( sizeIn[0] );
  return 0;
}

// DataSet_double::Add()
/** Insert data vIn at frame. */
void DataSet_double::Add(size_t frame, const void* vIn) {
  if (frame > Data_.size())
    Data_.resize( frame, 0.0 );
  // Always insert at the end
  // NOTE: No check for duplicate frame values.
  Data_.push_back( *((double*)vIn) );
}

// DataSet_double::WriteBuffer()
/** Write data at frame to CharBuffer. If no data for frame write 0.0.
  */
void DataSet_double::WriteBuffer(CpptrajFile &cbuffer, SizeArray const& frame) const {
  if (frame[0] >= Data_.size())
    cbuffer.Printf(format_.fmt(), 0.0);
  else
    cbuffer.Printf(format_.fmt(), Data_[frame[0]]);
}

// DataSet_double::Append()
int DataSet_double::Append(DataSet* dsIn) {
  if (dsIn->Empty()) return 0;
  if (dsIn->Group() != SCALAR_1D) return 1;
  if (dsIn->Type() == DOUBLE) {
    size_t oldsize = Size();
    std::vector<double> const& dataIn = ((DataSet_double*)dsIn)->Data_;
    Data_.resize( oldsize + dataIn.size() );
    std::copy( dataIn.begin(), dataIn.end(), Data_.begin() + oldsize );
  } else {
    DataSet_1D const& ds = static_cast<DataSet_1D const&>( *dsIn );
    for (unsigned int i = 0; i != ds.Size(); i++)
      Data_.push_back( ds.Dval(i) );
  }
  return 0;
}

// DataSet_double::Sync()
/** First, non-master threads convert their vectors into C-arrays.
  * These arrays are then sent to the master, where they are put 
  * into the master arrays. It is assumed that master (rank 0) has 
  * first chunk of data, rank 1 has next and so on.
  */
int DataSet_double::Sync() {
  unsigned int dataSize;
  unsigned int masterSize = 0;
  double* values = 0;

  if (worldsize==1) return 0;

  for ( int rank = 1; rank < worldsize; ++rank) {
    if ( worldrank == rank ) {
      // ----- RANK -------
      // Get size of data on rank.
      dataSize = Data_.size();
      // Send rank size to master
      parallel_sendMaster(&dataSize, 1, rank, PARA_INT);
      // If size is 0 on rank, skip this rank.
      if (dataSize == 0) continue;
      // Allocate space for temp array on rank, put Data_ into values.
      values = new double[ dataSize ];
      std::copy(Data_.begin(), Data_.end(), values);
      //frames = new int[ dataSize ];
      // Send arrays to master
      //parallel_sendMaster(frames, dataSize, rank, PARA_INT);
      parallel_sendMaster(values, dataSize, rank, PARA_DOUBLE);
      // Free arrays on rank
      delete[] values;
    } else if (worldrank == 0) {
      // ----- MASTER -----
      // Master receives size from rank
      parallel_sendMaster(&dataSize, 1, rank, PARA_INT);
      // If size was 0 on rank, skip rank.
      if (dataSize == 0) continue;
      // Reallocate temp array on master if necessary
      if (dataSize > masterSize) {
        if ( values != 0 ) delete[] values;
        values = new double[ dataSize ];
        masterSize = dataSize;
      }
      // Master receives arrays
      //parallel_sendMaster(frames, dataSize, rank, PARA_INT);
      parallel_sendMaster(values, dataSize, rank, PARA_DOUBLE);
      // Insert frames and values to master arrays
      for (unsigned int i = 0; i < dataSize; ++i) {
        //Frames_.push_back( frames[i] );
        Data_.push_back( values[i] );
      }
    }
  } // End loop over ranks > 0

  // Free master array
  if (worldrank == 0 && values != 0 ) delete[] values;

  return 0;
}
