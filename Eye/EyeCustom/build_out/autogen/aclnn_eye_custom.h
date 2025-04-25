
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_EYE_CUSTOM_H_
#define ACLNN_EYE_CUSTOM_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnEyeCustomGetWorkspaceSize
 * parameters :
 * yRef : required
 * numRows : required
 * numColumns : optional
 * batchShapeOptional : optional
 * dtype : optional
 * yRef : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnEyeCustomGetWorkspaceSize(
    aclTensor *yRef,
    int64_t numRows,
    int64_t numColumns,
    const aclIntArray *batchShapeOptional,
    int64_t dtype,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnEyeCustom
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnEyeCustom(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
