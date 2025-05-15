#include "widget.h"
#include "ui_widget.h"
#include<iostream>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QKeyEvent>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //虽然上面这一行代码进行widget和ui的窗口关联，但是如果发生窗口大小变化的时候，里面布局不会随之变化
    //通过下面这行代码进行显示说明 让窗口变化时 布局及其子部件随之调整
    this->setLayout(ui->verticalLayout); //窗口大小可以变化

    //QTextEdit 的 viewport() 返回其内部的视图区域（通常是一个 QWidget 或 QAbstractScrollArea::Viewport 对象），这是实际显示文本内容的区域。
    ui->textEdit->viewport()->installEventFilter(this); //给控件安装过滤器


    // horizontalLayout的垂直布局管理器设置给名为widgetBotton的部件。
    // 这样做的结果是，widgetBotton会按照horizontalLayout的规定来安排其子部件的布局
    ui->widgetBotton->setLayout(ui->horizontalLayout);

    //当编码格式变化时
    connect(ui->comboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&Widget::oncurrentIndexChanged);

    // 创建一个映射表：这个表用于将用户选择的文本（如 "UTF-8", "GBK" 等）映射到 QStringConverter::Encoding 枚举值。
    mp.insert("UTF-8",QStringConverter::Utf8);
    mp.insert("UTF-16",QStringConverter::Utf16);
    mp.insert("UTF-16BE",QStringConverter::Utf16BE);
    mp.insert("UTF-16LE",QStringConverter::Utf16LE);

    // 当 QTextEdit 控件中的光标位置发生变化时，这个信号就会被发射
    connect(ui->textEdit,&QTextEdit::cursorPositionChanged,this,&Widget::onCursorPositionChanged);

    //QShortcut：这个类用于创建键盘快捷方式。它允许你将特定的按键或组合键与某个槽（slot）关联起来，当用户按下这些键时，相应的槽就会被触发
    //QKeySequence：这是一个用来封装快捷键模式的类，可以表示标准的快捷键（比如撤销、重做、剪切、复制、粘贴等），也可以表示自定义的组合键
    //QShortcut：Qt 提供的类，用于创建键盘快捷键。
    //QKeySequence：表示键盘快捷键组合（如 Ctrl+O）。
   QShortcut *shortcutOpen = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")),this);
    QShortcut *shortcutSave = new QShortcut(QKeySequence(tr("Ctrl+S", "File|Save")),this);



    QShortcut *shortcutZoomIn = new QShortcut(QKeySequence(tr("Ctrl+Shift+=", "File|ZoomIn")),this); //字体放大
    QShortcut *sortcutZoomOut=new QShortcut(QKeySequence(tr("Ctrl+Shift+-", "File|ZoomOut")),this); //字体放小

//       当用户键入快捷方式的密钥序列时，发出此信号。
   connect(shortcutOpen,&QShortcut::activated,[=]() //快捷方式打开
            {
        on_btnFileOpen_clicked();
    });
    connect(shortcutSave,&QShortcut::activated,[=]()
            {
       on_btnSave_clicked();
            });

   connect(shortcutZoomIn,&QShortcut::activated,[=]()
            {
        zoomIn();
    });

    connect(sortcutZoomOut,&QShortcut::activated,this,[=]()
           {
       zoomOut();
   });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::zoomIn()
{

        //1.获得TextEdit的当前字体信息
        QFont font= ui->textEdit->font();
        //2.获得当前字体大小
        int fontSize=font.pointSize(); //返回int
        if(fontSize==-1) return;
        //3.改变大小 并设置字体大小
        int newFontSize=fontSize+2;
        font.setPointSize(newFontSize); //设置字体大小
        ui->textEdit->setFont(font); //字体大小设置到textEdit
}


