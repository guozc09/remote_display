<!--
 * @Description: 
 * @version: 
 * @Author: Zhc Guo
 * @Date: 2020-01-11 11:14:46
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-06-17 23:11:34
-->

# Remote DisPlay

## 功能说明

- 将本地的视频数据推送给远端。
- 实现一套视频流的解码的通用接口。

## 构建

1. 执行 ./configure --prefix=[path] , "path" 为绝对路径，如果不指定 make install 将会安装到默认路径。
2. make all
3. make install , 如果不 install, 那么只需要执行第2步操作，不需要执行第1，3步操作。

## 使用

***注意： make install 后生成的新的执行档会替换掉 out/ 下的执行档，如果只是进行了 make all 可在当前路径直接执行 ./remote_display***

1. cd out
2. ./remote_display
