
#include "eye_custom_tiling.h"
#include "register/op_def_registry.h"
#include "tiling/platform/platform_ascendc.h"
#include "tiling/tiling_api.h"

#include <cstdio>
namespace optiling
{
    static ge::graphStatus TilingFunc(gert::TilingContext *context)
    {

        EyeCustomTilingData tiling;
        uint32_t sizeofdatatype;
        uint32_t totalLengthAligned;

        // // 1. 获取平台信息
        // uint64_t ub_size;
        auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
        // ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::UB, ub_size);

        // 2. 获取数据信息
        int32_t totalLength = context->GetInputTensor(0)->GetShapeSize();

        const int64_t *pnum_rows = context->GetAttrs()->GetInt(0);
        const int64_t *pnum_columns = context->GetAttrs()->GetInt(1);
        // auto *pbatch_shape = context->GetAttrs()->GetListInt(0);
        // const int64_t *pdtype = context->GetAttrs()->GetInt(2);
        int32_t num_rows = *pnum_rows;
        int32_t num_columns = *pnum_columns;
        // int32_t dtype = *pdtype;
        int32_t dtype = context->GetInputTensor(0)->GetDataType();
        if (dtype == ge::DT_FLOAT)
        {
            dtype = 0;
        }
        else if (dtype == ge::DT_FLOAT16)
        {
            dtype = 1;
        }
        else if (dtype == ge::DT_INT32)
        {
            dtype = 2;
        }
        else if (dtype == ge::DT_DOUBLE)
        {
            dtype = 3;
        }

        auto shape = context->GetInputTensor(0)->GetOriginShape();
        int32_t shapeSize = shape.GetDimNum() - 2; 

        if (num_columns == 0)
        {
            num_columns = num_rows;
        }

        if (dtype == 1)
        {
            sizeofdatatype = 2;
        }
        else if (dtype == 3)
        {
            sizeofdatatype = 8;
        }
        else
        {
            sizeofdatatype = 4;
        }

        uint32_t coreNum = ascendcPlatform.GetCoreNumAiv();
        uint32_t totalSize = totalLength / (num_rows > num_columns ? num_rows : num_columns);
        uint32_t outSize = totalLength / num_rows / num_columns;
        uint32_t inSize = num_rows < num_columns ? num_rows : num_columns;
        uint32_t outStride = num_rows * num_columns;
        uint32_t inStride = num_columns + 1;
        uint32_t totalSizeAligned = (outSize + coreNum - 1) / coreNum * coreNum;
        uint32_t batchSize = totalSizeAligned / coreNum;
        uint32_t batchNum = coreNum / (totalLength / outStride);
        batchNum = batchNum < inSize ? batchNum : inSize;
        tiling.set_dtype(dtype);
        // printf("batchSize: %d, batchNum: %d\n", batchSize, batchNum);
        // printf("outSize: %d, inSize: %d\n", outSize, inSize);
        // printf("outStride: %d, inStride: %d\n", outStride, inStride);
        // printf("totalSizeAligned: %d\n", totalSizeAligned);

        context->SetBlockDim(coreNum);
        tiling.set_batchSize(batchSize);
        tiling.set_batchNum(batchNum);
        tiling.set_totalSize(totalSize);
        tiling.set_outSize(outSize);
        tiling.set_inSize(inSize);
        tiling.set_outStride(outStride);
        tiling.set_inStride(inStride);
        tiling.SaveToBuffer(context->GetRawTilingData()->GetData(), context->GetRawTilingData()->GetCapacity());
        context->GetRawTilingData()->SetDataSize(tiling.GetDataSize());
        size_t *currentWorkspace = context->GetWorkspaceSizes(1);
        currentWorkspace[0] = 0;
        // printf("op_host success!!!!!!!!!!!!!!!\n");
        return ge::GRAPH_SUCCESS;
    }
}

namespace ge
{
    static ge::graphStatus InferShape(gert::InferShapeContext *context)
    {
        const gert::Shape *x1_shape = context->GetInputShape(0);
        gert::Shape *y_shape = context->GetOutputShape(0);
        *y_shape = *x1_shape;
        return GRAPH_SUCCESS;
    }

    static ge::graphStatus InferDataType(gert::InferDataTypeContext *context)
    {
        context->SetOutputDataType(0, context->GetInputDataType(0));
        return GRAPH_SUCCESS;
    }
}

namespace ops
{
    class EyeCustom : public OpDef {
    public:
        explicit EyeCustom(const char* name) : OpDef(name)
        {
            this->Input("y")
                .ParamType(REQUIRED)
                .DataType({ge::DT_FLOAT, ge::DT_FLOAT16, ge::DT_INT32, ge::DT_DOUBLE})
                .Format({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND})
                .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND});
            this->Output("y")
                .ParamType(REQUIRED)
                .DataType({ge::DT_FLOAT, ge::DT_FLOAT16, ge::DT_INT32, ge::DT_DOUBLE})
                .Format({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND})
                .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND});

            // this->Input("y")
            //     .ParamType(REQUIRED)
            //     .DataType({ge::DT_FLOAT, ge::DT_FLOAT16, ge::DT_INT32})
            //     .Format({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND})
            //     .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND});
            // this->Output("y")
            //     .ParamType(REQUIRED)
            //     .DataType({ge::DT_FLOAT, ge::DT_FLOAT16, ge::DT_INT32})
            //     .Format({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND})
            //     .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND});
            this->Attr("num_rows").Int();
            this->Attr("num_columns").AttrType(OPTIONAL).Int(0);
            this->Attr("batch_shape").AttrType(OPTIONAL).ListInt({});
            this->Attr("dtype").AttrType(OPTIONAL).Int(0);

            this->SetInferShape(ge::InferShape);
            this->SetInferDataType(ge::InferDataType);

            this->AICore()
                .SetTiling(optiling::TilingFunc);
            this->AICore().AddConfig("ascend910b");

        }
    };

    OP_ADD(EyeCustom);
}
