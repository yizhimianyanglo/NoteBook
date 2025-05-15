#ifndef WIDGET_H //防止文件重命名
#define WIDGET_H

#include <QFile>
#include <QWidget>
#include<QMap>
using namespace Qt;

QT_BEGIN_NAMESPACE  //确保其库中的类不会与其他库中的同名类名和函数冲突
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT //当你在Qt中定义一个类时，如果这个类继承自QObject 或 其子类时,并且你想使用Qt的信号和槽机制、国际化、属性系统或其他Qt元对象系统提供的功能
        // 就必须在类中包含 Q_OBJECT 宏

public:
    Widget(QWidget *parent = nullptr);
    QFile file;
    ~Widget();

    void zoomIn();
    void zoomOut();

// QObject::eventFilter 是一个虚函数，允许你为 Qt 的事件系统实现自定义的事件过滤器。通过重写这个函数，你可以拦截并处理发送到其他对象的事件
    bool eventFilter(QObject *watched,QEvent *event) override; //声明事件过滤器

    //QObject *watched：这个参数指向的是被监视的对象，即安装了当前事件过滤器的那个对象
    //QEvent *event：这是一个指向 QEvent 对象的指针，表示发生的事件


private slots:
    void on_btnFileOpen_clicked();

    void on_btnSave_clicked();

    void on_btnClose_clicked();

public slots:
    void oncurrentIndexChanged(int index); //换了编码格式

    void onCursorPositionChanged(); //

private:
    Ui::Widget *ui;
    QMap<QString,QStringConverter::Encoding>mp;
};
#endif // WIDGET_H
