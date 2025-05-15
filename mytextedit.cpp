// #include "mytextedit.h"
// #include<QDebug>

// MyTextEdit::MyTextEdit(QWidget *parent):QTextEdit(parent)
// {

// }

// void MyTextEdit::wheelEvent(QWheelEvent *e)
// {
//     qDebug()<<e->angleDelta();
//     int ret=e->angleDelta().ry();
//     if(flage==true)
//     {
//         if(ret>0)
//         {
//             zoomIn();
//         }
//         else if(ret<0)
//         {
//             zoomOut();
//         }
//         e->accept();
//     }
//     else
//     {
//         QTextEdit::wheelEvent(e);  //没按ctrl键时，交给原来的处理方式
//     }
// }

// void MyTextEdit::keyPressEvent(QKeyEvent *e)
// {
//     if(e->key()==Qt::Key_Control)
//     {
//         qDebug()<<"ctrl pressed";
//         flage=true;
//     }
//     QTextEdit::keyPressEvent(e);//如果按的不是ctrl 原先该怎么弄得就怎么弄
// }

// void MyTextEdit::keyReleaseEvent(QKeyEvent *e)
// {
//     if(e->key()==Qt::Key_Control)
//     {
//         qDebug()<<"ctrl release";
//         flage=false;
//     }
//     QTextEdit::keyPressEvent(e);//如果按的不是ctrl 原先该怎么弄得就怎么弄
// }