void Widget::zoomOut()
{

    //1.获得TextEdit的当前字体信息
    QFont font= ui->textEdit->font();
    //2.获得当前字体大小
    int fontSize=font.pointSize(); //返回int
    if(fontSize==-1) return;
    //3.改变大小 并设置字体大小
    int newFontSize=fontSize-2;
    font.setPointSize(newFontSize); //设置字体大小
    ui->textEdit->setFont(font); //字体大小设置到textEdit
}
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
   /* QGuiApplication::keyboardModifiers()
    作用：获取当前按下的键盘修饰键（如 Ctrl、Shift、Alt 等）的状态。
    Qt::KeyboardModifiers
        常用值：
         Qt::NoModifier：无修饰键按下。
         Qt::ShiftModifier：Shift 键按下。
          Qt::ControlModifier：Ctrl 键按下。
         Qt::AltModifier：Alt 键按下。
          Qt::MetaModifier：Meta 键按下（Windows 上为 Win 键，Mac 上为 Command 键）。
*/

    if(event->type() ==QEvent::Wheel)
    {
        if(QGuiApplication::keyboardModifiers() == Qt::ControlModifier) //作用：返回当前按下的键盘修饰键（如 Shift、Ctrl、Alt 等）
        {
            qDebug()<<"ctrl+wheel";
            QWheelEvent *wheelEvent=dynamic_cast<QWheelEvent*>(event); //c++强制类型转换
            if(wheelEvent->angleDelta().ry()>0)  //获取滚轮滚动的偏移量。
            {
                zoomIn();
            }
            else if(wheelEvent->angleDelta().ry()<0)
            {
                zoomOut();
            }
            return true;
        }

    }
    return false;
}


//打开文件
void Widget::on_btnFileOpen_clicked()
{

    //打开多个文件 返回值是一个字符串列表，因为可能选择了多个文件
    QStringList fileNames=QFileDialog::getOpenFileNames(this,
                                                          tr("打开文件"),
                                                          tr("D:/qtStudent/QFile"),
                                                          tr("Text (*.txt *.doc)")
    );

    ui->textEdit->clear(); //每次打开新文件前 先把内容清空

    if(!fileNames.empty())
    //遍历多个路径
    for(auto fileName:fileNames)
    {
        file.setFileName(fileName); //设置路径
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug()<<"open error";
            continue;
        }
        this->setWindowTitle(fileName+"-周良根的记事本");//每次打开提示文件路径和标题信息
        QTextStream in(&file); //文本流 读取和写入

        QString str_=ui->comboBox->currentText();// /currentText() 返回的是 QString 类型
        if(mp.contains(str_)) // 判断某个键是否存在于映射中，返回布尔值  也可使用find 返回的是迭代器
        {
            //获取键 "str" 对应的值 QStringConverter::   也可以[] mp[str]
            in.setEncoding(mp.value(str_)); //   然后设置编码
        }
        // in.setEncoding(QStringConverter::Utf8);


        while(!in.atEnd()) //判断是否到末尾 到末尾返回true
        {
            // 返回 true：表示文件指针已经到达文件末尾（即无法再读取数据）。
            //     返回 false：表示文件指针尚未到达文件末尾（可以继续读取数据）。
            QString str=in.readLine(); //一行一行读取
            ui->textEdit->append(str); //将读取到的文件显示
        }

        //file.close();

    }

    else
        qDebug()<<"无选中任何文件";
}

//保存文件
void Widget::on_btnSave_clicked()
{
//   QFileDialog 是 Qt 框架中用于与用户进行文件对话交互的一个类。它允许用户从本地文件系统中选择文件或目录，或者保存文件到指定位置。
    if(!file.isOpen())  //
    {
    QString fileName=QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    tr("D:/qtStudent/QFile"),
                                                    tr("Text (*.txt *.doc)")
        );


    file.setFileName(fileName); //设置文件路径
    if(!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug()<<"打开失败";
    }
    this->setWindowTitle(fileName+"-周良根的记事本");
    //file.close();
    }


        QTextStream out(&file);
        QString str_=ui->comboBox->currentText();
        out.setEncoding(mp.value(str_));
        QString context=ui->textEdit->toPlainText(); //获取ui里的textEdit写的内容
        qDebug()<<context;
        out<<context; //写入

        QMessageBox::information(this,tr("File Save"),tr("保存成功"));


}

//关闭
void Widget::on_btnClose_clicked()
{
    // isEmpty()如果字符串长度为 0（即没有字符），则返回 true；如果有任何字符（包括空白字符，如空格、制表符或换行符），则返回 false。
    if(file.isOpen() || !ui->textEdit->toPlainText().isEmpty()) //textEdit有内容
    {
        int ret=QMessageBox::warning(this,tr("提醒"),tr("你还有文件未保存"),QMessageBox::Save |
                                                                                     QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Save);


        switch(ret)
        {
        case QMessageBox::Save:
        {
            on_btnSave_clicked();//调用自己已完成的保存函数
            break;
        }
        case QMessageBox::Discard: //丢弃
        {
            if(file.isOpen())//打开成功 返回true
            {
                file.close();
            }
            this->setWindowTitle("周良根的记事本"); //重新设置窗口的标题
            ui->textEdit->clear();
            break;
        }
        case QMessageBox::Cancel: //取消
        {
            break;
        }
        }
    }
    else
        ui->textEdit->clear();

}


