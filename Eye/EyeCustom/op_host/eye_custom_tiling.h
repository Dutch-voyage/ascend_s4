
#include "register/tilingdata_base.h"

namespace optiling {
BEGIN_TILING_DATA_DEF(EyeCustomTilingData)
  TILING_DATA_FIELD_DEF(uint32_t, dtype);
  TILING_DATA_FIELD_DEF(uint32_t, batchNum);
  TILING_DATA_FIELD_DEF(uint32_t, batchSize);
  TILING_DATA_FIELD_DEF(uint32_t, totalSize);
  TILING_DATA_FIELD_DEF(uint32_t, outSize);
  TILING_DATA_FIELD_DEF(uint32_t, inSize);
  TILING_DATA_FIELD_DEF(uint32_t, outStride);
  TILING_DATA_FIELD_DEF(uint32_t, inStride);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(EyeCustom, EyeCustomTilingData)
}
