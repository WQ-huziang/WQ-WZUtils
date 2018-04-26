# WQ-DataEngine

> Author : 胡子昂，李嘉恒，罗俊斌

## v1.0

- 确定map\<string, string\>和vector\<map\<string, string\>\>作为函数的接口。
- 设计了DataEngine基类和MongodbEngine子类
- 确定了基类含有insert，update，find三个虚函数

## v1.1

- 设计了新的数据结构KeyValue作为查询条件
- 将基类的三个虚函数增加到8个，添加delete操作，每个操作分为one和many，比如insert_one，insert_many

## v1.2

- 添加了结构体和map之间的转换
- MongodbEngine开发完毕，修改了编译错误

## v1.3

- 添加了init函数，用于初始化
- 添加了set_index函数，用于添加索引
- 修复了巨量bug

## v1.4

- 修复了少量bug
- 添加了gtest，对程序进行测试

## v1.5

- 修改了KeyValue的结构，使其可以进行半值范围查询
- 添加说明文档
