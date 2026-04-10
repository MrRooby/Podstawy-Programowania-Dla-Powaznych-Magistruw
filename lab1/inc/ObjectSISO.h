#include <vector>

class ObjectSISO
{
public:
  virtual ~ObjectSISO() = default;

  virtual double simulate(const double u) = 0;
};
