#ifndef MYGLVIEW_H
#define MYGLVIEW_H

#include "layoutobject.h"
#include <QListWidget>
#include <QMainWindow>
#include <QWidget>



QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;


class MyState;
class LayoutData;

class MyGLView: public MyLayoutObject, public QWidget
{
public:
    MyGLView();
public:
    MyGLView(QMainWindow * parent, QStringList & sl, LayoutData *ld, int init_state);
    ~MyGLView();
    void drawLayoutObject(QPainter& painter, QPoint &loffset);
    void selectLayoutObject();
    void setLayoutObjectStatus();
    void getLayoutObjectStatus();
    void setLayoutObjectPropertiy();
    void getLayoutObjectPropertiy();
    bool isCursorOver(QPoint &pos);
    bool isFileNameChanged(QString new_file_name);
    bool setState(int state);
    MyState* getState(LayoutData* ld, QString state_name);
    QString get_active_state_command();
    void removeQuotes( QString & str);

private:
    QSlider* createSlider();

    QString  g_code_file;
    QStringList elements;   // this list contains the states button can have.
    int current_state;      // if the more than one state possible this index keep it.
    LayoutData * ld_local;  // save the pointer to the data while initializing;
    QListWidget * g_list;
    QMainWindow * parent;
    QString file_name;
    QString old_file_name;
    QStringList program;
    int line_pos; // the line of g-code to be executed.

     GLWidget *glWidget;
     QSlider *xSlider;
     QSlider *ySlider;
     QSlider *zSlider;
     QPushButton *dockBtn;
     MainWindow *mainWindow;

};

#endif // MYGLVIEW_H
