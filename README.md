### Eye
#### 1 安装环境
https://www.hiascend.com/developer/download/community/result?module=cann&cann=8.0.0.alpha003
| list                                                     |
| -------------------------------------------------------- |
| Ascend-cann-toolkit_8.0.0.alpha003_linux-x86_64.run      |
| Ascend-cann-kernels-910b_8.0.0.alpha003_linux-x86_64.run |
#### 2 （可选）生成代码
https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/800alpha003/devaids/opdev/optool/atlasopdev_16_0021.html

可以根据链接文档中的json自定义代码输入输出格式
```
msopgen -i KernelName.json -lan cpp -c ai_core-Ascend910B -out output_dir
```
#### 3 编译
```
./build.sh
./build_out/custom_opp_ubuntu_x86_64.run
```
#### 4 测试
设置环境变量（将build_out/op_host加入LD_LIBRARY_PATH中）
```
export LD_LIBRARY_PATH=.../EyeCustom/build_out/op_host:$LD_LIBRARY_PATH
```

```
./AclNNInvocation/run.sh
```
主要更改scripts/gen_data.py和src/main.cpp（输入形状和数据格式）

Eye算子有现成的测试文件，别的算子可能要自己改main.cpp中的输入。

在run.sh中执行
```
msprof op simulator --soc-version=Ascend910B4 ./execute_op
```
指定simulator的模拟器型号（本次比赛为Ascend910B4）
