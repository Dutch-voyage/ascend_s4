#!/bin/bash
echo "[ascend910b] Generating EyeCustom_b41bf8c462119f9488d2db91247c5b20 ..."
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
res=$(opc $1 --main_func=eye_custom --input_param=/home/yyx/ascend/competition/EyeCustom/build_out/op_kernel/binary/ascend910b/gen/EyeCustom_b41bf8c462119f9488d2db91247c5b20_param.json --soc_version=Ascend910B1                 --output=$2 --impl_mode=high_performance,optional --simplified_key_mode=0 --op_mode=dynamic )

echo "${res}"

if ! test -f $2/EyeCustom_b41bf8c462119f9488d2db91247c5b20.json ; then
  echo "$2/EyeCustom_b41bf8c462119f9488d2db91247c5b20.json not generated!"
  exit 1
fi

if ! test -f $2/EyeCustom_b41bf8c462119f9488d2db91247c5b20.o ; then
  echo "$2/EyeCustom_b41bf8c462119f9488d2db91247c5b20.o not generated!"
  exit 1
fi
echo "[ascend910b] Generating EyeCustom_b41bf8c462119f9488d2db91247c5b20 Done"
