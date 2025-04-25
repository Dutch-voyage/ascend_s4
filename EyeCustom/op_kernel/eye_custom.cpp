#include "kernel_operator.h"

using namespace AscendC;
constexpr int32_t BUFFER_NUM = 2;

#define Buf_DTYPE float
#ifdef K_MAX_SHAPE_DIM
#undef K_MAX_SHAPE_DIM
#endif
#define K_MAX_SHAPE_DIM 0

class KernelEyeCustomInSplit
{
public:
    __aicore__ inline KernelEyeCustomInSplit() {}
    __aicore__ inline void Init(GM_ADDR y, GM_ADDR y_ref, uint32_t dtype,
                                uint32_t batchNum, uint32_t batchSize, uint32_t totalSize, uint32_t outSize, uint32_t inSize, 
                                uint32_t outStride, uint32_t inStride, TPipe* pipeIn)
    {
        pipe = pipeIn;
        uint32_t outIdx = GetBlockIdx() / batchNum;
        uint32_t splitInSize = (inSize + batchNum - 1) / batchNum;
        uint32_t inIdx = GetBlockIdx() % batchNum;
        uint32_t highInIdx = (inIdx + 1) * splitInSize < inSize ? (inIdx + 1) * splitInSize: inSize;
        inIdx = inIdx * splitInSize;
        uint32_t baseAddr = outIdx * outStride + inIdx * inStride;  
        // printf("outIdx: %d, outSize: %d, inIdx: %d, inSize: %d\n", outIdx, outSize, inIdx, inSize);
        if (outIdx >= outSize || inIdx >= inSize) {
            return;
        }
        // uint32_t offset = highInIdx == inSize ? outStride - inIdx * inStride : (highInIdx - inIdx) * inStride;
        // printf("baseAddr: %d, inIdx: %d, splitInSize: %d\n", baseAddr, inIdx, splitInSize);
        // printf("inIdx: %d, highInIdx: %d\n", inIdx, highInIdx); 
        yGm.SetGlobalBuffer((__gm__ DTYPE_Y *)y_ref + baseAddr, (highInIdx - inIdx) * inStride);
        
        pipe->InitBuffer(outQue, BUFFER_NUM, (highInIdx - inIdx) * 32);
        auto ones = outQue.AllocTensor<DTYPE_Y>();
        // Duplicate(ones, DTYPE_Y(1), highInIdx - inIdx);
        #pragma omp parallel for
        uint32_t dStride = 32 / sizeof(DTYPE_Y);
        for (int32_t i = 0; i < highInIdx - inIdx; i++) {
            ones(i * dStride) = 1;
        }   

        DataCopyExtParams copyParams = {(uint16_t)(highInIdx - inIdx), (uint32_t)sizeof(DTYPE_Y), 0, (uint32_t)((inStride - 1) * sizeof(DTYPE_Y)), 0};
        DataCopyPad(yGm, ones, copyParams);
        outQue.FreeTensor(ones);    
    }

    __aicore__ inline void Process()
    {
        
    }

private:
    TPipe* pipe;
    TQue<QuePosition::VECOUT, BUFFER_NUM> outQue;
    GlobalTensor<DTYPE_Y> yGm;
    int32_t outSize;
    int32_t inSize;
    int32_t sizeCacheLine;
    int32_t outStride;
    int32_t inStride;

};
class KernelEyeCustomOutSplit
{
public:
    __aicore__ inline KernelEyeCustomOutSplit() {}
    __aicore__ inline void Init(GM_ADDR y, GM_ADDR y_ref, uint32_t dtype,
                                uint32_t batchNum, uint32_t batchSize, uint32_t totalSize, uint32_t outSize, uint32_t inSize, 
                                uint32_t outStride, uint32_t inStride, TPipe* pipeIn)
    {
        pipe = pipeIn;
        uint32_t splitOutSize = batchSize;
        uint32_t outIdx = GetBlockIdx() * splitOutSize;
        uint32_t inIdx = 0;
        uint32_t baseAddr = outIdx * outStride + inIdx * inStride * splitOutSize;
        if (outIdx >= outSize) {
            return;
        }
        uint32_t highOutIdx = outIdx + splitOutSize < outSize ? outIdx + splitOutSize : outSize;
        yGm.SetGlobalBuffer((__gm__ DTYPE_Y *)y_ref + baseAddr, (highOutIdx - outIdx) * outStride * sizeof(DTYPE_Y));
        pipe->InitBuffer(outQue, BUFFER_NUM, inSize * 32);
        auto ones = outQue.AllocTensor<DTYPE_Y>();
        // Duplicate(ones, DTYPE_Y(1), highOutIdx - outIdx);
        #pragma omp parallel for
        uint32_t dStride = 32 / sizeof(DTYPE_Y);
        for (int32_t i = 0; i < inSize; i++) {
            ones(i * dStride) = 1;
        }
        #pragma omp parallel for
        for (int32_t i = 0; i < highOutIdx - outIdx; i++) {
            DataCopyExtParams copyParams = {(uint16_t)inSize, (uint32_t)sizeof(DTYPE_Y), 0, (uint32_t)((inStride - 1) * sizeof(DTYPE_Y)), 0};
            DataCopyPad(yGm[i * outStride], ones, copyParams);
        }
        outQue.FreeTensor(ones);    
    }

    __aicore__ inline void Process()
    {

    }

private:
    TPipe* pipe;
    TQue<QuePosition::VECOUT, BUFFER_NUM> outQue;
    GlobalTensor<DTYPE_Y> yGm;
    int32_t outSize;
    int32_t inSize;
    int32_t sizeCacheLine;
    int32_t outStride;
    int32_t inStride;
};

extern "C" __global__ __aicore__ void eye_custom(GM_ADDR y, GM_ADDR y_ref, GM_ADDR workspace, GM_ADDR tiling)
{
    GET_TILING_DATA(tiling_data, tiling);
    TPipe pipe;
    if (tiling_data.batchNum != 0) {
        KernelEyeCustomInSplit op;
        op.Init(y, y_ref, tiling_data.dtype,
            tiling_data.batchNum, tiling_data.batchSize, tiling_data.totalSize, tiling_data.outSize, tiling_data.inSize,
            tiling_data.outStride, tiling_data.inStride, &pipe);
        op.Process();
    } else {
        KernelEyeCustomOutSplit op;
        op.Init(y, y_ref, tiling_data.dtype,
            tiling_data.batchNum, tiling_data.batchSize, tiling_data.totalSize, tiling_data.outSize, tiling_data.inSize,
            tiling_data.outStride, tiling_data.inStride, &pipe);
        op.Process();   
    }
    
}