#ifndef INC_MATRIX_2D_H
#define INC_MATRIX_2D_H
#include "DataSet.h"
#include "DataSet_2D.h"
#include "Matrix.h"
/// Double-precision two-dimensional matrix.
class Matrix_2D : public DataSet, DataSet_2D {
  public:
    Matrix_2D();
    double& operator[](size_t idx)             { return mat_[idx];          }
    // ----- DataSet functions -------------------
    size_t Size()                        const { return mat_.size();        }
    int Sync()                                 { return 1;                  }
    void Info()                          const { return;                    }
    // ----- DataSet_2D functions ----------------
    int Allocate2D(size_t x,size_t y)          { return mat_.resize(x,y);   }
    void Write2D(CpptrajFile&, int, int) const;
    double GetElement(size_t x,size_t y) const { return mat_.element(x,y);  }
    size_t Nrows()                       const { return mat_.Nrows();       }
    size_t Ncols()                       const { return mat_.Ncols();       }
    // -------------------------------------------
    int AddElement(double d)                   { return mat_.addElement(d); }
    void SetElement(size_t x,size_t y,double d){ mat_.setElement(x,y,d);    }
  private:
    Matrix<double> mat_;
};
#endif
