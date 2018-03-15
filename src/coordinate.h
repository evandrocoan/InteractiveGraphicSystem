#ifndef GTKMM_APP_COORDINATE
#define GTKMM_APP_COORDINATE

class Coordinate
{
public:
  Coordinate(long int x, long int y, long int z = 1);
  ~Coordinate();

  void apply(Transformation);

  long int getx();
  long int gety();
  long int getz();

protected:
  long int coordinates[3];
};
#endif // GTKMM_APP_Coordinate
