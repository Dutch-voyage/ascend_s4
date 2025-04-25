#!/bin/bash
echo "[ascend910b] Generating EyeCustom_fc8cbc9942632392eefdd86e355c4c51 ..."
export ASCEND_GLOBAL_LOG_LEVEL=3
export ASCEND_SLOG_PRINT_TO_STDOUT=1

while true; do
  case "$1" in
    --kernel-src=*)
      export BUILD_KERNEL_SRC=$(echo "$1" | cut -d"=" -f2-)
      shift
      ;;
    -*)
      shift
      ;;
    *)
      break
      ;;
  esac
done
res=$(opc $1 --main_func=eye_custom --input_param=/home/yyx/ascend/competition/EyeCustom/build_out/op_kernel/binary/ascend910b/gen/EyeCustom_fc8cbc9942632392eefdd86e355c4c51_param.json --soc_version=Ascend910B1                 --output=$2 --impl_mode=high_performance,optional --simplified_key_mode=0 --op_mode=dynamic )

echo "${res}"

if ! test -f $2/EyeCustom_fc8cbc9942632392eefdd86e355c4c51.json ; then
  echo "$2/EyeCustom_fc8cbc9942632392eefdd86e355c4c51.json not generated!"
  exit 1
fi

if ! test -f $2/EyeCustom_fc8cbc9942632392eefdd86e355c4c51.o ; then
  echo "$2/EyeCustom_fc8cbc9942632392eefdd86e355c4c51.o not generated!"
  exit 1
fi
echo "[ascend910b] Generating EyeCustom_fc8cbc9942632392eefdd86e355c4c51 Done"
