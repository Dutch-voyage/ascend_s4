#ifndef OP_PROTO_H_
#define OP_PROTO_H_

#include "graph/operator_reg.h"
#include "register/op_impl_registry.h"

namespace ge {

REG_OP(EyeCustom)
    .INPUT(y, ge::TensorType::ALL())
    .OUTPUT(y, ge::TensorType::ALL())
    .REQUIRED_ATTR(num_rows, Int)
    .ATTR(num_columns, Int, 0)
    .ATTR(batch_shape, ListInt, {})
    .ATTR(dtype, Int, 0)
    .OP_END_FACTORY_REG(EyeCustom);

}

#endif
