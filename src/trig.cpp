#include <cstdint>

const int32_t sinTableMax = 65535;
const int32_t sinTable[] = {
    // Haskell for generating this table:
    // map (\x -> floor $ (2**16 -1) * sin(x * 1/90 * 1/2*pi)) [0..90]
    0, 1143, 2287, 3429, 4571, 5711, 6850, 7986,
    9120, 10251, 11380, 12504, 13625, 14742, 15854, 16961,
    18063, 19160, 20251, 21336, 22414, 23485, 24549, 25606,
    26655, 27696, 28728, 29752, 30766, 31771, 32767, 33753,
    34728, 35692, 36646, 37589, 38520, 39439, 40347, 41242,
    42125, 42994, 43851, 44694, 45524, 46340, 47141, 47929,
    48701, 49459, 50202, 50930, 51642, 52338, 53018, 53683,
    54330, 54962, 55576, 56174, 56754, 57318, 57863, 58392,
    58902, 59394, 59869, 60325, 60762, 61182, 61582, 61964,
    62327, 62671, 62996, 63301, 63588, 63855, 64102, 64330,
    64539, 64728, 64897, 65046, 65175, 65285, 65375, 65445,
    65495, 65525, 65535

};

int32_t fastSin(int32_t degrees)
{
  if (degrees < 0)
  {
    // negative degrees? no problem :)
    return fastSin(360 + (degrees % 360));
  }
  else if (degrees < 90)
  {
    // first quadrant
    return sinTable[degrees];
  }
  else if (degrees < 180)
  {
    // second quadrant
    return sinTableMax - sinTable[degrees - 90];
  }
  else if (degrees < 270)
  {
    // third quadrant
    return -sinTable[degrees - 180];
  }
  else if (degrees < 360)
  {
    // fourth quadrant
    return -(sinTableMax - sinTable[degrees - 270]);
  }
  else if (degrees >= 360)
  {
    // more than a single turn? no problem :)
    return fastSin(degrees % 360);
  }
  else
  {
    return 0;
  }
}

int32_t fastCos(int32_t degrees)
{
  return fastSin(90 - degrees);
}
