#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "layoutdata.h"

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                      Chord, Pie, Path, Text, Pixmap };
    
public:
    explicit MainWindow(QWidget *parent = 0);
    QSize get_screen_size();
    void set_window_size(QSize s, QPoint pt);
    void set_ggeom(int w, int h);
    void set_layout_params(QPoint p, int uc);
    int load_config(QString fname);
    void drawMesh();
    ~MainWindow();
protected:
    // parser functions
    bool parse_list(QStringList& list, LayoutData* ld);
    bool is_word(QString& s);
    bool is_string(QString& s);
    bool is_number(QString& s);
    bool is_var(QString& s);
    bool is_ml_comment_start(QString& s);
    bool is_ml_comment_stop(QString& s);
    bool is_sl_comment(QString& s);
    bool is_unknown(QString& s);
    bool exec_word(QStringList& list, int& cs, LayoutData *ld);


protected:
    void paintEvent(QPaintEvent * e);
    
private:
    Ui::MainWindow *ui;

    // Screen size
    QSize screen_size;

    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;
    QPixmap pixmap;

    // grid geometry
    QSize ggeom;
    int ucell;      // The unit cell size in pixels
    QPoint loffset; // The left top corner of the layout
    // ini file
    QString ini_file_str;
    LayoutData* ld; // All screen layouts for this application
    bool multiline_comment_on;

};

#endif // MAINWINDOW_H
