#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include "layoutdata.h"
#include "path.h"
#include "state.h"
#include "simplelayout.h"
#include "mybutton.h"

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
    int get_int_value_from_data(QString var);
    ~MainWindow();
protected:
    // parser functions
    bool parse_list(QStringList& list, LayoutData* ld);
    //bool parse_raw_string(QString &str, LayoutData* ld);
    bool is_word(QString& s);
    bool is_type(QString& s);
    bool is_string(QString& s);
    bool is_number(QString& s);
    bool is_number_pair(QString& s);
    bool is_var(QString& s);
    bool is_ml_comment_start(QString& s);
    bool is_ml_comment_stop(QString& s);
    bool is_sl_comment(QString& s);
    bool is_single_quote(QString& s);
    bool is_unknown(QString& s);
    bool exec_word(QStringList& list, int& cs, LayoutData *ld);
    int  get_int_value(QStringList& list, int& cs, LayoutData *dl);
    float get_float_value(QStringList& list, int& cs, LayoutData *dl);
    QString get_stri_value(QStringList& list, int& cs, LayoutData *dl);
    QString get_next_quoted_token(QString& str);
    Path2D * get_path_value(QStringList& list, int& cs, LayoutData *dl);
    State * get_state_value(QStringList& list, int& cs, LayoutData *dl);
    MyButton* get_button_value(QStringList& list, int& cs, LayoutData *dl);
    SimpleLayout* get_slayout_value(QStringList& list, int& cs, LayoutData *dl);

    bool exec_IVAR(QStringList& list, int& cs, LayoutData *dl);
    bool exec_PATH(QStringList& list, int& cs, LayoutData *dl); // a set of numbers
    bool exec_IVEC2(QStringList& list, int& cs, LayoutData *dl);
    bool exec_STRI(QStringList& list, int& cs, LayoutData *dl);
    bool exec_STATE(QStringList& list, int& cs, LayoutData *dl);
    bool exec_BUTTON(QStringList& list, int& cs, LayoutData *dl);
    bool exec_LAYOUT(QStringList& list, int& cs, LayoutData *dl);


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
    bool is_updating;

};

#endif // MAINWINDOW_H
