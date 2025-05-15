# NoteBook
文本编辑器开发（Qt框架）

本项目基于Qt6构建多编码支持的文本编辑解决方案，核心实现包括：

1. 核心功能架构
文件管理系统
完整实现文件打开/保存/关闭操作（QFile/QFileDialog）
支持UTF-8/UTF-16(LE/BE)编码格式手动切换（QComboBox联动）
交互增强模块
快捷键系统（Ctrl+S/Ctrl+O等标准绑定）
Ctrl+滚轮动态字体缩放（QFont::setPointSizeF）
实时坐标追踪（QLabel+事件过滤器实现光标位置显示）

2.技术实现亮点
编码转换引擎
使用QTextStream::setCodec实现多编码转换
BOM头检测逻辑（QTextStream::readLine()特征分析）
手动编码选择机制（QComboBox选项绑定QTextStream编码参数）

项目价值
通过Qt框架的深度应用，实现了具备多编码支持、高效文本处理能力的轻量级编辑器，
