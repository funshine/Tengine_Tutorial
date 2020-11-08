# Tengine 人脸检测RPI4版本

## 环境
**System**: Linux raspberrypi 5.4.72-v8+ aarch64

**CMAKE**: 3.13.4

## 准备

1. 按教程编译好的so和头文件复制到项目目录中的tengine目录，RPI4本地编译或者交叉编译，都会遇到neon问题：
使用：
```
cmake -DTENGINE_TOOLCHIN_FLAG=-flax-vector-conversions ..
```