//换了编码格式后 重新读取textEdit上的内容显示
void Widget::oncurrentIndexChanged(int index)
{
    // cout<<index<<endl; //获取comboBox的索引
    // // /currentText() 返回的是 QString 类型，而 std::cout 无法直接输出 QString。//获取文本信息
    // cout<<ui->comboBox->currentText().toStdString()<<endl;//：将 QString 转换为 std::string 或 const char*

    ui->textEdit->clear(); //先把textEdit的内容清空
    if(file.isOpen())
    {
        QTextStream in(&file);
        QString str=ui->comboBox->currentText();//Qt 中 QComboBox 类的一个常用方法，用于获取当前选中的项的文本内容。
        in.setEncoding(mp.value(str)); //设置编码格式
        in.seek(0);//重新让光标回到开始位置  因为打开读取完之后光标已经在最后位置 当你切换编码格式时光标还在末尾重新读取不了
        while(!in.atEnd())
        {
            QString context=in.readLine();
            ui->textEdit->append(context); //将内容通过追加方式显示到textEdit
        }
    }
}


void Widget::onCursorPositionChanged()
{

    //从一个 QTextEdit 控件获取当前的文本光标（QTextCursor），并将其赋值给一个名为 cursor 的变量。
    //调用 QTextEdit 类的 textCursor() 方法，该方法返回一个 QTextCursor 对象。
    //QTextEdit::textCursor() 是 Qt 框架中 QTextEdit 类的一个方法，其作用是 获取当前文本编辑控件中的文本光标对象（QTextCursor
    //QTextCursor 对象反映了用户在 QTextEdit 中的光标位置
    QTextCursor cursor=ui->textEdit->textCursor();  //光标的位置  返回QTextCuesor类型

    //调用 QTextCursor 对象的 blockNumber() columnNumber() 方法，该方法返回光标当前位置所在文本块的编号
    //qDebug()<<cursor.blockNumber()+1<<","<<cursor.columnNumber()+1; //文本光标的行和列


    // QString::number(...):
    //     将整数转换为 QString 类型，便于在界面上显示或与其他字符串拼接等操作。
    //：使用 QString::number() 函数将数值型的块编号（经过+1处理后的）转换为 QString 类型
    //QString::number() 是一个静态函数，可以接受整数、浮点数等类型的参数，并将其转换为字符串表示形式
    QString blockNum=QString::number(cursor.blockNumber()+1); //返回光标所在块（通常是一个段落）的编号。
    QString columnNum=QString ::number(cursor.columnNumber()+1); //返回光标在其当前块内的列位置。

    const QString labelMes="行号:"+blockNum+ ",列号:"+columnNum+"  ";
    ui->labelPosition->setText(labelMes); //参数为 const QString


    //设置当前行高亮

    // 定义一个 QList 容器，用于存储多个 QTextEdit::ExtraSelection 对象（即“额外选择”）。
    //     ExtraSelection 是 Qt 提供的一个结构体，用于在 QTextEdit 中添加额外的文本高亮或样式（例如当前行高亮）。
     QList<QTextEdit::ExtraSelection> extraSelection;
    QTextEdit::ExtraSelection ext; //创建一个 ExtraSelection 对象 ext，用于配置高亮的样式和位置

    //1.知道当前行
    ext.cursor=cursor;

     QBrush qBrush(Qt::lightGray); //创建一个浅灰色的画刷（QBrush），用于设置高亮的背景颜色。
    //2.颜色
    ext.format.setBackground(qBrush); //将 ext 的文本格式（format）的背景颜色设置为浅灰色。这样，高亮区域的背景会变成浅灰色。

     //配置属性 整行显示 没有这行不行
    ext.format.setProperty(QTextFormat::FullWidthSelection,true);//设置一个属性，确保高亮区域覆盖整行（而不是仅覆盖光标所在的位置）。

    ext.format.setFontUnderline(true);//当前行下划线

    //3.设置
    extraSelection.push_back(ext);
    ui->textEdit->setExtraSelections(extraSelection);//将 extraSelection 列表应用到 QTextEdit 控件中，从而实现当前行的高亮效果。
}

